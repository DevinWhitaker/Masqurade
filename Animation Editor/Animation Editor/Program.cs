using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Animation_Editor
{
    static class Program
    {
        //DateTime now;
        static DateTime gameTime;
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            Form1 theEditor = new Form1();

            theEditor.Show();
            
            while(theEditor.Looping)
            {
                DateTime now = System.DateTime.Now;
                TimeSpan elapsed = now - gameTime;
                gameTime = now;

                theEditor.Update((float)elapsed.TotalSeconds);
                theEditor.Render();
                Application.DoEvents();
            }
        }
    }
}
