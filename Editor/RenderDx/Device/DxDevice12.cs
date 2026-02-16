using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;
using Vortice.Direct3D;
using Vortice.Direct3D11;
using Vortice.Direct3D12;
using Vortice.Direct3D9;
using Vortice.DXGI;
using Vortice.Mathematics;
namespace Editor.RenderDx.Device
{
    public class DxDevice12
    {
        private ID3D12Resource _d3d12SharedTexture;
        private ID3D12Device _d3d12Device;
        AdvancedSharedTextureManager Creater;
        public DxDevice12() 
        {
            //Creater = new AdvancedSharedTextureManager();
            //Creater.Initialize();
            //Creater.WaitForGpu();
            //_d3d12Device = Creater._device;

            Vortice.DXGI.IDXGISwapChain2 iDXGISwapChain2;
        }
        ~DxDevice12()
        {
            
        }
        public void OpenSharedTexture(IntPtr sharedTextureHandle,uint width,uint height)
        {
          
            try
            {
               
               
                _d3d12Device = Creater._device;
                //_d3d12SharedTexture = _d3d12Device.OpenSharedHandle<ID3D12Resource>(sharedTextureHandle);
                //D3D11On12TextureManager on12TextureManager = new D3D11On12TextureManager();
                //on12TextureManager.Initialize(sharedTextureHandle, _d3d12Device, width, height);
            }
            catch (Exception ex)
            {
               
                var deviceRemovedReason = _d3d12Device.DeviceRemovedReason;
                Debug.WriteLine($"设备移除详细原因: 0x{deviceRemovedReason:X8}");

                // 根据 reason code 进一步判断问题根源
                switch ((uint)deviceRemovedReason.Code)
                {
                    case 0x887A0006: // DXGI_ERROR_DEVICE_HUNG
                        Debug.WriteLine("GPU 驱动程序无响应，通常由驱动或应用程序代码错误引起。");
                        break;
                    case 0x887A0005: // DXGI_ERROR_DEVICE_REMOVED (再次出现)
                        Debug.WriteLine("设备已被物理移除或驱动重置。");
                        break;
                    case 0x887A0007: // DXGI_ERROR_DEVICE_RESET
                        Debug.WriteLine("设备在执行命令时遇到问题并重置。");
                        break;
                    case 0x887A002D: // DXGI_ERROR_SDK_COMPONENT_MISSING
                        Debug.WriteLine("系统缺少必要的 DirectX 调试层或组件:cite[4]。");
                        break;
                    default:
                        Debug.WriteLine("未知的设备错误。");
                        break;
                }
            }
           
        }
    }
}
