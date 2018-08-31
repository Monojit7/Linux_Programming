/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            DeviceBase.cpp
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           The Base Class abstracting a "device" functionality.
*				   Implements common functionality expected from any device like play/pause etc.. Device specific behavior have to be overridden.
*/

#include<device/DeviceBase.h>
#include <browseengine/session/SessionQueryFS.h>
#include <browseengine/session/SessionQueryMetadata.h>
#include <browseengine/session/SessionQueryNowPlaying.h>
#include <browseengine/session/SessionQueryHighestRated.h>
#include <browseengine/session/SessionQueryRecentlyPlayed.h>
#include <browseengine/session/SessionQueryCategory.h>

std::unordered_map<v1::com::harman::media::BaseType::tMediumType, MMPresTypes::eDeviceType> DeviceBase::mDevTypeMapping = {
    { v1::com::harman::media::BaseType::tMediumType::MT_UMASS,MMPresTypes::eDeviceType::DEVICETYPE_USB }
   ,{ v1::com::harman::media::BaseType::tMediumType::MT_IPOD,MMPresTypes::eDeviceType::DEVICETYPE_IPOD }
   ,{ v1::com::harman::media::BaseType::tMediumType::MT_MTP,MMPresTypes::eDeviceType::DEVICETYPE_MTP }
   ,{ v1::com::harman::media::BaseType::tMediumType::MT_BLUETOOTH,MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH }
};

DeviceBase::DeviceBase(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo)
   :RequestBase(e_REQUEST_DEVICEINFO_TASK,"DeviceInfoTask")
   , mMediumInfo(mediumInfo)
   , mSpeedInfo(0u, MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN, MMPresTypes::ePlaybackSpeed::NORMAL)
   , mPlayState(MMPresTypes::ePlayState::PLAYSTATE_UNKNOWN)
   , mCurrentTime(0u)
   , mTotalDuration(0u)
   , mCurrentRate(0.0)
   , mRepeatMode(MMPresTypes::eRepeatMode::REPEAT_UNKNOWN)
   , mShuffleMode(MMPresTypes::eShuffleMode::SHUFFLE_UNKNOWN)
   , mTrackSeekSpeed(MMPresTypes::eTrackSeekSpeed::SPEED_ZERO)
   , mBrowseCaps(0u)
   , mBTStatus(MMPresTypes::eBTMediaProfile::UNKNOWN,0,0)
   , mCanSeek(false)
   , mCanPlay(false)
   , mCanPause(false)
   , mCanGoNext(false)
   , mCanGoPrevious(false)
   , mReadyToPlay(false)
   , mMultipleChapterAvailable(false)
   //, mPlaybackStatusToken(0u)
   , mMetadataToken(0u)
   , mRepeatToken(0u)
   , mShuffleToken(0u)
   , mRateToken(0u)
   , mGoNextToken(0u)
   , mGoPreviousToken(0u)
   , mCanPauseToken(0u)
   , mCanPlayToken(0u)
   , mCanSeekToken(0u)
   , mSessionEndToken(0u)
   , mPlayQueueChangedToken(0u)
   , mNowPlayingTotalTrack(0u)
   , mSlotIdForPodcastAudiobook(0)
   , mTypeForPodcastAudiobook(MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN)
   , mSubscribed(false)
   , mBrowseHistoryEnumValue(0u)
   , mInserted(false)
   , mResetMediaIndex(false)
   , mPlayPlanQueryId(0u)
   , mCurrentPlayingUri("")
   , musicUri("")
   , videoUri("")
   , mBreadCrumbForNowPlaying("/")
   , mNowPlayingType("")
   , mNowPlayingListSupported(false)
   , mSmartSearchAlphaJumpCandidateRequiredToBeSend(false)
   , mIsDeviceEmpty(false)
   , mIsTrackListSupported(false)
   , bAudioPodcastVal(false)
{
   initRateToSeekSpeedConversionMap();
   initAlbumArt();
   mPlayPlanQueryList.clear();
   mContentInfoMap = {{MMPresTypes::eTitleContentType::TITLETYPE_AUDIO,0},
                      { MMPresTypes::eTitleContentType::TITLETYPE_VIDEO,0},
                      { MMPresTypes::eTitleContentType::TITLETYPE_IMAGE,0}};
   mRootlevelPlaybackUriMap = { { MMPresTypes::eTitleContentType::TITLETYPE_AUDIO,{}},
                                { MMPresTypes::eTitleContentType::TITLETYPE_VIDEO,{}}};

   mVideoPlayStatus = false;
}

DeviceBase::DeviceBase(): RequestBase(e_REQUEST_DEVICEINFO_TASK,"DeviceInfoTaskRemoteCD")
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase constructor");
}

void DeviceBase::initRateToSeekSpeedConversionMap()
{
   seekSpeedToRateConversionMap[MMPresTypes::eTrackSeekSpeed::SPEED_ZERO] = 0.000000;
   seekSpeedToRateConversionMap[MMPresTypes::eTrackSeekSpeed::SPEED_1X] = 1.000000;
   seekSpeedToRateConversionMap[MMPresTypes::eTrackSeekSpeed::SPEED_2X] = 2.000000;
   seekSpeedToRateConversionMap[MMPresTypes::eTrackSeekSpeed::SPEED_4X] = 4.000000;
   seekSpeedToRateConversionMap[MMPresTypes::eTrackSeekSpeed::SPEED_8X] = 8.000000;
   seekSpeedToRateConversionMap[MMPresTypes::eTrackSeekSpeed::SPEED_16X] = 16.000000;
   seekSpeedToRateConversionMap[MMPresTypes::eTrackSeekSpeed::SPEED_32X] = 16.000000;
   seekSpeedToRateConversionMap[MMPresTypes::eTrackSeekSpeed::SPEED_64X] = 16.000000;
   seekSpeedToRateConversionMap[MMPresTypes::eTrackSeekSpeed::SPEED_DEVICE_DEPENDENT] = 16.000000;

   rateToSeekSpeedConversionMap[std::make_pair(0.000000, MINUS_1*0.000000)] = MMPresTypes::eTrackSeekSpeed::SPEED_ZERO;
   rateToSeekSpeedConversionMap[std::make_pair(1.000000, MINUS_1*1.000000)] = MMPresTypes::eTrackSeekSpeed::SPEED_1X;
   rateToSeekSpeedConversionMap[std::make_pair(2.000000, MINUS_1*2.000000)] = MMPresTypes::eTrackSeekSpeed::SPEED_2X;
   rateToSeekSpeedConversionMap[std::make_pair(4.000000, MINUS_1*4.000000)] = MMPresTypes::eTrackSeekSpeed::SPEED_4X;
   rateToSeekSpeedConversionMap[std::make_pair(8.000000, MINUS_1*8.000000)] = MMPresTypes::eTrackSeekSpeed::SPEED_8X;
   rateToSeekSpeedConversionMap[std::make_pair(16.000000, MINUS_1*16.000000)] = MMPresTypes::eTrackSeekSpeed::SPEED_16X;
   rateToSeekSpeedConversionMap[std::make_pair(32.000000, MINUS_1*32.000000)] = MMPresTypes::eTrackSeekSpeed::SPEED_32X;
   rateToSeekSpeedConversionMap[std::make_pair(64.000000, MINUS_1*64.000000)] = MMPresTypes::eTrackSeekSpeed::SPEED_64X;

}


void DeviceBase::onRun()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase : onRun: mMediumId=%lu,mMediumState=%d", mMediumInfo.getMMediumId(), (int32_t)(mMediumInfo.getMMediumState()));
    
   if (mMediumInfo.getMMediumState() == v1::com::harman::media::BaseType::tMediumState::MS_READY_TO_PLAY)
   {
      processReadyToPlay();
   }
   else if (mMediumInfo.getMMediumState() == v1::com::harman::media::BaseType::tMediumState::MS_FIRST_PASS_FIN)
   {
      processFirstPass();
   }
   else if (mMediumInfo.getMMediumState() == v1::com::harman::media::BaseType::tMediumState::MS_SECOND_PASS_FIN)
   {
      processSecondPass();
   }
   else if (mMediumInfo.getMMediumState() == v1::com::harman::media::BaseType::tMediumState::MS_ACTIVE)
   {
      processActive();
   }
   else if(mMediumInfo.getMMediumState() == v1::com::harman::media::BaseType::tMediumState::MS_AVAILABLE)
   {
      mInserted = true;
      processInsertion();
      //getDeviceHardWareInfo();
      //getDeviceFirmWareInfo();
      //getRemotePlayerCapability();
   }
   else
   {
   }
   setIdle();
}

DeviceBase::~DeviceBase()
{
   uint32_t temp_mediumId = mMediumInfo.getMMediumId();
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase :~DeviceBase: mMediumId=%lu", temp_mediumId);
   if (mSettings->getBrowseHistoryEnumeration())
   {
      clearPlayPlan();
   }
   mPlayEngine->intimatePlayEngineForCleanup(temp_mediumId);

   /* call removeDatabase of CoC media through browseEngine */
   if(getResetMediaIndex())
   {
      mBrowse->removeDatabse(mMediumInfo.getMMediumId());
      setResetMediaIndex(false);
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
   else
   {
      if (mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this)
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::~DeviceBase: deactivating active device");
         mDetect->setActiveDevice(0);
      }
      //Note:Do not unsubcribe here as there are some derived class virtual funcs. They will be invalid by the time base detsruction happens.
      //unSubscribeToPlayerEvents();
   }
   removeMe();
}


void DeviceBase::onCancelled()
{

}

void DeviceBase::updateSyncState(MMPresTypes::eSyncState state)
{
   int8_t temp_slotId = mDetect->findMatchingSlot(mMediumInfo.getMMediumId());

    if(mMediumInfo.getMMediumType() != MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH)
        mPlayEngine->updateSlotStateMap(temp_slotId, state);
}

void DeviceBase::mediumInfoUpdate(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo)
{
   mMediumInfo = mediumInfo;
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::mediumInfoUpdate-mMediumId=%lu, mediumInfoUpdate: mMediumState=%d", mMediumInfo.getMMediumId(), (int32_t)(mMediumInfo.getMMediumState()));

   if (mMediumInfo.getMMediumState() == v1::com::harman::media::BaseType::tMediumState::MS_READY_TO_PLAY)
   {
      processReadyToPlay();
      getDeviceHardWareInfo();
      getDeviceFirmWareInfo();
   }
   else if(mMediumInfo.getMMediumState() == v1::com::harman::media::BaseType::tMediumState::MS_FIRST_PASS_FIN)
   {
      processFirstPass();
   }
   else if(mMediumInfo.getMMediumState() == v1::com::harman::media::BaseType::tMediumState::MS_SECOND_PASS_FIN)
   {
      processSecondPass();
   }
   else if(mMediumInfo.getMMediumState() == v1::com::harman::media::BaseType::tMediumState::MS_ACTIVE)
   {
      processActive();

#if 0 //earlier workaround is commented - Nilesh
      uint32_t mediumId = mMediumInfo.getMMediumId();
      int8_t temp_slotid = mDetect->findMatchingSlot(mediumId);
      MMPresTypes::tTrackListSupport tmpListSupport;
      tmpListSupport.setIsTrackListSupported(true);
      tmpListSupport.setSlotId(temp_slotid);

      std::function<void(const MMPresTypes::tTrackListSupport&)> callback = mPlayEngine->getTracklistSupportStubCallBack();
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase : mediumInfoUpdate: TrackList Support is available now for SlotId %d",temp_slotid );
      if (callback != nullptr)
      {
         callback(tmpListSupport);
      }
#endif
   }
   else if (mMediumInfo.getMMediumState() == v1::com::harman::media::BaseType::tMediumState::MS_AVAILABLE)
   {
      processInsertion();
   }
   else
   {
   }
}

void DeviceBase::processInsertion()
{
   mInserted = true;
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::processInsertion-mediumId:%lu", mMediumInfo.getMMediumId());
    mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(), getDeviceType(), mMediumInfo.getMMediumName().c_str(), getDeviceCaps());
   updateSourceInfoToAudioPres();
   getRemotePlayerCapability();
}

void DeviceBase::processReadyToPlay()
{
   if (!mInserted)
   {
      processInsertion();
   }
   if (!mReadyToPlay)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::processReadyToPlay-mediumId:%lu", mMediumInfo.getMMediumId());
   //	mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(), getDeviceType(), mMediumInfo.getMMediumName(), getDeviceCaps());

      if (!(mSettings->getAMInteraction()))
      {
         // currently we assume only 1 USB device is supported.....if we want to support multiple USB,then make AMInteraction as true.
         if (mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this)
         {
            // subscribe to MediaPlayer events from MediaOne service.
            subscribeToPlayerEvents();
         }
      }
      if (mSettings->getAMInteraction())
      {
         updateAvailabilityToAudioManager(true);
      }
      mReadyToPlay = true;
      updateSyncState(MMPresTypes::eSyncState::STATE_FIRST_FILE_SYNCED);
      //Fix for ticket 2282239. 
      updateSyncState(MMPresTypes::eSyncState::STATE_START);

      determineAudio();
      if (true == mSettings->getIsVideoSupported())
      {
         determineVideo();
      }
      determineImage();
   }
}

void DeviceBase::processFirstPass()
{
   if (!mInserted)
   {
      processInsertion();
   }
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::processFirstPass-mediumId:%lu",mMediumInfo.getMMediumId());
   processReadyToPlay();
   getDeviceItemsInfo();
   updateSyncState(MMPresTypes::eSyncState::STATE_FILE_PASS_SYNCED);
   determineAudio();
   if (true == mSettings->getIsVideoSupported())
   {
      determineVideo();
   }
   determineImage();
}

void DeviceBase::processSecondPass()
{
   if (!mInserted)
   {
      processInsertion();
   }
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::processSecondPass-mediumId:%lu",mMediumInfo.getMMediumId());
   processReadyToPlay();
   updateSyncState(MMPresTypes::eSyncState::STATE_METADATA_PASS_SYNCED);
   updateSyncState(MMPresTypes::eSyncState::STATE_PLAYLIST_PASS_SYNCED);
   determineAudio();
   if (true == mSettings->getIsVideoSupported())
   {
      determineVideo();
   }
   determineImage();
}

void DeviceBase::processActive()
{
   if (!mInserted)
   {
      processInsertion();
   }
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::processActive-mediumId:%lu",mMediumInfo.getMMediumId());
   processReadyToPlay();
   getDeviceItemsInfo();
   updateSyncState(MMPresTypes::eSyncState::STATE_THUMBNAIL_PASS_SYNCED);
   determineAudio();
   if (true == mSettings->getIsVideoSupported())
   {
      determineVideo();
   }
   determineImage();
}

void DeviceBase::getCategoryCountMap()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getCategoryCountMap-mediumId:%lu", mMediumInfo.getMMediumId());
   determineAudio();
   if (true == mSettings->getIsVideoSupported())
   {
      determineVideo();
   }
   determineImage();
}

void DeviceBase::determineAudio()
{
   std::vector<std::string> filter = { "*" };
   std::string brpath = mMediumInfo.getMDeviceBrowsePath() + "/" + (BrowseHelper::MUSIC_CATEGORY_NAME);
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::determineAudio-brpath:%s", brpath.c_str());
   // We are querying the browsing capabilities. 
   mBrowse->listContainers(mBrowse.mClientID, brpath, 0u, 0u, filter, std::bind(&DeviceBase::deviceContainerUpdate, this, _1, _2, _3));
}

void DeviceBase::determineVideo()
{
   std::vector<std::string> filter = { "*" };
   std::string brpath = mMediumInfo.getMDeviceBrowsePath() +  "/" + (BrowseHelper::VIDEO_CATEGORY_NAME);
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::determineAudio-brpath:%s", brpath.c_str());
   // We are querying the browsing capabilities. 
   mBrowse->listContainers(mBrowse.mClientID, brpath, 0u, 0u, filter, std::bind(&DeviceBase::deviceContainerUpdateForVideo, this, _1, _2, _3));
}

void DeviceBase::determineImage()
{
   std::vector<std::string> filter = { "*" };
   std::string brpath = mMediumInfo.getMDeviceBrowsePath() +  "/" + (BrowseHelper::IMAGE_CATEGORY_NAME);
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::determineAudio-brpath:%s", brpath.c_str());
   // We are querying the browsing capabilities. 
   mBrowse->listContainers(mBrowse.mClientID, brpath, 0u, 0u, filter, std::bind(&DeviceBase::deviceContainerUpdateForImage, this, _1, _2, _3));
}

void DeviceBase::getContentInfoMap()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getContentInfoMap-mediumId:%lu", mMediumInfo.getMMediumId());
   determineAudio();
   getDeviceItemsInfo();
}


void DeviceBase::getDeviceFirmWareInfo()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getDeviceFirmWareInfo-mediumId:%lu",mMediumInfo.getMMediumId());
   mDetect->getDeviceFirmwareInfo(mDetect.mClientID,mMediumInfo.getMMediumId(),std::bind(&DeviceBase::getDeviceFirmWareInfoCallback,this,_1,_2,_3));
}

