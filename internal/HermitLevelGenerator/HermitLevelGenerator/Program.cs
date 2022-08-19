using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace HermitLevelGenerator
{
    static class Program
    {
        //[DllImport("kernel32.dll")]
        //static extern bool AttachConsole(int dwProcessId);
        //private const int ATTACH_PARENT_PROCESS = -1;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            //AttachConsole(ATTACH_PARENT_PROCESS);
            //Console.WriteLine("This is from the main program");

            Application.Run(new Form1());
        }
    }
}