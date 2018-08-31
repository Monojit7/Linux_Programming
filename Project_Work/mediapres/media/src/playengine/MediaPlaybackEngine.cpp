#include <spot/harman_kpimarker.h>
#include <playengine/MediaPlaybackEngine.h>
#include<device/DeviceBase.h>
#include<MediaPresCore.h>

std::unordered_map<PlayerTypes::PlayerError, MMPresTypes::eMediaError> MediaPlaybackEngine::mPlayerToMediaError = {
   { PlayerTypes::PlayerError::NO_ERROR , MMPresTypes::eMediaError::MEDIAERROR_NO_ERROR },
   { PlayerTypes::PlayerError::BAD_PLAYLIST, MMPresTypes::eMediaError::MEDIAERROR_BADPLAYLIST },
   { PlayerTypes::PlayerError::UNAVAILABLE_PLAYLIST, MMPresTypes::eMediaError::MEDIAERROR_NO_PLAYLIST },
   { PlayerTypes::PlayerError::BACKEND_UNREACHABLE, MMPresTypes::eMediaError::MEDIAERROR_BACKEND_UNREACHABLE },
   { PlayerTypes::PlayerError::ERROR_DRM, MMPresTypes::eMediaError::MEDIAERROR_DRM },
   { PlayerTypes::PlayerError::ERROR_COMMUNICATION, MMPresTypes::eMediaError::MEDIAERROR_COMMUNICATION },
   { PlayerTypes::PlayerError::ERROR_FILEUNAVAILABLE, MMPresTypes::eMediaError::MEDIAERROR_FILEUNAVAILABLE },
   { PlayerTypes::PlayerError::ERROR_FILENOTEXIST, MMPresTypes::eMediaError::MEDIAERROR_FILENOTEXIST },
   { PlayerTypes::PlayerError::ERROR_FILECRRUPT, MMPresTypes::eMediaError::MEDIAERROR_FILECORRUPT },
   { PlayerTypes::PlayerError::ERROR_PERMISSION, MMPresTypes::eMediaError::MEDIAERROR_NO_PERMISSION },
   { PlayerTypes::PlayerError::ERROR_NOTSUPPORT, MMPresTypes::eMediaError::MEDIAERROR_UNSUPPORTED},
   { PlayerTypes::PlayerError::INVALID_AUDIOLANGUAGE, MMPresTypes::eMediaError::MEDIAERROR_AUDIOLANGUAGE_INVALID },
   { PlayerTypes::PlayerError::INVALID_SUBTITLE, MMPresTypes::eMediaError::MEDIAERROR_SUBTITLE_INVALID },
   { PlayerTypes::PlayerError::UNAVAILABLE_SUBTITLE, MMPresTypes::eMediaError::MEDIAERROR_SUBTITLE_UNAVAILABLE },
   { PlayerTypes::PlayerError::UNAVAILABLE_VIDEO, MMPresTypes::eMediaError::MEDIAERROR_VIDEO_UNAVAILABLE }
};
MediaPlaybackEngine::MediaPlaybackEngine():mSubscribed(false)
   , mNumClients(0u)
   , mPlayer("local","inst0")
   , mMediaSetting("local","inst0")
   , mPlayerProxyStatus(CommonAPI::AvailabilityStatus::UNKNOWN)
   , mPlaybackStatusToken(0u)
   , mSubtitleToken(0)
   , mUpScaleToken(0u)
   , mAudioChannelToken(0u)
   , mCurrentMediumIdToken(0u)
   , mCurrentMediumId(0u)
   , mPlayState(MMPresTypes::ePlayState::PLAYSTATE_UNKNOWN)
   , mDriverRestrictionsVideoStatus(false)
   , mRestrictedModeToken(0u)
   , mDriverRestrictionsVideoStatusInformedToCoC(false)
   , mVideoPlaybackStatusFromCoC(false)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::MediaPlaybackEngine-");

   SUBSCRIBE(mPlayer, getProxyStatusEvent(), &MediaPlaybackEngine::availibility, std::placeholders::_1);

    //initialize Mpres ford-specific plugin, for e.g. MasterReset
   initPluginEvents();
   initAMBInterface();

   setDriverRestrictionsVideoStatusInformedToCoC(false);
   mtrackListSupportStubCallback = nullptr;
   mPromoteStatusStubCallBack = nullptr;
   mDemoteStatusStubCallBack = nullptr;
   mAddToWishListStatusStubCallBack = nullptr;
   mPlayStateStubCallback = nullptr;
   mCurrentTimeStubCallback = nullptr;
   mTotalDurationStubCallback = nullptr;
   mTrackStateStubCallback = nullptr;
   mNowPlayingDataStubCallback = nullptr;
   mRepeatModeStubCallback = nullptr;
   mShuffleModeStubCallback = nullptr;
   mTrackSeekSpeedStubCallback = nullptr;
   mArtworkStatusStubCallback = nullptr;
   mRemotePlayerCapabilityStubCallback = nullptr;
   mSyncStatusStubCallback = nullptr;
   mSubtitleStatusStubCallback = nullptr;
   mAudioChannelStatusStubCallback = nullptr;
   mUpScaleStatusStubCallback = nullptr;
   mSpeedInfoStubCallback = nullptr;
   mAudioInterface->setActiveDeviceBTLinkLossStubCallback(nullptr);
}

MediaPlaybackEngine::~MediaPlaybackEngine()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::~MediaPlaybackEngine-");
   subscribeToPlaybackStatus(false);
   mtrackListSupportStubCallback = nullptr;
   mPlayStateStubCallback = nullptr;
   mCurrentTimeStubCallback = nullptr;
   mTotalDurationStubCallback = nullptr;
   mTrackStateStubCallback = nullptr;
   mNowPlayingDataStubCallback = nullptr;
   mRepeatModeStubCallback = nullptr;
   mShuffleModeStubCallback = nullptr;
   mTrackSeekSpeedStubCallback = nullptr;
   mArtworkStatusStubCallback = nullptr;
   mRemotePlayerCapabilityStubCallback = nullptr;
   mSyncStatusStubCallback = nullptr;
   mSubtitleStatusStubCallback = nullptr;
   mAudioChannelStatusStubCallback = nullptr;
   mSpeedInfoStubCallback = nullptr;
   mAudioInterface->setActiveDeviceBTLinkLossStubCallback(nullptr);
}

void MediaPlaybackEngine::initPluginEvents()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::initPluginEvents ");

   //subscribe to masterReset event/command from Media pres ford plugin.
    PresEventAbstraction::getInstance()->subscribe<eventMasterReset>(REQ_MASTERRESET, std::bind(&MediaPlaybackEngine::masterResetEvent, this, std::placeholders::_1));

   //subscribe to DeviceError event/command from Media pres ford plugin.
   PresEventAbstraction::getInstance()->subscribe<eventDeviceError>(REQ_DEVICEERROR, std::bind(&MediaPlaybackEngine::deviceErrorEvent, this, std::placeholders::_1));

   //subscribe to driverRestrictionsVideo event/command from Media pres ford plugin.
   PresEventAbstraction::getInstance()->subscribe<eventDriverRestrictionsVideo>(REQ_DRIVERRESTRICTION_VIDEO, std::bind(&MediaPlaybackEngine::driverRestrictionsVideoStatusEvent, this, std::placeholders::_1));

}

void MediaPlaybackEngine::initAMBInterface()
{		/* get media player instance to set volume */
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::initAMBInterface");
    std::string domain = "local";
    std::string connection = "AMB_MediaPlayback";
    std::string instance = "AutomotiveMessageBrokerProvider.ford_MediaPlayback";
    mRuntime = CommonAPI::Runtime::get();
    mMediaPlayBack = mRuntime->buildProxy<mediaplaybackProxy>(domain,instance, connection);
    if(mMediaPlayBack != nullptr)
    {
        mMediaPlayBack->getProxyStatusEvent().subscribe(std::bind(&MediaPlaybackEngine::mediaPlayBackProxyAvailabiltyCB,this,std::placeholders::_1));
    }
    else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "mMediaPlayBack is null");
    }

}

void MediaPlaybackEngine::mediaPlayBackProxyAvailabiltyCB(const CommonAPI::AvailabilityStatus &AvailState)
{
    if(CommonAPI::AvailabilityStatus::AVAILABLE == AvailState)
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::mediaPlayBackProxyAvailabiltyCB : AVAILABLE");
    }
}

