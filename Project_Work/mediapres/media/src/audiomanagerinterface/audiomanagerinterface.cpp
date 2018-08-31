#include <spot/harman_kpimarker.h>
#include<audiomanagerinterface/audiomanagerinterface.h>
#include<device/DeviceBase.h>

// this contains a map between string sourceId and the actual sourceId
std::unordered_map<std::string, am_t::am_sourceID_e> AudioManagerInterface::sourceEnumMap = {
    {"eSRC_USB1",am_t::am_sourceID_e::eSRC_USB1}
   ,{"eSRC_USB2",am_t::am_sourceID_e::eSRC_USB2}
   ,{"eSRC_FU_IPOD",am_t::am_sourceID_e::eSRC_FU_IPOD }
   ,{"eSRC_FU_IPOD2",am_t::am_sourceID_e::eSRC_FU_IPOD2 }
   ,{"eSRC_BT_A2DP",am_t::am_sourceID_e::eSRC_BT_A2DP}
   ,{"eSRC_CDA",am_t::am_sourceID_e::eSRC_CDA}
};

// this contains a map between sourceId and it's corresponding slot Type
std::unordered_map<am_t::am_sourceID_e, MMPresTypes::eSlotType> AudioManagerInterface::sourceToSlotMap = {
	{am_t::am_sourceID_e::eSRC_USB1,MMPresTypes::eSlotType::SLOTTYPE_USB}
	,{am_t::am_sourceID_e::eSRC_USB2,MMPresTypes::eSlotType::SLOTTYPE_USB}
   ,{ am_t::am_sourceID_e::eSRC_FU_IPOD,MMPresTypes::eSlotType::SLOTTYPE_IPOD }
   ,{ am_t::am_sourceID_e::eSRC_FU_IPOD2,MMPresTypes::eSlotType::SLOTTYPE_IPOD }
	,{am_t::am_sourceID_e::eSRC_BT_A2DP,MMPresTypes::eSlotType::SLOTTYPE_BLUETOOTH}
	,{am_t::am_sourceID_e::eSRC_CDA, MMPresTypes::eSlotType::SLOTTYPE_REMOTE_CD}
};

// this contains a map between sourceId and it's corresponding slot Type
std::unordered_map<am_t::am_sourceID_e, audioPresCtrlTypes::eEntertainmentSrcs> AudioManagerInterface::AMtoAudioPresSourceMap = {
	{ am_t::am_sourceID_e::eSRC_USB1,audioPresCtrlTypes::eEntertainmentSrcs::SRC_USB1}
	,{ am_t::am_sourceID_e::eSRC_USB2,audioPresCtrlTypes::eEntertainmentSrcs::SRC_USB2}
   ,{ am_t::am_sourceID_e::eSRC_FU_IPOD,audioPresCtrlTypes::eEntertainmentSrcs::SRC_IPOD1}
   ,{ am_t::am_sourceID_e::eSRC_FU_IPOD2,audioPresCtrlTypes::eEntertainmentSrcs::SRC_IPOD2 }
	,{ am_t::am_sourceID_e::eSRC_BT_A2DP,audioPresCtrlTypes::eEntertainmentSrcs::SRC_BT_A2DP1}
	,{am_t::am_sourceID_e::eSRC_CDA, audioPresCtrlTypes::eEntertainmentSrcs::SRC_CDA}
};

std::unordered_map<MMPresTypes::eDeviceType, audioPresCtrlTypes::eEntSourceType> AudioManagerInterface::DeviceTypetoAudioSourceType = {
	{ MMPresTypes::eDeviceType::DEVICETYPE_USB,audioPresCtrlTypes::eEntSourceType::TYPE_USB_STORAGE}
	,{ MMPresTypes::eDeviceType::DEVICETYPE_IPOD,audioPresCtrlTypes::eEntSourceType::TYPE_IPOD}
	,{ MMPresTypes::eDeviceType::DEVICETYPE_MTP,audioPresCtrlTypes::eEntSourceType::TYPE_USB_MTP}
	,{ MMPresTypes::eDeviceType::DEVICETYPE_REMOTE_CD,audioPresCtrlTypes::eEntSourceType::TYPE_MEDIA}
};


AudioManagerInterface::AudioManagerInterface():mbTimerTimeExpired(true)
, mtimerId(0)
, mTimer(*this)
, mActiveDeviceBTLinkLossStubCallback(nullptr)
, mRoutingControlObserver("local","audioManager.routingInst")
, mAudioSoureInfo("local","AudioPresCtrl.srcInfoInst")
, mCommandControl("local", "audioManager.commandInst")
{
	if(mediaSettings->getAMInteraction())
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::AudioManagerInterface");
		asyncSourceStateCallback = nullptr;
		mDomainRegistratioComplete = false;
		mSourceRegistrationComplete = false;
      onGoingCall = false;
      mMainSourceSoundPropertyChangedSignalHandle = false;
		initDomain();
		initSources();
		subscribetoProxyStatus(true);
		subscribeToAudioSourceInfoProxy(true);
		subscribeToCommandControlProxy(true);
      mBTAvailabilityReason = am_t::am_AvailabilityReason_e::USER_DISCONNECTED;
		mBTDisconnectReason = am_t::am_Error_e::E_OK;
		mNumClients = 0;
	}
}

AudioManagerInterface::~AudioManagerInterface()
{
	if (mediaSettings->getAMInteraction())
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::~AudioManagerInterface");
		subscribeToRoutingReadyStatus(false);
		subscribetoProxyStatus(false);
		deRegisterSources();
		mRoutingControlObserver->deregisterDomainAsync(mDomainId, std::bind(&AudioManagerInterface::deRegisterDomainAsyncCallback, this, _1));
		mDomainRegistratioComplete = false;
		mSourceRegistrationComplete = false;
	}
}

void AudioManagerInterface::initSources()
{
   std::list<std::string> sourceList = mediaSettings->getSupportedSources();
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::initSources-source list size=%d", sourceList.size());
   std::unordered_map<std::string, am_t::am_sourceID_e>::iterator itr;
   std::unordered_map<am_t::am_sourceID_e, MMPresTypes::eSlotType>::iterator itr1;
   for (std::list<std::string>::iterator ctr = sourceList.begin(); ctr != sourceList.end(); ctr++)
   {
      itr = sourceEnumMap.find(*ctr);
      if (itr != sourceEnumMap.end())
      {
         itr1 = sourceToSlotMap.find(itr->second);
         if (itr1 != sourceToSlotMap.end())
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::initSources-source name=%s", (*ctr).c_str());
            mSourceList.push_back(Source(itr->second, itr1->second, (*ctr)));
         }
         else
         {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::initSources-source not found in source slot map");
         }
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::initSources-source not found in source enum map");
      }
   }
}

