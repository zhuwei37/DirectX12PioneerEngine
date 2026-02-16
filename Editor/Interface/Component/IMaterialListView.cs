using Editor.ViewModel.Inspector;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Interface.Component
{
    public interface IMaterialListView
    {
        void AddMaterialView(MaterialViewModel vm);
        void RemoveMaterialView(MaterialViewModel vm);  
    }
}
