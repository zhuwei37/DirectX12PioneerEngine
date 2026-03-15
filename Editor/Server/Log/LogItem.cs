using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using Editor.Enums;
namespace Editor.Server.Log
{
    public class LogItem:ObservableObject
    {
        public string Message { get; set; }
        public LogLevel Level { get; set; }
         
    }
    public class LogCodeItem : LogItem 
    {
        public int Line { get; set; }
        public string FunctionName { get; set; }
        public string FileName { get; set; }
    }
}
