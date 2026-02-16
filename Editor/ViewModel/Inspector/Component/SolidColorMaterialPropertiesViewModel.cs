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
    public class SolidColorMaterialPropertiesViewModel: MaterialPropertyListViewModel
    {
        private ResourceID materialResourceID=new ResourceID();
        public SolidColorMaterialPropertiesViewModel(ResourceID matResourceID)
        {
            materialResourceID = matResourceID;
            var gColorPropertyID=EngineDxImport.MaterialNameToPropertyID(nameof(gColor));
            gColor = new Vector4ViewModel(materialResourceID, gColorPropertyID);
        }
        [MaterialPropertyName("gColor")]
        [MaterialProperty( Interface.Component.MaterialPropertyType.Float4)]
        public Vector4ViewModel gColor
        { get; set; }
    }
}
