using Editor.Server.Scene;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.VisualStyles;
using ResourceID = System.Guid;
namespace Editor.RenderDx
{
    /// <summary>
    /// 组件
    /// </summary>
    public static partial class EngineDxImport
    {

        [DllImport("Engine", EntryPoint = "SetAll", CharSet = CharSet.Ansi)]
        public static extern uint SetAll();
        [DllImport("Engine",EntryPoint = "StringToComponentId")]
        public static extern uint StringToComponentId(IntPtr strPtr);

        [DllImport("Engine", EntryPoint = "GetComponentCount")]
        private static extern int GetComponentCount(uint actorid);
        [DllImport("Engine", EntryPoint = "GetActorAllComponent")]
        private static extern bool GetActorAllComponent(uint actorid, IntPtr dataPtr, int capacity);

        public static ComponentInfo[] GetComponents(uint actorid) 
        {
            int componentCount = EngineDxImport.GetComponentCount(actorid);
            if (componentCount > 0)
            {

                ComponentInfo[] data = new ComponentInfo[componentCount];

                var ptr = Marshal.AllocHGlobal(Marshal.SizeOf<ComponentInfo>() * componentCount);
                var flag = GetActorAllComponent(actorid, ptr, componentCount);
                if (flag)
                {
                    for (int i = 0; i < componentCount; i++)
                    {
                        var inc = Marshal.SizeOf<ComponentInfo>() * i;
                        var point = ptr.ToInt64() + inc;

                        data[i] = Marshal.PtrToStructure<ComponentInfo>((IntPtr)point);
                    }
                    Marshal.FreeHGlobal(ptr);
                    return data;
                }
            }
            return new ComponentInfo[0];
        }

        [DllImport("Engine",EntryPoint ="AddComponent")]
        public static extern bool AddComponent(uint actorId, uint componentId, ref ComponentInfo componentInfo);
        [DllImport("Engine",EntryPoint ="RemoveComponent")]
        public static extern bool RemoveComponent(uint actorId, uint ComponentId);


        #region 变换

        [DllImport("Engine", EntryPoint = "Get_Transform_Position")]
        public static extern Vector3 Get_Transform_Position(IntPtr transformPtr);
        [DllImport("Engine", EntryPoint = "Get_Transform_Rotate")]
        public static extern Vector3 Get_Transform_Rotate(IntPtr transformPtr);
        [DllImport("Engine", EntryPoint = "Get_Transform_Scale")]
        public static extern Vector3 Get_Transform_Scale(IntPtr transformPtr);
        [DllImport("Engine", EntryPoint = "Set_Transform_Position")]
        public static extern void Set_Transform_Position(IntPtr transformPtr, Vector3 vector);
        [DllImport("Engine", EntryPoint = "Set_Transform_Rotate")]
        public static extern void Set_Transform_Rotate(IntPtr transformPtr, Vector3 vector);
        [DllImport("Engine", EntryPoint = "Set_Transform_Scale")]
        public static extern void Set_Transform_Scale(IntPtr transformPtr, Vector3 vector);
        #endregion


        #region 网格数据 MeshFilter
        [DllImport("Engine",EntryPoint = "GetMeshFilter_MeshResourceID")]
        public static extern bool GetMeshFilter_MeshResourceID(IntPtr meshFilterHandle, ref ResourceID resourceID);
        [DllImport("Engine",EntryPoint = "SetMeshFilter_MeshResourceID")]
        public static extern void SetMeshFilter_MeshResourceID(IntPtr meshFilterHandle, ResourceID resourceID);
        #endregion

        #region 网格渲染器 MeshRenderer
        [DllImport("Engine",EntryPoint = "MeshRenderer_AddMaterialSlot")]
        public static extern void MeshRenderer_AddMaterialSlot(IntPtr componentPtr);
        [DllImport("Engine",EntryPoint = "MeshRenderer_RemoveMaterialSlot")]
        public static extern void MeshRenderer_RemoveMaterialSlot(IntPtr componentPtr);

        [DllImport("Engine",EntryPoint = "GetMeshRenderer_MaterialCount")]
        private static extern int GetMeshRenderer_MaterialCount(IntPtr componentPtr);

        [DllImport("Engine",EntryPoint = "GetMeshRenderer_MaterialResourceIDs")]
        private static extern bool GetMeshRenderer_MaterialResourceIDs(IntPtr componentPtr, IntPtr materialResourceIDs, ref int size);

        [DllImport("Engine",EntryPoint = "SetMeshRenderer_MaterialResourceID")]
        public static extern bool SetMeshRenderer_MaterialResourceID(IntPtr componentPtr, int index, ResourceID materialResourceID);

        public static ResourceID[] GetMeshRendererMateriels(IntPtr componentPtr)
        {
            var size = GetMeshRenderer_MaterialCount(componentPtr);
            if (size > 0)
            {
                var ptr = Marshal.AllocHGlobal(Marshal.SizeOf<ComponentInfo>() * size);
                var flag=GetMeshRenderer_MaterialResourceIDs(componentPtr, ptr, ref size);
                if (flag)
                {
                    ResourceID[] materialResourceIDs=new ResourceID[size];

                    for (int i = 0; i < size; i++)
                    {
                        var inc = Marshal.SizeOf<ComponentInfo>() * i;
                        var point = ptr.ToInt64() + inc;
                        materialResourceIDs[i] = Marshal.PtrToStructure<ResourceID>((IntPtr)point);
                    }
                    Marshal.FreeHGlobal(ptr);
                    return materialResourceIDs;

                }
                Marshal.FreeHGlobal(ptr);
                
            }
            return Array.Empty<ResourceID>();
        }

        #endregion
    }
}
