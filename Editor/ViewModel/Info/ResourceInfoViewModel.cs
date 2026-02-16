using Editor.View;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using System.Windows.Input;
using Editor.Interface.UI;
using CommunityToolkit.Mvvm.Input;
namespace Editor.ViewModel.Info
{
    public class ResourceInfoViewModel :ObservableObject , ITool
    {
        public ToolType UIToolType { get ; set; }=ToolType.Info;
        public string Title { get; set; } = "资源";
        
        public ResourceInfoViewModel()
        {

        }
        public ResourcePanelViewModel ResourcePanel { get; set; }=new ResourcePanelViewModel();
    }
}
