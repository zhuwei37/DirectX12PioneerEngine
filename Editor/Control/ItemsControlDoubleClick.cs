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
    public static class ContentControlDoubleClick
    {
        public static DependencyProperty IsEnableProperty=DependencyProperty.RegisterAttached
            ("IsEnable",
            typeof(bool),
            typeof(ContentControlDoubleClick), new PropertyMetadata(false, IsEnableChanged)
            );
        public static DependencyProperty DoubleClickCommandProeprty = DependencyProperty.RegisterAttached
            (
            "DoubleClickCommand",
            typeof(ICommand),
            typeof(ContentControlDoubleClick),
            new PropertyMetadata(null)
            );
        public static DependencyProperty DoubleClickCommandParameterProperty = DependencyProperty.RegisterAttached
            (
            "DoubleClickCommandParameter",
            typeof(object),
            typeof(ContentControlDoubleClick),
            new PropertyMetadata(null)
            );
        public static bool GetIsEnable(DependencyObject obj)
        {
            return (bool)obj.GetValue(IsEnableProperty);
        }
        public static void SetIsEnable(DependencyObject obj, bool value)
        {
            obj.SetValue(IsEnableProperty, value);
        }
        public static object GetDoubleClickCommandParameter(DependencyObject obj)
        {
            return (object)obj.GetValue(DoubleClickCommandParameterProperty);
        }
        public static void SetDoubleClickCommandParameter(DependencyObject obj, object parameter)
        {
            obj.SetValue (DoubleClickCommandParameterProperty, parameter);
        }
        public static ICommand GetDoubleClickCommand(DependencyObject obj)
        {
            return (ICommand)obj.GetValue(DoubleClickCommandProeprty);
        }
        public static void SetDoubleClickCommand(DependencyObject obj, ICommand command)
        {
            obj.SetValue(DoubleClickCommandProeprty, command);
        }
        private static void IsEnableChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
          
           var control= d as ContentControl;
            if (control != null)
            {
                if ((bool)e.NewValue)
                {
                    control.MouseDoubleClick += Control_MouseDoubleClick;
                }
                else
                {
                    control.MouseDoubleClick -= Control_MouseDoubleClick;
                }
            }
        }

        private static void Control_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            var  command = GetDoubleClickCommand((DependencyObject)sender);
            var parameter = GetDoubleClickCommandParameter((DependencyObject)sender);
            command?.Execute(parameter);
            //System.Diagnostics.Debug.WriteLine("双击");
        }
    }

}
