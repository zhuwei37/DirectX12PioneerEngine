using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace DirectX12PioneerEngine.UI.Helpers
{
    public static class WatermarkHelper
    {

        public static DependencyProperty ForegroundProperty = DependencyProperty.RegisterAttached
            ("Foreground", typeof(Brush), typeof(WatermarkHelper));
        public static Brush GetForeground(DependencyObject obj)
        {
            return (Brush)obj.GetValue(ForegroundProperty);
        }
        public static void SetForeground(DependencyObject obj, Brush value)
        {
            obj.SetValue(ForegroundProperty, value);
        }


        public static DependencyProperty FontSizeProperty = DependencyProperty.RegisterAttached
            ("FontSize", typeof(double), typeof(WatermarkHelper));

        public static double GetFontSize(DependencyObject obj)
        {
            return (double)obj.GetValue(FontSizeProperty);
        }
        public static void SetFontSize(DependencyObject obj, double value)
        {
            obj.SetValue(FontSizeProperty, value);
        }

        public static DependencyProperty FontFamilyProperty = DependencyProperty.RegisterAttached
            ("FontFamily", typeof(FontFamily), typeof(WatermarkHelper));

        public static FontFamily GetFontFamily(DependencyObject obj)
        {
            return (FontFamily)obj.GetValue(FontFamilyProperty);
        }

        public static void SetFontFamily(DependencyObject obj, FontFamily value)
        {
            obj.SetValue(FontFamilyProperty, value);
        }


        public static DependencyProperty MarginProperty = DependencyProperty.RegisterAttached
            ("Margin", typeof(Thickness), typeof(WatermarkHelper));

        public static Thickness GetMargin(DependencyObject obj) 
        {
            return (Thickness)obj.GetValue(MarginProperty);
        }
        public static void SetMargin(DependencyObject obj, Thickness value)
        {
             obj.SetValue(MarginProperty, value);
        }
    }
}
