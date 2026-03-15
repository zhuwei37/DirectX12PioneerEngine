using Editor.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Editor.RenderDx
{
    public static partial class EngineDxImport
    {
        [DllImport("Engine",EntryPoint = "LogMessage",CharSet = CharSet.Auto)]
        public static extern void LogMessage(LogLevel level, string message);


        [DllImport("Engine", EntryPoint ="SetLogMessageCallback",CharSet = CharSet.Auto)]
        public static extern void SetLogMessageCallback(LogMessageCallback callback);
    }
}
