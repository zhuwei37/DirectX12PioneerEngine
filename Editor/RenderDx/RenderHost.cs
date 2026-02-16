using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.RenderDx
{
    public class RenderHost
    {
        private RenderHost(nint hwnd, int width, int height)
        {
            RenderHostId = EngineDxImport.CreateRenderHost(hwnd, width, height);
            if (RenderHostId == 0)
            {
                throw new Exception("无效渲染主机");
            }
            cameraController=new CameraController(RenderHostId);
        }
        /// <summary>
        /// 后台渲染主机ID
        /// </summary>
        public uint RenderHostId
        {
            get;
            private set;
        }
        private CameraController cameraController;

        public CameraController RenderHostCameraController
        {
            get { return cameraController; }
        }
        public static RenderHost CreateRenderHost(nint hwnd,int width,int height)
        {
            return new RenderHost(hwnd,width,height);
        }
        /// <summary>
        /// 调整后台缓冲区
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        public void Resize(int width, int height)
        {
            EngineDxImport.Resize_RenderHost(RenderHostId, width, height);
        }
        /// <summary>
        /// 是否可见
        /// </summary>
        public bool Visible
        {
            get { return EngineDxImport.GetRenderHostVisibility(RenderHostId); }
            set { EngineDxImport.SetRenderHostVisibility(RenderHostId, value); }
        }
        ~RenderHost()
        {
            if (RenderHostId != 0)
            {
                EngineDxImport.Destory_RenderHost(RenderHostId);
            }
        }


    }
}
