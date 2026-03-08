using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.Xml;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;

namespace DirectX12PioneerEngine.UI.TemplateSelectors
{
    class WatermarkTemplateSelector:DataTemplateSelector
    {
        public WatermarkTemplateSelector() { }
        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            if (item != null)
            {
                if (item is string stringItem)
                {
                    return CreateTextBlockTemplate(stringItem);
                }
            }
            return CreateContentDataTemplate(item);
        }
        public DataTemplate CreateTextBlockTemplate(object obj)
        {
            var factory = new FrameworkElementFactory(typeof(TextBlock));
            factory.SetBinding(TextBlock.TextProperty, new Binding() { Source = obj });
            factory.SetBinding(TextBlock.MarginProperty, new Binding()
            {
                RelativeSource = new RelativeSource(RelativeSourceMode.FindAncestor, typeof(ContentControl), 1),
                Path = new PropertyPath(Control.PaddingProperty)
            });
            factory.SetValue(TextBlock.TextTrimmingProperty, TextTrimming.CharacterEllipsis);
            var dataTemplate = new DataTemplate()
            {
                VisualTree = factory
            };
            dataTemplate.Seal();
            return dataTemplate;
        }
        public DataTemplate CreateContentDataTemplate(object obj) 
        {
            var factory=new FrameworkElementFactory(typeof(ContentPresenter));
            factory.SetBinding(ContentPresenter.ContentProperty, new Binding() { Source = obj });
            factory.SetValue(ContentPresenter.FocusableProperty, false);
            factory.SetBinding(ContentPresenter.MarginProperty, 
                new Binding() { RelativeSource=new RelativeSource( RelativeSourceMode.FindAncestor,typeof(ContentControl),1),Path=new PropertyPath(Control.PaddingProperty)});
            factory.SetBinding(ContentPresenter.VerticalAlignmentProperty, new Binding() { RelativeSource=new RelativeSource(RelativeSourceMode.FindAncestor,typeof(ContentControl),1),Path=new PropertyPath(Control.VerticalContentAlignmentProperty)});
            factory.SetBinding(ContentPresenter.HorizontalAlignmentProperty, new Binding() { RelativeSource = new RelativeSource(RelativeSourceMode.FindAncestor, typeof(ContentControl), 1), Path = new PropertyPath(Control.HorizontalContentAlignmentProperty) });
            var dataTemplate = new DataTemplate()
            {
                VisualTree = factory
            };
            dataTemplate.Seal();
            return dataTemplate;
        }

    }
}
