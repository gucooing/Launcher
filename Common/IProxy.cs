using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Launcher.Common
{
    public interface IProxyController
    {
        bool _IsRun { get; set; }

        void Start();

        void Stop();
    }
}
