/*
 * Copyright 2017 <copyright holder> <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#include "RemoteCDplaybackengine.h"

using namespace std;
using namespace placeholders;


std::unordered_map<MMPresTypes::eShuffleMode, eCDActionContextValue> mpresShufflePluginContextMap = {
    {MMPresTypes::eShuffleMode::Literal::SHUFFLE_ALL, eCDActionContextValue::eOn},
    {MMPresTypes::eShuffleMode::SHUFFLE_OFF, eCDActionContextValue::eOff},
    {MMPresTypes::eShuffleMode::Literal::SHUFFLE_FOLDER, eCDActionContextValue::eOnFolder }
};

std::unordered_map<MMPresTypes::eRepeatMode, eCDActionContextValue> mpresRepeatPluginContextMap = {
    {MMPresTypes::eRepeatMode::Literal::REPEAT_SONG, eCDActionContextValue::eOn},
    {MMPresTypes::eRepeatMode::Literal::REPEAT_ALL, eCDActionContextValue::eOff},
    {MMPresTypes::eRepeatMode::Literal::REPEAT_FOLDER, eCDActionContextValue::eOnFolder},
    
};



RemoteCDPlaybackEngine::RemoteCDPlaybackEngine()
    : mFolderCount ( 0 )
	, mFileCount ( 0 )
	, mActiveFolderNumber ( 0 )
	, mCurrentFolder ( 0 )
	, mParentFolderNum ( 0 )
	, mRemoteCDPluginInstance(nullptr )
	, mCDPlayContext(eCDActionContext::eNoContext, eCDActionContextValue::eInvalid)
	, mNumofClient(0)
	, mCDStatus(eInvalidStatus)
	, mCDMetaDataType(MMPresTypes::eMetadataType::Literal::METADATATYPE_UNKNOWN)
	, mCDError(eInvalidError, eInvalidHWError)
	, pluginInstanceFlag(false)
	, mTrackIndex ( 0 )
	, mItemIndex ( 0 )
{

    mRemoteCDDiscStatusCallBack = nullptr;
    mCDErrorStatusCallBack = nullptr;
    mEolStatusCallBack = nullptr ;
    mCDEjectChimesWarningCallBack = nullptr;
    mRemoteCDPluginInstance = FacadeHelper<MediaPresCore>() -> getRemoteCDPluginInstance ();
   

}



RemoteCDPlaybackEngine::~RemoteCDPlaybackEngine()
{

}



void RemoteCDPlaybackEngine::initPluginEvent()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, " %s %d start ", __FUNCTION__, __LINE__);;
    PresEventAbstraction::getInstance()->subscribe<cdStatus>(EVENT_CD_STATUS, std::bind(&RemoteCDPlaybackEngine::CDStatusEvent, this, std::placeholders::_1));

    PresEventAbstraction::getInstance()->subscribe<cdError>(EVENT_CD_ERROR, std::bind(&RemoteCDPlaybackEngine::CDErrorEvent, this, std::placeholders::_1));

    PresEventAbstraction::getInstance()->subscribe<cdType>(EVENT_CD_TYPE, std::bind(&RemoteCDPlaybackEngine::CDTypeEvent, this, std::placeholders::_1));
    
    PresEventAbstraction::getInstance()->subscribe<activeTrack>(EVENT_PLAYCONTROL_ACTIVETRACK_STATUS, std::bind(&RemoteCDPlaybackEngine::RemoteCDActiveTrack, this,_1));

    PresEventAbstraction::getInstance()->subscribe<numberOfTracksStatus>(EVENT_PLAYCONTROL_NUMBER_TRACKS_STATUS, std::bind(&RemoteCDPlaybackEngine::RemoteCDNumofTracksStatus, this,_1));
    
    PresEventAbstraction::getInstance()->subscribe<ejectChimesWarning>(EVENT_EJECT_CHAIMS_WARNING, std::bind(&RemoteCDPlaybackEngine::RemoteCDEjectChimesWarning, this,_1));

    PresEventAbstraction::getInstance()->subscribe<activeFolder>(EVENT_PLAYCONTROL_ACTIVEFOLDER_STATUS, std::bind(&RemoteCDPlaybackEngine::RemoteCDActiveFolderStatus, this,_1));
    
    PresEventAbstraction::getInstance()->subscribe<cdNowPlayingInfo>(EVENT_PLAYCONTROL_NOWPLAYING_STATUS, std::bind(&RemoteCDPlaybackEngine::RemoteCDNowPlayingInfo, this,_1));
    

   eEOLResult mEOLResult = static_cast < eEOLResult > ( mRemoteCDPluginInstance->getFordEolData () ) ;
   
  // auto stubAdapter = CommonAPI::Stub<MediaPlayerStubAdapter, MediaPlayerStubRemoteEvent>::stubAdapter_.lock();
   
  // while ( !stubAdapter );
   mEolStatusCallBack  =  this->getEOLStatus () ;
   
   if ( mEolStatusCallBack != nullptr )
   {
       mEolStatusCallBack ( static_cast < MMPresTypes::eCDEOLStatus::Literal > ( mEOLResult ) );
       LOG_INFO(LOG_CONTEXT_MEDIA, " %s %d <Sending getFordEolData %d >", __FUNCTION__, __LINE__, mEOLResult );;
        
   }
    
    LOG_INFO(LOG_CONTEXT_MEDIA, " %s %d <getFordEolData %d >", __FUNCTION__, __LINE__, mEOLResult );;
    
    
    
    //PresEventAbstraction::getInstance()->subscribe<browseStatus>(EVENT_BROWSECONTROL_BROWSEINFO_STATUS, std::bind(&RemoteCDPlaybackEngine::queryDataCallback, this,_1, _2));
}

void RemoteCDPlaybackEngine::RemoteCDEjectChimesWarning(bool warningstatus){
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::RemoteCDEjectChimesWarning- :%d ", (int)warningstatus);
	this->mCDEjectChimesWarningCallBack = getCDEjectChimesWarningEventCallback();
	if (this->mCDEjectChimesWarningCallBack != nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::RemoteCDEjectChimesWarning-Entered ", (int)warningstatus);
		this->mCDEjectChimesWarningCallBack(warningstatus);
	}
}

void RemoteCDPlaybackEngine::setCDStatusEvent(std::function<void(::v3::com::harman::mmpres::MMPresTypes::eRemoteCDDiscStatus)> func)
{
    mRemoteCDDiscStatusCallBack = func;
}
void RemoteCDPlaybackEngine::setCDErrorEvent(std::function<void(::v3::com::harman::mmpres::MMPresTypes::RemoteCDErrorStatus)> func)
{
    mCDErrorStatusCallBack = func;
}

void RemoteCDPlaybackEngine::setCDEjectChimesWarningEvent(std::function<void(bool)> func)
{
	mCDEjectChimesWarningCallBack = func;
}

void RemoteCDPlaybackEngine::setEOLStatusEvent ( std::function < void (::v3::com::harman::mmpres::MMPresTypes::eCDEOLStatus)>  func )
{
    mEolStatusCallBack = func;
}

std::function<void(::v3::com::harman::mmpres::MMPresTypes::eRemoteCDDiscStatus)>& RemoteCDPlaybackEngine::getCDStatusEventCallback()
{
    return mRemoteCDDiscStatusCallBack;
}
std::function<void(::v3::com::harman::mmpres::MMPresTypes::RemoteCDErrorStatus)>& RemoteCDPlaybackEngine::getCDErrorEventCallback()
{
    return mCDErrorStatusCallBack;
}

std::function<void (::v3::com::harman::mmpres::MMPresTypes::eCDEOLStatus)>& RemoteCDPlaybackEngine::getEOLStatus () 
{
    return mEolStatusCallBack;
}
std::function<void(bool)>& RemoteCDPlaybackEngine::getCDEjectChimesWarningEventCallback()
{
    return mCDEjectChimesWarningCallBack;
}



void RemoteCDPlaybackEngine::setRepeatRemoteCD(int RepeatMode)
{
     LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::setRepeatRemoteCD: %d", RepeatMode);;
    mRemoteCDPluginInstance->setContext(getCDPlayContext(eRepeat, RepeatMode));
}
void RemoteCDPlaybackEngine::setShuffleRemoteCD(int ShuffleMode)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::setShuffleRemoteCD: %d", ShuffleMode);
    mRemoteCDPluginInstance->setContext(getCDPlayContext(eShuffle, ShuffleMode));
}
void RemoteCDPlaybackEngine::resumeRemoteCD()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::resumeRemoteCD");
    mRemoteCDPluginInstance->setAction(ePlay);
}
void RemoteCDPlaybackEngine::pauseRemoteCD()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::pauseRemoteCD");
    mRemoteCDPluginInstance->setAction(ePause);
}
void RemoteCDPlaybackEngine::stopRemoteCD()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::stopRemoteCD");
    mRemoteCDPluginInstance->setAction(eStop);
}
void RemoteCDPlaybackEngine::nextRemoteCD()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::nextRemoteCD");
    CDPlayItem mCDPlayItem(eNext);
    mRemoteCDPluginInstance->setPlayItem(mCDPlayItem);

}
void RemoteCDPlaybackEngine::previousRemoteCD()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::previousRemoteCD");
    CDPlayItem mCDPlayItem(ePrev);
    mRemoteCDPluginInstance->setPlayItem(mCDPlayItem);

}
void RemoteCDPlaybackEngine::fastForwardRemoteCD()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::fastForwardRemoteCD");
    mRemoteCDPluginInstance->setAction(eForwardSpeed1);

}
void RemoteCDPlaybackEngine::fastReverseRemoteCD()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::fastReverseRemoteCD");
    mRemoteCDPluginInstance->setAction(eReverseSpeed1);

}
void RemoteCDPlaybackEngine::playDeviceRemoteCD()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, " %s, %d", __FUNCTION__, __LINE__);


    mRemoteCDPluginInstance->setAction(ePlay);
}


void RemoteCDPlaybackEngine::setPlayItem(eCDPlayItemType mCDPlayItemType, uint32_t itemId)
{

    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: - setPlayItem : %d ", itemId );
    CDPlayItem mCDPlayItem(eCurrent, mCDPlayItemType, { ( ( itemId >> 16 ) & 0xFFFF ), ( itemId & 0xFFFF ) } ) ;
    mRemoteCDPluginInstance->setPlayItem(mCDPlayItem);

}




CDPlayContext RemoteCDPlaybackEngine::getCDPlayContext(eCDActionContext mCDActionContext, int mode)
{

    switch (mCDActionContext)
    {
    case eShuffle:
    {
        std::unordered_map<MMPresTypes::eShuffleMode, eCDActionContextValue>::iterator it;
        mCDPlayContext.first = eShuffle;
        it = mpresShufflePluginContextMap.find((MMPresTypes::eShuffleMode::Literal)mode);
        if(it != mpresShufflePluginContextMap.end() )
        {
             LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: - getCDPlayContext :%d , mode : %d", (int)it->second, mode);
            mCDPlayContext.second = it->second;
        }
    }
    break;
    case eRepeat:
    {
        std::unordered_map<MMPresTypes::eRepeatMode, eCDActionContextValue>::iterator it;
        mCDPlayContext.first = eRepeat;
        it = mpresRepeatPluginContextMap.find((MMPresTypes::eRepeatMode::Literal)mode);
        if(it != mpresRepeatPluginContextMap.end() )
        {
             LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: - getCDPlayContext :%d , mode: %d",  (int)it->second, mode);
            mCDPlayContext.second = it->second;
        }

    }
    break;
    case eScan:
    {
    }
    break;
    case eNoContext:
    {
    }
    break;
    }

    return mCDPlayContext;
}



void RemoteCDPlaybackEngine::CDStatusEvent(eCDStatus mCDStatus)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::CDStatusEvent - :%d ", (int)mCDStatus);
    this->mCDStatus = mCDStatus;
    
    if ( mCDStatus == eLoading )
    {
        ClearAllNames ( );
        ClearAllFolderItem ();
        MMPresTypes::tNowPlayingInfo nowPlayingInfo_;
        nowPlayingInfo_.setAlbum("");
        nowPlayingInfo_.setArtist("");
        nowPlayingInfo_.setGenre("");
        nowPlayingInfo_.setTitle("");
        nowPlayingInfo_.setFilename("");
        // LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::dummyNowPlaying", __FUNCTION__, __LINE__);
        MMPresTypes::eMetadataType mMetadatatype = getCDType();
        nowPlayingInfo_.setNowPlayingItemType(mMetadatatype);
        nowPlayingInfo_.setMediumId(REMOTE_CD_DEVICE_ID);
        //nowPlayingInfo.setNowPlayingItemType(METADATATYPE_TITLE);
        nowPlayingInfo_.setIsFav(false);
        nowPlayingInfo_.setChaptersAvailable(false);
        mNowPlayingInfo_RemoteCDPlayEngine = nowPlayingInfo_;
        //this -> dummyNowplaying ();
    }
    
    this->mRemoteCDDiscStatusCallBack = getCDStatusEventCallback();
    if(this->mRemoteCDDiscStatusCallBack != nullptr)
    {
        ::v3::com::harman::mmpres::MMPresTypes::eRemoteCDDiscStatus mRemoteCDDiscStatus = static_cast< ::v3::com::harman::mmpres::MMPresTypes::eRemoteCDDiscStatus::Literal > (mCDStatus);
        this->mRemoteCDDiscStatusCallBack(mRemoteCDDiscStatus);
    }
}

void RemoteCDPlaybackEngine::publishNowPlayingInfoIfNeeded(uint64_t itemid)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::publishNowPlayingInfoIfNeeded - :%llu currenttrack %d ", itemid, this->mTrackIndex);
   if (this->mTrackIndex == itemid)
   {
      std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();
      if (nowPlayingCallback != nullptr)
      {
         MMPresTypes::tNowPlayingInfo dummyNowPlayingInfo;
         dummyNowPlayingInfo.setAlbum("");
         dummyNowPlayingInfo.setArtist("");
         dummyNowPlayingInfo.setGenre("");
         dummyNowPlayingInfo.setTitle("");
         dummyNowPlayingInfo.setFilename("");
         // LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::dummyNowPlaying", __FUNCTION__, __LINE__);
         MMPresTypes::eMetadataType mMetadatatype = getCDType();
         dummyNowPlayingInfo.setNowPlayingItemType(mMetadatatype);
         dummyNowPlayingInfo.setMediumId(REMOTE_CD_DEVICE_ID);
         //nowPlayingInfo.setNowPlayingItemType(METADATATYPE_TITLE);
         dummyNowPlayingInfo.setIsFav(false);
         dummyNowPlayingInfo.setChaptersAvailable(false);

         // This callback is to send dummyNowPlaying callback 
         nowPlayingCallback(dummyNowPlayingInfo);
         // This call back must be send after dummyNowPlaying data to process if valid data received inside RemoteCDPlayEngine.cpp in the meanwhile 
         nowPlayingCallback(this->mNowPlayingInfo_RemoteCDPlayEngine);
         LOG_INFO(LOG_CONTEXT_MEDIA, "%s, %d publishNowPlayingInfoIfNeeded Sending NowPlaying Callback from RemoteCD PlayEngine Data ", __FUNCTION__, __LINE__);
      }
   }
}

void RemoteCDPlaybackEngine::CDErrorEvent(CDError mCDError)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::CDErrorEvent- :%d  and HWerror: %d", (int)mCDError.first, (int)mCDError.second);
    this->mCDError = mCDError;
    this->mCDErrorStatusCallBack = getCDErrorEventCallback();
    if (  this->mCDErrorStatusCallBack != nullptr)
    {
        ::v3::com::harman::mmpres::MMPresTypes::RemoteCDErrorStatus mRemoteCDErrorStatus;
        mRemoteCDErrorStatus.setDiskError(static_cast <::v3::com::harman::mmpres::MMPresTypes::eRemoteCDError::Literal> (mCDError.first));
        mRemoteCDErrorStatus.setHwError(static_cast <::v3::com::harman::mmpres::MMPresTypes::eRemoteCDHardwareError::Literal> (mCDError.second));

        this->mCDErrorStatusCallBack(mRemoteCDErrorStatus);
    }
}


void RemoteCDPlaybackEngine::dummyNowPlaying () 
{
    LOG_INFO( LOG_CONTEXT_MEDIA, " %s %d ", __FUNCTION__ , __LINE__ );
    MMPresTypes::tNowPlayingInfo nowPlayingInfo_;
    nowPlayingInfo_.setAlbum("");
    nowPlayingInfo_.setArtist("");
    nowPlayingInfo_.setGenre("");
    nowPlayingInfo_.setTitle("");
    nowPlayingInfo_.setFilename("");
   // LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceRemoteCD::dummyNowPlaying", __FUNCTION__, __LINE__);
    MMPresTypes::eMetadataType mMetadatatype = getCDType();
    nowPlayingInfo_.setNowPlayingItemType(mMetadatatype);
    nowPlayingInfo_.setMediumId(REMOTE_CD_DEVICE_ID);
    //nowPlayingInfo.setNowPlayingItemType(METADATATYPE_TITLE);
    nowPlayingInfo_.setIsFav(false);
    nowPlayingInfo_.setChaptersAvailable(false);
    mNowPlayingInfo_RemoteCDPlayEngine = nowPlayingInfo_;
    
}


void RemoteCDPlaybackEngine::RemoteCDNowPlayingInfo ( CDNowPlaying mCDNowPlaying )
{
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
                            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: Album Name  %s" , it->second.c_str());
                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            {
                                 nowPlayingInfo.setAlbum(it->second);
                            }
                            
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: invalid Album Name  ");
                                 nowPlayingInfo.setAlbum ( mediaSettings -> getInvalidString () ) ;
                            }
                      }
                    break;
                    case eArtistName:
                     {
                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            {                        
                                   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: Artist Name  %s" , it->second.c_str());
                                   nowPlayingInfo.setArtist(it->second);
                            }
                            
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: invalid Artist Name  ");
                                 nowPlayingInfo.setArtist ( mediaSettings -> getInvalidString () ) ;
                            }
                     }
                   break;
                    case eGenreName:
                     {
                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            { 
                           LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: Genre Name  %s" ,it->second.c_str());
                           nowPlayingInfo.setGenre(it->second);
                            }
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: invalid Genre Name  ");
                                nowPlayingInfo.setGenre ( mediaSettings -> getInvalidString () ) ;
                            }
                     }
                   break;
                    case eTrackName:
                    {
                         
                          
                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            { 
                                 LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: Track Name  %s" ,it->second.c_str());
                                nowPlayingInfo.setTitle(it->second);
                            }
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: invalid Track Name  ");
                                 nowPlayingInfo.setTitle ( mediaSettings -> getInvalidString () ) ;
                            }
                    }
                   break;
                   case eFileName:
                    {
                         
                           
                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            { 
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: File Name  %s" , it->second.c_str());
                               nowPlayingInfo.setFilename(it->second);
                            }
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: invalid File Name  ");
                                nowPlayingInfo.setFilename ( mediaSettings -> getInvalidString () ) ;
                            }
                    }
                   break;
                  case eFolderName :
                    {
                      

                            if ( mediaSettings -> is_valid_utf8 ( it->second.c_str () ) )
                            { 
                                    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: Folder Name  %s" , it->second.c_str() );
                                   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: File  Name After adding FolderName   %s" , nowPlayingInfo.getFilename ().c_str () );
                                   nowPlayingInfo.setFilename ( it->second +  "/" + nowPlayingInfo.getFilename ()   );  
                            }
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: invalid Folder Name  ");
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
             LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine:: Blank String found "); 
          }
        }

        MMPresTypes::eMetadataType mMetadatatype = getCDType();
        nowPlayingInfo.setNowPlayingItemType(mMetadatatype);
        nowPlayingInfo.setMediumId( 65537 );
        //nowPlayingInfo.setNowPlayingItemType(METADATATYPE_TITLE);
        nowPlayingInfo.setIsFav(false);
        nowPlayingInfo.setChaptersAvailable(false);


        //nowPlayingInfo.setMediumId("");
        //nowPlayingInfo.setIsFav(isFavourite);

        //nowPlayingInfo.setChaptersAvailable(isChapterAvailable);
        //nowPlayingInfo.setNowPlayingItemType(PlayHelper::getNowPlayingItemType(type));

        mNowPlayingInfo_RemoteCDPlayEngine = nowPlayingInfo;
   


    }
}
void RemoteCDPlaybackEngine::CDTypeEvent (eCDType mCDType)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::CDTypeEvent:%d ", (int)mCDType);
    switch(mCDType)
    {
    case eMP3:
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::CDTypeEvent:Mp3");
        this->mCDMetaDataType = MMPresTypes::eMetadataType::Literal::METADATATYPE_REMOTECD_MP3;

    }
    break;
    case eCDTextCDDA :
    case eNoCDTextCDDA :
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::CDTypeEvent:CDDA");
        this->mCDMetaDataType = MMPresTypes::eMetadataType::Literal::METADATATYPE_REMOTECD_CDDA;
    }
    break;
    default:
        this->mCDMetaDataType = MMPresTypes::eMetadataType::Literal::METADATATYPE_UNKNOWN;
    }
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> RemoteCDPlaybackEngine::CDTypeEvent- :%d", this->mCDMetaDataType);
}

eCDStatus RemoteCDPlaybackEngine::getCDSttaus()
{


    return this->mCDStatus;
}
MMPresTypes::eMetadataType RemoteCDPlaybackEngine::getCDType()
{
    return this->mCDMetaDataType;

}
CDError RemoteCDPlaybackEngine::getCDError()
{
    return this->mCDError;
}

IRemoteCDPlugin* RemoteCDPlaybackEngine::getPluginInstance()
{

    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d ", __FUNCTION__ , __LINE__ );
    
    if ( mRemoteCDPluginInstance == nullptr )
        return FacadeHelper<MediaPresCore>() -> getRemoteCDPluginInstance ();
    else
        return mRemoteCDPluginInstance;
    
    
    
}



void RemoteCDPlaybackEngine::RemoteCDActiveTrack( uint64_t  mTrackIndex )
{
     LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d", __FUNCTION__ , __LINE__);
//    LOG_INFO(LOG_CONTEXT_MEDIA, "RemoteCDPlayBackEngine ::%s, %d, folderNum: %d, TrackIndex: %d", __FUNCTION__, __LINE__, mTrackNumber.first, mTrackNumber.second);

    // this is a temporary change will be removed later
    
    LOG_INFO(LOG_CONTEXT_MEDIA, "RemoteCDPlayBackEngine::%s, %d, clearing NowPlaying data", __FUNCTION__, __LINE__);
    //nowPlayInfo.names_.clear();
    
    this->mTrackIndex = static_cast < uint16_t > ( mTrackIndex ) ;
    this->mTrackStateRemoteCD.setCurrentTrack(static_cast<uint64_t> (this->mTrackIndex ));


    this -> dummyNowPlaying ();
    std::function<void(const MMPresTypes::tTrackState&)> mTrackStateCallBack =  mPlayEngine->getTrackStateStubCallback();

    if (mTrackStateCallBack != nullptr &&  ( mTrackStateRemoteCD.getCurrentTrack() > 0 ) &&  ( mTrackStateRemoteCD.getTotalTracks() > 0 ) )
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "Sending TrackState :DeviceRemoteCD::%s, %d, CurrentTrack: %llu, TotalTracks: %llu", __FUNCTION__, __LINE__, mTrackStateRemoteCD.getCurrentTrack(), mTrackStateRemoteCD.getTotalTracks());
        mTrackStateCallBack(mTrackStateRemoteCD);
    }

}

void RemoteCDPlaybackEngine::RemoteCDNumofTracksStatus(uint64_t  numofTracks)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d", __FUNCTION__ , __LINE__);
    LOG_INFO(LOG_CONTEXT_MEDIA, " numberof Tracks: %llu, and trackIndex : %llu", numofTracks, this->mTrackIndex);
    this -> mTotalTracks  = static_cast < uint64_t > ( numofTracks );
    this -> mTrackStateRemoteCD.setTotalTracks( static_cast<uint64_t> ( this -> mTotalTracks ) ) ;

        std::function<void(const MMPresTypes::tTrackState&)> mTrackStateCallBack =  mPlayEngine->getTrackStateStubCallback();

    if (mTrackStateCallBack != nullptr && mTrackStateRemoteCD.getCurrentTrack()>0 && mTrackStateRemoteCD.getTotalTracks()>0 )
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "Sending TrackState : RemoteCDNumofTracksStatus DeviceRemoteCD::%s, %d, CurrentTrack: %lu, TotalTracks: %lu", __FUNCTION__, __LINE__, mTrackStateRemoteCD.getCurrentTrack(), mTrackStateRemoteCD.getTotalTracks());
        mTrackStateCallBack(mTrackStateRemoteCD);
    }

}


void RemoteCDPlaybackEngine::RemoteCDActiveFolderStatus ( CDActiveFolder mCDActiveFolder )
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d", __FUNCTION__ , __LINE__);


    mItemIndex          = ( mCDActiveFolder . path . begin() ) -> second ;

    LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d < ActivefolderNumber : %d, itemIndex: %d >", __FUNCTION__ , __LINE__, mActiveFolderNumber, mItemIndex );
    

    

    

    mFolderCount = ( mCDActiveFolder . summary . begin() ) -> second ;

    mFileCount =   ( mCDActiveFolder . summary . begin() + 1 ) -> second ;

    for ( auto it : mCDActiveFolder.summary )
    {
       LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d < itemType : %d, itemCount: %d >",      __FUNCTION__ , __LINE__,    it.first , it.second );

    }


    if ( mActiveFolderNumber != ( mCDActiveFolder . path . begin() ) -> first   )
    {

    mActiveFolderNumber = ( mCDActiveFolder . path . begin() ) -> first ;
    
     insertFolderItemCount ( (int ) mActiveFolderNumber, (int )  ( mFolderCount + mFileCount ) );
    
  
    //mFolderSequence.clear();

    for ( auto it : mCDActiveFolder.path )
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d < folderNumber : %d, itemIndex: %d >", __FUNCTION__ , __LINE__, it.first , it.second );
      //  mFolderSequence.push_back( it.first );

    }

    LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d Send BrowseRequest : < ActivefolderNumber: %d , totalItem: %d,  >",      __FUNCTION__ , __LINE__, mActiveFolderNumber,    ( mFolderCount + mFileCount )  );


    //FolderRequest mFolderRequest = make_shared <FolderRequestedData> ( mActiveFolderNumber, 0, ( mFolderCount + mFileCount ) >= 20 ? 20 : ( mFolderCount + mFileCount ) ,  FolderRequestedData::eHead , MAX_ITEM_NAME_LENGTH, FolderRequestedData::eRequestedFolder );

    //getPluginInstance() -> getBrowseInfo(mFolderRequest);

    }




}