void AudioManagerInterface::subscribetoProxyStatus(bool subscribe)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::subscribetoProxyStatus-subscribe=%d",subscribe);
	static uint32_t token = 0;
	if (subscribe)
	{
		token=SUBSCRIBE(mRoutingControlObserver, getProxyStatusEvent(), &AudioManagerInterface::RoutingControlObserverProxyavailibility, _1)
	}
	else
	{
		UNSUBSCRIBE(mRoutingControlObserver,getProxyStatusEvent(), token)
	}
}

void AudioManagerInterface::subscribeToRoutingReadyStatus(bool subscribe)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::subscribeToRoutingReadyStatus-subscribe=%d", subscribe);
	static uint32_t token = 0;
	if (subscribe)
	{
		token = SUBSCRIBE(mRoutingControlObserver, getRoutingReadyAttribute().getChangedEvent(), &AudioManagerInterface::RoutingReadyAttributeCallback, _1)
	}
	else
	{
		UNSUBSCRIBE(mRoutingControlObserver, getRoutingReadyAttribute().getChangedEvent(), token)
	}
}

void AudioManagerInterface::initDomain()
{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::initDomain");
		mDomainData.setDomainID( domainId_ );
		mDomainData.setName( domainName );
		mDomainData.setEarly(true);
		mDomainData.setComplete(false);
		mDomainData.setState( mDomainState );
		mBusName = BUS_NAME;
		mDomainId = 0;
}

void AudioManagerInterface::RoutingControlObserverProxyavailibility(const CommonAPI::AvailabilityStatus& avail)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::RoutingControlObserverProxyavailibility - routingcontrolobserver proxy is %s", mRoutingControlObserver.getStringAvailibility(avail).c_str());
	mRoutingControlObserverAvailableStatus = avail;
	if (mRoutingControlObserverAvailableStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		startAMProxyTasks();
	}
	else
	{
		mDomainRegistratioComplete = false;
		mSourceRegistrationComplete = false;
	}
}

void AudioManagerInterface::startAMProxyTasks()
{
	subscribeToRoutingReadyStatus(true);
}

void AudioManagerInterface::RoutingReadyAttributeCallback(const ::v3::org::genivi::am_t::am_RoutingReady_e& routingReady)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::RoutingReadyAttributeCallback - RoutingReady value is %d", (int)routingReady);
	if (routingReady == am_t::am_RoutingReady_e::RR_READY && !mDomainRegistratioComplete)
	{
        if ( mRoutingControlObserver -> isAvailable ()  )
        {
           if ( ( mediaSettings -> is_valid_utf8 ( ( mDomainData . getName () ).c_str () ) ) && ( mediaSettings -> is_valid_utf8 ( mBusName .c_str() )) )  
           {
              LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface: %s %d <busName : %s>", __FUNCTION__, __LINE__, mBusName.c_str ()  ); 
              mRoutingControlObserver->registerDomainAsync(mDomainData, mBusName,"",std::bind(&AudioManagerInterface::registerDomainAsyncCallback,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
           }
           else
           {
               LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface: %s %d <busName : %s not a valid utf8 > ", __FUNCTION__, __LINE__, mBusName.c_str()); 
           }
        }
	}
}

void AudioManagerInterface::registerDomainAsyncCallback(const CommonAPI::CallStatus& status, const ::v3::org::genivi::am_t::am_domainID_t& domainId, const ::v3::org::genivi::am_t::am_Error_e& error)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerDomainAsyncCallback - status:%d domainId:%d error:%d ", status,domainId,error);
	mDomainId = domainId;
	if (error == am_t::am_Error_e::E_OK)
	{
		mDomainRegistratioComplete = true;
		if (!mSourceRegistrationComplete)
		{
			registerSources();
		}
	}
}

