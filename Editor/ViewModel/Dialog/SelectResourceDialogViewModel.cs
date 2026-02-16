using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using Editor.RenderDx;
using Editor.Server.Resource;
using Editor.ViewModel.Info;
namespace Editor.ViewModel.Dialog
{
    public class SelectResourceDialogViewModel:ObservableObject
    {
        public ObservableCollection<ResourceItemViewModel> ResourceList { get; set; }=new ObservableCollection<ResourceItemViewModel>();
        private ResourceItemViewModel? selectedResource;
        public ResourceItemViewModel? SelectedResource
        {
            get { return selectedResource; }
            set 
            {
                SetProperty(ref selectedResource, value);
            }
        }
        public SelectResourceDialogViewModel(ResourceType resourceType) 
        {
            ResourceList.Add(ResourceManager.Instance.ResourceNone);
            selectedResource = ResourceManager.Instance.ResourceNone;
            foreach (var resource in ResourceManager.Instance.GetResources(resourceType))
            {
                ResourceList.Add(resource);
            }
        }
    }
}
