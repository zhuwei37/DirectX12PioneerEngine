using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.RenderDx
{
    public enum ResourceType
    {
        NONE,
        /// <summary>
        /// 网格
        /// </summary>
        MESH,
        /// <summary>
        /// 模型
        /// </summary>
        MODEL,
        /// <summary>
        /// 纹理
        /// </summary>
        TEXTURE,
        /// <summary>
        /// 材质
        /// </summary>
        MATERIAL,
        /// <summary>
        /// 文件夹
        /// </summary>
        FOLDER,
    }
}