void AudioManagerInterface::registerSources()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSources-sourceListSize=%d",mSourceList.size());
	am_t::am_Source_s SourceData;
    unsigned char rEOLData_ = ReadEOLData ( ADJ_E2P_DIAG_EOL_DE00_AUDIO_EXT_CD_ID , ADJ_E2P_DIAG_EOL_DE00_AUDIO_EXT_CD_SIZE );

   for (std::list<Source>::const_iterator itr = mSourceList.begin(); itr != mSourceList.end(); itr++)
   {
     
      
      if (  itr->mName != "eSRC_CDA"   )
      {

       SourceData.setSourceID(itr->mSourceIdEnum);
      SourceData.setDomainID(mDomainId);
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSources- source name=%s", (itr->mName).c_str());
      SourceData.setName(itr->mName);
      SourceData.setSourceState(itr->mSourceState);
      SourceData.setVisible(true);
      SourceData.setInterruptState(am_t::am_InterruptState_e::IS_OFF);
      /*	bool ret=updateAvailabilityIfAlreadyDetected(itr->mSourceIdEnum);
      if (ret)
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSources-sourceId:%d is already available", (int)(itr->mSourceIdEnum));
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSources-sourceId:%d is not yet available", (int)(itr->mSourceIdEnum));
      }*/
      if (mRoutingControlObserver->isAvailable())
      {
         SourceData.setAvailable(itr->mAvailability);
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSources-SourceData is set");
         mRoutingControlObserver->registerSourceAsync(SourceData, std::bind(&AudioManagerInterface::registerSourceCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
       }
      }
      else
      {
          
          if ( rEOLData_ != EXD_CD_PRESENT_DISABLED )
          {
       
       SourceData.setSourceID(itr->mSourceIdEnum);
      SourceData.setDomainID(mDomainId);        
      SourceData.setName(itr->mName);
      SourceData.setSourceState(itr->mSourceState);
      SourceData.setVisible(true);
      SourceData.setInterruptState(am_t::am_InterruptState_e::IS_OFF);
      /*	bool ret=updateAvailabilityIfAlreadyDetected(itr->mSourceIdEnum);
      if (ret)
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSources-sourceId:%d is already available", (int)(itr->mSourceIdEnum));
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSources-sourceId:%d is not yet available", (int)(itr->mSourceIdEnum));
      }*/
      if (mRoutingControlObserver->isAvailable())
      {
         SourceData.setAvailable(itr->mAvailability);
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSources-SourceData is set: External CD enabled ");
         mRoutingControlObserver->registerSourceAsync(SourceData, std::bind(&AudioManagerInterface::registerSourceCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
       }
    }
    else
    {
        
           LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSources- source name=%s External CD disabled ", (itr->mName).c_str());
    }
      }
   }

}

void AudioManagerInterface::deRegisterSources()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::deRegisterSources");
	for (std::list<Source>::iterator itr = mSourceList.begin(); itr != mSourceList.end(); itr++)
	{
		mRoutingControlObserver->deregisterSourceAsync((*itr).mSourceIdValue, std::bind(&AudioManagerInterface::deRegisterSourceCallback,this,_1,_2));
	}
}

void AudioManagerInterface::hookSourceAvailablityStatusChangeCallback(const am_t::am_sourceID_t& sourceId, bool availableStatus,const CommonAPI::CallStatus& status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::hookSourceAvailablityStatusChangeCallback - sourceId:%d,availableStatus:%d,callStatus:%d",(int)sourceId,availableStatus,(int)status);
    uint64_t deviceId =getDeviceId(sourceId);
    FacadeHelper<MediaDetectionEngine> mDetect;

    if(availableStatus)
    {
        if((mDetect->getMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_USB))==deviceId)
        {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::hookSourceAvailablityStatusChangeCallback for USB - sourceID:%d,mediumId:%d", sourceId, mDetect->getMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_USB));
            doSourceSwitch(sourceId, am_t::am_sinkID_e::eSNK_SPKS);
            mDetect->setMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_USB,0);
        }
        else if(sourceId == am_t::am_sourceID_e::eSRC_BT_A2DP)
        {
           LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::hookSourceAvailablityStatusChangeCallback - getmbTimerTimeExpired not expired :%d", getmbTimerTimeExpired());
           if (getmbTimerTimeExpired() == false)
           {
                if(mBTDisconnectReason==am_t::am_Error_e::E_OUT_OF_RANGE)
                {
                    if (mActiveDeviceBTLinkLossStubCallback != nullptr)
                    {
                        mActiveDeviceBTLinkLossStubCallback(false);
                    }
                }
              //If timer expired - Do Nothing
              //If it hasnt, check if mediumID == StoredMedioumID. If Yes call dosourceswitchand call canceltimer
              LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::hookSourceAvailablityStatusChangeCallback for BTSA - sourceID:%d,mediumId:%d", sourceId, mDetect->getMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH));
              if (mDetect->getMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH) == deviceId||mBTDisconnectReason==am_t::am_Error_e::E_ALREADY_EXISTS)
              {
                  mDetect->setActiveDevice(deviceId);
                  DeviceBase *actDevice = mDetect->getActiveDevice();
                  if (actDevice != nullptr)
                  {
                     actDevice->subscribeToPlayerEvents();
                     actDevice->playDevice(MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
                     LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::hookSourceAvailablityStatusChangeCallback::doPlay-activating deviceId=%llu", deviceId);
                     if(mBTDisconnectReason==am_t::am_Error_e::E_OUT_OF_RANGE)
                     {
                        mDetect->setMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH, 0);
                     }
                     cancelTimer();
                  }
                  else
                  {
                     // Do nothing
                  }
               }
               else
               {
                 // Do nothing
               }
           }
        }
        else
        {
        // Do Nothing
        }
    }
}

void AudioManagerInterface::registerSourceCallback(const CommonAPI::CallStatus& status, const ::v3::org::genivi::am_t::am_sourceID_t& sourceId, const ::v3::org::genivi::am_t::am_Error_e& error)
{
	static unsigned int count = 0;
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSourceCallback - status:%d sourceId:%d error:%d ", status, sourceId, error);
	if (error == am_t::am_Error_e::E_OK || error == am_t::am_Error_e::E_ALREADY_EXISTS) // E_ALREAD_EXISTS means someone has already registered it before...but anyway we shall handle it??
	{
		for (std::list<Source>::iterator itr = mSourceList.begin(); itr != mSourceList.end(); itr++)
		{
			if (sourceId == (*itr).mSourceIdEnum)
			{
				(*itr).registered = true;
				(*itr).mSourceIdValue = sourceId;
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSourceCallback - sourceId:%d registration status:%d ", sourceId,(*itr).registered);
				++count;
				bool stat=updateAvailabilitytoAudioPresIfAlreadyDetected(itr->mSourceIdEnum);
				if (stat)
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSourceCallback-sourceId:%d is already having media device", (int)(itr->mSourceIdEnum));
				}
				else
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSourceCallback-sourceId:%d is not having media device", (int)(itr->mSourceIdEnum));
				}
				bool ret = updateAvailabilitytoAMIfAlreadyDetected(itr->mSourceIdEnum);
				if (ret)
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSourceCallback-sourceId:%d is already available", (int)(itr->mSourceIdEnum));
				}
				else
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::registerSourceCallback-sourceId:%d is not yet available", (int)(itr->mSourceIdEnum));
				}
				break;
			}
		}
	}

	if (count == mSourceList.size() && !mSourceRegistrationComplete)
	{
		mSourceRegistrationComplete = true;
		mRoutingControlObserver->hookDomainRegistrationCompleteAsync(mDomainId,std::bind(&AudioManagerInterface::hookDomainRegistrationCompleteCallback,this,_1));
	}
}

void AudioManagerInterface::deRegisterSourceCallback(const CommonAPI::CallStatus& status, const ::v3::org::genivi::am_t::am_Error_e& error)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::deRegisterSourceCallback - status:%d,error:%d", (int)status,(int) error);
}

void AudioManagerInterface::hookDomainRegistrationCompleteCallback(const CommonAPI::CallStatus& status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::hookDomainRegistrationCompleteCallback - status:%d", (int)status);
}

void AudioManagerInterface::deRegisterDomainAsyncCallback(const CommonAPI::CallStatus& status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::deRegisterDomainAsyncCallback - status:%d", (int)status);
}

bool AudioManagerInterface::updateSourceAvailabilitytoAM(const uint64_t& deviceId)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateSourceAvailabilitytoAM - deviceId:%llu", deviceId);
	bool ret = false;
	if (mRoutingControlObserverAvailableStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{  
		am_t::am_sourceID_e sourceId = assignSourceIdToMediaDevice(deviceId);
		if (sourceId != am_t::am_sourceID_e::eSRC_UNKNOWN)
		{
			std::list<Source>::iterator sourceListitr;

			for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
			{
				if (sourceListitr->mSourceIdEnum == sourceId)
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateSourceAvailabilitytoAM - setting source Id:%d as available", (int)sourceListitr->mSourceIdValue);
					ret = true;
					sourceListitr->mDeviceId = deviceId;
					sourceListitr->mAvailability.setAvailability(am_t::am_Availability_e::A_AVAILABLE);
					mRoutingControlObserver->hookSourceAvailablityStatusChangeAsync(sourceListitr->mSourceIdValue, sourceListitr->mAvailability, std::bind(&AudioManagerInterface::hookSourceAvailablityStatusChangeCallback, this, sourceListitr->mSourceIdValue, true, _1));
					break;
				}
			}
		}
		else
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateSourceAvailabilitytoAM - the source Id is unknown for deviceId:%llu", deviceId);
		}
	}
	return ret;
}

