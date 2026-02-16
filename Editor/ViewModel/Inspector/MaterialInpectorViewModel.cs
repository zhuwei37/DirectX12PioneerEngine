using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using Editor.ViewModel.Info;
namespace Editor.ViewModel.Inspector
{
    public class MaterialInpectorViewModel:ObservableObject
    {
        public MaterialInpectorViewModel(ResourceItemViewModel resource)
        {
            this.material = new MaterialViewModel(resource.resourceID);
        }
        public void Reset(ResourceItemViewModel resource)
        {
            this.Material = new MaterialViewModel(resource.resourceID);
        }
        private MaterialViewModel material;
        public MaterialViewModel Material
        {
            get { return material; }
            set { SetProperty(ref material, value); }
        }
    }
}
