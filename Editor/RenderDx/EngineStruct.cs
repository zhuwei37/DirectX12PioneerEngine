using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using ResourceID = System.Guid;
namespace Editor.RenderDx
{
    [StructLayout(LayoutKind.Sequential,CharSet =CharSet.Ansi,Pack =1)]
    public struct SceneNodeInfo
    {
        public uint SceneNodeID;
        public uint ParentID;
        public uint ActorId;
        public int ChildCount;
        [MarshalAs( UnmanagedType.ByValTStr,SizeConst =50)]
        public string Name;
    }
    [StructLayout(LayoutKind.Sequential,CharSet =CharSet.Ansi)]
    public struct ComponentInfo
    {
        public uint ComponentId;
        public IntPtr Handle;
    }
    [StructLayout(LayoutKind.Sequential,CharSet=CharSet.Ansi,Pack =1)]
    public struct Vector3 {
        public float X;
        public float Y;
        public float Z; 
    }
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector4 
    {
        public float X;
        public float Y;
        public float Z;
        public float W;
    }
    [StructLayout(LayoutKind.Sequential,CharSet = CharSet.Ansi)]
    public struct ResourceInfo
    {
        public ResourceID resourceID;
        public ResourceType resourceType;
        public IntPtr path;
        public int childrenCount;
        public bool hasChildren;
    }
}
