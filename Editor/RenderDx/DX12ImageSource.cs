using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Editor.RenderDx.Device;
namespace Editor.RenderDx
{
    public class DX12ImageSource:System.Windows.Interop.D3DImage
    {
        DxDevice _device;
        //Texture texture;
        
        public DX12ImageSource(DxDevice device) 
        {
            _device = device;
        }

        public void SetSharedTexture(IntPtr handle,int width,int height)
        {
            int a=(int) handle;
            //if (texture != null)
            //{
            //    texture.Dispose();
            //}
            try
            {
                _device.CreateSharedTextureByHandle(handle, width, height);
               // SharpDX.Direct3D12.Device device=new SharpDX.Direct3D12.Device(SharpDX.Direct3D12.DebugDeviceParameterType.);
                //texture = _device.CreateSharedTextureByHandle(handle, width, height);
                //var surface = texture.GetSurfaceLevel(0);
                //this.SetBackBuffer(System.Windows.Interop.D3DResourceType.IDirect3DSurface9, surface.NativePointer);
                //this.Lock();
                //this.AddDirtyRect(new System.Windows.Int32Rect(0, 0, width, height));
                //this.Unlock();
            }
            catch (Exception ex) {
                Debug.WriteLine(ex.ToString());
            }
        }
      
        
    }
}
