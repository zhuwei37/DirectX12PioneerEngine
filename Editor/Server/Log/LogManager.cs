using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;
using Vortice.Direct3D11;
using Editor.Enums;
using Editor.RenderDx;
using System.Runtime.InteropServices;
namespace Editor.Server.Log
{
    public class LogManager
    {
        private LogMessageCallback _LogMessageCallback;
        public static LogManager Instance { get; private set; } = new Lazy<LogManager>(() => new LogManager()).Value;
        public ObservableCollection<LogItem> LogItems { get; set; } = new ObservableCollection<LogItem>();
        public LogManager()
        {
            _LogMessageCallback = this.LogMessage;
        }

        private void Log(LogLevel level, string message)
        {
            LogItems.Add(new LogItem()
            {
                Level = level,
                Message = message
            });
        }
        private  void LogMessage(LogLevel level, IntPtr messagePtr)
        {
            var message = new StringExport(messagePtr);
            Log(level, message.Str);
        }
        private void LogMessageX()
        {

        }
        public void Init()
        {

           // var _gcHandle = GCHandle.Alloc(LogMessage, GCHandleType.Normal);

         //   var handle =Marshal.GetFunctionPointerForDelegate<LogMessageCallback>(_LogMessageCallback); 
            EngineDxImport.SetLogMessageCallback(_LogMessageCallback);
        }
        public void LogInfo(
            string message,
            [System.Runtime.CompilerServices.CallerMemberName] string? memberName = null,
            [System.Runtime.CompilerServices.CallerFilePath] string? filePath = null,
            [System.Runtime.CompilerServices.CallerLineNumber] int line = 0)
        {
            Log(LogLevel.Info, message);
        }
        public void LogWarning(string message)
        {
            Log(LogLevel.Warning, message);
        }

        public void LogError(string message)
        {
            Log(LogLevel.Error, message);
        }
        public void LogDebug(string message)
        {
            Log(LogLevel.Debug, message);
        }
        ~LogManager()
        {

        }
    }
}
