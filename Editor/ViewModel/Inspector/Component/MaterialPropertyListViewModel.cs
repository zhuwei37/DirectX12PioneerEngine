using CommunityToolkit.Mvvm.ComponentModel;
using Editor.EditorAttribute;
using Editor.Interface.Component;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using static Editor.ViewModel.Inspector.Component.MaterialPropertyListViewModel;
namespace Editor.ViewModel.Inspector.Component
{
    public abstract class MaterialPropertyListViewModel:ObservableObject,IEnumerable<MaterialPropertyItemViewModel>
    {
        public class MaterialPropertyItemViewModel
        {
            public PropertyInfo? propertyInfo;
            public object? sourceObject;
            public string? Title { get; set; }
            public MaterialPropertyType PropertyType { get; set; }
            public object? PropertyValue
            {
                get
                {
                    if (sourceObject != null && propertyInfo != null)
                    {
                        return propertyInfo.GetValue(sourceObject);
                    }
                    return null;
                }
                set 
                {
                    if (sourceObject != null && propertyInfo != null)
                    {
                         propertyInfo.SetValue(sourceObject,value);
                    }
                }
            }
        }
        public MaterialPropertyListViewModel()
        {
            CollectMaterialProperty();
        }
        public ObservableCollection<MaterialPropertyItemViewModel> MaterialProperties { get; set; }=new ObservableCollection<MaterialPropertyItemViewModel>();

        protected void CollectMaterialProperty()
        {
            MaterialProperties.Clear();
            var type=this.GetType();
            foreach (var property in type.GetProperties())
            {
                var mp=property.GetCustomAttribute<MaterialPropertyAttribute>();
                var mpn=property.GetCustomAttribute<MaterialPropertyNameAttribute>();
                if (mp != null && mpn != null)
                {
                    MaterialPropertyItemViewModel itemvm = new MaterialPropertyItemViewModel()
                    {
                        propertyInfo = property,
                        sourceObject = this,
                        PropertyType = mp.MatPropertyType,
                        Title = mpn.MaterialPropertyName
                    };
                    MaterialProperties.Add(itemvm);
                }
            }
        }

        public IEnumerator<MaterialPropertyItemViewModel> GetEnumerator()
        {
            return MaterialProperties.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
