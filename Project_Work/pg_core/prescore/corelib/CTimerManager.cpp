#ifdef PLATFORM_WIN32
#include<iostream>

#include "CTimerManager.h"
#include"core/logging.h"
#include"EventQueueAbstraction.h"
#include"core/CTimerInterface.h"
#include<limits.h>

using namespace std;

#define INVALID_TIMER_ID (int)(-1)
#define TIMERID_STARTVALUE (int)(1)

static int assign_timer_id = TIMERID_STARTVALUE;

std::map<int, CTimerManager::TimerInfo> CTimerManager::mTimerList;
CTimerManager* CTimerManager::mSelf = nullptr;

CTimerManager::CTimerManager()
{
	mSelf = this;
}

CTimerManager* CTimerManager::self()
{
	return mSelf;
}

int CTimerManager::setTimer(ITimerEventListener& listener, CTimerInterface::timerType type, int duration_ms)
{
	int id = INVALID_TIMER_ID;
	
	HANDLE win_timer_handle = NULL;

	TimerInfo tInfo(listener);

	std::pair<std::map<int, TimerInfo>::iterator, bool> res = mTimerList.insert(std::make_pair(assign_timer_id, tInfo));

	if (!res.second)
	{
		LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "CHBTimerManager::createTimer - timer with timer id=%d already exists\n", assign_timer_id);

		/* If a timerID is already available in the map and we try inserting same ID again, this is an issue
		in ID assignment by pres ctrl core and should we erase the current timerID from map and return a INVALID_TIMER_ID to client indicating an error?
		*/
		mTimerList.erase(assign_timer_id);
		return id;
	}

	if (type == CTimerInterface::TIMER_ONESHOT)
	{
		if (CreateTimerQueueTimer(&win_timer_handle, NULL, (WAITORTIMERCALLBACK)(&timer_sig_handler),(void*)(&(mTimerList.find(assign_timer_id)->first)), duration_ms, 0, WT_EXECUTEDEFAULT) == 0)
		{
			LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "CTimerManager::setTimer-CreateTimerQueueTimer failed  \n");
			mTimerList.erase(assign_timer_id);
		}
		else
		{
			if (win_timer_handle == NULL)
			{
				LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "CTimerManager::setTimer-CreateTimer failed \n");
				mTimerList.erase(assign_timer_id);
			}
			else
			{
				(mTimerList.find(assign_timer_id)->second).mHandle = win_timer_handle;
				id = assign_timer_id;
			}
		}
	}
	else
	{
		if (CreateTimerQueueTimer(&win_timer_handle, NULL, (WAITORTIMERCALLBACK)(CTimerManager::timer_sig_handler), (void*)(&(mTimerList.find(assign_timer_id)->first)), duration_ms, duration_ms, WT_EXECUTEDEFAULT) == 0)
		{
			LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "CTimerManager::setTimer-CreateTimerQueueTimer failed  \n");
			mTimerList.erase(assign_timer_id);
		}
		else
		{
			if (win_timer_handle == NULL)
			{
				LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "CTimerManager::setTimer-CreateTimer failed \n");
				mTimerList.erase(assign_timer_id);
			}
			else
			{
				(mTimerList.find(assign_timer_id)->second).mHandle = win_timer_handle;
				id = assign_timer_id;

			}
		}
	}
	LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CTimerManager::setTimer-id is %d \n", id);

	assign_timer_id++;
	if (assign_timer_id==INT16_MAX)
	{
		assign_timer_id = TIMERID_STARTVALUE;
	}

	return id;
}

void CTimerManager::cancelTimer(int timerId)
{
	HANDLE win_timer_handle = NULL;
	std::map<int,TimerInfo>::iterator itr = mTimerList.find(timerId);

	if (itr == mTimerList.end())
	{

	}
	else
	{
		win_timer_handle = (itr)->second.mHandle;

		if (win_timer_handle != NULL)
		{
			/*	Check which flag has to be passed for the last parameter.Whether NULL or INVALID_HANDLE_VALUE ?

					INVALID_HANDLE_VALUE is blocking and will make sure that deleteTimer call returns only after all the pending callback for that timer expires.
					NULL means there will be an immediate asynchronous return with a IO_PENDING_ERROR and the windows will internally do the clean up in the background for the pending callbacks.

					*/
			if (DeleteTimerQueueTimer(NULL, win_timer_handle, NULL) == 0)
			{
				LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "CTimerManager::cancelTimer-cancelTimer failed for timer handle=%u \n", win_timer_handle);
			}
		}
		else
		{
			LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "CTimerManager::cancelTimer-timer not present \n");
		}

		win_timer_handle = NULL;
	}

	mTimerList.erase(timerId);
}

