using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Vortice.Direct3D;
using Vortice.Direct3D12;
using Vortice.Direct3D12.Debug;
using Vortice.Mathematics;

namespace Editor.RenderDx.Device
{
    public class AdvancedSharedTextureManager : IDisposable
    {
        public ID3D12Device _device;
        private ID3D12CommandQueue _commandQueue;
        private ID3D12Fence _fence;
        private AutoResetEvent _fenceEvent;
        private ulong _fenceValue = 1;

        private ID3D12Resource _sharedTexture;
        private IntPtr _sharedHandle;
        private ID3D12CommandAllocator _commandAllocator;
        private ID3D12GraphicsCommandList _commandList;
        public AdvancedSharedTextureManager()
        { }
        public bool Initialize( )
        {
            try
            {
                // 1. 创建设备和命令队列
                if (!CreateDeviceAndQueue(false))
                    return false;

                // 2. 创建同步对象
                if (!CreateSyncObjects())
                    return false;

                //// 3. 创建命令列表相关资源
                //if (!CreateCommandResources())
                //    return false;

                //// 4. 创建共享纹理
                ////if (!CreateSharedTextureInternal(width, height))
                ////    return false;

                //Debug.WriteLine("高级共享纹理管理器初始化成功");
                return true;
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"高级共享纹理管理器初始化失败: {ex.Message}");
                return false;
            }
        }

