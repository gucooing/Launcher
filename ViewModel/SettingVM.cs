using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Launcher.Common;
using Launcher.Model;
using Microsoft.Win32;
using System.IO;
using System.Windows;
using System.Windows.Input;

namespace Launcher.ViewModel
{
    internal class SettingVM : ObservableObject
    {
        public static SettingVM Instacne = new SettingVM();


        public SettingVM()
        {
            if (LauncherConfig == null)
            {
                LauncherConfig = new LauncherConfig();
            }
        }


        public LauncherConfig LauncherConfig
        {
            get { return App.launcherConfig; }
            set { SetProperty(ref App.launcherConfig, value); }
        }

        public ICommand SearchGameFileCommand => new RelayCommand(SearchGameFile);

        public ICommand SetGameExePathCommand => new RelayCommand(SetGameExePath);



        public void SearchGameFile()
        {
            LauncherConfig.GameInfo = new GameInfo(GameHelper.GameRegReader.GetGameExePath());
            if (File.Exists(LauncherConfig.GameInfo.GameExePath))
            {
                MessageBox.Show($"已找到位于{LauncherConfig.GameInfo.GameExePath}的游戏文件!");
            }
            else
            {
                MessageBox.Show($"搜索失败，注册表中没有相关信息!");

            }

        }

        private void SetGameExePath()
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "原神游戏程序（YuanShen.exe)|YuanShen.exe|原神游戏程序(GenshinImpact.exe)|GenshinImpact.exe";
            openFileDialog.Multiselect = false;
            if (openFileDialog.ShowDialog() == true)
            {

                LauncherConfig.GameInfo = new GameInfo(openFileDialog.FileName);

            }

        }
    }
}
