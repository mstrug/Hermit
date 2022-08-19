using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using HermitLevelEditor;

namespace HermitLevelEditor
{
    public partial class FormLevelGroupProperties : Form
    {
        LevelGroup iLG;

        public FormLevelGroupProperties( ref LevelGroup aLG )
        {
            InitializeComponent();

            SetValues(ref aLG);

            iLG = aLG;
        }

        private void SetValues(ref LevelGroup aLG)
        {
            nID.Value = (decimal)aLG.iLevelGroupId;
            tbLevelsCount.Text = aLG.iLevelCount.ToString();
        }

        private void SetLevelGroup(ref LevelGroup aLG)
        {
            aLG.iLevelGroupId = (UInt16)nID.Value;
        }

        private void bOk_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;

            SetLevelGroup(ref iLG);

            Close();
        }

    }
}