void DeviceBase::getDeviceFirmWareInfoCallback(const CommonAPI::CallStatus& status, const ::v1::com::harman::media::BaseType::tFirmwareInfo& firmWareInfo, const ::v1::com::harman::media::BaseType::tBaseError& baseError)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getDeviceFirmWareInfoCallback-mediumId:%lu,status:%d, baseError:%d",mMediumInfo.getMMediumId(),(int32_t)status, (int32_t)(baseError));

   if (status == CommonAPI::CallStatus::SUCCESS && baseError == BaseType::tBaseError::S_OK)
   {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getDeviceFirmWareInfoCallback-firmware:%s, protocol:%s,modelNumber:%s", firmWareInfo.getFirmware().c_str(), firmWareInfo.getProtocol().c_str(), firmWareInfo.getModelNumber().c_str());
         mFirmWareInfo = firmWareInfo;
         mAdditionalDeviceInfo.setFirmware(mFirmWareInfo.getFirmware());
         mAdditionalDeviceInfo.setProtocol(mFirmWareInfo.getProtocol());
         mAdditionalDeviceInfo.setModelNumber(mFirmWareInfo.getModelNumber());
   }
}

void DeviceBase::getDeviceHardWareInfo()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getDeviceHardWareInfo-mediumId:%lu",mMediumInfo.getMMediumId());
   mDetect->getDeviceHardwareInfo(mDetect.mClientID, mMediumInfo.getMMediumId(), std::bind(&DeviceBase::getDeviceHardWareInfoCallback, this, _1, _2, _3));
}

void DeviceBase::getDeviceHardWareInfoCallback(const CommonAPI::CallStatus& status, const ::v1::com::harman::media::BaseType::tHardwareInfo& hardwareInfo, const ::v1::com::harman::media::BaseType::tBaseError& baseError)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getDeviceHardWareInfoCallback-mediumId:%lu,status:%d, baseError:%d",mMediumInfo.getMMediumId(),(int32_t)status, (int32_t)(baseError));

   if (status == CommonAPI::CallStatus::SUCCESS && baseError == BaseType::tBaseError::S_OK)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getDeviceHardWareInfoCallback-prod name:%s,version number:%s", hardwareInfo.getProductName().c_str(),hardwareInfo.getVersionNumber().c_str());
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getDeviceHardWareInfoCallback-vendor id:%s,prod id:%s,manufacturer:%s,", hardwareInfo.getVendorId().c_str(), hardwareInfo.getProductId().c_str(), hardwareInfo.getManufacturer().c_str());
      mHardWareInfo = hardwareInfo;
      mAdditionalDeviceInfo.setProductName(mHardWareInfo.getProductName());
      mAdditionalDeviceInfo.setSerialNumber(mHardWareInfo.getSerialNumber());
      mAdditionalDeviceInfo.setVersionNumber(mHardWareInfo.getVersionNumber());
      mAdditionalDeviceInfo.setVendorId(mHardWareInfo.getVendorId());
      mAdditionalDeviceInfo.setProductId(mHardWareInfo.getProductId());
      mAdditionalDeviceInfo.setManufacturer(mHardWareInfo.getManufacturer());
      mAdditionalDeviceInfo.setMAC(mHardWareInfo.getMAC());
   }
}

void DeviceBase::getDeviceItemsInfo()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getDeviceItemsInfo-mediumId:%lu",mMediumInfo.getMMediumId());
   mDetect->getDeviceItemsInfo(mDetect.mClientID, mMediumInfo.getMMediumId(), std::bind(&DeviceBase::getDeviceItemsInfoCallback, this, _1, _2, _3));
}

void DeviceBase::getDeviceItemsInfoCallback(const CommonAPI::CallStatus& status, const ::v1::com::harman::media::BaseType::tItemsInfo& itemsInfo, const ::v1::com::harman::media::BaseType::tBaseError& baseError)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getDeviceItemsInfoCallback-mediumId:%lu,status:%d, baseError:%d",mMediumInfo.getMMediumId(),(int32_t)status, (int32_t)(baseError));
   if(status== CommonAPI::CallStatus::SUCCESS && baseError== BaseType::tBaseError::S_OK)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getDeviceItemsInfoCallback-audio count:%d,video count:%d,image count:%d", (int32_t)(itemsInfo.getTotalAudioItems()), (int32_t)(itemsInfo.getTotalVideoItems()), (int32_t)(itemsInfo.getTotalImageItems()));
      mItemsInfo = itemsInfo;
      uint8_t temp_slotid = mDetect->findMatchingSlot(mMediumInfo.getMMediumId());
      MMPresTypes::eDeviceType devtype = mDetect->getDeviceType(temp_slotid);
      //update image and video capabilities
      mContentInfoMap[MMPresTypes::eTitleContentType::TITLETYPE_AUDIO] = mItemsInfo.getTotalAudioItems();
      mContentInfoMap[MMPresTypes::eTitleContentType::TITLETYPE_VIDEO] = mItemsInfo.getTotalVideoItems();
      mContentInfoMap[MMPresTypes::eTitleContentType::TITLETYPE_IMAGE] = mItemsInfo.getTotalImageItems();

      if (mItemsInfo.getTotalImageItems() > 0)
      {
         mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_IMAGE;
         mImageCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_IMAGE] = mItemsInfo.getTotalImageItems();
      }
      if (mItemsInfo.getTotalVideoItems() > 0)
      {
         mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_VIDEO;
         mVideoCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_VIDEO] = mItemsInfo.getTotalVideoItems();
      }
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::getDeviceItemsInfoCallback-Browsing Capabilities : %lu", mBrowseCaps);
      mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(),mContentInfoMap);
        //if(!mItemsInfo.getTotalAudioItems() && !mItemsInfo.getTotalVideoItems())

      if(!mItemsInfo.getTotalAudioItems() && !mItemsInfo.getTotalVideoItems() && (devtype != MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH ))
      {
    	  LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::getDeviceItemsInfoCallback-Browsing temp_slotid  : %d for MEDIAERROR_DEVICE_EMPTY", temp_slotid);
    	  triggerMediaErrorToDevice(temp_slotid,MMPresTypes::eMediaError::MEDIAERROR_DEVICE_EMPTY);
    	  setIsDeviceEmpty(true);
      }
   }
}

void DeviceBase::getRemotePlayerCapability()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getRemotePlayerCapability-mediumId:%lu", mMediumInfo.getMMediumId());
}

void DeviceBase::getRemotePlayerCapabilityCallback(const CommonAPI::CallStatus& status, const ::v1::com::harman::media::BaseType::tRemotePlayerCapability& capability, const ::v1::com::harman::media::BaseType::tBaseError& baseError)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getRemotePlayerCapabilityCallback-mediumId:%lu,status:%d, baseError:%d", mMediumInfo.getMMediumId(), (int32_t)status, (int32_t)(baseError));
   if (status == CommonAPI::CallStatus::SUCCESS && baseError == BaseType::tBaseError::S_OK)
   {
      mRemotePlayerCapability = capability;

      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getRemotePlayerCapabilityCallback-play:%d,pause:%d,next:%d,prev:%d,ffwd:%d,frwd:%d",capability.getSupportPlay(),capability.getSupportPause(),capability.getSupportNext(),capability.getSSupportPrevious(),capability.getSupportFastForward(),capability.getSupportFastRewind());
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getRemotePlayerCapabilityCallback-rptOff:%d,rptSingle:%d,rptAll:%d,shuffle:%d,unshuffle:%d,browse:%d",capability.getSupportRepeatOff(),capability.getSupportRepeatSingle(),capability.getSupportRepeatAll(),capability.getSupportShuffle(),capability.getSupportUnShuffle(),capability.getSupportBrowsing());
   }
}

void DeviceBase::subscribeToPlayerEvents()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::subscribeToPlayerEvents");

   if (mSubscribed == false)
   {
      //subscribeToPlaybackStatusUpdate(true);
      subscribeToCurrentTrackUpdate(true);
      subscribeToTotalTrackUpdate(true);
      subscribeToMetadataUpdate(true);
      subscribeToPlayDurationUpdate(true);
      subscribeToPlayPositionUpdate(true);
      subscribeToRateUpdate(true);
      subscribeToRepeatStatusUpdate(true);
      subscribeToShuffleStatusUpdate(true);
      subscribeToCanGoNext(true);
      subscribeToCanGoPrevious(true);
      subscribeToCanPause(true);
      subscribeToCanPlay(true);
      subscribeToCanSeek(true);
      subscribeToSessionEnd(true);
      //	susbcribeToPlayQueueChanged(true);
      subscribeToPlayerType(true);
      subscribeToCanGetPlayQueue(true);
      mSubscribed = true;
   }
}

void DeviceBase::unSubscribeToPlayerEvents()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::unSubscribeToPlayerEvents");

   if (mSubscribed)
   {
      //subscribeToPlaybackStatusUpdate(false);
      subscribeToCurrentTrackUpdate(false);
      subscribeToTotalTrackUpdate(false);
      subscribeToMetadataUpdate(false);
      subscribeToPlayDurationUpdate(false);
      subscribeToPlayPositionUpdate(false);
      subscribeToRateUpdate(false);
      subscribeToRepeatStatusUpdate(false);
      subscribeToShuffleStatusUpdate(false);
      subscribeToCanGoNext(false);
      subscribeToCanGoPrevious(false);
      subscribeToCanPause(false);
      subscribeToCanPlay(false);
      subscribeToCanSeek(false);
      subscribeToSessionEnd(false);
   //	susbcribeToPlayQueueChanged(false);
      subscribeToPlayerType(false);
      subscribeToCanGetPlayQueue(false);
      mSubscribed = false;
   }
}

void DeviceBase::subscribeToCurrentTrackUpdate(bool subscribe)
{
   if (subscribe)
   {
      mTrackToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getCurrentTrackAttribute().getChangedEvent(), &DeviceBase::onCurrentTrackChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToCurrentTrackUpdate - subscription token: %d", mTrackToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToCurrentTrackUpdate - subscription token: %d", mTrackToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getCurrentTrackAttribute().getChangedEvent(), mTrackToken)
   }
}

void DeviceBase::subscribeToTotalTrackUpdate(bool subscribe)
{
   if (subscribe)
   {
      mTotalTrackToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getTotalTracksAttribute().getChangedEvent(), &DeviceBase::onTotalTracksChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToTotalTrackUpdate - subscription token: %d", mTotalTrackToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToTotalTrackUpdate - subscription token: %d", mTotalTrackToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getTotalTracksAttribute().getChangedEvent(), mTotalTrackToken)
   }
}


void DeviceBase::subscribeToMetadataUpdate(bool subscribe)
{
   if (subscribe)
   {
      mMetadataToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getMetadataAttribute().getChangedEvent(), &DeviceBase::onMetadataChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToMetadataUpdate - subscription token: %d", mMetadataToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToMetadataUpdate - subscription token: %d", mMetadataToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getMetadataAttribute().getChangedEvent(), mMetadataToken)
   }
}

#if 0
void DeviceBase::subscribeToPlaybackStatusUpdate(bool subscribe)
{
   if (subscribe)
   {
      mPlaybackStatusToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getPlaybackStatusAttribute().getChangedEvent(), &DeviceBase::onPlaybackStatusChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToPlaybackStatusUpdate - subscription token: %d", mPlaybackStatusToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToPlaybackStatusUpdate - subscription token: %d", mPlaybackStatusToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getPlaybackStatusAttribute().getChangedEvent(), mPlaybackStatusToken)
   }
}
#endif

void DeviceBase::subscribeToPlayDurationUpdate(bool subscribe)
{
   if (subscribe)
   {
      mPlayDurationToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getDurationAttribute().getChangedEvent(), &DeviceBase::onPlayDurationChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToPlayDurationUpdate - subscription token: %d", mPlayDurationToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToPlayDurationUpdate - subscription token: %d", mPlayDurationToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getDurationAttribute().getChangedEvent(), mPlayDurationToken)
   }
}

void DeviceBase::subscribeToPlayPositionUpdate(bool subscribe)
{
   if (subscribe)
   {
      mPlayPosToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getPositionAttribute().getChangedEvent(), &DeviceBase::onPlayPositionChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToPlayPositionUpdate - subscription token: %d", mPlayPosToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToPlayPositionUpdate - subscription token: %d", mPlayPosToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getPositionAttribute().getChangedEvent(), mPlayPosToken)
   }
}

void DeviceBase::subscribeToRateUpdate(bool subscribe)
{
   if (subscribe)
   {
      mRateToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getRateAttribute().getChangedEvent(), &DeviceBase::onRateChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToRateUpdate - subscription token: %d", mRateToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToRateUpdate - subscription token: %d", mRateToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getRateAttribute().getChangedEvent(), mRateToken)
   }
}

void DeviceBase::subscribeToRepeatStatusUpdate(bool subscribe)
{
   if (subscribe)
   {
      mRepeatToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getRepeatAttribute().getChangedEvent(), &DeviceBase::onRepeatChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToRepeatStatusUpdate - subscription token: %d", mRepeatToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToRepeatStatusUpdate - subscription token: %d", mRepeatToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getRepeatAttribute().getChangedEvent(), mRepeatToken)
   }
}

void DeviceBase::subscribeToShuffleStatusUpdate(bool subscribe)
{
   if (subscribe)
   {
      mShuffleToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getShuffleAttribute().getChangedEvent(), &DeviceBase::onShuffleChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToShuffleStatusUpdate - subscription token: %d", mShuffleToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToShuffleStatusUpdate - subscription token: %d", mShuffleToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getShuffleAttribute().getChangedEvent(), mShuffleToken)
   }
}

void DeviceBase::subscribeToCanGoNext(bool subscribe)
{
   if (subscribe)
   {
      mGoNextToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanGoNextAttribute().getChangedEvent(), &DeviceBase::onCanGoNextChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToCanGoNext - subscription token: %d", mGoNextToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToCanGoNext - subscription token: %d", mGoNextToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanGoNextAttribute().getChangedEvent(), mGoNextToken)
   }
}

void DeviceBase::subscribeToCanGoPrevious(bool subscribe)
{
   if (subscribe)
   {
      mGoPreviousToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanGoPreviousAttribute().getChangedEvent(), &DeviceBase::onCanGoPreviousChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToCanGoPrevious - subscription token: %d", mGoPreviousToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToCanGoPrevious - subscription token: %d", mGoPreviousToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanGoPreviousAttribute().getChangedEvent(), mGoPreviousToken)
   }
}

void DeviceBase::subscribeToCanPause(bool subscribe)
{
   if (subscribe)
   {
      mCanPauseToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanPauseAttribute().getChangedEvent(), &DeviceBase::onCanPauseChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToCanPause - subscription token: %d", mCanPauseToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToCanPause - subscription token: %d", mCanPauseToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanPauseAttribute().getChangedEvent(), mCanPauseToken)
   }
}

void DeviceBase::subscribeToCanPlay(bool subscribe)
{
   if (subscribe)
   {
      mCanPlayToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanPlayAttribute().getChangedEvent(), &DeviceBase::onCanPlayChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToCanPlay - subscription token: %d", mCanPlayToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToCanPlay - subscription token: %d", mCanPlayToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanPlayAttribute().getChangedEvent(), mCanPlayToken)
   }
}

void DeviceBase::subscribeToCanSeek(bool subscribe)
{
   if (subscribe)
   {
      mCanSeekToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanSeekAttribute().getChangedEvent(), &DeviceBase::onCanSeekChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToCanSeek - subscription token: %d", mCanSeekToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToCanSeek - subscription token: %d", mCanSeekToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanSeekAttribute().getChangedEvent(), mCanSeekToken)
   }
}

void DeviceBase::subscribeToSessionEnd(bool subscribe)
{
   if (subscribe)
   {
      mSessionEndToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getSessionEndAttribute().getChangedEvent(), &DeviceBase::onSessionEndChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToSessionEnd - subscription token: %d", mSessionEndToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToSessionEnd - subscription token: %d", mSessionEndToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getSessionEndAttribute().getChangedEvent(), mSessionEndToken)
   }
}

/*void DeviceBase::susbcribeToPlayQueueChanged(bool subscribe)
{
   static uint32_t token = 0;
   if (subscribe)
   {
      mPlayQueueChangedToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getPlayQueueChangedEvent(), &DeviceBase::onPlayQueueChanged)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::susbcribeToPlayQueueChanged - subscription token: %d", mPlayQueueChangedToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-susbcribeToPlayQueueChanged - subscription token: %d", mPlayQueueChangedToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getPlayQueueChangedEvent(), mPlayQueueChangedToken)
   }
}*/


void DeviceBase::setVolumeAttribute(const double& volume)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::setVolumeAttribute - volume:%f ", volume);
   //mPlayEngine->setVolume(mPlayEngine.mClientID, volume, std::bind(&DeviceBase::volumeCallback, this, _1, _2));////Removed Absolute Volume for Elvis Ticket no:2322696 
   //mPlayEngine->getPlayerProxy()->getVolumeAttribute().setValueAsync(volume,(std::bind(&DeviceBase::volumeCallback, this, _1)),_info);
}

