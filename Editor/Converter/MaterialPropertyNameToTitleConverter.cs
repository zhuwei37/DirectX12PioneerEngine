using Editor.EditorAttribute;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Editor.Converter
{
    public class MaterialPropertyNameToTitleConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value != null)
            {
                var attribute= value.GetType().GetCustomAttribute<MaterialPropertyNameAttribute>();
                if (attribute != null)
                {
                    return attribute.MaterialPropertyName;
                }
            }
            return "";
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
