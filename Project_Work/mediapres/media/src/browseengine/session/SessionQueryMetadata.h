
/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            SessionQueryMetadata.h
* @ingroup         mediapresctrl
* @author          sriram srivatsan (sriram.srivatsan@harman.com)
* @brief           This contains the implementation of a metadata based based session query..
*/

#ifndef _SESSION_QUERY_METADATA_
#define _SESSION_QUERY_METADATA_
#include <browseengine/session/SessionQuery.h>
#include <browseengine/BrowseHelper.h>

class SessionQueryMetadata :public SessionQuery
{
public:
	SessionQueryMetadata(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex=0,const uint64_t& numItems=0);
	SessionQueryMetadata(const uint32_t& sessionID,const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
	virtual ~SessionQueryMetadata();

protected : 
	virtual void onRun();
	virtual bool validateQueryInput();
	virtual bool validateFilterInput();
	virtual bool validateResultType();
	virtual void determineCount();
	virtual void determineRootLevelCategoryCount();
	virtual void categoryCountCallback(const std::string& parentPath, const std::string& categoryName, const std::string& mediaType,const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err);
	virtual void queryDataCallback(const uint64_t& totalItemCount, const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err);
	virtual void determineQueryItemList();
	virtual void determineCountViaGetAttribute();
 	std::string searchPath;
	virtual void determineCountForSearchQuery();
	virtual void searchQueryCountcallback(const CommonAPI::CallStatus&,const uint64_t&,const ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError&);
    virtual void searchQueryDataCallback(const CommonAPI::CallStatus&, const ::v1::org::genivi::mediamanager::MediaTypes::ResultMapList&, const ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError&);
    

private:
    bool mIsSmartSearch;
};

#endif
