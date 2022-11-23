using System.Windows.Controls;

namespace Launcher.View
{
    /// <summary>
    /// Setting.xaml 的交互逻辑
    /// </summary>
    public partial class Setting : Page
    {
        public Setting()
        {
            InitializeComponent();
            DataContext = ViewModel.SettingVM.Instacne;



            //Visibility = Visibility.Hidden;
        }

    }
}