void DeviceBase::resume()
{
   /*if (!mSettings->getAMInteraction() && tempcount == 1)
   {
      mBrowse->discoverMediaManagers(mBrowse.mClientID, std::bind(&DeviceBase::discoverMediaManagerCallback, this, _1, _2, _3));
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::resume - invoke playDevice-mediumId:%llu \n", mMediumInfo.getMMediumId());
      playDevice();
   }
   else*/
   uint64_t activeDeviceId= mDetect->getActiveDeviceId();
   am_t::am_sourceID_e activeDeviceSourceId = mAudioInterface->getSourceID(activeDeviceId);
	u_int16_t activeSourceId = mAudioInterface->getActiveSourceId();
	if( ( (activeSourceId == am_t::am_sourceID_e::eSRC_TUNER_DAB_TA) || (activeSourceId == am_t::am_sourceID_e::eSRC_TUNER_TA) ) && (getPlayState() == MMPresTypes::ePlayState::PLAYSTATE_PAUSED) )
	{	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::Resume - activeDevice=%d,active source=%d ", activeDeviceSourceId, activeSourceId);
		mAudioInterface->doSourceSwitch(static_cast<::v3::org::genivi::am_t::am_sourceID_t>(activeDeviceSourceId),am_t::am_sinkID_e::eSNK_SPKS);
		cancelSeekTimer();
	}
	else
   {
      cancelSeekTimer();
      mPlayEngine->resume(mPlayEngine.mClientID, std::bind(&DeviceBase::resumeCallback, this, _1, _2));
   }
}

void DeviceBase::pause()
{
   cancelSeekTimer();
   mPlayEngine->pause(mPlayEngine.mClientID, std::bind(&DeviceBase::pauseCallback, this, _1, _2));
}

void DeviceBase::stop()
{
   cancelSeekTimer();
   mPlayEngine->stop(mPlayEngine.mClientID, std::bind(&DeviceBase::stopCallback, this, _1, _2));
}

void DeviceBase::next(uint64_t step)
{
   cancelSeekTimer();
   if (mRepeatMode == MMPresTypes::eRepeatMode::REPEAT_OFF && mPlayState == MMPresTypes::ePlayState::PLAYSTATE_STOPPED)
   {
      uint8_t temp_slotid = mDetect->findMatchingSlot(mMediumInfo.getMMediumId());
      triggerMediaErrorToDevice(temp_slotid, MMPresTypes::eMediaError::MEDIAERROR_ENDOFPLAYLIST);
   }

   MMPresTypes::eMetadataType type = mNowPlayingInfo.getNowPlayingItemType();
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::next:mMultipleChapterAvailable:%d",mMultipleChapterAvailable);
   if((mMultipleChapterAvailable) && (type != MMPresTypes::eMetadataType::METADATATYPE_TITLE))
   {

      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::nextChapter");
      mPlayEngine->nextChapter(mPlayEngine.mClientID, std::bind(&DeviceBase::nextChapterCallback, this, _1, _2));
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::next");
      mPlayEngine->next(mPlayEngine.mClientID, std::bind(&DeviceBase::nextCallback, this, _1, _2));
   }
}

void DeviceBase::previous(uint64_t step)
{
   cancelSeekTimer();
   if (mRepeatMode == MMPresTypes::eRepeatMode::REPEAT_OFF && mPlayState == MMPresTypes::ePlayState::PLAYSTATE_STOPPED)
   {
      uint8_t temp_slotid = mDetect->findMatchingSlot(mMediumInfo.getMMediumId());
      triggerMediaErrorToDevice(temp_slotid, MMPresTypes::eMediaError::MEDIAERROR_ENDOFPLAYLIST);
   }

   MMPresTypes::eMetadataType type = mNowPlayingInfo.getNowPlayingItemType();
   if((mMultipleChapterAvailable) && (type != MMPresTypes::eMetadataType::METADATATYPE_TITLE))
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::previousChapter");
      mPlayEngine->previousChapter(mPlayEngine.mClientID, std::bind(&DeviceBase::previousChapterCallback, this, _1, _2));
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::previous");
      mPlayEngine->previous(mPlayEngine.mClientID, std::bind(&DeviceBase::previousCallback, this, _1, _2));
   }
}

void DeviceBase::seekToTime(uint64_t pos)
{
   uint64_t timePosition = pos*PLAY_POSITION_DIVISOR;
   mPlayEngine->seekToTime(mPlayEngine.mClientID, timePosition, std::bind(&DeviceBase::seekToTimeCallback, this, _1, _2));
}

void DeviceBase::seek(const int64_t pos)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::seek %ld", pos);
   mPlayEngine->seek(mPlayEngine.mClientID, pos, std::bind(&DeviceBase::seekCallback, this, _1, _2));
}

void DeviceBase::setRepeat(MMPresTypes::eRepeatMode mode)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::setRepeat -mediumId:%lu , repeat input is %d", mMediumInfo.getMMediumId(), (int)mode);
   PlayerTypes::RepeatStatus repeat;
   bool repeatSupported = true;
   switch (mode)
   {
   case MMPresTypes::eRepeatMode::REPEAT_ALL:
   case MMPresTypes::eRepeatMode::REPEAT_ALBUM:
   case MMPresTypes::eRepeatMode::REPEAT_FOLDER:
   case MMPresTypes::eRepeatMode::REPEAT_SUBFOLDER:
      repeat = PlayerTypes::RepeatStatus::REPEAT;
      break;
   case MMPresTypes::eRepeatMode::REPEAT_SONG:
      repeat = PlayerTypes::RepeatStatus::REPEAT_SINGLE;
      break;
   case MMPresTypes::eRepeatMode::REPEAT_OFF:
      repeat = PlayerTypes::RepeatStatus::NO_REPEAT;
      break;
   default:
      repeatSupported = false;
      break;
   }

   if (repeatSupported)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::setRepeat - repeat requested to mediaone is %d", (int)repeat);
      mPlayEngine->setRepeat(mPlayEngine.mClientID, repeat, std::bind(&DeviceBase::setRepeatCallback, this, _1, _2));
   }
}

void DeviceBase::setShuffle(MMPresTypes::eShuffleMode mode)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::setShuffle - mediumId:%lu ,shuffle input is %d", mMediumInfo.getMMediumId(), (int)mode);
   PlayerTypes::ShuffleStatus shuffle;
   bool shuffleSupported = true;

   switch (mode)
   {
   case MMPresTypes::eShuffleMode::SHUFFLE_ALL:
   case MMPresTypes::eShuffleMode::SHUFFLE_ALBUM:
   case MMPresTypes::eShuffleMode::SHUFFLE_FOLDER:
   case MMPresTypes::eShuffleMode::SHUFFLE_SUBFOLDER:
      shuffle = PlayerTypes::ShuffleStatus::SHUFFLE;
      break;

   case MMPresTypes::eShuffleMode::SHUFFLE_OFF:
      shuffle = PlayerTypes::ShuffleStatus::UNSHUFFLE;
      break;

   default:
      shuffleSupported = false;
      break;
   }

   if (shuffleSupported)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::setShuffle - shuffle requested to mediaone is %d", (int)shuffle);
      mPlayEngine->setShuffle(mPlayEngine.mClientID, shuffle, std::bind(&DeviceBase::setShuffleCallback, this, _1, _2));
   }
}

MMPresTypes::eCallResult DeviceBase::fastForward()
{
   const double rate = 128.000000;
   LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::fastForward - mediumId:%lu", mMediumInfo.getMMediumId());
   mPlayEngine->fastForward(mPlayEngine.mClientID, rate, std::bind(&DeviceBase::fastForwardCallback, this, _1, _2));
   return MMPresTypes::eCallResult::CALLRESULT_OK;
}

MMPresTypes::eCallResult DeviceBase::fastReverse()
{
   const double rate = (MINUS_1)*128.0000000;
   LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::fastReverse - mediumId:%lu", mMediumInfo.getMMediumId());
   mPlayEngine->fastReverse(mPlayEngine.mClientID, rate, std::bind(&DeviceBase::fastReverseCallback, this, _1, _2));
   return MMPresTypes::eCallResult::CALLRESULT_OK;
}

void DeviceBase::dequeAll()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::dequeAll-mediumId:%lu", mMediumInfo.getMMediumId());
   cancelSeekTimer();
   mPlayEngine->dequeAll(mPlayEngine.mClientID, std::bind(&DeviceBase::dequeAllCallback, this, _1, _2));
}

void DeviceBase::initAlbumArt()
{
   mArtworkStatus = MMPresTypes::eArtWorkAvailabilityStatus::ARTWORK_STATUS_UNKNOWN;
   mAlbumArtInformation.setTotalImages(0);
}

// temporary implementation.
void DeviceBase::setAlbumArt(uint8_t imageNum, std::string path)
{
   if (imageNum > 0)
   {
      MMPresTypes::tArtWorkArtImage image;
      image.setPositionType(MMPresTypes::tArtWorkImagePositionType::ALBUMART_POSITION_TIME);
      image.setArtWorkPosition(0);
      image.setImageNumber(imageNum);

      MMPresTypes::tArtWorkImageProperty imageProperty;
      imageProperty.setDescription("");
      imageProperty.setHeight(0);
      imageProperty.setSize(0);
      imageProperty.setWidth(0);
      image.setProperty(imageProperty);

      MMPresTypes::artWorkImageInfo info;
      info.push_back(image);

      mAlbumArtInformation.setInfo(info);
      mAlbumArtPathMap[imageNum] = path;

      mArtworkStatus = MMPresTypes::eArtWorkAvailabilityStatus::ARTWORK_STATUS_AVAILABLE;
      mAlbumArtInformation.setTotalImages(mAlbumArtPathMap.size());
   }
   else
   {
      mArtworkStatus = MMPresTypes::eArtWorkAvailabilityStatus::ARTWORK_STATUS_UNAVAILABLE;
      mAlbumArtInformation.setTotalImages(0);
      mAlbumArtPathMap.clear();
   }

   std::function<void(const MMPresTypes::eArtWorkAvailabilityStatus&)> callback = mPlayEngine->getArtworkStatusCallback();
   if (callback != nullptr)
   {
      callback(mArtworkStatus);
   }
}

std::string DeviceBase::getNowPlayingAlbumArtPath(uint8_t imageNum)
{
   std::string path = "";
   if (mAlbumArtPathMap.size() > 0)
   {
      std::map<uint8_t, std::string>::const_iterator itr;
      itr = mAlbumArtPathMap.find(imageNum);
      if (itr != mAlbumArtPathMap.end())
      {
         path = itr->second;
      }
   }
   return path;
}

void DeviceBase::playDevice(const MMPresTypes::eMetadataType objType ,std::string uri )
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<<  DeviceBase::playDevice :mediumId:%lu ,DeviceUri=%s", mMediumInfo.getMMediumId(), mMediumInfo.getMDeviceUri().c_str());

#ifdef PLATFORM_WIN32

#else
    //sleep(2);
#endif

        uint8_t temp_slotid = mDetect->findMatchingSlot(mMediumInfo.getMMediumId());
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<<  DeviceBase::playDevice : mReadyToPlay %d   getTotalAudioItems %d  getTotalVideoItems %d ", mReadyToPlay, mItemsInfo.getTotalAudioItems(),mItemsInfo.getTotalVideoItems()  );
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<<  DeviceBase::playDevice : Sync-Status %d ",  mMediumInfo.getMMediumState());
        /* below if-condition is to show Empty Device pop-up on HMI */
        if(getIsDeviceEmpty())
        {
        	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::playDevice Empty device Play :mediumId:%lu slot_id:%d", mMediumInfo.getMMediumId(), temp_slotid);
        	triggerMediaErrorToDevice(temp_slotid,MMPresTypes::eMediaError::MEDIAERROR_DEVICE_EMPTY);
        }
        else
        {
        	/* below is normal code for playback */
        	switch(objType)
        	{
        	case MMPresTypes::eMetadataType::METADATATYPE_TITLE:

        		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::playDevice-musicUri:%s", musicUri.c_str());
        		mPlayEngine->getPlayerProxy()->openUriAsync(musicUri, std::bind(&DeviceBase::openuriPlayAllCallback, this, _1, _2));
        		//setBreadCrumbForSetSession("Songs");
        		setBreadCrumbForSetSession("");
        		setPlayPlanQueryId(0);
        		break;

        	case MMPresTypes::eMetadataType::METADATATYPE_VIDEO:
        		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::playDevice-videoUri:%s", videoUri.c_str());

        		mPlayEngine->getPlayerProxy()->openUriAsync(videoUri, std::bind(&DeviceBase::openuriPlayAllCallback, this,_1,_2));
        		//setBreadCrumbForSetSession("Videos");
        		setBreadCrumbForSetSession("");
        		setPlayPlanQueryId(0);
        		break;

        	case MMPresTypes::eMetadataType::METADATATYPE_FOLDER:
        		mPlayEngine->getPlayerProxy()->openUriAsync(uri, std::bind(&DeviceBase::openuriPlayAllCallback, this,_1,_2));
        		break;

        	default:
        	{
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<<  DeviceBase::playDevice :mediumId:%lu ,invoke openUri=%s", mMediumInfo.getMMediumId(), mMediumInfo.getMDeviceUri  ().c_str ());
            if (!mMediumInfo.getMDeviceUri().empty())
            {
               mPlayEngine->playDevice(mPlayEngine.mClientID, mMediumInfo.getMDeviceUri(), std::bind(&DeviceBase::playDeviceCallback, this, _1, _2));
            }
        	}
        	break;

        	} //switch(objType)
        } //else

        mPlayEngine->getPlaybackSpeed(mPlayEngine.mClientID, BaseType::tAudioObjectType::PODCAST, std::bind(&DeviceBase::getSpeedPodCastCallback, this, _1, _2, _3));
        mPlayEngine->getPlaybackSpeed(mPlayEngine.mClientID, BaseType::tAudioObjectType::AUDIOBOOK, std::bind(&DeviceBase::getSpeedAudioBookCallback, this, _1, _2, _3));
}

void DeviceBase::openuriPlayAllCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::openuriPlayAllCallback :CallStatus : %d PlayerError %d", (int)status, (int)error);
}

void DeviceBase::playDeviceCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::playDeviceCallback :mediumId:%lu ,CallStatus : %d PlayerError %d", mMediumInfo.getMMediumId(), (int)status, (int)error);
   if (error == 0)
   {
   //	tempcount++;
   }
   else
   {
      //if (mSettings->getAMInteraction() && mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this)
      {
         //(mSourceSwitch->getSCI_Media())->raise_devErr(mMediumInfo.getMMediumId());
      }
   }
}


void DeviceBase::resumeCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::resumeCallback :mediumId:%lu ,CallStatus : %d PlayerError %d", mMediumInfo.getMMediumId(), (int)status, (int)error);
}


void DeviceBase::pauseCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::pauseCallback :mediumId:%lu ,CallStatus : %d PlayerError %d", mMediumInfo.getMMediumId(), (int)status, (int)error);
   /*if (mSettings->getAMInteraction() && error != PlayerTypes::PlayerError::NO_ERROR)
   {
      mSourceSwitch->getSCI_Media()->raise_devErr(mMediumInfo.getMMediumId());
      mSourceSwitch->runCycle();
   }*/
}

void DeviceBase::nextCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::nextCallback :CallStatus :mediumId:%lu , %d PlayerError %d", mMediumInfo.getMMediumId(), (int)status, (int)error);
}

void DeviceBase::nextChapterCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::nextChapterCallback :CallStatus :mediumId:%lu , %d PlayerError %d", mMediumInfo.getMMediumId(), (int)status, (int)error);
}

void DeviceBase::previousCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::previousCallback :mediumId:%lu ,CallStatus : %d PlayerError %d", mMediumInfo.getMMediumId(), (int)status, (int)error);
}

void DeviceBase::previousChapterCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::previousChapterCallback :CallStatus :mediumId:%lu , %d PlayerError %d", mMediumInfo.getMMediumId(), (int)status, (int)error);
}

void DeviceBase::stopCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::stopCallback :mediumId:%lu ,CallStatus : %d PlayerError %d", mMediumInfo.getMMediumId(), (int)status, (int)error);
   if (MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH == getDeviceType())
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::stopCallback :setmediumID to Zero done")
      mNowPlayingInfo.setMediumId(0);
   }
   /*if (status == CommonAPI::CallStatus::SUCCESS && error == PlayerTypes::PlayerError::NO_ERROR)
   {
       (mSourceSwitch->getSCI_Media())->raise_onStopped();
      mSourceSwitch->runCycle();
   }*/
}

void DeviceBase::seekToTimeCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::seekToTimeCallback :mediumId:%lu ,CallStatus : %d PlayerError %d", mMediumInfo.getMMediumId(), (int)status, (int)error);
}

void DeviceBase::seekCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::seekCallback :mediumId:%lu ,CallStatus : %d PlayerError %d", mMediumInfo.getMMediumId(), (int)status, (int)error);
}