bool AudioManagerInterface::updateAvailabilitytoAudioPresIfAlreadyDetected(am_t::am_sourceID_e sourceID)
{
	bool ret = false;
   FacadeHelper<MediaDetectionEngine> mDetect;

	if (mAudioSourceInfoAvailabilityStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateAvailabilitytoAudioPresIfAlreadyDetected - sourceID:%d", sourceID);
		MMPresTypes::tSlotList slotList = mDetect->getSlotList();
		MMPresTypes::tSlotList::const_iterator itr = slotList.begin();
		while (itr != slotList.end())
		{
		//	if (!checkIfDeviceAlreadyAssignedToSource(itr->getDevice().getDeviceId()))
			{
				am_t::am_sourceID_e sourceId = assignSourceIdToMediaDevice(itr->getDevice().getDeviceId(), itr->getSlotType());
				if (sourceID == sourceId)
				{
					ret = true;
					break;
				}
			}
			itr++;
		}
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateAvailabilitytoAudioPresIfAlreadyDetected - sourceID:%d,ret:%d", sourceID, ret);
		if (ret)
		{
			updateAudioSourceInfotoAudioPres(itr->getDevice().getDeviceId());
		}
	}
	return ret;
}

void AudioManagerInterface::updateAudioSourceInfotoAudioPres(const uint64_t& deviceId)
{
   FacadeHelper<MediaDetectionEngine> mDetect;
	if (mAudioSourceInfoAvailabilityStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateAudioSourceInfotoAudioPres - deviceId:%llu", deviceId);

		am_t::am_sourceID_e sourceId = assignSourceIdToMediaDevice(deviceId);
		if (sourceId != am_t::am_sourceID_e::eSRC_UNKNOWN)
		{
			std::list<Source>::iterator sourceListitr;

			for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
			{
				if (sourceListitr->mSourceIdEnum == sourceId)
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateAudioSourceInfotoAudioPres - setting source Id:%d as inserted", (int)sourceListitr->mSourceIdValue);
					sourceListitr->mDeviceId = deviceId;
					audioPresCtrlTypes::sSourceInfo sourceInfo;
					audioPresCtrlTypes::eEntertainmentSrcs src = audioPresCtrlTypes::eEntertainmentSrcs::SRC_ENT_UNKNOWN;
					audioPresCtrlTypes::eEntSourceType srcType = audioPresCtrlTypes::eEntSourceType::TYPE_MEDIA;
					std::unordered_map<am_t::am_sourceID_e, audioPresCtrlTypes::eEntertainmentSrcs>::const_iterator ctr = AMtoAudioPresSourceMap.find(sourceId);
					if (ctr != AMtoAudioPresSourceMap.end())
					{
						src = ctr->second;
					}

					uint8_t slotId = mDetect->findMatchingSlot((uint32_t)deviceId);

					if (slotId > 0)
					{
						sourceInfo.setSourceID(src);
						sourceInfo.setAvailability(audioPresCtrlTypes::eSourceAvailablity::A_INSERTED);
						sourceInfo.setId(slotId);
						std::string name = "";
						DeviceBase* pDevice = mDetect->getDevice(deviceId);
						if (pDevice != nullptr)
						{
							std::unordered_map<MMPresTypes::eDeviceType, audioPresCtrlTypes::eEntSourceType>::const_iterator itr = DeviceTypetoAudioSourceType.find(pDevice->getDevType());
							if (itr != DeviceTypetoAudioSourceType.end())
							{
								srcType = itr->second;
							}
							sourceInfo.setSourceType(srcType);
							sourceInfo.setName(pDevice->getDeviceName());
						}
                  CommonAPI::CallStatus status;
						mAudioSoureInfo->RegisterSourceInfo(sourceInfo, status);
						LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateAudioSourceInfo- invoked register source info call status is %d", (int)status);
					}
					break;
				}
			}
		}
		else
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateAudioSourceInfo - the source Id is unknown for deviceId:%llu", deviceId);
		}
	}
}

bool AudioManagerInterface::updateSourceUnavailabilitytoAM(const uint64_t& deviceId)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateSourceUnavailabilitytoAM - deviceId:%llu", deviceId);
	bool ret = false;
	if (mRoutingControlObserverAvailableStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
      FacadeHelper<MediaDetectionEngine> mDetect;
		am_t::am_sourceID_e sourceId = getSourceID(deviceId);
		if (sourceId != am_t::am_sourceID_e::eSRC_UNKNOWN)
		{
			std::list<Source>::iterator sourceListitr;

			for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
			{
				if (sourceListitr->mSourceIdEnum == sourceId)
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateSourceUnavailabilitytoAM - setting source Id:%d as not available", (int)sourceListitr->mSourceIdValue);
					ret = true;
					sourceListitr->mDeviceId = 0;
					sourceListitr->mAvailability.setAvailability(am_t::am_Availability_e::A_UNAVAILABLE);
					if (sourceListitr->mSourceIdEnum == am_t::am_sourceID_e::eSRC_BT_A2DP)
					{
						sourceListitr->mAvailability.setAvailabilityReason(mBTAvailabilityReason);
						LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateSourceUnavailabilitytoAM - mBTAvailabilityReason = %d", mBTAvailabilityReason);
                  DeviceBase* temp_actdevice = mDetect->getActiveDevice();
                  if(temp_actdevice!=nullptr)
                  {
                     LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateSourceUnavailabilitytoAM : mDetect->getActiveDeviceId()=%d,deviceId=%d",mDetect->getActiveDeviceId(),deviceId);
                     if(((temp_actdevice->getDevType())==MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH)&&
                     ((mDetect->getActiveDeviceId())==deviceId))
                     {
                        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateSourceUnavailabilitytoAM mBTDisconnectReason=%d", mBTDisconnectReason);
                        if ((mBTDisconnectReason==am_t::am_Error_e::E_OUT_OF_RANGE))
                        {
                           if (mActiveDeviceBTLinkLossStubCallback != nullptr)
                           {
                              mActiveDeviceBTLinkLossStubCallback(true);
                           }
                           mDetect->setMediumIdForResumePlay(temp_actdevice->getDevType(),deviceId);
                           startTimer();
                        }
                        else if(mBTDisconnectReason==am_t::am_Error_e::E_ALREADY_EXISTS)
                        {
                           startTimer();
                        }
                        else
                        {
                           // Do nothing
                        }
                     }
                     else
                     {
                        // Do nothing
                     }
                  }
                  else
                  {
                     LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateSourceUnavailabilitytoAM temp_actdevice is null");
                  }
               }
				   mRoutingControlObserver->hookSourceAvailablityStatusChangeAsync(sourceListitr->mSourceIdValue, sourceListitr->mAvailability, std::bind(&AudioManagerInterface::hookSourceAvailablityStatusChangeCallback, this, sourceListitr->mSourceIdValue, false, _1));
				   break;
				}
			}
		}
		else
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateSourceUnavailabilitytoAM - the source Id is unknown for deviceId:%llu", deviceId);
		}
	}
	return ret;
}