void MediaPlaybackEngine::setActiveTrackNum(const uint32_t track)
{
    mMediaPlayBack->getAPIM_ActiveTrackNum_StAttribute().setValueAsync(track,(std::bind(&MediaPlaybackEngine::ActiveTrackNumCB, this, _1)));
}

void MediaPlaybackEngine::setTotalTracks(const uint32_t track)
{
    mMediaPlayBack->getAPIM_NumberofTracks_StAttribute().setValueAsync(track,(std::bind(&MediaPlaybackEngine::TotalTracksNumCB, this, _1)));
}

void MediaPlaybackEngine::ActiveTrackNumCB(const CommonAPI::CallStatus &status)
{
    LOG_INFO(LOG_CONTEXT_MEDIA,"MediaPlaybackEngine::ActiveTrackNumCB called");
}

void MediaPlaybackEngine::TotalTracksNumCB(const CommonAPI::CallStatus &status)
{
    LOG_INFO(LOG_CONTEXT_MEDIA,"MediaPlaybackEngine::TotalTracksNumCB called");
}

void MediaPlaybackEngine::deviceErrorEvent(const tdeviceError& deviceError)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::deviceErrorEvent: deviceErrorEvent=%d", (int)deviceError);
   MMPresTypes::eMediaError MediaError = MMPresTypes::eMediaError::MEDIAERROR_UNKNOWN;
   uint8_t slotId = 0;

   switch (deviceError)
   {
   case eTOOMANYHUBS:
      //MediaError = MMPresTypes::eMediaError::MEDIAERROR_TOOMANY_USBDEVICES;
      MediaError = MMPresTypes::eMediaError::MEDIAERROR_USBHUB_UNSUPPORTED;
      break;

   case eTOOMANYHUBS_RESOLVED:
      slotId = 1;
      MediaError = MMPresTypes::eMediaError::MEDIAERROR_USBHUB_UNSUPPORTED;
      break;

   case eUNSUPPORTEDHUB:
      MediaError = MMPresTypes::eMediaError::MEDIAERROR_USBHUB_UNSUPPORTED;
      break;

   case eUNSUPPORTEDDEVICE:
      MediaError = MMPresTypes::eMediaError::MEDIAERROR_DEVICE_UNSUPPORTED;
      break;

   case eUNSUPPORTEDFILESYSTEM:
      MediaError = MMPresTypes::eMediaError::MEDIAERROR_FILESYSTEM_FORMAT_UNSUPPORTED;
      break;

   case eDEVICENORESPONSE:
      MediaError = MMPresTypes::eMediaError::MEDIAERROR_BUSY;
      break;
   default:
      break;
   }

   triggerMediaError(slotId, MediaError);
}

void MediaPlaybackEngine::masterResetEvent(const bool& factoryResetStatus)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::masterResetEvent: factoryResetStatus=%d", (int)factoryResetStatus);

   if(factoryResetStatus)
   {
     resetToFactory(std::bind(&MediaPlaybackEngine::resetToFactoryCallback, this, _1, _2));
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::masterResetEvent:not true");
   }
}

void MediaPlaybackEngine::factoryResetEvent(const bool& factoryResetStatus)
{
   if(factoryResetStatus)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::factoryResetEvent: factoryResetStatus=%d", (int)factoryResetStatus);
      resetToFactory(std::bind(&MediaPlaybackEngine::resetToMasterFactoryCallback, this, _1, _2));
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::factoryResetEvent:not true");
   }
}

void MediaPlaybackEngine::resetToFactory(MediaSettingProxyBase::ResetToFactoryAsyncCallback callback)
{
   if(mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::resetToFactory ");

      //call to CoC media resetToFactory method
      mMediaSetting->resetToFactoryAsync(callback);
   }
}

void MediaPlaybackEngine::resetToFactoryCallback(const CommonAPI::CallStatus& status, const BaseType::tBaseError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::resetToFactoryCallback: status=%d,error=%d", (int)(status), (int)error);

    //send masterResetEvent status to Mpres ford plugin so it can send back to VehicleSettings
    PresEventAbstraction::getInstance()->publish<eventMasterResetStatus>(REQ_MASTERRESET_STATUS, (BaseType::tBaseError::S_OK == error ? true : false));
}

void MediaPlaybackEngine::resetToMasterFactoryCallback(const CommonAPI::CallStatus& status, const BaseType::tBaseError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::resetToMasterFactoryCallback: status=%d,error=%d", (int)(status), (int)error);
}

void MediaPlaybackEngine::availibility(const CommonAPI::AvailabilityStatus& avail)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::availibility - playerProxy is %s", mPlayer.getStringAvailibility(avail).c_str());

   mPlayerProxyStatus = avail;
   if (mPlayerProxyStatus==CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      // if some default tasks are needed,perform them
      //startProxyTasks();
      FacadeHelper<MediaPresCore>()->setPlayerAvailable();
   }
   else
   {
      // what if proxy does not become available??
      // if some default tasks are needed,perform them
      stopProxyTasks();
   }
}

void MediaPlaybackEngine::startProxyTasks()
{
   if (mSubscribed == false)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::startProxyTasks");
      subscribeToPlayQueueChanged(true);
      subscribeToPlaybackStatus(true);
      subscribeToPlayError(true);
      subscribeToUpScaling(true);
      subscribeToSubtitle(true);
      subscribeToCurrentMediumId(true);
      subscribeToAudioChannel(true);
      subscribeToRestrictedMode(true);
      subscribeTodevicePlayStatusChange(true);
      subscribeToVideoPlayStatus(true);
      subscribeToplaybackSpeedChange(true);
      mSubscribed = true;
   }
}

void MediaPlaybackEngine::stopProxyTasks()
{
   if (mSubscribed == true)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::stopProxyTasks");
      subscribeToPlayQueueChanged(false);
      subscribeToPlaybackStatus(false);
      subscribeToPlayError(false);
      subscribeToUpScaling(false);
      subscribeToSubtitle(false);
      subscribeToCurrentMediumId(false);
      subscribeToAudioChannel(false);
      subscribeToRestrictedMode(false);
      subscribeTodevicePlayStatusChange(false);
      subscribeToVideoPlayStatus(false);
      subscribeToplaybackSpeedChange(false);
      mSubscribed = false;
   }
}


void MediaPlaybackEngine::subscribeToUpScaling(bool subscribe)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToUpScaling - subscribe: %d", subscribe);

   if(subscribe)
   {
      /* subscribe to UpScaling attribute changed event from MediaManager */
      mUpScaleToken = SUBSCRIBE(getPlayerProxy(), getAspectModeAttribute().getChangedEvent(), &MediaPlaybackEngine::onAspectModeAttributeChanged, _1)
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToUpScaling - subscription token: %d", mUpScaleToken);
   }
   else
   {
      /* un-subscribe to UpScale attribute changed event from MediaManager */
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::un-subscribeToUpScaling - subscription token: %d", mUpScaleToken);
      UNSUBSCRIBE(getPlayerProxy(), getAspectModeAttribute().getChangedEvent(), mUpScaleToken)
   }
}

void MediaPlaybackEngine::onAspectModeAttributeChanged(const PlayerTypes::AspectModeStatus& status )
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>>> MediaPlaybackEngine::onAspectModeAttributeChanged - AspectMode Status: %d", (int)status);

   bool Scaling;
   switch(status)
   {
   case PlayerTypes::AspectModeStatus::PRESERVE:
      Scaling = true;
      break;
   case PlayerTypes::AspectModeStatus::NATIVE:
      Scaling = false;
      break;
   default:
      break;
   }

   std::function<void(const bool&)> callback = getUpScaleStatusStubCallback();
   if(callback != nullptr)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::UpScale SetDisplayWindow Callback - firing fullscreenAttribute");
      callback(Scaling);
   }
}


void MediaPlaybackEngine::subscribeToSubtitle(bool subscribe)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToSubtitle - subscribe: %d", subscribe);

   if(subscribe)
   {
       /* subscribe to subtitle attribute changed event from MediaManager */
      mSubtitleToken = SUBSCRIBE(getPlayerProxy(), getSubtitleAttribute().getChangedEvent(), &MediaPlaybackEngine::onSubtitleStatusChanged, _1)
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToSubtitle - subscription token: %d", mSubtitleToken);
   }
   else
   {
      /* un-subscribe to subtitle attribute changed event from MediaManager */
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::un-subscribeToSubtitle - subscription token: %d", mSubtitleToken);
      UNSUBSCRIBE(getPlayerProxy(), getSubtitleAttribute().getChangedEvent(), mSubtitleToken)
   }
}

