using System;
using System.IO;
using System.Reflection;

namespace Launcher.Common
{
    internal class EmbedFileManager
    {
        /// <summary>
        /// 释放内嵌资源至指定位置
        /// </summary>
        /// <param name="resource">嵌入的资源，此参数写作：命名空间.文件夹名.文件名.扩展名</param>
        /// <param name="path">释放到位置</param>
        public static void ExtractFile(string file, string path)
        {
            var resource = $"Launcher.{file}";
            Assembly assembly = Assembly.GetExecutingAssembly();
            BufferedStream input = new BufferedStream(assembly.GetManifestResourceStream(resource));
            FileStream output = new FileStream(path, FileMode.Create);
            byte[] data = new byte[1024];
            int lengthEachRead;
            while ((lengthEachRead = input.Read(data, 0, data.Length)) > 0)
            {
                output.Write(data, 0, lengthEachRead);
            }
            output.Flush();
            output.Close();
        }


    }

    public static class RawFileHelper
    {
        public static byte[] GetKey(string file)
        {
            Stream sr = null; ;
            try
            {
                var _assembly = Assembly.GetExecutingAssembly();//获取当前执行代码的程序集
                sr = _assembly.GetManifestResourceStream($"Launcher.RSAPatch.{file}");

            }
            catch
            {
                //AConsole.e(new Spectre.Console.Markup("访问资源错误"));
                throw;
            }

            return streamToByteArray(sr);
        }

        private static byte[] streamToByteArray(Stream input)
        {
            MemoryStream ms = new MemoryStream();
            input.CopyTo(ms);
            return ms.ToArray();
        }
    }

    public static class RSAPatchHelper
    {
        public static string TempFolder = Path.Combine(System.IO.Path.GetTempPath(),"com.launcher");

        public static string WriteMhypbaseAllTo(Model.ServerItem item)
        {
            if (!Directory.Exists(TempFolder))
            {
                Directory.CreateDirectory(TempFolder);
            }
            else
            {
                var fs = Directory.GetFiles(TempFolder);
                foreach (var f in fs)
                {
                    File.Delete(f);
                }

            }
            var r = WriteDllTo(TempFolder);
            WriteInITo(TempFolder, item);
            return r;
        }

        public static void CleanTemp()
        {
            Directory.Delete(TempFolder, true);
        }

        public static string WriteDllTo(string folder)
        {
            string target_dll = Path.Combine(folder, "rsa.dll");
            try
            {
                EmbedFileManager.ExtractFile("RSAPatch.RSAPatch.dll", target_dll);

            }
            catch
            {
                throw;
            }
            return target_dll;

        }

        public static void WriteInITo(string folder, Model.ServerItem item)
        {
            try
            {
                

                if (App.launcherConfig.DebugMode)
                {
                    // debug
                }



                if (!string.IsNullOrEmpty(item.RSAPrivateKey))
                {
                    File.WriteAllText(Path.Combine(folder,"PrivateKey.txt"), item.RSAPrivateKey);
                }
                if (!string.IsNullOrEmpty(item.RSAPublicKey))
                {
                    File.WriteAllText(Path.Combine(folder, "PublicKey.txt"), item.RSAPublicKey);
                }
                else
                {
                    // use default
                    File.WriteAllBytes(Path.Combine(folder, "PublicKey.txt"), RawFileHelper.GetKey("PublicKey.txt"));
                }


            }
            catch
            {
                throw;
            }

        }

        private static byte[] streamToByteArray(Stream input)
        {
            MemoryStream ms = new MemoryStream();
            input.CopyTo(ms);
            return ms.ToArray();
        }
    }
}
