namespace easetool
{
    partial class FormMain
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.lblDrop = new System.Windows.Forms.Label();
            this.mnuPopup = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.mnuEncrypt = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuDecrypt = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuStrip1 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuExit = new System.Windows.Forms.ToolStripMenuItem();
            this.pbrProgress = new System.Windows.Forms.ProgressBar();
            this.lblMode = new System.Windows.Forms.Label();
            this.picProgress = new System.Windows.Forms.PictureBox();
            this.mnuPassword = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuStrip2 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuPopup.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picProgress)).BeginInit();
            this.SuspendLayout();
            // 
            // lblDrop
            // 
            this.lblDrop.AutoSize = true;
            this.lblDrop.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.lblDrop.Location = new System.Drawing.Point(24, 17);
            this.lblDrop.Name = "lblDrop";
            this.lblDrop.Size = new System.Drawing.Size(110, 12);
            this.lblDrop.TabIndex = 0;
            this.lblDrop.Text = "Drop Files Here";
            // 
            // mnuPopup
            // 
            this.mnuPopup.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuPassword,
            this.mnuStrip1,
            this.mnuEncrypt,
            this.mnuDecrypt,
            this.mnuStrip2,
            this.mnuExit});
            this.mnuPopup.Name = "mnuPopup";
            this.mnuPopup.Size = new System.Drawing.Size(159, 126);
            // 
            // mnuEncrypt
            // 
            this.mnuEncrypt.Name = "mnuEncrypt";
            this.mnuEncrypt.Size = new System.Drawing.Size(158, 22);
            this.mnuEncrypt.Text = "&Encrypt Mode";
            this.mnuEncrypt.Click += new System.EventHandler(this.mnuEncrypt_Click);
            // 
            // mnuDecrypt
            // 
            this.mnuDecrypt.Name = "mnuDecrypt";
            this.mnuDecrypt.Size = new System.Drawing.Size(158, 22);
            this.mnuDecrypt.Text = "&Decrypt Mode";
            this.mnuDecrypt.Click += new System.EventHandler(this.mnuDecrypt_Click);
            // 
            // mnuStrip1
            // 
            this.mnuStrip1.Name = "mnuStrip1";
            this.mnuStrip1.Size = new System.Drawing.Size(155, 6);
            // 
            // mnuExit
            // 
            this.mnuExit.Name = "mnuExit";
            this.mnuExit.Size = new System.Drawing.Size(158, 22);
            this.mnuExit.Text = "E&xit";
            this.mnuExit.Click += new System.EventHandler(this.mnuExit_Click);
            // 
            // pbrProgress
            // 
            this.pbrProgress.Location = new System.Drawing.Point(0, 109);
            this.pbrProgress.Name = "pbrProgress";
            this.pbrProgress.Size = new System.Drawing.Size(134, 8);
            this.pbrProgress.TabIndex = 3;
            // 
            // lblMode
            // 
            this.lblMode.AutoSize = true;
            this.lblMode.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.lblMode.Location = new System.Drawing.Point(33, 0);
            this.lblMode.Name = "lblMode";
            this.lblMode.Size = new System.Drawing.Size(89, 12);
            this.lblMode.TabIndex = 4;
            this.lblMode.Text = "Encrypt Mode";
            // 
            // picProgress
            // 
            this.picProgress.Location = new System.Drawing.Point(45, 38);
            this.picProgress.Name = "picProgress";
            this.picProgress.Size = new System.Drawing.Size(64, 64);
            this.picProgress.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.picProgress.TabIndex = 2;
            this.picProgress.TabStop = false;
            // 
            // mnuPassword
            // 
            this.mnuPassword.Name = "mnuPassword";
            this.mnuPassword.Size = new System.Drawing.Size(158, 22);
            this.mnuPassword.Text = "Set &Password";
            this.mnuPassword.Click += new System.EventHandler(this.mnuPassword_Click);
            // 
            // mnuStrip2
            // 
            this.mnuStrip2.Name = "mnuStrip2";
            this.mnuStrip2.Size = new System.Drawing.Size(155, 6);
            // 
            // FormMain
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(156, 118);
            this.ContextMenuStrip = this.mnuPopup;
            this.ControlBox = false;
            this.Controls.Add(this.lblMode);
            this.Controls.Add(this.pbrProgress);
            this.Controls.Add(this.picProgress);
            this.Controls.Add(this.lblDrop);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Name = "FormMain";
            this.TopMost = true;
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.FormMain_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.FormMain_DragEnter);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.FormMain_MouseDown);
            this.mnuPopup.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.picProgress)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblDrop;
        private System.Windows.Forms.ContextMenuStrip mnuPopup;
        private System.Windows.Forms.ToolStripMenuItem mnuExit;
        private System.Windows.Forms.PictureBox picProgress;
        private System.Windows.Forms.ProgressBar pbrProgress;
        private System.Windows.Forms.ToolStripMenuItem mnuEncrypt;
        private System.Windows.Forms.ToolStripMenuItem mnuDecrypt;
        private System.Windows.Forms.ToolStripSeparator mnuStrip1;
        private System.Windows.Forms.Label lblMode;
        private System.Windows.Forms.ToolStripMenuItem mnuPassword;
        private System.Windows.Forms.ToolStripSeparator mnuStrip2;

    }
}