void DeviceBase::setRepeatCallback(const CommonAPI::CallStatus& status, const PlayerTypes::RepeatStatus& mode)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::setRepeatCallback :mediumId:%lu ,CallStatus : %d RepeatStatus %d", mMediumInfo.getMMediumId(), (int)status, (int)mode);
}

void DeviceBase::setShuffleCallback(const CommonAPI::CallStatus& status, const PlayerTypes::ShuffleStatus& mode)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::setShuffleCallback :mediumId:%lu ,CallStatus : %d ShuffleStatus %d", mMediumInfo.getMMediumId(), (int)status, (int)mode);
}

void DeviceBase::fastForwardCallback(const CommonAPI::CallStatus& status, const double& rate)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::fastForwardCallback :mediumId:%lu ,CallStatus : %d rate: %f", mMediumInfo.getMMediumId(), (int)status, rate);
}

void DeviceBase::fastReverseCallback(const CommonAPI::CallStatus& status, const double& rate)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::fastReverseCallback :mediumId:%lu ,CallStatus : %d rate: %f", mMediumInfo.getMMediumId(), (int)status, rate);
}

void DeviceBase::dequeAllCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::dequeAllCallback :mediumId:%lu ,CallStatus : %d PlayerError %d", mMediumInfo.getMMediumId(), (int)status, (int)error);
}

#if 0
void DeviceBase::onPlaybackStatusChanged(const PlayerTypes::PlaybackStatus& status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onPlaybackStatusChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

    if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
         {
   LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onPlaybackStatusChanged - mediumId:%lu ,playback status: %d", mMediumInfo.getMMediumId(), (int)status);
   //MMPresTypes::ePlayState value;
   switch (status)
   {
   case PlayerTypes::PlaybackStatus::PAUSED:
      mPlayState = MMPresTypes::ePlayState::PLAYSTATE_PAUSED;
      cancelSeekTimer();
      if (mSettings->getAMInteraction() && mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this)
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onPlaybackStatusChanged - raise onPaused event");
      //	(mSourceSwitch->getSCI_Media())->raise_onPaused();
      //	mSourceSwitch->runCycle();
      }
      break;
   case PlayerTypes::PlaybackStatus::PLAYING:
      mPlayState = MMPresTypes::ePlayState::PLAYSTATE_PLAYING;
      cancelSeekTimer();
      if (mSettings->getAMInteraction() && mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this)
      {
        // LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onPlaybackStatusChanged - raise onPlaying event");
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d  <mediumId : %d , shuffle status : %d> ", __FUNCTION__, __LINE__,  mMediumInfo.getMMediumId(), (int)mShuffleMode);
         std::function<void(const MMPresTypes::eShuffleMode&)> callback = mPlayEngine->getShuffleModeStubCallback();
         if (callback != nullptr )
         {
            callback( this->mShuffleMode );
         }
         else
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::Shuffle Callback is NULL!! ");
         }
      //	(mSourceSwitch->getSCI_Media())->raise_onPlaying();
      //	mSourceSwitch->runCycle();
      }
      break;
   case PlayerTypes::PlaybackStatus::STOPPED:
      mPlayState = MMPresTypes::ePlayState::PLAYSTATE_STOPPED;
      cancelSeekTimer();
      if (mSettings->getAMInteraction() && mDetect->getActiveDevice() != nullptr && mDetect->getActiveDevice() == this)
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onPlaybackStatusChanged - raise onStopped event");
      //	(mSourceSwitch->getSCI_Media())->raise_onStopped();
      //	mSourceSwitch->runCycle();
      }
      break;
   default:
      mPlayState = MMPresTypes::ePlayState::PLAYSTATE_UNKNOWN;
      break;
   }
   //mPlayState = value; /*Useless Assignment CodeSonar warnings*/
   std::function<void(const MMPresTypes::ePlayState&)> callback = mPlayEngine->getPlayStateStubCallback();
   if (callback != nullptr)
   {
      callback(mPlayState);
   }
 }
}
#endif

void DeviceBase::onCurrentTrackChanged(const uint64_t& track)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onCurrentTrackChanged - mediumId:%lu , currentMediumId: %lu track %llu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId(), track);

   if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
   {
       mTrackState.setCurrentTrack(track);
       std::function<void(const MMPresTypes::tTrackState&)> callback = mPlayEngine->getTrackStateStubCallback();

      //Now we are simply sending the currentTrack.The totalTrack is currently not available from mediaOne srevice. So total track will be 0
      if ((callback != nullptr)&&
          ((mTrackState.getCurrentTrack()>0 && mTrackState.getTotalTracks()>0 && mTrackState.getCurrentTrack()<=mTrackState.getTotalTracks())))
      {
         cancelSeekTimer();
         int8_t temp_slotId = mDetect->findMatchingSlot(mMediumInfo.getMMediumId());
         if ((mPlayEngine->getSyncStatus(temp_slotId) >= MMPresTypes::eSyncState::STATE_FILE_PASS_SYNCED))
         {
            mPlayEngine->setActiveTrackNum(track);
         }
         callback(mTrackState);

         sendItemIndexTobeHighlighted(track);
      }
   } //   if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )

   if(mbShuffle == true)
   {
      std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();
      if (nowPlayingCallback != nullptr )
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onCurrentTrackChanged - sending nowPlaying event to HMI onShuffleChange");
         nowPlayingCallback(mNowPlayingInfo);
      }
      mbShuffle = false;
   }
}

void DeviceBase::onTotalTracksChanged(const uint64_t& totalTrack)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onTotalTracksChanged - mediumId:%lu, currentMediumId:%lu, total track count is %llu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId (), totalTrack);

   if ( mPlayEngine->getCurrentMediumId() == mMediumInfo.getMMediumId() )
  {
      if(totalTrack >0)
      {
         setNowPlayingTotalTrack(totalTrack);
      }

      mTrackState.setTotalTracks(totalTrack);
      std::function<void(const MMPresTypes::tTrackState&)> callback = mPlayEngine->getTrackStateStubCallback();

      if (callback != nullptr && mTrackState.getCurrentTrack()>0 && mTrackState.getTotalTracks()>0 && mTrackState.getCurrentTrack() <= mTrackState.getTotalTracks())
      {
         int8_t temp_slotId = mDetect->findMatchingSlot(mMediumInfo.getMMediumId());
         if ((mPlayEngine->getSyncStatus(temp_slotId) >= MMPresTypes::eSyncState::STATE_FILE_PASS_SYNCED))
         {
            mPlayEngine->setTotalTracks(totalTrack);
         }
         callback(mTrackState);
      }
  }
}

void DeviceBase::onPlayPositionChanged(const uint64_t& pos)
{
   //LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onPlayPositionChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

  if (mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId())
  {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onPlayPositionChanged play position is %llu", pos);
      mCurrentTime = pos / PLAY_POSITION_DIVISOR;
      if ((mTotalDuration>0) && (mCurrentTime <= mTotalDuration))
      {
         std::function<void(const uint64_t&)> callback = mPlayEngine->getCurrentTimeStubCallback();
         if (callback != nullptr)
         {
            callback(mCurrentTime);
         }
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onPlayPositionChanged - time condition violated ");
      }
  }
}

void DeviceBase::onPlayDurationChanged(const uint64_t& duration)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onPlayDurationChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

    if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
    {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onPlayDurationChanged -duration is %llu", duration);
      uint64_t totalDuration = duration / PLAY_POSITION_DIVISOR;
      mTotalDuration = totalDuration;
      if (mTotalDuration > 0 && mCurrentTime <= mTotalDuration)
      {
         std::function<void(const uint64_t&)> callback = mPlayEngine->getTotalDurationStubCallback();
         if (callback != nullptr)
         {
            callback(mTotalDuration);
         }
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onPlayDurationChanged - time condition violated ");
      }
    }
}

void DeviceBase::onMetadataChanged(const MediaTypes::ResultMap& metadataMap)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - mediumId:%lu CurrentmediumId:%lu  ", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId());

   if (mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId())
   {
      std::string displayName = "";
      std::string artist = "";
      std::string album = "";
      std::string genre = "";
      std::string albumart = "";
      std::string typeex = "";
      std::string path = "";
      std::string filename = "";
      std::string chapterPath = "";
      std::string composer = "";
      uint64_t chapterCount = 0;
      UInt64 fid = 0;
      uint64_t duration = 0;
      bool isFavourite = false;

      std::unordered_map<std::string, ::v1::org::genivi::mediamanager::MediaTypes::ResultUnion>::const_iterator itr;
      itr = metadataMap.find("DisplayName");
      if (itr != metadataMap.end())
      {
         displayName = itr->second.get<std::string>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - DisplayName:%s ", displayName.c_str());

         /*if (displayName.size() > 0)
         {
            sendBasicCategoryList();
         }*/
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No DisplayName");
      }
	  
	  itr =  metadataMap.find("Composer");
	  if (itr != metadataMap.end())
      {
         composer = itr->second.get<std::string>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - Composer:%s ", composer.c_str());
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No Composer");
      }
	  
      itr = metadataMap.find("URI");
      if (itr != metadataMap.end())
      {
         mCurrentPlayingUri = itr->second.get<std::string>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - URI %s", mCurrentPlayingUri.c_str());
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No URI");
      }

      itr = metadataMap.find("Album");
      if (itr != metadataMap.end())
      {
         album = itr->second.get<std::string>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - Album:%s ", album.c_str());
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No Album");
      }

      itr = metadataMap.find("Artists");
      if (itr != metadataMap.end())
      {
         artist = itr->second.get<std::string>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - Artist:%s ", artist.c_str());
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No Artist");
      }

      itr = metadataMap.find("Genre");
      if (itr != metadataMap.end())
      {
         genre = itr->second.get<std::string>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - Genre:%s ", genre.c_str());
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No Genre");
      }

      itr = metadataMap.find("AlbumArt");
      //Do Not Enable Coverart for Bluetooth
      if ((MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH != getDeviceType())&&(itr != metadataMap.end()))
      {
         albumart = itr->second.get<std::string>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - AlbumArt:%s ", albumart.c_str());
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No AlbumArt");
      }

      // duration not working for uint64_t??...metadata map gives error saying tht uint64_t is not supported.
      itr = metadataMap.find("Duration");
      if (itr != metadataMap.end())
      {
         if ((itr->second).isType<int64_t>())
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - duration is int64");
            duration = (uint64_t)(itr->second.get<int64_t>());
         }
         else if ((itr->second).isType<uint64_t>())
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - duration is uint64");
            duration = (uint64_t)(itr->second.get<uint64_t>());
         }
         else if ((itr->second).isType<int32_t>())
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - duration is int32");
            duration = (uint64_t)(itr->second.get<int32_t>());
         }
         else if ((itr->second).isType<uint32_t>())
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - duration is uint32");
            duration = (uint64_t)(itr->second.get<uint32_t>());
         }
         else
         {
         }
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - Duration:%llu", duration);
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No Duration");
      }

      itr = metadataMap.find("Type");
      if (itr != metadataMap.end())
      {
         mNowPlayingType = itr->second.get<std::string>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - type:%s ", mNowPlayingType.c_str());

   #if 0
         /* Resolution for Elvis ticket 2225991. CoC media does not update mVideoPlayStatus properly through attribute.
          * Below fix is to update mVideoPlayStatus depending upon whether audio or video playback.
          */
         if (mNowPlayingType.compare("video") == 0)
         {
    	     mVideoPlayStatus = true;
         }
         else
         {
    	     mVideoPlayStatus = false;
         }

         /* Above fix is not required now as "mVideoPlayStatus" is updated depending upon PlayerType attribute from CoC media.
          * If PlayerType is VIDEO then this is true else false.
          */
   #endif
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No metadata type");

         /* CoC media provides dummy onMetadataAttribute changed event where there is no metadata type
          * reset mNowPlayingType in case of no Metadata Type. So empty nowPlaying event will not be send to HMI
          */
         mNowPlayingType = "";
      }

      itr = metadataMap.find(BrowseHelper::ITEMID);
      if (itr != metadataMap.end())
      {
         if ((itr->second).isType<std::string>())
         {
            std::string sfid = (itr->second.get<std::string>());
            fid = (std::stoull(sfid));
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - ItemId is string :%llu ", fid);
         }
         else
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - ItemId wrong typeee");
         }
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No ItemId");
      }

      itr = metadataMap.find("TypeEx");
      if (itr != metadataMap.end())
      {
         typeex = itr->second.get<std::string>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - typeex:%s ", typeex.c_str());
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No metadata typeex");
      }

      itr = metadataMap.find("isFavorite");
      if (itr != metadataMap.end())
      {
         isFavourite = itr->second.get<bool>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - isFavourite:%d ", isFavourite);
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No isFavourite");
      }

      itr = metadataMap.find("Path");
      if (itr != metadataMap.end())
      { 
         path = itr->second.get<std::string>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - path:%s ", path.c_str());
           size_t pos = path.find("USB");
         if (pos != std::string::npos)
         {
            std::string findslash;
            std::string root = "root/";
            findslash = path.substr(pos);
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - findslash:%s ", findslash.c_str());
            std::size_t found = findslash.find_first_of("/");
            if (found != std::string::npos)
            {
              filename =findslash.substr(found + 1);
            }
            filename.insert(0,root);

             LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - filename:%s ", filename.c_str());
         }
         if(mBreadCrumbForNowPlaying == "/")
         {
           std::size_t found = path.find_last_of("/\\");
           std::string folder = path.substr(0,found);
           LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged Local- folder:%s ", folder.c_str());
          // setBreadCrumbForSetSession(folder);
         }
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No path");
      }
      itr = metadataMap.find("ChapterPath");
      if (itr != metadataMap.end())
      {
         chapterPath = itr->second.get<std::string>();
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - chapter path:%s ", chapterPath.c_str());
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No chapter path");
      }

      itr = metadataMap.find("ChapterCount");
      if (itr != metadataMap.end())
      {
         if ((itr->second).isType<int64_t>())
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - chapter count is int64");
            chapterCount =(uint64_t)(itr->second.get<int64_t>());
         }
         else if ((itr->second).isType<uint64_t>())
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - chapter count is uint64");
            chapterCount =(uint64_t)(itr->second.get<uint64_t>());
         }
         else if ((itr->second).isType<int32_t>())
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - chapter count is int32");
            chapterCount = (uint64_t)(itr->second.get<int32_t>());
         }
         else if ((itr->second).isType<uint32_t>())
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - chapter count is uint32");
            chapterCount = (uint64_t)(itr->second.get<uint32_t>());
         }
         else
         {
            //Do Nothing
         }
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - chapterCount:%llu", chapterCount);
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No ChapterCount");
      }


      /*if (chapterCount == 0 || chapterPath.empty())
      {
         isChapterAvailable = false;
      }
      else
      {
         isChapterAvailable = true;
      }*/

     /* Update CoC media for restrictedMode depending upon whether we have updated earlier or not.*/
      if(mPlayEngine->getDriverRestrictionsVideoStatusInformedToCoC() == false)
      {
         /* We have received driverRestrictionsVideo status from mpres plugin through vehicleSettings
          * We have not updated that one to CoC media as that time as none of the media device was active source. For e.g. booting up HU
          * At this moment, we have to inform CoC media for that status as one of the media device is not active source
          */
	     LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - setting RestrictedMode to CoC");
         setRestrictedMode();
         mPlayEngine->setDriverRestrictionsVideoStatusInformedToCoC(true);
      }

      MMPresTypes::tNowPlayingInfo nowPlayingInfo;
      MMPresTypes::eMetadataType metadatatype = PlayHelper::getNowPlayingItemType(mNowPlayingType);

      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - metadatatype:%lu",metadatatype);
      if((metadatatype == MMPresTypes::eMetadataType::METADATATYPE_VIDEO) && (chapterCount != 0))
      {
         mMultipleChapterAvailable = true;
      }
      else if ((chapterCount != 0) && (!chapterPath.empty()))
      {
         mMultipleChapterAvailable = true;
      }
      else
      {
         mMultipleChapterAvailable = false;
      }
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged:mMultipleChapterAvailable:%d",mMultipleChapterAvailable);

      if (MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN == metadatatype)
      {
         //Do Nothing as this is an invalid metadata problem. Will cause problems with HMI screen setting. Refer

	     /* below fix is for BT devices with AVRCP 1.0 where device deos not send metadata information to Cinemo.
	      * So in this case expectation is Cinemo to send empty metadata to mediaone. Mediaone to send empty metadata to mpres
	      * So mpres will send this empty metadata to HMI with metadata Type as "Title". With this HMI can enter into blank nowPlaying screen
	      * when such BT devices are selected for playback.
	      */
	      uint64_t id = mDetect->getActiveDeviceId();
	      int sourceId = mAudioInterface->getSourceID(id);

	      if(sourceId == am_t::am_sourceID_e::eSRC_BT_A2DP)
	      {
		      nowPlayingInfo.setNowPlayingItemType(MMPresTypes::eMetadataType::METADATATYPE_TITLE);
		      nowPlayingInfo.setAlbum("");
		      nowPlayingInfo.setArtist("");
		      nowPlayingInfo.setAudiobook("");
		      nowPlayingInfo.setChapterNumber(0);
		      nowPlayingInfo.setComposer("");
		      nowPlayingInfo.setEpisode("");
		      nowPlayingInfo.setFilename("");
		      nowPlayingInfo.setGenre("");
		      nowPlayingInfo.setPlaylist("");
		      nowPlayingInfo.setPodcast("");
		      nowPlayingInfo.setTitle("");
            nowPlayingInfo.setFid(0);
		      mNowPlayingInfo = nowPlayingInfo;

		      /* this is only for audio */
		      if(!mVideoPlayStatus)
		      {
			      std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();
			      if (nowPlayingCallback != nullptr)
			      {
				      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - sending empty nowPlaying event to HMI ");
				      nowPlayingCallback(nowPlayingInfo);
			      }
		      }
	      } //if(sourceId == am_t::am_sourceID_e::eSRC_BT_A2DP)
      }
      else
      {
         nowPlayingInfo.setNowPlayingItemType(metadatatype);
         std::string Unknowstr = "Unknown";
         checkUnknownAndReplace(Unknowstr, displayName);
         nowPlayingInfo.setTitle(displayName);
         checkUnknownAndReplace(Unknowstr, artist);
         nowPlayingInfo.setArtist(artist);
         checkUnknownAndReplace(Unknowstr, album);
         nowPlayingInfo.setAlbum(album);
         checkUnknownAndReplace(Unknowstr, genre);
         nowPlayingInfo.setGenre(genre);
         nowPlayingInfo.setFid(fid);
         nowPlayingInfo.setMediumId(mMediumInfo.getMMediumId());
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - setMediumId set");

         nowPlayingInfo.setIsFav(isFavourite);
         checkUnknownAndReplace(Unknowstr, composer);
         nowPlayingInfo.setComposer(composer);

         //For iTunes, Station name would be sent through the file name
         if (mNowPlayingType.compare("iTunesRadio") == 0)
         {
            itr = metadataMap.find("StationName");
            if (itr != metadataMap.end())
            {
               nowPlayingInfo.setFilename(itr->second.get<std::string>());
               LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - StationName %s", nowPlayingInfo.getFilename().c_str());
            }
            else
            {
               LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - No StationName");
            }
         }
         /* below fix is for only video file where filename is not provided by mediaone.
         * In this case, use displayname as filename in nowPlaying event
         */
         else if(filename.empty() && mVideoPlayStatus)
         {
	         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged:setting displayname as filename for video");
	         nowPlayingInfo.setFilename(displayName);
         }
         else
         {
	         nowPlayingInfo.setFilename(filename);
         }

         nowPlayingInfo.setChaptersAvailable(mMultipleChapterAvailable);

         mNowPlayingInfo = nowPlayingInfo;

         /* If video playback, do not send nowPlaying event to HMI immediately. Send nowPlaying event to HMI after "mVideoPlaybackStatusFromCoC" becomes true.
          * This is because HMI shows nowPlaying screen first (for a moment) and then "videoPlayStatus" attribute becomes true from CoC media so HMI shows video screen.
          * CoC media has added workaround to send "onMetadataChanged" event again once "videoPlayStatus" attribute becomes true from CoC media.
          * So below code will check that condition and accordingly nowPlaying event will be send to HMI for video playback.
          * mVideoPlayStatus is getting updated depending upon playerType from CoC media. If video then true else false.
          * Below are different cases:
          * 1) For audio playback, �mVideoPlayStatus� is false & mVideoPlaybackStatusFromCoC is false so �nowPlaying� event will be send asap.
          * 2) For video playback (without multiple chapters), 1st time �mVideoPlayStatus� is true & �mVideoPlaybackStatusFromCoC� is false,
          * so �nowPlaying� event will not be send to HMI asap. CoC media will send �videoPlayStatus� as true and hence �mVideoPlaybackStatusFromCoC� becomes �true�.
          * So 2nd time �onMetaDataChanged� api, �nowPlaying� event will be send to HMI.
          * 3) For video playback (with multiple chapters), for 2nd chapter  �onMetaDataChanged� will come from CoC media and
          * as �mVideoPlaybackStatusFromCoC� is true already and videoPlayer, so �nowPlaying� event will be sent to HMI for 2nd chapter details asap.
          *
          * Note:- Below workaround from mediapres is removed as CoC media send "videoPlayStatus" attribute as 1 for driving restriction
          * Because of mpres workaround and CoC media correct handling flicker of audio base screen & then actual video driver restriction is observed.
          * One more condition is added for driverRestrictions Video is enabled.
          * In this case, CoC media does not send "videoPlayStatus" attribute to true. So mpres was not sending nowPlaying event to HMI for video file.
          * So if videoPlayer type and driverRestrictionsVideo is enabled, then send nowPlaying event to HMI.
          */
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - mVideoPlayStatus=%d, mVideoPlaybackStatusFromCoC=%d", (int)mVideoPlayStatus, (int)(mPlayEngine->getVideoPlaybackStatusFromCoC()));

         if( (!mVideoPlayStatus) || (mVideoPlayStatus && mPlayEngine->getVideoPlaybackStatusFromCoC()) )
         {
	         std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();
	         if (nowPlayingCallback != nullptr )
	         {
		         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - sending nowPlaying event to HMI audio/video playback");
		         nowPlayingCallback(nowPlayingInfo);
	         }
         } //if((!mVideoPlayStatus) || (mVideoPlayStatus && mPlayEngine->getVideoPlaybackStatusFromCoC()))
   
         std::function<void(const MMPresTypes::eShuffleMode&)> callback = mPlayEngine->getShuffleModeStubCallback();
         if (callback != nullptr )
         {
            callback( this->mShuffleMode );
         }
         else
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::Shuffle Callback is NULL!! ");
         }

         if (duration > 0)
         {
            uint64_t totalDuration = duration / PLAY_POSITION_DIVISOR;
            mTotalDuration = totalDuration;

            /* If video playback, do not send durationCallback to HMI from here immediately.
             * Send this once mVideoPlaybackStatusFromCoC becomes true from CoC media and sent to HMI.
             */
            if( (!mVideoPlayStatus) || (mVideoPlayStatus && mPlayEngine->getVideoPlaybackStatusFromCoC()) )
            {
              if (mTotalDuration > 0 && mCurrentTime <= mTotalDuration)
              {
                 std::function<void(const uint64_t&)> durationCallback = mPlayEngine->getTotalDurationStubCallback();
                 if (durationCallback != nullptr )
                 {
                     LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - sending totalDuration event to HMI for audio/video playback");
                     durationCallback(mTotalDuration);
                 }
              }
            } //if((!mVideoPlayStatus) || (mVideoPlayStatus && mPlayEngine->getVideoPlaybackStatusFromCoC()))
         }

         /* If video playback, do not send albumArt from here to HMI immediately.
          * Send to HMI once "mVideoPlaybackStatusFromCoC" becomes true from CoC Media and send to HMI
          */
         mAlbumArt.assign(albumart);
         if( (!mVideoPlayStatus) || (mVideoPlayStatus && mPlayEngine->getVideoPlaybackStatusFromCoC()) )
         {
	         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - sending albumArt event to HMI for audio/video playback");

	         // this is just temporary
	         if (!albumart.compare(""))
	         {
		         setAlbumArt(0, albumart);
	         }
	         else
	         {
		         setAlbumArt(1, albumart);
	         }
         } // if(!mVideoPlayStatus)
         if (mNowPlayingType.compare("iTunesRadio") == 0)
         {
            bool promoteStatus = getPromoteStatus();
            std::function<void(const bool&)> promoteCallback = mPlayEngine->getPromoteStatusStubCallBack();
            if (promoteCallback != nullptr)
            {
               promoteCallback(promoteStatus);
            }

            bool demoteStatus = getDemoteStatus();
            std::function<void(const bool&)> demoteCallback = mPlayEngine->getDemoteStatusStubCallBack();
            if (demoteCallback != nullptr)
            {
               demoteCallback(demoteStatus);
            }

            bool addToWishListStatus = getAddToWishListStatus();
            std::function<void(const bool&)> addToWishListCallback = mPlayEngine->getAddToWishListStatusStubCallBack();
            if (addToWishListCallback != nullptr)
            {
               addToWishListCallback(addToWishListStatus);
            }
         }
      }
   }
   else
   {
      LOG_WARNING(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onMetadataChanged - Metadata broadcast received for inactive mediumID");		
   }
}

