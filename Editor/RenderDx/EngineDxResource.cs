using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using ResourceID = System.Guid;
namespace Editor.RenderDx
{
    public static partial class EngineDxImport
    {
        [DllImport("Engine",EntryPoint = "GetRootResourceInfo",CharSet = CharSet.Auto)]
        public static extern ResourceInfo GetRootResourceInfo();
        [DllImport("Engine", EntryPoint = "GetChildResourceInfo", CharSet = CharSet.Auto)]
        private static extern bool GetChildResourceInfo(ResourceID parentId,IntPtr infos,ref int size);
   
        public static ResourceInfo[] GetChildResourceInfo(ResourceID parentId, int size)
        {
            if (size > 0)
            {

                ResourceInfo[] data = new ResourceInfo[size];

                var ptr = Marshal.AllocHGlobal(Marshal.SizeOf<ResourceInfo>() * size);
                var flag = GetChildResourceInfo(parentId, ptr,ref size);
                if (flag)
                {
                    for (int i = 0; i < size; i++)
                    {
                        var inc = Marshal.SizeOf<ResourceInfo>() * i;
                        var point = ptr.ToInt64() + inc;

                        data[i] = Marshal.PtrToStructure<ResourceInfo>((IntPtr)point);
                    }
                    Marshal.FreeHGlobal(ptr);
                    return data;
                }
            }
            return new ResourceInfo[0];
        }

        [DllImport("Engine", EntryPoint = "CreateResource", CharSet = CharSet.Auto)]
        public static extern bool CreateResource(ResourceType type, ResourceID parent, IntPtr path, ref ResourceInfo resource);
        [DllImport("Engine", EntryPoint = "DeleteResource", CharSet = CharSet.Auto)]
        public static extern bool DeleteResource(ResourceID id, ResourceID parentId);



        [DllImport("Engine",EntryPoint = "GetInMemeryResourceCount")]
        private static extern int GetInMemeryResourceCount();
        [DllImport("Engine",EntryPoint = "GetAllInMemeryResourceInfos")]
        private static extern bool GetAllInMemeryResourceInfos(IntPtr infos, ref int size);
        public static ResourceInfo[] GetInMemeryResourceInfos()
        {
            int size = GetInMemeryResourceCount();
            if (size > 0)
            {
                var ptr = Marshal.AllocHGlobal(Marshal.SizeOf<ResourceInfo>() * size);
                var flag = GetAllInMemeryResourceInfos( ptr, ref size);
                if (flag)
                {
                    ResourceInfo[] data = new ResourceInfo[size];
                    for (int i = 0; i < size; i++)
                    {
                        var inc = Marshal.SizeOf<ResourceInfo>() * i;
                        var point = ptr.ToInt64() + inc;

                        data[i] = Marshal.PtrToStructure<ResourceInfo>((IntPtr)point);
                    }
                    Marshal.FreeHGlobal(ptr);
                    return data;
                }
            }
            return Array.Empty<ResourceInfo>();
        }


    }
}
