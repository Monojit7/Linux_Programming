/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            CThreadManager.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)

* @brief           CThreadManager will be used internally by pres ctrl core for managing different 
                   threads and assigning unique user friendly ID's to the threads for tracking 
				   purposes. CThreadManager will be used for a specific OS (windows/linux). 
				   This friendly ID will be passed to the CthreadInterface which is used by clients 
				   for doing different thread operations. Also this is used to invoke onData handler 
				   of the corresponding client when client wishes to process thread data in the 
				   mainloop context.

This interface is used only by pres ctrl core and not accessible to clients.
*/

#ifndef _CTHREADMANAGER_HPP_
#define _CTHREADMANAGER_HPP_

#include <list>
#include <map>
#include <thread>
#include "core/IDataHandler.h"
#include <core/PresCtrl.h>
#include <functional>
#include "core/logging.h"
#include <future>
//#include"core/CThreadInterface.h"
#ifndef PLATFORM_WIN32
#include <pthread.h>
#endif

using namespace std;
static int assign_thread_id = 1;

static int inc = 0;

class CThreadInterface;


class DLL_EXPORT CThreadManager
{
private:

	CThreadManager();

	~CThreadManager();

	class DLL_EXPORT ThreadData
	{
	public:
		ThreadData(IThreadDataHandler& dataHandler) :mDataHandler(dataHandler)
		{
			LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "ThreadData::ThreadData - copy constructor\n");
		}

		ThreadData(ThreadData&& data) :mDataHandler(data.mDataHandler), mThread(std::move(data.mThread))
		{
			LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "ThreadData::ThreadData - move constructor\n");
		}
		virtual ~ThreadData()
		{
			LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "ThreadData::~ThreadData - destructor\n");
		}
		std::thread mThread;
		IThreadDataHandler& mDataHandler;
	};

	template<typename...ts>
	int startThread(int& threadId, IThreadDataHandler& dataHandler, void(*callback)(IThreadDataHandler::SendDataFunction & sendData,ts...inputData), ts...inputData)
	{
		int id = assign_thread_id;
		threadId = id;
		ThreadData th(dataHandler);
		std::pair<std::map<int, ThreadData>::iterator, bool> res = getThreadList().insert(std::make_pair(id, std::move(th)));

		if (!res.second)
		{
			// If a thread is already available in the map and we try inserting same ID again, this is an issue in ID assignment by pres ctrl core.

			LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CThreadManager::createThread- thread with id=%d already exists\n", id);
			id = -1;
			threadId = -1;
			return id;
		}

		

		auto lambda = [callback,id](ts...data) { if (callback != nullptr) { IThreadDataHandler::SendDataFunction func = std::bind(&PresCtrl::addWorkerThreadData, id, std::placeholders::_1); (*callback)(func,data...); } };

		std::map<int, ThreadData>::iterator itr;

		try
		{
			std::thread startThread(lambda, inputData...);

			itr = getThreadList().find(id);
			if (itr != getThreadList().end())
			{
				itr->second.mThread = std::move(startThread);
#ifndef PLATFORM_WIN32
				pthread_setname_np(itr->second.mThread.native_handle(), "PRESCORE_USRTHR");
#endif
			}			
		}
		catch (std::exception& ex)
		{
			LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "Exception when starting thread ! %s", ex.what());
			id = -1;
			threadId = -1;
			itr = getThreadList().find(id);
			if (itr != getThreadList().end())
			{
				getThreadList().erase(itr);
			}
		}

		assign_thread_id++;
		if (assign_thread_id == INT16_MAX)
		{
			assign_thread_id = 1;
		}

		return id;
	}

	void joinThread(int threadId);

	static std::map<int, ThreadData> mThreadList;

public:

	static void invokeDataHandler(int threadId,BaseData* data = nullptr);

	static int getSize();

	static std::map<int, ThreadData>& getThreadList();

    friend class CThreadInterface;
};

#endif