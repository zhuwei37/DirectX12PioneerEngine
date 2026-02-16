using Editor.Interface.Component;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using Editor.RenderDx;
namespace Editor.ViewModel.Inspector.Component
{
    public class TransformComponent : ObservableObject, IComponent
    {
        private IntPtr _handle;
        private RenderDx.Vector3 pos;
        private RenderDx.Vector3 rotate;
        private RenderDx.Vector3 scale;
        public string ComponentName => "Transform";
        public bool CanClose { get => false; }
        private uint actorId;
        public TransformComponent(uint ActorId,IntPtr ptr)
        {
            _handle= ptr;
            actorId= ActorId;
            pos = EngineDxImport.Get_Transform_Position(_handle);
            rotate=EngineDxImport.Get_Transform_Rotate(_handle);
            scale=EngineDxImport.Get_Transform_Scale(_handle);
        }
     
        public TransformComponent() 
        {
           
        }
        public float Position_X
        {
            get
            {
                return pos.X;
            }
            set
            {
                if (SetProperty(ref pos.X, value))
                {
                    EngineDxImport.Set_Transform_Position(_handle, pos);
                }
            }
        }
        public float Position_Y
        {
            get
            {
                return pos.Y;
            }
            set
            {
                if (SetProperty(ref pos.Y, value))
                {
                    EngineDxImport.Set_Transform_Position(_handle, pos);
                }
            }
        }
        public float Position_Z
        {
            get
            {
                return pos.Z;
            }
            set
            {
                if (SetProperty(ref pos.Z, value))
                {
                    EngineDxImport.Set_Transform_Position(_handle, pos);
                }
            }
        }
        public float Rotate_X 
        {
            get
            {
                return rotate.X;
            }
            set
            {
                if (SetProperty(ref rotate.X, value))
                {
                    EngineDxImport.Set_Transform_Rotate(_handle, rotate);
                }
            }
        }
        public float Rotate_Y
        {
            get
            {
                return rotate.Y;
            }
            set
            {
                if (SetProperty(ref rotate.Y, value))
                {
                    EngineDxImport.Set_Transform_Rotate(_handle, rotate);
                }
            }
        }
        public float Rotate_Z
        {
            get 
            {
                return rotate.Z;
            }
            set
            {
                if (SetProperty(ref rotate.Z, value))
                {
                    EngineDxImport.Set_Transform_Rotate(_handle, rotate);
                }
            }
        }
        public float Scale_X
        {
            get
            {
                return scale.X;
            }
            set
            {
                if (SetProperty(ref scale.X, value))
                {
                    EngineDxImport.Set_Transform_Scale(_handle, scale);
                }
            }
        }
        public float Scale_Y
        {
            get
            {
                return scale.Y;
            }
            set
            {
                if (SetProperty(ref scale.Y, value))
                {
                    EngineDxImport.Set_Transform_Scale(_handle, scale);
                }
            }
        }
        public float Scale_Z
        {
            get
            {
                return scale.Z;
            }
            set
            {
                if (SetProperty(ref scale.Z, value))
                {
                    EngineDxImport.Set_Transform_Scale(_handle, scale);
                }
            }
        }
    }
}
