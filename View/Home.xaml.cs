using System.Windows.Controls;

namespace Launcher.View
{
    /// <summary>
    /// Home.xaml 的交互逻辑
    /// </summary>
    public partial class Home : Page
    {
        public Home()
        {
            InitializeComponent();
            DataContext = ViewModel.HomeVM.Instacne;
            pt.SelectedIndex = (int)ViewModel.HomeVM.Instacne.LauncherConfig.ProxyType;
        }

        private void ListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var list = (ListBox)sender;


            ViewModel.HomeVM.Instacne.LauncherConfig.ProxyType = (Model.ProxyType)list.SelectedIndex;

            switch (ViewModel.HomeVM.Instacne.LauncherConfig.ProxyType)
            {
                case Model.ProxyType.OFFICIAL:
                    ViewModel.HomeVM.Instacne.StartBtn_txt = Properties.Resources.btn_startgame;
                    break;
                case Model.ProxyType.PRIVATE:
                    
                    ViewModel.HomeVM.Instacne.StartBtn_txt = Properties.Resources.btn_startgame;

                    break;
                case Model.ProxyType.PROXY_ONLY:
                    ViewModel.HomeVM.Instacne.StartBtn_txt = Properties.Resources.btn_startproxy;

                    break;
                default:
                    break;
            }
        }
    }
}
