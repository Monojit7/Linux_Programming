#include<iostream>
#include "WdgManager.h"
#include"core/logging.h"
#include"EventQueueAbstraction.h"
#include"core/CTimerInterface.h"
#include<limits.h>

#ifndef PLATFORM_WIN32
#include "systemd/sd-daemon.h"
#endif

using namespace std;

WdgManager::WdgManager(int timer):
	mWdgTimer(*this),
	mWdgTimerId(0u)
{
	startTimer(timer);
}

WdgManager::~WdgManager()
{
	stopTimer();
}

void WdgManager::startTimer(int timer)
{
	mWdgTimerId = mWdgTimer.setTimer(CTimerInterface::TIMER_PERIODIC,timer);
	LOG_DEBUG(Log::e_LOG_CONTEXT_CORE,"WdgManager::startTimer -timer=%d,mWdgTimerId=%d",timer,mWdgTimerId);
}

void WdgManager::timerEvent(int timerId) 
{
#ifndef PLATFORM_WIN32
	sd_notify (0, "WATCHDOG=1");
#endif
}

void WdgManager::stopTimer()
{
	if (mWdgTimerId)
	{
		mWdgTimer.cancelTimer(mWdgTimerId); // cancel timer
		mWdgTimerId = 0;
	}
}
