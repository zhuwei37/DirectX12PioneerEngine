using Editor.View;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Editor.Interface.UI
{
    public interface ITool
    {
        public ToolType UIToolType { get; set; }
        public string Title { get; set; } 
    }
}
