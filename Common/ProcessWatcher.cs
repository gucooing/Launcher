using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Launcher.Common
{
    internal class ProcessWatcher
    {
        public Process proc;
        EventHandler pro_Exited;
        public ProcessWatcher(EventHandler pro_Exited)
        {
            this.pro_Exited = pro_Exited;
        }

        public void Watch()
        {
            Process[] ps = Process.GetProcesses();

            foreach (Process p in ps)
            {
                if (p.ProcessName== "GenshinImpact"||p.ProcessName== "YuanShen")
                {
                    proc = p;
                }
            }

            if (proc!=null)
            {
                proc.EnableRaisingEvents = true;
                proc.Exited += new EventHandler(pro_Exited);
            }
        }


    }
}
