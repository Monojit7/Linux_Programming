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

#ifndef _SESSION_QUERY_FS_REMOTE_CD
#define _SESSION_QUERY_FS_REMOTE_CD
#include <browseengine/session/SessionQuery.h>
#include <browseengine/session/SessionQueryFS.h>
#include <MediaPresCore.h>

class SessionQueryRemoteCDFS :public SessionQuery, public ITimerEventListener
{
public:
	SessionQueryRemoteCDFS(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
    
	SessionQueryRemoteCDFS(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
	
    virtual ~SessionQueryRemoteCDFS();
    
protected:
    
    virtual void onRun();
	virtual bool validateQueryInput();
	virtual bool validateFilterInput();
	virtual bool validateResultType();
	virtual void determineCount();
	virtual void determineQueryItemList();
	virtual void getRootFolderCount(){};
    void queryDataCallback(Folder mFolderData, bool status);
    void queryDataCountCallback (  short  TotalCount );
    void setItemRequestData();
    short getRequestedFolderNumber();
    FolderRequest getItemRequestData();
    void subscribeBrowseEvent();
    void unsubscribeBrowseEvent();
    FolderRequest mFolderRequest;
    FacadeHelper<RemoteCDPlaybackEngine> mRemoteCDPlayBackEngine;
private:
    bool msubscribestatus;
    CTimerInterface mTimer;
    int timerId;
    bool mRequestTimerExpired;
    int getTrackId(TrackNumber mTrackNumber);
    bool CheckIfWindowCachedCD (  MMPresTypes::MetadataItemList& itemList );
    void startTimer();
    void cancelTimer();
    void timerEvent(int timerid);
    map < RemoteCDEvents, uint32_t   > mBrowselistTokenMap;
};

#endif
