#include "MediaBrowseEngine.h"
#include <core/logging.h>
#include<detectionengine/MediaDetectionEngine.h>
#include<device/DeviceBase.h>
#include<MediaPresCore.h>
#include <config/MediaPresSettings.h>

MediaBrowseEngine::MediaBrowseEngine()
	:mNumClients(0u)
	,mBrowser("local","inst0")
	,mPlayerExtProxy("local","inst0")
	,mBrowserProxyStatus(CommonAPI::AvailabilityStatus::UNKNOWN)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowseEngine::MediaBrowseEngine+");
	SUBSCRIBE(mBrowser, getProxyStatusEvent(), &MediaBrowseEngine::availibility, std::placeholders::_1);
	mWindowUpdate = nullptr;
	mSearchCandidatesStubCallback = nullptr;
    mAlphaJumpUpdate = nullptr;
    mItemIndexTobeHighlightedStubCallBack = nullptr;
}

MediaBrowseEngine::~MediaBrowseEngine()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowseEngine::~MediaBrowseEngine-");
	mSearchCandidatesStubCallback = nullptr;
    mAlphaJumpUpdate = nullptr;
}

void MediaBrowseEngine::discoverMediaManagers(uint32_t ClientID, BrowserProxyBase::DiscoverMediaManagersAsyncCallback&& callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_DISCOVER_MM_ASYNC, ClientID, callback, discoverMediaManagersAsync)
	}
}


void MediaBrowseEngine::listContainers(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, BrowserProxyBase::ListContainersAsyncCallback callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_LISTFUNCS_ASYNC, ClientID, callback, listContainersAsync, _path, _offset, _count, _filter, )
	}
}
void MediaBrowseEngine::listContainersEx(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, const std::vector<::v1::org::genivi::mediamanager::BrowserTypes::SortKey> &_sortKey, BrowserProxyBase::ListContainersExAsyncCallback callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_LISTFUNCS_ASYNC, ClientID, callback, listContainersExAsync, _path, _offset, _count, _filter, _sortKey, )
	}
}
void MediaBrowseEngine::listItems(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, BrowserProxyBase::ListItemsAsyncCallback callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_LISTFUNCS_ASYNC, ClientID, callback, listItemsAsync, _path, _offset, _count, _filter, )
	}
}
void MediaBrowseEngine::listItemsEx(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, const std::vector<::v1::org::genivi::mediamanager::BrowserTypes::SortKey> &_sortKey, BrowserProxyBase::ListItemsExAsyncCallback callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_LISTFUNCS_ASYNC, ClientID, callback, listItemsExAsync, _path, _offset, _count, _filter, _sortKey, )
	}
}
void MediaBrowseEngine::listChildren(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, BrowserProxyBase::ListChildrenAsyncCallback callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_LISTFUNCS_ASYNC, ClientID, callback, listChildrenAsync, _path, _offset, _count, _filter, )
	}
}
void MediaBrowseEngine::listChildrenEx(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, const std::vector<::v1::org::genivi::mediamanager::BrowserTypes::SortKey> &_sortKey, BrowserProxyBase::ListChildrenExAsyncCallback callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_LISTFUNCS_ASYNC, ClientID, callback, listChildrenExAsync, _path, _offset, _count, _filter, _sortKey, )
	}
}

void MediaBrowseEngine::getSearchObjectsCount(uint32_t ClientID, const std::string &_path, const std::string &_query, BrowserProxyBase::GetSearchObjectsCountAsyncCallback callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_GETSCH_OBJECT_COUNT, ClientID, callback, getSearchObjectsCountAsync, _path, _query, )
	}
}

void MediaBrowseEngine::listThumbnails(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::string &_query,BrowserProxyBase::ListThumbnailsAsyncCallback _callback)
{
    if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
    {
        ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_DISCOVER_MM_ASYNC, ClientID, _callback,listThumbnailsAsync,_path, _offset,_count,_query, )
    }
}

void MediaBrowseEngine::getSearchObjectsUri(uint32_t ClientID, const std::string &_path, BrowserProxyBase::GetSearchObjectsUriAsyncCallback callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_SEARCH_OBJECTS_URI, ClientID, callback, getSearchObjectsUriAsync, _path, )
	}
}

