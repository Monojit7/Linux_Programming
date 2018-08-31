#include"core/CThreadManager.h"
#include"EventQueueAbstraction.h"
#include"core/logging.h"

std::map<int,CThreadManager::ThreadData> CThreadManager::mThreadList;
CThreadManager* CThreadManager::mSelf = nullptr;

CThreadManager::CThreadManager():
	mThreadId(1)
{
	mSelf = this;
}

CThreadManager::~CThreadManager()
{
}

CThreadManager* CThreadManager::self()
{
	return mSelf;
}

void CThreadManager::joinThread(int threadId)
{
	std::map<int, ThreadData>::iterator itr = mThreadList.find(threadId);

	if (itr != mThreadList.end())
	{
	  // the following element throws an abort error when thread object is already invalid and we try to join it
		if (itr->second.mThread.joinable())
		{
			itr->second.mThread.join();
		}
		mThreadList.erase(itr);
		LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CThreadManager::joinThread - erased thread event id=%d\n", threadId);
	}
}

void CThreadManager::invokeDataHandler(int threadId, BaseData* data)
{
	LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CThreadManager::invokeDataHandler -thread event id=%d\n", threadId);

	std::map<int, ThreadData>::iterator itr = mThreadList.find(threadId);

	if (itr!=mThreadList.end())
	{
		itr->second.mDataHandler.onData(threadId,data);
		delete data; 
	}
}

std::map<int, CThreadManager::ThreadData>& CThreadManager::getThreadList()
{
	return mThreadList;
}

