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
    public class PreviewSceneTreeView:System.Windows.Controls.TreeView
    {
        public static DependencyProperty SelectedCommandProperty = DependencyProperty.Register
            (
             "SelectedCommand",
             typeof(ICommand),
             typeof(PreviewSceneTreeView),
             new PropertyMetadata(null)
            );
        public static new DependencyProperty SelectedItemProperty = DependencyProperty.Register
            (
            "SelectedItem",
            typeof(object),
            typeof(PreviewSceneTreeView),
            new UIPropertyMetadata(null, OnSelectedItemChanged)
            );
        public new object SelectedItem
        {
            get { return (object)GetValue(SelectedItemProperty); }
            set { SetValue(SelectedItemProperty, value); }
        }
        public ICommand SelectedCommand
        {
            get { return (ICommand)GetValue(SelectedCommandProperty); }
            set { SetValue(SelectedCommandProperty, value); }
        }
        static PreviewSceneTreeView()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(PreviewSceneTreeView), new FrameworkPropertyMetadata(typeof(PreviewSceneTreeView)));
        }
        PreviewSceneTreeView()
        {
            this.SelectedItemChanged += PreviewSceneTreeView_SelectedItemChanged;
           
        }
        private static void OnSelectedItemChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (e.NewValue is PreviewSceneTreeViewItem item)
            {
                item.SetValue(PreviewSceneTreeViewItem.IsSelectedProperty, true);
            }
        }
        private void PreviewSceneTreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            SetCurrentValue(SelectedItemProperty, e.NewValue);
            SelectedCommand?.Execute(e.NewValue);
        }

        protected override DependencyObject GetContainerForItemOverride()
        {
            return new PreviewSceneTreeViewItem();
        }
    }
}
