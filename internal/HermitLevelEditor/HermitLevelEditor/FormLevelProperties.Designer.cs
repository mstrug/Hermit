namespace HermitLevelEditor
{
    partial class FormLevelProperties
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
            this.bOk = new System.Windows.Forms.Button();
            this.bCancel = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.nLevelId = new System.Windows.Forms.NumericUpDown();
            this.nWidth = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.nHeight = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.cbFieldShape = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.cDiagonalMoves = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.nLevelId)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nHeight)).BeginInit();
            this.SuspendLayout();
            // 
            // bOk
            // 
            this.bOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bOk.Location = new System.Drawing.Point(126, 254);
            this.bOk.Name = "bOk";
            this.bOk.Size = new System.Drawing.Size(75, 23);
            this.bOk.TabIndex = 0;
            this.bOk.Text = "&Ok";
            this.bOk.UseVisualStyleBackColor = true;
            this.bOk.Click += new System.EventHandler(this.bOk_Click);
            // 
            // bCancel
            // 
            this.bCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.bCancel.Location = new System.Drawing.Point(207, 254);
            this.bCancel.Name = "bCancel";
            this.bCancel.Size = new System.Drawing.Size(75, 23);
            this.bCancel.TabIndex = 1;
            this.bCancel.Text = "&Cancel";
            this.bCancel.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(76, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Level ID (hex):";
            // 
            // nLevelId
            // 
            this.nLevelId.Hexadecimal = true;
            this.nLevelId.Location = new System.Drawing.Point(94, 19);
            this.nLevelId.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nLevelId.Name = "nLevelId";
            this.nLevelId.Size = new System.Drawing.Size(75, 20);
            this.nLevelId.TabIndex = 3;
            // 
            // nWidth
            // 
            this.nWidth.Location = new System.Drawing.Point(94, 67);
            this.nWidth.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nWidth.Name = "nWidth";
            this.nWidth.Size = new System.Drawing.Size(75, 20);
            this.nWidth.TabIndex = 5;
            this.nWidth.Value = new decimal(new int[] {
            7,
            0,
            0,
            0});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 70);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Width (fields):";
            // 
            // nHeight
            // 
            this.nHeight.Location = new System.Drawing.Point(94, 93);
            this.nHeight.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nHeight.Name = "nHeight";
            this.nHeight.Size = new System.Drawing.Size(75, 20);
            this.nHeight.TabIndex = 7;
            this.nHeight.Value = new decimal(new int[] {
            7,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 96);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(74, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Height (fields):";
            // 
            // cbFieldShape
            // 
            this.cbFieldShape.FormattingEnabled = true;
            this.cbFieldShape.Items.AddRange(new object[] {
            "Square",
            "Diamond"});
            this.cbFieldShape.Location = new System.Drawing.Point(94, 141);
            this.cbFieldShape.Name = "cbFieldShape";
            this.cbFieldShape.Size = new System.Drawing.Size(101, 21);
            this.cbFieldShape.TabIndex = 8;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(14, 144);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(64, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Field shape:";
            // 
            // cDiagonalMoves
            // 
            this.cDiagonalMoves.AutoSize = true;
            this.cDiagonalMoves.Location = new System.Drawing.Point(17, 188);
            this.cDiagonalMoves.Name = "cDiagonalMoves";
            this.cDiagonalMoves.Size = new System.Drawing.Size(102, 17);
            this.cDiagonalMoves.TabIndex = 10;
            this.cDiagonalMoves.Text = "Diagonal moves";
            this.cDiagonalMoves.UseVisualStyleBackColor = true;
            // 
            // FormLevelProperties
            // 
            this.AcceptButton = this.bOk;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.bCancel;
            this.ClientSize = new System.Drawing.Size(294, 289);
            this.Controls.Add(this.cDiagonalMoves);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.cbFieldShape);
            this.Controls.Add(this.nHeight);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.nWidth);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.nLevelId);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.bCancel);
            this.Controls.Add(this.bOk);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormLevelProperties";
            this.Text = "Level Properties";
            ((System.ComponentModel.ISupportInitialize)(this.nLevelId)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nHeight)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button bOk;
        private System.Windows.Forms.Button bCancel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nLevelId;
        private System.Windows.Forms.NumericUpDown nWidth;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown nHeight;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cbFieldShape;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox cDiagonalMoves;
    }
}