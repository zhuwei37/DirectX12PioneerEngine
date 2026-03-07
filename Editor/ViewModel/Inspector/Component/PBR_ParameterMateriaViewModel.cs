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
    public class PBR_ParameterMateriaViewModel: MaterialPropertyListViewModel
    {
        private ResourceID materialResourceID = new ResourceID();
        private int roughnessPropertyID;
        private int metalnessPropertyID;
        public PBR_ParameterMateriaViewModel(ResourceID resourceID)
        {
            materialResourceID= resourceID;
            var gAlbedoPropertyID = EngineDxImport.MaterialNameToPropertyID(nameof(gAlbedo));
            gAlbedo = new Vector3ViewModel(materialResourceID, gAlbedoPropertyID);
            roughnessPropertyID = EngineDxImport.MaterialNameToPropertyID(nameof(gRoughness));
            metalnessPropertyID=EngineDxImport.MaterialNameToPropertyID(nameof(gMetalness));


            roughness = EngineDxImport.GetMaterialPropertyFloat(materialResourceID, roughnessPropertyID);
            metalness = EngineDxImport.GetMaterialPropertyFloat(materialResourceID, metalnessPropertyID);
        }

        private float roughness;
        private float metalness;

        [MaterialPropertyName(nameof(gAlbedo))]
        [MaterialProperty( Interface.Component.MaterialPropertyType.Float3)]
        public Vector3ViewModel gAlbedo { get; set; }

        [MaterialPropertyName(nameof(gRoughness))]
        [MaterialProperty( Interface.Component.MaterialPropertyType.Float)]
        public float gRoughness { get { return roughness; } 
            set 
            {
                if (SetProperty(ref roughness, value))
                {
                    EngineDxImport.SetMaterialPropertyFloat(materialResourceID, roughnessPropertyID, value);
                }
            } }


        [MaterialPropertyName(nameof(gMetalness))]
        [MaterialProperty( Interface.Component.MaterialPropertyType.Float)]
        public float gMetalness {
            get { return metalness; }
            set 
            {
                if (SetProperty(ref metalness, value))
                {
                    EngineDxImport.SetMaterialPropertyFloat(materialResourceID, metalnessPropertyID, value);
                }
            }
        }
    }
    
}
