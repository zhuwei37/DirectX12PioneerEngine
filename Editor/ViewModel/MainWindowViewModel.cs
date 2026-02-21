using CommunityToolkit.Mvvm.Input;
using Editor.RenderDx;
using Editor.RenderDx.Device;
using Editor.Server;
using Editor.Server.Scene;
using Editor.Time;
using Editor.ViewModel.Info;
using Editor.ViewModel.Inspector;
using Editor.ViewModel.Preview;
using Editor.ViewModel.Workspace;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection.Metadata;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.Integration;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Media3D;
using Vortice.Direct3D12.Video;
using Vortice.Direct3D9;

namespace Editor.ViewModel
{
    public class MainWindowViewModel:CommunityToolkit.Mvvm.ComponentModel.ObservableObject
    {
       
       public ICommand SaveCommand { get;private set; }
        public ICommand OpenReadySceneCommand { get; private set; }
        public ICommand AddSceneNodeCommand { get; private set; }
        public MainWindowViewModel()
        {
            OpenReadySceneCommand = new RelayCommand(OpenReadyScene);
            AddSceneNodeCommand= new RelayCommand(AddSceneNode);
            SaveCommand=new RelayCommand(Save);
            Init();
        }
       async void Init()
        {
            await Task.Delay(100);

            DockManager.Instance.AddTool(new SceneNodePreviewViewModel());


            DockManager.Instance.AddTool(new ResourcePanelViewModel());

            DockManager.Instance.AddTool(new InspectorMainViewModel());
            DockManager.Instance.AddPage(new ViewportDxViewModel());
        }
        private void Save()
        {
            EngineDxImport.Save();
        }
        private void OpenReadyScene()
        {
            // SceneNode sceneNode = new SceneNode();
            // var a1=new a();
            var doc = (DockManager)DockManager.Instance;
            var s=(SceneNodePreviewViewModel)doc.vm.Tools[0];
            //s.SceneNodes.Clear();
            GC.Collect();
           // EngineDxImport.SetAll();
        }
        private void AddSceneNode()
        {
            var doc = (DockManager)DockManager.Instance;
            var s = (SceneNodePreviewViewModel)doc.vm.Tools[0];
            s.CreateSceneNodeCommand.Execute(null);
        }
       




    }
}
