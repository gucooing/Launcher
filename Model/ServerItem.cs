namespace Launcher.Model
{
    public class ServerItem
    {
        public string Name { get; set; }
        public string Description { get; set; }

        public ProxyConfig proxy { get; set; }

        public string RSAPublicKey { get; set; }

        public string RSAPrivateKey { get; set; }
    }
}
