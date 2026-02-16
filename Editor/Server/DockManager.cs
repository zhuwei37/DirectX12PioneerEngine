using Editor.Interface.UI;
using Editor.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Server
{
    public class DockManager : IDockManager
    {
        public static IDockManager Instance { get => instance; } 


        private static IDockManager instance =new Lazy<DockManager>(()=>new DockManager()).Value;
        public DockMainViewModel vm = new DockMainViewModel();
        public object GetViewModel()
        {
            return vm;
        }
        public void AddPage(IPage page)
        {
            vm.MainDocuments.Add(page);
        }

        public void AddTool(ITool tool)
        {
            vm.Tools.Add(tool);
        }

        public void RemovePage(IPage page)
        {
            
        }

        public void RemovePahe(string pageName)
        {
            
        }

        public void RemoveTool(ITool tool)
        {
          
        }

        public void RemoveTool(string toolName)
        {
            
        }
    }
}
