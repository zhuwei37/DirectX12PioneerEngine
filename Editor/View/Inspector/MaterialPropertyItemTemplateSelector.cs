using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using static Editor.ViewModel.Inspector.Component.MaterialPropertyListViewModel;

namespace Editor.View.Inspector
{
    internal class MaterialPropertyItemTemplateSelector:System.Windows.Controls.DataTemplateSelector
    {
        public DataTemplate? MaterialFloatPorpertyTemplate { get; set; }
        public DataTemplate? MaterialFloat3PropertyTemplate { get; set; }
        public DataTemplate? MaterialFloat4PropertyTemplate { get; set; }
        public DataTemplate? MaterialTexturePropertyTemplate{get;set;}
        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            //this.GetType().GetCustomAttribute
            if (item is MaterialPropertyItemViewModel property)
            {
                switch (property.PropertyType)
                {
                    case Interface.Component.MaterialPropertyType.Float:
                        {
                            if (MaterialFloatPorpertyTemplate != null)
                            {
                                return MaterialFloatPorpertyTemplate;
                            }
                            break;
                        }
                    case Interface.Component.MaterialPropertyType.Float3:
                        {
                            if (MaterialFloat3PropertyTemplate != null)
                            {
                                return MaterialFloat3PropertyTemplate;
                            }
                            break;
                        }
                    case Interface.Component.MaterialPropertyType.Float4:
                        {
                            if (MaterialFloat4PropertyTemplate != null)
                            {
                                return MaterialFloat4PropertyTemplate;
                            }
                            break;
                        }
                    case Interface.Component.MaterialPropertyType._Texture:
                        {
                            if (MaterialTexturePropertyTemplate != null) 
                            { return MaterialTexturePropertyTemplate; }
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
            }
            return base.SelectTemplate(item, container);
        }
    }
}
