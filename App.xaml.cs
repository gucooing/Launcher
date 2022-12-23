using Launcher.Model;
using Launcher.ViewModel;
using Newtonsoft.Json;
using System;
using System.Globalization;
using System.Net;
using System.Threading;
using System.Windows;
using System.Windows.Navigation;

namespace Launcher
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {
//#if DEBUG
//        private CultureInfo cultureOverride = new CultureInfo("en-US");
//#endif
        System.Threading.Mutex mutex;

        protected override void OnStartup(StartupEventArgs e)
        {


            ServicePointManager.ServerCertificateValidationCallback += (s, cert, chain, sslPolicyErrors) => true;


            AppDomain currentDomain = AppDomain.CurrentDomain;
            // 当前作用域出现未捕获异常时，使用MyHandler函数响应事件

#if !DEBUG
            currentDomain.UnhandledException += new UnhandledExceptionEventHandler(MyHandler);

#endif


            //单例
            bool ret;
            mutex = new System.Threading.Mutex(true, "ElectronicNeedleTherapySystem", out ret);


            if (App.launcherConfig == null)
            {

                App.launcherConfig = LauncherConfig.Load("config.json");

                if (!string.IsNullOrEmpty(App.launcherConfig.Language))
                {
                    CultureInfo cultureOverride = new CultureInfo(App.launcherConfig.Language);
                    Thread.CurrentThread.CurrentCulture = cultureOverride;
                    Thread.CurrentThread.CurrentUICulture = cultureOverride;
                }
            }

            if (!ret)
            {
                MessageBox.Show(Launcher.Properties.Resources.tip_alreadyrunning);
                Environment.Exit(0);
            }

            

            base.OnStartup(e);

        }

        static void MyHandler(object sender, UnhandledExceptionEventArgs args)
        {
            Exception e = (Exception)args.ExceptionObject;

            MessageBox.Show(e.Message + "\n" + "请吧程序目录下的 err.log 提交至项目issues！", Launcher.Properties.Resources.tip_crash_title);
            System.IO.File.WriteAllText("err.log", e.Message + JsonConvert.SerializeObject(e));
            Environment.Exit(0);

        }

        protected override void OnLoadCompleted(NavigationEventArgs e)
        {
            base.OnLoadCompleted(e);

        }

        protected override void OnExit(ExitEventArgs e)
        {
            try
            {
                launcherConfig.Save("config.json");

                if (HomeVM.Instacne.proxyController!=null)
                {
                    HomeVM.Instacne.proxyController.Stop();
                }
            }
            catch { }

            base.OnExit(e);
        }

        public static LauncherConfig launcherConfig;
    }
}
