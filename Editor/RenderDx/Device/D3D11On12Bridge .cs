using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vortice.Direct3D;
using Vortice.Direct3D11;
using Vortice.Direct3D11on12;
using Vortice.Direct3D12;

namespace Editor.RenderDx.Device
{
    public class D3D11On12TextureManager : IDisposable
    {
        private ID3D12Device _d3d12Device;
        private ID3D11Device _d3d11Device;
        private ID3D11On12Device _d3d11on12Device;
        private ID3D11Resource _wrappedResource;

        public bool Initialize(IntPtr sharedHandle,ID3D12Device d3d12Device, uint width, uint height)
        {
            try
            {
                // 1. 创建D3D12设备（如果尚未创建）
                _d3d12Device = d3d12Device;

                // 2. 用D3D12设备打开共享纹理
                var d3d12Resource = _d3d12Device.OpenSharedHandle<ID3D12Resource>(sharedHandle);

                // 3. 创建D3D11设备（注意启用BGRA支持）
                var d3d11Device = D3D11.D3D11CreateDevice(
                    DriverType.Hardware,
                    DeviceCreationFlags.BgraSupport | DeviceCreationFlags.Debug // 调试标志可选
                );
                _d3d11Device = d3d11Device.QueryInterface<ID3D11Device>();
                // 4. 查询D3D11On12设备接口
                _d3d11on12Device = _d3d11Device.QueryInterface<ID3D11On12Device>();
                //ID3D11Resource
                // 5. 将D3D12资源包装为D3D11资源
               
                //_wrappedResource = _d3d11on12Device.CreateWrappedResource<ID3D11Resource>(
                //    , new Vortice.Direct3D11on12.ResourceFlags() { BindFlags= BindFlags.ShaderResource,  }
                //      ,
                //    ResourceStates.Common,
                //    ResourceStates.Common
                //);

                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"初始化D3D11On12失败: {ex.Message}");
                return false;
            }
        }

        public void BeginRender()
        {
            // 在D3D11使用纹理前，获取访问权
            _d3d11on12Device.AcquireWrappedResources(new[] { _wrappedResource }, 1);
        }

        public void EndRender()
        {
            // D3D11使用完毕后，释放访问权，交还给D3D12
            _d3d11on12Device.ReleaseWrappedResources(new[] { _wrappedResource }, 1);
            // 通常随后需要刷新D3D11命令队列
            using (var queue = _d3d11Device.ImmediateContext.QueryInterface<ID3D11DeviceContext>())
            {
                queue.Flush();
            }
        }

        public ID3D11Texture2D GetWrappedTexture2D()
        {
            return _wrappedResource.QueryInterface<ID3D11Texture2D>();
        }

        public void Dispose()
        {
            _wrappedResource?.Dispose();
            _d3d11on12Device?.Dispose();
            _d3d11Device?.Dispose();
            _d3d12Device?.Dispose();
        }
    }
}
