
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace Editor.RenderDx.Device
{
    public class DxDevice
    {
        private DxDevice9 device9;
        private DxDevice11 device11;
        private DxDevice12 device12;    
        public DxDevice()
        {
            device11 = new();
            device9 = new();
            device12=new DxDevice12();
           //AdvancedSharedTextureManager ad=new AdvancedSharedTextureManager();
           // ad.Initialize(800,600);
            

        }
        public void  CreateSharedTextureByHandle(IntPtr sharedTextureHandle, int width, int height)
        {
            try
            {
                device11.CreateSharedTextureByHandle(sharedTextureHandle);
            }
            catch(Exception ex) 
            {
                Debug.WriteLine(ex.ToString());
            }
            //var tex_Dx11 = device11.CreateSharedTextureByHandle((IntPtr)sharedTextureHandle);
            //var shared11 = tex_Dx11.QueryInterface<SharpDX.DXGI.Resource>();
            //var handle = shared11.SharedHandle;

            //var texture = device9.GetSharedTextureByDX11Handle(handle, height, width);
            //shared11.Dispose();
            //return texture;
        }
    }
}
