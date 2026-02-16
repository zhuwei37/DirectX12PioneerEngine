using Editor.EditorAttribute;
using Editor.RenderDx;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ResourceID = System.Guid;
namespace Editor.ViewModel.Inspector.Component
{
    public class PBR_DeferredMaterialViewModel: MaterialPropertyListViewModel
    {
        private ResourceID materialResourceID;
        public PBR_DeferredMaterialViewModel(ResourceID matResourceID)
        { 
            this.materialResourceID = matResourceID;
            BaseColorTex = new MaterialTextureViewModel(materialResourceID, EngineDxImport.MaterialNameToPropertyID("BaseColorTex"));
            NormalTex = new MaterialTextureViewModel(materialResourceID, EngineDxImport.MaterialNameToPropertyID("NormalTex"));
            MetallicTex = new MaterialTextureViewModel(materialResourceID, EngineDxImport.MaterialNameToPropertyID("MetallicTex"));
            RoughnessTex = new MaterialTextureViewModel(materialResourceID, EngineDxImport.MaterialNameToPropertyID("RoughnessTex"));
        }
        [MaterialPropertyName("BaseColorTex")]
        [MaterialProperty( Interface.Component.MaterialPropertyType._Texture)]
        public MaterialTextureViewModel BaseColorTex { get; set; }

        [MaterialPropertyName("NormalTex")]
        [MaterialProperty(Interface.Component.MaterialPropertyType._Texture)]
        public MaterialTextureViewModel NormalTex { get; set; }

        [MaterialPropertyName("MetallicTex")]
        [MaterialProperty( Interface.Component.MaterialPropertyType._Texture)]
        public MaterialTextureViewModel MetallicTex { get; set; }

        [MaterialPropertyName("RoughnessTex")]
        [MaterialProperty( Interface.Component.MaterialPropertyType._Texture)]
        public MaterialTextureViewModel RoughnessTex { get; set; }
    }
}