bool AudioManagerInterface::updateAvailabilitytoAMIfAlreadyDetected(am_t::am_sourceID_e sourceID)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateAvailabilitytoAMIfAlreadyDetected - sourceID:%d", sourceID);
	bool ret = false;

	if (mRoutingControlObserverAvailableStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
      FacadeHelper<MediaDetectionEngine> mDetect;
		MMPresTypes::tSlotList slotList = mDetect->getSlotList();
		MMPresTypes::tSlotList::const_iterator itr = slotList.begin();
		while (itr != slotList.end())
		{
			//	if (!checkIfDeviceAlreadyAssignedToSource(itr->getDevice().getDeviceId()))
			{
				am_t::am_sourceID_e sourceId = assignSourceIdToMediaDevice(itr->getDevice().getDeviceId(), itr->getSlotType());
				if (sourceID == sourceId)
				{
					ret = true;
					break;
				}
			}
			itr++;
		}
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateAvailabilitytoAMIfAlreadyDetected - sourceID:%d,ret:%d", sourceID, ret);
		if (ret)
		{
			updateSourceAvailabilitytoAM(itr->getDevice().getDeviceId());
		}
         
        if(ret)
        {
          if((mDetect->getMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_USB)) == (itr->getDevice().getDeviceId()))
          {
            LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateAvailabilitytoAMIfAlreadyDetected - sourceID:%d,mediumId:%d", sourceID,mDetect->getMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_USB));
            doSourceSwitch(sourceID, am_t::am_sinkID_e::eSNK_SPKS);
            mDetect->setMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_USB,0);
          }
          else if(((mDetect->getMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH)) == (itr->getDevice().getDeviceId()))||
                  mBTDisconnectReason==am_t::am_Error_e::E_ALREADY_EXISTS)
          {
             if(mBTDisconnectReason==am_t::am_Error_e::E_OUT_OF_RANGE)
             {
                 if (mActiveDeviceBTLinkLossStubCallback != nullptr)
                 {
                    mActiveDeviceBTLinkLossStubCallback(false);
                 }
             }
             uint64_t deviceID=itr->getDevice().getDeviceId();
             LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateAvailabilitytoAMIfAlreadyDetected - sourceID:%d,mediumId:%d", sourceID,mDetect->getMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH));
             mDetect->setActiveDevice(deviceID);
             DeviceBase *actDevice =  mDetect->getActiveDevice();

             if(actDevice!=nullptr)
             {
                 actDevice->subscribeToPlayerEvents();
                 actDevice->playDevice(MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
                 LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateAvailabilitytoAMIfAlreadyDetected-activating deviceID=%llu", deviceID);
                if(mBTDisconnectReason==am_t::am_Error_e::E_OUT_OF_RANGE)
                {
                    mDetect->setMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH,0);
                }
             }
             else
             {
                // Do nothing
             }
           }
        }
	}
	return ret;
}

am_t::am_sourceID_e AudioManagerInterface::assignSourceIdToMediaDevice(const uint64_t& deviceId)
{
	am_t::am_sourceID_e sourceId = am_t::am_sourceID_e::eSRC_UNKNOWN;
   FacadeHelper<MediaDetectionEngine> mDetect;
	if (deviceId > 0)
	{
		std::list<Source>::iterator sourceListitr;
		for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
		{
			if (sourceListitr->mDeviceId == deviceId)
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::assignSourceIdToMediaDevice- source already assigned, source id is:%d", sourceListitr->mSourceIdEnum);
				return (sourceListitr->mSourceIdEnum);
			}
		}

		int8_t slotNumber = mDetect->findMatchingSlot((uint32_t)deviceId);
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::assignSourceIdToMediaDevice- slotNumber:%d", slotNumber);
		if (slotNumber > 0)
		{
			MMPresTypes::tSlotList slotList = mDetect->getSlotList();
			MMPresTypes::tSlotList::const_iterator itr = slotList.begin();
			while (itr != slotList.end())
			{
				if (slotNumber == itr->getSlotId())
				{
					break;
				}
				itr++;
			}

			if (itr != slotList.end())
			{
				MMPresTypes::eSlotType slotType = itr->getSlotType();

				for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
				{
					if (sourceListitr->registered && slotType == sourceListitr->mSlotType)
					{
						if (sourceListitr->mDeviceId == 0)
						{
							sourceId = sourceListitr->mSourceIdEnum;
							sourceListitr->mDeviceId = deviceId;
							LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::assignSourceIdToMediaDevice - source Id:%d", (int)sourceId);
							break;
						}
					}
				}
			}
		}
	}
	return sourceId;
}

am_t::am_sourceID_e AudioManagerInterface::assignSourceIdToMediaDevice(const uint64_t& deviceId, const MMPresTypes::eSlotType& slotType)
{
	am_t::am_sourceID_e sourceId = am_t::am_sourceID_e::eSRC_UNKNOWN;
	if (deviceId > 0)
	{
		std::list<Source>::iterator sourceListitr;
	   for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
	   {
		   if (sourceListitr->mDeviceId == deviceId)
		   {
			   return sourceListitr->mSourceIdEnum;
		   }
		  if (sourceListitr->registered && slotType == sourceListitr->mSlotType && sourceListitr->mDeviceId == 0)
		  {
			  sourceId = sourceListitr->mSourceIdEnum;
			  sourceListitr->mDeviceId = deviceId;
			  LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::assignSourceIdToMediaDevice - source Id:%d", (int)sourceId);
			  break;
		  }
	  }
	}
	return sourceId;
}

