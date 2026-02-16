using GongSolutions.Wpf.DragDrop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.ViewModel.Info
{
    public partial class ResourcePanelViewModel : GongSolutions.Wpf.DragDrop.IDropTarget
    {
        #region 拖动
        //public bool CanStartDrag(IDragInfo dragInfo)
        //{
        //    throw new NotImplementedException();
        //}

        //public void DragCancelled()
        //{
        //    throw new NotImplementedException();
        //}

        //public void DragDropOperationFinished(System.Windows.DragDropEffects operationResult, IDragInfo dragInfo)
        //{
        //    throw new NotImplementedException();
        //}

        //public void Dropped(IDropInfo dropInfo)
        //{
        //    throw new NotImplementedException();
        //}

        //public void StartDrag(IDragInfo dragInfo)
        //{
        //    throw new NotImplementedException();
        //}

        //public bool TryCatchOccurredException(Exception exception)
        //{
        //    throw new NotImplementedException();
        //}
        #endregion
        #region 拖放
        public void DragOver(IDropInfo dropInfo)
        {
            if (dropInfo.Data is System.Windows.DataObject dataObject)
            {
               // var files=dataObject.GetFileDropList();
                dropInfo.Effects = System.Windows.DragDropEffects.Copy;
              // dataObject.Get
            }
        }

        public void Drop(IDropInfo dropInfo)
        {
            if (dropInfo.Data is System.Windows.DataObject dataObject)
            {
                var files=dataObject.GetFileDropList();
                foreach (var file in files)
                {
                    System.Diagnostics.Debug.WriteLine(file);
                }
            }
        }
        #endregion
       
    }
}
