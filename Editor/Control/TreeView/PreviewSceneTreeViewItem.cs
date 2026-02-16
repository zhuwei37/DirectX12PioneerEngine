using Editor.Server.Scene;
using Editor.ViewModel.Info;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Editor.Control.TreeView
{
    public class PreviewSceneTreeViewItem:System.Windows.Controls.TreeViewItem
    {
        static PreviewSceneTreeViewItem()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(PreviewSceneTreeViewItem), new System.Windows.FrameworkPropertyMetadata(typeof(PreviewSceneTreeViewItem)));
        }

        public static DependencyProperty RenameCommandProperty = DependencyProperty.
            Register("RenameCommand",
            typeof(ICommand),
            typeof(PreviewSceneTreeViewItem),
            new PropertyMetadata(null));
        public ICommand RenameCommand
        {
            get => (ICommand)GetValue(RenameCommandProperty);
            set => SetValue(RenameCommandProperty, value);
        }


        public PreviewSceneTreeViewItem()
        {
            
        }
        protected override DependencyObject GetContainerForItemOverride()
        {
            return new PreviewSceneTreeViewItem();
        }
        protected override void OnContextMenuOpening(ContextMenuEventArgs e)
        {

            if (this.DataContext is SceneNode)
            {
                this.ContextMenu = (ContextMenu)System.Windows.Application.Current.FindResource("SceneNodeMenu");
            }
            else if (this.DataContext is ResourceItemViewModel)
            {
                this.ContextMenu = (ContextMenu)System.Windows.Application.Current.FindResource("ResourceMenu");
            }

        }
      
        protected override void OnSelected(RoutedEventArgs e)
        {
            e.Handled = true;
        }
        ~PreviewSceneTreeViewItem()
        {
            
        }
    }
}
