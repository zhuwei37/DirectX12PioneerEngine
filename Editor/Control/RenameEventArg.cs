using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Control
{
    public class TextChangedEventArgs:EventArgs
    {
        public object? CommandParameter { get; set; }
        public string NewName { get; set; }=string.Empty;
        public string OldName { get; set; } = string.Empty;
    }
}