void MediaPlaybackEngine::onSubtitleStatusChanged(const PlayerTypes::SubtitleStatus& status)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>>> MediaPlaybackEngine::onSubtitleStatusChanged - subtitle status: %d", (int)status);

   MMPresTypes::eSubtitleStatus subtitleStatus = MMPresTypes::eSubtitleStatus::ON;

   // Hence the default value set in constructor is ON for "subtitleStatus", Its not required to set it explicitly again.
   if(status == PlayerTypes::SubtitleStatus::OFF)
      subtitleStatus = MMPresTypes::eSubtitleStatus::OFF;
#if 0
   switch(status)
   {
   case PlayerTypes::SubtitleStatus::ON:
      subtitleStatus = MMPresTypes::eSubtitleStatus::ON;
      break;

   case PlayerTypes::SubtitleStatus::OFF:
      subtitleStatus = MMPresTypes::eSubtitleStatus::OFF;
      break;
   default:
      break;
   }
#endif

   std::function<void(const MMPresTypes::eSubtitleStatus&)> callback = getSubtitleStatusStubCallback();
   if(callback != nullptr)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::onSubtitleStatusChanged - firing subtitleStatusAttribute");
       callback(subtitleStatus);
   }
}

void MediaPlaybackEngine::setSubtitleStatusStubCallBack(std::function<void(const MMPresTypes::eSubtitleStatus&)> func)
{
   mSubtitleStatusStubCallback = func;
}

std::function<void(const MMPresTypes::eSubtitleStatus&)>& MediaPlaybackEngine::getSubtitleStatusStubCallback()
{
   return mSubtitleStatusStubCallback;
}

void MediaPlaybackEngine::setUpScaleStatusStubCallBack(std::function<void(const bool&)> func)
{
   mUpScaleStatusStubCallback = func;
}

std::function<void(const bool&)>& MediaPlaybackEngine::getUpScaleStatusStubCallback()
{
   return mUpScaleStatusStubCallback;
}

void MediaPlaybackEngine::subscribeToAudioChannel(bool subscribe)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToAudioChannel - subscribe: %d", subscribe);

   if (subscribe)
   {
      /* subscribe to subtitle attribute changed event from MediaManager */
      mAudioChannelToken = SUBSCRIBE(getPlayerProxy(), getAudioChannelAttribute().getChangedEvent(), &MediaPlaybackEngine::onAudioChannelStatusChanged, _1)
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToAudioChannel - subscription token: %d", mAudioChannelToken);
   }
   else
   {
      /* un-subscribe to subtitle attribute changed event from MediaManager */
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::un-subscribeToAudioChannel - subscription token: %d", mAudioChannelToken);
      UNSUBSCRIBE(getPlayerProxy(), getAudioChannelAttribute().getChangedEvent(), mAudioChannelToken)
   }
}

void MediaPlaybackEngine::onAudioChannelStatusChanged(const PlayerTypes::AudioChannelStatus& status)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>>> MediaPlaybackEngine::onAudioChannelStatusChanged - AudioChannel status: %d", (int)status);


   MMPresTypes::eAudioChannelStatus AudioChannelStatus = MMPresTypes::eAudioChannelStatus::STEREO;

   switch(status)
   {
   case PlayerTypes::AudioChannelStatus::RIGHT:
      AudioChannelStatus = MMPresTypes::eAudioChannelStatus::RIGHT;
      break;

   case PlayerTypes::AudioChannelStatus::LEFT:
      AudioChannelStatus = MMPresTypes::eAudioChannelStatus::LEFT;
      break;

   case PlayerTypes::AudioChannelStatus::STEREO:
	   // Hence the default value set to "AudioChannelStatus" is STEREO, Its not required to set it again explicitly.
   default:
      break;
   }

   std::function<void(const MMPresTypes::eAudioChannelStatus&)> callback = getAudioChannelStatusStubCallback();

   if(callback != nullptr)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::onAudioChannelStatusChanged - firing AudioChannelStatusAttribute");
       callback(AudioChannelStatus);
   }
}

void MediaPlaybackEngine::setAudioChannelStatusStubCallBack(std::function<void(const MMPresTypes::eAudioChannelStatus&)> func)
{
   mAudioChannelStatusStubCallback = func;
}

std::function<void(const MMPresTypes::eAudioChannelStatus&)>& MediaPlaybackEngine::getAudioChannelStatusStubCallback()
{
   return mAudioChannelStatusStubCallback;
}


void MediaPlaybackEngine::subscribeToPlayQueueChanged(bool subscribe)
{
   static uint32_t token;
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToPlayQueueChanged - subscribe: %d", subscribe);
   if (subscribe)
   {
      token=SUBSCRIBE(mPlayer,getPlayQueueChangedEvent(),&MediaPlaybackEngine::playQueueChangedCallback)
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToPlayQueueChanged - subscription token: %d", token);
   }
   else
   {
      UNSUBSCRIBE(mPlayer,getPlayQueueChangedEvent(),token)
   }
}

void MediaPlaybackEngine::playQueueChangedCallback()//CommonAPI::WakeupCallback callback, std::function<void(CommonAPI::CallStatus)> listener)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::playQueueChangedCallback");
}

void MediaPlaybackEngine::subscribeToPlayError(bool subscribe)
{
   static uint32_t token;
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToPlayError - subscribe: %d", subscribe);
   if (subscribe)
   {
      token = SUBSCRIBE(mPlayer, getPlayErrorEvent(), &MediaPlaybackEngine::playErrorCallback, _1, _2);
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToPlayError - subscription token: %d", token);
   }
   else
   {
      UNSUBSCRIBE(mPlayer, getPlayErrorEvent(), token)
   }
}

void MediaPlaybackEngine::playErrorCallback(const uint32_t& mediumId, const PlayerTypes::PlayerError& error)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::playErrorCallback %d,  mediumId=%d",(int32_t)error, mediumId);
   auto it = mPlayerToMediaError.find(error);
   if (it != mPlayerToMediaError.end())
   {
      uint8_t slotId = mDetect->findMatchingSlot(mediumId);
      switch (it->second)
      {
         case MMPresTypes::eMediaError::MEDIAERROR_BACKEND_UNREACHABLE:
         {
           if (mDetect->getDeviceType(slotId) == MMPresTypes::eDeviceType::DEVICETYPE_MTP)
           {
               triggerMediaError(slotId,it->second);
           }
            break;
         }
         case MMPresTypes::eMediaError::MEDIAERROR_FILEUNAVAILABLE:
         {
            DeviceBase* currentActiveDevice = mDetect->getActiveDevice();
            if (currentActiveDevice != nullptr)
            {
               std::string lNowPlayingType= "";
               currentActiveDevice->getNowPlayingType(lNowPlayingType);
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::playErrorCallback : type %s", lNowPlayingType.c_str());
               if (lNowPlayingType.compare("iTunesRadio") == 0)
               {
                  it->second = MMPresTypes::eMediaError::MEDIAERROR_ITUNES_UNACCESSIBLE;
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::playErrorCallback : Error updated to MEDIAERROR_ITUNES_UNACCESSIBLE");
                  triggerMediaError(slotId, it->second);
               }
            }
            break;
         }
         default:
         {
            triggerMediaError(slotId, it->second);
            break;
         }
      }
   }
   else
   {
	   /* For some BTSA device where playback does not start and playState playing is not received from mediaone,
	    * medioane will send error as "ERROR_DEVICE_STATE" to mpres.
	    * Mpres will send success to audio manager so audio manager should not change source from BTSA to default one i.e. AM/FM
	    * Similarly mpres will send dummmy metadata to HMI so HMI can switch to BTSA nowPlaying metadata screen (empty one).
	    * For this mediaone error, mpres will not send any error to HMI. So this error will not be part of errorMap.
	    */
	   if(error == PlayerTypes::PlayerError::ERROR_DEVICE_STATE)
	   {
		   uint8_t slotId = mDetect->findMatchingSlot(mediumId);

		   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::playErrorCallback : activeDeviceId %d, deviceType=%d", mDetect->getActiveDeviceId(), mDetect->getDeviceType(slotId));
		   if((mediumId == mDetect->getActiveDeviceId()) && (mDetect->getDeviceType(slotId) == MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH))
		   {
			   FacadeHelper<SourceSwitch> mSourceSwitch;

			   /* resPlay of sourceSwitch already has mechanism of sending success to audio manager and sending dummy metadata to HMI
			    * This depends on whether device is BTSA & SourceSwitchTimerExpired flag is set.
			    * This flag is getting set when timer-expires by waiting for playState.
			    * For this case, flag is not set as error from mediaone (before timer-expiry), explicitly set this flag to use exisiting functionality.
			    * And once used, reset this flag here only.
			    */
			   mSourceSwitch->setSourceSwitchTimerExpired(true);

			   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::playErrorCallback - ERROR_DEVICE_STATE so BTSA source should remain active");
            mSourceSwitch->cancelTimer();
			   mSourceSwitch->resPlay(true);

			   mSourceSwitch->setSourceSwitchTimerExpired(false);
		   }
	   }
	   else
        LOG_INFO(LOG_CONTEXT_MEDIA, "Got invalid Playerror");
   }

}

