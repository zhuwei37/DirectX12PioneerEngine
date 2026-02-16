using Editor.RenderDx;
using Editor.Time;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace Editor.Server
{
    public class EngineManager
    {
        public static EngineManager 
            Instance { get; private set; } = new Lazy<EngineManager>(() => new EngineManager()).Value;
        private EngineManager() 
        {
           CompositionTarget.Rendering += CompositionTarget_Rendering;
        }

        private void CompositionTarget_Rendering(object? sender, EventArgs e)
        {
            GameTimer.Instance.Tick();
            EngineDxImport.Update();
        }
    }
}