void DeviceBase::checkUnknownAndReplace(std::string &str1, std::string &str2)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::checkUnknownAndReplace  ");
	if ((str1.size() == str2.size()) && std::equal(str1.begin(), str1.end(), str2.begin(), [](char & c1,char & c2){
		return (c1 == c2 || std::toupper((unsigned char)c1) == std::toupper((unsigned char)c2));
			})){
 		str2.insert(str2.begin(), '$');
 		str2.insert(str2.end(), '$');
 		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::REPLACED %s",str2);
	}
}

void DeviceBase::onRepeatChanged(const PlayerTypes::RepeatStatus& status)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onRepeatChanged -mediumId:%lu , currentMediumId:%lu, repeat status:%d ", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId(), (int)status);

   //MMPresTypes::eRepeatMode mode;
  if (mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId())
   {
      switch (status)
      {
         case PlayerTypes::RepeatStatus::REPEAT:
            mRepeatMode = MMPresTypes::eRepeatMode::REPEAT_ALL;
            break;
         case PlayerTypes::RepeatStatus::REPEAT_SINGLE:
            mRepeatMode = MMPresTypes::eRepeatMode::REPEAT_SONG;
            break;
         case PlayerTypes::RepeatStatus::NO_REPEAT:
            mRepeatMode = MMPresTypes::eRepeatMode::REPEAT_OFF;
            break;
         case PlayerTypes::RepeatStatus::REPEAT_CHANGE:
            mRepeatMode = MMPresTypes::eRepeatMode::REPEAT_TOGGLE;
            break;
         default:
            mRepeatMode = MMPresTypes::eRepeatMode::REPEAT_UNKNOWN;
            break;
      }

      //mRepeatMode = mode; /* Useless assignment. CodeSonar warnings*/

      std::function<void(const MMPresTypes::eRepeatMode&)> callback = mPlayEngine->getRepeatModeStubCallback();
      if (callback != nullptr )
      {
         callback(mRepeatMode);
      }
   }
}

void DeviceBase::onShuffleChanged(const PlayerTypes::ShuffleStatus& status)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onShuffleChanged -mediumId:%lu, currentMediumId:%lu, shuffle status:%d ", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId(), (int)status);

   if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
   {
      //MMPresTypes::eShuffleMode mode;
       
       LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d  <mediumId : %d , shuffle status : %d> ", __FUNCTION__, __LINE__,  mMediumInfo.getMMediumId(), (int)status);
      switch (status)
      {
         case PlayerTypes::ShuffleStatus::SHUFFLE:
            mShuffleMode = MMPresTypes::eShuffleMode::SHUFFLE_ALL;
            mbShuffle = true;
            break;
         case PlayerTypes::ShuffleStatus::UNSHUFFLE:
            mShuffleMode = MMPresTypes::eShuffleMode::SHUFFLE_OFF;
            mbShuffle = true;
            break;
         default:
            mShuffleMode = MMPresTypes::eShuffleMode::SHUFFLE_UNKNOWN;
            break;
      }

      std::function<void(const MMPresTypes::eShuffleMode&)> callback = mPlayEngine->getShuffleModeStubCallback();
      if (callback != nullptr )
      {
         callback(mShuffleMode);
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::Shuffle Callback is NULL!! ");
      }
   }
}

void DeviceBase::onRateChanged(const double& rate)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onRateChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

    if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
    {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onRateChanged - mediumId:%lu ,rate:%f ", mMediumInfo.getMMediumId(), rate);
      std::map<std::pair<double, double>, MMPresTypes::eTrackSeekSpeed>::const_iterator itr;

      if (rate >= 0)
      {
         itr = rateToSeekSpeedConversionMap.find(std::make_pair(rate, MINUS_1*rate));
      }
      else
      {
         itr = rateToSeekSpeedConversionMap.find(std::make_pair(MINUS_1*rate, rate));
      }

      if (itr != rateToSeekSpeedConversionMap.end())
      {
         MMPresTypes::eTrackSeekSpeed speed;
         speed = itr->second;
         std::function<void(const MMPresTypes::eTrackSeekSpeed&)> speedCallback = mPlayEngine->getTrackSeekSpeedStubCallback();
         if (speedCallback != nullptr)
         {
            speedCallback(speed);
         }

         if (rate == PLAYING_SPEED)
         {
            mPlayState = MMPresTypes::ePlayState::PLAYSTATE_PLAYING;
         }
         else if (rate == PAUSED_SPEED)
         {
            mPlayState = MMPresTypes::ePlayState::PLAYSTATE_PAUSED;
         }
         else if (rate>FFWD_RATE_THRESHOLD)
         {
            mPlayState = MMPresTypes::ePlayState::PLAYSTATE_FASTFORWARD;
         }
         else if (rate <= FRWD_RATE_THRESHOLD)
         {
            mPlayState = MMPresTypes::ePlayState::PLAYSTATE_FASTREVERSE;
         }
         else
         {
            //Do Nothing
         }
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onRateChanged - mPlayState:%d", (int)mPlayState);

         std::function<void(const MMPresTypes::ePlayState&)> playstateCallback = mPlayEngine->getPlayStateStubCallback();
         if (playstateCallback != nullptr )
         {
            playstateCallback(mPlayState);
         }
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onRateChanged - unrecognized rate");
      }
   }
}

void DeviceBase::volumeCallback(const CommonAPI::CallStatus& status,const double& vol)

{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::volumeCallback");//Not Used Elvis:2322696
}

void DeviceBase::onPlayQueueChanged()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onPlayQueueChanged -mediumId:%lu", mMediumInfo.getMMediumId());
}

void DeviceBase::onCanGoNextChanged(const bool& status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onCanGoNextChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

   if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
    {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onCanGoNextChanged - mediumId:%lu ,status=%d", mMediumInfo.getMMediumId(), status);
      mCanGoNext = status;
    }
}

void DeviceBase::onCanGoPreviousChanged(const bool& status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onCanGoPreviousChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

    if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
    {
    
       LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onCanGoPreviousChanged -mediumId:%lu , status=%d", mMediumInfo.getMMediumId(), status);
       mCanGoPrevious = status;
    } 
}

void DeviceBase::onCanPauseChanged(const bool& status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onCanPauseChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

    if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
    {
       LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onCanPauseChanged - mediumId:%lu ,status=%d", mMediumInfo.getMMediumId(), status);
       mCanPause = status;
    }
}

void DeviceBase::onCanPlayChanged(const bool& status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onCanPlayChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

    if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
    {
       LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onCanPlayChanged - mediumId:%lu ,status=%d", mMediumInfo.getMMediumId(), status);
       mCanPlay = status;
    }
}

void DeviceBase::onCanSeekChanged(const bool& status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onCanSeekChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

    if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onCanSeekChanged - mediumId:%lu ,status=%d", mMediumInfo.getMMediumId(), status);
        mCanSeek = status;
    }
}

void DeviceBase::onSessionEndChanged(const bool& status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onSessionEndChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

    if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onSessionEndChanged - mediumId:%lu ,status=%d", mMediumInfo.getMMediumId(), status);
        mSessionEnd = status;
        if ((mSessionEnd)&&(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH != getDeviceType()))
         {
             uint8_t temp_slotid = mDetect->findMatchingSlot(mMediumInfo.getMMediumId());
             triggerMediaErrorToDevice(temp_slotid, MMPresTypes::eMediaError::MEDIAERROR_ENDOFPLAYLIST);
             clearMetadata();
         }
    }
}

bool DeviceBase::getVideoPlayStatus()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::getVideoPlayStatus - mVideoPlayStatus = %d", mVideoPlayStatus);

   return mVideoPlayStatus;
}

void DeviceBase::cancelSeekTimer()
{

}

void DeviceBase::setSeekTimer()
{

}

MMPresTypes::eDeviceType DeviceBase::getDeviceType()
{
   auto it = mDevTypeMapping.find(mMediumInfo.getMMediumType());
   if (mDevTypeMapping.end() != it)
   {
      return it->second;
   }
   else
   {
      return MMPresTypes::eDeviceType::DEVICETYPE_UKNOWN;
   }
}