//Audiobook/Podcast speed broadcast
void MediaPlaybackEngine::subscribeToplaybackSpeedChange(bool subscribe)
{
  static uint32_t token;
   LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToplaybackSpeedChange - subscribe: %d", subscribe);
   if (subscribe)
   {
	   token = SUBSCRIBE(mMediaSetting, getPlaybackSpeedChangeEvent(), &MediaPlaybackEngine::playbackSpeedChangeCallback, _1, _2);
      LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToplaybackSpeedChange - subscription token: %d", token);
   }
   else
   {
      UNSUBSCRIBE(mMediaSetting, getPlaybackSpeedChangeEvent(), token)
   }
}

void MediaPlaybackEngine::playbackSpeedChangeCallback(const BaseType::tAudioObjectType audioObjectType, const double& speed)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::playbackSpeedChangeCallback:speed=%f",speed);

   DeviceBase* pDevice = mDetect->getActiveDevice();
   MMPresTypes::ePlaybackSpeed playSpeed = MMPresTypes::ePlaybackSpeed::NORMAL;
   MMPresTypes::eMetadataType ContainerType = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;
  // uint64_t deviceid;
   MMPresTypes::tPlaybackSpeed mSpeedInfo;

   if(audioObjectType == BaseType::tAudioObjectType::PODCAST )
   {
	   ContainerType = MMPresTypes::eMetadataType::METADTATATYPE_PODCAST;
   }
   else if(audioObjectType == BaseType::tAudioObjectType::AUDIOBOOK)
   {
	   ContainerType = MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK;
   }
   else
   {
	   LOG_INFO(LOG_CONTEXT_MEDIA,"MediaPlaybackEngine::playbackSpeedChangeCallback - Not Matching Type");
   }

   if(pDevice != nullptr)
   {
      //ContainerType = pDevice->getContainerType(audioObjectType);//BrowseHelper::getItemMetadataType(audioObjectType);
      mSpeedInfo = pDevice->getSpeedInfoObj();
      //deviceid = mDetect->getActiveDeviceId();
   }
    switch (ContainerType)
	{
	case MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK:
	case MMPresTypes::eMetadataType::METADTATATYPE_CHAPTER:
	case MMPresTypes::eMetadataType::METADTATATYPE_PODCAST:
	case MMPresTypes::eMetadataType::METADTATATYPE_EPISODE:
		{
          LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::playbackSpeedChangeCallback - Podcast/Audiobook");
		  if(PAUSED_SPEED < speed)//speed is positive
		   {
			 if (FFWD_RATE_THRESHOLD_AP > speed)//within playing range - 0.5 to 1.5//FFWD_RATE_THRESHOLD_AP
			 {
			  if(speed > PLAYING_SPEED)
			   {
				 playSpeed = MMPresTypes::ePlaybackSpeed::FASTER;
			   }
			   else if(speed < PLAYING_SPEED)
			   {
				 playSpeed = MMPresTypes::ePlaybackSpeed::SLOWER;
			   }
			  }
		    }
		  }
		  break;
	      default:
		  {
		  }
		   break;
	      }

         LOG_INFO(LOG_CONTEXT_MEDIA,"MediaPlaybackEngine::playbackSpeedChangeCallback - ContainerType:%d",ContainerType);
         mSpeedInfo.setSlotId(mDetect->findMatchingSlot((uint32_t)mDetect->getActiveDeviceId()));
	     mSpeedInfo.setType(ContainerType);
	     mSpeedInfo.setPlaybackSpeed(playSpeed);
         std::function<void(const MMPresTypes::tPlaybackSpeed&)> speedInfoCallback = getSpeedInfoStubCallback();
	     if(speedInfoCallback != nullptr)
	      {
		     speedInfoCallback(mSpeedInfo);
	      }
	      else
	      {
		     LOG_INFO(LOG_CONTEXT_MEDIA,"MediaPlaybackEngine::playbackSpeedChangeCallback - speedInfoCallback is null");
	      }
       LOG_INFO(LOG_CONTEXT_MEDIA,"MediaPlaybackEngine::playbackSpeedChangeCallback - playSpeed:%d",playSpeed);
}



CommonAPI::AvailabilityStatus MediaPlaybackEngine::getPlayerProxyStatus()
{
   return mPlayerProxyStatus;
}

void MediaPlaybackEngine::resume(uint32_t ClientID, PlayerProxyBase::PlayAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::resume");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, playAsync)
   }
}

void MediaPlaybackEngine::pause(uint32_t ClientID, PlayerProxyBase::PauseAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::pause");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, pauseAsync)
   }
}

void MediaPlaybackEngine::stop(uint32_t ClientID, PlayerProxyBase::StopAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::stop");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, stopAsync)
   }
}

void MediaPlaybackEngine::nextChapter(uint32_t ClientID, PlayerProxyBase::NextChapterAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::nextChapter");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, nextChapterAsync)
   }
}

void MediaPlaybackEngine::previousChapter(uint32_t ClientID, PlayerProxyBase::PreviousChapterAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::previousChapter");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, previousChapterAsync)
   }
}

void MediaPlaybackEngine::next(uint32_t ClientID, PlayerProxyBase::NextAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::next++");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, nextAsync)
   }
}

void MediaPlaybackEngine::previous(uint32_t ClientID, PlayerProxyBase::PreviousAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::previous");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, previousAsync)
   }
}

void MediaPlaybackEngine::seekToTime(uint32_t ClientID, uint64_t pos, PlayerProxyBase::SetPositionAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::seekToTime-pos=%llu", pos);
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, setPositionAsync, pos, )
   }
}

void MediaPlaybackEngine::seek(uint32_t ClientID, const int64_t pos, PlayerProxyBase::SeekAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::seek-pos=%d", pos);
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, seekAsync, pos, )
   }
}



void MediaPlaybackEngine::setRepeat(uint32_t ClientID, PlayerTypes::RepeatStatus repeat, std::function<void(const CommonAPI::CallStatus&, const PlayerTypes::RepeatStatus&)> callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setRepeat");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_REPEAT_RESPONSE, ClientID, callback, getRepeatAttribute().setValueAsync, repeat, )
   }
}

void MediaPlaybackEngine::setSubtitle(uint32_t ClientID, PlayerTypes::SubtitleStatus subtitle, std::function<void(const CommonAPI::CallStatus&, const PlayerTypes::SubtitleStatus&)> callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setSubtitle");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_SUBTITLE_RESPONSE, ClientID, callback, getSubtitleAttribute().setValueAsync, subtitle, )
   }
}

void MediaPlaybackEngine::setAudioChannel(uint32_t ClientID, PlayerTypes::AudioChannelStatus audioChannel, std::function<void(const CommonAPI::CallStatus&, const PlayerTypes::AudioChannelStatus&)> callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setAudioChannel");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_AUDIOCHANNEL_RESPONSE, ClientID, callback, getAudioChannelAttribute().setValueAsync, audioChannel, )
   }
}

