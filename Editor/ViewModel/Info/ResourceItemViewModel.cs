using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Editor.RenderDx;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Input;
using ResourceID = System.Guid;
namespace Editor.ViewModel.Info
{
    public class ResourceItemViewModel:ObservableObject
    {
        public ResourceItemViewModel Parent 
        {
            get;
            set;
        }
        public ResourceID resourceID { get; set; }
        public ResourceItemViewModel(ResourceType resourceType)
        {
            
            ResourceType = resourceType;
            
            Folders = CollectionViewSource.GetDefaultView(Children);

            Folders.Filter = new Predicate<object>((resource) =>
            {

                if (resource != null
                && resource is ResourceItemViewModel resource1
                && resource1.ResourceType == ResourceType.FOLDER)
                {
                    return true;
                }
                return false;
            });
        }
        public string Icon
        {
            get
            {
                if (ResourceType == ResourceType.FOLDER)
                {
                    return
                        "/Resource/Image/Icon/Folder.png";
                }
                else if (ResourceType == ResourceType.TEXTURE)
                {
                    return "/Resource/Image/Icon/Texture.png";
                }
                else if (ResourceType == ResourceType.MESH)
                {
                    return "/Resource/Image/Icon/Mesh.png";
                }
                else if (ResourceType == ResourceType.MATERIAL)
                {
                    return "/Resource/Image/Icon/Material.png";
                }
                return "/Resource/Image/Icon/UnknowFile.png";
            }
        }
        private string title;
        public string Title
        {
            get => title;
            set{ 
                SetProperty(ref title, value);
            }
        }
        public readonly ResourceType ResourceType;
        ObservableCollection<ResourceItemViewModel> children=new ObservableCollection<ResourceItemViewModel>();
        public ObservableCollection<ResourceItemViewModel> Children
        {
            get => children;
            set
            {
                SetProperty(ref children, value);
            }
        }
        private System.ComponentModel.ICollectionView folders;

        public System.ComponentModel.ICollectionView Folders 
        {
            get => folders;
            set { SetProperty(ref folders, value); }
        }
        public string Path { get; set; }

        private bool isEditable;
        public bool IsEditable
        {
            get => isEditable;
            set 
            {
                SetProperty(ref isEditable, value);
            }
        }
        private bool isEditableInFolderView;
        /// <summary>
        /// 文件详情下是否编辑名称
        /// </summary>
        public bool IsEditableInFolderView
        {
            get => isEditableInFolderView;
            set { SetProperty(ref isEditableInFolderView, value); }
        }
        /// <summary>
        /// 是否只存在内存中
        /// </summary>
        public bool InMemery { get; set; } = false;

    }
}
