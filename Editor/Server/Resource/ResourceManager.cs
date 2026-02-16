using CommunityToolkit.Mvvm.ComponentModel;
using Editor.RenderDx;
using Editor.ViewModel.Info;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Net.WebSockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TextBox;
using ResourceID = System.Guid;
namespace Editor.Server.Resource
{
    public class ResourceManager
    {

        public static string BOX_MESH_RESOURCEID_STR { get; } = "{F0886A11-15F6-517D-4477-B0DE36B266E7}";
        public static string SPHERE_MESH_RESOURCEID_STR { get; } = "{50479C36-6EF0-2B66-50D9-C64CE4BB765A}";
        public static string GRID_MESH_RESOURCEID_STR { get; } = "{494BF320-AABD-4CF5-7886-1E68EC0ECD49}";
        public static string SOLID_COLOR_MATERIAL_RESOURCEID_STR { get; } = "{8E061B14-7D0E-FE4E-9594-2112A9C78288}";

        public ResourceItemViewModel BoxMeshResource { get; private set; }
        public ResourceItemViewModel SphereMeshResource { get;private  set; }
        public ResourceItemViewModel GridMeshResource { get;private set; }

        public ResourceItemViewModel SolidColorMaterialResource { get; private set; }



        public Guid ResourceID_NONE { get; set; } = new ResourceID(new byte[16] { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0});
        public readonly ResourceItemViewModel ResourceNone;
        public static ResourceManager Instance { get; }=new Lazy<ResourceManager>(()=>new ResourceManager()).Value;

        private ObservableCollection<ResourceItemViewModel> Resources { get; set; }=new ObservableCollection<ResourceItemViewModel>();

        private Dictionary<ResourceID, ResourceItemViewModel> ResourceMap { get; set; } = new Dictionary<ResourceID, ResourceItemViewModel>();

        public List<ResourceItemViewModel> meshResources=new List<ResourceItemViewModel>();
        public List<ResourceItemViewModel> textrueResources=new List<ResourceItemViewModel>();
        public List<ResourceItemViewModel> materialResources=new List<ResourceItemViewModel>();




        public ResourceManager() 
        {
            //Resources.Add( new ResourceItemViewModel(ResourceType.FOLDER) { Title = "asset" });
            //var r1 = new ResourceItemViewModel( ResourceType.FOLDER) { Title = "文件夹2",Parent = Resources[0] };
            //Resources[0].Children.Add(r1);
            //Resources[0].Children.Add(new ResourceItemViewModel(ResourceType.TEXTURE) { Title = "图片1", Parent = Resources[0] });
            //Resources[0].Children.Add(new ResourceItemViewModel(ResourceType.TEXTURE) { Title = "图片2", Parent = Resources[0] });
            //Resources[0].Children.Add(new ResourceItemViewModel(ResourceType.FOLDER) { Title = "文件夹2" , Parent = Resources[0] });
            //Resources[0].Children.Add(new ResourceItemViewModel(ResourceType.TEXTURE) { Title = "图片3" , Parent = Resources[0] });
            //r1.Children.Add(new ResourceItemViewModel(ResourceType.FOLDER) { Title = "文件4" ,Parent=r1 });
            //r1.Children.Add(new ResourceItemViewModel(ResourceType.TEXTURE) { Title = "图片4" , Parent = r1 });
            //r1.Children.Add(new ResourceItemViewModel(ResourceType.FOLDER) { Title = "文件5" , Parent = r1 });
            //var r2 = r1.Children[0];
            //r2.Children.Add(new ResourceItemViewModel(ResourceType.TEXTURE) { Title = "文件图片4", Parent = r2 });
            //r2.Children.Add(new ResourceItemViewModel(ResourceType.TEXTURE) { Title = "文件图片5", Parent = r2 });
            //r2.Children.Add(new ResourceItemViewModel(ResourceType.FOLDER) { Title = "文件4", Parent = r2 });
            //r2.Children.Add(new ResourceItemViewModel(ResourceType.TEXTURE) { Title = "文件图片7", Parent = r2 });
            ResourceNone = new ResourceItemViewModel(ResourceType.NONE) { resourceID = ResourceID_NONE ,Title="None"};
            LoadAsset();
            LoadInMemeryResoureceAsset();
            ResourceID boxMeshResourceID = ResourceID.Parse(BOX_MESH_RESOURCEID_STR);
            ResourceID sphereResourceID = ResourceID.Parse(SPHERE_MESH_RESOURCEID_STR);
            ResourceID gridResourceID = ResourceID.Parse(GRID_MESH_RESOURCEID_STR);
            BoxMeshResource= this.GetResource(boxMeshResourceID)!;
            SphereMeshResource= this.GetResource(sphereResourceID)!;
            GridMeshResource = this.GetResource(gridResourceID)!;

            ResourceID solidColorMaterialResourceID = ResourceID.Parse(SOLID_COLOR_MATERIAL_RESOURCEID_STR);
            SolidColorMaterialResource = this.GetResource(solidColorMaterialResourceID)!;




        }
        private void LoadAsset()
        {
            var root = EngineDxImport.GetRootResourceInfo();
            string? path = Marshal.PtrToStringAnsi(root.path);
            Resources.Add(new ResourceItemViewModel(ResourceType.FOLDER) { Title = "asset" ,resourceID= root.resourceID,Path=path });
            if (root.childrenCount > 0)
            {
                
                LoadAsset(Resources[0], root.childrenCount);
            }
        }
        private void LoadAsset(ResourceItemViewModel parent, int childCount)
        {
            var childs=EngineDxImport.GetChildResourceInfo(parent.resourceID, childCount);
            foreach (var child in childs)
            {
                string? path=Marshal.PtrToStringAnsi(child.path);
                var title=System.IO.Path.GetFileNameWithoutExtension(path);
                var res = new ResourceItemViewModel(child.resourceType)
                {
                    Title = title,
                    resourceID = child.resourceID,
                    Parent = parent,
                    Path = path
                };
                parent.Children.Add(res);
                if (child.hasChildren)
                {
                    LoadAsset(res, child.childrenCount);
                }
                FilterResource(res);
            }
        }
        private void LoadInMemeryResoureceAsset()
        {
            var inMemeryResources = EngineDxImport.GetInMemeryResourceInfos();
            foreach (var resourceinfo in inMemeryResources)
            {

                string? path = Marshal.PtrToStringAnsi(resourceinfo.path);
                var title = System.IO.Path.GetFileNameWithoutExtension(path);
                var res = new ResourceItemViewModel(resourceinfo.resourceType)
                {
                    Title = title,
                    resourceID = resourceinfo.resourceID,
                    Parent = null,
                    Path = path
                };
                FilterResource(res);
            }
        }

