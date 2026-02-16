using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Interface.UI
{
    public interface IDockManager
    {
        object GetViewModel();
        void AddTool(ITool tool);
        void RemoveTool(ITool tool);
        void RemoveTool(string toolName);
        void AddPage(IPage page);
        void RemovePage(IPage page);
        void RemovePahe(string pageName);
    }
}