void DeviceBase::deviceContainerUpdate(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
   uint16_t numContainersFound = map.size();
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdate:mediumId:%lu , Status : %d  Num Containers : %d Browse Error %d", mMediumInfo.getMMediumId(), stat, numContainersFound, err);
   std::string DisplayName;
   std::string Path;
   std::string Parent;
   std::string Type;
   std::string TypeEx;
   std::string uri = "";

   if (mSettings->getAttributeUsageForCount())
   {
      std::unordered_map<MMPresTypes::eMetadataType,std::string> mType;
      mType.clear();

      MediaTypes::ResultMapList resultList = map;

      auto it = resultList.begin();
      while (it != resultList.end())
      {
         const MediaTypes::ResultMap& result = *it;

         auto itp = result.find(BrowseHelper::URI);

         if (itp != result.end())
         {
            uri = itp->second.get<std::string>();
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdate-uri:%s", uri.c_str());
         }

         itp = result.find(BrowseHelper::PATH);
         if (itp != result.end())
         {
            Path = itp->second.get<std::string>();
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdate-path:%s", Path.c_str());
            std::string name = BrowseHelper::getFolderNameFromPath(Path);
            MMPresTypes::eMetadataType ContainerType = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;

            ContainerType = BrowseHelper::getContainerId(name,BrowseHelper::MUSIC_CATEGORY_NAME);
            if (ContainerType != MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN)
            {
               mType.insert(std::make_pair(ContainerType, Path));
               mRootlevelPlaybackUriMap[MMPresTypes::eTitleContentType::TITLETYPE_AUDIO][ContainerType] = uri;
               if (ContainerType == MMPresTypes::eMetadataType::METADATATYPE_FOLDER)
               {
                  mRootlevelPlaybackUriMap[MMPresTypes::eTitleContentType::TITLETYPE_AUDIO][MMPresTypes::eMetadataType::METADATATYPE_FILE] = uri;
               }
               else if(ContainerType == MMPresTypes::eMetadataType::METADATATYPE_TITLE)
               {
                  musicUri = uri;
                  LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdate-musicUri:%s", musicUri.c_str());
               }
               else
               {
                  //Do Nothing
               }
            }
            it++;
         }
         else
         {
            resultList.erase(it);
         }
      }// end of while
      std::unordered_map<MMPresTypes::eMetadataType, std::string>::iterator mitr = mType.begin();
      while (mitr != mType.end())
      {
         //mBrowse->getAttributes(mBrowse.mClientID,mitr->second, { BrowseHelper::CHILD_COUNT }, std::bind(&DeviceBase::categoryChildCountCallback, this, mitr->first, _1, _2, _3));
         mitr++;
      }
   }
   else
   {
      for (uint16_t i = 0; i < numContainersFound; i++)
      {
         const MediaTypes::ResultMap& result = map[i];

         auto it = result.find(BrowseHelper::URI);

         if (it != result.end())
         {
            uri = it->second.get<std::string>();
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdate-uri:%s", uri.c_str());
         }

         it = result.find(BrowseHelper::PATH);
         if (it != result.end())
         {
            Path = it->second.get<std::string>();
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdate-path:%s", Path.c_str());
            std::string name = BrowseHelper::getFolderNameFromPath(Path);
            MMPresTypes::eMetadataType ContainerType = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;

            ContainerType = BrowseHelper::getContainerId(name,BrowseHelper::MUSIC_CATEGORY_NAME);
            if (MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN != ContainerType)
            {
               mBrowseCaps |= ContainerType;
               it = result.find(BrowseHelper::CHILD_COUNT);
               int32_t n = 0;
               if (result.end() != it)
               {
                  n = it->second.get<std::int32_t>();
               }
               LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::deviceContainerUpdate-childCount:%lu",n);
               mAudioCategoriesCount[ContainerType] = (n > 0) ? n : 0u;
               mRootlevelPlaybackUriMap[MMPresTypes::eTitleContentType::TITLETYPE_AUDIO][ContainerType] = uri;
               if (ContainerType == MMPresTypes::eMetadataType::METADATATYPE_FOLDER)
               {
                  mRootlevelPlaybackUriMap[MMPresTypes::eTitleContentType::TITLETYPE_AUDIO][MMPresTypes::eMetadataType::METADATATYPE_FILE] = uri;
               }

               if (ContainerType == MMPresTypes::eMetadataType::METADATATYPE_TITLE)
               {
                  mContentInfoMap[MMPresTypes::eTitleContentType::TITLETYPE_AUDIO] = n;
                  mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(), mContentInfoMap);
               }
            }
         }
      }// end of for
   }
}

void DeviceBase::categoryChildCountCallback(const MMPresTypes::eMetadataType& type,const CommonAPI::CallStatus& status, const MediaTypes::ResultMap& map, const BrowserTypes::BrowserError& browseError)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::categoryChildCountCallback-mediumId=%lu,metadata type=%d,callStatus=%d,Browser error:%d,ResultMap size=%d",mMediumInfo.getMMediumId(),(int)type,(int)status,(int)browseError,(int)map.size());
   
   uint16_t num = map.size();
   uint64_t childCount = 0;
   
   if (browseError == BrowserTypes::BrowserError::NO_ERROR)
   {
      for (uint16_t i = 0; i < num; i++)
      {
         auto it = map.find(BrowseHelper::CHILD_COUNT);

         if (it != map.end())
         {
            if (it->second.isType<int32_t>())
            {
              childCount = (uint64_t)(it->second.get<std::int32_t>());
              LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::categoryChildCountCallback-32 bit ChildCount=%llu", childCount);
            }
            else if (it->second.isType<uint32_t>())
            {
              childCount = (uint64_t)(it->second.get<std::uint32_t>());
            }
            else if (it->second.isType<int64_t>())
            {
              childCount = (uint64_t)(it->second.get<std::int64_t>());
            }
            else if (it->second.isType<uint64_t>())
            {
              childCount = (uint64_t)(it->second.get<std::uint64_t>());
            }
            else
            {

            }
            if (type != MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN)
            {
               mAudioCategoriesCount[type] = childCount;
               if (type == MMPresTypes::eMetadataType::METADATATYPE_TITLE)
               {
                  mContentInfoMap[MMPresTypes::eTitleContentType::TITLETYPE_AUDIO] = childCount;
                  mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(),mContentInfoMap);
               }
            }
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::categoryChildCountCallback-metadata type=%d,childCount=%llu", (int)type, childCount);
           break;
         }
      }
   }
}

void DeviceBase::deviceContainerUpdateForVideo(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
   uint16_t numContainersFound = map.size();
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdateForVideo:mediumId:%lu , Status : %d  Num Containers : %d Browse Error %d", mMediumInfo.getMMediumId(), stat, numContainersFound, err);
   std::string DisplayName;
   std::string Path;
   std::string Parent;
   std::string Type;
   std::string TypeEx;
   std::string uri = "";

   if (mSettings->getAttributeUsageForCount())
   {
      std::unordered_map<MMPresTypes::eMetadataType, std::string> mType;
      mType.clear();

      MediaTypes::ResultMapList resultList = map;

      auto it = resultList.begin();
      while (it != resultList.end())
      {
         const MediaTypes::ResultMap& result = *it;

         //auto itp = result.find(BrowseHelper::PATH);
         auto itp = result.find(BrowseHelper::URI);

         if (itp != result.end())
         {
            uri = itp->second.get<std::string>();
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdate-uri:%s", uri.c_str());
         }

         itp = result.find(BrowseHelper::PATH);
         if (itp != result.end())
         {
            Path = itp->second.get<std::string>();
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdateForVideo-path:%s", Path.c_str());
            std::string name = BrowseHelper::getFolderNameFromPath(Path);
            MMPresTypes::eMetadataType ContainerType = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;

            ContainerType = BrowseHelper::getContainerId(name, BrowseHelper::VIDEO_CATEGORY_NAME);
            if (ContainerType != MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN)
            {
               mType.insert(std::make_pair(ContainerType, Path));
               mRootlevelPlaybackUriMap[MMPresTypes::eTitleContentType::TITLETYPE_VIDEO][ContainerType] = uri;
               if (ContainerType == MMPresTypes::eMetadataType::METADATATYPE_FOLDER)
               {
                mRootlevelPlaybackUriMap[MMPresTypes::eTitleContentType::TITLETYPE_VIDEO][MMPresTypes::eMetadataType::METADATATYPE_FILE] = uri;
               }
               if(ContainerType == MMPresTypes::eMetadataType::METADATATYPE_VIDEO)
               {
                  videoUri = uri;
                  LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdate-videoUri:%s", videoUri.c_str());
               }
            }
            it++;
         }
         else
         {
            resultList.erase(it);
         }
      }// end of while
      std::unordered_map<MMPresTypes::eMetadataType, std::string>::iterator mitr = mType.begin();
      while (mitr != mType.end())
      {
         //mBrowse->getAttributes(mBrowse.mClientID, mitr->second, { BrowseHelper::CHILD_COUNT }, std::bind(&DeviceBase::categoryChildCountCallbackForVideo, this, mitr->first, _1, _2, _3));
         mitr++;
      }
   }
   else
   {
      for (uint16_t i = 0; i < numContainersFound; i++)
      {
         const MediaTypes::ResultMap& result = map[i];
         auto it = result.find(BrowseHelper::PATH);
         if (it != result.end())
         {
            Path = it->second.get<std::string>();
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdateForVideo-path:%s", Path.c_str());
            std::string name = BrowseHelper::getFolderNameFromPath(Path);
            MMPresTypes::eMetadataType ContainerType = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;

            ContainerType = BrowseHelper::getContainerId(name, BrowseHelper::VIDEO_CATEGORY_NAME);
            if (MMPresTypes::eMetadataType::METADATATYPE_VIDEO == ContainerType)
            {
               mBrowseCaps |= ContainerType;
               it = result.find(BrowseHelper::CHILD_COUNT);
               int32_t n = 0;
               if (result.end() != it)
               {
                  n = it->second.get<std::int32_t>();
               }
               LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::deviceContainerUpdateForVideo-childCount:%lu", n);
               mVideoCategoriesCount[ContainerType] = (n > 0) ? n : 0u;
               mRootlevelPlaybackUriMap[MMPresTypes::eTitleContentType::TITLETYPE_VIDEO][ContainerType] = uri;
               if(ContainerType == MMPresTypes::eMetadataType::METADATATYPE_FOLDER)
               {
                mRootlevelPlaybackUriMap[MMPresTypes::eTitleContentType::TITLETYPE_VIDEO][MMPresTypes::eMetadataType::METADATATYPE_FILE] = uri;
               }
               mContentInfoMap[MMPresTypes::eTitleContentType::TITLETYPE_VIDEO] = n;
               mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(), mContentInfoMap);
               break;
            }
         }
      }// end of for
   }
}

void DeviceBase::categoryChildCountCallbackForVideo(const MMPresTypes::eMetadataType& type, const CommonAPI::CallStatus& status, const MediaTypes::ResultMap& map, const BrowserTypes::BrowserError& browseError)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::categoryChildCountCallbackForVideo-mediumId=%lu,metadata type=%d,callStatus=%d,Browser error:%d,ResultMap size=%d", mMediumInfo.getMMediumId(), (int)type, (int)status, (int)browseError, (int)map.size());

   uint16_t num = map.size();
   uint64_t childCount = 0;

   if (browseError == BrowserTypes::BrowserError::NO_ERROR)
   {
      for (uint16_t i = 0; i < num; i++)
      {
         auto it = map.find(BrowseHelper::CHILD_COUNT);

         if (it != map.end())
         {
            if (it->second.isType<int32_t>())
            {
               childCount = (uint64_t)(it->second.get<std::int32_t>());
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::categoryChildCountCallbackForVideo-32 bit ChildCount=%llu", childCount);
            }
            else if (it->second.isType<uint32_t>())
            {
               childCount = (uint64_t)(it->second.get<std::uint32_t>());
            }
            else if (it->second.isType<int64_t>())
            {
               childCount = (uint64_t)(it->second.get<std::int64_t>());
            }
            else if (it->second.isType<uint64_t>())
            {
               childCount = (uint64_t)(it->second.get<std::uint64_t>());
            }
            else
            {

            }
            if (type != MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN)
            {
               mVideoCategoriesCount[type] = childCount;
               if (type == MMPresTypes::eMetadataType::METADATATYPE_VIDEO)
               {
                  mContentInfoMap[MMPresTypes::eTitleContentType::TITLETYPE_VIDEO] = childCount;
                  mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(), mContentInfoMap);
               }
            }
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::categoryChildCountCallbackForVideo-metadata type=%d,childCount=%llu", (int)type, childCount);
            break;
         }
      }
   }
}

void DeviceBase::deviceContainerUpdateForImage(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
   uint16_t numContainersFound = map.size();
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdateForImage:mediumId:%lu , Status : %d  Num Containers : %d Browse Error %d", mMediumInfo.getMMediumId(), stat, numContainersFound, err);
   std::string DisplayName;
   std::string Path;
   std::string Parent;
   std::string Type;
   std::string TypeEx;

   if (mSettings->getAttributeUsageForCount())
   {
      std::unordered_map<MMPresTypes::eMetadataType, std::string> mType;
      mType.clear();

      MediaTypes::ResultMapList resultList = map;

      auto it = resultList.begin();
      while (it != resultList.end())
      {
         const MediaTypes::ResultMap& result = *it;

         auto itp = result.find(BrowseHelper::PATH);
         if (itp != result.end())
         {
            Path = itp->second.get<std::string>();
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdateForImage-path:%s", Path.c_str());
            std::string name = BrowseHelper::getFolderNameFromPath(Path);
            MMPresTypes::eMetadataType ContainerType = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;

            ContainerType = BrowseHelper::getContainerId(name, BrowseHelper::IMAGE_CATEGORY_NAME);
            if (ContainerType == MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN)
            {
               mType.insert(std::make_pair(ContainerType, Path));
               //break;
            }
            it++;
         }
         else
         {
            resultList.erase(it);
         }
      }// end of while
      std::unordered_map<MMPresTypes::eMetadataType, std::string>::iterator mitr = mType.begin();
      while (mitr != mType.end())
      {
         //mBrowse->getAttributes(mBrowse.mClientID, mitr->second, { BrowseHelper::CHILD_COUNT }, std::bind(&DeviceBase::categoryChildCountCallbackForImage, this, mitr->first, _1, _2, _3));
         mitr++;
      }
   }
   else
   {
      for (uint16_t i = 0; i < numContainersFound; i++)
      {
         const MediaTypes::ResultMap& result = map[i];
         auto it = result.find(BrowseHelper::PATH);
         if (it != result.end())
         {
            Path = it->second.get<std::string>();
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::deviceContainerUpdateForImage-path:%s", Path.c_str());
            std::string name = BrowseHelper::getFolderNameFromPath(Path);
            MMPresTypes::eMetadataType ContainerType = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;

            ContainerType = BrowseHelper::getContainerId(name, BrowseHelper::IMAGE_CATEGORY_NAME);
            if (MMPresTypes::eMetadataType::METADATATYPE_IMAGE == ContainerType)
            {
               mBrowseCaps |= ContainerType;
               it = result.find(BrowseHelper::CHILD_COUNT);
               int32_t n = 0;
               if (result.end() != it)
               {
                  n = it->second.get<std::int32_t>();
               }
               LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::deviceContainerUpdateForImage-childCount:%lu", n);
               mImageCategoriesCount[ContainerType] = (n > 0) ? n : 0u;
               mContentInfoMap[MMPresTypes::eTitleContentType::TITLETYPE_IMAGE] = n;
               mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(), mContentInfoMap);
               break;
            }
         }
      }// end of for
   }
}

void DeviceBase::categoryChildCountCallbackForImage(const MMPresTypes::eMetadataType& type, const CommonAPI::CallStatus& status, const MediaTypes::ResultMap& map, const BrowserTypes::BrowserError& browseError)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::categoryChildCountCallbackForImage-mediumId=%lu,metadata type=%d,callStatus=%d,Browser error:%d,ResultMap size=%d", mMediumInfo.getMMediumId(), (int)type, (int)status, (int)browseError, (int)map.size());

   uint16_t num = map.size();
   uint64_t childCount = 0;

   if (browseError == BrowserTypes::BrowserError::NO_ERROR)
   {
      for (uint16_t i = 0; i < num; i++)
      {
         auto it = map.find(BrowseHelper::CHILD_COUNT);

         if (it != map.end())
         {
            if (it->second.isType<int32_t>())
            {
               childCount = (uint64_t)(it->second.get<std::int32_t>());
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::categoryChildCountCallbackForImage-32 bit ChildCount=%llu", childCount);
            }
            else if (it->second.isType<uint32_t>())
            {
               childCount = (uint64_t)(it->second.get<std::uint32_t>());
            }
            else if (it->second.isType<int64_t>())
            {
               childCount = (uint64_t)(it->second.get<std::int64_t>());
            }
            else if (it->second.isType<uint64_t>())
            {
               childCount = (uint64_t)(it->second.get<std::uint64_t>());
            }
            else
            {

            }
            if (type != MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN)
            {
               mImageCategoriesCount[type] = childCount;
               if (type == MMPresTypes::eMetadataType::METADATATYPE_IMAGE)
               {
                  mContentInfoMap[MMPresTypes::eTitleContentType::TITLETYPE_IMAGE] = childCount;
                  mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(), mContentInfoMap);
               }
            }
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::categoryChildCountCallbackForImage-metadata type=%d,childCount=%llu", (int)type, childCount);
            break;
         }
      }
   }
}