void MediaBrowseEngine::searchObjects(uint32_t ClientID, const std::string &_path, const std::string &_query, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, BrowserProxyBase::SearchObjectsAsyncCallback callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_LISTFUNCS_ASYNC, ClientID, callback, searchObjectsAsync, _path, _query, _offset, _count, _filter, )
	}
}
void MediaBrowseEngine::searchObjectsEx(uint32_t ClientID, const std::string &_path, const std::string &_query, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, const std::vector<::v1::org::genivi::mediamanager::BrowserTypes::SortKey> &_sortKey, BrowserProxyBase::SearchObjectsExAsyncCallback callback)
{
	ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_LISTFUNCS_ASYNC, ClientID, callback, searchObjectsExAsync, _path, _query, _offset, _count, _filter,_sortKey,)
}
void MediaBrowseEngine::listIndexes(uint32_t ClientID, const std::string &_container, const uint64_t &_count, const std::vector<std::string> &_filter, const ::v1::org::genivi::mediamanager::BrowserTypes::SortKey &_sortKey, BrowserProxyBase::ListIndexesAsyncCallback _callback)
{

}

void MediaBrowseEngine::getAttributes(uint32_t ClientID, const std::string &_path, const std::vector<std::string> &_filter, BrowserProxyBase::GetAttributesAsyncCallback callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_GETATTRIBUTES, ClientID, callback, getAttributesAsync, _path, _filter, )
	}
}

void MediaBrowseEngine::getAlphaJumpInfo(uint32_t ClientID, const std::string &_path, const ::v1::org::genivi::mediamanager::BrowserTypes::SortKey &_sortKey, BrowserProxyBase::GetAlphaJumpInfoAsyncCallback _callback)
{
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		ASYNC_INVOKE_3ARG(mBrowser, BrowserProxy, ARG_LIST_GETALPHAJUMPINFO, ClientID, _callback, getAlphaJumpInfoAsync, _path, _sortKey,)
	}
}

void MediaBrowseEngine::availibility(const CommonAPI::AvailabilityStatus& avail)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowseEngine::availability-BrowserProxyAvail : %s", mBrowser.getStringAvailibility(avail).c_str()); 

	mBrowserProxyStatus = avail;
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
	//	startProxyTasks();
		FacadeHelper<MediaPresCore>()->setBrowserAvailable();
	}
}

void MediaBrowseEngine::startProxyTasks()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaBrowseEngine::startProxyTasks");
	discoverMediaManagers(1, std::bind(&MediaBrowseEngine::discoverMediaManagersListener, this, _1, _2, _3));
	//computeCategoryCountMap();
	computeContentInfoMap();
}

void MediaBrowseEngine::computeCategoryCountMap()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowseEngine::computeCategoryCountMap");
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		FacadeHelper<MediaDetectionEngine> mDetect;
		std::unordered_map<uint64_t, DeviceBase*> deviceMap = mDetect->getDeviceList();
		for (auto itr = deviceMap.begin(); itr != deviceMap.end(); itr++)
		{
			DeviceBase* pDevice = itr->second;
			if (pDevice != nullptr)
			{
				pDevice->getCategoryCountMap();
			}
		}
	}
}

void MediaBrowseEngine::computeContentInfoMap()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowseEngine::computeContentInfoMap");
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		FacadeHelper<MediaDetectionEngine> mDetect;
		std::unordered_map<uint64_t, DeviceBase*> deviceMap = mDetect->getDeviceList();
		for (auto itr = deviceMap.begin(); itr != deviceMap.end(); itr++)
		{
			DeviceBase* pDevice = itr->second;
			if (pDevice != nullptr)
			{
				pDevice->getContentInfoMap();
			}
		}
	}
}

