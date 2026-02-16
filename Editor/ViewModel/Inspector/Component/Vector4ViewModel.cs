using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using Editor.RenderDx;
using ResourceID = System.Guid;
namespace Editor.ViewModel.Inspector.Component
{
    public class Vector4ViewModel: MaterialPropertyViewModel
    {
        private Vector4 data;
        public Vector4ViewModel(ResourceID materialResourceID, long propertyID) 
        {
            this.MaterialResourceID = materialResourceID;
            this.MaterialPorpertyID = propertyID;
            data = EngineDxImport.GetMaterialPropertyFloat4(this.MaterialResourceID, this.MaterialPorpertyID);
        }
        
        public float X
        {
            get { return data.X; }
            set
            {
                if (SetProperty(ref data.X, value))
                {
                    EngineDxImport.SetMaterialPropertyFloat4(this.MaterialResourceID, this.MaterialPorpertyID, data);
                }
            }
        }
       
        public float Y
        {
            get { return data.Y; }
            set
            {
                if (SetProperty(ref data.Y, value))
                {
                    EngineDxImport.SetMaterialPropertyFloat4(this.MaterialResourceID, this.MaterialPorpertyID, data);
                }
            }
        }
       
        public float Z
        {
            get { return data.Z; }
            set 
            {
                if (SetProperty(ref data.Z, value))
                {
                    EngineDxImport.SetMaterialPropertyFloat4(this.MaterialResourceID, this.MaterialPorpertyID, data);
                }
            }
        }
       
        public float W
        {
            get { return data.W; }
            set 
            {
                if (SetProperty(ref data.W, value))
                {
                    EngineDxImport.SetMaterialPropertyFloat4(this.MaterialResourceID, this.MaterialPorpertyID, data); 
                }
            }
        }
    }
}
