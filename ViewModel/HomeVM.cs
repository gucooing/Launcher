using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Launcher.Model;
using System.Diagnostics;
using System;
using System.IO;
using System.Windows;
using Launcher.Common;
using static Launcher.Common.ProxyHelper;
using System.Net.Http.Headers;
using Launcher.Control;
using System.Windows.Input;
using System.Linq;
using static System.Net.Mime.MediaTypeNames;
using Newtonsoft.Json;
using System.Text;

namespace Launcher.ViewModel
{
    internal class HomeVM : ObservableObject
    {
        public static HomeVM Instacne = new HomeVM();

        public LauncherConfig LauncherConfig
        {
            get { return App.launcherConfig; }
            set { SetProperty(ref App.launcherConfig, value); }
        }

        public HomeVM()
        {
            if (LauncherConfig == null)
            {
                LauncherConfig = new LauncherConfig();

                LauncherConfig.Servers.Add(new ServerItem()
                {
                    Name = "TestServer",
                    Description = "Styles are the visual aspects of a UI that give it a distinct look and feel.",

                    proxy = new ProxyConfig("127.0.0.1", true, "25566")
                });
            }
        }

        public RelayCommand StartGameCommand => new RelayCommand(StartGame);

        public ProxyController proxyController;

        private string startBtn_txt = Properties.Resources.btn_startgame;

        public string StartBtn_txt
        {
            get { return startBtn_txt; }
            set { SetProperty(ref startBtn_txt, value); }
        }
        bool CanStart = true;

        private bool canChangeProxyType = true;

        public bool CanChangeProxyType
        {
            get { return canChangeProxyType; }
            set { SetProperty(ref canChangeProxyType,value); }
        }

        #region 启动游戏

        private void StartGame()
        {

            if (!CanStart)
            {

                SnackBar.Show(Properties.Resources.tip_nostart,null);

                return;
            }

            if (LauncherConfig.GameInfo==null||
                LauncherConfig.GameInfo.GameExePath == null )
                //LauncherConfig.GameInfo.Version == null
                
            {
                SnackBar.Show(Properties.Resources.tip_setcfg,new RelayCommand(() =>
                {
                    //MainWindow.Instance.rootFrame.Navigate(new Uri("/View/Setting.xaml", UriKind.Relative));
                    MainWindow.Instance.nav.SelectedIndex = 1;
                }));
                return;
            }
            var fp = LauncherConfig.GameInfo.GameExePath;
            try
            {

                fp = Path.GetFullPath(fp);
            }
            catch (Exception ex)
            {
                SnackBar.Show(ex.Message, new RelayCommand(() =>
                {
                    MainWindow.Instance.nav.SelectedIndex = 1;
                }));
                return;
            }
            var fd = LauncherConfig.GameInfo.GameExeFolder;
            if (!File.Exists(fp)&&LauncherConfig.ProxyType!=ProxyType.PROXY_ONLY)
            {

                SnackBar.Show(Properties.Resources.tip_correctgamepath, new RelayCommand(() =>
                {
                    //MainWindow.Instance.rootFrame.Navigate(new Uri("/View/Setting.xaml", UriKind.Relative));
                    MainWindow.Instance.nav.SelectedIndex = 1;
                }));
                return;
            }


            switch (LauncherConfig.ProxyType)
            {
                case ProxyType.OFFICIAL:
                    ProcessStartInfo startInfo = new ProcessStartInfo()
                    {
                        CreateNoWindow = true,
                        FileName = fp,
                        UseShellExecute = true,
                    };
                    try
                    {
                        Process.Start(startInfo);

                        StartBtn_txt = Properties.Resources.btn_running;

                        CanStart = false;

                        CanChangeProxyType = false;

                        new ProcessWatcher(new EventHandler(pro_Exited)).Watch();


                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                    }
                    break;
                case ProxyType.PRIVATE:
                    {
                        if (LauncherConfig.Servers.Count==0)
                        {
                            SnackBar.Show(Properties.Resources.tip_reqaddone, null);
                            return;
                        }
                        if (SelectedSrv==null)
                        {
                            SnackBar.Show(Properties.Resources.tip_reqselectone, null);
                            return;
                        }
                        var SelectedProxy_C = SelectedSrv.proxy;
                        proxyController = new ProxyController(SelectedProxy_C.ProxyPort, SelectedProxy_C.ProxyServer,SelectedProxy_C.UseHttp);
                        proxyController.Start();



                        var dll = RSAPatchHelper.WriteMhypbaseAllTo(SelectedSrv); ;
                        if (dll != null)
                        {
                            GameHelper.StartGame(fp, dll);
                        }
                        CanChangeProxyType = false;
                        CanStart = false;
                        StartBtn_txt = Properties.Resources.btn_running;
                        new ProcessWatcher(new EventHandler(pro_Exited)).Watch();

                        break;
                    }
                case ProxyType.PROXY_ONLY:
                    {
                        if (LauncherConfig.Servers.Count == 0||SelectedSrv==null)
                        {
                            SnackBar.Show(Properties.Resources.tip_reqaddone, null);
                            return;
                        }
                        var SelectedProxy_C = SelectedSrv.proxy;


                        if (proxyController!=null&&proxyController._IsRun)
                        {
                            CanChangeProxyType = true;

                            proxyController.Stop();

                            StartBtn_txt = Properties.Resources.btn_startproxy;
                        }
                        else
                        {
                            CanChangeProxyType = false;

                            proxyController = new ProxyController(SelectedProxy_C.ProxyPort, SelectedProxy_C.ProxyServer, SelectedProxy_C.UseHttp);

                            proxyController.Start();

                            StartBtn_txt = Properties.Resources.btn_stopproxy;

                        }
                    }



                    break;
                default:
                    break;
            }
        }


