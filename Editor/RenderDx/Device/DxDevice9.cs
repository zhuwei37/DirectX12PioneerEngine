
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Interop;

namespace Editor.RenderDx.Device
{
    public class DxDevice9
    {
       // DeviceEx d3d9Device;
        public DxDevice9()
        {
            //Direct3DEx direct3DEx = new Direct3DEx();
            //PresentParameters presentParams = new PresentParameters()
            //{
            //    Windowed = true,
            //    SwapEffect = SwapEffect.Discard,
            //    //DeviceWindowHandle = GetDesktopWindow(),
            //    PresentationInterval = PresentInterval.Default,
            //    BackBufferHeight = 1,
            //    BackBufferWidth = 1,
            //    BackBufferFormat = Format.Unknown

            //};
          
            // d3d9Device = new DeviceEx(
            //    direct3DEx,
            //    0,
            //    DeviceType.Hardware,
            //    IntPtr.Zero,
            //    CreateFlags.Multithreaded | CreateFlags.HardwareVertexProcessing,
            //    presentParams
            //);
           
        }
        
        //public Texture GetSharedTextureByDX11Handle(IntPtr d3d11Handle,int height,int width)
        //{
        //    Texture d3dTexture = new Texture(
        //        d3d9Device,
        //        width,
        //        height,
        //        1, Usage.RenderTarget,
        //         Format.A8R8G8B8,
        //          Pool.Default,
        //          ref d3d11Handle
        //        );
        //    return d3dTexture;
        //}

    }
}
