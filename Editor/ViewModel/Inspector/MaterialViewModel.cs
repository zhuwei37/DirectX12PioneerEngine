using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using Editor.RenderDx;
using Editor.ViewModel.Inspector.Component;
using ResourceID = System.Guid;
namespace Editor.ViewModel.Inspector
{
    public class MaterialViewModel:ObservableObject
    { 
        //public static DefaultMaterialPropertiesViewModel 
        public class ShaderViewModel
        {
            public string? ShaderName { get; set; }
            public int ShaderID { get; set; }
        }
        private ResourceID materialResourecID;
        public static ObservableCollection<ShaderViewModel> Shaders { get; set; } = new ObservableCollection<ShaderViewModel>()
        {
            new ShaderViewModel(){ ShaderName="DefaultDiffuse",ShaderID=0},
            new ShaderViewModel(){ ShaderName="SolidColor",ShaderID=1},
             new ShaderViewModel(){ ShaderName="NONE",ShaderID=2},
              new ShaderViewModel(){ ShaderName="NONE",ShaderID=3},
               new ShaderViewModel(){ ShaderName="NONE",ShaderID=4},
                new ShaderViewModel(){ ShaderName="NONE",ShaderID=5},
            new ShaderViewModel(){ ShaderName="PBR_Deferred",ShaderID=6},
        };
        
        public MaterialViewModel(ResourceID matResourceID)
        {
            materialResourecID= matResourceID;
            selectedShaderIndex = EngineDxImport.GetMaterialShaderID(matResourceID);
            SelectedShaderHandle();
        }
        private void SelectedShaderHandle()
        {
            //Shaders[]
            switch (SelectedShaderIndex)
            {
                case 0:
                    {
                        MaterialProperties = new DefaultMaterialPropertiesViewModel(materialResourecID);
                        break;
                    }
                case 1:
                    {
                        MaterialProperties = new SolidColorMaterialPropertiesViewModel(materialResourecID);
                        break;
                    }
                case 6:
                    {
                        MaterialProperties = new PBR_DeferredMaterialViewModel(materialResourecID);
                        break;
                    }
                default: break;
            }
        }
        private int selectedShaderIndex;
        public int SelectedShaderIndex
        {
            get { return selectedShaderIndex; }
            set 
            {
                if (SetProperty(ref selectedShaderIndex, value))
                {

                    EngineDxImport.SetMaterialShaderID(materialResourecID, SelectedShaderIndex);
                    SelectedShaderHandle();
                }
            }
        }
        private string materialName="MaterialTest";
        public string MaterialName
        {
            get { return materialName; }
            set { SetProperty(ref materialName, value); }
        }
        private MaterialPropertyListViewModel? materialProperties;
        public MaterialPropertyListViewModel? MaterialProperties {
            get
            {
                return materialProperties;
            }
            set 
            {
                SetProperty(ref materialProperties, value);
            } }
    }
}