void MediaBrowseEngine::discoverMediaManagersListener(const CommonAPI::CallStatus & stat, const std::vector<std::string>& list, const BrowserTypes::BrowserError& err)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DiscoverMM : %s : Browse Error %d", mBrowser.getStringCallStatus(stat).c_str() , err );
	for (size_t i = 0; i < list.size(); i++)
	{
        LOG_INFO(LOG_CONTEXT_MEDIA, "List  %s : ", list[i].c_str());
	}
	//std::vector<std::string> filter = { "*" };
	//mBrowser->listContainersAsync(list[0], 0, 0, filter, std::bind(&MediaBrowseEngine::listContainersListener,this, _1, _2,_3));
}

void MediaBrowseEngine::listContainersListener(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
	
	uint16_t numContainersFound = map.size(); 
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< ListContainers : Status : %s  Num Containers : %d Browse Error %d", mBrowser.getStringCallStatus(stat).c_str(),numContainersFound, (int)err);
	std::string DisplayName; 
	std::string Path;
	std::string Parent;
	std::string Type;
	std::string TypeEx;
	bool Searchable = false; 
	
	for (uint16_t i = 0; i < 1; i++)
	{
		const MediaTypes::ResultMap& result = map[i]; 

		auto it = result.find("DisplayName");
		DisplayName = it->second.get<std::string>();
		it = result.find("Path"); 
		Path = it->second.get<std::string>();
		it = result.find("Parent");
		Parent = it->second.get<std::string>();
		it = result.find("Type");
		Type = it->second.get<std::string>();
		it = result.find("TypeEx");
		TypeEx = it->second.get<std::string>();
	//	it = result.find("Searchable");
	//	Searchable = it->second.get<bool>();

        LOG_INFO(LOG_CONTEXT_MEDIA, "ContainerData : DispName  %s , Path %s , Parent : %s , Type %s , TypeEx : %s , Searchable  : %d  : ", \
			DisplayName.c_str(),Path.c_str(),Parent.c_str(),Type.c_str(),TypeEx.c_str(),Searchable
			);
		std::vector<std::string> filter = { "*" };
		mBrowser->listChildrenAsync(Path, 0, 0, filter, std::bind(&MediaBrowseEngine::listItemsListener, this, _1, _2, _3));
	}
}

void MediaBrowseEngine::listItemsListener(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
	uint16_t numContainersFound = map.size();
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< ListItems : Status : %s  Num Containers : %d Browse Error %d", mBrowser.getStringCallStatus(stat).c_str(), numContainersFound, (int)err);
	std::string DisplayName;
	std::string Path;
	std::string Parent;
	std::string Type;
	std::string TypeEx;
	bool Searchable = false;

	for (uint16_t i = 0; i < numContainersFound; i++)
	{
		const MediaTypes::ResultMap& result = map[i];

		auto it = result.find("DisplayName");
		DisplayName = it->second.get<std::string>();
		it = result.find("Path");
		Path = it->second.get<std::string>();
		it = result.find("Parent");
		Parent = it->second.get<std::string>();
		it = result.find("Type");
		Type = it->second.get<std::string>();
		it = result.find("TypeEx");
		TypeEx = it->second.get<std::string>();
		//	it = result.find("Searchable");
		//	Searchable = it->second.get<bool>();

        LOG_INFO(LOG_CONTEXT_MEDIA, "ContainerData : DispName  %s , Path %s , Parent : %s , Type %s , TypeEx : %s , Searchable  : %d  : ", \
			DisplayName.c_str(), Path.c_str(), Parent.c_str(), Type.c_str(), TypeEx.c_str(), Searchable
			);
		std::vector<std::string> filter = { "*" };
		mBrowser->listChildrenAsync(Path, 0, 0, filter, std::bind(&MediaBrowseEngine::listItemsListener, this, _1, _2, _3));
	}
}

void MediaBrowseEngine::setWindowUpdateHandler(std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::eQueryStatus &_status, const uint64_t &_startIndex, const uint64_t &_numItems, const ::v3::com::harman::mmpres::MMPresTypes::MetadataItemList &_itemList, const bool &_windowContainsCurrentlyPlayingTrack, const uint64_t &_itemIndexTobeHighlighted)>&& callback)
{
	mWindowUpdate = std::move(callback);
}

