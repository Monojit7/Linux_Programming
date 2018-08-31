#include"core/CTimerInterface.h"
#include"CTimerManager.h"
#include<limits.h>
#include"core/logging.h"
#include"EventQueueAbstraction.h"
#include<iostream>
#include<list>

using namespace std;

CTimerInterface::CTimerInterface(ITimerEventListener& listener):mListener(listener)
{
	CTimerManager::self()->Init();
}

CTimerInterface::~CTimerInterface()
{
		for (std::list<int>::iterator itr = mTimerIdList.begin(); mTimerIdList.size()>0 && itr != mTimerIdList.end(); itr++)
		{
			CTimerManager::self()->cancelTimer((*itr));
			LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, " CTimerInterface::~CTimerInterface- deleted timer id=%d\n", (*itr));
		}

		mTimerIdList.clear();
}

int CTimerInterface::setTimer(timerType type, int duration_ms)
{
	int id = -1;

	id = CTimerManager::self()->setTimer(mListener, type, duration_ms);

	if (id > 0)
	{
		mTimerIdList.push_back(id);
	}

	return id;
}

void CTimerInterface::cancelTimer(int timerId)
{
	CTimerManager::self()->cancelTimer(timerId);

	for (std::list<int>::iterator itr = mTimerIdList.begin(); itr != mTimerIdList.end(); itr++)
	{
			if (timerId==(*itr))
			{
				mTimerIdList.erase(itr);
				LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, " CTimerInterface::cancelTimer- deleted timer id=%d \n", timerId);
				break;
			}
	}	
}
//#endif

