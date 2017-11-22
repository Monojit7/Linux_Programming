/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2017
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            CJobQueueExecutor.hpp
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           Can be used to run asynchronous time consuming tasks without blocking the main loop. One JobQueueExecutor creates a single dedicated thread. Jobs can be added to this thread in the form of
*					std::functions. 
*                  This is designed for tasks which do not need prolonged execution, but need to be run from a seperate thread context. 
*				   If the user uses class member functions as jobs, it is their responsibility to check that it does not go out of scope !!
*/

#ifndef _CJOBQUEUEEXECUTOR_
#define _CJOBQUEUEEXECUTOR_

#include <core/PresCtrlTypes.h>
#include <functional>
#include <list>
#include <mutex>
#include <condition_variable>
#include "CThreadInterface.h"

class DLL_EXPORT CJobQueueExecutor :  public IThreadDataHandler
{
public : 
	CJobQueueExecutor(); 
	~CJobQueueExecutor();

	/**
	* Starts the executor thread
	*/
	void start(); 

	/**
	* Cancels all added jobs. And stops the thread. Need to use start before adding jobs again. 
	*/
	void stop(); 

	/**
	* Adds a binded  std::function to the thread for execution. It may/may not execute immediately depending on the old jobs which are pending. 
	* Returns true if job is successfully added for execution. 
	*/
	bool addJob(std::function<void()>&& jobFunc);


protected : 

	virtual void onData(int threadId, BaseData* data = nullptr); 

private : 
	CJobQueueExecutor(const CJobQueueExecutor& );
	CJobQueueExecutor& operator=(const CJobQueueExecutor&);

	enum JobEngineState
	{
		JOB_ENGINE_IDLE=0u,
		JOB_ENGINE_RUNNING,
		JOB_ENGINE_COMPLETED
	};

	static void ThreadFunc(IThreadDataHandler::SendDataFunction & func, CJobQueueExecutor *ptr);


	std::function<void()> mCurrentJob; 

	std::mutex mQueueMutex;
	std::condition_variable mCond;
	int mMyThreadID; 

	std::list<std::function<void()>> mJobList; 
	CThreadInterface mThread;
	JobEngineState   mEngineState; 
};

#endif