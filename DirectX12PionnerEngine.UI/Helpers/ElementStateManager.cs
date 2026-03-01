using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace DirectX12PionnerEngine.UI.Helpers
{
    public static class ElementStateManager
    {
        #region Property

        #region Background
        public static DependencyProperty BackgroundProperty = DependencyProperty.RegisterAttached
            ("Background", typeof(Brush), typeof(ElementStateManager), new PropertyMetadata(null));

        public static Brush GetBackground(DependencyObject obj)
        {
            return (Brush)obj.GetValue(BackgroundProperty);
        }
        public static void SetBackground(DependencyObject obj, Brush brush)
        {
            obj.SetValue(BackgroundProperty, brush);
        }
        #endregion

        #region Foreground
        public static DependencyProperty ForegroundProperty = DependencyProperty.RegisterAttached
            ("Foreground", typeof(Brush), typeof(ElementStateManager), new PropertyMetadata());

        public static Brush GetForeground(DependencyObject obj) 
        {
            return (Brush)obj.GetValue(ForegroundProperty);
        }
        public static void SetForeground(DependencyObject obj, Brush brush)
        {
            obj.SetValue(ForegroundProperty, brush);
        }
        #endregion

        #region BorderCornerRadius
        public static DependencyProperty BorderCornerRadiusProperty = DependencyProperty.RegisterAttached
            ("BorderCornerRadius", typeof(CornerRadius), typeof(ElementStateManager), new PropertyMetadata());

        public static CornerRadius GetCornerRadius(DependencyObject obj)
        {
            return (CornerRadius)obj.GetValue(BorderCornerRadiusProperty);
        }
        public static void SetCornerRadius(DependencyObject obj, CornerRadius value)
        {
            obj.SetValue(BorderCornerRadiusProperty, value);
        }

        #endregion

        #region Border

        #endregion




        #endregion
    }
}
