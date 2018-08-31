/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            DeviceRemoteCD.cpp
* @ingroup         mediapresctrl
* @author          Monojit Chatterjee (Monojit.Chatterjee@harman.com)
* @brief           This contains the implementation of RemoteCD device specific functionality.
*/
 

#include "DeviceRemoteCD.h"
#include <browseengine/session/SessionQueryFS.h>
#include <browseengine/session/SessionQueryMetadata.h>
#include <browseengine/session/SessionQueryNowPlaying.h>
#include <browseengine/session/SessionQueryHighestRated.h>
#include <browseengine/session/SessionQueryRecentlyPlayed.h>
#include <browseengine/session/SessionQueryCategory.h>
#include <browseengine/session/SessionQueryRemoteCDNowPlaying.h>



/*std::unordered_map<eCDActionContextValue, MMPresTypes::eShuffleMode> mpresShuffleContextMap = {
    {eCDActionContextValue::eOn, MMPresTypes::eShuffleMode::Literal::SHUFFLE_ALL},
    {eCDActionContextValue::eOff,MMPresTypes::eShuffleMode::Literal::SHUFFLE_OFF},
    {eCDActionContextValue::eOnFolder, MMPresTypes::eShuffleMode::Literal::SHUFFLE_FOLDER}
};

std::unordered_map<eCDActionContextValue, MMPresTypes::eRepeatMode> mpresRepeatContextMap = {
    {eCDActionContextValue::eOn, MMPresTypes::eRepeatMode::Literal::REPEAT_ALL},
    { eCDActionContextValue::eOff, MMPresTypes::eRepeatMode::Literal::REPEAT_OFF},
    { eCDActionContextValue::eOnFolder, MMPresTypes::eRepeatMode::Literal::REPEAT_FOLDER}
};*/


DeviceRemoteCD::DeviceRemoteCD(eCDStatus& mCDStatusEvent): mPlayState(MMPresTypes::ePlayState::Literal::PLAYSTATE_UNKNOWN),
    subscriptionStatus( false ),
    mReadyToPlay(false),
    mTrackNumber(-1),
    mBrowseHistoryEnum ( 0 ),
    mTrackIndex(0),
    numofTracks(0)
{
   
    
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceRemoteCD constructor:CDStatus");

}

DeviceRemoteCD::DeviceRemoteCD(const DeviceRemoteCD& other)
{

    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceRemoteCD constructor:default");

}

DeviceRemoteCD::~DeviceRemoteCD()
{

 if (mSettings->getBrowseHistoryEnumeration())
   {
      clearPlayPlan();
   }
  if (mSettings->getAMInteraction())
   {
      updateAvailabilityToAudioManager(false);
      if (mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this)
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::~DeviceBase: raise stopped event upon active device disconnection");
      //	mSourceSwitch->getSCI_Media()->raise_onStopped();
      //	mSourceSwitch->runCycle();
         mSourceSwitch->cancelTimer();
         //Note:Do not unsubcribe here as there are some derived class virtual funcs. They will be invalid by the time base detsruction happens.
         //unSubscribeToPlayerEvents();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::~DeviceBase: deactivating active device");
         mDetect->setActiveDevice(0);
      }
   }
    
}


void DeviceRemoteCD::subscribeToPlayerEvents()
{

  if ( subscriptionStatus == false )
    {
          LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::subscribeToPlayerEvents");

          mPlaybacklistTokenMap [EVENT_PLAYCONTROL_ACTION_STATUS] =  PresEventAbstraction::getInstance()->subscribe<playActionStatus>(EVENT_PLAYCONTROL_ACTION_STATUS, std::bind(&DeviceRemoteCD::RemoteCDPlayActionStatus, this,_1));

          mPlaybacklistTokenMap [EVENT_PLAYCONTROL_CONTEXT_STATUS] = PresEventAbstraction::getInstance()->subscribe<playContextStatus>(EVENT_PLAYCONTROL_CONTEXT_STATUS, std::bind(&DeviceRemoteCD::RemoteCDPlayContextStatus, this,_1));

          mPlaybacklistTokenMap [ EVENT_PLAYCONTROL_NOWPLAYING_STATUS ] =   PresEventAbstraction::getInstance()->subscribe<cdNowPlayingInfo>(EVENT_PLAYCONTROL_NOWPLAYING_STATUS, std::bind(&DeviceRemoteCD::RemoteCDNowPlayingInfo, this,_1));

          mPlaybacklistTokenMap [ EVENT_PLAYCONTROL_ACTIVETRACK_STATUS ] = PresEventAbstraction::getInstance()->subscribe<activeTrack>(EVENT_PLAYCONTROL_ACTIVETRACK_STATUS, std::bind(&DeviceRemoteCD::RemoteCDActiveTrack, this,_1));

          mPlaybacklistTokenMap [ EVENT_PLAYCONTROL_NUMBER_TRACKS_STATUS ] = PresEventAbstraction::getInstance()->subscribe<numberOfTracksStatus>(EVENT_PLAYCONTROL_NUMBER_TRACKS_STATUS, std::bind(&DeviceRemoteCD::RemoteCDNumofTracksStatus, this,_1));

          mPlaybacklistTokenMap [ EVENT_PLAYCONTROL_TRACKTIME_STATUS ] = PresEventAbstraction::getInstance()->subscribe<playtrackTime>(EVENT_PLAYCONTROL_TRACKTIME_STATUS, std::bind(&DeviceRemoteCD::RemoteCDTrackTimeStatus, this,_1));

    

    //  PresEventAbstraction::getInstance()->subscribe<playtrackTime>(EVENT_PLAYCONTROL_TRACKTIME_STATUS, std::bind(&DeviceRemoteCD::RemoteCDTrackTimeStatus, this,_1));

         subscriptionStatus = true ;
    
    }
}