void MediaPlaybackEngine::setDisplayWindow(uint32_t ClientID, bool _fullscreen, MMPresTypes::tScreen screeenDimension, std::function<void(const CommonAPI::CallStatus&, const PlayerTypes::AspectModeStatus&)> callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      PlayerTypes::AspectModeStatus UpScale = PlayerTypes::AspectModeStatus::PRESERVE;
      if(!_fullscreen)
         UpScale = PlayerTypes::AspectModeStatus::NATIVE;

      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setDisplayWindow- left = %d, top = %d, right = %d, bottom = %d ", _rect.getLeft(), _rect.getTop(), _rect.getRight(), _rect.getBottom());
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_UPSCALE_RESPONSE, ClientID, callback, getAspectModeAttribute().setValueAsync, UpScale, )
   }
}

void MediaPlaybackEngine::setShuffle(uint32_t ClientID, PlayerTypes::ShuffleStatus shuffle, std::function<void(const CommonAPI::CallStatus&, const PlayerTypes::ShuffleStatus&)> callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setShuffle");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_SHUFFLE_RESPONSE, ClientID, callback, getShuffleAttribute().setValueAsync, shuffle, )
   }
}

void MediaPlaybackEngine::fastForward(uint32_t ClientID, double rate, std::function<void(const CommonAPI::CallStatus& status, const double& rate)> callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::fastForward");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_SPEED_RESPONSE, ClientID, callback, getRateAttribute().setValueAsync, rate, )
   }
}

void MediaPlaybackEngine::fastReverse(uint32_t ClientID,double rate, std::function<void(const CommonAPI::CallStatus& status, const double& rate)> callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::fastReverse");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_SPEED_RESPONSE, ClientID, callback, getRateAttribute().setValueAsync, rate, )
   }
}

void MediaPlaybackEngine::setVolume(uint32_t ClientID,double vol, std::function<void(const CommonAPI::CallStatus& status, const double& vol)> callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setVolume");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_VOLUME_RESPONSE, ClientID, callback, getVolumeAttribute().setValueAsync, vol, )
   }
}

void MediaPlaybackEngine::playDevice(uint32_t ClientID, std::string deviceUri, PlayerProxyBase::OpenUriAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::playDevice");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, openUriAsync, deviceUri, )
   }
}

void MediaPlaybackEngine::enqueueUri(uint32_t ClientID, std::string itemUri, PlayerProxyBase::EnqueueUriAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::enqueueUri");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, enqueueUriAsync, itemUri, )
   }
}

void MediaPlaybackEngine::dequeAll(uint32_t ClientID, PlayerProxyBase::DequeueAllAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::dequeAll");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, dequeueAllAsync)
   }
}

void MediaPlaybackEngine::dequeIndex(uint32_t ClientID, uint64_t index, PlayerProxyBase::DequeueIndexAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::dequeIndex");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, dequeueIndexAsync, index, )
   }
}

void MediaPlaybackEngine::getCurrentPlayQueue(uint32_t ClientID, PlayerProxyBase::GetCurrentPlayQueueAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::getCurrentPlayQueue");
      ASYNC_INVOKE_3ARG(mPlayer, PlayerProxy, ARG_LIST_GETCURRENTPLAYQUEUE_CALLBACK, ClientID, callback, getCurrentPlayQueueAsync)
   }
}

void MediaPlaybackEngine::getCurrentPlayQueueEx(uint32_t ClientID, uint64_t offset, uint64_t count, PlayerProxyBase::GetCurrentPlayQueueExAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::getCurrentPlayQueueEx");
      ASYNC_INVOKE_3ARG(mPlayer, PlayerProxy, ARG_LIST_GETCURRENTPLAYQUEUE_CALLBACK, ClientID, callback, getCurrentPlayQueueExAsync, offset, count, )
   }
}

void MediaPlaybackEngine::setRate(uint32_t ClientID, double rate, std::function<void(const CommonAPI::CallStatus& status, const double& rate)> callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setRate");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_SPEED_RESPONSE, ClientID, callback, getRateAttribute().setValueAsync, rate, )
   }
}

void MediaPlaybackEngine::sendIAP2KeyEvent(uint32_t ClientID, ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType buttonType, bool pressed, PlayerProxyBase::SendIAP2KeyEventAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::sendIAP2KeyEvent");
      //ASYNC_INVOKE_3ARG(mPlayer, PlayerProxy, ARG_LIST_IAP2KEY_CALLBACK, ClientID, callback, sendIAP2KeyEventAsync, (::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType)buttonType, pressed, )
      //sendIAP2KeyEventAsync(const ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType &_buttonType, const bool &_pressed, SendIAP2KeyEventAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
      mPlayer->sendIAP2KeyEventAsync(buttonType, pressed, callback);
   }
}

void MediaPlaybackEngine::setPlaybackSpeed(uint32_t ClientID,::v1::com::harman::media::BaseType::tAudioObjectType _type, double _speed, MediaSettingProxyBase::SetPlaybackSpeedAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setPlaybackSpeed");
   //	ASYNC_INVOKE_2ARG(mMediaSetting, MediaSettingProxy, ARG_LIST_SETPLAYBACKSPEED_RESPONSE, ClientID, callback, setPlaybackSpeedAsync, _type, _speed, )
      mMediaSetting->setPlaybackSpeedAsync(_type, _speed, callback);
   }
}

void MediaPlaybackEngine::getPlaybackSpeed(uint32_t ClientID, ::v1::com::harman::media::BaseType::tAudioObjectType _type, MediaSettingProxyBase::GetPlaybackSpeedAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::getPlaybackSpeed");
   //	ASYNC_INVOKE_3ARG(mMediaSetting, MediaSettingProxy, ARG_LIST_GETPLAYBACKSPEED_RESPONSE, ClientID, callback, getPlaybackSpeedAsync, _type, )
      mMediaSetting->getPlaybackSpeedAsync(_type, callback);
   }
}


void MediaPlaybackEngine::promote(uint32_t ClientID, std::string& uri, PlayerProxyBase::PromoteAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::promote");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, promoteAsync, uri ,)
   }
}

void MediaPlaybackEngine::demote(uint32_t ClientID, std::string& uri, PlayerProxyBase::DemoteAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::demote");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, demoteAsync, uri, )
   }
}

void MediaPlaybackEngine::addToWishList(uint32_t ClientID, std::string& uri, PlayerProxyBase::AddToWishListAsyncCallback callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::addToWishList");
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_PLAYBACK_RESPONSE, ClientID, callback, addToWishListAsync , uri, )
   }
}

void MediaPlaybackEngine::setTracklistSupportStubCallBack(std::function<void(const MMPresTypes::tTrackListSupport&)> func)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setTracklistSupportStubCallBack");
   mtrackListSupportStubCallback = func;
}

void MediaPlaybackEngine::setPromoteStatusStubCallBack(std::function<void(const bool&)> func)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setPromoteStatusStubCallBack");
   mPromoteStatusStubCallBack = func;
}

void MediaPlaybackEngine::setDemoteStatusStubCallBack(std::function<void(const bool&)> func)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setDemoteStatusStubCallBack");
   mDemoteStatusStubCallBack = func;
}

void MediaPlaybackEngine::setAddToWishListStatusStubCallBack(std::function<void(const bool&)> func)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setAddToWishListStatusStubCallBack");
   mAddToWishListStatusStubCallBack = func;
}


void MediaPlaybackEngine::setActiveDeviceBTLinkLossCallBack(std::function<void(const bool&)> func)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setActiveDeviceBTLinkLossCallBack");
   mAudioInterface->setActiveDeviceBTLinkLossStubCallback(func);
}

void MediaPlaybackEngine::setPlaystateStubCallBack(std::function<void(const MMPresTypes::ePlayState&)> func)
{
   mPlayStateStubCallback = func;
}

void MediaPlaybackEngine::setSpeedInfoStubCallback(std::function<void(const MMPresTypes::tPlaybackSpeed&)> func)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setSpeedInfoStubCallback");
   mSpeedInfoStubCallback = func;
}

void MediaPlaybackEngine::setTrackStateStubCallBack(std::function<void(const MMPresTypes::tTrackState&)> func)
{
    mTrackStateStubCallback = func;
}

void MediaPlaybackEngine::setNowPlayingDataStubCallBack(std::function<void(const MMPresTypes::tNowPlayingInfo&)> func)
{
   mNowPlayingDataStubCallback = func;
}

