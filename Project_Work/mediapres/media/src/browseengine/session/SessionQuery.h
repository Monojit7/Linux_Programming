/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            SessionQuery.h
* @ingroup         mediapresctrl
* @author          sriram srivatsan (sriram.srivatsan@harman.com)
* @brief           This is the base class which can be used by clients for managing a session query.The different types of session query
                   such as file syytem,metadata etc are derived from this class. 
*/

#ifndef _SESSION_QUERY_
#define _SESSION_QUERY_

#include<core.h>
#include <browseengine/session/SessionManager.h>
#include <browseengine/BrowseHelper.h>
#include <browseengine/MediaBrowseEngine.h>
#include <config/MediaPresSettings.h>
#include <MediaPresControlTypes.hpp>

#define INVALID_ITEM_ID 0

using namespace v3::com::harman::mmpres;
class MediaDetetcionEngine;
class MediaBrowseEngine;
class DeviceBase;

class SessionQuery :public RequestBase
{

public:
	/**
	* Constructor to be used for new session queries. 
	*/
	SessionQuery(const uint32_t& sessionID,const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t &&,MediaBrowserStub::gotoPreviousSessionQueryReply_t&&,MediaBrowserStub::jumpToSessionQueryReply_t&&, const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex=0,const uint64_t& numItems=0);
	/**
	Constructor to create session query from existing query. 
	*/
	SessionQuery(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t &&,MediaBrowserStub::gotoPreviousSessionQueryReply_t&&, MediaBrowserStub::jumpToSessionQueryReply_t&&,const Session::SessionQueryData& queryCacheData,const uint64_t& startIndex=0,const uint64_t& numItems=0);
	~SessionQuery();

	virtual void getAttributeCallback(const std::string browsePath,const CommonAPI::CallStatus &, const MediaTypes::ResultMap&, const BrowserTypes::BrowserError&);
    virtual void listThumbnailsDataCallback(const CommonAPI::CallStatus&,  const std::vector<std::string> &_thumbnails, const ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError&);
    virtual void getAttributesCallbackForSmartSearch(const CommonAPI::CallStatus&, const ::v1::org::genivi::mediamanager::MediaTypes::ResultMap&, const ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError&);
    virtual void getAlphaJumpInfoDataCallback(const CommonAPI::CallStatus&, const ::v1::org::genivi::mediamanager::MediaTypes::arAlphaIndex&, const ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError&);


protected:
	uint32_t mSessionId;
	uint32_t mQueryId;
	uint64_t mMediumId;
	MMPresTypes::FilterIDMap mFilterIDMap;
	MMPresTypes::FilterMap mFilters;
	MMPresTypes::eMetadataType mResultType;
   MMPresTypes::eMetadataType mBrowseResultType;
	uint32_t mMetadataAttributes;
	uint32_t mMetadataCountTypes;
	MMPresTypes::eSessionQueryTypes mQueryType;
	bool mResetPrevious;
	bool mAutoUpdateEnabled;
	uint32_t mContentMask;
	MediaBrowserStub::setSessionQueryReply_t mQueryCallback;
	MediaBrowserStub::gotoPreviousSessionQueryReply_t mGotoPreviousCallback;
	MediaBrowserStub::jumpToSessionQueryReply_t mJumpToQueryCallback;
	const std::shared_ptr<CommonAPI::ClientId> mClient;
	uint64_t mStartIndex;
	uint64_t mNumItems;
	bool mQueryCached;
	bool mUpdateCount;
	uint64_t mContainerId;
	uint32_t mLevelId;
	std::string mBreadCrumbPath;

	FacadeHelper<SessionManager> mManager;
	FacadeHelper<MediaDetectionEngine> mDetect;
	FacadeHelper<MediaBrowseEngine> mBrowser;
    FacadeHelper<MediaPresSettings> mediaSettings;

	DeviceBase* getDevice();
	Session* getSession();
	uint64_t getQueryCount(uint32_t queryId);
	void setCurrentQuery(uint32_t queryId);

	// send the session query count to clients.
	virtual void sendQueryCount(const MMPresTypes::eCallResult& result, const uint64_t&count);

	//send the session query data to clients.
	virtual void sendQueryData(const MMPresTypes::eQueryStatus& status, const MMPresTypes::MetadataItemList& list,const bool& windowContainsCurrentlyPlayingTrack=false,const uint64_t& itemIndexForHighlight=0);
        virtual void sendQueryDataforThumbnail(const MMPresTypes::eQueryStatus& status, const std::vector<std::string> &_thumbnails);
        virtual void sendQueryDataforAlphajump(const MMPresTypes::eQueryStatus& status, const MMPresTypes::arAlphaIndex& ajList);

        virtual void determineBreadCrumbPath();
	virtual bool checkIfWindowContainsCurrentTrack(uint64_t& index);
	virtual void onCancelled();
	virtual uint64_t getQueryCountFromCachedItemList();
	virtual std::string getItemPathFromCachedItemList();
	// check if the query data is cached already
	virtual bool checkIfWindowDataCached(MMPresTypes::MetadataItemList& itemList);
	virtual void determineData();
        virtual void determineAlphajumpData();
	virtual void determineQueryItemList();
	virtual void processAndSendCachedCount();
    void JobCompleted () ;
    void setCurrentBrowsePath ( std::string path ) {   this->path = path ;  }
    std::string getCurrentBrowsePath () { return path; }
    virtual void sendItemIndexTobeHighlighted();
    void cancelTimer (); 
   
     
private:
    map < tCommandName, uint32_t   > requestQueueTokenMap;
    void doPlayStatus ( const bool& doPlayStatus );
   virtual void determineBrowseResultType();
   std::string path;

};
#endif