// get the source id corresponding the the media device id.
am_t::am_sourceID_e AudioManagerInterface::getSourceID(const uint64_t& deviceId)
{
	am_t::am_sourceID_e sourceId = am_t::am_sourceID_e::eSRC_UNKNOWN;
	if (deviceId > 0)
	{
		std::list<Source>::iterator sourceListitr;
		for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
		{
			if (deviceId == sourceListitr->mDeviceId)
			{
				sourceId = sourceListitr->mSourceIdEnum;
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::getSourceID - source Id:%d", (int)sourceId);
				break;
			}
		}
	}
	return sourceId;
}

// This returns the media device Id for the source
uint64_t AudioManagerInterface::getDeviceId(const am_t::am_sourceID_t& sourceId)
{
	uint64_t deviceId = 0;
	std::list<Source>::iterator sourceListitr;
	for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
	{
		if (sourceId == sourceListitr->mSourceIdValue)
		{
			return sourceListitr->mDeviceId;
		}
	}
	return deviceId;
}

void AudioManagerInterface::doSourceSwitch(const ::v3::org::genivi::am_t::am_sourceID_t sourceID, const ::v3::org::genivi::am_t::am_sinkID_t sinkID)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::doSourceSwitch-sourceID %d - sinkID : %d",sourceID,sinkID);
    CommonAPI::CallStatus internalCallStatus;
    v3::org::genivi::am_t::am_mainConnectionID_t mainConnectionID;
    v3::org::genivi::am_t::am_Error_e amError = v3::org::genivi::am_t::am_Error_e::E_OK;
    mCommandControl->connect(sourceID, sinkID, internalCallStatus, mainConnectionID, amError);
}

bool AudioManagerInterface::checkIfDeviceAlreadyAssignedToSource(const uint64_t& deviceId)
{
	bool ret = false;
	if (deviceId > 0)
	{
		std::list<Source>::iterator sourceListitr;
		for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
		{
			if (deviceId == sourceListitr->mDeviceId)
			{
				ret = true;
				break;
			}
		}
	}
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::checkIfDeviceAlreadyAssignedToSource-ret%d",ret);
	return ret;
}

void AudioManagerInterface::updateError(am_t::am_sourceID_t sourceID, am_t::am_Error_e error)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateError-sourceID=%d,error=%d", (int)sourceID, (int)error);
	std::list<Source>::iterator sourceListitr;
	std::list<AudioManagerInterface::RequestInfo>::iterator requestIterator;
	am_t::am_t::am_Handle_s handle;
	for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateError-source id value=%d from registered source list", sourceListitr->mSourceIdValue);
		if (sourceID == sourceListitr->mSourceIdValue)
		{
			handle = sourceListitr->mRequestInfo.mSourceHandle;

			if(asyncSourceStateCallback != nullptr)
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateError-sending response for source handle type=%d,source handle value=%d", (int)(handle.getHandleType()), (int)(handle.getHandle()));
				asyncSourceStateCallback(handle, error, nullptr);
			}
			break;
		}
	}
}

void AudioManagerInterface::updateError(uint64_t deviceId, am_t::am_Error_e error)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateError-deviceId=%llu,error=%d", deviceId, (int)error);
	std::list<Source>::iterator sourceListitr;
	std::list<AudioManagerInterface::RequestInfo>::iterator requestIterator;
	am_t::am_t::am_Handle_s handle;
	for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateError-source id value=%d from registered source list", sourceListitr->mSourceIdValue);
		if (deviceId == sourceListitr->mDeviceId)
		{
			handle = sourceListitr->mRequestInfo.mSourceHandle;
			if(asyncSourceStateCallback != nullptr)
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateError-sending response for source handle type=%d,source handle value=%d", (int)(handle.getHandleType()), (int)(handle.getHandle()));
				asyncSourceStateCallback(handle, error, nullptr);
			}
			break;
		}
	}
}

void AudioManagerInterface::updateRequestInfo(am_t::am_sourceID_t sourceID, am_t::am_t::am_Handle_s handle, am_t::am_SourceState_e sourceState)
{
   std::list<Source>::iterator sourceListitr;
   for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateRequestInfo-source id value=%d from registered source list", sourceListitr->mSourceIdValue);
      if (sourceID == sourceListitr->mSourceIdValue)
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::updateRequestInfo-source handle type=%d,source handle value=%d,source state=%d", (int)(handle.getHandleType()), (int)(handle.getHandle()), (int)sourceState);
         (sourceListitr->mRequestInfo) = RequestInfo(handle, sourceState);
         break;
      }
   }
}

void AudioManagerInterface::setAsyncSetSourceStateFunction(std::function<void(const ::v3::org::genivi::am_t::am_Handle_s &_handle, const ::v3::org::genivi::am_t::am_Error_e &_amError, const std::shared_ptr<CommonAPI::ClientIdList> _receivers)> func)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::setAsyncSetSourceStateFunction");
   asyncSourceStateCallback = func;
}

void AudioManagerInterface::subscribeToAudioSourceInfoProxy(bool subscribe)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::subscribeToAudioSourceInfoProxy-subscribe=%d", subscribe);
   static uint32_t _aptoken = 0;
   static uint32_t _amtoken = 0;
   bool mMainConnectionStatusChangedSignalHandle = false;
   bool mMainConnectionSignalHandle = false;
   if (subscribe)
   {
      _amtoken = SUBSCRIBE(mRoutingControlObserver, getProxyStatusEvent(), &AudioManagerInterface::AudioSourceInfoProxyAvailibility, _1)
         mMainConnectionStatusChangedSignalHandle = SUBSCRIBE(mCommandControl, getMainConnectionStateChangedEvent(), &AudioManagerInterface::onMainConnectionStateChanged, _1, _2);
      mMainConnectionSignalHandle = SUBSCRIBE(mCommandControl, getNewMainConnectionEvent(), &AudioManagerInterface::onNewMainConnection, _1);
      _aptoken = SUBSCRIBE(mAudioSoureInfo, getProxyStatusEvent(), &AudioManagerInterface::AudioPresProxyAvailibility, _1)
   }
   else
   {
      UNSUBSCRIBE(mRoutingControlObserver, getProxyStatusEvent(), _amtoken)
      UNSUBSCRIBE(mCommandControl, getMainConnectionStateChangedEvent(), mMainConnectionStatusChangedSignalHandle)
      UNSUBSCRIBE(mCommandControl, getNewMainConnectionEvent(), mMainConnectionSignalHandle)
      UNSUBSCRIBE(mAudioSoureInfo, getProxyStatusEvent(), _aptoken)
   }
}

