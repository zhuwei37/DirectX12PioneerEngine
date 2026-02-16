using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Editor.Control
{
    public static class ItemClick
    {
        public static DependencyProperty ClickCommandProperty =
            DependencyProperty.RegisterAttached(
                "ClickCommand",
                typeof(ICommand),
                typeof(ItemClick),
                new FrameworkPropertyMetadata(ClickCommandChanged)
                );
        public static ICommand GetClickCommand(DependencyObject obj)
        {
            return (ICommand)obj.GetValue(ClickCommandProperty);
        }
        public static void SetClickCommand(DependencyObject obj, ICommand value)
        {
            obj.SetValue(ClickCommandProperty, value);
        }

        public static DependencyProperty ClickCommandParameterProperty = DependencyProperty.RegisterAttached
            (
            "ClickCommandParameter",
            typeof(object),
            typeof(ItemClick),
            new PropertyMetadata()
            );

        public static object GetClickCommandParameter(DependencyObject obj)
        {
            return obj.GetValue(ClickCommandParameterProperty);
        }
        public static void SetClickCommandParameter(DependencyObject obj, object value)
        {
            obj.SetValue(ClickCommandParameterProperty, value);
        }


        private static void ClickCommandChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
            var control = sender as System.Windows.FrameworkElement; ;
            if (control != null)
            {
                if (e.NewValue != null)
                {
                    control.MouseDown-= Control_MouseDown;
                    control.MouseDown += Control_MouseDown;
                }
                else
                {
                    control.MouseDown -= Control_MouseDown;
                }
            }
        }

        private static void Control_MouseDown(object sender, MouseButtonEventArgs e)
        {
            var command = GetClickCommand((DependencyObject)sender);
            var parameter = GetClickCommandParameter((DependencyObject)(sender));
            command?.Execute(parameter);
        }
    }
}
