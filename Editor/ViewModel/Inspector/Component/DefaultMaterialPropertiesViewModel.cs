using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using Editor.EditorAttribute;
using Editor.RenderDx;
using Editor.ViewModel.Inspector.Component;
using ResourceID = System.Guid;
namespace Editor.ViewModel.Inspector
{
   
    public class DefaultMaterialPropertiesViewModel:MaterialPropertyListViewModel
    {


        private ResourceID materialResourceID;


        private long _gRoughnessPropertyID;
        private float _gRoughness;
        public DefaultMaterialPropertiesViewModel(ResourceID matResourceID)
        {
            this.materialResourceID = matResourceID;
            _gRoughnessPropertyID = EngineDxImport.MaterialNameToPropertyID("gRoughness");
            _gRoughness = EngineDxImport.GetMaterialPropertyFloat(this.materialResourceID, _gRoughnessPropertyID);
            gDiffuseAlbedo = new Vector4ViewModel(this.materialResourceID, EngineDxImport.MaterialNameToPropertyID("gDiffuseAlbedo"));
            gFresnelR0 = new Vector3ViewModel(this.materialResourceID, EngineDxImport.MaterialNameToPropertyID("gFresnelR0"));
            MainTex = new MaterialTextureViewModel(this.materialResourceID, EngineDxImport.MaterialNameToPropertyID("MainTex"));
            NormalTex = new MaterialTextureViewModel(this.materialResourceID, EngineDxImport.MaterialNameToPropertyID("NormalTex"));
        }
        
        [MaterialPropertyName("gDiffuseAlbedo")]
        [MaterialProperty(Interface.Component.MaterialPropertyType.Float4)]
        public Vector4ViewModel gDiffuseAlbedo { get; set; } 



        [MaterialPropertyName("gFresnelR0")]
        [MaterialProperty(Interface.Component.MaterialPropertyType.Float3)]
        public Vector3ViewModel gFresnelR0 { get; set; }




       
        [MaterialPropertyName("gRoughness")]
        [MaterialProperty( Interface.Component.MaterialPropertyType.Float)]
        public float gRoughness { 
            get => _gRoughness;
            set 
            {
                if (SetProperty(ref _gRoughness, value))
                {
                    EngineDxImport.SetMaterialPropertyFloat(this.materialResourceID,_gRoughnessPropertyID, value);
                }
            }
        }
        [MaterialPropertyName("MainTex")]
        [MaterialProperty(Interface.Component.MaterialPropertyType._Texture)]
        public MaterialTextureViewModel MainTex 
        {
            get;
            set;
        }
        [MaterialPropertyName("NormalTex")]
        [MaterialProperty(Interface.Component.MaterialPropertyType._Texture)]
        public MaterialTextureViewModel NormalTex
        {
            get;set;
        }

    }
}
