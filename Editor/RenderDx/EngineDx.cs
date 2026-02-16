
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.DirectoryServices;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Editor.RenderDx
{
    public static  partial class EngineDxImport
    {
        [DllImport("Engine", EntryPoint = "GetNum", CharSet = CharSet.Auto)]
        public static extern int GetNum();
        [DllImport("Engine", EntryPoint = "InitD3D", CharSet = CharSet.Auto)]
        public static extern void InitD3D(IntPtr hwnd,uint width,uint height);
        [DllImport("Engine",EntryPoint ="Show",CharSet = CharSet.Auto)]
        public static extern void Show(nint point);
        [DllImport("Engine",EntryPoint ="Run",CharSet = CharSet.Auto)]
        public static extern void Run();
        [DllImport("Engine", EntryPoint = "Resize", CharSet = CharSet.Auto)]
        public static extern void Resize(uint width, uint height);
        [DllImport("Engine",EntryPoint = "GetSharedTextureHandle",CharSet =CharSet.Auto)]
        public static extern IntPtr GetSharedTextureHandle();
        [DllImport("Engine",EntryPoint = "Update", CharSet =CharSet.Auto)]
        public static extern void Update();
        /// <summary>
        /// 初始化场景
        /// </summary>
        [DllImport("Engine",EntryPoint = "InitEngine",CharSet =CharSet.Auto)]
        public static extern void InitEngine();





        #region 渲染主机
        [DllImport("Engine",EntryPoint = "CreateRenderHost",CharSet =CharSet.Auto)]
        public static extern uint CreateRenderHost(nint hwnd, int width, int height);
        [DllImport("Engine", EntryPoint = "Resize_RenderHost", CharSet = CharSet.Auto)]
        public static extern void Resize_RenderHost(uint renderHostId, int width, int height);
        [DllImport("Engine", EntryPoint = "Destory_RenderHost", CharSet = CharSet.Auto)]
        public static extern void Destory_RenderHost(uint renderHostId);
        [DllImport("Engine", EntryPoint = "GetRenderHostVisibility", CharSet = CharSet.Auto)]
        public static extern bool GetRenderHostVisibility(uint renderHostId);
        [DllImport("Engine", EntryPoint = "SetRenderHostVisibility", CharSet = CharSet.Auto)]
        public static extern void SetRenderHostVisibility(uint renderHostId, bool vis);
        #endregion




        #region 错误
        [DllImport("Engine",EntryPoint = "SetGraphicsErrorCallBack", CharSet = CharSet.Auto)]
        public static extern void SetGraphicsErrorCallBack(GraphicsErrorCallBack callBack);
        #endregion




        #region 摄像机

        [DllImport("Engine",EntryPoint = "Walk",CharSet = CharSet.Auto)]
        public static extern void Walk(uint renderHostId, float d);
        [DllImport("Engine",EntryPoint="Strafe",CharSet = CharSet.Auto)]
        public static extern void Strafe(uint renderHostId,float d);
        [DllImport("Engine",EntryPoint ="Pitch",CharSet =CharSet.Auto)]
        public static extern void Pitch(uint renderHostId, float d);
        [DllImport("Engine",EntryPoint ="RotateY",CharSet = CharSet.Auto)]
        public static extern void RotateY(uint renderHostId, float d);
        #endregion


        #region 场景
        [DllImport("Engine",EntryPoint = "GetRootSceneNodeInfo", CharSet = CharSet.Ansi)]
        public static extern SceneNodeInfo GetRootSceneNodeInfo();
        [DllImport("Engine",EntryPoint = "GetSceneNodeCount",CharSet =CharSet.Ansi)]
        public static extern int GetSceneNodeCount();
        [DllImport("Engine",EntryPoint = "GetAllSceneNodeInfos",CharSet =CharSet.Ansi)]
        public static extern bool GetAllSceneNodeInfos(IntPtr data, int len);
        [DllImport("Engine", EntryPoint = "GetSceneNodeInfosByID", CharSet = CharSet.Ansi)]

        public static extern bool GetSceneNodeInfosByID(uint id, IntPtr data, int len);
        public static SceneNodeInfo[]? GetSceneNodeInfosByID(uint sceneNodeID,int len)
        {
            SceneNodeInfo[] data=new SceneNodeInfo[len];
            
                var ptr = Marshal.AllocHGlobal(Marshal.SizeOf<SceneNodeInfo>() * len);
                var flag = GetSceneNodeInfosByID(sceneNodeID, ptr, len);
                if (flag)
                {
                    for (int i = 0; i < len; i++)
                    {
                    var inc = Marshal.SizeOf<SceneNodeInfo>() * i;
                      var point = ptr.ToInt64()+inc;

                      data[i]= Marshal.PtrToStructure<SceneNodeInfo>((IntPtr)point);
                    Debug.WriteLine(data[i].Name);
                    }
                    Marshal.FreeHGlobal(ptr);
                    return data;
                }
            
            return null;
        }

        public static SceneNodeInfo[]? GetAllSceneNodeInfos()
        {
            int len = GetSceneNodeCount();
            if (len > 0)
            {
                var ptr = Marshal.AllocHGlobal(Marshal.SizeOf<SceneNodeInfo>() * len);
                try
                {
                    if (GetAllSceneNodeInfos(ptr, len))
                    {
                        SceneNodeInfo[] data = new SceneNodeInfo[len];

                        for (int i = 0; i < len; i++)
                        {
                            var point =ptr.ToInt64()+i*Marshal.SizeOf<SceneNodeInfo>();
                            data[i]=Marshal.PtrToStructure<SceneNodeInfo>((IntPtr)point);
                        }
                        return data;
                    }
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e.Message);
                }
                finally 
                {
                    Marshal.FreeHGlobal(ptr);
                }
            }
            return null;
        }

        [DllImport("Engine",EntryPoint = "DeleteSceneNode",CharSet =CharSet.Ansi)]
        public static extern void RemoveSceneNode(uint sceneNodeID);
        [DllImport("Engine",EntryPoint = "CreateSceneNode",CharSet = CharSet.Ansi)]
        public static extern SceneNodeInfo CreateSceneNode(string name, uint parentId);

        [DllImport("Engine",EntryPoint = "SetSceneNodeName")]
        public static extern void SetSceneNodeName(uint sceneNodeID, string newName);

        [DllImport("Engine",EntryPoint ="LoadScene",CharSet =CharSet.Ansi)]
        public static extern void LoadScene();

        #endregion


      
    }
}