std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::eQueryStatus &_status, const uint64_t &_startIndex, const uint64_t &_numItems, const ::v3::com::harman::mmpres::MMPresTypes::MetadataItemList &_itemList, const bool &_windowContainsCurrentlyPlayingTrack, const uint64_t &_itemIndexTobeHighlighted)>& MediaBrowseEngine::getWindowUpdateHandler()
{
	return mWindowUpdate;
}

void MediaBrowseEngine::setWindowUpdateForThumbnailHandler(std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const uint64_t &_startIndex, const uint64_t &_numItems, const std::vector<std::string> &_thumbnaiList)>&& callback)
{
    mWindowThumbnailsUpdate = std::move(callback);
}

std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const uint64_t &_startIndex, const uint64_t &_numItems, const std::vector<std::string> &_thumbnaiList)>& MediaBrowseEngine::getThumbnailListHandler()
{
    return mWindowThumbnailsUpdate;
}

void MediaBrowseEngine::setAlphajumpUpdateHandler(std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::arAlphaIndex &_ajList)>&& callback)
{
    mAlphaJumpUpdate = std::move(callback);
}

std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::arAlphaIndex &_ajList)>& MediaBrowseEngine::getAlphajumpListHandler()
{
    return mAlphaJumpUpdate;
}

void MediaBrowseEngine::getMostPlayedCount(uint32_t ClientID, const std::vector<uint64_t> &_devices, PlayerExtProxyBase::GetMostPlayedCountAsyncCallback _callback)
{
	mPlayerExtProxy->getMostPlayedCountAsync(_devices,_callback);
}

void MediaBrowseEngine::getMostPlayedList(uint32_t ClientID, const std::vector<uint64_t> &_devices, const uint64_t &_offset, const uint64_t &_count, PlayerExtProxyBase::GetMostPlayedListAsyncCallback _callback)
{
	mPlayerExtProxy->getMostPlayedListAsync(_devices, _offset, _count, _callback);
}

void MediaBrowseEngine::getRecentlyPlayedCount(uint32_t ClientID, const std::vector<uint64_t> &_devices, PlayerExtProxyBase::GetRecentPlayedCountAsyncCallback _callback)
{
	mPlayerExtProxy->getRecentPlayedCountAsync(_devices, _callback);
}

void MediaBrowseEngine::getRecentlyPlayedList(uint32_t ClientID, const std::vector<uint64_t> &_devices, const uint64_t &_offset, const uint64_t &_count, PlayerExtProxyBase::GetRecentPlayedListAsyncCallback _callback)
{
	mPlayerExtProxy->getRecentPlayedListAsync(_devices,_offset,_count,_callback);
}

void MediaBrowseEngine::reIndexing(uint32_t _id, IndexerProxyBase::ReIndexingAsyncCallback callback)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowseEngine::reIndexing");
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowseEngine::reIndexing- _id = %d",_id);
    FacadeHelper<MediaPresSettings> mSettings;

	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
       /* call to reIndexingAsync of CoC media */
        mSettings->reIndexing(_id, callback);
	}
}

void MediaBrowseEngine::removeDatabse(uint32_t _id)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowseEngine::removeDatabse- _id = %d",_id);
    FacadeHelper<MediaPresSettings> mSettings;
	if (mBrowserProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
       /* call to removeDatabaseAsync of CoC media */
       mSettings->removeDatabse(_id);
	}
}

void MediaBrowseEngine::setSearchCandidatesStubCallBack(std::function<void(const std::vector<std::string>&)> func)
{
   mSearchCandidatesStubCallback = func;
}

std::function<void(const std::vector<std::string>&)>& MediaBrowseEngine::getSearchCandidateStubCallback()
{
   return mSearchCandidatesStubCallback;
}

void MediaBrowseEngine::setItemIndexTobeHighlightedStubCallBack(std::function<void(const MMPresTypes::HighlightItemIndexMap&)>  func)
{
	mItemIndexTobeHighlightedStubCallBack = func;
}

std::function<void(const MMPresTypes::HighlightItemIndexMap&)>& MediaBrowseEngine::getItemIndexTobeHighlightedStubCallBack()
{
	return mItemIndexTobeHighlightedStubCallBack;
}
