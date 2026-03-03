using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Animation;

namespace Editor.View
{
    public static class ThemeProps
    {
        public static DependencyProperty BackgroundProperty = DependencyProperty.RegisterAttached
            ("Background", typeof(System.Windows.Media.Brush), typeof(ThemeProps), new PropertyMetadata(BackgroundChanged));


        public static System.Windows.Media.Brush GetBackground(DependencyObject obj)
        {
            return (System.Windows.Media.Brush)obj.GetValue(BackgroundProperty);
        }
        public static void SetBackground(DependencyObject obj, System.Windows.Media.Brush brush)
        {
            obj.SetValue(BackgroundProperty, brush);
        }
        private static void BackgroundChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is FrameworkElement element && e.NewValue is SolidColorBrush newBrush)
            {
                AnimateBrushProperty(element, newBrush, "Background");
            }
        }
        private static void AnimateBrushProperty(FrameworkElement element, SolidColorBrush newBrush, string propertyName)
        {
            var property=element.GetType().GetProperty(propertyName);
            if (property == null)
            {
                return;
            }
            var currentBrush=property.GetValue(element) as SolidColorBrush;
            if (currentBrush == null || currentBrush.IsFrozen)
            {
                currentBrush = new SolidColorBrush(newBrush.Color);
                property.SetValue(element, currentBrush);
            }
            currentBrush.BeginAnimation(SolidColorBrush.ColorProperty, new ColorAnimation()
            {
                To = newBrush.Color,
                Duration = TimeSpan.FromSeconds(0.3)
            });
        }

    }
}
