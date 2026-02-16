using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace Editor.RenderDx
{
    internal struct GraphicsErrorInfo
    {
        public int HR;
        public string ErrorMessage; 
        public string FunctionName;
        public string FileName;
        public int LineNumber;
        public override string ToString()
        {
            return $"hr:{HR},message:{ErrorMessage}\n      FileName---{FileName}   \n FunctionName---{FunctionName} \n LineNumber---{LineNumber} ";
        }
    }
    public static class ErrorManager
    {
        private static GraphicsErrorInfo graphicsErrorInfo;
   
        public static bool HasError { get; private set; }
        public static void GraphicsErrorCallBack(int errorCode, IntPtr errorMessage,IntPtr fileName, IntPtr functionName, int lineNumber)
        {
            graphicsErrorInfo.LineNumber=lineNumber;
            graphicsErrorInfo.HR=errorCode;
            graphicsErrorInfo.FileName = Marshal.PtrToStringAnsi(fileName)!;
            graphicsErrorInfo.FunctionName=Marshal.PtrToStringAnsi(functionName)!;
          graphicsErrorInfo.ErrorMessage=Marshal.PtrToStringAnsi(errorMessage)!;
            HasError = true;
            
        }
        public static Exception Retrieve()
        {
            if (HasError)
            {
                HasError = false;
                System.Diagnostics.Debug.WriteLine(graphicsErrorInfo.ToString());
                return new Exception(graphicsErrorInfo.ToString());
            }
            return new Exception("无错误");
        }
    }
}
