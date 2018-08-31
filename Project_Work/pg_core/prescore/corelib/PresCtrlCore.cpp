/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            PresCtrlCore.cpp
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           The core internal framework class for the Presentation Control Core
*
*/
#include "PresCtrlCore.h"
#include<iostream>
#include "core/logging.h"
#include "core/CTimerInterface.h"
#include "CTimerManager.h"
#include "core/CThreadManager.h"
#include "core/IDataHandler.h"
#include "LifeCycleManager.h"
#include "CEventManager.hpp"
#ifndef PLATFORM_WIN32
#include <systemd/sd-daemon.h>
#endif
#include "CMainLoop.h"

PresCtrlCore* PresCtrlCore::mCore = nullptr;
LifeCycleManager* PresCtrlCore::mlifeCycleManager = nullptr;
WdgManager* PresCtrlCore::mWdgManager = nullptr;

/**
* \brief  The core and entry point of the PresCtrl.
*/
PresCtrlCore::PresCtrlCore(const std::unordered_map <uint64_t, Request> & reqTable, ShutdownCallback && sCallback, bool bDelayedCAPIInit)
	:mManager(reqTable)
	,msCallback(sCallback)
	,mCAPIEngine(nullptr)
	,mThreadManager(nullptr)
	,mRunMainLoop(true)
{
	EventQueueAbstraction::init(); 

	if (!bDelayedCAPIInit)
	{
		mCAPIEngine = new CommonAPIMainLoopEngine(*this);
		
	}
	
	mCore = this;
	mThreadManager = new CThreadManager();
	createWdg();
}

void PresCtrlCore::createWdg()
{
	char* timeout;
	timeout = getenv("WATCHDOG_USEC");
	if (timeout != nullptr)
	{
		int timer = atoi(timeout)/2000;
		LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "PresCtrlCore::createWdg timer=%d",timer);
		mWdgManager = new WdgManager(timer);
	}
}

PresCtrlCore::~PresCtrlCore()
{
	if (mCAPIEngine != nullptr) {
		delete mCAPIEngine;
	}
	if (mlifeCycleManager != nullptr) {
		delete mlifeCycleManager;
	}
	if (mWdgManager != nullptr){
		delete mWdgManager;
	}
}

void PresCtrlCore::initLCM()
{
	mlifeCycleManager = (msCallback == nullptr) ? nullptr : new LifeCycleManager(std::move(msCallback));
}

void PresCtrlCore::delayedCAPIInit()
{
	if (nullptr == mCAPIEngine)
	{
		mCAPIEngine = new CommonAPIMainLoopEngine(*this);
	}
}

PresCtrlCore& PresCtrlCore::getCore()
{
	return *mCore;
}


e_AddReqResponse PresCtrlCore::addRequest(uint32_t RequestID, std::function<RequestBase*()>&& requestFunc, uint64_t DynamicGroupID)
{
	return mManager.addRequest(RequestID, std::move(requestFunc), DynamicGroupID);
}

void PresCtrlCore::addCAPIEvent(CommonAPI::Watch* watch, uint32_t revents)
{
	EventQueueAbstraction::addNewCAPIEvent(watch,revents);
}

void PresCtrlCore::addWorkerThreadData(int threadId,BaseData* threadData)
{
#ifdef PLATFORM_WIN32
	EventQueueAbstraction::addWorkerThreadEvent(threadId,threadData);
#else
	CMainLoop::Self()->addWorkerThreadEvent(threadId,threadData);
#endif
}

void PresCtrlCore::publishEvent(uint32_t EventID, BaseData* pData)
{
#ifdef PLATFORM_WIN32
	EventQueueAbstraction::addUserEventData(EventID, pData);
#else
	CMainLoop::Self()->addUserEventData(EventID,pData);
#endif	
}

void PresCtrlCore::addExternalEvent(std::function<void()>&& callback)
{
	//continue to use queue for context switch
	EventQueueAbstraction::addExternalEvent(std::move(callback));
#ifndef PLATFORM_WIN32
	CMainLoop::Self()->addExternalEvent();
#endif
}

/**
* \brief  Cleans up everything from the Queue.
*/
void PresCtrlCore::purgeAllRequests()
{

}


