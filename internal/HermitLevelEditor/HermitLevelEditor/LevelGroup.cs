using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace HermitLevelEditor
{

    public class LevelGroup : Object
    {
        public UInt16 iLevelGroupId;
        public Byte iLevelCount;
        public String iFileName;

        ArrayList iLevels;


        public LevelGroup()
        {
            iLevels = new ArrayList();
        }

        public Boolean LoadFromFile(String aFileName)
        {
            BinaryReader file = new BinaryReader( File.Open(aFileName, FileMode.Open) );

            try
            {
                if (file.PeekChar() == -1) // file is empty
                {
                    return false;
                }

                if (file.ReadUInt32() != 0x47564c48)
                {
                    return false;
                }

                if (file.ReadByte() != 0x01) // version
                {
                    return false;
                }

                int vminor = file.ReadByte();
                if (vminor != 0x00 && vminor != 0x01) // version
                {
                    return false;
                }

                iLevelGroupId = file.ReadUInt16();

                iLevelCount = file.ReadByte();

                file.ReadBytes(3); // reserved

                for (int i = 0; i < iLevelCount; i++)
                {
                    Level lvl = new Level();
                    if (vminor == 0x00)
                    {
                        lvl.LoadFromFile10(file);
                        lvl.iID = (Byte)i;
                    }
                    else
                    {
                        lvl.LoadFromFile11(file);
                    }

                    iLevels.Add(lvl);
                }

                iFileName = aFileName;
            }
            catch (EndOfStreamException e)
            {
                Console.WriteLine("{0} caught and ignored. " +
                    "Using default values.", e.GetType().Name);
                return false;
            }
            finally
            {
                file.Close();
            }

            return true;
        }

        public Boolean SaveToFile()
        {
            if (iFileName == null)
            {
                return false;
            }
            return SaveToFile(iFileName);
        }

        public Boolean SaveToFile(String aFileName)
        {
            BinaryWriter file = new BinaryWriter(File.Open(aFileName, FileMode.Create));

            try
            {
                file.Write((UInt32)0x47564c48);
                file.Write((Byte)0x01);
                file.Write((Byte)0x01);
                file.Write(iLevelGroupId);
                file.Write(iLevelCount);
                file.Write((Byte)0); // reserved
                file.Write((Byte)0); // reserved
                file.Write((Byte)0); // reserved

                for (int i = 0; i < iLevels.Count; i++)
                {
                    ((Level)iLevels[i]).SaveToFile11(file);
                }

                iFileName = aFileName;
            }
            catch (EndOfStreamException e)
            {
                Console.WriteLine("{0} caught and ignored. " +
                    "Using default values.", e.GetType().Name);
                return false;
            }
            finally
            {
                file.Close();
            }

            return true;
        }


        public Boolean ImportLevelFromFile(String aFileName, ref TabControl aTab, Byte aLevelId, Byte aWidth, Byte aHeight, Byte aFieldShape, Boolean aDiagonalMoves)
        {
            BinaryReader file = new BinaryReader(File.Open(aFileName, FileMode.Open));

            try
            {
                if (file.PeekChar() == -1) // file is empty
                {
                    return false;
                }

                if (file.ReadUInt32() != 0x474C4248)    // HBLG
                {
                    return false;
                }

                if (file.ReadByte() != 0x01) // version
                {
                    return false;
                }

                int vminor = file.ReadByte();
                if (vminor != 0x00 && vminor != 0x01) // version
                {
                    return false;
                }

                byte wx = file.ReadByte();
                byte wy = file.ReadByte();

                file.ReadBytes(2); // skip
                bool diagonalMoves = file.ReadBoolean();
                file.ReadBoolean(); // skip
                file.ReadBoolean(); // skip
                file.ReadBoolean(); // skip
                file.ReadBoolean(); // skip
                if (vminor == 0x01)
                {
                    file.ReadBoolean(); // skip
                }

                Byte[] data = new Byte[wx * wy];
                for (int j = 0; j < wy; j++)
                {
                    for (int i = 0; i < wx; i++)
                    {
                        Byte b = file.ReadByte();
                        int idx = j * wx + i;

                        if ((b & 0x01) == 0x01) data[idx] = 0; // iFields[j, i].iNone = true;
                        if ((b & 0x02) == 0x02) data[idx] = 1; // iFields[j, i].iEmpty = true;
                        //if ((b & 0x04) == 0x04) data[idx] = 2; // iFields[j, i].iItem = true;
                        //if ((b & 0x08) == 0x08) iFields[j, i].iDone = true;
                        if ((b & 0x10) == 0x10) data[idx] |= 0x10; // iFields[j, i].BackColor = Color.Black;
                        //if ((b & 0x20) == 0x20) iFields[j, i].Checked = false;
                    }
                }

                String results = file.ReadString();
                if (results.Length == 0)
                {
                    return false;
                }

                for (int j = 0, c = 0; j < wy; j++)
                {
                    for (int i = 0; i < wx; i++)
                    {
                        while ( true )
                        {
                            if (results[c] == '\r') c++;
                            else if (results[c] == '\n') c++;
                            //else if (results[c] == ' ') c++;
                            else break;
                        }

                        int idx = j * wx + i;

                        if (results[c] == 'x' && ( ( data[idx] & 0x10 ) == 0x10 ) )
                        {
                            data[idx] = 0x12;
                        }
                        else if (results[c] == 'x')
                        {
                            data[idx] = 2;
                        }

                        c++;
                    }
                }

                AddNewLevel(ref aTab, aLevelId, wx, wy, aFieldShape, diagonalMoves, data );
            }
            catch (EndOfStreamException e)
            {
                Console.WriteLine("{0} caught and ignored. " +
                    "Using default values.", e.GetType().Name);
                return false;
            }
            finally
            {
                file.Close();
            }

            return true;
        }

        public void CreateLevelTabPages( ref TabControl aTab )
        {
            for (int i = 0; i < iLevels.Count; i++)
            {
                aTab.TabPages.Add( i.ToString() );

                ((Level)iLevels[i]).CreateTabPage(aTab.TabPages[i]);
            }
        }

        public void AddNewLevel(ref TabControl aTab, Byte aLevelId, Byte aWidth, Byte aHeight, Byte aFieldShape, Boolean aDiagonalMoves, Byte[] aData)
        {
            Level lvl = new Level( aLevelId, aWidth, aHeight, aFieldShape, aDiagonalMoves, aData );

            int idx = iLevels.Count;
            iLevels.Add(lvl);

            aTab.TabPages.Add(aLevelId.ToString());
            
            ((Level)iLevels[idx]).CreateTabPage(aTab.TabPages[idx]);

            iLevelCount++;
        }

        public void MoveLeft(int aLevelIdx)
        {
            Object tmp = iLevels[aLevelIdx - 1];
            iLevels[aLevelIdx - 1] = iLevels[aLevelIdx];
            iLevels[aLevelIdx] = tmp;
        }

        public void MoveRight(int aLevelIdx)
        {
            Object tmp = iLevels[aLevelIdx + 1];
            iLevels[aLevelIdx + 1] = iLevels[aLevelIdx];
            iLevels[aLevelIdx] = tmp;
        }

    }
}
