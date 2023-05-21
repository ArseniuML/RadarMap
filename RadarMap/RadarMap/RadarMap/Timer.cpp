#include "StdAfx.h"
#include "Timer.h"

namespace RadarMap
{ 
	namespace Timer
	{
		int mTimeToUpdate;	
		IlmThread::Mutex mMutex;

		int GetTimeToUpdate()
		{
			IlmThread::Lock lock(mMutex);
			return mTimeToUpdate;
		}

		void SetTimeToUpdate(int timeToUpdate )
		{
			IlmThread::Lock lock(mMutex);
			mTimeToUpdate = timeToUpdate;
		}
	}
}