void DeviceRemoteCD::updateDeviceInformation()
{
    this->setRemoteCDSlots();
    mDetect->deviceInformationUpdateRemoteCD( (int)REMOTE_CD_DEVICE_ID, getRemoteCDSlots() , (int)getDeviceCaps(), (string)"REMOTE_CD" );

}

void DeviceRemoteCD::unSubscribeToPlayerEvents()
{
    if(subscriptionStatus)
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::unSubscribeToPlayerEvents");
        
        PRESEVENT_UNSUBSCRIBE( EVENT_PLAYCONTROL_ACTION_STATUS,           mPlaybacklistTokenMap [ EVENT_PLAYCONTROL_ACTION_STATUS ] );
        
        PRESEVENT_UNSUBSCRIBE( EVENT_PLAYCONTROL_CONTEXT_STATUS,          mPlaybacklistTokenMap [ EVENT_PLAYCONTROL_CONTEXT_STATUS ] );
        
        PRESEVENT_UNSUBSCRIBE( EVENT_PLAYCONTROL_NOWPLAYING_STATUS,       mPlaybacklistTokenMap [ EVENT_PLAYCONTROL_NOWPLAYING_STATUS ] );
        
        PRESEVENT_UNSUBSCRIBE( EVENT_PLAYCONTROL_ACTIVETRACK_STATUS,      mPlaybacklistTokenMap [ EVENT_PLAYCONTROL_ACTIVETRACK_STATUS ] );
        
        PRESEVENT_UNSUBSCRIBE( EVENT_PLAYCONTROL_NUMBER_TRACKS_STATUS,    mPlaybacklistTokenMap [ EVENT_PLAYCONTROL_NUMBER_TRACKS_STATUS ] );
        
        PRESEVENT_UNSUBSCRIBE( EVENT_PLAYCONTROL_TRACKTIME_STATUS,        mPlaybacklistTokenMap [ EVENT_PLAYCONTROL_TRACKTIME_STATUS ] );
        
        // this->updateAvailabilityToAudioManager(false);
        
        subscriptionStatus = false;
        //unsubscribe the event here
    }
}


void DeviceRemoteCD::onRun()
{

    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::On Run");


    if (!mReadyToPlay)
    {


        this->dummyNowPlaying();
        //	mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(), getDeviceType(), mMediumInfo.getMMediumName(), getDeviceCaps());

        if ((mSettings->getAMInteraction()))
        {
            this->updateDeviceInformation();
            this->updateSourceInfoToAudioPres();
            this->updateAvailabilityToAudioManager(true);
            // currently we assume only 1 USB device is upported.....if we want to support multiple USB,then make AMInteraction as true.
        }
        mReadyToPlay = true;

    }

    setIdle();
}

void DeviceRemoteCD::updateSourceInfoToAudioPres()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD :updateSourceInfoToAudioPres:");
    if (mSettings->getAMInteraction())
    {
        mAudioInterface->updateAudioSourceInfotoAudioPres(REMOTE_CD_DEVICE_ID);
    }
}


MMPresTypes::ePlayState DeviceRemoteCD::getPlayState()
{

    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD :getPlayState: %d", (int)this->mPlayState);
    return  this->mPlayState;
}

