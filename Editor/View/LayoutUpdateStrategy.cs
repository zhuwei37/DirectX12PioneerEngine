
using Editor.Interface.UI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xceed.Wpf.AvalonDock.Layout;

namespace Editor.View
{
    internal class LayoutUpdateStrategy : ILayoutUpdateStrategy
    {
       
        public void AfterInsertAnchorable(LayoutRoot layout, LayoutAnchorable anchorableShown)
        {
           
        }

        public void AfterInsertDocument(LayoutRoot layout, LayoutDocument anchorableShown)
        {
           var p= layout.Parent;
        }

        public bool BeforeInsertAnchorable(LayoutRoot layout, LayoutAnchorable anchorableToShow, ILayoutContainer destinationContainer)
        {
            //   anchorableToShow.D = 200;
           
            ITool inTool;
            if (anchorableToShow.Content != null && anchorableToShow.Content is ITool tool)
            {
                inTool = tool;
            }
            else
            {
                return false;
            }
            var toolPane = layout.Descendents().OfType<LayoutAnchorable>().Where((item) =>
            {
                ITool? tool = item.Content as ITool;
                if (tool != null)
                {
                    if (tool.UIToolType == inTool.UIToolType)
                    {
                        return true;
                    }
                }
                return false;
            }).FirstOrDefault();
            if (toolPane != null)
            {
                //toolPane.PreviousContainerIndex
                var parent = toolPane.Parent;
                if (parent != null&&parent is LayoutAnchorablePane layoutAnchorablePane) 
                {
                    layoutAnchorablePane.Children.Add(anchorableToShow);
                    return true;
                }
            }
            else
            {
                switch (inTool.UIToolType)
                {
                    case ToolType.Preview:
                        {
                            
                            anchorableToShow.AddToLayout(layout.Manager, AnchorableShowStrategy.Left);
                           
                            if (anchorableToShow.Parent is LayoutAnchorablePane pane)
                            {
                                pane.DockWidth = new System.Windows.GridLength( 150);
                            }
                           
                            //layoutAnchorablePane.Dir
                            return true;
                        }
                    case ToolType.Info:
                        {
                            anchorableToShow.AddToLayout(layout.Manager, AnchorableShowStrategy.Bottom);

                            if (anchorableToShow.Parent is LayoutAnchorablePane pane)
                            {
                                pane.DockHeight = new System.Windows.GridLength(300);
                            }
                            return true;
                        }
                    case ToolType.Tool:
                        {
                            anchorableToShow.AddToLayout(layout.Manager, AnchorableShowStrategy.Right);

                            if (anchorableToShow.Parent is LayoutAnchorablePane pane)
                            {
                                pane.DockWidth = new System.Windows.GridLength(400);
                            }
                            return true;
                        }
                    default:break;
                }
            }
            return false;
        }

        public bool BeforeInsertDocument(LayoutRoot layout, LayoutDocument anchorableToShow, ILayoutContainer destinationContainer)
        {
          return false;
        }
    }
}
