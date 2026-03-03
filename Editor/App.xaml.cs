using Editor.RenderDx;
using Editor.Server;
using System.Configuration;
using System.Data;
using System.Windows;

namespace Editor
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : System.Windows.Application
    {

        protected override void OnStartup(StartupEventArgs e)
        {
            RenderDx.EngineDxImport.InitEngine();
           // RenderDx.EngineDxImport.SetGraphicsErrorCallBack(ErrorManager.GraphicsErrorCallBack);
            RenderDx.EngineDxImport.LoadScene();

            ThemeManager.RegisterTheme("Dark", "DirectX12PionnerEngine.UI", "Themes/Dark.xaml");
            ThemeManager.RegisterTheme("LightBlue", "DirectX12PionnerEngine.UI", "Themes/LightBlue.xaml");
            ThemeManager.ApplyTheme("Dark");


            //Render.Instance.Init();
            base.OnStartup(e);
        }
        /// <summary>
        /// 程序退出时
        /// </summary>
        /// <param name="e"></param>
        protected override void OnExit(ExitEventArgs e)
        {
            base.OnExit(e);
        }
        /// <summary>
        /// 程序不可见时
        /// </summary>
        /// <param name="e"></param>
        protected override void OnDeactivated(EventArgs e)
        {
            base.OnDeactivated(e);
        }
    }

}
