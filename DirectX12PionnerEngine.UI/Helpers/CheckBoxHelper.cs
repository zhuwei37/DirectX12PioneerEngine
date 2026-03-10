using DirectX12PioneerEngine.UI.Enums;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace DirectX12PioneerEngine.UI.Helpers
{
    public  static class CheckBoxHelper
    {
        public static DependencyProperty BoxHeightProperty = DependencyProperty.RegisterAttached
            ("BoxHeight", typeof(double), typeof(CheckBoxHelper));

        public static double GetBoxHeight(DependencyObject obj)
        {
            return (double)obj.GetValue(BoxHeightProperty);
        }
        public static void SetBoxHeight(DependencyObject obj, double value)
        {
            obj.SetValue(BoxHeightProperty, value);
        }
        public static DependencyProperty BoxWidthProperty = DependencyProperty.RegisterAttached
            ("BoxWidth", typeof(double), typeof(CheckBoxHelper));

        public static double GetBoxWidth(DependencyObject obj)
        {
            return (double)obj.GetValue(BoxWidthProperty);
        }
        public static void SetBoxWidth(DependencyObject obj, double value)
        {
            obj.SetValue(BoxWidthProperty, value);
        }




        public static DependencyProperty ContentPlacementProperty = DependencyProperty.RegisterAttached
            ("ContentPlacement", typeof(ContentPlacement), typeof(CheckBoxHelper));

        public static ContentPlacement GetContentPlacement(DependencyObject obj)
        {
            return (ContentPlacement)obj.GetValue(ContentPlacementProperty);
        }
        public static void SetContentPlacement(DependencyObject obj, ContentPlacement value)
        {
            obj.SetValue(ContentPlacementProperty, value);
        }
    }
}
