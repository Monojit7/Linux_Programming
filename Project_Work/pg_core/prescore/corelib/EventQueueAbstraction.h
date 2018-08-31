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
*				   Allocate memory (if any) before pushing the event to the Queue. The memory will be freed by the reader. 
*/

#ifndef _EVENT_QUEUE_ABSTRACTION_
#define _EVENT_QUEUE_ABSTRACTION_

#include <mutex>
#include <CommonAPI/CommonAPI.hpp>
#include <condition_variable>
#include <functional>
#include <core/LinkedList.h>
#include<core/IDataHandler.h>
#include<iostream>

class RequestBase;

class EventQueueAbstraction
{
public : 

	static void init(); 
	
	enum e_EventQueueAbsEventType
	{
		e_EVENT_TIMER_EVENT=0u,
		e_EVENT_CAPI_EVENT,
		e_EVENT_THREAD_EVENT,
		e_EVENT_NEW_REQ_EVENT,
		e_EVENT_USER_EVENT_DATA,
		e_EVENT_EXTERNAL,
		e_EVENT_SHUTDOWN
	};

	class InternalEventItem
	{
	public : 
		InternalEventItem(e_EventQueueAbsEventType, uint64_t = 0u, uint64_t = 0u ,void* = nullptr, std::function<RequestBase*()>&& = nullptr, std::function<void()>&& =nullptr);

		e_EventQueueAbsEventType mType; 
		uint64_t				 mEventData1;
		uint64_t				 mEventData2;
		void*					 mpEventPtr;
		std::function<RequestBase*()>    mFunc;
		std::function<void()> mExternalEventCallback;
	};

	static void addTimerEvent(int TimerID);
	static void addCommonAPIEvent();
	static void addWorkerThreadEvent(int threadId,void* threadData=nullptr);
	static void addUserEventData(uint32_t eventId, void* data);
	static void addExternalEvent(std::function<void()>&&);
	static void addNewCAPIEvent(CommonAPI::Watch* watch, uint32_t revents);
	static void addShutdownEvent();

	static void waitForEvents(std::unique_lock<std::mutex>& lock); 

	static std::mutex& lockList();
	static std::mutex& unlockList();


	static LinkedList<InternalEventItem> mInternalEventList; /// Must be accessed in a thread safe manner. 
	static std::mutex mQueueMutex;
	static std::condition_variable mCAPICond;
private : 

	static std::condition_variable mCond;
	static int32_t mMainLoopThreadID; 
	
};


#endif
