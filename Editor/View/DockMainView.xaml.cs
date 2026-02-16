

using Editor.Interface.UI;
using Editor.Server;
using Editor.ViewModel;
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
using Xceed.Wpf.AvalonDock;

namespace Editor.View
{
    public class CC : ICommand
    {
        public CC()
        {
            
        }
        public event EventHandler? CanExecuteChanged;

        public bool CanExecute(object? parameter)
        {
            return true;
        }

        public void Execute(object? parameter)
        {
            
        }
    }
    /// <summary>
    /// DockMainView.xaml 的交互逻辑
    /// </summary>
    public partial class DockMainView : System.Windows.Controls.UserControl
    {
        public DockMainView()
        {
            InitializeComponent();
            this.DataContext = DockManager.Instance.GetViewModel();

         
           
        }
    }
}
