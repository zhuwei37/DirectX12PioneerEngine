using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Server.Scene
{
    public class RenameSceneNodeArg : EventArgs
    {
        public string NewName { get; set; }
        public string OldName { get; set; }
        public SceneNode RenameSceneNode { get; set; }
    }
    public class SceneManager
    {
        public static SceneManager Instance { get; private set; } = new Lazy<SceneManager>(() => new SceneManager()).Value;
        /// <summary>
        /// 删除场景节点
        /// </summary>
        public event EventHandler<SceneNode> OnRemoveSceneNode;
        /// <summary>
        /// 重命名场景节点
        /// </summary>
        public event EventHandler<RenameSceneNodeArg> OnRenameSceneNode;
        public SceneManager() 
        {

        }
    }
}
