using CommunityToolkit.Mvvm.ComponentModel;
using Editor.Interface.UI;
using Editor.Server.Scene;
using Editor.View;
using Editor.ViewModel.Info;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.ViewModel.Inspector
{
    public class InspectorMainViewModel : ObservableObject, ITool
    {
        public ToolType UIToolType { get ; set ; }=ToolType.Tool;
        public string Title { get; set; } = "检查器";
        Editor.ViewModel.Inspector.NULLInspectorViewModel NULLInspector = new NULLInspectorViewModel();
        public InspectorMainViewModel()
        {
            Editor.Server.Inspector.InspectorManager.Instance.OnInspectSceneNode += SelectSceneNodeHandle;
            Editor.Server.Inspector.InspectorManager.Instance.OnInspectResource += SelectResourceHandle;


            currentViewModel=NULLInspector;
        }


        private object currentViewModel;
        public object CurrentViewModel 
        {
            get { return currentViewModel; }
            set { SetProperty(ref currentViewModel, value); }
        }
        private void SelectResourceHandle(object? sender, ResourceItemViewModel? resource)
        {
            if (resource != null)
            {
                if (resource.ResourceType == RenderDx.ResourceType.MATERIAL)
                {
                    if (CurrentViewModel is MaterialInpectorViewModel materialInpector)
                    {
                        materialInpector.Reset(resource);
                    }
                    else
                    {
                        this.CurrentViewModel = new MaterialInpectorViewModel(resource);
                    }
                       
                }
            }
            else
            {
                CurrentViewModel = NULLInspector;
            }
        }
        private void SelectSceneNodeHandle(object? sender, SceneNode sceneNode)
        {
            if (sceneNode != null)
            {
                if (CurrentViewModel is ActorComponentInspectorViewModel actor)
                {
                    actor.Reset(sceneNode);
                }
                else
                {
                    CurrentViewModel = new ActorComponentInspectorViewModel(sceneNode);
                }
            }
            else
            {
                CurrentViewModel = NULLInspector;
            }
        }
    }
}
