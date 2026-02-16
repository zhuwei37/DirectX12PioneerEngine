using Editor.Time;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Editor.RenderDx
{
    public class CameraController
    {
        private Point lastMouseDownPos;
        private uint renderHostId;
        public CameraController(uint _renderHostId)
        { 
            this.renderHostId = _renderHostId;
        }
        public void MouseDown(int x,int y)
        {
            lastMouseDownPos.X = x;
            lastMouseDownPos.Y=y;
        }
        public void MouseMove(int x, int y, bool IsLeft)
        {
            if (IsLeft)
            {
                int curX = x;
                int curY = y;
                float dx = (curX - lastMouseDownPos.X) * 0.25f * (float)(Math.PI / 180.0f);
                float dy = (curY - lastMouseDownPos.Y) * 0.25f * (float)(Math.PI / 180.0f);
                Pitch(dy);
                RotateY(dx);
            }
            lastMouseDownPos.X = x;
            lastMouseDownPos.Y = y;
        }
        public void MouseUp()
        {
         
        }
        public void KeyDonw()
        {
           
            if (Keyboard.IsKeyDown(Key.W))
            {
               Walk(5.0f * GameTimer.Instance.DeltaTime);
            }
            if (Keyboard.IsKeyDown(Key.S))
            {
                Walk(-5.0f * GameTimer.Instance.DeltaTime);
            }
            if (Keyboard.IsKeyDown(Key.A))
            {
               Strafe(-5.0f * GameTimer.Instance.DeltaTime);
            }
            if (Keyboard.IsKeyDown(Key.D))
            {
                Strafe(5.0f * GameTimer.Instance.DeltaTime);
            }
        }

        public void Walk(float d)
        {
            EngineDxImport.Walk(renderHostId, d);
        }
        public void Strafe(float d)
        { 
            EngineDxImport.Strafe(renderHostId,d);
        }
        public void Pitch(float d)
        { 
            EngineDxImport.Pitch(renderHostId, d);
        }
        public void RotateY(float d)
        {
            EngineDxImport.RotateY(renderHostId, d);
        }
    }
}
