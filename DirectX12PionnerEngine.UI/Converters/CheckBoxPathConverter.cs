using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace DirectX12PioneerEngine.UI.Converters
{

    public class CheckBoxPathConverter : System.Windows.Data.IMultiValueConverter
    {
        public static string Format(double value)
        {
            return value.ToString(CultureInfo.InvariantCulture);
        }
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            var actualWidth = (double)values[0];
            var actualHeight = (double)values[1];
            if (actualWidth == 0 || actualHeight == 0)
            {
                return null;
            }
            var path = $"M {Format(actualWidth / 6)},{Format(actualHeight * 7 / 12 - 1)} L{Format(actualWidth / 2 - 1)},{Format(actualHeight * 5 / 6 - 1)} L{Format(actualWidth * 5 / 6)},{Format(actualHeight * 3 / 12 - 1)}";

           path = "F1 M 9.97498,1.22334L 4.6983,9.09834L 4.52164,9.09834L 0,5.19331L 1.27664,3.52165L 4.255,6.08833L 8.33331,1.52588e-005L 9.97498,1.22334 Z";
            return Geometry.Parse(path);
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
