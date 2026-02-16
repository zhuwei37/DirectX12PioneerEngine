using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Interface.Component
{
    public interface IComponent
    {
        public string ComponentName { get; }
        public bool CanClose { get; }

        public void Destory() { }
        //void Destory();
    }
}
