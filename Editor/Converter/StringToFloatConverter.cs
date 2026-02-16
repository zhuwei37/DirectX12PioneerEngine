using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace Editor.Converter
{
    public class StringToFloatConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is float f)
            {
                return f.ToString();
            }
          
            return string.Empty;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {

            if (value != null && value is string valueStr)
            {
                if (float.TryParse(valueStr, out float result))
                {
                    return result;
                }

            }
            return 0.0f;
          
        }
    }
}
