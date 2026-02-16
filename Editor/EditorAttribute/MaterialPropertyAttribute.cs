using Editor.Interface.Component;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.EditorAttribute
{
    [AttributeUsageAttribute(AttributeTargets.Property)]
    public class MaterialPropertyAttribute: Attribute
    {
        public MaterialPropertyType MatPropertyType { get; set; }
        public MaterialPropertyAttribute(MaterialPropertyType materialPropertyType) 
        {
            MatPropertyType = materialPropertyType;
        }
    }
    public class MaterialPropertyNameAttribute : Attribute
    {
        public string MaterialPropertyName { get; set; }
        public MaterialPropertyNameAttribute(string materialPropertyName) 
        {
            MaterialPropertyName= materialPropertyName;
        }


    }
}
