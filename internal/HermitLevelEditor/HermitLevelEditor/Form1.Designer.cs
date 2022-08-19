namespace HermitLevelEditor
{
    partial class FormMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.iMenu = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newLevelGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openLevelGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.saveLevelGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveLevelGroupAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.importLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.levelGroupPropertiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.addNewLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.levelPropertiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.Status = new System.Windows.Forms.StatusStrip();
            this.statusLabelFile = new System.Windows.Forms.ToolStripStatusLabel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.Tab = new System.Windows.Forms.TabControl();
            this.LevelTabPage = new System.Windows.Forms.TabPage();
            this.panel2 = new System.Windows.Forms.Panel();
            this.bRight = new System.Windows.Forms.Button();
            this.bLeft = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialogImport = new System.Windows.Forms.OpenFileDialog();
            this.iMenu.SuspendLayout();
            this.Status.SuspendLayout();
            this.panel1.SuspendLayout();
            this.Tab.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // iMenu
            // 
            this.iMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.iMenu.Location = new System.Drawing.Point(0, 0);
            this.iMenu.Name = "iMenu";
            this.iMenu.Size = new System.Drawing.Size(692, 24);
            this.iMenu.TabIndex = 0;
            this.iMenu.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newLevelGroupToolStripMenuItem,
            this.openLevelGroupToolStripMenuItem,
            this.toolStripMenuItem1,
            this.saveLevelGroupToolStripMenuItem,
            this.saveLevelGroupAsToolStripMenuItem,
            this.toolStripMenuItem2,
            this.importLevelToolStripMenuItem,
            this.toolStripMenuItem4,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newLevelGroupToolStripMenuItem
            // 
            this.newLevelGroupToolStripMenuItem.Name = "newLevelGroupToolStripMenuItem";
            this.newLevelGroupToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.newLevelGroupToolStripMenuItem.Text = "New level group...";
            this.newLevelGroupToolStripMenuItem.Click += new System.EventHandler(this.newLevelGroupToolStripMenuItem_Click);
            // 
            // openLevelGroupToolStripMenuItem
            // 
            this.openLevelGroupToolStripMenuItem.Name = "openLevelGroupToolStripMenuItem";
            this.openLevelGroupToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.openLevelGroupToolStripMenuItem.Text = "Open level group...";
            this.openLevelGroupToolStripMenuItem.Click += new System.EventHandler(this.openLevelGroupToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(177, 6);
            // 
            // saveLevelGroupToolStripMenuItem
            // 
            this.saveLevelGroupToolStripMenuItem.Name = "saveLevelGroupToolStripMenuItem";
            this.saveLevelGroupToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.saveLevelGroupToolStripMenuItem.Text = "Save level group";
            this.saveLevelGroupToolStripMenuItem.Click += new System.EventHandler(this.saveLevelGroupToolStripMenuItem_Click);
            // 
            // saveLevelGroupAsToolStripMenuItem
            // 
            this.saveLevelGroupAsToolStripMenuItem.Name = "saveLevelGroupAsToolStripMenuItem";
            this.saveLevelGroupAsToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.saveLevelGroupAsToolStripMenuItem.Text = "Save level group as...";
            this.saveLevelGroupAsToolStripMenuItem.Click += new System.EventHandler(this.saveLevelGroupAsToolStripMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(177, 6);
            // 
            // importLevelToolStripMenuItem
            // 
            this.importLevelToolStripMenuItem.Name = "importLevelToolStripMenuItem";
            this.importLevelToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.importLevelToolStripMenuItem.Text = "Import level...";
            this.importLevelToolStripMenuItem.Click += new System.EventHandler(this.importLevelToolStripMenuItem_Click);
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(177, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.levelGroupPropertiesToolStripMenuItem,
            this.toolStripMenuItem3,
            this.addNewLevelToolStripMenuItem,
            this.levelPropertiesToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // levelGroupPropertiesToolStripMenuItem
            // 
            this.levelGroupPropertiesToolStripMenuItem.Name = "levelGroupPropertiesToolStripMenuItem";
            this.levelGroupPropertiesToolStripMenuItem.Size = new System.Drawing.Size(182, 22);
            this.levelGroupPropertiesToolStripMenuItem.Text = "Level group properties";
            this.levelGroupPropertiesToolStripMenuItem.Click += new System.EventHandler(this.levelGroupPropertiesToolStripMenuItem_Click);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(179, 6);
            // 
            // addNewLevelToolStripMenuItem
            // 
            this.addNewLevelToolStripMenuItem.Name = "addNewLevelToolStripMenuItem";
            this.addNewLevelToolStripMenuItem.Size = new System.Drawing.Size(182, 22);
            this.addNewLevelToolStripMenuItem.Text = "Add new level";
            this.addNewLevelToolStripMenuItem.Click += new System.EventHandler(this.addNewLevelToolStripMenuItem_Click);
            // 
            // levelPropertiesToolStripMenuItem
            // 
            this.levelPropertiesToolStripMenuItem.Name = "levelPropertiesToolStripMenuItem";
            this.levelPropertiesToolStripMenuItem.Size = new System.Drawing.Size(182, 22);
            this.levelPropertiesToolStripMenuItem.Text = "Level properties";
            this.levelPropertiesToolStripMenuItem.Click += new System.EventHandler(this.levelPropertiesToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(103, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // openFileDialog
            // 
            this.openFileDialog.DefaultExt = "dat";
            this.openFileDialog.Filter = "Hermit level group files|*.dat|All files|*.*";
            this.openFileDialog.ReadOnlyChecked = true;
            this.openFileDialog.Title = "Open Hermit Level Group file";
            // 
            // Status
            // 
            this.Status.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusLabelFile});
            this.Status.Location = new System.Drawing.Point(0, 400);
            this.Status.Name = "Status";
            this.Status.Size = new System.Drawing.Size(692, 22);
            this.Status.TabIndex = 2;
            this.Status.Text = "statusStrip1";
            // 
            // statusLabelFile
            // 
            this.statusLabelFile.Name = "statusLabelFile";
            this.statusLabelFile.Size = new System.Drawing.Size(0, 17);
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Controls.Add(this.Tab);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 24);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(568, 376);
            this.panel1.TabIndex = 5;
            // 
            // Tab
            // 
            this.Tab.Controls.Add(this.LevelTabPage);
            this.Tab.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Tab.Location = new System.Drawing.Point(0, 0);
            this.Tab.Name = "Tab";
            this.Tab.SelectedIndex = 0;
            this.Tab.Size = new System.Drawing.Size(564, 372);
            this.Tab.TabIndex = 0;
            // 
            // LevelTabPage
            // 
            this.LevelTabPage.Location = new System.Drawing.Point(4, 22);
            this.LevelTabPage.Name = "LevelTabPage";
            this.LevelTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.LevelTabPage.Size = new System.Drawing.Size(556, 346);
            this.LevelTabPage.TabIndex = 0;
            this.LevelTabPage.Text = "tabPage1";
            this.LevelTabPage.UseVisualStyleBackColor = true;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.bRight);
            this.panel2.Controls.Add(this.bLeft);
            this.panel2.Controls.Add(this.label4);
            this.panel2.Controls.Add(this.label3);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Controls.Add(this.label1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel2.Location = new System.Drawing.Point(568, 24);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(124, 376);
            this.panel2.TabIndex = 4;
            // 
            // bRight
            // 
            this.bRight.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bRight.Location = new System.Drawing.Point(43, 347);
            this.bRight.Name = "bRight";
            this.bRight.Size = new System.Drawing.Size(31, 23);
            this.bRight.TabIndex = 6;
            this.bRight.Text = ">>";
            this.bRight.UseVisualStyleBackColor = true;
            this.bRight.Click += new System.EventHandler(this.bRight_Click);
            // 
            // bLeft
            // 
            this.bLeft.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bLeft.Location = new System.Drawing.Point(6, 347);
            this.bLeft.Name = "bLeft";
            this.bLeft.Size = new System.Drawing.Size(31, 23);
            this.bLeft.TabIndex = 5;
            this.bLeft.Text = "<<";
            this.bLeft.UseVisualStyleBackColor = true;
            this.bLeft.Click += new System.EventHandler(this.bLeft_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(26, 89);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(68, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "o - finish field";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(26, 66);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "2 - item field";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(26, 44);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(72, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "1 - empty field";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(21, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(46, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Legend:";
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.DefaultExt = "dat";
            this.saveFileDialog.Filter = "Hermit level group files|*.dat|All files|*.*";
            this.saveFileDialog.Title = "Save Hermit level as";
            // 
            // openFileDialogImport
            // 
            this.openFileDialogImport.DefaultExt = "dat";
            this.openFileDialogImport.FileName = "board.dat";
            this.openFileDialogImport.Filter = "Board file (*.dat)|*.dat|All files|*.*";
            this.openFileDialogImport.Multiselect = true;
            this.openFileDialogImport.Title = "Select Hermit Level Generator file";
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(692, 422);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.Status);
            this.Controls.Add(this.iMenu);
            this.MainMenuStrip = this.iMenu;
            this.Name = "FormMain";
            this.Text = "Hermit Level Editor";
            this.iMenu.ResumeLayout(false);
            this.iMenu.PerformLayout();
            this.Status.ResumeLayout(false);
            this.Status.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.Tab.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip iMenu;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newLevelGroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openLevelGroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem saveLevelGroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveLevelGroupAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem levelGroupPropertiesToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.StatusStrip Status;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TabControl Tab;
        private System.Windows.Forms.TabPage LevelTabPage;
        private System.Windows.Forms.ToolStripStatusLabel statusLabelFile;
        private System.Windows.Forms.ToolStripMenuItem addNewLevelToolStripMenuItem;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.Button bRight;
        private System.Windows.Forms.Button bLeft;
        private System.Windows.Forms.ToolStripMenuItem levelPropertiesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importLevelToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
        private System.Windows.Forms.OpenFileDialog openFileDialogImport;
    }
}

