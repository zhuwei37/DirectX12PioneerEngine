using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using ResourceID = System.Guid;
namespace Editor.ViewModel.Inspector.Component
{
    public class Vector2ViewModel : MaterialPropertyViewModel
    {
      
        public Vector2ViewModel(ResourceID materialResourceID,int propertyID)
        {
            this.MaterialResourceID = materialResourceID;
            this.MaterialPorpertyID = propertyID;
            
        }
        private float x;
        public float X
        {
            get { return x; }
            set 
            {
                SetProperty(ref x, value);
            }
        }
        private float y;
        public float Y
        {
            get { return y; }
            set { 
                SetProperty(ref y, value);
            }
        }
    }
}
