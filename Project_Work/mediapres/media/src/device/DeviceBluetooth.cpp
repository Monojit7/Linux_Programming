#include <device/DeviceBluetooth.h>
#include<browseengine/session/SessionQueryFS_BTSA.h>

DeviceBluetooth::DeviceBluetooth(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo)
   :DeviceBase(mediumInfo)
   ,mBTConnManager("local", "btpres.connectionManager")
{
   init();
}
void DeviceBluetooth::init()
{
   SUBSCRIBE(mBTConnManager, getProxyStatusEvent(), &DeviceBluetooth::btConnManagerProxyAvailability, _1)
}

void DeviceBluetooth::btConnManagerProxyAvailability(const CommonAPI::AvailabilityStatus& avail)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::btConnManagerProxyAvailability : %s", mBTConnManager.getStringAvailibility(avail).c_str());
   if (CommonAPI::AvailabilityStatus::AVAILABLE == avail)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::btConnManagerProxyAvailability");
      //TODO: To be uncommented
        SUBSCRIBE(mBTConnManager, getServiceConnectionStatusEvent(), &DeviceBluetooth::getServiceConnectionStatusCallback, _1, _2, _3, _4, _5)
   }
}

void DeviceBluetooth::subscribeToCurrentTrackUpdate(bool subscribe)
{
   if (subscribe)
   {
      mTrackToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getCurrentTrackAttribute().getChangedEvent(), &DeviceBluetooth::onCurrentTrackChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBluetooth::subscribeToCurrentTrackUpdate - subscription token: %d", mTrackToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBluetooth::un-subscribeToCurrentTrackUpdate - subscription token: %d", mTrackToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getCurrentTrackAttribute().getChangedEvent(), mTrackToken)
   }
}

void DeviceBluetooth::subscribeToTotalTrackUpdate(bool subscribe)
{
   if (subscribe)
   {
      mTotalTrackToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getTotalTracksAttribute().getChangedEvent(), &DeviceBluetooth::onTotalTracksChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBluetooth::subscribeToTotalTrackUpdate - subscription token: %d", mTotalTrackToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBluetooth::un-subscribeToTotalTrackUpdate - subscription token: %d", mTotalTrackToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getTotalTracksAttribute().getChangedEvent(), mTotalTrackToken)
   }
}

void DeviceBluetooth::subscribeToPlayPositionUpdate(bool subscribe)
{
   if (subscribe)
   {
      mPlayPosToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getPositionAttribute().getChangedEvent(), &DeviceBluetooth::onPlayPositionChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBluetooth::subscribeToPlayPositionUpdate - subscription token: %d", mPlayPosToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBluetooth::un-subscribeToPlayPositionUpdate - subscription token: %d", mPlayPosToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getPositionAttribute().getChangedEvent(), mPlayPosToken)
   }
}

void DeviceBluetooth::subscribeToPlayDurationUpdate(bool subscribe)
{
   if (subscribe)
   {
      mPlayDurationToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getDurationAttribute().getChangedEvent(), &DeviceBluetooth::onPlayDurationChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBluetooth::subscribeToPlayDurationUpdate - subscription token: %d", mPlayDurationToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBluetooth::un-subscribeToPlayDurationUpdate - subscription token: %d", mPlayDurationToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getDurationAttribute().getChangedEvent(), mPlayDurationToken)
   }
}

uint32_t DeviceBluetooth::getDeviceCaps()
{
    return((getMediumInfo().getMBrowseAble()==true)?MMPresTypes::eDeviceCapabilities::DEVICECAP_FSBROWSE:0);
}

void DeviceBluetooth::SessionQueryCreateNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t & mediumID, const MMPresTypes::FilterIDMap & filterIDs, const MMPresTypes::FilterMap & filters, const MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask, const uint64_t& startIndex, const uint64_t& numItems)
{
   switch (type)
   {
      case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE:
      {
         PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBluetooth::SessionQueryFSNew, this, sessionID, _client, reply, gotoPreviousReply, jumpToQueryReply, mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
      }
      break;
      case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE:
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth : SessionQueryCreateNew: SESSION_QUERY_NOWPLAYINGLIST_BROWSE");
         PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryNowPlayingNew, this, sessionID, _client, reply, gotoPreviousReply, jumpToQueryReply, mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
      }
      break;
      default:
      {
         //TBD
      }
      break;
   }
}