        private bool CreateDeviceAndQueue(bool enableDebug)
        {
            try
            {
                if (enableDebug)
                {
                    try
                    {
                        D3D12.D3D12GetDebugInterface<ID3D12Debug>(out var debugController);
                        debugController.EnableDebugLayer();
                        debugController.Dispose();
                    }
                    catch
                    {
                        Debug.WriteLine("调试层不可用");
                    }
                }

                // 创建设备
                _device = D3D12.D3D12CreateDevice<ID3D12Device>(0, FeatureLevel.Level_11_0);
                if (_device == null) return false;

                // 创建命令队列
                var queueDesc = new CommandQueueDescription(CommandListType.Direct);
                _commandQueue = _device.CreateCommandQueue(queueDesc);

                return _commandQueue != null;
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"创建设备和队列失败: {ex.Message}");
                return false;
            }
        }

        private bool CreateSyncObjects()
        {
            try
            {
                _fence = _device.CreateFence(0, FenceFlags.None);
                _fenceEvent = new AutoResetEvent(false);
                return _fence != null;
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"创建同步对象失败: {ex.Message}");
                return false;
            }
        }

        private bool CreateCommandResources()
        {
            try
            {
                _commandAllocator = _device.CreateCommandAllocator(CommandListType.Direct);
                _commandList = _device.CreateCommandList<ID3D12GraphicsCommandList>(
                    CommandListType.Direct,
                    _commandAllocator,
                    null);

                return _commandAllocator != null && _commandList != null;
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"创建命令资源失败: {ex.Message}");
                return false;
            }
        }

        private bool CreateSharedTextureInternal(int width, int height)
        {
            try
            {
                // 创建堆属性
                var heapProperties = new HeapProperties(HeapType.Default);

                // 创建资源描述
                var resourceDesc = ResourceDescription.Texture2D(
                    format:  Vortice.DXGI.Format.B8G8R8A8_UNorm,
                    width: (uint)width,
                    height: (uint)height,
                    arraySize: 1,
                    mipLevels: 1,
                    sampleCount: 1,
                    sampleQuality: 0,
                    layout: TextureLayout.Unknown,
                    flags: ResourceFlags.AllowRenderTarget | ResourceFlags.AllowUnorderedAccess
                );

                // 创建清除值
                var clearValue = new ClearValue
                {
                    Format =  Vortice.DXGI.Format.B8G8R8A8_UNorm,
                    Color = new Color4(0.1f, 0.1f, 0.1f, 1.0f)
                };

                // 创建共享纹理
                _sharedTexture = _device.CreateCommittedResource(
                    heapProperties,
                    HeapFlags.Shared,
                    resourceDesc,
                    ResourceStates.Common,
                    clearValue
                );

                if (_sharedTexture == null) return false;

                // 获取共享句柄
                _sharedHandle = _device.CreateSharedHandle(_sharedTexture, null, null);

                if (_sharedHandle == IntPtr.Zero)
                {
                    _sharedTexture.Dispose();
                    _sharedTexture = null;
                    return false;
                }

                // 初始化纹理内容（可选）
                InitializeTextureContent();

                return true;
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"创建共享纹理失败: {ex.Message}");
                return false;
            }
        }

        private void InitializeTextureContent()
        {
            try
            {
                // 重置命令列表
                _commandAllocator.Reset();
                _commandList.Reset(_commandAllocator, null);

                // 将纹理转换为渲染目标状态
                var barrier =new  ResourceBarrier(new ResourceTransitionBarrier(
                    _sharedTexture,
                    ResourceStates.Common,
                    ResourceStates.RenderTarget)
                );
                _commandList.ResourceBarrier(barrier);

                // 这里可以添加清除渲染目标等操作
                // ...

                // 将纹理转换回通用状态（适合共享）
                barrier =new ResourceBarrier(new ResourceTransitionBarrier(
                    _sharedTexture,
                    ResourceStates.RenderTarget,
                    ResourceStates.Common)
                );
                _commandList.ResourceBarrier(barrier);

                // 关闭命令列表
                _commandList.Close();

                // 执行命令列表
                ID3D12CommandList[] commandLists = { _commandList };
                _commandQueue.ExecuteCommandLists(commandLists);

                // 等待命令执行完成
                WaitForGpu();
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"初始化纹理内容失败: {ex.Message}");
            }
        }

        public void WaitForGpu()
        {
            try
            {
                // 发送围栏信号
                _commandQueue.Signal(_fence, _fenceValue);

                // 等待 GPU 完成
                if (_fence.CompletedValue < _fenceValue)
                {
                    _fence.SetEventOnCompletion(_fenceValue, _fenceEvent.SafeWaitHandle.DangerousGetHandle());
                    _fenceEvent.WaitOne();
                }

                _fenceValue++;
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"等待 GPU 失败: {ex.Message}");
            }
        }

        public bool UpdateTexture(Action<ID3D12GraphicsCommandList> updateAction)
        {
            try
            {
                // 重置命令分配器和列表
                _commandAllocator.Reset();
                _commandList.Reset(_commandAllocator, null);

                // 将纹理转换为可写状态
                var barrierToWrite =new  ResourceBarrier(new ResourceTransitionBarrier(
                    _sharedTexture,
                    ResourceStates.Common,
                    ResourceStates.RenderTarget)
                );
                _commandList.ResourceBarrier(barrierToWrite);

                // 执行用户提供的更新操作
                updateAction?.Invoke(_commandList);

                // 将纹理转换回共享状态
                var barrierToCommon = new ResourceBarrier(new ResourceTransitionBarrier(
                    _sharedTexture,
                    ResourceStates.RenderTarget,
                    ResourceStates.Common)
                );
                _commandList.ResourceBarrier(barrierToCommon);

                // 关闭并执行命令列表
                _commandList.Close();
                ID3D12CommandList[] commandLists = { _commandList };
                _commandQueue.ExecuteCommandLists(commandLists);

                // 等待完成
                WaitForGpu();

                return true;
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"更新纹理失败: {ex.Message}");
                return false;
            }
        }
        ~AdvancedSharedTextureManager() 
        {

        }
        public void Dispose()
        {
            WaitForGpu();

            _sharedTexture?.Dispose();
            _sharedTexture = null;
            _commandList?.Dispose();
            _commandAllocator?.Dispose();
            _fence?.Dispose();
            _fenceEvent?.Dispose();
            _commandQueue?.Dispose();
            _device?.Dispose();

            Debug.WriteLine("高级共享纹理管理器已释放");
        }
    }
}
