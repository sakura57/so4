namespace DataEditor
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
            this.listSubEditors = new System.Windows.Forms.ListBox();
            this.groupSubEditors = new System.Windows.Forms.GroupBox();
            this.groupSubEditors.SuspendLayout();
            this.SuspendLayout();
            // 
            // listSubEditors
            // 
            this.listSubEditors.FormattingEnabled = true;
            this.listSubEditors.Location = new System.Drawing.Point(6, 22);
            this.listSubEditors.Name = "listSubEditors";
            this.listSubEditors.Size = new System.Drawing.Size(169, 160);
            this.listSubEditors.TabIndex = 0;
            // 
            // groupSubEditors
            // 
            this.groupSubEditors.Controls.Add(this.listSubEditors);
            this.groupSubEditors.Location = new System.Drawing.Point(12, 12);
            this.groupSubEditors.Name = "groupSubEditors";
            this.groupSubEditors.Size = new System.Drawing.Size(306, 188);
            this.groupSubEditors.TabIndex = 1;
            this.groupSubEditors.TabStop = false;
            this.groupSubEditors.Text = "Sub-Editors";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(482, 211);
            this.Controls.Add(this.groupSubEditors);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "SG7 Editor";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupSubEditors.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox listSubEditors;
        private System.Windows.Forms.GroupBox groupSubEditors;
    }
}

