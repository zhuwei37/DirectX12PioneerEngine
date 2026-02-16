using CommunityToolkit.Mvvm.Collections;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Editor.Interface.Scene;
using Editor.Interface.UI;
using Editor.RenderDx;
using Editor.Server.Inspector;
using Editor.Server.Scene;
using Editor.View;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Vortice.Direct3D11;
namespace Editor.ViewModel.Preview
{
    public partial class SceneNodePreviewViewModel : ObservableObject, ITool
    {
        public ToolType UIToolType { get ; set; }=ToolType.Preview;
        public string Title
        { 
            get;
            set;
        } = "层级";



        private SceneNode Root;
        public ObservableCollection<ISceneNode> SceneNodes 
        {
            get 
            {
                if (Root != null)
                {
                    return Root.Children;
                }
                return null;
            }
            set 
            {
                OnPropertyChanged(new System.ComponentModel.PropertyChangedEventArgs(nameof(SceneNodes)));
            }
        }

  
        
       


        public SceneNodePreviewViewModel()
        {
            Root = new SceneNode();
            initCommand();
          
            //EngineDxImport.CreateSceneNode("1234sfsf", 1);
            //EngineDxImport.RemoveSceneNode(3);
            LoadMainSceneNode();
        }
        private void SelectSceneNode(SceneNode? sceneNode)
        {
           
            InspectorManager.Instance.InspectSceneNode(sceneNode);
        }
        private void CreateSceneNode(SceneNode? node)
        {
            SceneNode currentNode;
            if (node != null)
            {
                currentNode = node;
            }
            else 
            {
                currentNode = Root;
            }
            CreateSceneNodeImp(currentNode,"GameObject");
        }
        private SceneNode? CreateSceneNodeImp(SceneNode parentNode,string defualtName)
        {
            var info = EngineDxImport.CreateSceneNode("GameObject", parentNode.ID);
            var newnodeID = info.SceneNodeID;
            if (newnodeID != 0)
            {
                string title = defualtName;
                for (int i = 1; i < 1000; i++)
                {
                   
                    var result= parentNode.Children.Where(item => { return item.Title == title; }).Count();
                    if (result==0)
                    {
                        break;
                    }
                    title = defualtName + i.ToString();
                }
                SceneNode sceneNode = new SceneNode();
                sceneNode.Title = title;

                sceneNode.ID = newnodeID;
                sceneNode.ParentID = parentNode.ID;
                sceneNode.Parent = parentNode;
                sceneNode.ActorID = info.ActorId;
                parentNode.Children.Add(sceneNode);
                System.Diagnostics.Debug.WriteLine($"创建:{sceneNode.ID}");
                return sceneNode;
            }
            return null;
        }
        private void DeleteSceneNode(SceneNode? node)
        {
            if (node != null)
            {
                var parent = node.Parent;
                if (parent != null)
                {
                    EngineDxImport.RemoveSceneNode(node.ID);
                    parent.Children.Remove(node);
                   // Root.Children.Clear();

                }
            }
        }
        public void RenameSceneNode(SceneNode? node)
        {
            if (node != null)
            {
                node.IsEditable = true;
            }
        }
        public async Task LoadMainSceneNode()
        {
           // return;
            var scenenodes = EngineDxImport.GetAllSceneNodeInfos();
            if (scenenodes == null)
            {
                return;
            }
            Queue<SceneNode> queue = new Queue<SceneNode>();
            queue.Enqueue(Root);
            int index = 1;
            Root.Count = scenenodes[0].ChildCount;

            while (queue.Count > 0)
            {
                var parent = queue.Dequeue();
                int childCount = parent.Count;
                for (int j = 0; j < childCount; j++)
                {
                    var node = new SceneNode()
                    {
                        title = scenenodes[index + j].Name,
                        ParentID = parent.ID,
                        ID = scenenodes[index + j].SceneNodeID,
                        Parent = parent,
                        Count = scenenodes[index + j].ChildCount,
                        ActorID = scenenodes[index + j].ActorId
                    };
                    parent.Children.Add(node);
                    queue.Enqueue(node);

                }
                index += childCount;
            }
         
        }
        ~SceneNodePreviewViewModel() 
        {
        }
    }
}
