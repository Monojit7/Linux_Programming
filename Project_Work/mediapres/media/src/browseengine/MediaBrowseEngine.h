/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            MediaBrowseEngine.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           The heart of the MediaBrowseEngine.
*/

#ifndef _MEDIA_BROWSE_ENGINE_H
#define _MEDIA_BROWSE_ENGINE_H
#include <core.h>
#include <unordered_map>


using namespace v1::org::genivi::mediamanager;
using namespace v1::com::harman::media;

#define ARG_LIST_DISCOVER_MM_ASYNC <CommonAPI::CallStatus, std::vector<std::string>, ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError>
#define ARG_LIST_LISTFUNCS_ASYNC    <CommonAPI::CallStatus ,::v1::org::genivi::mediamanager::MediaTypes::ResultMapList, ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError>
#define ARG_LIST_GETSCH_OBJECT_COUNT <CommonAPI::CallStatus ,uint64_t ,::v1::org::genivi::mediamanager::BrowserTypes::BrowserError>
#define ARG_CREATE_CONT_ARGS <CommonAPI::CallStatus, std::string, ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError>
#define ARG_LIST_SEARCH_OBJECTS_URI <CommonAPI::CallStatus, std::string, ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError>
#define ARG_LIST_GETATTRIBUTES <CommonAPI::CallStatus, ::v1::org::genivi::mediamanager::MediaTypes::ResultMap, ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError>
#define ARG_LIST_GETALPHAJUMPINFO <CommonAPI::CallStatus, ::v1::org::genivi::mediamanager::MediaTypes::arAlphaIndex, ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError>

class MediaBrowseEngine
{
public : 

	void discoverMediaManagers(uint32_t ClientID,BrowserProxyBase::DiscoverMediaManagersAsyncCallback&& callback);
	void listContainers(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, BrowserProxyBase::ListContainersAsyncCallback callback);
	void listContainersEx(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, const std::vector<::v1::org::genivi::mediamanager::BrowserTypes::SortKey> &_sortKey, BrowserProxyBase::ListContainersExAsyncCallback callback);
	void listItems(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, BrowserProxyBase::ListItemsAsyncCallback callback);
	void listItemsEx(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, const std::vector<::v1::org::genivi::mediamanager::BrowserTypes::SortKey> &_sortKey, BrowserProxyBase::ListItemsExAsyncCallback callback);
	void listChildren(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, BrowserProxyBase::ListChildrenAsyncCallback callback);
	void listChildrenEx(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, const std::vector<::v1::org::genivi::mediamanager::BrowserTypes::SortKey> &_sortKey, BrowserProxyBase::ListChildrenExAsyncCallback callback);
	
	void getAttributes(uint32_t ClientID, const std::string &_path, const std::vector<std::string> &_filter,BrowserProxyBase::GetAttributesAsyncCallback callback);
	void getSearchObjectsCount(uint32_t ClientID, const std::string &_path, const std::string &_query, BrowserProxyBase::GetSearchObjectsCountAsyncCallback callback);
    void listThumbnails(uint32_t ClientID, const std::string &_path, const uint64_t &_offset, const uint64_t &_count, const std::string &_query,BrowserProxyBase::ListThumbnailsAsyncCallback _callback);
    void getSearchObjectsUri(uint32_t ClientID, const std::string &_path,BrowserProxyBase::GetSearchObjectsUriAsyncCallback callback);
        void getAlphaJumpInfo(uint32_t ClientID, const std::string &_path, const ::v1::org::genivi::mediamanager::BrowserTypes::SortKey &_sortKey,BrowserProxyBase::GetAlphaJumpInfoAsyncCallback _callback);