void DeviceBluetooth::SessionQueryCreateExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t & startIndex, const uint64_t& numItems)
{
   switch (queryCacheData.mQueryType)
   {
      case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE:
      {
         PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBluetooth::SessionQueryFSExisting, this, sessionID, queryID, _client, reply, gotoPreviousReply, jumpToQueryReply, queryCacheData, startIndex, numItems));
      }
      break;
      case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE:
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth : SessionQueryCreateExisting: SESSION_QUERY_NOWPLAYINGLIST_BROWSE");
         PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryNowPlayingExisting, this, sessionID, queryID, _client, reply, gotoPreviousReply, jumpToQueryReply, queryCacheData, startIndex, numItems));
      }
      break;
      default:
      {
         //TBD
      }
      break;
   }
}

RequestBase* DeviceBluetooth::SessionQueryFSNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex, const uint64_t& numItems)
{
   return (new SessionQueryFS_BTSA(sessionID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
}

RequestBase* DeviceBluetooth::SessionQueryFSExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
{
   return (new SessionQueryFS_BTSA(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems));
}

void DeviceBluetooth::getDeviceHardWareInfo()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::getDeviceHardWareInfo-mediumId:%lu", mMediumInfo.getMMediumId());
   mDetect->getDeviceHardwareInfo(mDetect.mClientID, mMediumInfo.getMMediumId(), std::bind(&DeviceBluetooth::getDeviceHardWareInfoCallback, this, _1, _2, _3));
}

void DeviceBluetooth::getDeviceHardWareInfoCallback(const CommonAPI::CallStatus& status, const ::v1::com::harman::media::BaseType::tHardwareInfo& hardwareInfo, const ::v1::com::harman::media::BaseType::tBaseError& baseError)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::getDeviceHardWareInfoCallback-mediumId:%lu,status:%d, baseError:%d", mMediumInfo.getMMediumId(), (int32_t)status, (int32_t)(baseError));

   if (status == CommonAPI::CallStatus::SUCCESS && baseError == BaseType::tBaseError::S_OK)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::getDeviceHardWareInfoCallback-prod name:%s,version number:%s", hardwareInfo.getProductName().c_str(), hardwareInfo.getVersionNumber().c_str());
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::getDeviceHardWareInfoCallback-vendor id:%s,prod id:%s,manufacturer:%s", hardwareInfo.getVendorId().c_str(), hardwareInfo.getProductId().c_str(), hardwareInfo.getManufacturer().c_str());
      mHardWareInfo = hardwareInfo;

      std::unordered_map<std::string, btTypedefs::profileList> bondedDevList = mDetect->getBondedDeviceList();
      std::unordered_map<std::string, btTypedefs::profileList>::iterator it;
      it = bondedDevList.find(mHardWareInfo.getMAC());
      if (it != bondedDevList.end()) // traverse throught all the paired device and find Media BT device is available
      {
         btTypedefs::profileList profileList = it->second;
         uint16_t i = 0;
         for ( ; i < profileList.size(); i++) //traverse through the profile list of the Media BT device 
         {
            if (!profileList.at(i).getProfileName().compare(DeviceManagerHelper::AVRCP_PROFILE))// 0 == strcmp(profileList.at(i).getProfileName().c_str, "AVRCP") )
            {
               mBTStatus.setProfile(MMPresTypes::eBTMediaProfile::AVRCP);
               mBTStatus.setMajorVersion(profileList.at(i).getMajorNum());
               mBTStatus.setMinorVersion(profileList.at(i).getMinorNum());
               break;
            }
         }
         if (i == profileList.size()) //If AVRCP profile is not found, check whether the device contains only A2DP
         {
            for (i = 0; i < profileList.size(); i++)
            {
               if (!profileList.at(i).getProfileName().compare(DeviceManagerHelper::A2DP_PROFILE))
               {
                  mBTStatus.setProfile(MMPresTypes::eBTMediaProfile::A2DP);
                  mBTStatus.setMajorVersion(profileList.at(i).getMajorNum());
                  mBTStatus.setMinorVersion(profileList.at(i).getMinorNum());
                  break;
               }
            }
         }
         if (i == profileList.size())//IF no AVRCP/A2DP profile is found
         {
            mBTStatus.setProfile(MMPresTypes::eBTMediaProfile::UNKNOWN);
            mBTStatus.setMajorVersion(0);
            mBTStatus.setMinorVersion(0);
         }
         mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(), mBTStatus);
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::getDeviceHWInfoCallbk- Mac addr not found");
      }
      mAdditionalDeviceInfo.setProductName(hardwareInfo.getProductName());
      mAdditionalDeviceInfo.setSerialNumber(hardwareInfo.getSerialNumber());
      mAdditionalDeviceInfo.setVersionNumber(hardwareInfo.getVersionNumber());
      mAdditionalDeviceInfo.setVendorId(hardwareInfo.getVendorId());
      mAdditionalDeviceInfo.setProductId(hardwareInfo.getProductId());
      mAdditionalDeviceInfo.setManufacturer(hardwareInfo.getManufacturer());
      mAdditionalDeviceInfo.setMAC(hardwareInfo.getMAC());
   }
}

