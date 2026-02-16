using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using Vortice.Direct3D12;
namespace Editor.ViewModel.Info
{
    public class FolderLevelViewMdeol:ObservableObject
    {
        public FolderLevelViewMdeol() { }
        public string FolderName 
        {
            get=>_folderName;
            set 
            {
               SetProperty(ref _folderName, value);
            }
        }
        private string _folderName;
        public bool IsLast
        {
            get { return _isLast; }
            set { SetProperty(ref _isLast, value); }
        }
        private bool _isLast=false;
        public ResourceItemViewModel ResourceItem { get; set; }
        
    }
}
