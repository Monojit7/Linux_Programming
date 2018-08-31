/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            DeviceRemoteCD.h
* @ingroup         mediapresctrl
* @author          Monojit Chatterjee (Monojit.Chatterjee@harman.com)
* @brief           This contains the implementation of RemoteCD device specific functionality.
*/


#ifndef DEVICECD_H
#define DEVICECD_H

#include <device/DeviceBase.h>
#include <browseengine/session/SessionQueryRemoteCDFS.h>






using namespace v1::com::harman::media;
using namespace v3::com::harman::mmpres;

#define DLT_LOGS_UPDATED() \
do { \
    LOG_INFO(LOG_CONTEXT_MEDIA, " %s, %d", __FUNCTION__,   __LINE__); \
}while(0);

class DeviceRemoteCD : public DeviceBase
{
public:
    DeviceRemoteCD(eCDStatus& mCDStatusEvent);
    DeviceRemoteCD(const DeviceRemoteCD& other);
    ~DeviceRemoteCD();
    virtual void resume();
    virtual void pause();
    virtual void stop();
    virtual void next(uint64_t step);
    virtual void previous(uint64_t step);
    virtual void seekToTime(uint64_t pos);
    virtual void setRepeat(MMPresTypes::eRepeatMode mRepeatMode);
    virtual void setShuffle(MMPresTypes::eShuffleMode mShuffleMode);
    virtual void playDevice( const MMPresTypes::eMetadataType objType ,std::string uri );
    virtual MMPresTypes::ePlayState getPlayState();
    
    virtual MMPresTypes::eCallResult fastForward();
    virtual MMPresTypes::eCallResult fastReverse();



    virtual void SessionQueryCreateNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t & mediumID, const MMPresTypes::FilterIDMap & filterIDs, const MMPresTypes::FilterMap & filters, const MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask, const uint64_t& startIndex, const uint64_t& numItems);

    virtual RequestBase* SessionQueryFSNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
    
    virtual RequestBase* SessionQueryNowPlayingNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
    
    
    virtual void SessionQueryCreateExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
    
    
	virtual RequestBase* SessionQueryNowPlayingExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
    
    virtual RequestBase* SessionQueryFSExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
    


    virtual void subscribeToPlayerEvents();
    virtual void unSubscribeToPlayerEvents();
    virtual bool getVideoPlayStatus();
    virtual MMPresTypes::tTrackState getTrackState();
    int getActiveTrackNum();
    
    virtual int32_t getBrowseHistoryEnumeration() { return mBrowseHistoryEnum; }
	virtual void setBrowseHistoryEnumeration(int32_t value) { mBrowseHistoryEnum = value; }
	
	
    void RemoteCDPlayActionStatus ( eCDPlayActions & mCDPlayActions);
    void RemoteCDPlayContextStatus(CDPlayContext mCDPlayContext);
    void RemoteCDActiveTrack( uint64_t mTrackNumber);
    void RemoteCDNumofTracksStatus(uint64_t  numofTracks);
    void RemoteCDTrackTimeStatus(TrackTime mTrackTime);
    void RemoteCDNowPlayingInfo(CDNowPlaying mCDNowPlaying);
    void ShuffleStatus(eCDActionContextValue mCDActionContextValue);
    void RepeatStatus(eCDActionContextValue mCDActionContextValue);
    void dummyNowPlaying();
    FacadeHelper<RemoteCDPlaybackEngine> mRemoteCDPlayBackEngine;
    
    FacadeHelper<MediaPresSettings> mediaSettings;
    //FacadeHelper<MediaPlaybackEngine> mPlayEngine;
     MMPresTypes::tNowPlayingInfo mNowPlayingInfo_RemoteCD;
     MMPresTypes::tTrackState mTrackStateRemoteCD;
     MMPresTypes::ePlayState mPlayState;
    

protected:
    virtual uint32_t getDeviceCaps();
    virtual void onRun();
    void updateAvailabilityToAudioManager(bool available);
    pRemoteCDSlot mpRemoteCDSlot;
    void updateSourceInfoToAudioPres();
    virtual bool getResetMediaIndex() {
        return false;
    }
    virtual void setResetMediaIndex(bool) {};
    virtual void setMasterMediaReset(bool) {};


    inline void setRemoteCDSlots()
    {
        mpRemoteCDSlot.first = MMPresTypes::eDeviceType::DEVICETYPE_REMOTE_CD;

        mpRemoteCDSlot.second = MMPresTypes::eSlotType::SLOTTYPE_REMOTE_CD;
    }
    inline pRemoteCDSlot getRemoteCDSlots() const {
        return mpRemoteCDSlot ;
    };


    void updateDeviceInformation();



private:
    bool subscriptionStatus;
    bool mReadyToPlay;
    int mTrackNumber;
    int32_t mBrowseHistoryEnum;
    uint64_t mTrackIndex = 0 ;
    uint64_t numofTracks = 0 ;
    void CallSessionQuery();
    uint64_t getNumofTracks();
    map < RemoteCDEvents, uint32_t   > mPlaybacklistTokenMap;
   
};
#endif // DEVICECD_H
