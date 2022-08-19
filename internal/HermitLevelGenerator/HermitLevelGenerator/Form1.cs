using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using System.Diagnostics;
using System.Security.Cryptography;


namespace HermitLevelGenerator
{
    public partial class Form1 : Form
    {
        class TWorkerArg
        {
            public int xItem;
            public int yItem;
            public bool iDiagonalMoves;
            public bool iMirrorVertical;
            public bool iMirrorHorizontal;
            public bool iMirrorCenter;
        };

        class CField : CheckBox
        {
            public bool iNone;
            public bool iEmpty;
            public bool iItem;
            public bool iDone;

            public void SetItem()
            {
                iNone = false;
                iEmpty = false;
                iItem = true;
            }
        };


        CField[,] iFields;
        int iMaxDeep;
        int iEmptyFields;
        bool iPrintBoard;
        bool iHashCheck;
        bool iUseMD5;
        String iResult;
        Stopwatch iTimer;

        //Hashtable iHashLevels;
        ArrayList iHashLevels;
        Crc16 iCrc16;
        Crc32 iCrc32;
        MD5 iMD5;
        int iGeneratedMoves;
        int iGeneratedBoards;
        int iGeneratedDeep;
        int iGeneratedCurX;
        int iGeneratedCurY;



        public Form1()
        {
            InitializeComponent();

            iCrc16 = new Crc16();
            iCrc32 = new Crc32();
            iResult = "";
            iTimer = new Stopwatch();

            iMD5 = System.Security.Cryptography.MD5.Create();


            /*byte[] tmp = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 };
            Console.WriteLine("chk0: {0}", iCrc16.ComputeChecksum(tmp));
            byte[] tmp1 = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
            Console.WriteLine("chk1: {0}", iCrc16.ComputeChecksum(tmp1));*/
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (thrGenerator.IsBusy)
            {
                MessageBox.Show("Generator is busy! Stop to proceed.");
                return;
            }

            int m = 5;
            int w = 50;
            int h = w;
            int s = 5;

            iFields = new CField[(int)nFieldsY.Value, (int)nFieldsX.Value];
            
            pBoard.Controls.Clear();

            for (int y = 0; y < nFieldsY.Value; y++)
                for (int x = 0; x < nFieldsX.Value; x++)
                {
                    CField cb = new CField();
                    cb.Appearance = Appearance.Button;
                    cb.Left = m + x * w + x * s + s;
                    cb.Top = m + y * h + y * s + s;
                    cb.Width = w;
                    cb.Height = h;
                    cb.Checked = true;
                    cb.Text = "";
                    cb.MouseUp += new MouseEventHandler(cb_MouseUp);
                    cb.MouseClick += new MouseEventHandler(cb_MouseClick);
                    cb.iEmpty = true;
                    cb.iNone = false;
                    cb.iItem = false;
                    cb.iDone = false;
                    iFields[y, x] = cb;

                    pBoard.Controls.Add(cb);
                }
        }