void DeviceRemoteCD::resume()
{
    DLT_LOGS_UPDATED();

    if ( mPlayState != MMPresTypes::ePlayState::Literal::PLAYSTATE_PLAYING )
    {
        mRemoteCDPlayBackEngine->resumeRemoteCD();
    }
    else
    {
        if ( ( mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this ) && ( mDetect-> getActiveDeviceId () == REMOTECD_DEVICE_ID ))
       {
           std::function<void(const MMPresTypes::ePlayState&)> callback = mPlayEngine->getPlayStateStubCallback();
           if (callback != nullptr)
            {
                LOG_INFO(LOG_CONTEXT_MEDIA, " %s, %d sending playstate <: %d>", __FUNCTION__, __LINE__, mPlayState );
                callback(mPlayState);
            }
       }
        
    }
}
void DeviceRemoteCD::pause()
{
    DLT_LOGS_UPDATED();
    if ( mPlayState != MMPresTypes::ePlayState::Literal::PLAYSTATE_PAUSED )
    {
        mRemoteCDPlayBackEngine->pauseRemoteCD();
    }
    else
    {
            if ( ( mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this ) && ( mDetect-> getActiveDeviceId () == REMOTECD_DEVICE_ID ))
       {
           std::function<void(const MMPresTypes::ePlayState&)> callback = mPlayEngine->getPlayStateStubCallback();
           if (callback != nullptr)
            {
                LOG_INFO(LOG_CONTEXT_MEDIA, " %s, %d sending pausestate <: %d>", __FUNCTION__, __LINE__, mPlayState );
                callback(mPlayState);
            }
       }
        
    } 
}
void DeviceRemoteCD::stop()
{
  if ( mPlayState != MMPresTypes::ePlayState::Literal::PLAYSTATE_STOPPED )
   {
         DLT_LOGS_UPDATED();
         mRemoteCDPlayBackEngine->stopRemoteCD();
   }
    else
    {
       //if ( ( mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this ) && ( mDetect-> getActiveDeviceId () == REMOTECD_DEVICE_ID ))
      // {
           std::function<void(const MMPresTypes::ePlayState&)> callback = mPlayEngine->getPlayStateStubCallback();
           if (callback != nullptr)
            {
                LOG_INFO(LOG_CONTEXT_MEDIA, " %s, %d sending playstate <: %d>", __FUNCTION__, __LINE__, mPlayState );
                callback(mPlayState);
            }
      // }
    }
}
void DeviceRemoteCD::next(uint64_t step)
{
    DLT_LOGS_UPDATED();
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::next out %llu" , step);
    while(step--){
    	mRemoteCDPlayBackEngine->nextRemoteCD();
    }
}
void DeviceRemoteCD::previous(uint64_t step)
{
    DLT_LOGS_UPDATED();
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::previous out %llu" , step);
    while(step--){
    	mRemoteCDPlayBackEngine->previousRemoteCD();
    }
}
void DeviceRemoteCD::seekToTime(uint64_t pos)
{
    DLT_LOGS_UPDATED();
    //mRemoteCD->
}
void DeviceRemoteCD::setRepeat(MMPresTypes::eRepeatMode mRepeatMode)
{
    DLT_LOGS_UPDATED();
    mRemoteCDPlayBackEngine->setRepeatRemoteCD(mRepeatMode);
}
void DeviceRemoteCD::setShuffle(MMPresTypes::eShuffleMode mShuffleMode)
{
    DLT_LOGS_UPDATED();
    mRemoteCDPlayBackEngine->setShuffleRemoteCD(mShuffleMode);
}

void DeviceRemoteCD::RemoteCDPlayActionStatus ( eCDPlayActions & mCDPlayActions)
{
    DLT_LOGS_UPDATED();

    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::RemoteCDPlayActionStatus: %d" , (int)mCDPlayActions);
    switch (mCDPlayActions)
    {
    case ePlay:
    {
        mPlayState = MMPresTypes::ePlayState::Literal::PLAYSTATE_PLAYING;
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::Cancelling timer and resplay");
        mSourceSwitch->cancelTimer();
        mSourceSwitch->resPlay(true);
        
       if (mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this)
        {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> Active Device so sending the data ");
            
        std::function<void(const MMPresTypes::tTrackState&)> mTrackStateCallBack =  mPlayEngine->getTrackStateStubCallback();
       
        LOG_INFO(LOG_CONTEXT_MEDIA, "%s, %d CurrentTrack: %llu, TotalTracks: %llu", __FUNCTION__, __LINE__, mRemoteCDPlayBackEngine -> mTrackStateRemoteCD.getCurrentTrack(), mRemoteCDPlayBackEngine -> mTrackStateRemoteCD.getTotalTracks());

        if ( mTrackStateCallBack != nullptr &&  ( mRemoteCDPlayBackEngine -> mTrackStateRemoteCD.getCurrentTrack() > 0 )  && ( mRemoteCDPlayBackEngine -> mTrackStateRemoteCD.getTotalTracks() > 0 ) )
        {

           mTrackStateCallBack( mRemoteCDPlayBackEngine -> mTrackStateRemoteCD );
           
            std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();
            if (nowPlayingCallback != nullptr)
            {
               LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::Sending the old NowPlaying data");
               MMPresTypes::eMetadataType metadatatype = mRemoteCDPlayBackEngine->getCDType();
               mRemoteCDPlayBackEngine -> mNowPlayingInfo_RemoteCDPlayEngine.setNowPlayingItemType(metadatatype);
               nowPlayingCallback( mRemoteCDPlayBackEngine -> mNowPlayingInfo_RemoteCDPlayEngine );
            }
        }
            

        
        }
            
       

    }
    break;
    case ePause:
    {
        mPlayState = MMPresTypes::ePlayState::Literal::PLAYSTATE_PAUSED;
    }
    break;
    case eStop:
    {
        mPlayState = MMPresTypes::ePlayState::Literal::PLAYSTATE_STOPPED;
    }
    break;
    default:
        mPlayState = MMPresTypes::ePlayState::Literal::PLAYSTATE_UNKNOWN;
    }

    //
    if (mDetect-> getActiveDeviceId () == REMOTECD_DEVICE_ID ){
      std::function<void(const MMPresTypes::ePlayState&)> callback = mPlayEngine->getPlayStateStubCallback();
      if (callback != nullptr)
         {
             LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::Sending the playState :%d ", mPlayState );
            callback(mPlayState);
         }
    }
    else{
        
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::Not Sending the playState and device id :%d ", mDetect-> getActiveDeviceId () );
        
    }
}