void AudioManagerInterface::AudioSourceInfoProxyAvailibility(const CommonAPI::AvailabilityStatus& avail)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::AudioSourceInfoProxyAvailibility - audio source info proxy is %s", mAudioSoureInfo.getStringAvailibility(avail).c_str());
   mAudioSourceInfoAvailabilityStatus = avail;
   if (mAudioSourceInfoAvailabilityStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      KPIMARKER(306, "AV_AUDIOMANAGER-ROUTPROXY");
      startAudioPresProxyTasks();
   }
}

void AudioManagerInterface::AudioPresProxyAvailibility(const CommonAPI::AvailabilityStatus& avail)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::AudioPresProxyAvailibility - audio source info proxy is %s", mAudioSoureInfo.getStringAvailibility(avail).c_str());
   mAudioPresInfoAvailabilityStatus = avail;
   if (mAudioPresInfoAvailabilityStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
   {
      KPIMARKER(306, "AV_AUDIOPRESPROXY");
      startAudioPresProxyTasks();
   }
}

/* Signal Callback funtions of Audio Manager */

/* This function will handle mainConnectionStateChanged signal */
void AudioManagerInterface::onMainConnectionStateChanged(const am_t::am_mainConnectionID_t & connectionID, const am_t::am_ConnectionState_e & connectionState)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "onMainConnectionStateChanged: connectionId : %d and ConnectionState : %d", ( static_cast <int> ( connectionID ) ) , ( static_cast <int> ( connectionState ) ) );
    SOURCELIST::iterator pos;
    FacadeHelper<MediaDetectionEngine> mDetect;
    u_int16_t sourceID ,connectionId;
    if(connectionState == am_t::am_ConnectionState_e::CS_CONNECTED) //setting the connected sourceid to activeSourceId
    {
    	for(pos = mActiveSourceInfo.begin(); pos!=mActiveSourceInfo.end(); ++pos)
    	{
    		if(pos->second == connectionID)
    		{
    			activeSourceId=pos->first;
    			break;
    		}
    	}
    }
    for (pos = mActiveSourceInfo.begin(); pos != mActiveSourceInfo.end(); ++pos)
    {
       sourceID = pos->first;
       connectionId = pos->second;

       LOG_INFO(LOG_CONTEXT_MEDIA, "AudioManagerInterface::onMainConnectionStateChanged: sourceID : %d, connectionId : %d", ( static_cast <int> ( sourceID ) ) , ( static_cast <int> ( connectionId ) ) );

       //SIRI automatic source switch handling
       if ((am_t::am_sourceID_e::eSRC_TEL_HANDSFREE == sourceID) && (connectionId == connectionID)) //Check if source is SIRI or Phone call
       {
          if (connectionState == am_t::am_ConnectionState_e::CS_CONNECTED)
          {
             onGoingCall = true;
          }
          else if (connectionState == am_t::am_ConnectionState_e::CS_DISCONNECTED) // Check if the SIRI source has been released
          {
             onGoingCall = false;
             if ((true == mbdoSourceSwitch)&&(false==mDetect->getVRmodeActive()))   // Check if we have received the playStatus for BTSA playback and added Vrmode check also for elvis ID 2399285
             {
                LOG_INFO(LOG_CONTEXT_MEDIA, "AudioManagerInterface::onMainConnectionStateChanged calling do source Switch up on Receing the devicePlayStatus normal use case");
                doSourceSwitch(am_t::am_sourceID_e::eSRC_BT_A2DP, am_t::am_sinkID_e::eSNK_SPKS);
                /* After Source Swicth Cancel timer ans Reset timer flag and setPerformBTSASourceSwitch falg*/
                mDetect->cancelTimer();
             }
             else if (false==mDetect->getbTimerTimeExpired())
             {
                 LOG_INFO(LOG_CONTEXT_MEDIA, "AudioManagerInterface::onMainConnectionStateChanged setPerformBTSASourceSwitch on Bug use case");
                 setPerformBTSASourceSwitch(true);
             }
             else
             {
            	 /* Below code is to support requirement of video "VP-FUR-REQ-234527/A-Phone Call handling"
            	  * If video playback has been paused when a phone call is sourced, then video playback shall resume after the phone call has been completed.
            	  *
            	  * Logic:-
            	  * Once phone call is going on, HMI calls explicit pause to mediapres to pause video playback.
            	  * So "retainPauseStateForDevice" becomes True.
            	  * Once phone call is ended, reset "retainPauseStateForDevice" flag to "False".
            	  * So once doPlay() happens from Audio Manager once phone call is ended, video playback will be automatically resumed.
            	  */
            	 FacadeHelper<MediaDetectionEngine> mDetect;
            	 FacadeHelper<SourceSwitch> mSourceSwitch;

            	  DeviceBase *actDevice = mDetect->getActiveDevice();
            	  if(actDevice != nullptr)
            	  {
                      LOG_INFO(LOG_CONTEXT_MEDIA, "AudioManagerInterface::onMainConnectionStateChanged: mVideoPlayStatus: %d", actDevice->getVideoPlayStatus());

                       if(actDevice->getVideoPlayStatus())
                       {
                           LOG_INFO(LOG_CONTEXT_MEDIA, "AudioManagerInterface::onMainConnectionStateChanged: setting pauseState flag to false for Video player as Phone call ends");
                           mSourceSwitch->SetretainPauseStateForDevice(false);
                       }
            	  }

             } //else

          } //else if (connectionState == am_t::am_ConnectionState_e::CS_DISCONNECTED)
          else
          {
             //Do Nothing
          }
       }
    }
}

bool AudioManagerInterface::isCallOnGoing()
{
    return onGoingCall;
}

/* This function will handle newMainConnection signal */
void AudioManagerInterface::onNewMainConnection( const am_t::am_MainConnectionType_s & mainConnection )
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "onNewMainConnection: source id %d and connectionId %d", mainConnection.getSourceID(), mainConnection.getMainConnectionID());
    mActiveSourceInfo[mainConnection.getSourceID()] = mainConnection.getMainConnectionID();
}

u_int16_t AudioManagerInterface::getActiveSourceId(){
	LOG_INFO(LOG_CONTEXT_MEDIA, "AudioManagerInterface::getActiveSourceId::SourceId=%d", activeSourceId);
	return activeSourceId;
}

