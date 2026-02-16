using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Time
{
    public class GameTimer
    {
        public static GameTimer Instance { get; } = new Lazy<GameTimer>(()=> new GameTimer()).Value;
        System.Diagnostics.Stopwatch timer = new System.Diagnostics.Stopwatch();
        public GameTimer()
        {
            mSecondsPreCount = 1.0f/ (float)System.Diagnostics.Stopwatch.Frequency;
            timer.Start();
        }
        public float TotalTime { get {
                if (mStoped)
                {
                  return  (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPreCount);
                }
                else
                {
                    return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPreCount);
                }
            } }
        
        public float DeltaTime { get { return mDeltaTime; } }
        public void Reset()
        {
            long currTime = timer.ElapsedTicks;

            mBaseTime = currTime;
            mPrevTime = currTime;
            mStopTime = 0;
            mStoped = false;
        }
        public void Start()
        {
            long startTime = timer.ElapsedTicks;
            if (mStoped)
            {
                mPausedTime += (startTime - mStopTime);
                mPrevTime = startTime;
                mStopTime = 0;
                mStoped = false;
            }
        }
        public void Stop()
        {
            if (!mStoped)
            {
                long currTime;
                currTime=timer.ElapsedTicks;
                mStopTime = currTime;
                mStoped = true;
            }
        }
        public void Tick()
        {
            if (mStoped)
            {
                mDeltaTime = 0.0f;
                return;
            }
            long currTime;
            currTime = timer.ElapsedTicks;
            mCurrTime=currTime;
            mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPreCount;
            mPrevTime = mCurrTime;
            if (mDeltaTime < 0.0)
            {
                mDeltaTime = 0.0f;
            }
        }
        private float mSecondsPreCount;
        private float mDeltaTime;

        private long mBaseTime;
        private long mPausedTime;
        private long mStopTime;
        private long mPrevTime;
        private long mCurrTime;

        bool mStoped;

    }
}
