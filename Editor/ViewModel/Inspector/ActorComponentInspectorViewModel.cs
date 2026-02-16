using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using Editor.Interface.Component;
using Editor.RenderDx;
using Editor.Server.Scene;
using Editor.ViewModel.Inspector.Component;
namespace Editor.ViewModel.Inspector
{
    public class ActorComponentInspectorViewModel:ObservableObject, IMaterialListView
    {

        SceneNode curSceneNode;

        public ObservableCollection<IComponent> Components { get; set; }=new ObservableCollection<IComponent>();

        public ObservableCollection<MaterialViewModel> Materials { get; set; }=new ObservableCollection<MaterialViewModel>();

        public static List<string> ComponentTypes { get; set; } = new List<string>()
        {
            "MeshFilter",
            "MeshRenderer"
        };
        public ICommand AddComponentCommand { get; private set; }
        public ICommand RemoveComponentCommand { get; private set; }

        string sceneNodeName;
        public string SceneNodeName
        {
            get { return curSceneNode.Title; }
            set 
            {
                curSceneNode.Title = value;
                OnPropertyChanged(nameof(SceneNodeName));
            }
        }


        public ActorComponentInspectorViewModel(SceneNode node)
        {
            AddComponentCommand = new RelayCommand<string>(AddComponent);
            RemoveComponentCommand = new RelayCommand<IComponent>(RemovceComponent);
            Reset(node);
          
        }

      
       public void Reset(SceneNode node)
        { 
            
            curSceneNode = node;
            SceneNodeName = node.Title;
            Components.Clear();
            //Components.Add(new TransformComponent());
           
            var componentInfos = EngineDxImport.GetComponents(curSceneNode.ActorID);
            Materials.Clear();
            foreach (var component in ComponentCreator.BuildComponentInspector(curSceneNode.ActorID, componentInfos))
            {
                Components.Add(component);
                if (component is MeshRendererComponent meshRenderer)
                {
                    meshRenderer.SetMaterialListView(this);
                }
            }
        }
        private void AddComponent(string? componentName)
        {
            if (componentName != null)
            {
                var component=ComponentCreator.CreateComponent(this.curSceneNode.ActorID, componentName);
                if (component != null)
                {
                    this.Components.Add(component);
                    if (component is MeshRendererComponent meshRenderer)
                    {
                        meshRenderer.SetMaterialListView(this);
                    }
                }
            }
        }
        private void RemovceComponent(IComponent? component)
        {
            if (component != null)
            {
                component.Destory();
                this.Components.Remove(component);
            }
        }

        public void AddMaterialView(MaterialViewModel vm)
        {
            this.Materials.Add(vm);
        }

        public void RemoveMaterialView(MaterialViewModel vm)
        {
          this.Materials.Remove(vm);
        }
        ~ActorComponentInspectorViewModel()
        {
            
        }
    }
}
