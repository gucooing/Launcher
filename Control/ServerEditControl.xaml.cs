using Launcher.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Launcher.Control
{
    /// <summary>
    /// ServerEditControl.xaml 的交互逻辑
    /// </summary>
    public partial class ServerEditControl : UserControl
    {
        public static ServerEditControl instance;

        public static void Show()
        {
            instance.Visibility =Visibility.Visible;
        }

        public ServerEditControl()
        {
            InitializeComponent();
            instance = this;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Visibility = Visibility.Collapsed;

        }

        public ServerItem ServerItem
        {
            get { return (ServerItem)GetValue(ServerItemProperty); }
            set { SetValue(ServerItemProperty, value); }
        }

        // Using a DependencyProperty as the backing store for ServerItem.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ServerItemProperty =
            DependencyProperty.Register("ServerItem", typeof(ServerItem), typeof(ServerEditControl), new PropertyMetadata(null));

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {


            Visibility = Visibility.Collapsed;

        }
    }
}
