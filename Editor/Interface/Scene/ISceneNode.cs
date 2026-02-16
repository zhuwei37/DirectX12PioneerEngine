using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Animation;

namespace Editor.Interface.Scene
{
    public interface ISceneNode
    {
        public uint ID { get; set; }
        public uint ParentID { get; set; }
        public string Title { get; set; }
        public ISceneNode Parent { get; set; }
        string Tag { get; set; }
        string Layout { get; set; }

        ObservableCollection<ISceneNode> Children { get; set; }
    }
}
