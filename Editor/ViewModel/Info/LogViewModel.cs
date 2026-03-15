using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using Editor.Interface.UI;
using Editor.Server.Log;
using Editor.View;
namespace Editor.ViewModel.Info
{
    public class LogViewModel : ObservableObject, ITool
    {
        public ToolType UIToolType { get; set; }= ToolType.Info;
        public string Title { get; set; } = "日志";

        public ObservableCollection<LogItem> Logs { get =>LogManager.Instance.LogItems; }
        public LogViewModel()
        { 
        }
       
    }
}