	void searchObjects(uint32_t ClientID, const std::string &_path, const std::string &_query, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, BrowserProxyBase::SearchObjectsAsyncCallback callback);
	void searchObjectsEx(uint32_t ClientID, const std::string &_path, const std::string &_query, const uint64_t &_offset, const uint64_t &_count, const std::vector<std::string> &_filter, const std::vector<::v1::org::genivi::mediamanager::BrowserTypes::SortKey> &_sortKey, BrowserProxyBase::SearchObjectsExAsyncCallback callback);
	void listIndexes(uint32_t ClientID, const std::string &_container, const uint64_t &_count, const std::vector<std::string> &_filter, const ::v1::org::genivi::mediamanager::BrowserTypes::SortKey &_sortKey, BrowserProxyBase::ListIndexesAsyncCallback _callback);	
	void setWindowUpdateHandler(std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::eQueryStatus &_status, const uint64_t &_startIndex, const uint64_t &_numItems, const ::v3::com::harman::mmpres::MMPresTypes::MetadataItemList &_itemList, const bool &_windowContainsCurrentlyPlayingTrack, const uint64_t &_itemIndexTobeHighlighted)>&& callback);
	std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::eQueryStatus &_status, const uint64_t &_startIndex, const uint64_t &_numItems, const ::v3::com::harman::mmpres::MMPresTypes::MetadataItemList &_itemList, const bool &_windowContainsCurrentlyPlayingTrack, const uint64_t &_itemIndexTobeHighlighted)>& getWindowUpdateHandler();

    void setWindowUpdateForThumbnailHandler(std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const uint64_t &_startIndex, const uint64_t &_numItems, const std::vector<std::string> &_thumbnaiList)>&& callback);
    std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const uint64_t &_startIndex, const uint64_t &_numItems, const std::vector<std::string> &_thumbnaiList)>& getThumbnailListHandler();

    void setSearchCandidatesStubCallBack(std::function<void(const std::vector<std::string>&)>);
    std::function<void(const std::vector<std::string>&)>& getSearchCandidateStubCallback();

    void setAlphajumpUpdateHandler(std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::arAlphaIndex &_ajList)>&& callback);
    std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::arAlphaIndex &_ajList)>& getAlphajumpListHandler();

    void startProxyTasks();

	void getMostPlayedCount(uint32_t ClientID,const std::vector<uint64_t> &_devices, PlayerExtProxyBase::GetMostPlayedCountAsyncCallback _callback);
	void getMostPlayedList(uint32_t ClientID,const std::vector<uint64_t> &_devices, const uint64_t &_offset, const uint64_t &_count, PlayerExtProxyBase::GetMostPlayedListAsyncCallback _callback);
	void getRecentlyPlayedCount(uint32_t ClientID, const std::vector<uint64_t> &_devices, PlayerExtProxyBase::GetRecentPlayedCountAsyncCallback _callback);
	void getRecentlyPlayedList(uint32_t ClientID,const std::vector<uint64_t> &_devices, const uint64_t &_offset, const uint64_t &_count, PlayerExtProxyBase::GetRecentPlayedListAsyncCallback _callback);
	void reIndexing(uint32_t _id, IndexerProxyBase::ReIndexingAsyncCallback callback);
	void removeDatabse(uint32_t _id);

	void setItemIndexTobeHighlightedStubCallBack(std::function<void(const ::v3::com::harman::mmpres::MMPresTypes::HighlightItemIndexMap&)>);
	std::function<void(const ::v3::com::harman::mmpres::MMPresTypes::HighlightItemIndexMap&)>& getItemIndexTobeHighlightedStubCallBack();

protected : 

	void availibility(const CommonAPI::AvailabilityStatus& avail);
	void computeCategoryCountMap();
	void computeContentInfoMap();
	void discoverMediaManagersListener(const CommonAPI::CallStatus & stat,const std::vector<std::string>& list,const BrowserTypes::BrowserError& err); 
	void listContainersListener(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map,const BrowserTypes::BrowserError& err);
	void listItemsListener(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err);

private : 

	MediaBrowseEngine();
	~MediaBrowseEngine();

	uint32_t registerClient()
	{
		mBrowser.addClient(++mNumClients);
		return mNumClients;
	}
	void deregisterClient(uint32_t ClientID)
	{
		mBrowser.removeClient(ClientID);
	}
	uint32_t mNumClients;

	//void init(); 

	friend class FacadeHelper<MediaBrowseEngine>;

	CommonAPIClientAbstraction<BrowserProxy> mBrowser; 
	CommonAPIClientAbstraction<PlayerExtProxy> mPlayerExtProxy;
	
	std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::eQueryStatus &_status, const uint64_t &_startIndex, const uint64_t &_numItems, const ::v3::com::harman::mmpres::MMPresTypes::MetadataItemList &_itemList, const bool &_windowContainsCurrentlyPlayingTrack, const uint64_t &_itemIndexTobeHighlighted)> mWindowUpdate;
    std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const uint64_t &_startIndex, const uint64_t &_numItems, const std::vector<std::string> &_thumbnaiList)> mWindowThumbnailsUpdate;
    std::function<void(const std::vector<std::string>&)> mSearchCandidatesStubCallback;
    std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::arAlphaIndex &_ajList)> mAlphaJumpUpdate;
    CommonAPI::AvailabilityStatus mBrowserProxyStatus;
    std::function<void(const ::v3::com::harman::mmpres::MMPresTypes::HighlightItemIndexMap&)> mItemIndexTobeHighlightedStubCallBack;
};



#endif // !_MEDIA_BROWSE_ENGINE_H