MMPresTypes::eCallResult DeviceRemoteCD::fastForward()
{
    
    DLT_LOGS_UPDATED();
    mRemoteCDPlayBackEngine->fastForwardRemoteCD();
    return MMPresTypes::eCallResult::CALLRESULT_OK;
    
}
MMPresTypes::eCallResult DeviceRemoteCD::fastReverse()
{
    DLT_LOGS_UPDATED();
    mRemoteCDPlayBackEngine->fastReverseRemoteCD();
    return MMPresTypes::eCallResult::CALLRESULT_OK;
}



void DeviceRemoteCD::RemoteCDPlayContextStatus(CDPlayContext mCDPlayContext)
{
    DLT_LOGS_UPDATED();
    switch(mCDPlayContext.first)
    {
    case eShuffle:
    {
        ShuffleStatus(mCDPlayContext.second);
    }
    break;
    case eRepeat:
    {
        RepeatStatus(mCDPlayContext.second);
    }
    break;
    case eNoContext:
    case eScan:
    break;
    }
}
void DeviceRemoteCD::RemoteCDNowPlayingInfo(CDNowPlaying mCDNowPlaying)
{
    DLT_LOGS_UPDATED();

    if ( mCDNowPlaying.names_. size () > 0 )
    {
        MMPresTypes::tNowPlayingInfo nowPlayingInfo;
        
        for (auto it = mCDNowPlaying.names_.begin(); it != mCDNowPlaying.names_.end(); it++)
        {
          if ( ( it->second.c_str() != nullptr  ) && ( it->second != "") )
            {
               switch( it->first)
               {
                     case eAlbumName:
                      {
                            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: Album Name  %s" , it->second.c_str());
                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            {
                                 nowPlayingInfo.setAlbum(it->second);
                            }
                            
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: invalid Album Name  ");
                                 nowPlayingInfo.setAlbum ( mediaSettings -> getInvalidString () ) ;
                            }
                      }
                    break;
                    case eArtistName:
                     {
                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            {                        
                                   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: Artist Name  %s" , it->second.c_str());
                                   nowPlayingInfo.setArtist(it->second);
                            }
                            
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: invalid Artist Name  ");
                                 nowPlayingInfo.setArtist ( mediaSettings -> getInvalidString () ) ;
                            }
                     }
                   break;
                    case eGenreName:
                     {
                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            { 
                           LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: Genre Name  %s" ,it->second.c_str());
                           nowPlayingInfo.setGenre(it->second);
                            }
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: invalid Genre Name  ");
                                nowPlayingInfo.setGenre ( mediaSettings -> getInvalidString () ) ;
                            }
                     }
                   break;
                    case eTrackName:
                    {
                         
                          
                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            { 
                                 LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: Track Name  %s" ,it->second.c_str());
                                nowPlayingInfo.setTitle(it->second);
                            }
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: invalid Track Name  ");
                                 nowPlayingInfo.setTitle ( mediaSettings -> getInvalidString () ) ;
                            }
                    }
                   break;
                   case eFileName:
                    {
                         
                           
                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            { 
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: File Name  %s" , it->second.c_str());
                               nowPlayingInfo.setFilename(it->second);
                            }
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: invalid File Name  ");
                                nowPlayingInfo.setFilename ( mediaSettings -> getInvalidString () ) ;
                            }
                    }
                   break;
                  case eFolderName :
                    {
                      

                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            { 
                                    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: Folder Name  %s" , it->second.c_str() );
                                   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: File  Name After adding FolderName   %s" , nowPlayingInfo.getFilename ().c_str () );
                                   nowPlayingInfo.setFilename ( it->second +  "/" + nowPlayingInfo.getFilename ()   );  
                            }
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: invalid Folder Name  ");
                                nowPlayingInfo.setFilename ( mediaSettings -> getInvalidString () ) ;
                            }
                // nowPlayingInfo.setNowPlayingItemType(Literal::METADATATYPE_FOLDER);
                    }
                  break;
                  default :
                    break;
              }
          }
          else
          {
             LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: Blank String found "); 
          }
        }

        MMPresTypes::eMetadataType mMetadatatype = mRemoteCDPlayBackEngine->getCDType();
        DLT_LOGS_UPDATED();
        nowPlayingInfo.setNowPlayingItemType(mMetadatatype);
        DLT_LOGS_UPDATED();
        nowPlayingInfo.setMediumId(REMOTE_CD_DEVICE_ID);
        //nowPlayingInfo.setNowPlayingItemType(METADATATYPE_TITLE);
        nowPlayingInfo.setIsFav(false);
        nowPlayingInfo.setChaptersAvailable(false);

         DLT_LOGS_UPDATED();
        //nowPlayingInfo.setMediumId("");
        //nowPlayingInfo.setIsFav(isFavourite);

        //nowPlayingInfo.setChaptersAvailable(isChapterAvailable);
        //nowPlayingInfo.setNowPlayingItemType(PlayHelper::getNowPlayingItemType(type));

        mNowPlayingInfo_RemoteCD = nowPlayingInfo;
        DLT_LOGS_UPDATED();

        std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();
        DLT_LOGS_UPDATED();
       if ( ( mDetect->getActiveDevice() != nullptr )  && ( mDetect->getActiveDevice() == this ) && ( mRemoteCDPlayBackEngine -> mTrackStateRemoteCD.getCurrentTrack() > 0 )  && ( mRemoteCDPlayBackEngine -> mTrackStateRemoteCD.getTotalTracks() > 0 ) )
        {
            std::function<void(const MMPresTypes::ePlayState&)> callback = mPlayEngine->getPlayStateStubCallback();
            if (callback != nullptr)
             {
                   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::Sending the playState from nowplaying Info :%d ", this -> mPlayState );
                   callback(this -> mPlayState);
             }
            std::function<void(const MMPresTypes::tTrackState&)> mTrackStateCallBack =  mPlayEngine->getTrackStateStubCallback();
             mTrackStateCallBack( mRemoteCDPlayBackEngine -> mTrackStateRemoteCD );
             DLT_LOGS_UPDATED();
            if (nowPlayingCallback != nullptr)
             {
               DLT_LOGS_UPDATED();
               nowPlayingCallback( nowPlayingInfo );
               DLT_LOGS_UPDATED();
             }
        }
        else
        {
            DLT_LOGS_UPDATED();
        }
    }
    else    LOG_ERROR ( LOG_CONTEXT_MEDIA, "%s:%d empty data received from remotecd", __FUNCTION__, __LINE__ ) ;
}

