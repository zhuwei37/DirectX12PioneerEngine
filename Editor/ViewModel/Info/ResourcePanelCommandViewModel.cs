using CommunityToolkit.Mvvm.Input;
using Editor.Server.Resource;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Editor.ViewModel.Info
{
    public partial class ResourcePanelViewModel
    {
        public ICommand SelectFileLevelCommnad { get; private set; }

        public ICommand InspectResourceCommand { get; private set; }
        public ICommand CreateMaterialCommand { get; private set; }

        private void InitCommand()
        {
            FolderTreeSelectedCommand = new RelayCommand<ResourceItemViewModel>(FolderTreeSelected);
            //资源处理
            OpenFileExplorerCommand = new RelayCommand<ResourceItemViewModel>(OpenFileExplorer);
            CreateFolderCommand = new RelayCommand<ResourceItemViewModel>(CreateFolder);
            DeleteResourceCommand = new RelayCommand<ResourceItemViewModel>(DeleteResource);
            RenameResourceCommand = new RelayCommand<ResourceItemViewModel>(RenameResource);
            RenameResourceCompletedCommand = new RelayCommand<object>(RenameResourceCompleted);

            DoubleClickResourceCommand = new RelayCommand<ResourceItemViewModel>(DoubleClickResource);

            CreateMaterialCommand = new RelayCommand<ResourceItemViewModel>(CreateMaterial);



            InspectResourceCommand = new RelayCommand<ResourceItemViewModel>(InspectResource);
            //FileLevel
            SelectFileLevelCommnad = new RelayCommand<FolderLevelViewMdeol>(SelectFileLevel);
        }
        private void CreateMaterial(ResourceItemViewModel? parentResource)
        {
           var parent= getResourceParent(parentResource);
            if (parent != null) 
            {
                string newName = getNewResourceName(parent.Path, "Material","Mat",isFile:true);
                if (ResourceManager.Instance.CreateResource(RenderDx.ResourceType.MATERIAL, parent, newName))
                {
                    RefreshFolderChildrenView(parent);
                }
            }

        }

        private void InspectResource(ResourceItemViewModel? item)
        {
           Editor.Server.Inspector.InspectorManager.Instance.InspectResource(item);
        }
        private void SelectFileLevel(FolderLevelViewMdeol? item)
        {
            if (item != null)
            {
                FolderTreeSelected(item.ResourceItem);
            }
        }
    }
}
