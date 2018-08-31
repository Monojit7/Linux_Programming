/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            SessionQueryFS.h
* @ingroup         mediapresctrl
* @author          Monojit Chatterjee (Monojit.Chatterjee@harman.com)
* @brief           This contains the implementation of a Now playing based session query.This is genrally used for USB MSD devices.
*/


#ifndef _SESSION_QUERY_NOWPLAYING_REMOTE_CD
#define _SESSION_QUERY_NOWPLAYING_REMOTE_CD
#include<core.h>
#include <browseengine/session/SessionQuery.h>
#include <browseengine/session/SessionQueryFS.h>
#include <MediaPresCore.h>

class MediaPlaybackEngine;
class SessionQueryRemoteCDNowPlaying :public SessionQuery, public ITimerEventListener
{
public:

    // constructor for  creating a new file system query from scratch
    SessionQueryRemoteCDNowPlaying(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex=0,const uint64_t& numItems=0);
    // constructor for  creating a new file system query from cached data.
    SessionQueryRemoteCDNowPlaying(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
    void queryDataCallback(Folder mFolderData, bool status);
    void queryDataCountCallback (  short  TotalCount );
    void startTimer();
    void cancelTimer();
    virtual ~SessionQueryRemoteCDNowPlaying();
protected :
    virtual void onRun();
    virtual bool validateQueryInput();
    virtual bool validateFilterInput();
    virtual bool validateResultType();
    virtual void determineCount();

    // this is computing the item data of the current query
    //virtual void determineData();
    
    void setItemRequestData();
    short getRequestedFolderNumber();
    FolderRequest getItemRequestData();
    void subscribeBrowseEvent();
    void unsubscribeBrowseEvent();
    virtual void determineQueryItemList();
    FolderRequest mFolderRequest;
    FacadeHelper<RemoteCDPlaybackEngine> mRemoteCDPlayBackEngine;
private:
    bool msubscribestatus;
    int getTrackId(TrackNumber mTrackNumber);

private:
    FacadeHelper<MediaPlaybackEngine> mPlayer;
    FacadeHelper<MediaBrowseEngine> mBrowserRemoteCD;
    FacadeHelper<SessionManager> mManagerRemoteCD;
    void setBrowseHistoryEnumeration ( );
    bool CheckIfWindowCachedCD ( MMPresTypes::MetadataItemList& itemList );

    void timerEvent(int timerid);
    map < RemoteCDEvents, uint32_t   > mBrowselistTokenMap;
    CTimerInterface mTimer;
    int timerId;
    bool mRequestTimerExpired;

};

#endif
