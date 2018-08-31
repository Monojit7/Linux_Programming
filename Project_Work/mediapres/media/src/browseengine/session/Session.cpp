#include <core.h>
#include <browseengine/session/Session.h>
#include<browseengine/BrowseHelper.h>

Session::Session(uint32_t sessionID)
	:mCurrentQueryID(0u)
	,mAssignQueryId(0u)
	,mSessionID(sessionID)
	,mCurrentLevelId(0u)
{
	mQueryList.clear();
}

uint32_t Session::registerQuery(SessionQuery* query)
{
/*	++mCurrentQueryID;
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::registerQuery-mAssignQueryID=%lu", mCurrentQueryID);
	return mCurrentQueryID;*/

	++mAssignQueryId;
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::registerQuery-mAssignQueryID=%lu",mAssignQueryId);
	return mAssignQueryId;
}

Session::SessionQueryData* Session::getSessionQueryData(uint32_t queryId)
{
	SessionQueryData* data = nullptr;
	auto it = mQueryDataCache.find(queryId);
	if (it != mQueryDataCache.end())
	{
		data = it->second;
	}

	return data;
}

uint32_t Session::getCurrentQueryId()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::getCurrentQueryId-mCurrentQueryID=%lu", mCurrentQueryID);
	return mCurrentQueryID;
}

void Session::setCurrentQueryId(uint32_t id)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::setCurrentQueryId-set current query id=%lu", id);
	mCurrentQueryID = id;
}

uint32_t Session::getPreviousQueryId(uint32_t id)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::getPreviousQueryId-id=%lu", id);
	printQuerylist();
	uint32_t queryid = 0;
	std::list<uint32_t>::const_iterator itr;
	for (itr = mQueryList.begin(); itr != mQueryList.end(); itr++)
	{
		if (id == *itr)
		{
			break;
		}
		queryid = (*itr);
	}
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::getPreviousQueryId-queryid=%lu", queryid);
	return queryid;
}

bool Session::isJumpValidBasedOnFilter(MMPresTypes::FilterIDMap filterID,uint32_t& id)
{
	bool ret = false;
	uint32_t queryId = getCurrentQueryId();
	if (queryId > 0)
	{
		std::list<uint32_t>::const_iterator itr;
		for (itr = mQueryList.begin(); itr != mQueryList.end(); itr++)
		{
			if (queryId == *itr)
			{
				break;
			}
			Session::SessionQueryData* data = getSessionQueryData(*itr);
			if (data != nullptr)
			{
				if (BrowseHelper::map_compare(filterID, data->mFilterIDMap))
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::isJumpValidBasedOnFilter-queryId=%lu matched", queryId);
					ret = true;
					id = *itr;
					break;
				}
			}
		}
	}
	return ret;
}

void Session::insertToQueryList(uint32_t id)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::insertToQueryList-id=%lu", id);
	if (mQueryList.empty())
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::insertToQueryList-empty list..so push");
		mQueryList.push_back(id);
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::insertToQueryList-non empty list..so insert");
		std::list<uint32_t>::const_iterator itr;
		uint32_t currqueryid = getCurrentQueryId();
		for (itr = mQueryList.begin(); itr != mQueryList.end(); itr++)
		{
			if (currqueryid == *itr)
			{
				itr++;
				break;
			}
		}
		mQueryList.insert(itr, id);
	}
}

void Session::printQuerylist()
{
	std::list<uint32_t>::const_iterator itr;
	for (itr = mQueryList.begin(); itr != mQueryList.end(); itr++)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::printQuerylist - %lu",(*itr));
	}
}

void Session::insertDummyQueryId()
{
	printQuerylist();
	mQueryList.push_back(0);
	printQuerylist();
}

void Session::switchToPlayPlanQuery(std::list<uint32_t> playPlanQueryList)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::switchToPlayPlanQuery-size=%d", playPlanQueryList.size());
	mQueryList.clear();
	mQueryList = playPlanQueryList;
	if (playPlanQueryList.size() > 0)
	{
		setCurrentQueryId(playPlanQueryList.back());
	}
}

void Session::setCurrentLevelId(int32_t id)
{
	mCurrentLevelId = id;
}

int32_t Session::getCurrentLevelId()
{
	return mCurrentLevelId;
}

void Session::clearSessionCache()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::clearSessionCache");
	std::map<uint32_t, Session::SessionQueryData*>::iterator itr;
	for (itr = mQueryDataCache.begin(); itr != mQueryDataCache.end(); itr++)
	{
		if (itr->second != nullptr)
		{
			delete itr->second;
			itr->second = nullptr;
		}
	}
	mQueryDataCache.clear();
	setCurrentQueryId(0);
	mQueryList.clear();
	mPlayPlanDeviceList.clear();
}

void Session::removeQueryFromCache(uint32_t id)
{
	std::map<uint32_t, Session::SessionQueryData*>::iterator itr = mQueryDataCache.find(id);
	if (itr != mQueryDataCache.end())
	{
		if (itr->second != nullptr)
		{
			delete itr->second;
			itr->second = nullptr;
		}
		mQueryDataCache.erase(id);
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::removeQueryFromCache-removed queryId from cache=%lu", id);
		std::list<uint32_t>::const_iterator ctr;
		uint32_t queryid = 0;
		for (ctr = mQueryList.begin(); ctr != mQueryList.end(); ctr++)
		{
			if (id == *ctr)
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::removeQueryFromCache-removed queryId=%lu from querylist", id);
				mQueryList.erase(ctr);
				break;
			}
			queryid = (*ctr);
		}
		if (id == mCurrentQueryID)
		{
			mCurrentQueryID = queryid;
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::removeQueryFromCache-reset current query id to %lu", mCurrentQueryID);
		}
	}
}

void Session::insertNewQueryToCache(uint32_t queryId,Session::SessionQueryData* data)
{
	mQueryDataCache.insert(std::make_pair(queryId,data));
}

bool Session::isJumpValid(uint32_t queryIdToJump)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::isJumpValid");
	bool ret = false;

	std::list<uint32_t>::const_iterator ctr;
	for (ctr = mQueryList.begin(); ctr != mQueryList.end(); ctr++)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::isJumpValid-query id=%lu", *ctr);
		if (queryIdToJump == *ctr)
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Session::isJumpValid-queryIdToJump=%lu can be jumped", queryIdToJump);
			ret = true;
			break;
		}
		if (mCurrentQueryID == *ctr)
		{
			break;
		}
	}

	return ret;
}

std::list<uint32_t> Session::getQueryList()
{
	return mQueryList;
}

void Session::clearQueryList()
{
	mQueryList.clear();
}

void Session::insertToPlayPlanDeviceList(uint64_t deviceId)
{
	std::list<uint64_t>::iterator itr = mPlayPlanDeviceList.begin();
	bool avail = false;
	while (itr != mPlayPlanDeviceList.end())
	{
		if (*itr == deviceId)
		{
			avail = true;
			break;
		}
		itr++;
	}
	if (!avail)
	{
		mPlayPlanDeviceList.push_back(deviceId);
	}
}
void Session::removeFromPlayPlanDeviceList(uint64_t deviceId)
{
	std::list<uint64_t>::iterator itr=mPlayPlanDeviceList.begin();
	while (itr != mPlayPlanDeviceList.end())
	{
		if (*itr == deviceId)
		{
			mPlayPlanDeviceList.erase(itr);
			break;
		}
		itr++;
	}
}

std::list<uint64_t>& Session::getPlayPlanDeviceList()
{
	return mPlayPlanDeviceList;
}
