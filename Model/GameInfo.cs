using System.IO;

namespace Launcher.Model
{
    //public enum GameVer
    //{
    //    OSRELWin3_2_0,
    //    CNRELWin3_2_0
    //}

    public class GameInfo
    {
        public string GameExeFolder
        {
            get
            {
                return Path.GetDirectoryName(GameExePath);
            }
        }

        public string GetGameDataFolder()
        {
            var dataDirName = string.Empty;
            switch (GetGameType())
            {
                case GameType.CN:
                    dataDirName = "YuanShen_Data";
                    break;
                case GameType.OS:
                    dataDirName = "GenshinImpact_Data";

                    break;
                default:
                    break;
            }
            return Path.Combine(GameExeFolder, dataDirName);
        }

        //public GameVer? Version { get; set; }

        public GameInfo(string gameExePath)
        {
            GameExePath = gameExePath;

            //Version = null;
            //GameExeFolder = Path.GetDirectoryName(gameExePath);


        }



        public enum GameType
        {
            CN,
            OS,
            UnKnown,
        }

        public GameType GetGameType()
        {
            GameType gameType = GameType.UnKnown;

            if (Directory.Exists(Path.Combine(GameExeFolder, "YuanShen_Data")))
            {
                gameType = GameType.CN;
            }
            else if (Directory.Exists(Path.Combine(GameExeFolder, "GenshinImpact_Data")))
            {
                gameType = GameType.OS;
            }

            return gameType;
        }

        public string GameExePath { get; set; }

    }
}
