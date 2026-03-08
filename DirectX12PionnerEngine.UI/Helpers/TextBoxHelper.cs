using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace DirectX12PioneerEngine.UI.Helpers
{
    public static class TextBoxHelper
    {
        public static DependencyProperty WatermarkProperty = DependencyProperty.Register
            ("Watermark", typeof(object), typeof(TextBoxHelper));

        public static object GetWatermark(DependencyObject obj)
        {
            return (object)obj.GetValue(WatermarkProperty);
        }
        public static void SetWatermark(DependencyObject obj, object value)
        {
            obj.SetValue(WatermarkProperty, value);
        }
    }
}
