using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Editor.Interface.UI;
using Editor.RenderDx;
using Editor.Server.Resource;
using Editor.View;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Input;
using ResourceID = System.Guid;
namespace Editor.ViewModel.Info
{
    public partial class ResourcePanelViewModel:ObservableObject,ITool
    {

        public ToolType UIToolType { get; set; } = ToolType.Info;
        public string Title { get; set; } = "资源";

        public ICommand FolderTreeSelectedCommand { get; set; }
        //public ICommand ResourceListS


        public ObservableCollection<ResourceItemViewModel> Resources { get; set; }
        private System.ComponentModel.ICollectionView folders;
        public System.ComponentModel.ICollectionView Folders
        {
            get => folders;
            set { SetProperty(ref folders, value); }
        }
        private ObservableCollection<ResourceItemViewModel> folderChildrenResources;
        public ObservableCollection<ResourceItemViewModel> FolderChildrenResources
        {
            get => folderChildrenResources;
            set { SetProperty(ref folderChildrenResources, value); }
        }


        private ObservableCollection<FolderLevelViewMdeol> _FolderLevels = new ObservableCollection<FolderLevelViewMdeol>();
        public ObservableCollection<FolderLevelViewMdeol> FolderLevels
        {
            get => _FolderLevels;
            set { SetProperty(ref _FolderLevels, value); }
        }

        private ResourceItemViewModel selectedResourceItem;
        public ResourceItemViewModel SelectedResourceItem
        {
            get => selectedResourceItem;
            set {
                SetProperty(ref selectedResourceItem, value);
                this.InspectResource(selectedResourceItem);
                    }
        }
        public ICommand CreateFolderCommand { get; private set; }
        public ICommand ImportTextureCommand { get; private set; }
       
        public ICommand ImportModelCommand { get; private set; }
        public ICommand OpenFileExplorerCommand { get; private set; }
        public ICommand DeleteResourceCommand { get; private set; }
        public ICommand RenameResourceCommand { get; private set; }
        public ICommand RenameResourceCompletedCommand { get; private set; }


        public ICommand DoubleClickResourceCommand { get; private set; }

