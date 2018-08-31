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

#ifndef _SESSION_QUERY_RECENTLYPLAYED_
#define _SESSION_QUERY_RECENTLYPLAYED_
#include <browseengine/session/SessionQuery.h>

class SessionQueryRecentlyPlayed :public SessionQuery
{
public:
	
	// constructor for  creating a new file system query from scratch
	SessionQueryRecentlyPlayed(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex=0,const uint64_t& numItems=0);
	
	// constructor for  creating a new file system query from cached data.
	SessionQueryRecentlyPlayed(const uint32_t& sessionID,const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
	virtual ~SessionQueryRecentlyPlayed();

protected : 
	virtual void onRun();
	virtual bool validateQueryInput();
	virtual bool validateFilterInput();
	virtual bool validateResultType();
	virtual void determineCount();
	virtual void determineQueryItemList();
	// this is a callback for the browse request to mediaone.This is used for computing the count(currently done only for computing item count of the root folder)
    virtual void queryCountCallback(const CommonAPI::CallStatus & stat, const uint64_t& count, const ::v1::com::harman::media::BaseType::tBaseError& err);
	// this is a callback for the browse request to mediaone.This is used for computing the query data
	virtual void queryDataCallback(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const ::v1::com::harman::media::BaseType::tBaseError& err);

private:
};

#endif