using Launcher.Model;
using Launcher.ViewModel;
using Newtonsoft.Json;
using System;
using System.Net;
using System.Windows;
using System.Windows.Navigation;

namespace Launcher
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {
        System.Threading.Mutex mutex;
        protected override void OnStartup(StartupEventArgs e)
        {
            ServicePointManager.ServerCertificateValidationCallback += (s, cert, chain, sslPolicyErrors) => true;


            AppDomain currentDomain = AppDomain.CurrentDomain;
            // 当前作用域出现未捕获异常时，使用MyHandler函数响应事件
            currentDomain.UnhandledException += new UnhandledExceptionEventHandler(MyHandler);


            //单例
            bool ret;
            mutex = new System.Threading.Mutex(true, "ElectronicNeedleTherapySystem", out ret);

            if (!ret)
            {
                MessageBox.Show("已有一个启动器正在运行！");
                Environment.Exit(0);
            }


            base.OnStartup(e);
        }

        static void MyHandler(object sender, UnhandledExceptionEventArgs args)
        {
            Exception e = (Exception)args.ExceptionObject;
            //Console.WriteLine("UnHandled Exception Caught : " + e.Message);
            //Console.WriteLine("Runtime terminating: {0}", args.IsTerminating);

            MessageBox.Show(e.Message + "\n" + "请吧程序目录下的 err.log 提交至项目issues！", "程序崩溃了！");
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