void DeviceRemoteCD::SessionQueryCreateNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t & mediumID, const MMPresTypes::FilterIDMap & filterIDs, const MMPresTypes::FilterMap & filters, const MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask, const uint64_t& startIndex, const uint64_t& numItems)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceRemoteCD::SessionQueryCreateNew : %d ", type);
    switch (type)
    {
    case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE:
    case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_CATEGORY_BROWSE:
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: SessionQueryCreateNew: SESSION_QUERY_FS_BROWSE");
        PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceRemoteCD::SessionQueryFSNew, this, sessionID, _client, reply, gotoPreviousReply, jumpToQueryReply, mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
    }
    break;
    case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE:
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: SessionQueryCreateNew: SESSION_QUERY_NOWPLAYINGLIST_BROWSE");
        PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceRemoteCD::SessionQueryNowPlayingNew, this, sessionID, _client, reply, gotoPreviousReply, jumpToQueryReply, mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
    }
    break;
    case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE:
    {
        PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceRemoteCD::SessionQueryNowPlayingNew, this, sessionID, _client, reply, gotoPreviousReply, jumpToQueryReply, mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
    }
    break;
    default:
        break;
    }
}

RequestBase* DeviceRemoteCD::SessionQueryFSNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex, const uint64_t& numItems)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: SessionQueryCreateNew: SESSION_QUERY_FS_BROWSE: SessionQueryFSNew");
    return (new SessionQueryRemoteCDFS(sessionID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
}

RequestBase* DeviceRemoteCD::SessionQueryNowPlayingNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex, const uint64_t& numItems)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: SessionQueryCreateNew: SESSION_QUERY_NOWPLAYINGLIST_BROWSE: SessionQueryNowPlayingNew");
    return  (new SessionQueryRemoteCDNowPlaying(sessionID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
}

void DeviceRemoteCD::SessionQueryCreateExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t & startIndex, const uint64_t& numItems)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d : <type: %d> ", __FUNCTION__, __LINE__, queryCacheData.mQueryType);
    switch (queryCacheData.mQueryType)
    {
    case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE:
    case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_CATEGORY_BROWSE:
    {
        PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceRemoteCD::SessionQueryFSExisting, this, sessionID, queryID, _client, reply, gotoPreviousReply, jumpToQueryReply, queryCacheData, startIndex, numItems));
    }
    break;
    case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE:
    {
        PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceRemoteCD::SessionQueryNowPlayingExisting, this, sessionID, queryID, _client, reply, gotoPreviousReply, jumpToQueryReply, queryCacheData, startIndex, numItems));
    }
    break;
    case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE:
    {
        PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceRemoteCD::SessionQueryNowPlayingExisting, this, sessionID, queryID, _client, reply, gotoPreviousReply, jumpToQueryReply, queryCacheData, startIndex, numItems));
    }
    break;
    default:
        break;
    }
}


