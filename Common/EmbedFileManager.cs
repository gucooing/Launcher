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

    [Obsolete]
    public static class RawFileHelper
    {
        public static byte[] GetKey(string file)
        {
            Stream sr = null; ;
            try
            {
                var _assembly = Assembly.GetExecutingAssembly();//获取当前执行代码的程序集
                sr = _assembly.GetManifestResourceStream($"Launcher.key.{file}");

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

    public static class mhypbaseHelper
    {
        public static string WriteMhypbaseAllTo(string path1, Model.ServerItem item)
        {
            WriteDllTo(path1);
            WriteInITo(path1, item);
            return Path.Combine(path1, "mhypbase.cr.dll");
        }


        public static void WriteDllTo(string path1)
        {

            try
            {
                //EmbedFileManager.ExtractFile("mhypbase.mhypbase.cr.dll",file);
                EmbedFileManager.ExtractFile("mhypbase.mhypbase.cr.dll", Path.Combine(".\\", "mhypbase.cr.dll"));

            }
            catch
            {
                throw;
            }

        }

        public static void WriteInITo(string path1, Model.ServerItem item)
        {
            try
            {
                var inif = Path.Combine(path1, "mhypbase.ini");

                if (File.Exists(inif))
                {
                    File.Delete(inif);
                }
                EmbedFileManager.ExtractFile("mhypbase.mhypbase.ini", inif);

                if (App.launcherConfig.DebugMode)
                {
                    IniHelper.INIWrite("Basic", "EnableConsole", "true", inif);
                }



                if (!string.IsNullOrEmpty(item.RSAPrivateKey))
                {
                    IniHelper.INIWrite("Value", "RSAPrivateKey", item.RSAPrivateKey, inif);

                }
                if (!string.IsNullOrEmpty(item.RSAPublicKey))
                {
                    IniHelper.INIWrite("Value", "RSAPublicKey", item.RSAPublicKey, inif);
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
