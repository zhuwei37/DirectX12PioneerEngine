using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.RenderDx
{
    public delegate void GraphicsErrorCallBack(int errorCode,IntPtr errorMessage,IntPtr fileName,IntPtr functionName,int lineNumber);
}
