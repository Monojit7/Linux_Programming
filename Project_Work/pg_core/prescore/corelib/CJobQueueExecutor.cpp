/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2017
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            CJobQueueExecutor.cpp
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           Can be used to run asynchronous time consuming tasks without blocking the main loop. One JobQueueExecutor creates a single dedicated thread. Jobs can be added to this thread in the form of
*					std::functions.
*                  This is designed for tasks which do not need prolonged execution, but need to be run from a seperate thread context.
*				   If the user uses class member functions as jobs, it is their responsibility to check that it does not go out of scope !!
*/

#include "core/CJobQueueExecutor.hpp"



CJobQueueExecutor::CJobQueueExecutor()
	:mThread(*this)
{

}

CJobQueueExecutor::~CJobQueueExecutor()
{

}

void CJobQueueExecutor::onData(int threadId, BaseData* data)
{
	
}

void CJobQueueExecutor::ThreadFunc(IThreadDataHandler::SendDataFunction & func, CJobQueueExecutor *ptr)
{
	while (true)
	{
		std::function<void()> CurrentJob;
		{
			std::unique_lock<std::mutex> lock(ptr->mQueueMutex);
			ptr->mCond.wait(lock, [ptr]{ return !ptr->mJobList.empty(); });
			CurrentJob = std::move(ptr->mJobList.front());
			ptr->mJobList.pop_front();
		}
		if (CurrentJob)
		{
			CurrentJob();
		}
	}
}




/**
* Starts the executor thread
*/
void CJobQueueExecutor::start()
{
	mThread.template startThread<CJobQueueExecutor*>(mMyThreadID, &ThreadFunc, this);
}

/**
* Cancels all added jobs. And stops the thread. Need to use start before adding jobs again.
*/
void CJobQueueExecutor::stop()
{

}

/**
* Adds a binded  std::function to the thread for execution. It may/may not execute immediately depending on the old jobs which are pending.
* Returns true if job is successfully added for execution.
*/
bool CJobQueueExecutor::addJob(std::function<void()>&& jobFunc)
{
	{
		std::unique_lock<std::mutex> lock(mQueueMutex);
		mJobList.push_back(std::move(jobFunc));
		mCond.notify_one();
	}
	return true; 
}
