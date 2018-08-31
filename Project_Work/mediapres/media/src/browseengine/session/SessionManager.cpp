#include <core.h>
#include <browseengine/session/SessionManager.h>

SessionManager::SessionManager()
:mLastSessionId(0u)
{
	mSessionList.clear();
}

SessionManager::~SessionManager()
{

}

uint32_t SessionManager::openSession()
{
	mLastSessionId++; 
	mSessionList[mLastSessionId] = new Session(mLastSessionId);

	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionManager::openSession-session id=%lu ", mLastSessionId);

	return mLastSessionId;
}

uint32_t SessionManager::registerSessionQuery(uint32_t sessionID, SessionQuery* query)
{
	uint32_t QueryID = 0u; 
	auto it = mSessionList.find(sessionID);
	if (mSessionList.end() != it)
	{
		QueryID = it->second->registerQuery(query);
	}

	return QueryID;
}

bool SessionManager::cancelSession(uint32_t sessionId)
{
	bool bRet = false;
	auto it = mSessionList.find(sessionId);
	if (mSessionList.end() != it)
	{
		/**
		* TBD cancel session. 
		*/
		it->second->clearSessionCache();
		bRet = true;
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionManager::cancelSession-session id=%lu does not exist", sessionId);
		/*
		* TBD. Handle Error for Invalid Session.
		*/
	}

	return bRet;
}

bool SessionManager::closeSession(uint32_t sessionId)
{
	bool bRet = false; 
	auto it = mSessionList.find(sessionId); 
	if (mSessionList.end() != it)
	{
		it->second->clearSessionCache();
		delete it->second;
		mSessionList.erase(it);
		bRet = true; 
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionManager::closeSession-session id=%lu does not exist", sessionId);
		/**
		* TBD. Handle Error for Invalid Session. 
		*/
	}

	return bRet; 
}

Session* SessionManager::getSession(uint32_t sessionId)
{
	Session* sessPtr = nullptr;
	auto it = mSessionList.find(sessionId);
	if (it != mSessionList.end())
	{
		sessPtr=it->second;
	}
	return sessPtr;
}

std::unordered_map <uint32_t, Session*>& SessionManager::getSessionList()
{
	return mSessionList;
}
