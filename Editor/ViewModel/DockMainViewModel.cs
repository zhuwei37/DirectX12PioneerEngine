using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;
using Editor.ViewModel.Workspace;
using Editor.View.Preview;
using Editor.ViewModel.Preview;
using System.Windows.Input;
using CommunityToolkit.Mvvm.Input;
using Editor.ViewModel.Info;
using System.Diagnostics;
using Editor.Interface.UI;
using Editor.View.Workspace;
using Editor.Server;
namespace Editor.ViewModel
{
    public class DockMainViewModel:CommunityToolkit.Mvvm.ComponentModel.ObservableObject
    {
        public ObservableCollection<IPage> MainDocuments { get; set; }=new ObservableCollection<IPage>();
        public ObservableCollection<ITool> Tools { get; set; }=new ObservableCollection<ITool>();
     
        public DockMainViewModel()
        {
        }
    }
}