void AudioManagerInterface::onMainSoundPropertyChanged( const am_t::am_sourceID_t& sourceID, const am_t::am_MainSoundProperty_s& sound_property )
{
#if 0
   //(am_t::am_CustomSoundPropertyType_e::SP_GENIVI_SOURCE_VOLUME_STEP == sound_property.getType())
  // LOG_INFO(LOG_CONTEXT_MEDIA, "onMainSoundPropertyChanged1: source id: %d and type: %ld and value:%ld", sourceID, sound_property.getType(),sound_property.getValue());
   uint64_t deviceId = getDeviceId(sourceID);
   FacadeHelper<MediaDetectionEngine> mDetect;
   DeviceBase *pDevice = nullptr, *actDevice = nullptr;
   pDevice = mDetect->getDevice(deviceId);
   actDevice = mDetect->getActiveDevice();
   if((pDevice != nullptr)&&(am_t::am_sourceID_e::eSRC_BT_A2DP == sourceID))
   {
      if(actDevice != nullptr)
      {
         if(am_t::am_CustomSoundPropertyType_e::SP_GENIVI_SOURCE_VOLUME_STEP == sound_property.getType())
         {
            if (sound_property.getValue() < MAXAUDVOLUME)
            {
            //AudioManager volume level is 0 to 30 max but Media volume level is 0 to 1.0 max
            //pDevice->setVolumeAttribute(sound_property.getValue() * ABSOLUTEVOLUME); //Removed Absolute Volume for Elvis Ticket no:2322696 
            }
            else
            {
            //pDevice->setVolumeAttribute(MAXMEDIAVOLUME); //Removed Absolute Volume for Elvis Ticket no:2322696 
            }
         }
      }
   }
#endif
}

void AudioManagerInterface::startAudioPresProxyTasks()
{
	if (mSourceRegistrationComplete)
	{
		std::list<Source>::iterator sourceListitr;
		for (sourceListitr = mSourceList.begin(); sourceListitr != mSourceList.end(); sourceListitr++)
		{
			updateAvailabilitytoAudioPresIfAlreadyDetected(sourceListitr->mSourceIdEnum);
		}
	}
}

void AudioManagerInterface::subscribeToCommandControlProxy(bool subscribe)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::subscribeToCommandControlProxy-subscribe=%d", subscribe);

	static uint32_t token = 0;
	if(subscribe)
	{
		token = SUBSCRIBE(mRoutingControlObserver, getProxyStatusEvent(), &AudioManagerInterface::CommandControlProxyAvailibility, _1)
	}
	else
	{
		UNSUBSCRIBE(mRoutingControlObserver, getProxyStatusEvent(), token)
      UNSUBSCRIBE(mCommandControl, getMainSourceSoundPropertyChangedEvent(), mMainSourceSoundPropertyChangedSignalHandle)
	}

}

void AudioManagerInterface::CommandControlProxyAvailibility(const CommonAPI::AvailabilityStatus& avail)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::CommandControlProxyAvailibility - commandControl proxy is %s", mCommandControl.getStringAvailibility(avail).c_str());

	mCommandControlAvailabilityStatus = avail;
	if (mCommandControlAvailabilityStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		startCommandControlProxyTasks();
	}
}

void AudioManagerInterface::startCommandControlProxyTasks()
{
    //mMainSourceSoundPropertyChangedSignalHandle = SUBSCRIBE(mCommandControl, getMainSourceSoundPropertyChangedEvent(), &AudioManagerInterface::onMainSoundPropertyChanged, _1, _2);
}

void AudioManagerInterface::CommandControlConnect()
{
   //LOG_INFO(LOG_CONTEXT_MEDIA, "AudioManagerInterface::CommandControlConnect");

   /* call connect method here with Source id of BTSA and sink id as am_t::am_sinkID_e::eSNK_SPKS */
}

void AudioManagerInterface::setBTDisconnectReason(const am_t::am_Error_e reason)
{
	mBTDisconnectReason = reason;
}

void AudioManagerInterface::setBTAvailabilityReason(const am_t::am_AvailabilityReason_e reason)
{
   mBTAvailabilityReason = reason;
}

void AudioManagerInterface::startTimer()
{
    /* start timer of 30 seconds */
    mtimerId = mTimer.setTimer(CTimerInterface::TIMER_ONESHOT,30000);
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::startTimer-timerId=%d", mtimerId);
    setmbTimerTimeExpired(false);
}
void AudioManagerInterface::cancelTimer()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> AudioManagerInterface::cancelTimer");
    setmbTimerTimeExpired(true);
    mTimer.cancelTimer(mtimerId);
    mtimerId = 0;
}

void AudioManagerInterface::timerEvent(int timerId)
{
   FacadeHelper<MediaDetectionEngine> mDetect;
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< AudioManagerInterface::timerEvent - timerId = %d", timerId);
    setmbTimerTimeExpired(true);
    if (mActiveDeviceBTLinkLossStubCallback != nullptr)
    {
       mActiveDeviceBTLinkLossStubCallback(false);
    }
    mDetect->setMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH,0);
}

void AudioManagerInterface::setActiveDeviceBTLinkLossStubCallback(std::function<void(const bool&)> func)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< AudioManagerInterface::setActiveDeviceBTLinkLossCallBack");
   mActiveDeviceBTLinkLossStubCallback = func;
}

void AudioManagerInterface::setmbTimerTimeExpired(bool isExpired)
{
  mbTimerTimeExpired=isExpired;
}

const bool AudioManagerInterface::getmbTimerTimeExpired()
{
    return (mbTimerTimeExpired);
}

void AudioManagerInterface::setPerformBTSASourceSwitch(bool _value)
{
   mbdoSourceSwitch = _value;
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< AudioManagerInterface::setPerformBTSASourceSwitch mbdoSourceSwitch_value= %d",mbdoSourceSwitch);
}

const bool AudioManagerInterface::getPerformBTSASourceSwitch()
{
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaDetectionEngine::getPerformBTSASourceSwitch mbdoSourceSwitch_value= %d",mbdoSourceSwitch);
   return mbdoSourceSwitch;
}


unsigned char  AudioManagerInterface::ReadEOLData ( unsigned long sysSetID , unsigned long sysIDSize )
{
    LOG_INFO ( LOG_CONTEXT_RCD_PLUGIN, "%s:%d ", __FUNCTION__, __LINE__ ) ;
    
           char id[4];
           char buf_byte[1]="";

           unsigned char Result_ = 0; 
           id[0] = (sysSetID >> 24) & 0xFF;
           id[1] = (sysSetID >> 16) & 0xFF;
           id[2] = (sysSetID >> 8) & 0xFF;
           id[3] = sysSetID & 0xFF;

           pclKeyReadData(PCL_LDBID_EOL, (const char *)id, PCL_STD_USER_NO, PCL_STD_SEAT_NO, (unsigned char*)buf_byte, sysIDSize);
    
           Result_  = buf_byte [ 0 ];
    
          LOG_INFO ( LOG_CONTEXT_RCD_PLUGIN, "%s:%d < result : %d>", __FUNCTION__, __LINE__ , Result_ ) ;
    
  
    
    
    return Result_ ;
     
     
}