VOID CALLBACK CTimerManager::timer_sig_handler(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	if (lpParameter != NULL)
	{
		int timerID = *(int*)lpParameter;
		std::thread::id tid = std::this_thread::get_id();
		LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CTimerManager::timer_sig_handler-timerEvent for timer id=%d, timer thread id is %d \n", timerID, tid);
		EventQueueAbstraction::addTimerEvent(timerID);
	}
}

void CTimerManager::invokeListener(int timerId)
{
	std::map<int, TimerInfo>::iterator itr = mTimerList.find(timerId);

	if (itr == mTimerList.end())
	{
		LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CTimerManager::invokeListener-timerEvent for timer id=%d but timer is already deleted", timerId);
	}
	else
	{
		std::thread::id tid = std::this_thread::get_id();
		LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CTimerManager::invokeListener-timerEvent id=%d", timerId);
		(itr)->second.mListener.timerEvent(timerId);
	}
}
#else
#include <string.h>
#include "CTimerManager.h"
#include "core/logging.h"
#include "CMainLoop.h"

using namespace std;

#define INVALID_TIMER_ID (int)(-1)

#define SEC_TO_MILLISEC (1000)
#define MILLISEC_TO_NANOSEC (1000000)

std::map<int, CTimerManager::TimerInfo> CTimerManager::mTimerList;
CTimerManager* CTimerManager::mSelf = nullptr;

CTimerManager::CTimerManager()
{
	mSelf = this;
}

CTimerManager::~CTimerManager()
{
}

CTimerManager* CTimerManager::self()
{
	return mSelf;
}

void CTimerManager::Init()
{
}

int CTimerManager::setTimer(ITimerEventListener& listener, CTimerInterface::timerType type, int duration_ms)
{
	TimerInfo tInfo(listener);
	int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
	
	if (tfd == INVALID_TIMER_ID)
	{
		LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "CTimerManager::setTimer - timer_create failed with errno=%d,message=%s", errno, strerror(errno));
	}
	else
	{
		long ntime = 0;
		time_t stime = 0;
		struct itimerspec itime;

		stime = duration_ms / SEC_TO_MILLISEC;
		ntime = (duration_ms % SEC_TO_MILLISEC) * MILLISEC_TO_NANOSEC;

		LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CTimerManager::setTimer - stime=%ld,ntime=%ld\n",stime,ntime);

		if (type == CTimerInterface::TIMER_ONESHOT)
		{		
			itime.it_value.tv_sec = stime;
			itime.it_value.tv_nsec = ntime;
			itime.it_interval.tv_sec = 0;
			itime.it_interval.tv_nsec = 0;
		}
		else
		{
			itime.it_value.tv_sec = stime;
			itime.it_value.tv_nsec = ntime;
			itime.it_interval.tv_sec = stime;
			itime.it_interval.tv_nsec = ntime;
		}

		tInfo.mTimerType = type;
		auto res = mTimerList.insert(std::make_pair(tfd, std::move(tInfo)));
		
		//Add and start listening
		CMainLoop::Self()->addWatchFd(E_TIMER_FD, tfd);
		if (timerfd_settime(tfd, 0, &itime, NULL) == -1)
		{
			LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "CTimerManager::setTimer - timer_settime failed with errno=%d,message=%s\n", errno, strerror(errno));
			mTimerList.erase(res.first);
			CMainLoop::Self()->removeWatchFd(tfd);
			tfd = INVALID_TIMER_ID;
		}
		else
		{
			res.first->second.mTimerFd = tfd;
		}
	}
	return tfd;
}

void CTimerManager::cancelTimer(int timerId)
{
	auto itr = mTimerList.find(timerId);
	if (itr != mTimerList.end())
	{
		mTimerList.erase(itr);
		CMainLoop::Self()->removeWatchFd(timerId);

		//clean timer object
		close(timerId);
	}
}

void CTimerManager::invokeListener(int timerId)
{
	LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CTimerManager::invokeListener-timer=%d", timerId);
	std::map<int, TimerInfo>::iterator itr = mTimerList.find(timerId);

	if (itr == mTimerList.end())
	{
		LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CTimerManager::invokeListener-timerEvent for timer event id=%d but timer is already deleted", timerId);
	}
	else
	{
		(itr)->second.mListener.timerEvent(timerId);
		
		//cancel and free up fd if TIMER_ONESHOT
		if (itr->second.mTimerType == CTimerInterface::TIMER_ONESHOT) {
			mSelf->cancelTimer(timerId);
		}
	}
}

#endif
