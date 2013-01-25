using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace easetool
{
    public partial class FormKey : Form
    {
        public string Password
        {
            get;
            private set;
        }

        public FormKey()
        {
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (txtPassword.Text == "")
            {
                MessageBox.Show("Please enter your password!", "Password", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            if (txtPassword.Text != txtPassword2.Text)
            {
                MessageBox.Show("These tow passwords are not the same!", "Password", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }
            Password = txtPassword.Text;
            DialogResult = System.Windows.Forms.DialogResult.OK;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
        }
    }
}
