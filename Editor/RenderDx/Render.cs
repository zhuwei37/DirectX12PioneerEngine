using Editor.Time;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Editor.RenderDx
{

   public  enum RenderHostType
	{
		MainRendering,
		MaterialRendering,
		AnimationRendering,
    };
public class Render
    {
        public static Render Instance { get; }
        =new Lazy<Render>(() => { return new Render(); }).Value;
        public Render()
        { 
        }
        public uint Width { get; set; } = 800;
        public uint Height { get; set; } = 600;
        private bool Initialize;
        public void Init(IntPtr hWnd)
        {
            EngineDxImport.InitD3D(hWnd, Width, Height);
            if (ErrorManager.HasError)
            {
                throw ErrorManager.Retrieve();
            }
            Initialize=true;
        }
        public void OnRender()
        {
           
            GameTimer.Instance.Tick();
            EngineDxImport.Run();
        }
        public void Resize()
        {
            if (Initialize)
            {
                EngineDxImport.Resize(Width, Height);
                if (ErrorManager.HasError)
                {
                    throw ErrorManager.Retrieve();
                }
            }
            }
        public IntPtr GetSharedTextureHandle()
        {
            return EngineDxImport.GetSharedTextureHandle();
        }
    }
}
