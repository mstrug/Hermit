using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Drawing;
using System.Windows.Forms;

namespace HermitLevelEditor
{
    public class Level : Object
    {
        public Byte iID;
        public Byte iWidth;
        public Byte iHeight;
        public Byte iFieldShape; // 0 square, 1 diamond
        public Boolean iDiagonalMoves;
        Byte[] iData;

        TabPage iTabPage;

        const Byte KHermitLevelFieldTypeMask = 0x0F;
        enum TFieldType
        {
            EFieldNone,
            EFieldEmpty,
            EFieldItem,
            EFieldSelected,
            EFieldHint,
            EFieldFinish = 0x10
        };

        public Level()
        {
            iWidth = 0;
            iHeight = 0;
        }

        public Level(Byte aId, Byte aWidth, Byte aHeight, Byte aFieldShape, Boolean aDiagonalMoves, Byte[] aData)
        {
            iID = aId;
            iWidth = aWidth;
            iHeight = aHeight;
            iFieldShape = aFieldShape;
            iDiagonalMoves = aDiagonalMoves;
            if (aData == null)
            {
                iData = new Byte[iWidth * iHeight];
            }
            else
            {
                iData = aData;
            }
        }

        public Boolean LoadFromFile10(BinaryReader aFile)
        {
            /*
             * version 1.0
             * 
             * 1 byte  width
             * 1 byte  height
             * width * height bytes  data
             * FF FF
             */
            iWidth = aFile.ReadByte();
            iHeight = aFile.ReadByte();

            iData = aFile.ReadBytes(iWidth * iHeight);

            // 0xFFFF
            aFile.ReadBytes(2);

            return true;
        }

        public Boolean LoadFromFile11(BinaryReader aFile)
        {
            /*
             * version 1.1
             * 
             * 1 byte  ID (per group)
             * 1 byte  width
             * 1 byte  height
             * 1 byte  field shape
             * 1 byte  diagonal moves boolean
             * width * height bytes  data
             * FF FF
             * 
             */

            iID = aFile.ReadByte();
            iWidth = aFile.ReadByte();
            iHeight = aFile.ReadByte();
            iFieldShape = aFile.ReadByte();
            iDiagonalMoves = ( aFile.ReadByte() == 1 ? true : false );

            iData = aFile.ReadBytes(iWidth * iHeight);

            // 0xFFFF
            aFile.ReadBytes(2);

            return true;
        }

        public Boolean SaveToFile11(BinaryWriter aFile)
        {
            aFile.Write(iID);
            aFile.Write(iWidth);
            aFile.Write(iHeight);
            aFile.Write(iFieldShape);
            aFile.Write(iDiagonalMoves);

            for (int i = 0; i < iTabPage.Controls.Count; i++)
            {
                if (iTabPage.Controls[i].GetType().ToString().ToLower().Contains("button"))
                {
                    if (iTabPage.Controls[i].Text == "")
                    {
                        aFile.Write((Byte)0);
                    }
                    else if (iTabPage.Controls[i].Text == "1")
                    {
                        aFile.Write((Byte)1);
                    }
                    else if (iTabPage.Controls[i].Text == "1o")
                    {
                        aFile.Write((Byte)(1 | (Byte)TFieldType.EFieldFinish));
                    }
                    else if (iTabPage.Controls[i].Text == "2")
                    {
                        aFile.Write((Byte)2);
                    }
                    else if (iTabPage.Controls[i].Text == "2o")
                    {
                        aFile.Write((Byte)(2 | (Byte)TFieldType.EFieldFinish));
                    }
                }
            }

            aFile.Write((Byte)0xFF);
            aFile.Write((Byte)0xFF);

            return true;
        }

        Byte FieldC( int aX, int aY )
        {
            return (Byte)( iData[ ( iHeight - aY - 1 ) * iWidth + aX ] & KHermitLevelFieldTypeMask );
        }
        Boolean IsFieldFinish( int aX, int aY )
	    {
            return ((iData[(iHeight - aY - 1) * iWidth + aX] & (Byte)TFieldType.EFieldFinish) == (Byte)TFieldType.EFieldFinish);
        }

        public void CreateTabPage( TabPage aTabPage )
        {
            int marginX = 30;
            int marginY = 30;
            int w = 30;
            int h = 30;

            for (int j = 0; j < iHeight; j++)
            {
                for (int i = 0; i < iWidth; i++)
                {
                    Button btn = new Button();
                    btn.SetBounds(marginX + i * w, marginY + j * h, w, h);
                    ///btn.Click += new MouseEventHandler(PageButton_Click);
                    btn.Font = new Font(btn.Font.FontFamily, 7);
                    btn.MouseUp += new MouseEventHandler(PageButton_Click);
                    btn.Margin = new Padding( 0, btn.Margin.Top, 0, btn.Margin.Bottom );

                    Byte field = (Byte)FieldC(i, iHeight - j - 1);
                    if (field > 0)
                    {
                        btn.Text = field.ToString();
                    }
                    if (IsFieldFinish(i, iHeight - j - 1))
                    {
                        btn.Text += "o";
                    }

                    aTabPage.Controls.Add(btn);
                    //Console.WriteLine(aTabPage.Controls[0].GetType().ToString());
                }
            }

            iTabPage = aTabPage;
            iTabPage.Tag = (Object)this;
        }

        void PageButton_Click(object sender, MouseEventArgs e)
        {
            Button btn = (Button)sender;
            
            if (!btn.Bounds.Contains(e.Location))
            {
                //return;
            }

            if (e.Button == MouseButtons.Left)
            {
                if (btn.Text == "" || btn.Text == "2o")
                {
                    btn.Text = "1";
                }
                else if (btn.Text == "1" || btn.Text == "1o")
                {
                    btn.Text = "2";
                }
                else
                {
                    btn.Text = "";
                }
            }
            else if (e.Button == MouseButtons.Right)
            {
                if (btn.Text == "" || btn.Text == "1o")
                {
                    btn.Text = "2";
                }
                else if (btn.Text == "2" || btn.Text == "2o")
                {
                    btn.Text = "1";
                }
                else
                {
                    btn.Text = "";
                }
            }
            else if (e.Button == MouseButtons.Middle)
            {
                if (btn.Text == "1")
                {
                    btn.Text = "1o";
                }
                else if (btn.Text == "2")
                {
                    btn.Text = "2o";
                }
                else if (btn.Text == "1o")
                {
                    btn.Text = "1";
                }
                else if (btn.Text == "2o")
                {
                    btn.Text = "2";
                }
            }

        }
    }
}
