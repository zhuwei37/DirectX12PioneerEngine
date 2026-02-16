using Editor.Server.Scene;
using Editor.ViewModel.Info;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Server.Inspector
{
    public class InspectorManager
    {
        public static InspectorManager Instance { get; }= new Lazy<InspectorManager>(()=>new InspectorManager()).Value;

        public event EventHandler<SceneNode>? OnInspectSceneNode;
        public event EventHandler<ResourceItemViewModel?>? OnInspectResource;
        public void InspectSceneNode(SceneNode node)
        {
            OnInspectSceneNode?.Invoke(this, node);
        }
        public void InspectResource(ResourceItemViewModel? resource)
        {
            OnInspectResource?.Invoke(this, resource);
        }
        public void InspectModel()
        { }
    }
}