        private void pro_Exited(object sender, EventArgs e)
        {
            //MessageBox.Show("游戏退出！");


            CanStart = true;

            switch (LauncherConfig.ProxyType)
            {
                case ProxyType.OFFICIAL:
                    StartBtn_txt = Properties.Resources.btn_startgame;

                    CanChangeProxyType = true;
                    break;
                case ProxyType.PRIVATE:

                    StartBtn_txt = Properties.Resources.btn_startgame;

                    proxyController.Stop();

                    CanChangeProxyType = true;

                    //var inif = System.IO.Path.Combine(LauncherConfig.GameInfo.GameExeFolder, "mhypbase.ini");

                    //if (File.Exists(inif))
                    //{
                    //    File.Delete(inif);
                    //}

                    break;
                case ProxyType.PROXY_ONLY:

                default:
                    break;
            }
        }
        #endregion

        public ICommand AddCommand => new RelayCommand(Add);
        public ICommand DeleteCommand => new RelayCommand(Delete);
        public ICommand EditCommand => new RelayCommand(Edit);
        public ICommand ImportCommand => new RelayCommand(Import);
        public ICommand ExportCommand => new RelayCommand(Export);

        private void Export()
        {
            if (SelectedSrv!=null)
            {
                byte[] bytes = Encoding.Default.GetBytes(JsonConvert.SerializeObject(SelectedSrv));
                string str = Convert.ToBase64String(bytes);
                Clipboard.SetDataObject(str, true);
                SnackBar.Show(Properties.Resources.tip_exp_succ, null);

            }
        }

        private void Import()
        {
            try
            {
                IDataObject data = Clipboard.GetDataObject();
                string srv = (string)data.GetData(typeof(string));
                byte[] outputb = Convert.FromBase64String(srv);
                string orgStr = Encoding.Default.GetString(outputb);
                LauncherConfig.Servers.Add(JsonConvert.DeserializeObject<ServerItem>(orgStr));

                SnackBar.Show(Properties.Resources.tip_imp_succ, null);

            }
            catch (Exception ex)
            {
                SnackBar.Show(Properties.Resources.tip_imp_err, null);
            }
        }

        private ServerItem selectedSrv;

        public ServerItem SelectedSrv
        {
            get { return selectedSrv; }
            set { SetProperty(ref  selectedSrv , value); }
        }



        private void Add()
        {

            var item = new ServerItem()
            {
                proxy = new ProxyConfig("127.0.0.1")
            };

            ServerEditControl.instance.ServerItem = item;
            ServerEditControl.Show();

            LauncherConfig.Servers.Add(item);


        }

        private void Delete()
        {
            if (SelectedSrv==null)
            {
                return;
            }
            LauncherConfig.Servers.Remove(SelectedSrv);

            if (LauncherConfig.SelectedSrvIndex>0)
            {
                LauncherConfig.SelectedSrvIndex -= 1;
            }

        }

        private void Edit()
        {
            if (SelectedSrv == null)
            {
                return;
            }

            ServerEditControl.instance.ServerItem = SelectedSrv;
            ServerEditControl.Show();

        }


    }
}