        private void FilterResource(ResourceItemViewModel resource)
        {
            ResourceMap[resource.resourceID] = resource;
            switch (resource.ResourceType)
            {
                case ResourceType.MESH:
                    {
                        meshResources.Add(resource);
                        break;
                    }
                case ResourceType.TEXTURE:
                    {
                        textrueResources.Add(resource);
                        break;
                    }
                case ResourceType.MATERIAL:
                    {
                        materialResources.Add(resource);
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }
        public bool CreateResource
            (ResourceType resourceType,
            ResourceItemViewModel parent, 
            string path)
        {
            using (StringExport pathExport = new StringExport(path))
            {
                ResourceInfo info = new ResourceInfo(); ;
                if (EngineDxImport.CreateResource(ResourceType.MATERIAL, parent.resourceID, pathExport.Ptr, ref info))
                {
                    ResourceItemViewModel resource = new ResourceItemViewModel(info.resourceType)
                    {
                        Parent = parent,
                        Path = path,
                        resourceID = info.resourceID,
                        Title = System.IO.Path.GetFileNameWithoutExtension(path)
                    };
                   
                    parent.Children.Add(resource);
                    FilterResource(resource);
                    return true;
                }
            }
            return false;
        }
        public ObservableCollection<ResourceItemViewModel> GetResourceTree()
        {
            return Resources;
        }
        public ResourceItemViewModel? GetResource(ResourceID resourceID)
        {
            if (ResourceMap.ContainsKey(resourceID))
            {
                return ResourceMap[resourceID];
            }
            return null;
        }
        public IEnumerable<ResourceItemViewModel> GetResources(ResourceType resourceType)
        {
            List<ResourceItemViewModel>? resources = null;
            switch (resourceType)
            {
                case ResourceType.MESH:
                    {
                        resources = meshResources;
                        break;
                    }
                    case ResourceType.TEXTURE: 
                    {
                        resources= textrueResources;
                        break;
                    }
                    case ResourceType.MATERIAL: 
                    {
                        resources= materialResources;
                        break;
                    }
                    default:break;
            }
            if (resources != null)
            {
                foreach (var resource in resources)
                {
                    yield return resource;
                }
            }
            
        }
    }
}