RequestBase* DeviceRemoteCD::SessionQueryNowPlayingExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
{
    DLT_LOGS_UPDATED();
    
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: SessionQueryCreateNew: startIndex: %d, numItems: %d", startIndex, numItems);
    
    return (new SessionQueryRemoteCDNowPlaying(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems));
}

RequestBase* DeviceRemoteCD::SessionQueryFSExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
{
    DLT_LOGS_UPDATED();
    
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: SessionQueryCreateNew: startIndex: %d, numItems: %d", startIndex, numItems);
    
    return (new SessionQueryRemoteCDFS(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems));
}

/*RequestBase* DeviceRemoteCD::SessionQueryNowPlayingList(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex, const uint64_t& numItems)
{
	return (new SessionQueryRemoteCDFS(sessionID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
}*/

uint32_t DeviceRemoteCD::getDeviceCaps()
{

    return ( MMPresTypes::eDeviceCapabilities::DEVICECAP_FSBROWSE);
}


void DeviceRemoteCD::updateAvailabilityToAudioManager(bool available)
{
    //LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase :updateAvailabilityToAudioManager: mMediumId=%lu", mMediumInfo.getMMediumId());
    if (mSettings->getAMInteraction())
    {
        bool ret = false;
        if (available == true)
        {
            ret = mAudioInterface->updateSourceAvailabilitytoAM(REMOTE_CD_DEVICE_ID);
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::updateAvailabilityToAudioManager: true");
        }
        else
        {
            ret = mAudioInterface->updateSourceUnavailabilitytoAM(REMOTE_CD_DEVICE_ID);
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::updateAvailabilityToAudioManager: false");
        }
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::updateAvailabilityToAudioManager: status=%d,ret=%d",available,ret);
    }
}

void DeviceRemoteCD::RemoteCDActiveTrack( uint64_t mTrackIndex)
{
    DLT_LOGS_UPDATED();
//    LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceRemoteCD::%s, %d, folderNum: %d, TrackIndex: %d", __FUNCTION__, __LINE__, mTrackNumber.first, mTrackNumber.second);

    // this is a temporary change will be removed later

    this->mTrackIndex = static_cast<uint64_t> ( mTrackIndex );
     this->mTrackStateRemoteCD.setCurrentTrack(static_cast<uint64_t> (this->mTrackIndex ));

    this->dummyNowPlaying();
    std::function<void(const MMPresTypes::tTrackState&)> mTrackStateCallBack =  mPlayEngine->getTrackStateStubCallback();

    if ( ( mTrackStateCallBack != nullptr )  && ( mTrackStateRemoteCD.getCurrentTrack() > 0 )  && ( mTrackStateRemoteCD.getTotalTracks() > 0 )   )
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "%s, %d Sending TrackState, CurrentTrack: %lu, TotalTracks: %lu", __FUNCTION__, __LINE__, mTrackStateRemoteCD.getCurrentTrack(), mTrackStateRemoteCD.getTotalTracks());
        mTrackStateCallBack(mTrackStateRemoteCD);
        
        std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();
        DLT_LOGS_UPDATED();
        if (nowPlayingCallback != nullptr)
        {
            DLT_LOGS_UPDATED();
            // This callback is to send dummyNowPlaying callback 
            nowPlayingCallback( mNowPlayingInfo_RemoteCD );
            DLT_LOGS_UPDATED();
            
            // This call back must be send after dummyNowPlaying data to process if valid data received inside RemoteCDPlayEngine.cpp in the meanwhile
            // Below line is commented as fix for Elvis-2490390
            //nowPlayingCallback( mRemoteCDPlayBackEngine -> mNowPlayingInfo_RemoteCDPlayEngine );
             LOG_INFO(LOG_CONTEXT_MEDIA, "%s, %d Sending NowPlaying Callback from RemoteCD PlayEngine Data ", __FUNCTION__, __LINE__ );
        }
    }
}

