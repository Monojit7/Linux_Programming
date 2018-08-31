/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            CommonAPIMainLoopEngine.cpp
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           Handles the platfrom independent implementation to integrate the common api main loop to the pres ctrl core main loop. 
*/

#include "CommonAPIMainLoopEngine.h"
#include "PresCtrlCore.h"
#include <core/logging.h>
#ifndef PLATFORM_WIN32
#include <sys/select.h>
#include <poll.h>
#include <pthread.h>
#include "CMainLoop.h"
#endif


std::thread* CommonAPIMainLoopEngine::mWatcherThread = nullptr; 

CommonAPIMainLoopEngine* CommonAPIMainLoopEngine::mpMySelf = nullptr;

std::mutex CommonAPIMainLoopEngine::mWatchMutex;

std::unordered_map<uint32_t, CommonAPI::Watch*> CommonAPIMainLoopEngine::mWatches;

CommonAPIMainLoopEngine::CommonAPIMainLoopEngine(PresCtrlCore& core)
	:mCore(core)
{
	mpMySelf = this; 
	mainloopContext = std::make_shared<CommonAPI::MainLoopContext>("dbusconnection");
	mainloopContext->subscribeForWatches(watchAddedCallback, watchRemovedCallback);
}

void CommonAPIMainLoopEngine::watchAddedCallback(CommonAPI::Watch* watch, const CommonAPI::DispatchPriority dispatchPriority)
{
	
#ifdef  PLATFORM_WIN32
	mWatcherThread = new std::thread(pollLoop, watch);
#else
	mWatchMutex.lock();
	mWatches[watch->getAssociatedFileDescriptor().fd] = watch;
	mWatchMutex.unlock();

	CMainLoop::Self()->addWatchFd(
		E_CAPI_FD,
		watch->getAssociatedFileDescriptor().fd,
		watch->getAssociatedFileDescriptor().events);
#endif
}

void CommonAPIMainLoopEngine::watchRemovedCallback(CommonAPI::Watch* watch)
{
#ifndef  PLATFORM_WIN32
	mWatchMutex.lock();
	mWatches.erase(watch->getAssociatedFileDescriptor().fd);
	mWatchMutex.unlock();

	CMainLoop::Self()->removeWatchFd(watch->getAssociatedFileDescriptor().fd);
#endif
}

void CommonAPIMainLoopEngine::pollLoop(CommonAPI::Watch* watch)
{
#ifdef PLATFORM_WIN32
	pollfd pollfds = watch->getAssociatedFileDescriptor(); 
	//LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "Poll Event Data : %d", poll.events);
	while (SOCKET_ERROR != WSAPoll(&pollfds, 1, -1))
	{
		mpMySelf->mCore.addCAPIEvent(watch, pollfds.revents);
	}
#endif
}

void CommonAPIMainLoopEngine::handleEvent(uint32_t fd, uint32_t revents)
{
	CommonAPI::Watch* watch = nullptr;

	mWatchMutex.lock();
	auto watchIter = mWatches.find(fd);
	if (watchIter != mWatches.end())
	{
		watch = watchIter->second;
	}
	mWatchMutex.unlock();

	if (watch != nullptr)
	{
		watch->dispatch(revents);

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
	else
	{
		LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "CommonAPIMainLoopEngine::handleEvent %d removed!!", fd);
	}
}
