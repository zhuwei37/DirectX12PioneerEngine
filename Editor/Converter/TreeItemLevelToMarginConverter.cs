using Editor.Control.TreeView;
using System;
using System.Collections.Generic;
using System.Drawing.Printing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;

namespace Editor.Converter
{
    public class TreeItemLevelToMarginConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
           // double colunwidth = 10;
            double left = 0.0;

            if (value is TreeViewItem item)
            {

                var p = VisualTreeHelper.GetParent(item);
                while (p is not PreviewSceneTreeView )
                { 
                  p= VisualTreeHelper.GetParent(p);
                    if (p is ItemsPresenter presenter)
                    {
                        if (presenter.Name == "PART_Items")
                        {
                            left += 10;
                        }
                    }
                }

            }
            return new Thickness(left, 0, 0, 0);
        }
        
        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