        public ResourcePanelViewModel()
        {
            Resources = Resources =ResourceManager.Instance.GetResourceTree();
            Folders=CollectionViewSource.GetDefaultView(Resources);
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



            InitCommand();
            FolderTreeSelected(Resources[0]);
        }
        private void OpenFileExplorer(ResourceItemViewModel? item)
        {
            if (item == null&&FolderLevels.Count>0)
            {
                item = FolderLevels[FolderLevels.Count - 1].ResourceItem;
            }
            if (item != null)
            {
                System.Diagnostics.Process.Start("Explorer.exe", item.Path);
            }

        }
        private ResourceItemViewModel getResourceParent(ResourceItemViewModel? item)
        {
            ResourceItemViewModel parent=null;
            if (item == null && FolderLevels.Count > 0)
            {
                parent = FolderLevels[FolderLevels.Count - 1].ResourceItem;
            }
            else if(item!=null)
            {
                parent = item;
            }
            return parent;
        }
        private string getNewResourceName(string parentPath,string defaultName, string extension , bool isFile=false )
        {
            string newName = defaultName;
            string path = "";
            for (int i = 1; i < 200; i++)
            {
               
                bool hasFolder;
                if (isFile)
                {
                    path = System.IO.Path.Join(parentPath, $"{newName}{i}.{extension}");
                    hasFolder =System.IO.File.Exists(path);
                }
                else
                {
                    path = System.IO.Path.Join(parentPath, $"{newName}{i}");
                    hasFolder =System.IO.Directory.Exists(path);
                }

                if (!hasFolder)
                {
                    break;
                }
            }
            return path;
        }
        private void CreateFolder(ResourceItemViewModel? item)
        {
            if (item == null && FolderLevels.Count > 0)
            {
                item = FolderLevels[FolderLevels.Count - 1].ResourceItem;
            }
            if (item != null)
            {
                var resourceType = item.ResourceType;
                string parentPath;
                ResourceID parentId;
                ResourceItemViewModel parent;
                if (resourceType == ResourceType.FOLDER)
                {
                    parent = item;
                    parentId = item.resourceID;
                    parentPath = item.Path;
                }
                else 
                {
                    parent = item.Parent;
                    if (parent == null)
                    {
                        return;
                    }
                    parentId = parent.resourceID;
                    parentPath = parent.Path;
                }
                string newName="文件夹";
                string path="";
                for (int i = 1; i < 200; i++)
                {
                    path=System.IO.Path.Join(parentPath,$"{newName}{i}");
                    var hasFolder = System.IO.Directory.Exists(path);
                    if (!hasFolder)
                    {
                        break;
                    }
                }
                using (StringExport pathExport = new StringExport(path))
                {
                    ResourceInfo info = new ResourceInfo(); ;
                    if (EngineDxImport.CreateResource(ResourceType.FOLDER, parentId, pathExport.Ptr, ref info))
                    {
                        ResourceItemViewModel resource = new ResourceItemViewModel(info.resourceType)
                        {
                            Parent = parent,
                            Path = path,
                            resourceID=info.resourceID,
                             Title=System.IO.Path.GetFileNameWithoutExtension(path)
                        };
                        parent.Children.Add(resource);
                        RefreshFolderChildrenView(parent);
                    }

                }
            }
        }
        private void DeleteResource(ResourceItemViewModel? item)
        {
            if (item != null)
            {
                if (item.Parent != null)
                {
                    EngineDxImport.DeleteResource(item.resourceID, item.Parent.resourceID);
                    item.Parent.Children.Remove(item);
                    RefreshFolderChildrenView(item.Parent);
                }
                        
            }
        }
        private void RenameResource(ResourceItemViewModel? item)
        {
            if (item!=null)
            {
                item.IsEditable = true;
            }
        }
        private void RenameResourceCompleted(object? obj)
        { 

        }
        private void DoubleClickResource(ResourceItemViewModel? item)
        {
            if (item != null)
            {
                switch (item.ResourceType)
                {
                    case ResourceType.FOLDER:
                        {
                            FolderTreeSelected(item);
                            break;
                        }
                    default:break;
                }
              
            }
        }
        private void FolderTreeSelected(ResourceItemViewModel? folder)
        {
            if (folder != null)
            {
                FolderChildrenResources =new ObservableCollection<ResourceItemViewModel>( folder.Children);
                var root = Resources[0];
                FolderLevels.Clear();
                FolderLevelViewMdeol folderLevel = new FolderLevelViewMdeol() { FolderName = "asset", ResourceItem = Resources[0] };
                FolderLevels.Add(folderLevel);
                ResourceItemViewModel current=folder ;
                while (current != null&&current != root  )
                {
                    FolderLevels.Insert(1, new FolderLevelViewMdeol() { FolderName = current.Title, ResourceItem = current });
                    current = current.Parent;
                }
                FolderLevels[FolderLevels.Count-1].IsLast=true;
            }
        }
        private void RefreshFolderChildrenView(ResourceItemViewModel parentfolder)
        {
            if (parentfolder != null)
            {
                FolderChildrenResources = new ObservableCollection<ResourceItemViewModel>(parentfolder.Children);
                var root = Resources[0];
                FolderLevels.Clear();
                FolderLevelViewMdeol folderLevel = new FolderLevelViewMdeol() { FolderName = "asset", ResourceItem = Resources[0] };
                FolderLevels.Add(folderLevel);
                ResourceItemViewModel current = parentfolder;
                while (current != null && current != root)
                {
                    FolderLevels.Insert(1, new FolderLevelViewMdeol() { FolderName = current.Title, ResourceItem = current });
                    current = current.Parent;
                }
                FolderLevels[FolderLevels.Count - 1].IsLast = true;
            }
        }


    }
}