void MediaPlaybackEngine::setCurrentPlayTimeStubCallBack(std::function<void(const uint64_t&)> func)
{
   mCurrentTimeStubCallback = func;
}

void MediaPlaybackEngine::setTotalPlayDurationStubCallBack(std::function<void(const uint64_t&)> func)
{
   mTotalDurationStubCallback = func;
}

void MediaPlaybackEngine::setRemotePlayerChangedStubCallBack(std::function<void(const uint32_t&)> func)
{
   mRemotePlayerChangedStubCallback = func;
}

void MediaPlaybackEngine::setRepeatModeStubCallBack(std::function<void(const MMPresTypes::eRepeatMode&)> func)
{
   mRepeatModeStubCallback = func;
}

void MediaPlaybackEngine::setShuffleModeStubCallBack(std::function<void(const MMPresTypes::eShuffleMode&)> func)
{
   mShuffleModeStubCallback = func;
}

void MediaPlaybackEngine::setTrackSeekSpeedStubCallBack(std::function<void(const MMPresTypes::eTrackSeekSpeed&)> func)
{
   mTrackSeekSpeedStubCallback = func;
}

void MediaPlaybackEngine::setArtworkStatusStubCallback(std::function<void(const MMPresTypes::eArtWorkAvailabilityStatus&)> func)
{
   mArtworkStatusStubCallback = func;
}

void MediaPlaybackEngine::setRemotePlayerCapabilityStubCallback(std::function<void(const MMPresTypes::tRemotePlayerCapability&)> func)
{
   mRemotePlayerCapabilityStubCallback = func;
}

void MediaPlaybackEngine::setVideoPlayStatusCallBack(std::function<void(const bool&)> func)
{
   mVideoPlayStatusCallback = func;
}

void MediaPlaybackEngine::setSyncStatusStubCallback(std::function<void(const MMPresTypes::SyncMap&)> func)
{
   mSyncStatusStubCallback = func;
}

void MediaPlaybackEngine::setMediaErrorStubCallback(std::function<void(const MMPresTypes::MediaErrorMap&)> func)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::setMediaErrorStubCallback:-");
   mMediaErrorStubCallback = func;
}

std::function<void(const MMPresTypes::tTrackListSupport&)>& MediaPlaybackEngine::getTracklistSupportStubCallBack()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::getTracklistSupportStubCallBack");
    KPIMARKER(306, "AV_HMI_UPDATE_trackListSupport");
   return mtrackListSupportStubCallback;
}

std::function<void(const bool&)>& MediaPlaybackEngine::getPromoteStatusStubCallBack()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::getPromoteStatusStubCallBack");
   return mPromoteStatusStubCallBack;
}

std::function<void(const bool&)>& MediaPlaybackEngine::getDemoteStatusStubCallBack()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::getDemoteStatusStubCallBack");
   return mDemoteStatusStubCallBack;
}


std::function<void(const bool&)>& MediaPlaybackEngine::getAddToWishListStatusStubCallBack()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::getAddToWishListStatusStubCallBack");
   return mAddToWishListStatusStubCallBack;
}


std::function<void(const MMPresTypes::ePlayState&)>& MediaPlaybackEngine::getPlayStateStubCallback()
{
    KPIMARKER(306, "AV_HMI_UPDATE_PlayState");
   return mPlayStateStubCallback;
}

std::function<void(const MMPresTypes::tPlaybackSpeed&)>& MediaPlaybackEngine::getSpeedInfoStubCallback()
{
   KPIMARKER(306, "AV_HMI_UPDATE_SpeedState");
   return mSpeedInfoStubCallback;
}

std::function<void(const MMPresTypes::tTrackState&)>& MediaPlaybackEngine::getTrackStateStubCallback()
{
   KPIMARKER(306, "AV_HMI_UPDATE_TrackState");
   return mTrackStateStubCallback;
}

std::function<void(const MMPresTypes::tNowPlayingInfo&)>& MediaPlaybackEngine::getNowPlayingDataStubCallback()
{
    KPIMARKER(306, "AV_HMI_UPDATE_NowPlayingData");
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::getNowPlayingDataStubCallback - currentMediumID: %d", mCurrentMediumId);

   return mNowPlayingDataStubCallback;
}

std::function<void(const MMPresTypes::eRepeatMode&)>& MediaPlaybackEngine::getRepeatModeStubCallback()
{
   return mRepeatModeStubCallback;
}

std::function<void(const MMPresTypes::eShuffleMode&)>& MediaPlaybackEngine::getShuffleModeStubCallback()
{
   return mShuffleModeStubCallback;
}

std::function<void(const uint64_t&)>& MediaPlaybackEngine::getCurrentTimeStubCallback()
{
   return mCurrentTimeStubCallback;
}

std::function<void(const uint64_t&)>& MediaPlaybackEngine::getTotalDurationStubCallback()
{
   return mTotalDurationStubCallback;
}

std::function<void(const uint32_t&)>& MediaPlaybackEngine::getRemotePlayerChangedStubCallback()
{
   return mRemotePlayerChangedStubCallback;
}

std::function<void(const MMPresTypes::eTrackSeekSpeed&)>& MediaPlaybackEngine::getTrackSeekSpeedStubCallback()
{
   KPIMARKER(306, "AV_HMI_UPDATE_SeekSpeed");
   return mTrackSeekSpeedStubCallback;
}

std::function<void(const MMPresTypes::eArtWorkAvailabilityStatus&)>& MediaPlaybackEngine::getArtworkStatusCallback()
{
   return mArtworkStatusStubCallback;
}

std::function<void(const MMPresTypes::tRemotePlayerCapability&)>& MediaPlaybackEngine::getRemotePlayerCapabilityStubCallback()
{
   return mRemotePlayerCapabilityStubCallback;
}

std::function<void(const MMPresTypes::SyncMap&)>& MediaPlaybackEngine::getSyncStatusStubCallback()
{
    KPIMARKER(306, "AV_HMI_UPDATE_SyncStatus");
   return mSyncStatusStubCallback;
}
std::function<void(const bool&)>& MediaPlaybackEngine::getVideoPlayStatusCallback()
{
    KPIMARKER(306, "AV_HMI_UPDATE_VideoPlayStatus");
   return mVideoPlayStatusCallback;
}


std::function<void(const MMPresTypes::MediaErrorMap&)>& MediaPlaybackEngine::getMediaErrorStubCallback()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::getMediaErrorStubCallback - mMediaErrorStubCallback:");
   return mMediaErrorStubCallback;
}

void MediaPlaybackEngine::subscribeToPlaybackStatus(bool subscribe)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToPlaybackStatus - subscribe: %d", subscribe);
   if (subscribe)
   {
      mPlaybackStatusToken = SUBSCRIBE(getPlayerProxy(), getPlaybackStatusAttribute().getChangedEvent(), &MediaPlaybackEngine::onPlaybackStatusChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToPlaybackStatusUpdate - subscription token: %d", mPlaybackStatusToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::un-subscribeToPlaybackStatusUpdate - subscription token: %d", mPlaybackStatusToken);
      UNSUBSCRIBE(getPlayerProxy(), getPlaybackStatusAttribute().getChangedEvent(), mPlaybackStatusToken)
   }
}