        void cb_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                CField cb = (CField)sender;
                if (cb.Checked)
                {
                    if (cb.BackColor == Color.Black)
                    {
                        cb.BackColor = pBoard.BackColor;
                        cb.iItem = false;
                        cb.iEmpty = true;
                        cb.iNone = false;
                    }
                    else
                    {
                        cb.BackColor = Color.Black;
                        cb.iItem = true;
                        cb.iEmpty = false;
                        cb.iNone = false;
                    }
                }
            }
        }

        void cb_MouseClick(object sender, MouseEventArgs e)
        {
            CField cb = (CField)sender;
            cb.BackColor = pBoard.BackColor;
            if (cb.Checked)
            {
                cb.iNone = false;
                cb.iEmpty = true;
                cb.iItem = false;
            }
            else
            {
                cb.iNone = true;
                cb.iEmpty = false;
                cb.iItem = false;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (iFields == null || pBoard.Controls.Count == 0)
            {
                MessageBox.Show("Set fields first.");
                return;
            }

            // 1. find iItem 
            int xItem = -1;
            int yItem = -1;
            int itemCnt = 0;
            for (int j = 0; j < nFieldsY.Value; j++)
            {
                for (int i = 0; i < nFieldsX.Value; i++)
                {
                    if (iFields[j, i].iItem)
                    {
                        xItem = i;
                        yItem = j;
                        itemCnt++;
                        if ( itemCnt > 1 ) break;
                    }
                }
                if (itemCnt > 1) break;
            }

            if (xItem == -1 && yItem == -1)
            {
                //MessageBox.Show("Item not found! (select by right mouse button)");
                //return;
                Console.WriteLine("Item not found");
                statusLabel1.Text = "Item not found";
            }
            else if (itemCnt == 1)
            {
                Console.WriteLine("Item found ( {0}, {1} )", xItem, yItem);
                statusLabel1.Text = "Item found ( " + xItem + ", " + yItem + " )";
            }
            else
            {
                Console.WriteLine("Items found: {0}", itemCnt);
                statusLabel1.Text = "Items found: " + itemCnt;
            }


            //iHashLevels = new Hashtable();
            iHashLevels = new ArrayList();

            TWorkerArg arg = new TWorkerArg();
            arg.xItem = xItem;
            arg.yItem = yItem;
            arg.iDiagonalMoves = cbDiagonalMoves.Checked;
            arg.iMirrorVertical = cbMirrorVertical.Checked;
            arg.iMirrorHorizontal = cbMirrorHorizontal.Checked;
            arg.iMirrorCenter = cbMirrorCenter.Checked;

            iMaxDeep = (int)nDeep.Value;
            iPrintBoard = cbPrint.Checked;
            iEmptyFields = (int)nEmptyFields.Value;
            iHashCheck = cbHashCheck.Checked;
            iUseMD5 = cbMD5CRC.Checked;

            bGenerate.Enabled = false;
            bStop.Enabled = true;
            iResult = "";
            tbResult.Text = "";
            statusLabelGenerated.Text = "";

            iGeneratedMoves = 0;
            iGeneratedBoards = 0;
            iGeneratedDeep = 0;
            iGeneratedCurX = 0;
            iGeneratedCurY = 0;

            iTimer.Reset();
            iTimer.Start();

            // 2. run algorithm
            thrGenerator.RunWorkerAsync( arg );
        }

        CField[,] CreateBoard(ref CField[,] level, int wx, int wy)
        {
            CField[,] newLevel = new CField[wy, wx];
            for (int j = 0; j < wy; j++)
            {
                for (int i = 0; i < wx; i++)
                {
                    newLevel[j, i] = new CField();
                    newLevel[j, i].iDone = level[j, i].iDone;
                    newLevel[j, i].iEmpty = level[j, i].iEmpty;
                    newLevel[j, i].iItem = level[j, i].iItem;
                    newLevel[j, i].iNone = level[j, i].iNone;
                }
            }
            return newLevel;
        }

        delegate void ShowProgressDelegate(String text);
        void ShowProgress(String text)
        {
            if (this.InvokeRequired == false)
            {
                statusLabelGenerated.Text = text;
            }
            else
            {
                ShowProgressDelegate showProgress = new ShowProgressDelegate(ShowProgress);
                this.Invoke(showProgress, new object[] { text });
            }
        }

        void UpdateProgress(int wx, int wy)
        {
            //int percent = 100 * ( iGeneratedCurY * wx + iGeneratedCurX + 1 ) / (wx * wy);
            String str = "   m[" + iGeneratedMoves + "] b[" + iGeneratedBoards + "] d[" + iGeneratedDeep + "] xy[" + iGeneratedCurX + "," + iGeneratedCurY + "] sec[" + iTimer.ElapsedMilliseconds / 1000 + "]";
            ShowProgress(str);
        }

        void PrintBoard(ref CField[,] level, int wx, int wy)
        {
            for (int j = 0; j < wy; j++)
            {
                for (int i = 0; i < wx; i++)
                {
                    if (level[j, i].iNone)
                    {
                        Console.Write(" ");
                    }
                    else if (level[j, i].iEmpty)
                    {
                        Console.Write("o");
                    }
                    else if (level[j, i].iItem && !level[j, i].iDone)
                    {
                        Console.Write("x");
                    }
                    else if (level[j, i].iItem && level[j, i].iDone)
                    {
                        Console.Write("X");
                    }
                }
                Console.WriteLine();
            }
            Console.WriteLine();
        }

        void PrintBoardOnList(ref CField[,] level, int wx, int wy)
        {
            for (int j = 0; j < wy; j++)
            {
                for (int i = 0; i < wx; i++)
                {
                    if (level[j, i].iNone)
                    {
                        iResult += " ";
                    }
                    else if (level[j, i].iEmpty)
                    {
                        iResult += "o";
                    }
                    else if (level[j, i].iItem && !level[j, i].iDone)
                    {
                        iResult += "x";
                    }
                    else if (level[j, i].iItem && level[j, i].iDone)
                    {
                        iResult += "X";
                    }
                }
                iResult += Environment.NewLine;
            }
            iResult += Environment.NewLine;
        }

        bool CheckGameWin(ref CField[,] level, int wx, int wy, int x, int y)
        {
            int emptyCnt = 0;
            for (int j = 0; j < wy; j++)
            {
                for (int i = 0; i < wx; i++)
                {
                    if (level[j, i].iNone)
                    {
                        continue;
                    }
                    if (level[j, i].iEmpty)
                    {
                        if (++emptyCnt > iEmptyFields)
                        {
                            return false;
                        }
                    }
                }
            }
            return true;
        }

        int GenerateMove(ref CField[,] fields, int x, int y, int wx, int wy, bool dg, int move_cnt)
        {
            if (/*!fields[y, x].iDone && */fields[y, x].iItem)
            {
                if ( move_cnt == -1 && x > 1 && fields[y, x - 2].iEmpty && fields[y, x - 1].iEmpty)
                {
                    //fields[y, x].iDone = true;
                    fields[y, x].iItem = false;
                    fields[y, x].iEmpty = true;
                    fields[y, x - 2].SetItem();
                    fields[y, x - 1].SetItem();
                    return 0;
                }
                else if (move_cnt <= 0 && x < wx - 2 && fields[y, x + 2].iEmpty && fields[y, x + 1].iEmpty)
                {
                    //fields[y, x].iDone = true;
                    fields[y, x].iItem = false;
                    fields[y, x].iEmpty = true; 
                    fields[y, x + 2].SetItem();
                    fields[y, x + 1].SetItem();
                    return 1;
                }
                else if (move_cnt <= 1 && y > 1 && fields[y - 2, x].iEmpty && fields[y - 1, x].iEmpty)
                {
                    //fields[y, x].iDone = true;
                    fields[y, x].iItem = false;
                    fields[y, x].iEmpty = true;
                    fields[y - 2, x].SetItem();
                    fields[y - 1, x].SetItem();
                    return 2;
                }
                else if (move_cnt <= 2 && y < wy - 2 && fields[y + 2, x].iEmpty && fields[y + 1, x].iEmpty)
                {
                    //fields[y, x].iDone = true;
                    fields[y, x].iItem = false;
                    fields[y, x].iEmpty = true;
                    fields[y + 2, x].SetItem();
                    fields[y + 1, x].SetItem();
                    return 3;
                }
                else if (dg)
                {
                    if (move_cnt <= 3 && x < wx - 2 && y < wy - 2 && fields[y + 2, x + 2].iEmpty && fields[y + 1, x + 1].iEmpty)
                    {
                        fields[y, x].iItem = false;
                        fields[y, x].iEmpty = true;
                        fields[y + 2, x + 2].SetItem();
                        fields[y + 1, x + 1].SetItem();
                        return 4;
                    }
                    else if (move_cnt <= 4 && x > 1 && y < wy - 2 && fields[y + 2, x - 2].iEmpty && fields[y + 1, x - 1].iEmpty)
                    {
                        fields[y, x].iItem = false;
                        fields[y, x].iEmpty = true;
                        fields[y + 2, x - 2].SetItem();
                        fields[y + 1, x - 1].SetItem();
                        return 5;
                    }
                    else if (move_cnt <= 5 && x > 1 && y > 1 && fields[y - 2, x - 2].iEmpty && fields[y - 1, x - 1].iEmpty)
                    {
                        fields[y, x].iItem = false;
                        fields[y, x].iEmpty = true;
                        fields[y - 2, x - 2].SetItem();
                        fields[y - 1, x - 1].SetItem();
                        return 6;
                    }
                    else if (move_cnt <= 6 && x < wx - 2 && y > 1 && fields[y - 2, x + 2].iEmpty && fields[y - 1, x + 1].iEmpty)
                    {
                        fields[y, x].iItem = false;
                        fields[y, x].iEmpty = true;
                        fields[y - 2, x + 2].SetItem();
                        fields[y - 1, x + 1].SetItem();
                        return 7;
                    }
                }
            }
            return -1;
        }

        uint CraeteHash(ref CField[,] level, int wx, int wy, bool mv, bool mh)
        {
            byte[] tab = new byte[wy * wx];
            int k = 0;

            if (!mv && !mh)
            {
                for (int j = 0; j < wy; j++)
                {
                    for (int i = 0; i < wx; i++)
                    {
                        if (level[j, i].iItem) tab[k++] = 1;
                        else tab[k++] = 0;
                    }
                }
            }
            else if (mv && !mh)
            {
                for (int j = 0; j < wy; j++)
                {
                    for (int i = 0; i < wx; i++)
                    {
                        if (level[wy - j - 1, i].iItem) tab[k++] = 1;
                        else tab[k++] = 0;
                    }
                }
            }
            else if (!mv && mh)
            {
                for (int j = 0; j < wy; j++)
                {
                    for (int i = 0; i < wx; i++)
                    {
                        if (level[j, wx - i - 1].iItem) tab[k++] = 1;
                        else tab[k++] = 0;
                    }
                }
            }
            else if (mv && mh)
            {
                for (int j = 0; j < wy; j++)
                {
                    for (int i = 0; i < wx; i++)
                    {
                        if (level[wy - j - 1, wx - i - 1].iItem) tab[k++] = 1;
                        else tab[k++] = 0;
                    }
                }
            }

            if (iUseMD5)
            {
                return iCrc32.ComputeChecksum(iMD5.ComputeHash(tab));
            }
            else
            {
                return iCrc32.ComputeChecksum(tab);
            }
        }

        int GenerateNewBoardStep(ref CField[,] fields, int x, int y, int wx, int wy, bool dg, bool mv, bool mh, bool mc, int deep)
        {
            int ret = -1;

            if ( iGeneratedBoards == 22418 )
            {
                //iPrintBoard = true;
                ret = -2;
            }
            if (iMaxDeep > 0 && deep > iMaxDeep)
            {
                return ret;
            }
            if (iPrintBoard)
            {
                Console.WriteLine("deep: {0}", deep);
                PrintBoard(ref fields, wx, wy);
            }
            iGeneratedDeep = deep;
            iGeneratedCurX = x; 
            iGeneratedCurY = y;

            for (int j = 0; j < wy; j++)
            {
                for (int i = 0; i < wx; i++)
                {
                    /*if (deep == 1) { iGeneratedCurX = i; iGeneratedCurY = j; Console.WriteLine("xy: {0} {1}", i, j);  } */
                    if (thrGenerator.CancellationPending) return 3;
                    if (fields[j, i].iNone || fields[j, i].iEmpty) continue;
                    int move_cnt = -1;
                    do
                    {
                        CField[,] newLevel = CreateBoard( ref fields, wx, wy );
                        move_cnt = GenerateMove(ref newLevel, i, j, wx, wy, dg, move_cnt);

                        if (move_cnt != -1)
                        { // zrobiono ruch
                            ret = 1;
                            iGeneratedMoves++;

                            if (iHashCheck)
                            {
                                uint h = CraeteHash(ref newLevel, wx, wy, false, false);
                                if (iHashLevels.Contains(h))
                                {
                                    continue;
                                }
                                if (iPrintBoard) Console.WriteLine("hash: {0}", h);
                                iHashLevels.Add(h);
                                if (mv) iHashLevels.Add(CraeteHash(ref newLevel, wx, wy, false, true));
                                if (mh) iHashLevels.Add(CraeteHash(ref newLevel, wx, wy, true, false));
                                if ((mv && mh) || mc) iHashLevels.Add(CraeteHash(ref newLevel, wx, wy, true, true));
                            }

                            iGeneratedBoards++;

                            if (CheckGameWin(ref newLevel, wx, wy, x, y))
                            {
                                //Console.WriteLine("win:");
                                //PrintBoard(ref newLevel, wx, wy);
                                PrintBoardOnList(ref newLevel, wx, wy);
                                return 2;
                            }

                            if (GenerateNewBoardStep(ref newLevel, i, j, wx, wy, dg, mv, mh, mc, deep + 1) == 2)
                            {
                                //Console.WriteLine("win:");
                                //PrintBoard(ref newLevel, wx, wy);
                                PrintBoardOnList(ref newLevel, wx, wy);
                                return 2;
                            }
                        }

                        
                    }
                    while (move_cnt != -1);                    
                }
            }

            if (iGeneratedMoves % 100 == 0)
            {
                UpdateProgress( wx, wy );
            }

            return ret;
        }

        private void thrGenerator_DoWork(object sender, DoWorkEventArgs e)
        {
            TWorkerArg arg = (TWorkerArg) e.Argument;

            int x = arg.xItem;
            int y = arg.yItem;
            int wx = (int)nFieldsX.Value;
            int wy = (int)nFieldsY.Value;
            bool dg = arg.iDiagonalMoves;
            bool mv = arg.iMirrorVertical;
            bool mh = arg.iMirrorHorizontal;
            bool mc = arg.iMirrorCenter;
            int ret = 0;

            Console.WriteLine("Generator started");
            UpdateProgress(wx, wy);

            if (x == -1 && y == -1)
            {
                for (int j = 0; j < wy; j++)
                {
                    if (thrGenerator.CancellationPending) break;
                    for (int i = 0; i < wx; i++)
                    {
                        if (thrGenerator.CancellationPending) break;
                        if (iFields[j, i].iNone) continue;

                        CField[,] newLevel = CreateBoard(ref iFields, wx, wy);
                        newLevel[j, i].iItem = true;
                        newLevel[j, i].iEmpty = false;
                        ret = GenerateNewBoardStep(ref newLevel, i, j, wx, wy, dg, mv, mh, mc, 0);
                        if (ret == 2)
                        {
                            PrintBoardOnList(ref newLevel, wx, wy);
                            break;
                        }
                    }
                    if (ret == 2) break;
                }

            }
            else
            {
                for (int j = 0; j < wy; j++)
                {   
                    if (thrGenerator.CancellationPending) break;
                    for (int i = 0; i < wx; i++)
                    {
                        if (thrGenerator.CancellationPending) break;
                        if (iFields[j, i].iItem)
                        {
                            ret = GenerateNewBoardStep(ref iFields, i, j, wx, wy, dg, mv, mh, mc, 0);
                            if (ret == 2)
                            {
                                break;
                            }
                        }
                    }
                    if (ret == 2) break;
                }

                //ret = GenerateNewBoardStep(ref iFields, x, y, wx, wy, dg, mv, mh, mc, 0);
            }
            UpdateProgress(wx, wy);
            if (ret == 2)
            {
                //Console.WriteLine("win:");
                //PrintBoard(ref iFields, wx, wy);
                PrintBoardOnList(ref iFields, wx, wy);

                MessageBox.Show("Solution found!!!");
            }
            else
            {
                MessageBox.Show("Solution not found.");
            }

            Console.WriteLine("Generate done, result: {0}", ret);
            if (ret == 2) Console.WriteLine("win!");
            else Console.WriteLine("not win");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            thrGenerator.CancelAsync();

            bGenerate.Enabled = true;
            bStop.Enabled = false;
        }

        private void thrGenerator_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            bGenerate.Enabled = true;
            bStop.Enabled = false;

            CreateResult(iResult);
        }

        void CreateResult( String result )
        {
            if (result.Length > 0)
            {
                tbResult.Text = result;
            }
        }

        private void newBoardToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (thrGenerator.IsBusy)
            {
                MessageBox.Show("Generator is busy! Stop to proceed.");
                return;
            }

            pBoard.Controls.Clear();
            tbResult.Text = "";
            this.Text = "Hermit Level Generator";
        }

        private void saveBoardToolStripMenuItem_Click(object sender, EventArgs ev)
        {
            if (thrGenerator.IsBusy)
            {
                MessageBox.Show("Generator is busy! Stop to proceed.");
                return;
            }
            if ( iFields == null )
            {
                MessageBox.Show("Setup board first.");
                return;
            }

            saveFD.FileName = "board.dat";
            if (saveFD.ShowDialog() == DialogResult.OK)
            {
                BinaryWriter file = new BinaryWriter(File.Open(saveFD.FileName, FileMode.Create));

                try
                {
                    file.Write((UInt32)0x474C4248);
                    file.Write((Byte)0x01);
                    file.Write((Byte)0x01);
                    
                    file.Write((Byte)nFieldsX.Value);
                    file.Write((Byte)nFieldsY.Value);
                    file.Write((Byte)nDeep.Value);
                    file.Write((Byte)nEmptyFields.Value);
                    file.Write(cbDiagonalMoves.Checked);
                    file.Write(cbMirrorVertical.Checked);
                    file.Write(cbMirrorHorizontal.Checked);
                    file.Write(cbMirrorCenter.Checked);
                    file.Write(cbHashCheck.Checked);
                    file.Write(cbMD5CRC.Checked);

                    for (int j = 0; j < nFieldsY.Value; j++)
                    {
                        for (int i = 0; i < nFieldsX.Value; i++)
                        {
                            Byte b = 0;
                            if (iFields[j, i].iNone) b |= 0x01;
                            if (iFields[j, i].iEmpty) b |= 0x02;
                            if (iFields[j, i].iItem) b |= 0x04;
                            if (iFields[j, i].iDone) b |= 0x08;
                            if (iFields[j, i].BackColor == Color.Black) b |= 0x10;
                            if (!iFields[j, i].Checked) b |= 0x20;

                            file.Write(b);
                        }
                    }

                    file.Write(iResult);
                    file.Write(statusLabelGenerated.Text);

                    this.Text = "Hermit Level Generator - " + saveFD.FileName;
                }
                catch (EndOfStreamException e)
                {
                    Console.WriteLine("{0} caught and ignored. " +
                        "Using default values.", e.GetType().Name);
                }
                finally
                {
                    file.Close();
                }
            }
        }

        private void loadBoardToolStripMenuItem_Click(object sender, EventArgs ev)
        {
            if (thrGenerator.IsBusy)
            {
                MessageBox.Show("Generator is busy! Stop to proceed.");
                return;
            }

            pBoard.Controls.Clear();
            tbResult.Text = "";
            this.Text = "Hermit Level Generator";

            if (openFD.ShowDialog() == DialogResult.OK)
            {
                BinaryReader file = new BinaryReader(File.Open(openFD.FileName, FileMode.Open));

                try
                {
                    if (file.PeekChar() == -1) // file is empty
                    {
                        return;
                    }

                    if (file.ReadUInt32() != 0x474C4248)
                    {
                        return;
                    }

                    if (file.ReadByte() != 0x01) // version
                    {
                        return;
                    }

                    int vminor = file.ReadByte();
                    if (vminor != 0x00 && vminor != 0x01) // version
                    {
                        return;
                    }

                    int wx = file.ReadByte();
                    int wy = file.ReadByte();
                    nFieldsX.Value = (decimal)wx;
                    nFieldsY.Value = (decimal)wy;
                    nDeep.Value = (decimal)file.ReadByte();
                    nEmptyFields.Value = (decimal)file.ReadByte();
                    cbDiagonalMoves.Checked = file.ReadBoolean();
                    cbMirrorVertical.Checked = file.ReadBoolean();
                    cbMirrorHorizontal.Checked = file.ReadBoolean();
                    cbMirrorCenter.Checked = file.ReadBoolean();
                    cbHashCheck.Checked = file.ReadBoolean();
                    if (vminor == 0x01)
                    {
                        cbMD5CRC.Checked = file.ReadBoolean();
                    }

                    int m = 5;
                    int w = 50;
                    int h = w;
                    int s = 5;

                    iFields = new CField[wy, wx];
                    for (int j = 0; j < wy; j++)
                    {
                        for (int i = 0; i < wx; i++)
                        {
                            CField cb = new CField();
                            cb.Appearance = Appearance.Button;
                            cb.Left = m + i * w + i * s + s;
                            cb.Top = m + j * h + j * s + s;
                            cb.Width = w;
                            cb.Height = h;
                            cb.Checked = true;
                            cb.Text = "";
                            cb.MouseUp += new MouseEventHandler(cb_MouseUp);
                            cb.MouseClick += new MouseEventHandler(cb_MouseClick);
                            cb.iEmpty = false;
                            cb.iNone = false;
                            cb.iItem = false;
                            cb.iDone = false;
                            iFields[j, i] = cb;

                            Byte b = file.ReadByte();
                            if ((b & 0x01) == 0x01) iFields[j, i].iNone = true;
                            if ((b & 0x02) == 0x02) iFields[j, i].iEmpty = true;
                            if ((b & 0x04) == 0x04) iFields[j, i].iItem = true;
                            if ((b & 0x08) == 0x08) iFields[j, i].iDone = true;
                            if ((b & 0x10) == 0x10) iFields[j, i].BackColor = Color.Black;
                            if ((b & 0x20) == 0x20) iFields[j, i].Checked = false;

                            pBoard.Controls.Add(cb);
                        }
                    }

                    iResult = file.ReadString();
                    CreateResult(iResult);

                    statusLabelGenerated.Text = file.ReadString();

                    this.Text = "Hermit Level Generator - " + openFD.FileName;
                }
                catch (EndOfStreamException e)
                {
                    Console.WriteLine("{0} caught and ignored. " +
                        "Using default values.", e.GetType().Name);
                    return;
                }
                finally
                {
                    file.Close();
                }

            }
        }

        private void cbPrint_CheckedChanged(object sender, EventArgs e)
        {
        }

        private void cbHashCheck_CheckedChanged(object sender, EventArgs e)
        {
            cbMD5CRC.Enabled = cbHashCheck.Checked;
        }

    }
}