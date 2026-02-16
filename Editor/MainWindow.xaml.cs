
using CommunityToolkit.Mvvm.Input;
using Editor.RenderDx;
using Editor.RenderDx.Device;
using Editor.Server;
using Editor.ViewModel;
using Editor.ViewModel.Info;
using Editor.ViewModel.Inspector;
using Editor.ViewModel.Preview;
using Editor.ViewModel.Workspace;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Editor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ViewModel.MainWindowViewModel viewModel;
        public MainWindow()
        {
           
            InitializeComponent();
          
            viewModel = new ViewModel.MainWindowViewModel();
           
            this.DataContext = viewModel;
            _ = Editor.Server.EngineManager.Instance;
            //Guid
            //cc();
           
         
        }
        async Task cc()
        {
            
           // DockManager.Instance.AddPage(new ViewportDxViewModel());
            //DockManager.Instance.AddPage(new ViewportDxViewModel());
            //DockManager.Instance.AddPage(new ViewportDxViewModel());
        }


        
    }
}