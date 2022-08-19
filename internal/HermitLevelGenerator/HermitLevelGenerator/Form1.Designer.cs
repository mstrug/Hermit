namespace HermitLevelGenerator
{
    partial class Form1
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.cbMirrorCenter = new System.Windows.Forms.CheckBox();
            this.nEmptyFields = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.cbMirrorHorizontal = new System.Windows.Forms.CheckBox();
            this.cbMirrorVertical = new System.Windows.Forms.CheckBox();
            this.cbDiagonalMoves = new System.Windows.Forms.CheckBox();
            this.cbPrint = new System.Windows.Forms.CheckBox();
            this.nDeep = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.bStop = new System.Windows.Forms.Button();
            this.bGenerate = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.nFieldsY = new System.Windows.Forms.NumericUpDown();
            this.nFieldsX = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newBoardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadBoardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveBoardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusBar = new System.Windows.Forms.StatusStrip();
            this.statusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusLabelGenerated = new System.Windows.Forms.ToolStripStatusLabel();
            this.thrGenerator = new System.ComponentModel.BackgroundWorker();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.pBoard = new System.Windows.Forms.Panel();
            this.tbResult = new System.Windows.Forms.TextBox();
            this.openFD = new System.Windows.Forms.OpenFileDialog();
            this.saveFD = new System.Windows.Forms.SaveFileDialog();
            this.cbHashCheck = new System.Windows.Forms.CheckBox();
            this.cbMD5CRC = new System.Windows.Forms.CheckBox();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nEmptyFields)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nDeep)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nFieldsY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nFieldsX)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.statusBar.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.cbMD5CRC);
            this.panel1.Controls.Add(this.cbHashCheck);
            this.panel1.Controls.Add(this.cbMirrorCenter);
            this.panel1.Controls.Add(this.nEmptyFields);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.cbMirrorHorizontal);
            this.panel1.Controls.Add(this.cbMirrorVertical);
            this.panel1.Controls.Add(this.cbDiagonalMoves);
            this.panel1.Controls.Add(this.cbPrint);
            this.panel1.Controls.Add(this.nDeep);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.bStop);
            this.panel1.Controls.Add(this.bGenerate);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.nFieldsY);
            this.panel1.Controls.Add(this.nFieldsX);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 24);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(632, 116);
            this.panel1.TabIndex = 0;
            // 
            // cbMirrorCenter
            // 
            this.cbMirrorCenter.AutoSize = true;
            this.cbMirrorCenter.Location = new System.Drawing.Point(392, 44);
            this.cbMirrorCenter.Name = "cbMirrorCenter";
            this.cbMirrorCenter.Size = new System.Drawing.Size(33, 17);
            this.cbMirrorCenter.TabIndex = 15;
            this.cbMirrorCenter.Text = "X";
            this.cbMirrorCenter.UseVisualStyleBackColor = true;
            // 
            // nEmptyFields
            // 
            this.nEmptyFields.Location = new System.Drawing.Point(373, 74);
            this.nEmptyFields.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nEmptyFields.Name = "nEmptyFields";
            this.nEmptyFields.Size = new System.Drawing.Size(90, 20);
            this.nEmptyFields.TabIndex = 14;
            this.nEmptyFields.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(301, 76);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(66, 13);
            this.label4.TabIndex = 13;
            this.label4.Text = "Empty fields:";
            // 
            // cbMirrorHorizontal
            // 
            this.cbMirrorHorizontal.AutoSize = true;
            this.cbMirrorHorizontal.Location = new System.Drawing.Point(348, 44);
            this.cbMirrorHorizontal.Name = "cbMirrorHorizontal";
            this.cbMirrorHorizontal.Size = new System.Drawing.Size(32, 17);
            this.cbMirrorHorizontal.TabIndex = 12;
            this.cbMirrorHorizontal.Text = "=";
            this.cbMirrorHorizontal.UseVisualStyleBackColor = true;
            // 
            // cbMirrorVertical
            // 
            this.cbMirrorVertical.AutoSize = true;
            this.cbMirrorVertical.Location = new System.Drawing.Point(304, 44);
            this.cbMirrorVertical.Name = "cbMirrorVertical";
            this.cbMirrorVertical.Size = new System.Drawing.Size(30, 17);
            this.cbMirrorVertical.TabIndex = 11;
            this.cbMirrorVertical.Text = "||";
            this.cbMirrorVertical.UseVisualStyleBackColor = true;
            // 
            // cbDiagonalMoves
            // 
            this.cbDiagonalMoves.AutoSize = true;
            this.cbDiagonalMoves.Location = new System.Drawing.Point(304, 19);
            this.cbDiagonalMoves.Name = "cbDiagonalMoves";
            this.cbDiagonalMoves.Size = new System.Drawing.Size(102, 17);
            this.cbDiagonalMoves.TabIndex = 10;
            this.cbDiagonalMoves.Text = "Diagonal moves";
            this.cbDiagonalMoves.UseVisualStyleBackColor = true;
            // 
            // cbPrint
            // 
            this.cbPrint.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cbPrint.AutoSize = true;
            this.cbPrint.Location = new System.Drawing.Point(557, 18);
            this.cbPrint.Name = "cbPrint";
            this.cbPrint.Size = new System.Drawing.Size(47, 17);
            this.cbPrint.TabIndex = 9;
            this.cbPrint.Text = "Print";
            this.cbPrint.UseVisualStyleBackColor = true;
            this.cbPrint.CheckedChanged += new System.EventHandler(this.cbPrint_CheckedChanged);
            // 
            // nDeep
            // 
            this.nDeep.Location = new System.Drawing.Point(65, 74);
            this.nDeep.Name = "nDeep";
            this.nDeep.Size = new System.Drawing.Size(90, 20);
            this.nDeep.TabIndex = 8;
            this.nDeep.Value = new decimal(new int[] {
            7,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(23, 76);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(36, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Deep:";
            // 
            // bStop
            // 
            this.bStop.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bStop.Enabled = false;
            this.bStop.Location = new System.Drawing.Point(545, 77);
            this.bStop.Name = "bStop";
            this.bStop.Size = new System.Drawing.Size(75, 23);
            this.bStop.TabIndex = 6;
            this.bStop.Text = "Stop";
            this.bStop.UseVisualStyleBackColor = true;
            this.bStop.Click += new System.EventHandler(this.button3_Click);
            // 
            // bGenerate
            // 
            this.bGenerate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bGenerate.Location = new System.Drawing.Point(545, 48);
            this.bGenerate.Name = "bGenerate";
            this.bGenerate.Size = new System.Drawing.Size(75, 23);
            this.bGenerate.TabIndex = 5;
            this.bGenerate.Text = "Generate";
            this.bGenerate.UseVisualStyleBackColor = true;
            this.bGenerate.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(175, 15);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 4;
            this.button1.Text = "Set fields";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // nFieldsY
            // 
            this.nFieldsY.Location = new System.Drawing.Point(65, 44);
            this.nFieldsY.Name = "nFieldsY";
            this.nFieldsY.Size = new System.Drawing.Size(90, 20);
            this.nFieldsY.TabIndex = 3;
            this.nFieldsY.Value = new decimal(new int[] {
            7,
            0,
            0,
            0});
            // 
            // nFieldsX
            // 
            this.nFieldsX.Location = new System.Drawing.Point(65, 18);
            this.nFieldsX.Name = "nFieldsX";
            this.nFieldsX.Size = new System.Drawing.Size(90, 20);
            this.nFieldsX.TabIndex = 2;
            this.nFieldsX.Value = new decimal(new int[] {
            7,
            0,
            0,
            0});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 46);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Fields Y:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Fields X:";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(632, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newBoardToolStripMenuItem,
            this.loadBoardToolStripMenuItem,
            this.saveBoardToolStripMenuItem,
            this.toolStripMenuItem1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newBoardToolStripMenuItem
            // 
            this.newBoardToolStripMenuItem.Name = "newBoardToolStripMenuItem";
            this.newBoardToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.newBoardToolStripMenuItem.Text = "New board";
            this.newBoardToolStripMenuItem.Click += new System.EventHandler(this.newBoardToolStripMenuItem_Click);
            // 
            // loadBoardToolStripMenuItem
            // 
            this.loadBoardToolStripMenuItem.Name = "loadBoardToolStripMenuItem";
            this.loadBoardToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.loadBoardToolStripMenuItem.Text = "Load board...";
            this.loadBoardToolStripMenuItem.Click += new System.EventHandler(this.loadBoardToolStripMenuItem_Click);
            // 
            // saveBoardToolStripMenuItem
            // 
            this.saveBoardToolStripMenuItem.Name = "saveBoardToolStripMenuItem";
            this.saveBoardToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveBoardToolStripMenuItem.Text = "Save board...";
            this.saveBoardToolStripMenuItem.Click += new System.EventHandler(this.saveBoardToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(149, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // statusBar
            // 
            this.statusBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.statusLabel1,
            this.statusLabelGenerated});
            this.statusBar.Location = new System.Drawing.Point(0, 541);
            this.statusBar.Name = "statusBar";
            this.statusBar.Size = new System.Drawing.Size(632, 22);
            this.statusBar.TabIndex = 2;
            this.statusBar.Text = "statusStrip1";
            // 
            // statusLabel1
            // 
            this.statusLabel1.Name = "statusLabel1";
            this.statusLabel1.Size = new System.Drawing.Size(25, 17);
            this.statusLabel1.Text = "      ";
            // 
            // statusLabelGenerated
            // 
            this.statusLabelGenerated.Name = "statusLabelGenerated";
            this.statusLabelGenerated.Size = new System.Drawing.Size(25, 17);
            this.statusLabelGenerated.Text = "      ";
            // 
            // thrGenerator
            // 
            this.thrGenerator.WorkerSupportsCancellation = true;
            this.thrGenerator.DoWork += new System.ComponentModel.DoWorkEventHandler(this.thrGenerator_DoWork);
            this.thrGenerator.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.thrGenerator_RunWorkerCompleted);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 140);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.pBoard);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.tbResult);
            this.splitContainer1.Size = new System.Drawing.Size(632, 401);
            this.splitContainer1.SplitterDistance = 419;
            this.splitContainer1.TabIndex = 4;
            // 
            // pBoard
            // 
            this.pBoard.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.pBoard.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pBoard.Location = new System.Drawing.Point(0, 0);
            this.pBoard.Name = "pBoard";
            this.pBoard.Size = new System.Drawing.Size(419, 401);
            this.pBoard.TabIndex = 4;
            // 
            // tbResult
            // 
            this.tbResult.AcceptsReturn = true;
            this.tbResult.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.tbResult.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbResult.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.tbResult.Location = new System.Drawing.Point(0, 0);
            this.tbResult.Multiline = true;
            this.tbResult.Name = "tbResult";
            this.tbResult.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tbResult.Size = new System.Drawing.Size(209, 401);
            this.tbResult.TabIndex = 0;
            // 
            // openFD
            // 
            this.openFD.DefaultExt = "dat";
            this.openFD.FileName = "board.dat";
            this.openFD.Filter = "Board files (*.dat)|*.dat";
            this.openFD.InitialDirectory = ".";
            this.openFD.Title = "Select board file to load";
            // 
            // saveFD
            // 
            this.saveFD.DefaultExt = "dat";
            this.saveFD.FileName = "board.dat";
            this.saveFD.Filter = "Board files (*.dat)|*.dat";
            this.saveFD.InitialDirectory = ".";
            this.saveFD.Title = "Select file to save";
            // 
            // cbHashCheck
            // 
            this.cbHashCheck.AutoSize = true;
            this.cbHashCheck.Location = new System.Drawing.Point(175, 52);
            this.cbHashCheck.Name = "cbHashCheck";
            this.cbHashCheck.Size = new System.Drawing.Size(84, 17);
            this.cbHashCheck.TabIndex = 16;
            this.cbHashCheck.Text = "Hash check";
            this.cbHashCheck.UseVisualStyleBackColor = true;
            this.cbHashCheck.CheckedChanged += new System.EventHandler(this.cbHashCheck_CheckedChanged);
            // 
            // cbMD5CRC
            // 
            this.cbMD5CRC.AutoSize = true;
            this.cbMD5CRC.Enabled = false;
            this.cbMD5CRC.Location = new System.Drawing.Point(175, 77);
            this.cbMD5CRC.Name = "cbMD5CRC";
            this.cbMD5CRC.Size = new System.Drawing.Size(83, 17);
            this.cbMD5CRC.TabIndex = 17;
            this.cbMD5CRC.Text = "MD5 + CRC";
            this.cbMD5CRC.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(632, 563);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.statusBar);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Hermit Level Generator";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nEmptyFields)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nDeep)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nFieldsY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nFieldsX)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusBar.ResumeLayout(false);
            this.statusBar.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.NumericUpDown nFieldsY;
        private System.Windows.Forms.NumericUpDown nFieldsX;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button bGenerate;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusBar;
        private System.ComponentModel.BackgroundWorker thrGenerator;
        private System.Windows.Forms.Button bStop;
        private System.Windows.Forms.NumericUpDown nDeep;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.CheckBox cbPrint;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Panel pBoard;
        private System.Windows.Forms.ToolStripMenuItem newBoardToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadBoardToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveBoardToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.OpenFileDialog openFD;
        private System.Windows.Forms.SaveFileDialog saveFD;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel1;
        private System.Windows.Forms.TextBox tbResult;
        private System.Windows.Forms.ToolStripStatusLabel statusLabelGenerated;
        private System.Windows.Forms.CheckBox cbMirrorHorizontal;
        private System.Windows.Forms.CheckBox cbMirrorVertical;
        private System.Windows.Forms.CheckBox cbDiagonalMoves;
        private System.Windows.Forms.NumericUpDown nEmptyFields;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox cbMirrorCenter;
        private System.Windows.Forms.CheckBox cbHashCheck;
        private System.Windows.Forms.CheckBox cbMD5CRC;
    }
}

