/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            CHBTimerManager.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)

* @brief           CHBTimerManager will be used internally by pres ctrl core for managing different timers and
                   assigning unique user friendly ID's to the timers for tracking purposes. a CHBTimerManagerInterface will be used of for a specific 
				   OS (windows/linux). This friendly ID will be passed to the CHBTimerInterface which is used by clients for doing different timer operations. Also
                   this is used to dispatch each timer callback events to it's corresponding client listener.

				   This interface is used only by pres ctrl core and not accessible to clients.
*/

#ifndef _CTIMERMANAGER_HPP_
#define _CTIMERMANAGER_HPP_

#ifndef WINVER
#define WINVER 0x500
#endif
#ifdef PLATFORM_WIN32
#include<Windows.h>
#include"core/CTimerInterface.h"
#include<map>
#include<mutex>

class CTimerManager
{
	struct TimerInfo
	{
		TimerInfo(ITimerEventListener& listener) :mListener(listener),mHandle(NULL)
		{

		}

		ITimerEventListener& mListener;
		HANDLE mHandle;
	};

	static VOID CALLBACK timer_sig_handler(PVOID lpParameter,BOOLEAN TimerOrWaitFired);
	static std::map<int,TimerInfo> mTimerList;
	static CTimerManager* mSelf;
public:
	CTimerManager();
	static CTimerManager* self();
	void Init() {}
	int setTimer(ITimerEventListener& listener, CTimerInterface::timerType type, int duration_ms);
	void cancelTimer(int timerId);
	static void invokeListener(int timerId);
};
#else

#include <map>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/timerfd.h>
#include <mutex>
#include "core/CTimerInterface.h"

class CTimerManager
{

	struct TimerInfo
	{
		TimerInfo(ITimerEventListener& listener) :mListener(listener)
		{

		}

		ITimerEventListener& mListener;
		int mTimerFd;
		CTimerInterface::timerType mTimerType;
	};

	static std::map<int, TimerInfo> mTimerList;
	static CTimerManager* mSelf;
public:
	CTimerManager();
	~CTimerManager();
	static CTimerManager* self();
	void Init();
	int setTimer(ITimerEventListener& listener, CTimerInterface::timerType type, int duration_ms);
	void cancelTimer(int timerId);

	static void invokeListener(int timerId);
};

#endif
#endif
