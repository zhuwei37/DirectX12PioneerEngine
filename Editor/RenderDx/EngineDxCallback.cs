using Editor.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Editor.RenderDx
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void LogMessageCallback(LogLevel logLevel, IntPtr messagePtr);
}
