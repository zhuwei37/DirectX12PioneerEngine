using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace DirectX12PionnerEngine.UI.Helpers
{
    public static class ButtonHelper
    {
        public static DependencyProperty ClickBackgroundProperty = DependencyProperty.RegisterAttached
            ("ClickBackground", typeof(Brush), typeof(ButtonHelper), new PropertyMetadata());

        public static Brush GetClickBackground(DependencyObject obj)
        {
            return (Brush)obj.GetValue(ClickBackgroundProperty);
        }
        public static void SetClickBackground(DependencyObject obj, Brush value) 
        {
            obj.SetValue(ClickBackgroundProperty, value);
        }

        public static DependencyProperty HoverBackgroundProperty = DependencyProperty.RegisterAttached
            ("HoverBackground", typeof(Brush), typeof(ButtonHelper), new PropertyMetadata());

        public static Brush GetHoverBackground(DependencyObject obj)
        {
            return (Brush)obj.GetValue (HoverBackgroundProperty);
        }
        public static void SetHoverBackground(DependencyObject obj, Brush brush)
        {
            obj.SetValue(HoverBackgroundProperty, brush);
        }



    }
}
