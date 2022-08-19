namespace HermitLevelEditor
{
    partial class FormLevelGroupProperties
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
            this.nID = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.tbLevelsCount = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.nID)).BeginInit();
            this.SuspendLayout();
            // 
            // bOk
            // 
            this.bOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bOk.Location = new System.Drawing.Point(206, 69);
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
            this.bCancel.Location = new System.Drawing.Point(287, 69);
            this.bCancel.Name = "bCancel";
            this.bCancel.Size = new System.Drawing.Size(75, 23);
            this.bCancel.TabIndex = 1;
            this.bCancel.Text = "&Cancel";
            this.bCancel.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(23, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "ID (hex):";
            // 
            // nID
            // 
            this.nID.Hexadecimal = true;
            this.nID.Increment = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.nID.Location = new System.Drawing.Point(76, 12);
            this.nID.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.nID.Name = "nID";
            this.nID.Size = new System.Drawing.Size(70, 20);
            this.nID.TabIndex = 5;
            this.nID.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(216, 15);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(65, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Levels cout:";
            // 
            // tbLevelsCount
            // 
            this.tbLevelsCount.BackColor = System.Drawing.SystemColors.Menu;
            this.tbLevelsCount.Enabled = false;
            this.tbLevelsCount.Location = new System.Drawing.Point(287, 12);
            this.tbLevelsCount.Name = "tbLevelsCount";
            this.tbLevelsCount.ReadOnly = true;
            this.tbLevelsCount.Size = new System.Drawing.Size(60, 20);
            this.tbLevelsCount.TabIndex = 7;
            this.tbLevelsCount.Text = "0";
            this.tbLevelsCount.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // FormLevelGroupProperties
            // 
            this.AcceptButton = this.bOk;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.bCancel;
            this.ClientSize = new System.Drawing.Size(374, 104);
            this.Controls.Add(this.tbLevelsCount);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.nID);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.bCancel);
            this.Controls.Add(this.bOk);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormLevelGroupProperties";
            this.Text = "Level Group Properties";
            ((System.ComponentModel.ISupportInitialize)(this.nID)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button bOk;
        private System.Windows.Forms.Button bCancel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nID;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbLevelsCount;
    }
}