/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            SessionQueryFS.h
* @ingroup         mediapresctrl
* @author          sriram srivatsan (sriram.srivatsan@harman.com)
* @brief           This contains the implementation of a file system based session query.This is genrally used for USB MSD devices.
*/

#ifndef _SESSION_QUERY_NOWPLAYING_
#define _SESSION_QUERY_NOWPLAYING_
#include <browseengine/session/SessionQuery.h>
#include <playengine/MediaPlaybackEngine.h>
#include <config/MediaPresSettings.h>
class MediaPlaybackEngine;
class SessionQueryNowPlaying :public SessionQuery
{
public:
	
	// constructor for  creating a new file system query from scratch
	SessionQueryNowPlaying(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex=0,const uint64_t& numItems=0);
	// constructor for  creating a new file system query from cached data.
	SessionQueryNowPlaying(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
	virtual ~SessionQueryNowPlaying();
protected : 
	virtual void onRun();
	virtual bool validateQueryInput();
	virtual bool validateFilterInput();
	virtual bool validateResultType();
	virtual void QueryDataCallback(const uint64_t& offset, const uint64_t& numItems, const CommonAPI::CallStatus& status, const ::v1::org::genivi::mediamanager::MediaTypes::ResultMapList& resultList, const ::v1::org::genivi::mediamanager::PlayerTypes::PlayerError& error);
    virtual void queryDataCallback(const uint64_t& totalItemCount, const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err);
    // this is computing the item data of the current query
	virtual void determineData();
	virtual void determineCount();
	virtual void determineAlphajumpData();

private:
	FacadeHelper<MediaPlaybackEngine> mPlayer;
    FacadeHelper<MediaBrowseEngine> mBrowse;

	void setBrowseHistoryEnumeration();
};

#endif