void DeviceBase::SessionQueryCreateNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t & mediumID, const MMPresTypes::FilterIDMap & filterIDs, const MMPresTypes::FilterMap & filters, const MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask, const uint64_t& startIndex, const uint64_t& numItems)
{
   switch (type)
   {
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE:
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::SessionQueryCreateNew-SESSION_QUERY_FS_BROWSE-mediumId:%lu", mMediumInfo.getMMediumId());
      PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryFSNew, this, sessionID, _client, reply, gotoPreviousReply, jumpToQueryReply, mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
   }
   break;
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE:
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::SessionQueryCreateNew-SESSION_QUERY_METADATA_BROWSE-mediumId:%lu", mMediumInfo.getMMediumId());
      PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryMetadataNew, this, sessionID, _client, reply, gotoPreviousReply, jumpToQueryReply, mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
   }
   break;
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE:
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::SessionQueryCreateNew-SESSION_QUERY_NOWPLAYINGLIST_BROWSE-mediumId:%lu", mMediumInfo.getMMediumId());
        PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryNowPlayingNew, this, sessionID, _client, reply, gotoPreviousReply, jumpToQueryReply, mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
   }
   break;
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_HIGHESTRATED:
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::SessionQueryCreateNew-SESSION_QUERY_HIGHESTRATED-mediumId:%lu", mMediumInfo.getMMediumId());
      PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryHighestRatedNew, this, sessionID, _client, reply, gotoPreviousReply, jumpToQueryReply, mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
   }
   break;
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_RECENTLYPLAYED:
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::SessionQueryCreateNew-SESSION_QUERY_RECENTLYPLAYED-mediumId:%lu", mMediumInfo.getMMediumId());
      PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryRecentlyPlayedNew, this, sessionID, _client, reply, gotoPreviousReply, jumpToQueryReply, mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
   }
   break;
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_CATEGORY_BROWSE:
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::SessionQueryCreateNew-SESSION_QUERY_CATEGORY_BROWSE-mediumId:%lu", mMediumInfo.getMMediumId());
      PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryCategoryNew, this, sessionID, _client, reply, gotoPreviousReply, jumpToQueryReply, mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
   }
   break;
   default:
   {
      //TBD
   }
   break;
   }
}

void DeviceBase::SessionQueryCreateExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t & startIndex, const uint64_t& numItems)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowserStubImpl::jumpToSessionQuery- SessionQueryyCreateExisting %d : ",queryCacheData.mQueryType);
    switch (queryCacheData.mQueryType)
   {
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE:
   {
      PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryFSExisting, this, sessionID, queryID, _client, reply, gotoPreviousReply, jumpToQueryReply, queryCacheData, startIndex, numItems));
   }
   break;
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE:
   {
      PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryMetadataExisting, this, sessionID, queryID, _client, reply, gotoPreviousReply, jumpToQueryReply, queryCacheData, startIndex, numItems));
   }
   break;
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE:
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::SessionQueryCreateExisting-SESSION_QUERY_NOWPLAYINGLIST_BROWSE-mediumId:%lu", mMediumInfo.getMMediumId());
      PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryNowPlayingExisting, this, sessionID, queryID, _client, reply, gotoPreviousReply, jumpToQueryReply, queryCacheData, startIndex, numItems));
   }
   break;
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_HIGHESTRATED:
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::SessionQueryCreateExisting-SESSION_QUERY_HIGHESTRATED-mediumId:%lu", mMediumInfo.getMMediumId());
      PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryHighestRatedExisting, this, sessionID, queryID, _client, reply, gotoPreviousReply, jumpToQueryReply, queryCacheData, startIndex, numItems));
   }
   break;
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_RECENTLYPLAYED:
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::SessionQueryCreateExisting-SESSION_QUERY_RECENTLYPLAYED-mediumId:%lu", mMediumInfo.getMMediumId());
      PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryRecentlyPlayedExisting, this, sessionID, queryID, _client, reply, gotoPreviousReply, jumpToQueryReply, queryCacheData, startIndex, numItems));
   }
   break;
   case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_CATEGORY_BROWSE:
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::SessionQueryCreateExisting-SESSION_QUERY_CATEGORY_BROWSE-mediumId:%lu", mMediumInfo.getMMediumId());
      PresCtrl::addRequest(e_REQUEST_SESSION_QUERY_TASK, std::bind(&DeviceBase::SessionQueryCategoryExisting, this, sessionID, queryID, _client, reply, gotoPreviousReply, jumpToQueryReply, queryCacheData, startIndex, numItems));
   }
   break;
   default:
   {
      //TBD
   }
   break;
   }
}

RequestBase* DeviceBase::SessionQueryFSNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex, const uint64_t& numItems)
{
   return (new SessionQueryFS(sessionID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
}

RequestBase* DeviceBase::SessionQueryFSExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
{
   return (new SessionQueryFS(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems));
}

RequestBase* DeviceBase::SessionQueryMetadataNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex, const uint64_t& numItems)
{
   return (new SessionQueryMetadata(sessionID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
}

RequestBase* DeviceBase::SessionQueryMetadataExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
{
   return (new SessionQueryMetadata(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems));
}


RequestBase* DeviceBase::SessionQueryNowPlayingNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex, const uint64_t& numItems)
{
   return (new SessionQueryNowPlaying(sessionID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
}

RequestBase* DeviceBase::SessionQueryNowPlayingExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
{
   return (new SessionQueryNowPlaying(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems));
}

RequestBase* DeviceBase::SessionQueryHighestRatedNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex, const uint64_t& numItems)
{
   return (new SessionQueryHighestRated(sessionID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
}

RequestBase* DeviceBase::SessionQueryHighestRatedExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
{
   return (new SessionQueryHighestRated(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems));
}

RequestBase* DeviceBase::SessionQueryRecentlyPlayedNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex, const uint64_t& numItems)
{
   return (new SessionQueryRecentlyPlayed(sessionID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
}

RequestBase* DeviceBase::SessionQueryRecentlyPlayedExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
{
   return (new SessionQueryRecentlyPlayed(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems));
}

RequestBase* DeviceBase::SessionQueryCategoryNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex, const uint64_t& numItems)
{
   return (new SessionQueryCategory(sessionID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems));
}

RequestBase* DeviceBase::SessionQueryCategoryExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
{
   return (new SessionQueryCategory(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems));
}

void DeviceBase::updateAvailabilityToAudioManager(bool status)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase :updateAvailabilityToAudioManager: mMediumId=%lu", mMediumInfo.getMMediumId());
   if (mSettings->getAMInteraction())
   {
      bool ret = false;
      if (status == true)
      {
         ret = mAudioInterface->updateSourceAvailabilitytoAM(mMediumInfo.getMMediumId());
      }
      else
      {
         ret = mAudioInterface->updateSourceUnavailabilitytoAM(mMediumInfo.getMMediumId());
      }
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::updateAvailabilityToAudioManager: status=%d,ret=%d",status,ret);
   }
}

void DeviceBase::updateSourceInfoToAudioPres()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase :updateSourceInfoToAudioPres: mMediumId=%lu", mMediumInfo.getMMediumId());
   if (mSettings->getAMInteraction())
   {
      mAudioInterface->updateAudioSourceInfotoAudioPres(mMediumInfo.getMMediumId());
   }
}

MMPresTypes::tTrackState DeviceBase::getTrackState()
{
   return mTrackState;
}

uint64_t DeviceBase::getTotalDuration()
{
   return mTotalDuration;
}

uint64_t DeviceBase::getCurrentTime()
{
   return mCurrentTime;
}


bool DeviceBase::getCanNext()
{
   return mCanGoNext;
}

bool DeviceBase::getCanPrevious()
{
   return mCanGoPrevious;
}

bool DeviceBase::getCanPause()
{
   return mCanPause;
}

bool DeviceBase::getCanPlay()
{
   return mCanPlay;
}

bool DeviceBase::getCanSeek()
{
   return mCanSeek;
}

MMPresTypes::eTrackSeekSpeed DeviceBase::getTrackSeekSpeed()
{
   return mTrackSeekSpeed;
}

uint64_t DeviceBase::getDeviceId()
{
   return mMediumInfo.getMMediumId();
}

MMPresTypes::ePlayState DeviceBase::getPlayState()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase :getPlayState: mPlayState=%d", mPlayState);
   return mPlayState;
}

MMPresTypes::eRepeatMode DeviceBase::getRepeatMode()
{
   return mRepeatMode;
}

MMPresTypes::tNowPlayingInfo DeviceBase::getNowPlayingInfo()
{
   //LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getNowPlayingInfo: MediumID=%d", mNowPlayingInfo.getMediumId());
   return mNowPlayingInfo;
}

double DeviceBase::getCurrentRate()
{
   return mCurrentRate;
}

void DeviceBase::setPlayState(const MMPresTypes::ePlayState& playState)
{
   mPlayState = playState;
}


void DeviceBase::cancelTimers()
{
   cancelSeekTimer();
}

std::string DeviceBase::getBrowsePath()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase :getBrowsePath() :path : %s", mMediumInfo.getMDeviceBrowsePath().c_str() );
   return mMediumInfo.getMDeviceBrowsePath();
}

int32_t DeviceBase::getBrowseHistoryEnumeration()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase :getBrowseHistoryEnumeration: mBrowseHistoryEnumValue=%lu", mBrowseHistoryEnumValue);
   return mBrowseHistoryEnumValue;
}

const void DeviceBase::onRemotePlayerChanged()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase :onRemotePlayerChanged: Do Nothing");
}

void DeviceBase::setBrowseHistoryEnumeration(int32_t value)
{
   mBrowseHistoryEnumValue = value;
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase :setBrowseHistoryEnumeration: mBrowseHistoryEnumValue=%lu", mBrowseHistoryEnumValue);
}

MMPresTypes::eDeviceType DeviceBase::getDevType()
{
   MMPresTypes::eDeviceType devType = MMPresTypes::eDeviceType::DEVICETYPE_UKNOWN;
   std::unordered_map<v1::com::harman::media::BaseType::tMediumType, MMPresTypes::eDeviceType>::const_iterator itr = mDevTypeMapping.find(mMediumInfo.getMMediumType());
   if(itr != mDevTypeMapping.end())
   {
      devType = (itr->second);
   }
   return devType;
}

std::string DeviceBase::getDeviceName()
{
   return mMediumInfo.getMMediumName();
}

bool DeviceBase::getReadyToPlay()
{
   return mReadyToPlay;
}

void DeviceBase::setPlayPlanQueryId(uint32_t id)
{
   mPlayPlanQueryId = id;
}
uint32_t DeviceBase::getPlayPlanQueryId()
{
   return mPlayPlanQueryId;
}

std::list<uint32_t> DeviceBase::getPlayPlanQueryList()
{
   return mPlayPlanQueryList;
}

uint32_t DeviceBase::getPreviousPlayPlanQueryId(const uint32_t id)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::getPreviousPlayPlanQueryId-id=%lu", id);
   uint32_t queryid = 0;
   std::list<uint32_t>::const_iterator itr;
   for (itr = mPlayPlanQueryList.begin(); itr != mPlayPlanQueryList.end(); itr++)
   {
      if (id == *itr)
      {
         break;
      }
      queryid = (*itr);
   }
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::getPreviousPlayPlanQueryId-queryid=%lu", queryid);
   return queryid;
}


void DeviceBase::contructPlayPlan(std::list<uint32_t> queryList)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::contructPlayPlan");
   std::list<uint32_t>::const_iterator itr;
   mPlayPlanQueryList.clear();
   for (itr = queryList.begin(); itr != queryList.end(); itr++)
   {
      mPlayPlanQueryList.push_back(*itr);
      if (mPlayPlanQueryId == *itr)
      {
         break;
      }
   }
}

/*void DeviceBase::sendBasicCategoryList()
{
   MMPresTypes::MetadataCountMap::iterator itr;
   itr = mCategoriesCount.find(MMPresTypes::eMetadataType::METADATATYPE_ALBUM);
   if (itr != mCategoriesCount.end())
   {
      if (itr->second == 0)
      {
         mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_ALBUM] = 1;
         mBrowseCaps |=MMPresTypes::eMetadataType::METADATATYPE_ALBUM;
      }
   }
   else
   {
    mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_ALBUM] = 1;
    mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_ALBUM;
   }

   itr = mCategoriesCount.find(MMPresTypes::eMetadataType::METADATATYPE_ARTIST);
   if (itr != mCategoriesCount.end())
   {
      if (itr->second == 0)
      {
         mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_ARTIST] = 1;
         mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_ARTIST;
      }
   }
   else
   {
      mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_ARTIST] = 1;
      mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_ARTIST;
   }

   itr = mCategoriesCount.find(MMPresTypes::eMetadataType::METADATATYPE_GENRE);
   if (itr != mCategoriesCount.end())
   {
      if (itr->second == 0)
      {
         mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_GENRE] = 1;
         mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_GENRE;
      }
   }
   else
   {
      mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_GENRE] = 1;
      mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_GENRE;
   }

   itr = mCategoriesCount.find(MMPresTypes::eMetadataType::METADATATYPE_COMPOSER);
   if (itr != mCategoriesCount.end())
   {
      if (itr->second == 0)
      {
         mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_COMPOSER] = 1;
         mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_COMPOSER;
      }
   }
   else
   {
      mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_COMPOSER] = 1;
      mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_COMPOSER;
   }

   if (getDevType() != MMPresTypes::eDeviceType::DEVICETYPE_IPOD)
   {
      itr = mCategoriesCount.find(MMPresTypes::eMetadataType::METADATATYPE_FOLDER);
      if (itr != mCategoriesCount.end())
      {
         if (itr->second == 0)
         {
            mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_FOLDER] = 1;
            mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_FOLDER;
         }
      }
      else
      {
         mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_FOLDER] = 1;
         mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_FOLDER;
      }
   }

   itr = mCategoriesCount.find(MMPresTypes::eMetadataType::METADATATYPE_TITLE);
   if (itr != mCategoriesCount.end())
   {
      if (itr->second == 0)
      {
         mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_TITLE] = 1;
         mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_TITLE;
         mContentInfoMap[MMPresTypes::eTitleContentType::TITLETYPE_AUDIO] = 1;
         mDetect->deviceInformationUpdate(mMediumInfo.getMMediumId(), mContentInfoMap);
      }
   }
   else
   {
      mCategoriesCount[MMPresTypes::eMetadataType::METADATATYPE_TITLE] = 1;
      mBrowseCaps |= MMPresTypes::eMetadataType::METADATATYPE_TITLE;
   }
}*/

void DeviceBase::clearPlayPlan()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::clearPlayPlan-setting browse transition to default");
   setBrowseHistoryEnumeration(0);
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::clearPlayPlan-setting play plan availability to false");
   setPlayPlanQueryId(0);
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::clearPlayPlan-removing playplans from the active sessions");
   FacadeHelper<SessionManager> mSessionManager;
   std::unordered_map <uint32_t, Session*> sessionList = mSessionManager->getSessionList();
   std::unordered_map <uint32_t, Session*>::iterator itr= sessionList.begin();
   while(itr!=sessionList.end())
   {
      Session* sess = itr->second;
      if (sess != nullptr)
      {
         sess->removeFromPlayPlanDeviceList(mMediumInfo.getMMediumId());
      }
      itr++;
   }
}

BaseType::tMediumInfo DeviceBase::getMediumInfo()
{
   return mMediumInfo;
}

MMPresTypes::tAdditionalDeviceInfo DeviceBase::getAdditionalDeviceInfo()
{
   return mAdditionalDeviceInfo;
}

void DeviceBase::triggerMediaErrorToDevice(uint8_t slotId, MMPresTypes::eMediaError error)
{
   mPlayEngine->triggerMediaError(slotId, error);
}

bool DeviceBase::getResetMediaIndex()
{
   return mResetMediaIndex;
}

void DeviceBase::setResetMediaIndex(bool value)
{
   mResetMediaIndex = value;
}

void DeviceBase::setMasterMediaReset(bool factoryResetStatus)
{
   mPlayEngine->factoryResetEvent(factoryResetStatus);
}

string DeviceBase::getCurrentPlayingUri()
{
   return mCurrentPlayingUri;
}

std::string DeviceBase::getRootlevelPlaybackUri(const MMPresTypes::eTitleContentType& contentType, const MMPresTypes::eMetadataType& metadataType)
{
   std::string uri = "";
   auto ctr = mRootlevelPlaybackUriMap.find(contentType);
   if (ctr != mRootlevelPlaybackUriMap.end())
   {
      auto itr= (ctr->second).find(metadataType);
      if (itr != (ctr->second).end())
      {
         uri = itr->second;
      }
   }
   return uri;
}

void DeviceBase::setBreadCrumbForSetSession(std::string path)
{
  mBreadCrumbForNowPlaying = path;
}
std::string DeviceBase::getBreadCrumbForSetSession()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getBreadCrumbForSetSession - mBreadCrumbForNowPlaying:%s ", mBreadCrumbForNowPlaying.c_str());
    return mBreadCrumbForNowPlaying;
}

