using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Editor.RenderDx
{
    public class StringExport:IDisposable
    {
        private bool _disposed=false;
        public StringExport(string str) 
        {
            _ptr= Marshal.StringToHGlobalAnsi(str);
            _str= str;
        }
        public StringExport(IntPtr ptr,bool selfDispose=false)
        {
            _ptr = ptr;
            _str = Marshal.PtrToStringAnsi(ptr);
            _disposed = !selfDispose;
        }
        private string _str;
        private IntPtr _ptr=IntPtr.Zero;
        public IntPtr Ptr 
        {
            get 
            {
                return _ptr;
            }
        }
        public string Str=>_str;
        public void Dispose()
        {
            if (_ptr != IntPtr.Zero&&!_disposed)
            {
                _disposed = true;
                _ptr = IntPtr.Zero;
                Marshal.FreeHGlobal(_ptr);
            }
        }
        ~StringExport()
        {
            Dispose();
        }
    }

    
}