const void DeviceBluetooth::onRemotePlayerChanged()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth :onRemotePlayerChanged");
   if (mMediumInfo.getMMediumId() == mPlayEngine->getCurrentMediumId())
   {
      clearMetadata();
   }
   getRemotePlayerCapability();
   std::function<void(const uint32_t&)> remotePlayerChangedCallback = mPlayEngine->getRemotePlayerChangedStubCallback();
   if (nullptr != remotePlayerChangedCallback)
   {
      remotePlayerChangedCallback(mMediumInfo.getMMediumId());
   }
}


void DeviceBluetooth::getRemotePlayerCapability()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::getRemotePlayerCapability-mediumId:%lu", mMediumInfo.getMMediumId());
   mDetect->getRemotePlayerCapability(mDetect.mClientID, mMediumInfo.getMMediumId(), std::bind(&DeviceBluetooth::getRemotePlayerCapabilityCallback, this, _1, _2, _3));
}

void DeviceBluetooth::getRemotePlayerCapabilityCallback(const CommonAPI::CallStatus& status, const ::v1::com::harman::media::BaseType::tRemotePlayerCapability& capability, const ::v1::com::harman::media::BaseType::tBaseError& baseError)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::getRemotePlayerCapabilityCallback-mediumId:%lu,status:%d, baseError:%d", mMediumInfo.getMMediumId(), (int32_t)status, (int32_t)(baseError));
   if (status == CommonAPI::CallStatus::SUCCESS && baseError == BaseType::tBaseError::S_OK)
   {
      mRemotePlayerCapability = capability;
      if ((capability.getPlayerName().compare("iBooks") == 0) || (capability.getPlayerName().compare("Podcasts") == 0))
      {
         setAudiobookPodcast(true);
      }
      else
      {
         setAudiobookPodcast(false);
      }
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::getRemotePlayerCapabilityCallback-playername:%s play:%d,pause:%d,next:%d,prev:%d,ffwd:%d,frwd:%d", capability.getPlayerName().c_str(), capability.getSupportPlay(), capability.getSupportPause(), capability.getSupportNext(), capability.getSSupportPrevious(), capability.getSupportFastForward(), capability.getSupportFastRewind());
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::getRemotePlayerCapabilityCallback-rptOff:%d,rptSingle:%d,rptAll:%d,shuffle:%d,unshuffle:%d,browse:%d", capability.getSupportRepeatOff(), capability.getSupportRepeatSingle(), capability.getSupportRepeatAll(), capability.getSupportShuffle(), capability.getSupportUnShuffle(), capability.getSupportBrowsing());

      uint32_t caps = 0;
      if (capability.getSupportPlay())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_PLAY;
      if (capability.getSupportPause())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_PAUSE;
      if (capability.getSupportNext())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_NEXT;
      if (capability.getSSupportPrevious())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_PREVIOUS;
      if (capability.getSupportFastForward())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_FASTFORWARD;
      if (capability.getSupportFastRewind())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_FASTREWIND;
      if (capability.getSupportRepeatOff())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_REPEATOFF;
      if (capability.getSupportRepeatSingle())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_REPEATSINGLE;
      if (capability.getSupportRepeatAll())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_REPEATALL;
      if (capability.getSupportShuffle())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_SHUFFLE;
      if (capability.getSupportUnShuffle())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_UNSHUFFLE;
      if (capability.getSupportBrowsing())
         caps |= MMPresTypes::eRemotePlayerCapability::REMOTEPLAYERCAP_BROWSING;

      uint8_t slotid = mDetect->findMatchingSlot(mMediumInfo.getMMediumId());

      MMPresTypes::tRemotePlayerCapability temp_capability;
      temp_capability.setCapability(caps);
      temp_capability.setSlotId(slotid);

      std::function<void(const MMPresTypes::tRemotePlayerCapability&)> callback = mPlayEngine->getRemotePlayerCapabilityStubCallback();
      if (callback != nullptr)
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::getRemotePlayerCapability - attributecallback");
         callback(temp_capability);
      }

      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBluetooth::getRemotePlayerCapabilityCallback - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());
   }
}

