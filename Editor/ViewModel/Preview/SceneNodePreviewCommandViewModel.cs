using CommunityToolkit.Mvvm.Input;
using Editor.Server.Resource;
using Editor.Server.Scene;
using Editor.ViewModel.Inspector.Component;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Editor.ViewModel.Preview
{
    public partial  class SceneNodePreviewViewModel
    {
        public ICommand CreateSceneNodeCommand { get; private set; }
        public ICommand DeleteSceneNodeCommand { get; private set; }
        public ICommand RenameSceneNodeCommand { get; private set; }
        public ICommand SelectSceneNodeCommand { get; private set; }


       


        public ICommand CreateBoxGameObjectCommand { get; private set; }
        public ICommand CreateSphereGameObjectCommand {  get; private set; }
        public ICommand CreateGridGameObjectCommand { get; private set; }
        private void initCommand()
        {
            CreateSceneNodeCommand = new RelayCommand<SceneNode>(CreateSceneNode);
            DeleteSceneNodeCommand = new RelayCommand<SceneNode>(DeleteSceneNode);
            SelectSceneNodeCommand = new RelayCommand<SceneNode>(SelectSceneNode);
            RenameSceneNodeCommand = new RelayCommand<SceneNode>(RenameSceneNode);

            CreateBoxGameObjectCommand = new RelayCommand<SceneNode>(CreateBoxGameObject);
            CreateSphereGameObjectCommand=new RelayCommand<SceneNode>(CreateSphereGameObject);
            CreateGridGameObjectCommand = new RelayCommand<SceneNode>(CreateGridGameObject);
        }
        private void CreateBoxGameObject(SceneNode? node)
        {
            SceneNode parent = Root;
            if (node != null)
            {
                parent= node;
            }
            Create3DGameObject(parent,0,"Box");
        }
        private void CreateSphereGameObject(SceneNode? node)
        {
            SceneNode parent = Root;
            if (node != null)
            {
                parent = node;
            }
            Create3DGameObject(parent, 1,"Sphere");
        }
        private void CreateGridGameObject(SceneNode? node)
        {
            SceneNode parent = Root;
            if (node != null)
            {
                parent = node;
            }
            Create3DGameObject(parent, 2,"Grid");
        }
        private void Create3DGameObject(SceneNode parent, int shape,string defualtName)
        {
            if (parent == null)
            {
                return;
            }
            var sceneNode=CreateSceneNodeImp(parent, defualtName);
            if (sceneNode == null)
            {
                return;
            }
            var meshFilter=(MeshFilterComponent) ComponentCreator.CreateComponent(sceneNode.ActorID, "MeshFilter")!;
            var meshRenderer = (MeshRendererComponent)ComponentCreator.CreateComponent(sceneNode.ActorID, "MeshRenderer")!;
            meshRenderer.AddMaterial();
            meshRenderer.SetMaterial(0, ResourceManager.Instance.SolidColorMaterialResource);
            switch (shape)
            {
                case 0:
                    {
                        meshFilter.SetMeshResource(ResourceManager.Instance.BoxMeshResource.resourceID);
                        break;
                    }
                case 1:
                    {
                        meshFilter.SetMeshResource(ResourceManager.Instance.SphereMeshResource.resourceID);
                        break;
                    }
                case 2: 
                    {
                        meshFilter.SetMeshResource(ResourceManager.Instance.GridMeshResource.resourceID);
                        break;
                    }
                default:
                    break;
            }
        }

    }
}
