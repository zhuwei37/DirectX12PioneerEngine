using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace DirectX12PioneerEngine.UI.Helpers
{
    public static class ComboBoxItemHelper
    {
        public static DependencyProperty SelectedForegroundProperty = DependencyProperty.RegisterAttached
            ("SelectedForeground", typeof(Brush), typeof(ComboBoxItemHelper));

        public static Brush GetSelectedForeground(DependencyObject obj)
        {
            return (Brush)obj.GetValue(SelectedForegroundProperty);
        }
        public static void SetSelectedForeground(DependencyObject obj,Brush value)
        { 
            obj.SetValue(SelectedForegroundProperty, value);
        }

        public static DependencyProperty SelectedBackgroundProperty = DependencyProperty.RegisterAttached
            ("SelectedBackground", typeof(Brush), typeof(ComboBoxItemHelper));
        public static Brush GetSelectedBackground(DependencyObject obj) 
        {
            return (Brush)obj.GetValue (SelectedBackgroundProperty);
        }
        public static void SetSelectedBackground(DependencyObject obj, Brush value)
        {
            obj.SetValue(SelectedBackgroundProperty, value);
        }
    }
}
