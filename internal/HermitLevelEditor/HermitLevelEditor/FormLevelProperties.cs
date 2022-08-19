using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HermitLevelEditor
{
    public partial class FormLevelProperties : Form
    {
        Level iLevelRef;

        public FormLevelProperties()
        {
            InitializeComponent();

            cbFieldShape.SelectedIndex = 0;
            iLevelRef = null;
        }

        public FormLevelProperties( int aLevelId )
        {
            InitializeComponent();

            cbFieldShape.SelectedIndex = 0;
            nLevelId.Value = (decimal)aLevelId;
            iLevelRef = null;
        }

        public FormLevelProperties(Level aLevel)
        {
            InitializeComponent();

            cbFieldShape.SelectedIndex = aLevel.iFieldShape;
            nLevelId.Value = (decimal)aLevel.iID;
            nWidth.Value = (decimal)aLevel.iWidth;
            nWidth.Enabled = false;
            nHeight.Value = (decimal)aLevel.iHeight;
            nHeight.Enabled = false;
            cDiagonalMoves.Checked = aLevel.iDiagonalMoves;

            iLevelRef = aLevel;
        }

        public void SetImport()
        {
            nWidth.Enabled = false;
            nWidth.Value = 0;
            nHeight.Enabled = false;
            nHeight.Value = 0;
            cDiagonalMoves.Enabled = false;
        }

        private void bOk_Click(object sender, EventArgs e)
        {
            if (cbFieldShape.SelectedIndex != 0)
            {
                return;
            }
            DialogResult = DialogResult.OK;

            if (iLevelRef != null)
            {
                iLevelRef.iID = GetId();
                iLevelRef.iDiagonalMoves = GetDiagonalMoves();
                iLevelRef.iFieldShape = GetFieldShape();
            }
        }

        public Byte GetWidth()
        {
            return (Byte)nWidth.Value;
        }

        public Byte GetHeight()
        {
            return (Byte)nHeight.Value;
        }

        public Byte GetId()
        {
            return (Byte)nLevelId.Value;
        }

        public Byte GetFieldShape()
        {
            return (Byte)cbFieldShape.SelectedIndex;
        }

        public Boolean GetDiagonalMoves()
        {
            return cDiagonalMoves.Checked;
        }

    }
}