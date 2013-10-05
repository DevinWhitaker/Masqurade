using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Particle_Editor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new Form1());

            // Make the form
            Form1 theform = new Form1();
            
            //display the form
            theform.Show();
            
            //Start the main program loop
            while(theform.Looping)
            {
                
                //call our forms Update function();
                theform.Update();

                //call our forms Render function();
                theform.Render();

                //handle events
                Application.DoEvents();
            }
        }
    }
}
