using CommunityToolkit.Mvvm.Input;
using Launcher.Control;
using Microsoft.Win32;
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows;

namespace Launcher.Common
{
    internal class GameHelper
    {
        [DllImport("InjectorLib.dll", CallingConvention = CallingConvention.Cdecl)]
        extern static bool Begin(string game_path, string dll_path);



        public static void StartGame(string filePath,string target_dll)
        {
            string currentDir = Path.GetDirectoryName(Process.GetCurrentProcess().MainModule.FileName);
            string dll_file = Path.Combine(currentDir, "InjectorLib.dll");
            if (File.Exists(filePath))
            {
                //if (!File.Exists("InjectorLib.dll"))
                //{
                //    EmbedFileManager.ExtractFile("InjectorLib.dll", dll_file);
                //}

                target_dll =Path.Combine(currentDir, target_dll);
                bool r=Begin(filePath, target_dll);

                if (r)
                {
                    SnackBar.Show("Inject Success!",null);
                }
                else
                {
                    SnackBar.Show("Inject Failed!", null);

                }
            }
            else
            {

            }
        }

        public static class GameRegReader
        {
            /// <summary>
            /// 获取游戏目录，是静态方法
            /// </summary>
            /// <returns></returns>
            public static string GetGamePath()
            {
                try
                {
                    string startpath = "";
                    string launcherpath = GetLauncherPath();
                    #region 获取游戏启动路径，和官方配置一致
                    string cfgPath = Path.Combine(launcherpath, "config.ini");
                    if (File.Exists(launcherpath) || File.Exists(cfgPath))
                    {
                        //获取游戏本体路径
                        using (StreamReader reader = new StreamReader(cfgPath))
                        {
                            string[] abc = reader.ReadToEnd().Split(new string[] { "\r\n" }, StringSplitOptions.None);
                            foreach (var item in abc)
                            {
                                //从官方获取更多配置
                                if (item.IndexOf("game_install_path") != -1)
                                {
                                    startpath += item.Substring(item.IndexOf("=") + 1);
                                }
                            }
                        }
                    }
                    byte[] bytearr = Encoding.UTF8.GetBytes(startpath);
                    string path = Encoding.UTF8.GetString(bytearr);
                    return path;
                }
                catch
                {
                    return null;
                }
                #endregion
            }
            /// <summary>
            /// 启动器地址
            /// </summary>
            /// <returns></returns>
            public static string GetLauncherPath()
            {
                try
                {
                    RegistryKey key = Registry.LocalMachine;            //打开指定注册表根
                                                                        //获取官方启动器路径
                    string launcherpath = "";
                    try
                    {
                        launcherpath = key.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\原神").GetValue("InstallPath").ToString();


                    }
                    catch (Exception)
                    {
                        launcherpath = key.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Genshin Impact").GetValue("InstallPath").ToString();

                    }

                    byte[] bytepath = Encoding.UTF8.GetBytes(launcherpath);     //编码转换
                    string path = Encoding.UTF8.GetString(bytepath);
                    return path;

                }
                catch
                {
                    return null;
                }
            }

            public static string GetGameExePath()
            {

                var gamepath = GetGamePath();
                if (gamepath == null)
                {
                    return null;
                }
                var cnpath = gamepath + @"/YuanShen.exe";
                var ospath = gamepath + @"/GenshinImpact.exe";

                if (File.Exists(cnpath))
                {
                    return cnpath;
                }
                else if (File.Exists(ospath))
                {
                    return ospath;
                }
                else
                {
                    return null;
                }
            }


        }

        public static class GameLocalReader
        {

            public static string GetGameExePath()
            {
                string Self = Process.GetCurrentProcess().MainModule.FileName;
                var Folder = Path.GetDirectoryName(Self);
                string ret;
                string cn = Path.Combine(Folder, "YuanShen.exe");
                string os = Path.Combine(Folder, "GenshinImpact.exe");
                if (File.Exists(cn))
                {
                    ret = Path.Combine(Folder, cn);
                }
                else if (File.Exists(os))
                {
                    ret = Path.Combine(Folder, os);

                }
                else
                {
                    throw new Exception("未能从当前路径找到原神游戏文件！");
                }
                return ret;
            }

            public static string GetGameExeFolder()
            {
                return Path.GetDirectoryName(GetGameExePath());
            }


        }
    }
}
