using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Editor.RenderDx;
using Editor.Server.Resource;
using Editor.View.Dialog;
using Editor.ViewModel.Info;
using GongSolutions.Wpf.DragDrop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using ResourceID = System.Guid;
namespace Editor.ViewModel.Inspector.Component
{
    public class MaterialTextureViewModel:MaterialPropertyViewModel, GongSolutions.Wpf.DragDrop.IDropTarget
    {
        private ResourceID textureResourceID = new ResourceID();

        
        public ICommand SelectTextureCommand { get; private set; }

        public MaterialTextureViewModel(ResourceID matResourceID, long propertyID)
        { 
            this.MaterialResourceID = matResourceID;
            this.MaterialPorpertyID = propertyID;
            var textureResourceID= EngineDxImport.GetMaterialPropertyTexture(this.MaterialResourceID, this.MaterialPorpertyID);
            var resource=ResourceManager.Instance.GetResource(textureResourceID);
            SetTexture(resource);
            SelectTextureCommand = new RelayCommand(SelectTexture);
        }
        private bool hasTexture;
        public bool HasTexture
        {
            get { return hasTexture; }
            set { SetProperty(ref hasTexture, value); }
        }

        public void DragOver(IDropInfo dropInfo)
        {
            if (dropInfo.Data is ResourceItemViewModel resource)
            {
                if (resource.ResourceType == RenderDx.ResourceType.TEXTURE)
                {
                    dropInfo.DropTargetAdorner = DropTargetAdorners.Highlight;
                    dropInfo.Effects= System.Windows.DragDropEffects.Move;
                }

            }
        }

        public void Drop(IDropInfo dropInfo)
        {
            if (dropInfo.Data is ResourceItemViewModel resource)
            {
                if (resource.ResourceType == RenderDx.ResourceType.TEXTURE)
                {
                    SetTexture(resource);
                }

            }
        }

        private void SelectTexture()
        {
            SelectResourceDialog selectResourceDialog = new SelectResourceDialog(ResourceType.TEXTURE);
            var result= selectResourceDialog.ShowDialog();
            if (result != null && result == true)
            {
                var resource= selectResourceDialog.GetSelectedResource();
               SetTexture(resource);
            }
        }
        private void SetTexture(ResourceItemViewModel? resource)
        {
            if (resource!=null&&resource != ResourceManager.Instance.ResourceNone&&resource.ResourceType== ResourceType.TEXTURE)
            {
                HasTexture = true;
                if (resource.resourceID != textureResourceID)
                {
                    textureResourceID = resource.resourceID;
                    EngineDxImport.SetMaterialPropertyTexture(this.MaterialResourceID, this.MaterialPorpertyID, textureResourceID);
                }
            }
            else
            {
                EngineDxImport.SetMaterialPropertyTexture(this.MaterialResourceID, this.MaterialPorpertyID, ResourceManager.Instance.ResourceID_NONE);
                hasTexture = false;
            }
        }
    }
}