int DeviceRemoteCD::getActiveTrackNum()
{
    DLT_LOGS_UPDATED();
    
    return this->mTrackNumber;
}
void DeviceRemoteCD::RemoteCDNumofTracksStatus( uint64_t  numofTracks)
{
    DLT_LOGS_UPDATED();
    LOG_INFO(LOG_CONTEXT_MEDIA, " numberof Tracks: %llu, and trackIndex : %llu", numofTracks, this->mTrackIndex);
    this->numofTracks = static_cast <uint64_t> (numofTracks);
    this->mTrackStateRemoteCD.setTotalTracks(static_cast <uint64_t> ( this-> numofTracks));

        std::function<void(const MMPresTypes::tTrackState&)> mTrackStateCallBack =  mPlayEngine->getTrackStateStubCallback();

    if (mTrackStateCallBack != nullptr &&  ( mTrackStateRemoteCD.getCurrentTrack() > 0 ) &&  ( mTrackStateRemoteCD.getTotalTracks() > 0  ))
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "Sending TrackState : RemoteCDNumofTracksStatus DeviceRemoteCD::%s, %d, CurrentTrack: %lu, TotalTracks: %lu", __FUNCTION__, __LINE__, mTrackStateRemoteCD.getCurrentTrack(), mTrackStateRemoteCD.getTotalTracks());
        mTrackStateCallBack(mTrackStateRemoteCD);
    }

}
void DeviceRemoteCD::RemoteCDTrackTimeStatus(TrackTime mTrackTime)
{
    DLT_LOGS_UPDATED();
    LOG_INFO(LOG_CONTEXT_MEDIA, " Current Time : %lu, Total time: %lu", mTrackTime.first, mTrackTime.second);


    std::function<void(const uint64_t&)> currentTimeCallback = mPlayEngine->getCurrentTimeStubCallback();

    if ( currentTimeCallback != nullptr )
    {
        currentTimeCallback(mTrackTime.first);
    }

    std::function<void(const uint64_t&)> totalTimeCallback =  mPlayEngine->getTotalDurationStubCallback();

    if ( totalTimeCallback != nullptr )
    {
        totalTimeCallback(mTrackTime.second);
    }

}


void DeviceRemoteCD::playDevice( const MMPresTypes::eMetadataType objType ,std::string uri )
{
    if ( mPlayState != MMPresTypes::ePlayState::Literal::PLAYSTATE_PLAYING )
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceRemoteCD::playDeviceRemoteCD %s, %d", __FUNCTION__, __LINE__);
         mRemoteCDPlayBackEngine->playDeviceRemoteCD();
    }
    else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::Cancelling timer and resplay");
        mSourceSwitch->cancelTimer();
        mSourceSwitch->resPlay(true);
      // if ( ( mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this ) && ( mDetect-> getActiveDeviceId () == REMOTECD_DEVICE_ID ))
     //  {
           std::function<void(const MMPresTypes::ePlayState&)> callback = mPlayEngine->getPlayStateStubCallback();
           if (callback != nullptr)
            {
                LOG_INFO(LOG_CONTEXT_MEDIA, " %s, %d sending playstate <: %d>", __FUNCTION__, __LINE__, mPlayState );
                callback(mPlayState);
            }
      // }
    }
}


