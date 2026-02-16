using Editor.Control.TreeView;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls.Primitives;

namespace Editor.Control.ToggleButtons
{
    public class TreeItemSwitchButton: ToggleButton
    {
        static TreeItemSwitchButton()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(TreeItemSwitchButton), new FrameworkPropertyMetadata(typeof(TreeItemSwitchButton)));
        }
    }
}
