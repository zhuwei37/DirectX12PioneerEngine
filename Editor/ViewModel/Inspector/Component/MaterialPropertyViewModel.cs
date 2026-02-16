using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommunityToolkit.Mvvm.ComponentModel;
using ResourceID = System.Guid;
namespace Editor.ViewModel.Inspector.Component
{
    public class MaterialPropertyViewModel:ObservableObject
    {
        public  ResourceID MaterialResourceID { get; set; }
        public long MaterialPorpertyID { get; set; }
    }
}