void DeviceBluetooth::onCurrentTrackChanged(const uint64_t& track)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBluetooth::onCurrentTrackChanged - mediumId:%lu , currentMediumId: %lu track %llu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId(), track);

   if (mPlayEngine->getCurrentMediumId() == mMediumInfo.getMMediumId())
   {
      mTrackState.setCurrentTrack(track);
      std::function<void(const MMPresTypes::tTrackState&)> callback = mPlayEngine->getTrackStateStubCallback();

      //Now we are simply sending the currentTrack.The totalTrack is currently not available from mediaOne srevice. So total track will be 0
      if (callback != nullptr) 
      {
          MMPresTypes::tTrackState l_trackState = mTrackState;

         if (mTrackState.getCurrentTrack() > mTrackState.getTotalTracks())
         {
            l_trackState.setTotalTracks(0);
            mPlayEngine->setTotalTracks(0);
         }
         cancelSeekTimer();
         mPlayEngine->setActiveTrackNum(track);
         callback(l_trackState);
         sendItemIndexTobeHighlighted(track);
      }
   } //   if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )

   if (mbShuffle == true)
   {
      std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();
      if (nowPlayingCallback != nullptr)
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::onCurrentTrackChanged - sending nowPlaying event to HMI onShuffleChange");
         nowPlayingCallback(mNowPlayingInfo);
      }
      mbShuffle = false;
   }
}

void DeviceBluetooth::onTotalTracksChanged(const uint64_t& totalTrack)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::onTotalTracksChanged - mediumId:%lu, currentMediumId:%lu, total track count is %llu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId(), totalTrack);

   if (mPlayEngine->getCurrentMediumId() == mMediumInfo.getMMediumId())
   {
      mTrackState.setTotalTracks(totalTrack);
      MMPresTypes::tTrackState l_trackState = mTrackState;
      if (totalTrack >0)
      {
         setNowPlayingTotalTrack(totalTrack);
      }

      if (mTrackState.getCurrentTrack() > mTrackState.getTotalTracks())
      {
         l_trackState.setTotalTracks(0);
      }

      //mTrackState.setTotalTracks(totalTrack);
      std::function<void(const MMPresTypes::tTrackState&)> callback = mPlayEngine->getTrackStateStubCallback();

      if (callback != nullptr)
      {
         mPlayEngine->setTotalTracks(l_trackState.getTotalTracks());
         callback(l_trackState);
      }
   }
}

void DeviceBluetooth::onPlayPositionChanged(const uint64_t& pos)
{
  if (mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId())
  {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::onPlayPositionChanged play position is %llu", pos);
      mCurrentTime = pos / PLAY_POSITION_DIVISOR;
      uint64_t l_CurrentTime = mCurrentTime;
      if (mTotalDuration <= 0)
      {
         l_CurrentTime = 0;
      }
      std::function<void(const uint64_t&)> callback = mPlayEngine->getCurrentTimeStubCallback();
      if (callback != nullptr)
      {
          callback(l_CurrentTime);
      }
  }
}

void DeviceBluetooth::onPlayDurationChanged(const uint64_t& duration)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBluetooth::onPlayDurationChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId());

   if (mPlayEngine->getCurrentMediumId() == mMediumInfo.getMMediumId())
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::onPlayDurationChanged -duration is %llu", duration);
      uint64_t totalDuration = duration / PLAY_POSITION_DIVISOR;
      mTotalDuration = totalDuration;
      /* The progress bar and total play time shall not be displayed 
         1) if the total play time is unknown or equal to 0.
         2) if the elapsed time is greater than the total time or equals -1
      */
      if ((mTotalDuration <= 0)||(mTotalDuration < mCurrentTime))
      {
         totalDuration = 0;
      }
      std::function<void(const uint64_t&)> callback = mPlayEngine->getTotalDurationStubCallback();
      if (callback != nullptr)
      {
         callback(totalDuration);
      }
   }
}


