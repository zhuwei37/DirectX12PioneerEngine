using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DirectX12PioneerEngine.UI.Converters; 
namespace DirectX12PioneerEngine.UI.Resources
{
    public static class Converters
    {
        public static StringNonullAndNotEmptyToCollapseConverter StringNonullAndNotEmptyToCollapseConverter = new StringNonullAndNotEmptyToCollapseConverter();

        public static CheckBoxPathConverter CheckBoxPathConverter=new CheckBoxPathConverter();
    }
}
