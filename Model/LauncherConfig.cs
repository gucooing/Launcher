using CommunityToolkit.Mvvm.ComponentModel;
using Newtonsoft.Json;
using Panuon.WPF;
using System.Collections.ObjectModel;
using System.IO;
using static Org.BouncyCastle.Math.EC.ECCurve;

namespace Launcher.Model
{
    public enum ProxyType
    {
        OFFICIAL,
        PRIVATE,
        PROXY_ONLY
    }


    public partial class LauncherConfig : ObservableObject
    {



        public static LauncherConfig Load(string file)
        {
            if (File.Exists(file))
            {
                var content = File.ReadAllText(file);
                return JsonConvert.DeserializeObject<LauncherConfig>(content) ?? new LauncherConfig();
            }
            else
            {
                return new LauncherConfig();
            }
        }

        public void Save(string file)
        {
            File.WriteAllText(file, JsonConvert.SerializeObject(this));
        }



        //public GameInfo GameInfo { get; set; }
        //public ProxyType ProxyType { get; set; }
        //public ObservableCollection<ServerItem> Servers { get; set; }

        public bool DebugMode { get; set; }

        public string Language { get; set; }


        public string BgUrl { get; set; } = "https://i0.hdslb.com/bfs/new_dyn/be2c9d48dfa1e57161609e693b48982a401742377.png";

        private GameInfo gameInfo;

        private ProxyType pType;

        public ProxyType ProxyType
        {
            get { return pType; }
            set { SetProperty(ref pType, value); }
        }


        public GameInfo GameInfo
        {
            get { return gameInfo; }
            set { SetProperty(ref gameInfo, value); }
        }

        private int selectedSrvIndex;

        public int SelectedSrvIndex
        {
            get { return selectedSrvIndex; }
            set { SetProperty(ref  selectedSrvIndex , value); }
        }


        private ObservableCollection<ServerItem> servers = new ObservableCollection<ServerItem>();

        public ObservableCollection<ServerItem> Servers
        {
            get { return servers; }
            set { SetProperty(ref servers, value); }
        }




    }
}