void DeviceRemoteCD::ShuffleStatus(eCDActionContextValue mCDActionContextValue)
{
    DLT_LOGS_UPDATED();
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: ShuffleStatus  %d" , (int)mCDActionContextValue);
    std::function<void(const MMPresTypes::eShuffleMode&)> ShuffleStatusCallback = mPlayEngine->getShuffleModeStubCallback();

    MMPresTypes::eShuffleMode mShuffleMode = MMPresTypes::eShuffleMode::Literal::SHUFFLE_ALL;
    switch(mCDActionContextValue)
    {
    case eOn:
    {

    	// Hence the default value is initialized as SHUFFLE_ALL for "mShuffleMode".
    }
    break;
    case eOff:
    {
    	mShuffleMode = MMPresTypes::eShuffleMode::Literal::SHUFFLE_OFF;
    }
    break;
    case eOnFolder:
    {
    	mShuffleMode = MMPresTypes::eShuffleMode::Literal::SHUFFLE_FOLDER;
    }
    break;
    default:
    	break;
    }

    if( ShuffleStatusCallback != nullptr )
    {
        ShuffleStatusCallback(mShuffleMode);
    }

    /*std::unordered_map<eCDActionContextValue, MMPresTypes::eShuffleMode>::iterator it;
     it = mpresShuffleContextMap.find(mCDActionContextValue);
     if((it != mpresShuffleContextMap.end()) && (ShuffleStatusCallback != nullptr))
     {
         ShuffleStatusCallback(it->second);
     }*/

}
void DeviceRemoteCD::RepeatStatus(eCDActionContextValue mCDActionContextValue)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: RepeatStatus  %d" , (int)mCDActionContextValue);
    DLT_LOGS_UPDATED();
    std::function<void(const MMPresTypes::eRepeatMode&)>  RepeatStatusCallback = mPlayEngine->getRepeatModeStubCallback();

    /* std::unordered_map<eCDActionContextValue, MMPresTypes::eRepeatMode>::iterator it;
    it = mpresRepeatContextMap.find(mCDActionContextValue);
    if((it != mpresRepeatContextMap.end()) && (RepeatStatusCallback != nullptr))
    {
        RepeatStatusCallback(it->second);
    }
     */


    MMPresTypes::eRepeatMode mRepeatMode = MMPresTypes::eRepeatMode::Literal::REPEAT_ALL;
    switch(mCDActionContextValue)
    {
    case eOn:
    {
    	mRepeatMode = MMPresTypes::eRepeatMode::Literal::REPEAT_SONG;
    }
    break;
    case eOff:
    {
    	// Hence the default value is initialized as REPEAT_ALL for "mRepeatMode" its not required to assing again.
    }
    break;
    case eOnFolder:
    {
    	mRepeatMode = MMPresTypes::eRepeatMode::Literal::REPEAT_FOLDER;
    }
    break;
    default:
    	break;
    }

    if( RepeatStatusCallback != nullptr )
    {
        RepeatStatusCallback(mRepeatMode);
    }
}


uint64_t DeviceRemoteCD::getNumofTracks()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD:: getNumofTracks  %d" , this->numofTracks);
    return this->numofTracks;
}

MMPresTypes::tTrackState DeviceRemoteCD::getTrackState()
{
    return this->mTrackStateRemoteCD;
}


void DeviceRemoteCD::CallSessionQuery()
{
    FacadeHelper<SessionManager> mManager;
    uint32_t sessionID = mManager->openSession();
    std::shared_ptr<CommonAPI::ClientId> _client;
    MMPresTypes::FilterIDMap  filterIDs;
    filterIDs[MMPresTypes::eMetadataType::METADATATYPE_FOLDER] = 0x000;
    MMPresTypes::FilterMap filters;
    uint32_t  metadataAttributes = 20;
    uint32_t metadataCountTypes = 20;
    MMPresTypes::eSessionQueryTypes type = MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE;
    MMPresTypes::eMetadataType resultType = MMPresTypes::eMetadataType::Literal::METADATATYPE_REMOTECD_MP3;
    bool resetPrevious = false;
    bool AutoUpdateEnabled = false;
    uint32_t ContentMask = 0;
    uint64_t startIndex = 0;
    uint64_t NumofItems = 20;
    uint64_t mediumId = REMOTE_CD_DEVICE_ID;
    this->SessionQueryCreateNew(sessionID, _client, std::move(nullptr), std::move(nullptr), std::move(nullptr), mediumId , filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, NumofItems);
    //bool ret = mManager->closeSession(sessionID);
}


void DeviceRemoteCD::dummyNowPlaying()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::%s Line: %d", __FUNCTION__, __LINE__);
    MMPresTypes::tNowPlayingInfo nowPlayingInfo_;
    nowPlayingInfo_.setAlbum("");
    nowPlayingInfo_.setArtist("");
    nowPlayingInfo_.setGenre("");
    nowPlayingInfo_.setTitle("");
    nowPlayingInfo_.setFilename("");
   // LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::dummyNowPlaying", __FUNCTION__, __LINE__);
    MMPresTypes::eMetadataType mMetadatatype = mRemoteCDPlayBackEngine->getCDType();
    nowPlayingInfo_.setNowPlayingItemType(mMetadatatype);
    nowPlayingInfo_.setMediumId(REMOTE_CD_DEVICE_ID);
    //nowPlayingInfo.setNowPlayingItemType(METADATATYPE_TITLE);
    nowPlayingInfo_.setIsFav(false);
    nowPlayingInfo_.setChaptersAvailable(false);
    mNowPlayingInfo_RemoteCD = nowPlayingInfo_;
   //LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::dummyNowPlaying", __FUNCTION__, __LINE__);
   /* std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();
    if (nowPlayingCallback != nullptr)
    {
        nowPlayingCallback(nowPlayingInfo);
    }*/

}


bool DeviceRemoteCD::getVideoPlayStatus()
{
    DLT_LOGS_UPDATED();
    return false;
}


