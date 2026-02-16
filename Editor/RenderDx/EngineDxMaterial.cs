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
        [DllImport("Engine",EntryPoint = "MaterialNameToPropertyID")]
        public static extern int MaterialNameToPropertyID(string matProertyName);

        [DllImport("Engine",EntryPoint = "GetMaterialShaderID")]
        public static extern int GetMaterialShaderID(ResourceID matResourceID);
        [DllImport("Engine", EntryPoint = "SetMaterialShaderID")]
        public static extern bool SetMaterialShaderID(ResourceID matResourceID, int shaderID);


        [DllImport("Engine", EntryPoint = "GetMaterialPropertyFloat")]
        public static extern float GetMaterialPropertyFloat(ResourceID matResourceID, long proertyId);
        [DllImport("Engine", EntryPoint = "GetMaterialPropertyFloat3")]
        public static extern Vector3 GetMaterialPropertyFloat3(ResourceID matResourceID, long porertyId);
        [DllImport("Engine", EntryPoint = "GetMaterialPropertyFloat4")]
        public static extern Vector4 GetMaterialPropertyFloat4(ResourceID matResourceID, long propertyId);
        [DllImport("Engine", EntryPoint = "GetMaterialPropertyTexture")]
        public static extern ResourceID GetMaterialPropertyTexture(ResourceID matResourceID, long propertyId);




        [DllImport("Engine", EntryPoint = "SetMaterialPropertyFloat")]
        public static extern void  SetMaterialPropertyFloat(ResourceID matResourceID, long propertyId, float refValue);
        [DllImport("Engine", EntryPoint = "SetMaterialPropertyFloat3")]
        public static extern void SetMaterialPropertyFloat3(ResourceID matResourceID, long propertyId, Vector3 refValue);
        [DllImport("Engine", EntryPoint = "SetMaterialPropertyFloat4")]
        public static extern void SetMaterialPropertyFloat4(ResourceID matResourceID, long propertyId, Vector4 refValue);
        [DllImport("Engine", EntryPoint = "SetMaterialPropertyTexture")]
        public static extern void SetMaterialPropertyTexture(ResourceID matResourceID, long propertyId, ResourceID refValue);

    }
}
