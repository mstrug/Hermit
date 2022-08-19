using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HermitLevelEditor
{
    public partial class FormMain : Form
    {
        LevelGroup iLG;

        public FormMain()
        {
            InitializeComponent();

            Tab.TabPages.Clear();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void newLevelGroupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            iLG = new LevelGroup();
            FormLevelGroupProperties f = new FormLevelGroupProperties(ref iLG);
            DialogResult dr = f.ShowDialog();
            if (dr == DialogResult.OK)
            {
                Console.WriteLine("OK");
                Console.WriteLine(iLG.iLevelGroupId);
                statusLabelFile.Text = "New level group created";
                Tab.TabPages.Clear();
            }
        }

        private void openLevelGroupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                Console.WriteLine(openFileDialog.FileName);
                iLG = new LevelGroup();
                if (iLG.LoadFromFile(openFileDialog.FileName))
                {
                    statusLabelFile.Text = iLG.iFileName;

                    Tab.TabPages.Clear();

                    iLG.CreateLevelTabPages(ref Tab);
                }
            }
        }

        private void levelGroupPropertiesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (iLG != null)
            {
                FormLevelGroupProperties f = new FormLevelGroupProperties(ref iLG);
                f.ShowDialog();
            }
        }

        private void saveLevelGroupToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (iLG != null)
            {
                if (!iLG.SaveToFile())
                {
                    saveLevelGroupAsToolStripMenuItem_Click(sender, e);
                }
            }
        }

        private void saveLevelGroupAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (iLG != null)
            {
                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    if (iLG.SaveToFile(saveFileDialog.FileName))
                    {
                        statusLabelFile.Text = iLG.iFileName;
                    }
                }
            }
        }

        private void addNewLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (iLG != null)
            {
                FormLevelProperties prop = new FormLevelProperties(Tab.TabCount);
                if (prop.ShowDialog() == DialogResult.OK)
                {
                    iLG.AddNewLevel(ref Tab, prop.GetId(), prop.GetWidth(), prop.GetHeight(), prop.GetFieldShape(), prop.GetDiagonalMoves(), null);
                    Tab.SelectedIndex = Tab.TabCount - 1;
                }
            }
        }

        private void bLeft_Click(object sender, EventArgs e)
        {
            int idx = Tab.SelectedIndex;
            if (idx > 0)
            {
                TabPage tab = Tab.TabPages[idx - 1];
                Tab.TabPages[idx - 1] = Tab.TabPages[idx];
                Tab.TabPages[idx] = tab;
                Tab.SelectedIndex--;
                iLG.MoveLeft(idx);
            }
        }

        private void bRight_Click(object sender, EventArgs e)
        {
            int idx = Tab.SelectedIndex;
            if (idx < Tab.TabCount - 1)
            {
                TabPage tab = Tab.TabPages[idx + 1];
                Tab.TabPages[idx + 1] = Tab.TabPages[idx];
                Tab.TabPages[idx] = tab;
                Tab.SelectedIndex++;
                iLG.MoveRight(idx);
            }
        }

        private void bID_Click(object sender, EventArgs e)
        {
            int idx = Tab.SelectedIndex;
            if (idx >= 0)
            {
                String str = Tab.TabPages[idx].Text;
                if (InputBox("Set level ID", "Level ID:", ref str) == DialogResult.OK)
                {
                    Tab.TabPages[idx].Text = str;
                }
            }
        }


        public static DialogResult InputBox(string title, string promptText, ref string value)
        {
            Form form = new Form();
            Label label = new Label();
            TextBox textBox = new TextBox();
            Button buttonOk = new Button();
            Button buttonCancel = new Button();

            form.Text = title;
            label.Text = promptText;
            textBox.Text = value;

            buttonOk.Text = "OK";
            buttonCancel.Text = "Cancel";
            buttonOk.DialogResult = DialogResult.OK;
            buttonCancel.DialogResult = DialogResult.Cancel;

            label.SetBounds(9, 20, 372, 13);
            textBox.SetBounds(12, 36, 372, 20);
            buttonOk.SetBounds(228, 72, 75, 23);
            buttonCancel.SetBounds(309, 72, 75, 23);

            label.AutoSize = true;
            textBox.Anchor = textBox.Anchor | AnchorStyles.Right;
            buttonOk.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            buttonCancel.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;

            form.ClientSize = new Size(396, 107);
            form.Controls.AddRange(new Control[] { label, textBox, buttonOk, buttonCancel });
            form.ClientSize = new Size(Math.Max(300, label.Right + 10), form.ClientSize.Height);
            form.FormBorderStyle = FormBorderStyle.FixedDialog;
            form.StartPosition = FormStartPosition.CenterScreen;
            form.MinimizeBox = false;
            form.MaximizeBox = false;
            form.AcceptButton = buttonOk;
            form.CancelButton = buttonCancel;

            DialogResult dialogResult = form.ShowDialog();
            value = textBox.Text;
            return dialogResult;
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox about = new AboutBox();
            about.ShowDialog();
        }

        private void levelPropertiesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (iLG != null && Tab.SelectedTab.Tag != null)
            {
                FormLevelProperties prop = new FormLevelProperties((Level)Tab.SelectedTab.Tag);
                prop.ShowDialog();
            }
        }

        private void importLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (iLG != null)
            {
                openFileDialogImport.FileName = "board.dat";
                if (openFileDialogImport.ShowDialog() == DialogResult.OK)
                {
                    int cntOk = 0, cntFail = 0;
                    foreach (String fname in openFileDialogImport.FileNames)
                    {
                        FormLevelProperties prop = new FormLevelProperties(Tab.TabCount);
                        prop.SetImport();
                        if (prop.ShowDialog() == DialogResult.OK)
                        {
                            if (iLG.ImportLevelFromFile(fname, ref Tab, prop.GetId(), prop.GetWidth(), prop.GetHeight(), prop.GetFieldShape(), prop.GetDiagonalMoves()))
                            {
                                //MessageBox.Show("Level imported successfully.", "Information");
                                //statusLabelFile.Text = iLG.iFileName;
                                Tab.SelectedIndex = Tab.TabCount - 1;
                                cntOk++;
                            }
                            else
                            {
                                //MessageBox.Show("Failed to import levels.", "Error");
                                cntFail++;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    MessageBox.Show("Successfully imported " + cntOk + " levels. Failed to import " + cntFail + " levels.", "Information");
                }
            }
        }




    }
}