void DeviceBase::clearMetadata()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::clearMetadata");
    mTrackState.setCurrentTrack(0);
    mTrackState.setTotalTracks(0);
    mPlayEngine->setActiveTrackNum(0);
    mPlayEngine->setTotalTracks(0);
    std::function<void(const MMPresTypes::tTrackState&)> callbackTrackState = mPlayEngine->getTrackStateStubCallback();

    //Now we are simply sending the currentTrack.The totalTrack is currently not available from mediaOne srevice. So total track will be 0.
    if (callbackTrackState != nullptr)
    {
      cancelSeekTimer();
      callbackTrackState(mTrackState);
    }

    std::function<void(const uint64_t&)> callbackTimer = mPlayEngine->getCurrentTimeStubCallback();
    if (callbackTimer != nullptr)
    {
      callbackTimer(0);
    }

    callbackTimer = mPlayEngine->getTotalDurationStubCallback();
    if (callbackTimer != nullptr)
    {
       callbackTimer(0);
    }

    mNowPlayingInfo.setAlbum("");
    mNowPlayingInfo.setArtist("");
    mNowPlayingInfo.setAudiobook("");
    mNowPlayingInfo.setChapterNumber(0);
    mNowPlayingInfo.setComposer("");
    mNowPlayingInfo.setEpisode("");
    mNowPlayingInfo.setFilename("");
    mNowPlayingInfo.setGenre("");
    mNowPlayingInfo.setPlaylist("");
    mNowPlayingInfo.setPodcast("");
    mNowPlayingInfo.setTitle("");

    /* below is workaround from mpres side when BT device is not responding for playback.
     * In that case, timer in mpres expires and mpres informs no error to AudioManager so that BTSA will be active source only.
     * Then dummy nowPlaying event sent to HMI so HMI can enter blank nowPlaying screen.
     * But if nowPlayingItemType is other than UNKNOWN then only HMI enters nowPlaying screen.
     * Hence below change is done and that is for only BTSA when timer expires.
     */
    if(mSourceSwitch->getSourceSwitchTimerExpired())
    {
      uint64_t id = mDetect->getActiveDeviceId();
      int sourceId = mAudioInterface->getSourceID(id);

      if(sourceId == am_t::am_sourceID_e::eSRC_BT_A2DP)
      {
          LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::clearMetadata - for BTSA making nowPlaying item as Title - workaround");
      	mNowPlayingInfo.setNowPlayingItemType(MMPresTypes::eMetadataType::METADATATYPE_TITLE);
      }
    }

    std::function<void(const MMPresTypes::tNowPlayingInfo&)> nowPlayingCallback = mPlayEngine->getNowPlayingDataStubCallback();
    if (nowPlayingCallback != nullptr)
    {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::clearMetadata - sending nowPlaying event to HMI");
      nowPlayingCallback(mNowPlayingInfo);
    }
}

void DeviceBase::setRestrictedMode()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::setRestrictedMode");
	mPlayEngine->setRestrictedMode(mPlayEngine.mClientID, std::bind(&DeviceBase::restrictedModeCallback, this, _1, _2));
}

void DeviceBase::restrictedModeCallback(const CommonAPI::CallStatus& status, const PlayerTypes::RestrictedModeStatus& value)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::restrictedModeCallback : CallStatus : %d RestrictedModeStatus: %d", (int)status, (int)value);
}

void DeviceBase::getNowPlayingType(std::string & type)
{
   type = mNowPlayingType;
}

void DeviceBase::subscribeToPlayerType(bool subscribe)
{
   if (subscribe)
   {
      mPlayerTypeToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getPlayerTypeAttribute().getChangedEvent(), &DeviceBase::onPlayerTypeChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToPlayerType - subscription token: %d", mPlayerTypeToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToPlayerType - subscription token: %d", mPlayerTypeToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getPlayerTypeAttribute().getChangedEvent(), mPlayerTypeToken)
   }
}

void DeviceBase::onPlayerTypeChanged(const PlayerTypes::PlayerType &playerType)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onPlayerTypeChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

    if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
    {
       /* If playerType is video then set mVideoPlayer, else reset it. */
      if (playerType == PlayerTypes::PlayerType::VIDEO)
      {
         mVideoPlayStatus = true;
      }
      else
      {
         mVideoPlayStatus = false;
      }
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onPlayerTypeChanged - playerType=%d mVideoPlayStatus=%d", playerType, mVideoPlayStatus);
    }
}

void DeviceBase::subscribeToCanGetPlayQueue(bool subscribe)
{
   if (subscribe)
   {
	  /* subscribe to canGetPlayQueue attribute from CoC media which informs mpres whether nowPlaying list is available for current active device */
      mCanGetPlayQueueToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanGetPlayQueueAttribute().getChangedEvent(), &DeviceBase::onCanGetPlayQueueAttributeChanged, _1)
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::subscribeToCanGetPlayQueue - subscription token: %d", mCanGetPlayQueueToken);
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::un-subscribeToCanGetPlayQueue - subscription token: %d", mCanGetPlayQueueToken);
      UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getCanGetPlayQueueAttribute().getChangedEvent(), mCanGetPlayQueueToken)
   }
}

void DeviceBase::onCanGetPlayQueueAttributeChanged(const bool &nowPlayingListAvailable)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::onCanGetPlayQueueAttributeChanged - mediumId:%lu , currentMediumId: %lu", mMediumInfo.getMMediumId(), mPlayEngine->getCurrentMediumId ());

    if ( mPlayEngine->getCurrentMediumId () == mMediumInfo.getMMediumId() )
    {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::onCanGetPlayQueueAttributeChanged - mediumId:%lu ,nowPlayingListAvailable=%d", mMediumInfo.getMMediumId(), nowPlayingListAvailable);

      /* if nowPlayingList is available from CoC media, then inform HMI about the same through trackListSupport
	      * else inform HMI through trackListSupport that nowPlayingList is not available
	      */
      uint32_t mediumId  = mMediumInfo.getMMediumId();
      int8_t temp_slotid = mDetect->findMatchingSlot(mediumId);

      MMPresTypes::tTrackListSupport tmpTrackListSupport;

      mNowPlayingListSupported = nowPlayingListAvailable;

      if(nowPlayingListAvailable)
      {
	      tmpTrackListSupport.setIsTrackListSupported(true);
            setTrackListSupported(true);
	      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase : onCanGetPlayQueueAttributeChanged: nowPlayingList is available for SlotId %d",temp_slotid );
      }
      else
      {
	      tmpTrackListSupport.setIsTrackListSupported(false);
            setTrackListSupported(false);
	      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase : onCanGetPlayQueueAttributeChanged: nowPlayingList is not available for SlotId %d",temp_slotid );
      }

      tmpTrackListSupport.setSlotId(temp_slotid);

      std::function<void(const MMPresTypes::tTrackListSupport&)> callback = mPlayEngine->getTracklistSupportStubCallBack();
      if (callback != nullptr)
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase : onCanGetPlayQueueAttributeChanged: sending trackListSupport changed event to HMI");
         callback(tmpTrackListSupport);
      }
    }
}

bool DeviceBase::getNowPlayingListSupported()
{
    return mNowPlayingListSupported;
}

void DeviceBase::setTrackListSupported(bool track)
{
  mIsTrackListSupported = track;
}

bool DeviceBase::getTrackListSupported()
{
    return mIsTrackListSupported;
}


void DeviceBase::setNowPlayingTotalTrack(uint64_t track)
{
  mNowPlayingTotalTrack = track;
}
uint64_t DeviceBase::getNowPlayingTotalTrack()
{
  LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getNowPlayingTrack - mNowPlayingTotalTrack=%d", mNowPlayingTotalTrack);
  return mNowPlayingTotalTrack;
}

MMPresTypes::tPlaybackSpeed DeviceBase::getSpeedInfoObj()
{
   return mSpeedInfo;
}

void DeviceBase::setSlotForAudiobookPodcast(int8_t slot)
{
  mSlotIdForPodcastAudiobook = slot;
}
int8_t DeviceBase::getSlotForAudiobookPodcast()
{
  return mSlotIdForPodcastAudiobook;
}
void DeviceBase::setMetadatType(MMPresTypes::eMetadataType mtype)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::setMetadatType - mtype=%d", mtype);
  mTypeForPodcastAudiobook = mtype;
}
MMPresTypes::eMetadataType DeviceBase::getMetadataType()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getMetadataType - mTypeForPodcastAudiobook=%d", mTypeForPodcastAudiobook);
  return mTypeForPodcastAudiobook;
}

void DeviceBase::getSpeedAudioBookCallback(const CommonAPI::CallStatus& status, const double& speed, const ::v1::com::harman::media::BaseType::tBaseError& error)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getSpeedAudioBookCallback: status=%d,speed=%f,error=%d",(int)(status),speed,(int)error);
   MMPresTypes::ePlaybackSpeed playSpeed = MMPresTypes::ePlaybackSpeed::NORMAL;
   if (error == BaseType::tBaseError::S_OK)
   {
   //Changes for setSpeed for Audiobook/Podcast
        if (speed < 1)
		 {
			 playSpeed = MMPresTypes::ePlaybackSpeed::SLOWER;
		 }
		 else if (speed > 1)
		 {
			 playSpeed = MMPresTypes::ePlaybackSpeed::FASTER;
		 }
             
      mSpeedInfo.setSlotId(mDetect->findMatchingSlot(mMediumInfo.getMMediumId()));
	   mSpeedInfo.setType(MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK);
	   mSpeedInfo.setPlaybackSpeed(playSpeed);
      std::function<void(const MMPresTypes::tPlaybackSpeed&)> speedInfoCallback = mPlayEngine->getSpeedInfoStubCallback();
	   if(speedInfoCallback != nullptr)
	   {
		speedInfoCallback(mSpeedInfo);
	   }
	   else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::getSpeedAudioBookCallback - speedInfoCallback is null");
	   }
      LOG_INFO(LOG_CONTEXT_MEDIA,"DeviceBase::getSpeedAudioBookCallback - playSpeed:%d",playSpeed);
   }
}

void DeviceBase::getSpeedPodCastCallback(const CommonAPI::CallStatus& status, const double& speed, const ::v1::com::harman::media::BaseType::tBaseError& error)
{
  LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getSpeedPodCastCallback: status=%d,speed=%f,error=%d",(int)(status),speed,(int)error);
  MMPresTypes::ePlaybackSpeed playSpeed = MMPresTypes::ePlaybackSpeed::NORMAL;
   if (error == BaseType::tBaseError::S_OK)
   {
   //Changes for setSpeed for Audiobook/Podcast
             
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getSpeedPodCastCallback: speed=%f",speed);
      if (speed < 1)
      {
         playSpeed = MMPresTypes::ePlaybackSpeed::SLOWER;
	   }
	   else if (speed > 1)
	   {
	      playSpeed = MMPresTypes::ePlaybackSpeed::FASTER;
	   }
      mSpeedInfo.setSlotId(mDetect->findMatchingSlot(mMediumInfo.getMMediumId()));
      mSpeedInfo.setType(MMPresTypes::eMetadataType::METADTATATYPE_PODCAST);
      mSpeedInfo.setPlaybackSpeed(playSpeed);
      std::function<void(const MMPresTypes::tPlaybackSpeed&)> speedInfoCallback = mPlayEngine->getSpeedInfoStubCallback();
      if(speedInfoCallback != nullptr)
      {
         speedInfoCallback(mSpeedInfo);
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceBase::getSpeedPodCastCallback - speedInfoCallback is null");
      }
      LOG_INFO(LOG_CONTEXT_MEDIA,"DeviceBase::getSpeedPodCastCallback - playSpeed:%d",playSpeed);  
   }
}

void DeviceBase::setSmartSearchAlphaJumpCandidateRequiredToBeSend(bool value)
{
	mSmartSearchAlphaJumpCandidateRequiredToBeSend = value;
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::setSmartSearchAlphaJumpCandidateRequiredToBeSend - mSmartSearchAlphaJumpCandidateRequiredToBeSend=%d", mSmartSearchAlphaJumpCandidateRequiredToBeSend);
}

bool DeviceBase::getSmartSearchAlphaJumpCandidateRequiredToBeSend()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getSmartSearchAlphaJumpCandidateRequiredToBeSend - mSmartSearchAlphaJumpCandidateRequiredToBeSend=%d", mSmartSearchAlphaJumpCandidateRequiredToBeSend);
   return mSmartSearchAlphaJumpCandidateRequiredToBeSend;
}

void DeviceBase::sendIAP2KeyEvent(::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType buttonType, bool pressed)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::sendIAP2KeyEvent - Do Nothing");
}

void DeviceBase::promote(uint64_t fileid)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::promote - Do Nothing");
}

void DeviceBase::demote(uint64_t fileid)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::demote - Do Nothing");
}

void DeviceBase::addToWishList(uint64_t fileid)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::addToWishList - Do Nothing");
}

bool DeviceBase::getPromoteStatus()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getPromoteStatus - Do Nothing");
   return false;
}

void DeviceBase::setPromoteStatus(bool _value)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::setPromoteStatus - Do Nothing");
}

bool DeviceBase::getDemoteStatus()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getDemoteStatus - Do Nothing");
   return false;
}

void DeviceBase::setDemoteStatus(bool _value)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::setDemoteStatus - Do Nothing");
}

bool DeviceBase::getAddToWishListStatus()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getAddToWishListStatus - Do Nothing");
   return false;
}

void DeviceBase::setAddToWishListStatus(bool _value)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::setAddToWishListStatus - Do Nothing");
}

bool DeviceBase::getIsDeviceEmpty()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::getIsDeviceEmpty - mIsDeviceEmpty=%d", (int)mIsDeviceEmpty);
	return mIsDeviceEmpty;
}

void DeviceBase::setIsDeviceEmpty(bool value)
{
    mIsDeviceEmpty = value;
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::setIsDeviceEmpty - mIsDeviceEmpty=%d", (int)mIsDeviceEmpty);
}

void DeviceBase::setAudiobookPodcast(bool val)
{
	bAudioPodcastVal = val;
}

bool DeviceBase::getAudiobookPodcast()
{
   return bAudioPodcastVal;
}

void DeviceBase::gotoTrack(const uint64_t trackPosition)
{
   const uint64_t lcurrentTrack = getTrackState().getCurrentTrack();

   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceBase::gotoTrack ReqtrackPosition %llu currentTrack %llu ", trackPosition, lcurrentTrack);

   // Session Boundary situation
   if ((trackPosition == 0)&& (getRepeatMode() == MMPresTypes::eRepeatMode::REPEAT_OFF))
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

   /*
   HMI was calling next or previous multiple times causing track change .
   Therefore introduced new COC media API "seek" to handle the scenario. This would be removed permanently after sometime*/
#if 0
   // Prev() and next() call can be used
   else if (1 >= (std::max(lcurrentTrack, trackPosition) - std::min(lcurrentTrack, trackPosition)))
   {
      if (lcurrentTrack < trackPosition)
      {
         next(STEPCOUNT_ONE);
      }
      else
      {
         previous(STEPCOUNT_ONE);
      }
   }
#endif
   else // Multiple tracks skip
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< After MediaBrowserStubImpl::gotoTrack ReqtrackPosition %llu currentTrack %llu ", trackPosition, lcurrentTrack);
      seek(trackPosition);
   }
}

void DeviceBase::sendItemIndexTobeHighlighted(const uint64_t& track)
{
    /* Below code is to send current playing track to HMI only if current queryData is for nowPlaying browsing
     * This is when user is in nowPlaying list browsing window and next/previous track is played using button panel by user */

    FacadeHelper<SessionManager> mSessionManager;
    MMPresTypes::HighlightItemIndexMap highlightItemIndexMap;

    highlightItemIndexMap.clear();

    std::unordered_map <uint32_t, Session*> sessionList = mSessionManager->getSessionList();
    std::unordered_map <uint32_t, Session*>::iterator itr= sessionList.begin();

    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::sendItemIndexTobeHighlighted - current track number is %llu",track);

    while(itr != sessionList.end())
    {
        Session* sess = itr->second;
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::sendItemIndexTobeHighlighted - sessionId = %lu", itr->first);

        if (sess != nullptr)
        {
           Session::SessionQueryData* currData = nullptr;
           uint32_t currQueryId = sess->getCurrentQueryId();
           currData    = sess->getSessionQueryData(currQueryId);
           if(currData != nullptr)
           {
                LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::sendItemIndexTobeHighlighted - currData->mQueryType = %d", currData->mQueryType);

      		  if(currData->mQueryType == MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE)
      		  {
      			  LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::sendItemIndexTobeHighlighted - Insertion to Map: trackNumber = %llu", track);
      			  highlightItemIndexMap[itr->first] = track;
      		  }
      	  }

        } //if (sess != nullptr)

  	  itr++;

    } //while(itr != sessionList.end())

    for(auto& iterator: highlightItemIndexMap)
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::sendItemIndexTobeHighlighted - trackNumber = %llu for sessionId = %lu from highlightItemIndexMap", iterator.second, iterator.first);
    }

    if(!highlightItemIndexMap.empty())
    {
      std::function<void(const MMPresTypes::HighlightItemIndexMap&)> itemIndexTobeHighlightedStubCallBack = mBrowse->getItemIndexTobeHighlightedStubCallBack();

      if(itemIndexTobeHighlightedStubCallBack != nullptr)
      {
  	      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceBase::sendItemIndexTobeHighlighted - sending highlightItemIndexMap to HMI");
  	      itemIndexTobeHighlightedStubCallBack(highlightItemIndexMap);
      }
    } //if(!highlightItemIndexMap.empty())
}
