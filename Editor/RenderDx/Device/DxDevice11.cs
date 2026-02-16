
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vortice.Direct3D11;
using Vortice.DXGI;

namespace Editor.RenderDx.Device
{
    public class DxDevice11
    {

        private Vortice.Direct3D11.ID3D11Device device11;
        private Vortice.Direct3D11.ID3D11Device1 device1;
        public DxDevice11()
        {
             device11 = Vortice.Direct3D11.D3D11.D3D11CreateDevice(
                 Vortice.Direct3D.DriverType.Hardware, Vortice.Direct3D11.DeviceCreationFlags.BgraSupport | Vortice.Direct3D11.DeviceCreationFlags.Debug
                );
             device1= device11.QueryInterface<ID3D11Device1>();

        }
        public void CreateSharedTextureByHandle(nint sharedTextureHandle)
        {
            
            var tex = device1.OpenSharedResource1<ID3D11Texture2D>(sharedTextureHandle);
             
            //var d3d11Resource=tex.QueryInterface<IDXGIResource>();
            
            //var h = d3d11Resource.SharedHandle;
            //SharpDX.Direct3D12.Device device12 = new SharpDX.Direct3D12.Device();
            //IntPtr ptr=IntPtr.Zero;
            //device12.OpenSharedHandle(sharedTextureHandle, Guid.NewGuid(), out ptr);

        }
    }
}
