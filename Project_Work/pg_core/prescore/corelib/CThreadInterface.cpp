#include <core/CThreadInterface.h>


CThreadInterface::CThreadInterface(IThreadDataHandler &dataHandler) :mDataHandler(dataHandler)
{
}

CThreadInterface::~CThreadInterface()
{
	for (std::list<int>::iterator itr = mThreadIdList.begin(); itr != mThreadIdList.end(); itr++)
	{
		CThreadManager::self()->joinThread(*itr);
		LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "CThreadInterface::~CThreadInterface- joined thread id=%d\n", *itr);
	}

	mThreadIdList.clear();
}

void CThreadInterface::joinThread(int threadId)
{
	CThreadManager::self()->joinThread(threadId);

	for (std::list<int>::iterator itr = mThreadIdList.begin(); mThreadIdList.size()>0 && itr != mThreadIdList.end(); itr++)
	{
		if (threadId == (*itr))
		{
			mThreadIdList.erase(itr);
			break;
		}
	}
}

