using CommunityToolkit.Mvvm.ComponentModel;
using Editor.Interface.Scene;
using Editor.RenderDx;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
namespace Editor.Server.Scene
{
    public class SceneNode : ObservableObject, ISceneNode
    {
        public string title;
        public string Title
        {
            get => title;
            set
            {
                if (SetProperty(ref title, value))
                {
                    
                    EngineDxImport.SetSceneNodeName(ID, title);
                }
            }
        }
        public string Icon
        {
            get => "/Resource/Image/Icon/GameObject.png";
        }
        public ISceneNode Parent { get ; set; }
        public string Tag { get ; set; }
        public string Layout { get ; set ; }
        public ObservableCollection<ISceneNode> Children { get; set; } = new ObservableCollection<ISceneNode>();
        public uint ID { get ; set ; }=0;
        public uint ParentID { get ; set ; }
        public uint ActorID { get; set; }
        public int Count { get; set; }
        private bool isEditable=false;
        public bool IsEditable 
        {
            get { return isEditable; }
            set { SetProperty(ref isEditable, value); }
        }
        ~SceneNode()
        {
            System.Diagnostics.Debug.WriteLine($"销毁:{this.ID}");
        }
    }
}
