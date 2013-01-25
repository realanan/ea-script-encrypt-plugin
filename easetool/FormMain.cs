using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
using System.Security.Cryptography;

namespace easetool
{
    public partial class FormMain : Form
    {
        [DllImport("user32.dll")]
        private static extern bool ReleaseCapture();
        [DllImport("user32.dll")]
        private static extern bool SendMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);

        private const int WM_SYSCOMMAND = 0x0112;
        private const int SC_MOVE = 0xF010;
        private const int HTCAPTION = 0x0002;

        private bool isWorking = false;
        private byte[] key = null;
        
        public FormMain()
        {
            InitializeComponent();

            lblMode.Left = (ClientSize.Width - lblMode.Width) / 2;
            lblDrop.Left = (ClientSize.Width - lblDrop.Width) / 2;

            picProgress.Left = (ClientSize.Width - picProgress.Width) / 2;
            picProgress.Image = Resource.pbrone;

            pbrProgress.Left = 0;
            pbrProgress.Width = ClientSize.Width;
            pbrProgress.Top = ClientSize.Height - pbrProgress.Height;

            mnuEncrypt.Checked = true;
            mnuDecrypt.Checked = false;

            pbrProgress.Visible = false;
        }

        private void FormMain_MouseDown(object sender, MouseEventArgs e)
        {
            ReleaseCapture();
            SendMessage(Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }

        private void FormMain_DragEnter(object sender, DragEventArgs e)
        {
            if (isWorking == false && e.Data.GetDataPresent(DataFormats.FileDrop)) e.Effect = DragDropEffects.Link;
        }

        private void FormMain_DragDrop(object sender, DragEventArgs e)
        {
            if (isWorking) return;
            if (key == null)
            {
                if (!SetKey()) return;
            }
            string[] fileLists = (string[])e.Data.GetData(DataFormats.FileDrop);

            isWorking = true;
            pbrProgress.Maximum = 0;
            pbrProgress.Value = 0;
            pbrProgress.Visible = true;
            picProgress.Image = Resource.pbranim;

            if (mnuEncrypt.Checked)
            {
                DoEncryptDelegate doEncrypt = new DoEncryptDelegate(DoEncrypt);
                doEncrypt.BeginInvoke(fileLists, new AsyncCallback(DoEncryptCallback), doEncrypt);
            }
            else
            {
                DoDecryptDelegate doDecrypt = new DoDecryptDelegate(DoDecrypt);
                doDecrypt.BeginInvoke(fileLists, new AsyncCallback(DoDecryptCallback), doDecrypt);
            }
        }

        private void mnuEncrypt_Click(object sender, EventArgs e)
        {
            mnuEncrypt.Checked = true;
            mnuDecrypt.Checked = false;
            lblMode.Text = "Encrypt Mode";
        }

        private void mnuDecrypt_Click(object sender, EventArgs e)
        {
            mnuEncrypt.Checked = false;
            mnuDecrypt.Checked = true;
            lblMode.Text = "Decrypt Mode";
        }

        private void mnuExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void mnuPassword_Click(object sender, EventArgs e)
        {
            SetKey();
        }

        private bool SetKey()
        {
            FormKey frm = new FormKey();
            if (frm.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                HMACSHA256 hmac = new HMACSHA256(new byte[] { 0x24, 0xde, 0x14, 0x64, 0x45, 0x78, 0xfa, 0x46, 0x55, 0x9c, 0xd3, 0xf9, 0xfd, 0x02, 0x19, 0x88 });
                key = hmac.ComputeHash(Encoding.Default.GetBytes(frm.Password));
                return true;
            }
            else return false;
        }

        private delegate void DoEncryptDelegate(string[] fileLists);
        private void DoEncrypt(string[] fileLists)
        {
            Rijndael rijndael = RijndaelManaged.Create();
            rijndael.Mode = CipherMode.CBC;
            rijndael.Padding = PaddingMode.Zeros;
            ICryptoTransform ict = rijndael.CreateEncryptor(key, new byte[16]);

            byte[] buf = new byte[1024 * 16];

            int i = 0;
            foreach (string filename in fileLists)
            {
                if (!File.Exists(filename)) continue;
                byte[] encbuf;
                using (FileStream stream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.Read))
                {
                    using (MemoryStream ms = new MemoryStream())
                    {
                        using (CryptoStream cs = new CryptoStream(ms, ict, CryptoStreamMode.Write))
                        {
                            while (true)
                            {
                                int len = stream.Read(buf, 0, buf.Length);
                                if (len == 0) break;
                                cs.Write(buf, 0, len);
                            }
                            cs.FlushFinalBlock();
                            encbuf = ms.ToArray();
                        }
                    }
                }
                using (StreamWriter sw = new StreamWriter(filename, false, Encoding.Default))
                {
                    sw.WriteLine("[EASEP]");
                    sw.Write(Convert.ToBase64String(encbuf));
                }
                SetProgress(fileLists.Length, ++i);
            }
        }