void DeviceBluetooth::gotoTrack(const uint64_t trackPosition)
{
   const uint64_t lcurrentTrack = getTrackState().getCurrentTrack();

   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBluetooth::gotoTrack ReqtrackPosition %llu currentTrack %llu ", trackPosition, lcurrentTrack);

   // Session Boundary situation
   if ((trackPosition == 0) && (getRepeatMode() == MMPresTypes::eRepeatMode::REPEAT_OFF))
   {
      if (lcurrentTrack == getNowPlayingTotalTrack())
      {
         next(STEPCOUNT_ONE);
      }
      else
      {
         previous(STEPCOUNT_ONE);
      }
   }
   // Prev() and next() call can be used
   else if ((lcurrentTrack == trackPosition)&&(mCurrentTime <= TIMELIMIT_FOR_PREVIOUSUSAGE))
   {
      previous(STEPCOUNT_ONE);
   }
   else // Multiple tracks skip
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< After MediaBrowserStubImpl::gotoTrack ReqtrackPosition %llu currentTrack %llu ", trackPosition, lcurrentTrack);
      seek(trackPosition);
   }
}

void DeviceBluetooth::getServiceConnectionStatusCallback(const std::string& macAddress, const std::string& name, const ::v4::com::harman::btpres::BTPresTypes::eSupportedProfile& profile, const ::v4::com::harman::btpres::BTPresTypes::eConnectionStatus& status, const ::v4::com::harman::btpres::BTPresTypes::eServiceConnectDisconnectReason& reason)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth:: getServiceConnectionStatusCallback- profile: %d status:%d",(int)profile, (int)status);
   if ((profile == BTPresTypes::eSupportedProfile::PROFILE_A2DP) && (status == BTPresTypes::eConnectionStatus::SERVICE_DISCONNECTED)
      && (macAddress == mHardWareInfo.getMAC()))
   {
      setDisconnectReason(reason);
   }
}

void DeviceBluetooth::setDisconnectReason(BTPresTypes::eServiceConnectDisconnectReason reason)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth:: setDisconnectReason- %d", (int)reason);
   am_t::am_Error_e disconnectReason = am_t::am_Error_e::E_OK;
   am_t::am_AvailabilityReason_e lreason = am_t::am_AvailabilityReason_e::USER_DISCONNECTED;

   switch (reason)
   {
      case BTPresTypes::eServiceConnectDisconnectReason::SERVICE_DISCONNECTED_PHONE_INITIATED:
         lreason = am_t::am_AvailabilityReason_e::PEER_DISCONNECTED;
         break;
      case BTPresTypes::eServiceConnectDisconnectReason::SERVICE_DISCONNECTED_LINK_LOSS:
         lreason = am_t::am_AvailabilityReason_e::OUT_OF_RANGE_DISCONNECTED;
         disconnectReason = am_t::am_Error_e::E_OUT_OF_RANGE;
         break;
      case BTPresTypes::eServiceConnectDisconnectReason::SERVICE_DISCONNECTED_DEVICE_DISCONNECTED:
         lreason = am_t::am_AvailabilityReason_e::DATALINK_DISCONNECTED;
         break;
      case BTPresTypes::eServiceConnectDisconnectReason::SERVICE_DISCONNECTED_NEW_CONNECTION_REQUEST:
         lreason = am_t::am_AvailabilityReason_e::ON_CONNECTION_RESET_DISCONNECTED;
         disconnectReason = am_t::am_Error_e::E_ALREADY_EXISTS;
         break;
      case BTPresTypes::eServiceConnectDisconnectReason::SERVICE_DISCONNECTED_HEADUNIT_INITIATED:
      default:
         //lreason = am_t::am_AvailabilityReason_e::USER_DISCONNECTED;
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBluetooth::setDisconnectReason- reason not found");
         break;
   }
   mAudioInterface->setBTDisconnectReason(disconnectReason);
   mAudioInterface->setBTAvailabilityReason(lreason);
}

