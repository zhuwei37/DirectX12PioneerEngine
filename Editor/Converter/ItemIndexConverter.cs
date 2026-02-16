using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace Editor.Converter
{
    public class ItemIndexConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is DependencyObject obj)
            {
                var container= ItemsControl.ItemsControlFromItemContainer(obj);
                var index=container.ItemContainerGenerator.IndexFromContainer(obj);
                return index;
            }
            return System.Windows.Data.Binding.DoNothing;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
