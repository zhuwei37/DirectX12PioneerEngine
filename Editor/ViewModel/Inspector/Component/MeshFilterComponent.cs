using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Editor.Interface.Component;
using Editor.RenderDx;
using Editor.Server.Resource;
using Editor.View.Dialog;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using ResourceID = System.Guid;
namespace Editor.ViewModel.Inspector.Component
{
    public class MeshFilterComponent : ObservableObject, IComponent
    {
        public string ComponentName => "MeshFilter";

        public bool CanClose => true;
        private IntPtr _handle;
        private uint actorId;
        private ResourceID _resourceID;



        private string selectedMeshName=string.Empty;
        public string SelectedMeshName
        {
            get => selectedMeshName;
            set 
            {
                SetProperty(ref selectedMeshName, value);
            }
        }

        public ICommand SelectMeshCommand { get; private set; }
        public MeshFilterComponent(uint actorId,nint handle)
        {
            this._handle = handle;
            this.actorId = actorId;
            SelectMeshCommand = new RelayCommand(SelectMesh);
        
            EngineDxImport.GetMeshFilter_MeshResourceID(this._handle, ref _resourceID);
            SetMeshResource(_resourceID);
        }
        public void SetMeshResource(ResourceID resourceID)
        {
            EngineDxImport.SetMeshFilter_MeshResourceID(this._handle, resourceID);
            var meshResource = ResourceManager.Instance.GetResource(resourceID);
            if (meshResource != null&&meshResource!=ResourceManager.Instance.ResourceNone)
            {
                SelectedMeshName = meshResource.Title;
            }
            else
            {
                SelectedMeshName = "无(网格)";
            }
        }
        private void SelectMesh()
        {
            SelectResourceDialog selectResourceDialog = new SelectResourceDialog(ResourceType.MESH);
            var result=selectResourceDialog.ShowDialog();
            if (result != null && result == true)
            {
               var selectedResource =selectResourceDialog.GetSelectedResource();
                if (selectedResource != null)
                {

                    SetMeshResource(selectedResource.resourceID);
                }
            }
        }
       
    }
}
