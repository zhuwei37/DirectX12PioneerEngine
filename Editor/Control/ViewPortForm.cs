using Editor.RenderDx;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms.Integration;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ViewPortLib;
using Vortice.Direct3D9;

namespace Editor.Control
{
  
    public class ViewPortForm : System.Windows.Controls.Control
    {
        static ViewPortForm()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(ViewPortForm), new FrameworkPropertyMetadata(typeof(ViewPortForm)));
        }
        public static readonly DependencyProperty ContentProperty
            = DependencyProperty.Register("Content", typeof(object), typeof(ViewPortForm), new PropertyMetadata(null));

        public static readonly DependencyProperty RenderHostIdProerpty =
            DependencyProperty.Register("RenderHostId", typeof(int), typeof(ViewPortForm), new FrameworkPropertyMetadata(null)  );

        public static readonly DependencyProperty RenderHostTypeProerpty =
            DependencyProperty.Register("RenderHostType", typeof(RenderHostType), typeof(ViewPortForm), new PropertyMetadata(RenderHostType.MainRendering));
        public RenderHostType RenderHostType 
        {
            get { return (RenderHostType)GetValue(RenderHostTypeProerpty); }
            set { SetValue(RenderHostTypeProerpty, value); }
        }
        public int RenderHostId
        {
            get { return (int)GetValue(RenderHostIdProerpty); }
            set { SetValue(RenderHostIdProerpty, value); }
        }


        public object Content
        {
            get { return (object)GetValue(ContentProperty); }
            set { SetValue(ContentProperty, value); }
        }
        private RenderHost _renderHost;
      

        public ViewPortForm() 
        {
            this.Loaded += ViewPortForm_Loaded;
            this.Unloaded += ViewPortForm_Unloaded;
        }
        
        private void ViewPortForm_Unloaded(object sender, RoutedEventArgs e)
        {
          _renderHost.Visible= false;
          
        }

        private void ViewPortForm_Loaded(object sender, RoutedEventArgs e)
        {
            _renderHost.Visible = true;
        }

       



     

     
      
        private IntPtr hWnd;

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();
            //content = (ContentPresenter)this.GetTemplateChild("PART_Content");
            var viewPort = new ViewPortLib.ViewPort();
            hWnd = viewPort.Handle;
            var formHost = new System.Windows.Forms.Integration.WindowsFormsHost();
            formHost.Child = viewPort;
            if (hWnd == IntPtr.Zero)
            {
                base.OnApplyTemplate();
                return;  
            }
            viewPort.SizeChanged += ViewPort_SizeChanged;
            viewPort.MouseDown += ViewPort_MouseDown;
            viewPort.MouseMove += ViewPort_MouseMove;
            viewPort.MouseUp += ViewPort_MouseUp;
            this.KeyDown += ViewPortForm_KeyDown;
            Content = formHost;
            //从DataContext里面直接获取渲染主机配置 IRenderHostConfig 
           // var vm=this.DataContext;
            _renderHost = RenderHost.CreateRenderHost(hWnd, 800, 600);
            
            this.RenderHostId =(int) _renderHost.RenderHostId;
            //System.Diagnostics.Debug.WriteLine("ssssssssssssssssssssssssssssssssssssssssss");
          
        }

        private void ViewPort_MouseUp(object? sender, System.Windows.Forms.MouseEventArgs e)
        {
            
        }

        private void ViewPort_MouseMove(object? sender, System.Windows.Forms.MouseEventArgs e)
        {
            _renderHost.RenderHostCameraController.MouseMove(e.X, e.Y, e.Button == MouseButtons.Left);
        }

        private void ViewPortForm_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            _renderHost.RenderHostCameraController.KeyDonw();
        }

        private void ViewPort_MouseDown(object? sender, System.Windows.Forms.MouseEventArgs e)
        {
            _renderHost.RenderHostCameraController.MouseDown(e.X, e.Y);
        }

    

        private void ViewPort_SizeChanged(object? sender, EventArgs e)
        {
            var c = (WindowsFormsHost)(Content);
             var width= ((ViewPort)sender!).Width;
            var height = ((ViewPort)sender!).Height;
            _renderHost?.Resize(width, height);


        }
        ~ViewPortForm()
        {
            EngineDxImport.Destory_RenderHost((uint)this.RenderHostId);
        }
    }
}
