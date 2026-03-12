using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace DirectX12PioneerEngine.UI.Helpers
{
   public static class PopupHelper
    {
        public static DependencyProperty BackgroundProperty = DependencyProperty.RegisterAttached
            ("Background", typeof(Brush), typeof(PopupHelper));

        public static Brush GetBackground(DependencyObject obj)
        {
            return (Brush)obj.GetValue(BackgroundProperty);
        }
        public static void SetBackground(DependencyObject obj, Brush value)
        {
            obj.SetValue(BackgroundProperty, value);
        }


        public static DependencyProperty BorderBrushProperty = DependencyProperty.RegisterAttached
            ("BorderBrush", typeof(Brush), typeof(PopupHelper));

        public static Brush GetBorderBrush(DependencyObject obj)
        {
            return (Brush)obj.GetValue (BorderBrushProperty);
        }
        public static void SetBorderBrush(DependencyObject obj, Brush value)
        {
            obj.SetValue(BorderBrushProperty, value);
        }


    }
}