        private void DoEncryptCallback(IAsyncResult ar)
        {
            if (ar.IsCompleted)
            {
                DoEncryptDelegate doEncrypt = (DoEncryptDelegate)ar.AsyncState;
                doEncrypt.EndInvoke(ar);
                WorkDone();
            }
        }

        private delegate void DoDecryptDelegate(string[] fileLists);
        private void DoDecrypt(string[] fileLists)
        {
            Rijndael rijndael = RijndaelManaged.Create();
            rijndael.Mode = CipherMode.CBC;
            rijndael.Padding = PaddingMode.Zeros;
            ICryptoTransform ict = rijndael.CreateDecryptor(key, new byte[16]);

            byte[] buf = new byte[1024 * 16];

            int i = 0;
            foreach (string filename in fileLists)
            {
                if (!File.Exists(filename)) continue;
                byte[] decbuf = null;
                using (StreamReader sr = new StreamReader(filename, Encoding.Default))
                {
                    string line = sr.ReadLine();
                    if (line != "[EASEP]") continue;
                    string data = sr.ReadToEnd();
                    try
                    {
                        decbuf = Convert.FromBase64String(data);
                    }
                    catch
                    {
                        continue;
                    }
                }
                if (decbuf == null) continue;
                using (MemoryStream ms = new MemoryStream(decbuf))
                {
                    using (CryptoStream cs = new CryptoStream(ms, ict, CryptoStreamMode.Read))
                    {
                        using (FileStream stream = new FileStream(filename, FileMode.Create, FileAccess.ReadWrite, FileShare.None))
                        {
                            while (true)
                            {
                                int len = cs.Read(buf, 0, buf.Length);
                                if (len == 0) break;
                                stream.Write(buf, 0, len);
                            }
                        }
                    }
                }
                SetProgress(fileLists.Length, ++i);
            }
        }

        private void DoDecryptCallback(IAsyncResult ar)
        {
            if (ar.IsCompleted)
            {
                DoDecryptDelegate doDecrypt = (DoDecryptDelegate)ar.AsyncState;
                doDecrypt.EndInvoke(ar);
                WorkDone();
            }
        }

        private delegate void WorkDoneDelegate();
        private void WorkDone()
        {
            if (InvokeRequired)
            {
                Invoke(new WorkDoneDelegate(WorkDone), null);
                return;
            }
            isWorking = false;
            pbrProgress.Visible = false;
            picProgress.Image = Resource.pbrone;
        }

        private delegate void SetProgressDelegate(int max, int val);
        private void SetProgress(int max, int val)
        {
            if (InvokeRequired)
            {
                Invoke(new SetProgressDelegate(SetProgress), max, val);
                return;
            }
            pbrProgress.Maximum = max;
            pbrProgress.Value = val;
        }
    }
}
