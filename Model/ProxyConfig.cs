namespace Launcher.Model
{
    public class ProxyConfig
    {

        public ProxyConfig(string proxyServer, bool usehttp = false, string proxyPort = "25565",bool enable = false)
        {
            ProxyServer = proxyServer;
            UseHttp = usehttp;
            ProxyPort = proxyPort;
            ProxyEnable = enable;
        }

        public string ProxyServer { get; set; }

        public string ProxyPort { get; set; }

        public bool UseHttp { get; set; }
        public bool ProxyEnable { get; internal set; }

    }
}