void MediaPlaybackEngine::onPlaybackStatusChanged(const PlayerTypes::PlaybackStatus& status)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::onPlaybackStatusChanged - playback status: %d", (int)status);
   FacadeHelper<MediaPresSettings> mSettings;
   FacadeHelper<SourceSwitch> mSourceSwitch;
   MMPresTypes::ePlayState value;
   DeviceBase* tmpActiveDevice = mDetect->getActiveDevice();

   switch (status)
   {
   case PlayerTypes::PlaybackStatus::PAUSED:
      value = MMPresTypes::ePlayState::PLAYSTATE_PAUSED;
      if (mSettings->getAMInteraction())
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::onPlaybackStatusChanged - raise onPaused event");

         if ((tmpActiveDevice != nullptr)&&( mDetect ->getActiveDeviceId() != REMOTECD_DEVICE_ID ))
         {
            /* Below condition is to support video SPSS requirement.
            * 4.4.2.15 VP-FUR-REQ-234537/A-Resume Operation for Video Playback
            * If the Video Player was paused before the power state changed away from Infotainment, then video playback shall remain paused.
            * After ignition cycle complete, mediaone will send video playback status as Pause and
            * check is added if playState is anything other than playing. This is for deep sleep and normal sleep.
            * Hence accordingly checks are added to support this requirement.
            * Once ONLY these checks are satisfied, send ok status to Audio Manager within 10 sec, so Audio manager will not change source to AM/FM.
            */
            if (tmpActiveDevice->getVideoPlayStatus() && (tmpActiveDevice->getPlayState() != MMPresTypes::ePlayState::PLAYSTATE_PLAYING))
            {
               /* for this requirement, video playback will be in pause state after ignition cycle.
               * So set "retainPauseStateForDevice" as True so have further normal processing.
               */
               mSourceSwitch->SetretainPauseStateForDevice(true);
               mSourceSwitch->cancelTimer();
               mSourceSwitch->resPlay(true);
            }
         }

      } //if (mSettings->getAMInteraction())
      break;
   case PlayerTypes::PlaybackStatus::PLAYING:
      value = MMPresTypes::ePlayState::PLAYSTATE_PLAYING;
      mSourceSwitch->SetretainPauseStateForDevice(false);
      if (mSettings->getAMInteraction())
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::onPlaybackStatusChanged - raise onPlaying event");
         mSourceSwitch->cancelTimer();
         mSourceSwitch->resPlay(true);
         //	(mSourceSwitch->getSCI_Media())->raise_onPlaying();
         //	mSourceSwitch->runCycle();
      }
      break;
   case PlayerTypes::PlaybackStatus::STOPPED:
      value = MMPresTypes::ePlayState::PLAYSTATE_STOPPED;
      if (mSettings->getAMInteraction())
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::onPlaybackStatusChanged - raise onStopped event");
         //		(mSourceSwitch->getSCI_Media())->raise_onStopped();
         //	mSourceSwitch->runCycle();
      }
      break;
   default:
      value = MMPresTypes::ePlayState::PLAYSTATE_UNKNOWN;
      mPlayState = value;
      break;
   }

   if ((tmpActiveDevice != nullptr)&&(mDetect -> getActiveDeviceId()!= REMOTECD_DEVICE_ID ))
   {
      tmpActiveDevice->cancelTimers();
      tmpActiveDevice->setPlayState(value);
      std::function<void(const MMPresTypes::ePlayState&)> callback = getPlayStateStubCallback();
      if (callback != nullptr)
      {
         callback(value);
      }
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "No Active device, hence Playstate is not updated!!");
   }
}

void MediaPlaybackEngine::subscribeToCurrentMediumId(bool subscribe)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToCurrentMediumId - subscribe: %d", subscribe);

    if(subscribe)
    {
       mCurrentMediumIdToken = SUBSCRIBE(getPlayerProxy(), getCurrentMediumIdAttribute().getChangedEvent(), &MediaPlaybackEngine::onCurrentMediumIdChanged, _1)
       LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToCurrentMediumId - subscription token: %d", mCurrentMediumIdToken);
    }
    else
    {
       LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::un-subscribeToCurrentMediumId - subscription token: %d", mCurrentMediumIdToken);
       UNSUBSCRIBE(getPlayerProxy(), getCurrentMediumIdAttribute().getChangedEvent(), mCurrentMediumIdToken)
    }
}

void MediaPlaybackEngine::onCurrentMediumIdChanged(const uint32_t& currentMediumId)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::onCurrentMediumIdChanged-currentMediumId: %d", currentMediumId);
    mCurrentMediumId = currentMediumId;
}

uint32_t MediaPlaybackEngine::getCurrentMediumId()
{
   return mCurrentMediumId;
}

MMPresTypes::ePlayState MediaPlaybackEngine::getPlayState()
{
   return mPlayState;
}

const MMPresTypes::eSyncState MediaPlaybackEngine::getSyncStatus(const uint8_t slotId)
{
   MMPresTypes::eSyncState lSyncState= MMPresTypes::eSyncState::STATE_UNKNOWN;
   auto it = mSyncMap.find(slotId);
   if (it != mSyncMap.end())
   {
      lSyncState = it->second;
   }
   return lSyncState;
}


void MediaPlaybackEngine::updateSlotStateMap(uint8_t slotId, MMPresTypes::eSyncState state)
{
   auto it = mSyncMap.find(slotId);
   if (it != mSyncMap.end())
   {
      it->second = state;
   }
   else
   {
      mSyncMap.insert(std::pair<uint8_t, MMPresTypes::eSyncState>(slotId, state));
   }

   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::SyncStatusChanged - slotid=%d,syncstate=%d", slotId, (int)state);
   if (mSyncStatusStubCallback != nullptr)
   {
      mSyncStatusStubCallback(mSyncMap);
   }
}

void MediaPlaybackEngine::triggerMediaError(uint8_t slotId, MMPresTypes::eMediaError error)
{
   auto it = mMediaErrorMap.find(slotId);
   if (it != mMediaErrorMap.end())
   {
      it->second = error;
   }
   else
   {
      mMediaErrorMap.insert(std::pair<uint8_t, MMPresTypes::eMediaError>(slotId, error));
   }
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::Mediaerror triggered - slotid=%d,error=%d", slotId, (int)error);
    if (mMediaErrorStubCallback!=nullptr)
    {
        mMediaErrorStubCallback(mMediaErrorMap);
    }
    mMediaErrorMap.clear();
}

void MediaPlaybackEngine::intimatePlayEngineForCleanup(uint32_t mediumId)
{
   uint8_t temp_slotId = mDetect->findMatchingSlot(mediumId);
   
   auto it = mSyncMap.find(temp_slotId);
   if (it != mSyncMap.end())
   {
      mSyncMap.erase(it);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "invalid slot id1");
   }

   auto it1 = mMediaErrorMap.find(temp_slotId);
   if (it1 != mMediaErrorMap.end())
   {
      mMediaErrorMap.erase(it1);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "invalid slot id2");
   }
}

/* This event is received from mpres plugin to mpres module
 * This is for sending driverRestrictionsVideo status which is received by mpres plugin from vehicleSettings
 */
void MediaPlaybackEngine::driverRestrictionsVideoStatusEvent(const bool& driverRestrictionsVideoStatus)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::driverRestrictionsVideoStatusEvent: driverRestrictionsVideoStatus=%d", (int)driverRestrictionsVideoStatus);

   /* Save driverRestrictionsVideo status coming from mpres plugin to member variable of MediaPlaybackEngine for further usage */
   setDriverRestrictionsVideoStatus(driverRestrictionsVideoStatus);

   DeviceBase* pDevice = mDetect->getActiveDevice();
   if(pDevice != nullptr)
   {
      /* Media device is active source at this moment
       * So directly update driverRestrictionsVideo status to CoC media
       * And set mDriverRestrictionsVideoStatusInformedToCoC as true
       */
       LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::driverRestrictionsVideoStatusEvent - active device so setting restrictedMode to CoC");
      pDevice->setRestrictedMode();
      setDriverRestrictionsVideoStatusInformedToCoC(true);
   }
   else
   {
      /* Media device is not active source at this moment.
       * So do not update driverRestricted mode to CoC media.
       * This should be done in onMetadaChanged event from CoC media when one of the media device becomes active source
       * Hence here set mDriverRestrictionsVideoStatusInformedToCoC as false
       */
       LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::driverRestrictionsVideoStatusEvent - No active device");
      setDriverRestrictionsVideoStatusInformedToCoC(false);
   }
}

bool MediaPlaybackEngine::getDriverRestrictionsVideoStatus()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::getDriverRestrictionsVideoStatus: status=%d", (int)mDriverRestrictionsVideoStatus);
   return mDriverRestrictionsVideoStatus;
}

/* This API saves driverRestrcitionsVideo status value received from vehicleSettings to mpres plugin */
void MediaPlaybackEngine::setDriverRestrictionsVideoStatus(bool status)
{
   mDriverRestrictionsVideoStatus = status;
}

