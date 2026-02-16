using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace Editor.View
{
    public class ViewTemplateSelector : DataTemplateSelector
    {
        
        public DataTemplate failTemplate { get; set; }
        public ViewTemplateSelector() 
        {
            string failTemplateName = "FailTemplate";
            var resource = System.Windows.Application.Current.TryFindResource(failTemplateName);
            if (resource != null)
            {
                if (resource is DataTemplate failDataTemplate)
                {
                    failTemplate = failDataTemplate;
                }
            }
        }
        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            var dataTemplateName = string.Empty;
            if (item is ContentPresenter presenter)
            {
               
                var content = presenter.Content;
                dataTemplateName = content.GetType().Name+ "Template";
              
            }
            else
            {
                dataTemplateName = item.GetType().Name + "Template";
            }
            var resource=System.Windows.Application.Current.TryFindResource(dataTemplateName);
            if (resource != null)
            {
                if (resource is DataTemplate datatemplate)
                {
                    
                    return datatemplate;
                }
            }
            return failTemplate;
        }
    }
}
