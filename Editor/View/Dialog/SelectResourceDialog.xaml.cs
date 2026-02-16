using Editor.RenderDx;
using Editor.ViewModel.Dialog;
using Editor.ViewModel.Info;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Editor.View.Dialog
{
    /// <summary>
    /// SelectResourceDialog.xaml 的交互逻辑
    /// </summary>
    public partial class SelectResourceDialog : Window
    {
        SelectResourceDialogViewModel VM;
        public SelectResourceDialog(ResourceType resourceType)
        {
            InitializeComponent();
            VM= new SelectResourceDialogViewModel(resourceType);    
            this.DataContext = VM;
        }
        public ResourceItemViewModel? GetSelectedResource()
        {
            return VM.SelectedResource;
        }
        private void Confirm_Click(object sender, RoutedEventArgs e)
        {
            if (VM.SelectedResource != null)
            {
                this.DialogResult = true;
            }
        }
        
        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }
    }
}
