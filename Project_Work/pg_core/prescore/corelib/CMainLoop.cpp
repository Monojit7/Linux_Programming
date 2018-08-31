/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            PresCoreMainLoop.cpp
* @ingroup         presctrlcore
* @author          amit.yatheesh@harman.com	
* @brief           Mainloop class that handles CAPI/timer/thread events
*
*/
#ifndef PLATFORM_WIN32

#include "CMainLoop.h"
#include <core/logging.h>
#include "CommonAPIMainLoopEngine.h"
#include <assert.h>
#include <cstring>
#include <unistd.h>
#include "CTimerManager.h"
#include <core/CThreadManager.h>
#include "CEventManager.hpp"
#include "EventQueueAbstraction.h"
#include "PresCtrlCore.h"

#define MAX_EVENTS 50
#define READ_HANDLE 0
#define WRITE_HANDLE 1

CMainLoop* CMainLoop::mSelf = nullptr;

struct ThreadData
{
	int id;
	BaseData* data;	
};
	
CMainLoop::CMainLoop()
{
	mSelf = this;

	mEpollFd = epoll_create1(0);
	assert(mEpollFd != -1);
	
	//Handle data from a thread
	pipe(mThreadCtrlFds);
	addWatchFd(E_THREAD_FD, mThreadCtrlFds[READ_HANDLE]);

	//Handle event from same / different thread
	pipe(mEventCtrlFds);
	addWatchFd(E_EVENT_FD, mEventCtrlFds[READ_HANDLE]);
	
	//Handle context switch from different thread
	pipe(mCtxSwitchCtrlFds);
	addWatchFd(E_CONTEXT_SWITCH_FD, mCtxSwitchCtrlFds[READ_HANDLE]);
	
	//ex:shutdown mainloop
	pipe(mCtrlFds);
	addWatchFd(E_EXIT_FD, mCtrlFds[READ_HANDLE]);
}

CMainLoop::~CMainLoop()
{

}

CMainLoop* CMainLoop::Self()
{
	return mSelf;
}

void CMainLoop::addWatchFd(eFdType type, uint32_t fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	
	if (-1 == epoll_ctl(mEpollFd, EPOLL_CTL_ADD, fd, &ev))
	{
		LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "addWatchFd error fd-%d, type-%d, error %d - %s", fd, type, errno, std::strerror(errno));
		return;
	}
	else
	{
		mFdMapMutex.lock();
		mFdMap[fd] = type;
		mFdMapMutex.unlock();
	}
	LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "addWatchFd fd %d type %d", fd, type);
}

void CMainLoop::removeWatchFd(uint32_t fd)
{
	if (-1 == epoll_ctl(mEpollFd, EPOLL_CTL_DEL, fd, nullptr))
	{
		LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "removeWatchFd error fd-%d, error %d - %s", fd, errno, std::strerror(errno));
		return;
	}
	else
	{
		mFdMapMutex.lock();
		LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "removeWatchFd fd %d type %d", fd, mFdMap[fd]);
		mFdMap.erase(fd);
		mFdMapMutex.unlock();
	}
}

void CMainLoop::addWorkerThreadEvent(int id,BaseData* threadData)
{
	struct ThreadData data;
	data.id = id;
	data.data = threadData;
	
	write(mThreadCtrlFds[WRITE_HANDLE], &data, sizeof(data));
}

void CMainLoop::addUserEventData(uint32_t id, BaseData* eventData)
{
	struct ThreadData data;
	data.id = id;
	data.data = eventData;
	
	write(mEventCtrlFds[WRITE_HANDLE], &data, sizeof(data));
}

void CMainLoop::addExternalEvent()
{
	char buf = '1';
	write(mCtxSwitchCtrlFds[WRITE_HANDLE], &buf, sizeof(buf));
}

void CMainLoop::shutdown()
{
	char buf = '1';
	write(mCtrlFds[WRITE_HANDLE], &buf, sizeof(buf));
}

void CMainLoop::mainloop()
{
	int epollNfds = 0;
	struct epoll_event events[MAX_EVENTS];
	
	if (-1 != (epollNfds = epoll_wait(mEpollFd, events, MAX_EVENTS, -1)))
	{
		for (int i = 0; i < epollNfds; ++i)
		{
			eFdType fdType = E_UNKNOWN;

			mFdMapMutex.lock();
			auto fdIter = mFdMap.find(events[i].data.fd);
			if (fdIter != mFdMap.end()) {
				fdType = fdIter->second ;
			}
			else {
				LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "CMainLoop::mainloop %d removed!!", events[i].data.fd);
			}
			mFdMapMutex.unlock();

			switch (fdType)
			{
			case E_CAPI_FD: {
				CommonAPIMainLoopEngine::handleEvent(events[i].data.fd, events[i].events);
				break;
			}
			case E_TIMER_FD: {
				uint64_t res;
				read(events[i].data.fd, &res, sizeof(res));
				CTimerManager::invokeListener(events[i].data.fd);
				break;
			}
			case E_THREAD_FD: {
				struct ThreadData res;
				read(events[i].data.fd, &res, sizeof(res));
				CThreadManager::self()->invokeDataHandler(res.id, res.data);
				break;
			}
			case E_EVENT_FD: {
				struct ThreadData res;
				read(events[i].data.fd, &res, sizeof(res));
				CEventManager::handleEventData(res.id, res.data); 
				break;
			}			
			case E_CONTEXT_SWITCH_FD: {
				char res;
				read(events[i].data.fd, &res, sizeof(res));
				
				EventQueueAbstraction::lockList();
				EventQueueAbstraction::InternalEventItem *item = EventQueueAbstraction::mInternalEventList.pop_head();
				EventQueueAbstraction::unlockList();
				
				std::function<void()>&& callback = std::move(item->mExternalEventCallback);
				callback();
				
				delete item;
				break;
			}
			case E_EXIT_FD: {
				char res;
				read(events[i].data.fd, &res, sizeof(res));
				break;
			}
			default:
				break;
			}

			PresCtrlCore::getCore().runQueue();
		}
	}
}
#endif