/**
* @brief Cleansup the core. Call this if application is terminating.
*/
void PresCtrlCore::shutDown()
{
#ifdef PLATFORM_WIN32
	EventQueueAbstraction::addShutdownEvent();
#else
	mRunMainLoop = false; 
	mLinuxMainloop.shutdown();
#endif
}

/**
* @brief  Removes all requests belonging to a specific group.
*/
void PresCtrlCore::purgeAllRequestsInGroup(uint64_t GroupID, uint64_t GroupDynamicID)
{

}

void PresCtrlCore::runQueue()
{
	mManager.runQueue();
}

void PresCtrlCore::mainLoop(bool initNotify)
{
	LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "MainLoop started");

#ifndef PLATFORM_WIN32
	// Pres ctrl init is done , proceed with notification (currently only systemd) and request processing
	if (true == initNotify)
	{
		sd_notify(0, "STATUS=Prescore init successful");
		sd_notify(0, "READY=1");
	}
	else {
		// do nothing
	}
#endif

	do
	{
#ifdef PLATFORM_WIN32
		std::unique_lock<std::mutex> lock(EventQueueAbstraction::mQueueMutex);
		
		EventQueueAbstraction::InternalEventItem *item = nullptr;

		while ( nullptr !=  ( item = EventQueueAbstraction::mInternalEventList.pop_head()) )
		{
			decodeAndHandleEventIntern(item);
		}

		// wait for events if loop should run , else exit
		if (mRunMainLoop == true)
		{
			EventQueueAbstraction::waitForEvents(lock);
		}
#else
		mLinuxMainloop.mainloop();
#endif
	} while (mRunMainLoop);

	LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "PresCtrlCore::mainLoop exit");

#ifndef PLATFORM_WIN32
	if (true == initNotify)
	{
		sd_notify(0, "STATUS=Prescore shutdown successful");
		sd_notify(0, "STOPPING=1");
	}
	else {
		// do nothing
	}
#endif
}

void PresCtrlCore::decodeAndHandleEventIntern(EventQueueAbstraction::InternalEventItem * item)
{
	switch (item->mType)
	{
		case EventQueueAbstraction::e_EVENT_TIMER_EVENT:
		{
			int id = item->mEventData1;
			CTimerManager::invokeListener(id);
		}
		break;
		case EventQueueAbstraction::e_EVENT_USER_EVENT_DATA:
		{
			uint32_t id = static_cast<uint32_t>(item->mEventData1);
			BaseData* data = static_cast<BaseData*>(item->mpEventPtr);
			CEventManager::handleEventData(id, data); 
		}
		break;
		case EventQueueAbstraction::e_EVENT_CAPI_EVENT:
		{
			CommonAPI::Watch* watch = static_cast<CommonAPI::Watch*>(item->mpEventPtr);

			if (watch != nullptr) {

				CommonAPI::Watch* watch = static_cast<CommonAPI::Watch*>(item->mpEventPtr);
				
				watch->dispatch(item->mEventData1);

				const std::vector<CommonAPI::DispatchSource*>& dependentSources = watch->getDependentDispatchSources();
				int32_t depSize = dependentSources.size();
				for (int i = 0; i < depSize; i++)
				{
					if (dependentSources[i]->check())
					{
						while (true == dependentSources[i]->dispatch())
						{
							//run again
						}
					}
				}
			}
			EventQueueAbstraction::mCAPICond.notify_one();
		}
		break;
		case EventQueueAbstraction::e_EVENT_THREAD_EVENT:
		{
			int id = item->mEventData1;
			BaseData* data = static_cast<BaseData*>(item->mpEventPtr);
			mThreadManager->invokeDataHandler(id,data);
		}
		break;
		case EventQueueAbstraction::e_EVENT_EXTERNAL:
		{
			std::function<void()>&& callback = std::move(item->mExternalEventCallback);
			callback();
		}
		break;
		case EventQueueAbstraction::e_EVENT_SHUTDOWN :
		{
			mRunMainLoop = false;
		}
		default : 
		{

		}
		break;
	}

	mManager.runQueue();
	
	delete item; 
}
