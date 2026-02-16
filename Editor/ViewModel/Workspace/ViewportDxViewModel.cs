using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using Editor.Interface.UI;
using Editor.RenderDx;
namespace Editor.ViewModel.Workspace
{
    public class ViewportDxViewModel : ObservableObject, IPage
    {
        public ViewportDxViewModel() 
        {

        }
        private int renderHostId=-1;
        private RenderHostType mRenderType;
        public string Title { get; set; } = "渲染视口";
        public int RenderHostId
        { 
            get { return renderHostId; }
            set {
                
                SetProperty(ref renderHostId, value); 
            }
        }
        public RenderHostType RenderType
        {
            get { return mRenderType; }
            set { SetProperty(ref this.mRenderType, value); }
        }
    }
}
