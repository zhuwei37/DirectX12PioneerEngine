using Editor.Interface.Component;
using Editor.RenderDx;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.ViewModel.Inspector.Component
{
    public static class ComponentCreator
    {
        static Dictionary<string, uint> ComponentNameToIdMap;
        static Dictionary<uint,( string ComponentName,Func<uint,IntPtr,IComponent> Creator)> IdToComponentNameMap;
        static   ComponentCreator() 
        {
            ComponentNameToIdMap = new Dictionary<string, uint>();
            IdToComponentNameMap=new();
            //////////////////////////////////////////////////////////////////////////
            var transformComponentId = EngineDxImport.StringToComponentId((new StringExport("Transform").Ptr));
            ComponentNameToIdMap["Transform"] =transformComponentId;
            IdToComponentNameMap[transformComponentId] =( "Transform",CreateTransformInspector);
            //////////////////////////////////////////////////////////////////
            var meshFilterComponentId = EngineDxImport.StringToComponentId((new StringExport("MeshFilter").Ptr));
            ComponentNameToIdMap["MeshFilter"]=meshFilterComponentId;
            IdToComponentNameMap[meshFilterComponentId] = ("MeshFilter", CreateMeshFilterInspector);
            /////////////////////////////////////////////////////////
            var meshRendererComponentId = EngineDxImport.StringToComponentId((new StringExport("MeshRenderer").Ptr));
            ComponentNameToIdMap["MeshRenderer"] = meshRendererComponentId;
            IdToComponentNameMap[meshRendererComponentId] = ("MeshRenderer", CreateMeshRendererInspector);
            /////////////////////////////////////////////

        }
        public static IComponent? CreateComponent(uint actorId, string ComponentName)
        {
            if (ComponentNameToIdMap.ContainsKey(ComponentName))
            {
                var componentId =ComponentNameToIdMap[ComponentName];
                ComponentInfo info=new ComponentInfo();
                if (EngineDxImport.AddComponent(actorId, componentId, ref info))
                {
                    var component=IdToComponentNameMap[componentId].Creator.Invoke(actorId, info.Handle);
                    return component;
                }
            }
            return null; 
        }

        public static IEnumerable<IComponent> BuildComponentInspector(uint actorId,ComponentInfo[] componentInfos)
        {
            foreach (var info in componentInfos)
            {
                if (IdToComponentNameMap.TryGetValue(info.ComponentId, out var creator))
                {
                    if (creator.Creator is not null)
                    {
                       yield return creator.Creator.Invoke(actorId, info.Handle);
                    }
                }
            }
           yield break;
        }
        private static IComponent CreateTransformInspector(uint ActorId,IntPtr componentPtr)
        {
            return new TransformComponent(ActorId, componentPtr);
        }
        private static IComponent CreateMeshRendererInspector(uint ActorId, IntPtr componentPtr)
        {
            return new MeshRendererComponent(ActorId, componentPtr);
        }
        private static IComponent CreateMeshFilterInspector(uint ActorId, IntPtr componentPtr)
        {
            return new MeshFilterComponent(ActorId, componentPtr);
        }

         
    }
}
