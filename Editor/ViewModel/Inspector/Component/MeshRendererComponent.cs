using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Editor.Interface.Component;
using Editor.RenderDx;
using Editor.Server.Resource;
using Editor.View.Dialog;
using Editor.ViewModel.Info;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
namespace Editor.ViewModel.Inspector.Component
{
    public class MeshRendererComponent : ObservableObject, IComponent
    {


        IMaterialListView materialListView;
        public class MeshRendererMaterialViewModel()
        {
            
        }
        
        public string ComponentName => "MeshRenderer";

        public bool CanClose => true;

        private IntPtr _handle;
        private uint actorId;
        
        public ICommand SelectMaterialCommand { get; private set; }
        public ICommand AddMaterialCommand { get; private set; }
        public ICommand RemoveMaterialCommand { get; private set; }

       public ObservableCollection<ResourceItemViewModel> Materials { get; set; }=new ObservableCollection<ResourceItemViewModel>();
        private List<MaterialViewModel?> materialViewModels = new List<MaterialViewModel?>();
        public MeshRendererComponent(uint actorId, IntPtr handle)
        {
            this.actorId = actorId;
            this._handle = handle;

            SelectMaterialCommand = new RelayCommand<int>(SelectMaterial);
            AddMaterialCommand=new RelayCommand(AddMaterial);
            RemoveMaterialCommand=new RelayCommand(RemoveMaterial);


            var matIds=EngineDxImport.GetMeshRendererMateriels(this._handle);
            foreach (var matid in matIds) 
            {
                var resource=ResourceManager.Instance.GetResource(matid);
                if (resource != null)
                {
                    this.Materials.Add(resource);
                    if (resource != ResourceManager.Instance.ResourceNone)
                    {
                        materialViewModels.Add(new MaterialViewModel(matid));
                    }
                    else
                    {
                        materialViewModels.Add(null);
                    }
                }
            }

        }
        private void SelectMaterial(int index)
        {

            SelectResourceDialog selectResourceDialog = new SelectResourceDialog(ResourceType.MATERIAL);
            var result = selectResourceDialog.ShowDialog();
            if (result != null && result == true)
            {

                var resource = selectResourceDialog.GetSelectedResource();
                SetMaterial(index, resource);

                //if (resource != null)
                //{
                //    var flag = EngineDxImport.SetMeshRenderer_MaterialResourceID(this._handle, index, resource.resourceID);
                //    if (flag)
                //    {
                //        Materials[index] = resource;
                //        var matviewmodel = this.materialViewModels[index];

                //        if (resource != ResourceManager.Instance.ResourceNone)
                //        {
                //            var newMat = new MaterialViewModel(resource.resourceID);
                //            this.materialViewModels[index] = newMat;
                //            this.materialListView?.AddMaterialView(newMat);
                //        }
                //        if (matviewmodel != null)
                //        {
                //            this.materialListView?.RemoveMaterialView(matviewmodel);

                //        }
                //    }
                //}
            }
        }

        public void SetMaterial(int index, ResourceItemViewModel? resource)
        {
            if (resource != null)
            {
                var flag = EngineDxImport.SetMeshRenderer_MaterialResourceID(this._handle, index, resource.resourceID);
                if (flag)
                {
                    Materials[index] = resource;
                    var matviewmodel = this.materialViewModels[index];

                    if (resource != ResourceManager.Instance.ResourceNone)
                    {
                        var newMat = new MaterialViewModel(resource.resourceID);
                        this.materialViewModels[index] = newMat;
                        this.materialListView?.AddMaterialView(newMat);
                    }
                    if (matviewmodel != null)
                    {
                        this.materialListView?.RemoveMaterialView(matviewmodel);

                    }
                }
            }
        }
        public void SetMaterialListView(IMaterialListView materialListView)
        {
            this.materialListView = materialListView;
            foreach (var matview in this.materialViewModels)
            {
                if (matview != null)
                {
                    this.materialListView.AddMaterialView(matview);
                }
            }
           
        }
        public void AddMaterial()
        {
            EngineDxImport.MeshRenderer_AddMaterialSlot(this._handle);
            this.Materials.Add(ResourceManager.Instance.ResourceNone);
            this.materialViewModels.Add(null);
        }
        public void RemoveMaterial() 
        {
            EngineDxImport.MeshRenderer_RemoveMaterialSlot(this._handle);
            this.Materials.RemoveAt(this.Materials.Count - 1);
            var item=this.materialViewModels[this.materialViewModels.Count - 1];
            materialViewModels.RemoveAt(this.materialViewModels.Count - 1);
            if (item != null)
            {
                this.materialListView.RemoveMaterialView(item);
            }
        }

        public void Destory()
        {
            foreach (var item in this.materialViewModels)
            {
                if (item != null)
                {
                    this.materialListView.RemoveMaterialView(item);
                }
            }
        }


    }
}
