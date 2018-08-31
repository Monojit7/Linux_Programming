/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            EventQueueAbstraction.h
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           EventQueue will be used to synchronize events between different threads.
*				   Since the Pres Ctrl is single threaded, all data from other threads which reaches the pres ctrl has to go through this queue.
*/

#include "EventQueueAbstraction.h"
#ifndef PLATFORM_WIN32
#include <pthread.h>
#endif


std::mutex EventQueueAbstraction::mQueueMutex;
LinkedList<EventQueueAbstraction::InternalEventItem> EventQueueAbstraction::mInternalEventList; /// Must be accessed in a thread safe manner. 
std::condition_variable EventQueueAbstraction::mCond;
std::condition_variable EventQueueAbstraction::mCAPICond;
int32_t EventQueueAbstraction::mMainLoopThreadID = -1;

EventQueueAbstraction::InternalEventItem::InternalEventItem(e_EventQueueAbsEventType type, uint64_t data1,uint64_t data2, void* ptr, std::function<RequestBase*()>&& func, std::function<void()>&& externalEvent)
	:mType(type)
	,mEventData1(data1)
	,mEventData2(data2)
	,mpEventPtr(ptr)
	,mFunc(func)
	,mExternalEventCallback(std::move(externalEvent))
{
}

void EventQueueAbstraction::init()
{
#ifdef PLATFORM_WIN32
	mMainLoopThreadID = GetCurrentThreadId(); 
#else
	mMainLoopThreadID = pthread_self(); 
#endif
}

void EventQueueAbstraction::addTimerEvent(int TimerID)
{
	lockList();
	mInternalEventList.push_tail(new InternalEventItem(e_EVENT_TIMER_EVENT, TimerID,0, nullptr,nullptr));
	mCond.notify_one();
	unlockList();
}

void EventQueueAbstraction::addWorkerThreadEvent(int threadId,void* threadData)
{
	lockList();
	mInternalEventList.push_tail(new InternalEventItem(e_EVENT_THREAD_EVENT, threadId, 0, threadData, nullptr));
	mCond.notify_one();
	unlockList();
}

void EventQueueAbstraction::addUserEventData(uint32_t eventId, void* data)
{
//We want to allow users to add events even from the Main Thread Context. Ensure we are not doing a double locking !
#ifdef PLATFORM_WIN32
	int32_t currentTid = GetCurrentThreadId();
#else
	int32_t currentTid = pthread_self();
#endif

	if (mMainLoopThreadID != currentTid)
	{
		lockList();
	}

	
	mInternalEventList.push_tail(new InternalEventItem(e_EVENT_USER_EVENT_DATA, eventId, 0, data, nullptr));

	if (mMainLoopThreadID != currentTid)
	{
		mCond.notify_one();
		unlockList();
	}


}

void EventQueueAbstraction::addNewCAPIEvent(CommonAPI::Watch* watch, uint32_t revents)
{
	std::unique_lock<std::mutex> lock(EventQueueAbstraction::mQueueMutex);
	mInternalEventList.push_tail(new InternalEventItem(e_EVENT_CAPI_EVENT, revents, 0, watch,nullptr));
	mCond.notify_one();
	mCAPICond.wait(lock);
}

void EventQueueAbstraction::addExternalEvent(std::function<void()>&& event)
{
	lockList();
	mInternalEventList.push_tail(std::move(new InternalEventItem(e_EVENT_EXTERNAL, 0, 0, nullptr, nullptr, std::move(event))));
#ifdef PLATFORM_WIN32
	mCond.notify_one();
#endif
	unlockList();
}

void EventQueueAbstraction::addShutdownEvent() 
{
#ifdef PLATFORM_WIN32
	int32_t currentTid = GetCurrentThreadId();
#else
	int32_t currentTid = pthread_self();
#endif

	if (mMainLoopThreadID != currentTid)
	{
		lockList();
	}

	mInternalEventList.push_tail(new InternalEventItem(e_EVENT_SHUTDOWN));

	if (mMainLoopThreadID != currentTid)
	{
		mCond.notify_one();
		unlockList();
	}
}

void EventQueueAbstraction::waitForEvents(std::unique_lock<std::mutex>& lock)
{
	mCond.wait(lock);
}

std::mutex& EventQueueAbstraction::lockList()
{
	mQueueMutex.lock();
	return mQueueMutex;
}

std::mutex& EventQueueAbstraction::unlockList()
{
	mQueueMutex.unlock();
	return mQueueMutex;
}
