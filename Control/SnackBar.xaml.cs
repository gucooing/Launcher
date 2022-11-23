using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Launcher.Control
{
    /// <summary>
    /// SnackBar.xaml 的交互逻辑
    /// </summary>
    public partial class SnackBar : UserControl
    {
        public SnackBar()
        {
            InitializeComponent();
        }

        public static void Show(string Msg, ICommand Command, string ActionStr = "确定")
        {
            var sb = new SnackBar();
            sb.Command = Command;
            sb.ActionStr = ActionStr;
            sb.Msg = Msg;

            MainWindow.Instance.sb_container.Children.Add(sb);
        }




        public string Msg
        {
            get { return (string)GetValue(MsgProperty); }
            set { SetValue(MsgProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Msg.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty MsgProperty =
            DependencyProperty.Register("Msg", typeof(string), typeof(SnackBar), new PropertyMetadata("这是一个 Snack Bar", (s, e) =>
            {
                var bar = s as SnackBar;
                if (bar != null)
                    bar.msg_tb.Text = e.NewValue.ToString();

            }));



        public string ActionStr
        {
            get { return (string)GetValue(ActionStrProperty); }
            set { SetValue(ActionStrProperty, value); }
        }

        // Using a DependencyProperty as the backing store for ActionStr.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ActionStrProperty =
            DependencyProperty.Register("ActionStr", typeof(string), typeof(SnackBar), new PropertyMetadata("确定", (s, e) =>
            {
                var bar = s as SnackBar;
                if (bar != null)
                    bar.action_btn.Content = e.NewValue.ToString();

            }));

        private void action_btn_Click(object sender, RoutedEventArgs e)
        {
            if (Command != null)
            {
                Command.Execute(null);

            }

            this.Visibility = Visibility.Collapsed;

            StackPanel stackPanel = this.Parent as StackPanel;
            if (stackPanel != null)
            {
                stackPanel.Children.Remove(this);
            }
        }




        public ICommand Command
        {
            get { return (ICommand)GetValue(CommandProperty); }
            set { SetValue(CommandProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Command.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty CommandProperty =
            DependencyProperty.Register("Command", typeof(ICommand), typeof(SnackBar), new PropertyMetadata(null));


    }
}