void MediaPlaybackEngine::subscribeToRestrictedMode(bool subscribe)
{
   LOG_DEBUG(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToRestrictedMode - subscribe: %d", subscribe);
   if (subscribe)
   {
      /* subscribe to restrictedMode attribute of CoC media */
      mRestrictedModeToken = SUBSCRIBE(getPlayerProxy(), getRestrictedModeAttribute().getChangedEvent(), &MediaPlaybackEngine::onRestrictedModeChanged, _1)
      LOG_DEBUG(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToRestrictedMode - subscription token: %d", mRestrictedModeToken);
   }
   else
   {
      LOG_DEBUG(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::un-subscribeToRestrictedMode - subscription token: %d", mRestrictedModeToken);
      UNSUBSCRIBE(getPlayerProxy(), getRestrictedModeAttribute().getChangedEvent(), mRestrictedModeToken)
   }
}

/* Below API is for attribute value changed notification from CoC media */
void MediaPlaybackEngine::onRestrictedModeChanged(const PlayerTypes::RestrictedModeStatus& status)
{
   LOG_DEBUG(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::onRestrictedModeChanged - status: %d", (int)status);
}

bool MediaPlaybackEngine::getDriverRestrictionsVideoStatusInformedToCoC()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::getDriverRestrictionsVideoStatusInformedToCoC: value=%d", (int)mDriverRestrictionsVideoStatusInformedToCoC);
   return mDriverRestrictionsVideoStatusInformedToCoC;
}

void MediaPlaybackEngine::setDriverRestrictionsVideoStatusInformedToCoC(bool status)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::setDriverRestrictionsVideoStatusInformedToCoC: value=%d", (int)mDriverRestrictionsVideoStatusInformedToCoC);
    mDriverRestrictionsVideoStatusInformedToCoC = status;
}


void MediaPlaybackEngine::setRestrictedMode(uint32_t ClientID, std::function<void(const CommonAPI::CallStatus& status, const PlayerTypes::RestrictedModeStatus& value)> callback)
{
   if (mPlayerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      PlayerTypes::RestrictedModeStatus restrictedMode = PlayerTypes::RestrictedModeStatus::OFF;

      /* get driverRestrictionsVideo status which was received from vehicleSetting.
       * If value is "true", enable restrictedMode of CoC media
       * Else, disable restrictedMode of CoC media
       */
      if (getDriverRestrictionsVideoStatus())
      {
         restrictedMode = PlayerTypes::RestrictedModeStatus::ON;
      }
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPlaybackEngine::setRestrictedMode: restrictedMode=%d", (int)restrictedMode);
      ASYNC_INVOKE_2ARG(mPlayer, PlayerProxy, ARG_LIST_RESTRICTEDMODE_RESPONSE, ClientID, callback, getRestrictedModeAttribute().setValueAsync, restrictedMode, )
    }
}

void MediaPlaybackEngine::subscribeTodevicePlayStatusChange(bool subscribe)
{
    static uint32_t token;
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeTodevicePlayStatusChange - subscribe: %d", subscribe);
    if (subscribe)
    {
        token = SUBSCRIBE(mPlayer, getDevicePlayStatusChangeEvent(), &MediaPlaybackEngine::devicePlayStatusCallback, _1, _2, _3);
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeTodevicePlayStatusChange - subscription token: %d", token);
    }
    else
    {
        UNSUBSCRIBE(mPlayer, getDevicePlayStatusChangeEvent(), token)
    }
}

void MediaPlaybackEngine::devicePlayStatusCallback(const uint32_t& mediumId, PlayerTypes::PlaybackStatus status,std::string uri)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::devicePlayStatusCallback - mediumId: %d status = %d ,uri= %s ", mediumId,status,uri.c_str());

    int8_t currentSlotID = mDetect->findMatchingSlot(mediumId);
    MMPresTypes::tSlotList mSlotList = mDetect->getSlotList();
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::devicePlayStatusCallback-currentMediumId: %d mSlotList.size() = %d currentSlotID = %d ,mDetect->getbTimerTimeExpired() =%d", mediumId,mSlotList.size(),currentSlotID,mDetect->getbTimerTimeExpired());

    if ((0 != mediumId))
    {
    	DeviceBase* temp_device = mDetect->getDevice((uint64_t)mediumId);
    	if (nullptr != temp_device)
    	{
         ::v3::org::genivi::am_t::am_sourceID_t targetSourceID = am_t::am_sourceID_e::eSRC_UNKNOWN;
         if(temp_device->getDeviceType() == MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH)
         {
         	targetSourceID = am_t::am_sourceID_e::eSRC_BT_A2DP;
         	//Based on Device targetSourceID will be updating
         }
         else
         {
         	//DO Nothing
         }
         if ((status == MMPresTypes::ePlayState::PLAYSTATE_PLAYING)&& (targetSourceID!=am_t::am_sourceID_e::eSRC_UNKNOWN) && (mediumId!=mDetect->getActiveDeviceId   ()))
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::devicePlayStatusCallback - getbTimerTimeExpired: %d", mDetect->getbTimerTimeExpired());
            if (true == mAudioInterface->getPerformBTSASourceSwitch()) /*check for deviceplay status already recevied and dosource Switch */
            {
                LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::devicePlayStatusCallback calling do source Swith up on Receing the devicePlayStatus after VR Connection Release Bug use case");
                mAudioInterface->doSourceSwitch(targetSourceID, am_t::am_sinkID_e::eSNK_SPKS);
            }
            else if(false==mDetect->getbTimerTimeExpired())
            {
               //mAudioInterface->doSourceSwitch(targetSourceID, am_t::am_sinkID_e::eSNK_SPKS);
               LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::devicePlayStatusCallback mDetect->getVRmodeActive(): %d", mDetect->getVRmodeActive());
               if(true == mDetect->getVRmodeActive())
               {
                  mDetect->cancelTimer();
               }
               else
               {
                  LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::devicePlayStatusCallback setPerformBTSASourceSwitch normal use case");
                  mAudioInterface->setPerformBTSASourceSwitch(true);
               }
            }
        }
        else
        {
            // Do Nothing
        }
       }
    }
    else
    {
    	LOG_INFO(LOG_CONTEXT_MEDIA, "Invalid Current Medium ID");
    }

}

void MediaPlaybackEngine::subscribeToVideoPlayStatus(bool subscribe)
{
    static uint32_t token=0;
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToVideoPlayStatus - subscribe: %d", subscribe);
    if (subscribe)
    {
        token = SUBSCRIBE(mPlayer, getVideoPlayStatusAttribute().getChangedEvent(), &MediaPlaybackEngine::onVideoPlayStatusChanged, _1);
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPlaybackEngine::subscribeToVideoPlayStatus - subscription token: %d", token);
    }
    else
    {
        UNSUBSCRIBE(mPlayer, getVideoPlayStatusAttribute().getChangedEvent(), token)
    }
}

void MediaPlaybackEngine::onVideoPlayStatusChanged(const bool&  status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::onVideoPlayStatusChanged - status=%d", status);

	/* Resolution for Elvis ticket 2225991. CoC media does not update mVideoPlayStatus properly
	 * So below is not required and same stuff is done in onMetadataChanged() api depending upon audio & video playback
	 */
	//mVideoPlayStatus = status;

	/* Use "mVideoPlaybackStatusFromCoC" in onMetadataChanged api to send "nowPlaying" event to HMI for video playback.
	 * CoC media has done workaround to send "onMetadataChanged" event again once "videoPlayStatus" becomes true.
	 * So if player type is Video and mVideoPlaybackStatusFromCoC is true, mpres is sending "nowPlaying" event to HMI from "onMetdataChanged" api.
	 */
	setVideoPlaybackStatusFromCoC(status);

	std::function<void(const bool&)> videoPlayStatusCallback= getVideoPlayStatusCallback();
	if (videoPlayStatusCallback != nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::onVideoPlayStatusChanged - sending videoPlayStatus event to HMI with value=%d", status);
		videoPlayStatusCallback(status);
	}
}

void MediaPlaybackEngine::setVideoPlaybackStatusFromCoC(bool value)
{
     mVideoPlaybackStatusFromCoC = value;
}

bool MediaPlaybackEngine::getVideoPlaybackStatusFromCoC()
{
     return mVideoPlaybackStatusFromCoC;
}

void MediaPlaybackEngine::setMediaLocalName(uint32_t ClientID, std::string value, std::function<void(const CommonAPI::CallStatus& status, const std::string& value)> callback)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPlaybackEngine::setMediaLocalName - value=%s", value.c_str());

	ASYNC_INVOKE_2ARG(mMediaSetting, MediaSettingProxy, ARG_LIST_LOCALENAME_RESPONSE, ClientID, callback, getLocaleNameAttribute().setValueAsync, value, )
}
