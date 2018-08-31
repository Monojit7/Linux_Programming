/*#include<statemachine/sourceswitch.h>

SourceSwitch::SourceSwitch()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::SourceSwitch");
	operation = new SourceOperation();
	setSCI_Media_OCB(operation);
	init();
	enter();
	runCycle();
}

SourceSwitch::~SourceSwitch()
{
	delete operation;
}

SourceOperation::SourceOperation()
:mTimer(*this)
,timerId(0)
{
 LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::SourceOperation");
}

SourceOperation::~SourceOperation()
{

}

sc_boolean SourceOperation::doPlay(sc_integer sourceId)
{
	print("ON_REQUESTED");
	bool ret = false;
	DeviceBase *device = nullptr,*actDevice = nullptr;;
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doPlay-sourceId=%d", (int)sourceId);
	uint64_t deviceId = mAudioManager->getDeviceId(sourceId);
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doPlay-deviceId=%llu", deviceId);
	if (deviceId > 0)
	{
		device = mDetect->getDevice(deviceId);
		actDevice = mDetect->getActiveDevice();
		if (device != nullptr)
		{
			//device->subscribeToPlayerEvents();
			if (actDevice == nullptr)
			{
				device->subscribeToPlayerEvents();
				device->playDevice();
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doPlay-activating deviceId=%llu", deviceId);
				mDetect->setActiveDevice(deviceId);
			//	startTimer();
			}
			else if (device==actDevice)
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doPlay-deviceId=%llu is already active, so just do a resume", deviceId);
				device->resume();
			//	startTimer();
			}
			else
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doPlay-deviceId=%llu is different from active", deviceId);
				actDevice->unSubscribeToPlayerEvents();
				mDetect->setActiveDevice(0);
				device->subscribeToPlayerEvents();
				device->playDevice();
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doPlay-activating deviceId=%llu", deviceId);
				mDetect->setActiveDevice(deviceId);
			}
			startTimer();
			ret = true;
		}
	}
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doPlay-ret=%d",ret);
	if (!ret)
	{
		mAudioManager->updateError((am_t::am_sourceID_t)sourceId, am_t::am_Error_e::E_ABORTED);
	}
	return ret;
}

sc_boolean SourceOperation::doPause(sc_integer sourceId)
{
	print("PAUSE_REQUESTED");
	bool ret = false;
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doPause-sourceId=%d", (int)sourceId);
	uint64_t deviceId = mAudioManager->getDeviceId(sourceId);
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doPause-deviceId=%llu", deviceId);
	DeviceBase *device = nullptr, *actDevice = nullptr;
	if (deviceId > 0)
	{
		actDevice = mDetect->getActiveDevice();
		device = mDetect->getDevice(deviceId);
		if (device == nullptr || actDevice == nullptr || device != actDevice)
		{
		}
		else
		{
			ret = true;
			device->pause();
		}
	}
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doPause-ret=%d", ret);
	if (!ret)
	{
		mAudioManager->updateError((am_t::am_sourceID_t)sourceId,am_t::am_Error_e::E_ABORTED);
	}
	return ret;
}

sc_boolean SourceOperation::doOff(sc_integer sourceId)
{
	print("OFF_REQUESTED");
	bool ret = false;
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doOff-sourceId=%d", (int)sourceId);
	uint64_t deviceId = mAudioManager->getDeviceId(sourceId);
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doOff-deviceId=%llu", deviceId);
	ret = deactivatePlayback(deviceId);
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::doOff-ret=%d", ret);
	if (!ret)
	{
	 mAudioManager->updateError((am_t::am_sourceID_t)sourceId, am_t::am_Error_e::E_ABORTED);
	}
	else
	{
		FacadeHelper<MediaPlaybackEngine> mPlayer;
		if (mPlayer->getPlayState() == MMPresTypes::ePlayState::PLAYSTATE_STOPPED)
		{
			mAudioManager->updateError((am_t::am_sourceID_t)sourceId, am_t::am_Error_e::E_OK);
		}
	}
	return ret;
}

bool SourceOperation::deactivatePlayback(uint64_t deviceId)
{
	bool ret = false;
	DeviceBase *device = nullptr, *actDevice = nullptr;
	actDevice = mDetect->getActiveDevice();
	device = mDetect->getDevice(deviceId);
	if (device == nullptr && actDevice==nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::deactivatePlayback-there is no active device and also no device with id=%llu",deviceId);
		ret = true;
	}
	else if (device != nullptr && actDevice != nullptr && device == actDevice)
	{
		ret = true;
		device->stop();
		device->unSubscribeToPlayerEvents();
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::deactivatePlayback-deactivating active device");
		mDetect->setActiveDevice(0);
	}
	else
	{

	}
	return ret;
}

void SourceOperation::startTimer()
{
	timerId=mTimer.setTimer(CTimerInterface::TIMER_ONESHOT,10000);
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::startTimer-timerId=%d", timerId);
}

void SourceOperation::timerEvent(int timerId)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::timerEvent-timerId=%d expired..so AM goees out of source", timerId);
	FacadeHelper<SourceSwitch> mSwitch;
	mSwitch->getSCI_Media()->raise_reqTimeout();
	FacadeHelper<MediaDetectionEngine> mDetect;
	deactivatePlayback(mDetect->getActiveDeviceId());
}

void SourceOperation::cancelTimer()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::cancelTimer");
	mTimer.cancelTimer(timerId);
	timerId = 0;
}

void SourceOperation::resPlay(sc_integer sourceId,sc_boolean status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::resPlay-sourceId=%d,status=%d",sourceId,status);
	am_t::am_Error_e error;
	if (status == true)
	{
	 error = am_t::am_Error_e::E_OK;
	}
	else
	{
	  error = am_t::am_Error_e::E_ABORTED;
	}
	
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::resPlay- updating error=%d to AM", (int)error);
	mAudioManager->updateError((am_t::am_sourceID_t)sourceId,error);
}

void SourceOperation::resPause(sc_integer sourceId,sc_boolean status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::resPause-sourceId=%d,status=%d",sourceId,status);
	am_t::am_Error_e error;
	if (status == true)
	{
		error = am_t::am_Error_e::E_OK;
	}
	else
	{
		error = am_t::am_Error_e::E_ABORTED;
	}
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::resPause- updating error=%d to AM", (int)error);
	mAudioManager->updateError((am_t::am_sourceID_t)sourceId,error);
}

void SourceOperation::resOff(sc_integer sourceId,sc_boolean status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::resOff-sourceId=%d,status=%d",sourceId,status);
	am_t::am_Error_e error;
	if (status == true)
	{
		error = am_t::am_Error_e::E_OK;
	}
	else
	{
		error = am_t::am_Error_e::E_ABORTED;
	}
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::resOff- updating error=%d to AM", (int)error);
	mAudioManager->updateError((am_t::am_sourceID_t)sourceId,error);
}

void SourceOperation::print(sc_string name)
{
	const char* state = (const char*)name;
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceOperation::print-current state is %s", state);
}

void SourceSwitch::startTimer()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::startTimer");
}

void SourceSwitch::cancelTimer()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::cancelTimer");
	if (operation != nullptr)
	{
		operation->cancelTimer();
	}
}

*/

#include<statemachine/sourceswitch.h>
#include<device/DeviceBase.h>

SourceSwitch::SourceSwitch()
:mTimer(*this)
,timerId(0)
,mRetainPauseStateFlag(false)
,mSourceSwitchTimerExpired(false)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::SourceSwitch");
}

SourceSwitch::~SourceSwitch()
{
}

// initiate a play for the media source.This method is triggered from the state machine.
bool SourceSwitch::doPlay(int sourceId)
{
	cancelTimer();
	bool ret = false;
	DeviceBase *device = nullptr, *actDevice = nullptr;
	am_t::am_Error_e error = am_t::am_Error_e::E_ABORTED;
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doPlay-sourceId=%d", (int)sourceId);
	uint64_t deviceId = mAudioManager->getDeviceId(sourceId);
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doPlay-deviceId=%llu", deviceId);

	if (deviceId > 0)
	{
		device = mDetect->getDevice(deviceId);
		actDevice = mDetect->getActiveDevice();
		if (device != nullptr)
		{
			if (actDevice == nullptr)
			{
				//device->subscribeToPlayerEvents();
            if(retainPauseStateForDevice()==true)
            {
			//device->subscribeToPlayerEvents();
               resPlay(true);
               SetretainPauseStateForDevice(false);
            }
            else
            {
               startTimer();
               device->playDevice(MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
               LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doPlay-activating deviceId=%llu", deviceId);
               mDetect->setActiveDevice(deviceId);
            }
			}
			else if (device == actDevice)
			{
            if(retainPauseStateForDevice()==true)
            {
               resPlay(true);
            }
            else
            {
               LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doPlay-deviceId=%llu is already active, so just do a resume", deviceId);

               DeviceBase* tmpPtr = mDetect->getActiveDevice();
               if( tmpPtr != nullptr) // Null pointer checkadded for CodeSonar warning
               {
                  if(tmpPtr->getPlayState()== MMPresTypes::ePlayState::PLAYSTATE_PLAYING)
                  {
                     resPlay(true);
                  }
                  else
                  {
                     startTimer();
                  }
               }
               else
               {
                  LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doPlay Invalid or Null pointer");
               }
               device->resume();
            }
         }
         else
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doPlay-deviceId=%llu is different from active", deviceId);
				//actDevice->unSubscribeToPlayerEvents();
				mDetect->setActiveDevice(0);
				//device->subscribeToPlayerEvents();
            device->playDevice(MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN);
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doPlay-activating deviceId=%llu", deviceId);
				mDetect->setActiveDevice(deviceId);
            startTimer();
			}
		    error = am_t::am_Error_e::E_OK;
			ret = true;
		}
	}
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doPlay-ret=%d", ret);
//	mAudioManager->updateError((am_t::am_sourceID_t)sourceId,error);
	return ret;
}

// initiate a pause for the media source.This method is triggered from the state machine.
bool SourceSwitch::doPause(int sourceId)
{
	cancelTimer();
	bool ret = false;
	am_t::am_Error_e error = am_t::am_Error_e::E_ABORTED;
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doPause-sourceId=%d", (int)sourceId);
    

    
	uint64_t deviceId = mAudioManager->getDeviceId(sourceId);
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doPause-deviceId=%llu", deviceId);
    
    
    
	DeviceBase *device = nullptr, *actDevice = nullptr;
	if (deviceId > 0)
	{
		actDevice = mDetect->getActiveDevice();
		device = mDetect->getDevice(deviceId);
		if (device == nullptr || actDevice == nullptr || device != actDevice)
		{
		}
		else
		{
			ret = true;
			device->pause();
			error = am_t::am_Error_e::E_OK;
		}
	}
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doPause-ret=%d", ret);
    mAudioManager->updateError((am_t::am_sourceID_t)sourceId,error);
	return ret;
}

// initiate a off for the media source.This method is triggered from the state machine.
bool SourceSwitch::doOff(int sourceId)
{
	cancelTimer();
	bool ret = false;
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doOff-sourceId=%d", (int)sourceId);
    
    if ( sourceId == am_t::am_sourceID_e::eSRC_CDA )
    {
        
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch :: CD source =%d", (int)sourceId);
        PRESEVENT_PUBLISH ( eventrequestQueueCD, REQ_CLEAR_QUEUE_CD,  clear_queue_status_ ) ;
         
            FacadeHelper<MediaPlaybackEngine> lPlayEngine;
	        std::function<void(const MMPresTypes::ePlayState&)> callback = lPlayEngine->getPlayStateStubCallback();
	        if (callback != nullptr)
	         {
	                 LOG_INFO(LOG_CONTEXT_MEDIA, " %s, %d sending playstate as Stop", __FUNCTION__, __LINE__);
	                 callback(MMPresTypes::ePlayState::PLAYSTATE_STOPPED);
	         }
    }
    
	am_t::am_Error_e error = am_t::am_Error_e::E_ABORTED;
	uint64_t deviceId = mAudioManager->getDeviceId(sourceId);
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doOff-deviceId=%llu", deviceId);
   SetretainPauseStateForDevice(false);
	ret = deactivatePlayback(deviceId);
	/*Sometimes the STOP playstate is not received for CD in time or not at all received. This causes issues in the
	 * publishing of future playstates. Below if condition is the workaround to handle this bug
	 */

	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::doOff-ret=%d", ret);
	if (!ret)
	{
		mAudioManager->updateError((am_t::am_sourceID_t)sourceId,error);
	}
	else
	{
		error = am_t::am_Error_e::E_OK;
		mAudioManager->updateError((am_t::am_sourceID_t)sourceId,error);
	}
	return ret;
}

bool SourceSwitch::deactivatePlayback(uint64_t deviceId)
{
	bool ret = false;
	DeviceBase *device = nullptr, *actDevice = nullptr;
	actDevice = mDetect->getActiveDevice();
	device = mDetect->getDevice(deviceId);
	if (device == nullptr && actDevice == nullptr)
	{
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::deactivatePlayback-there is user source switch, getmbTimerTimeExpired=%d",mAudioManager->getmbTimerTimeExpired());
      if(mAudioManager->getmbTimerTimeExpired()==false)
      {
         mAudioManager->cancelTimer();
      }
      LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::deactivatePlayback-there is no active device and also no device with id=%llu", deviceId);
      ret = true;

	}
	else if (device != nullptr && actDevice != nullptr && device == actDevice)
	{
		ret = true;
		if (mSettings->isDequeAllReqdForSourceSwitch())
		{
			device->dequeAll();
		}
		else
		{
			device->stop();
		}
		//device->unSubscribeToPlayerEvents();
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::deactivatePlayback-deactivating active device");
		mDetect->setActiveDevice(0);
	}
	else
	{

	}
	return ret;
}

void SourceSwitch::resPlay(bool val)
{
	am_t::am_Error_e error = am_t::am_Error_e::E_OK;

	if (!val)
	{
		error = am_t::am_Error_e::E_ABORTED;
	}

	uint64_t id = mDetect->getActiveDeviceId();
	int sourceId = mAudioManager->getSourceID(id);

	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::resPlay-sourceId=%d,device id=%llu,error=%d", sourceId, id,(int)error);
	mAudioManager->updateError((am_t::am_sourceID_t)sourceId,error);
	timerId = 0;

	/* below is workaround from mpres till Wicom handles actual issue
	 * When source switch to BTSA happens, device does not respond and hence timerexpires.
	 * In this case, mpres used to send error to AudioManager and AudioManager changed source from BTSA to AM/FM
	 * As a workaround, when timer expires and source is BTSA, do not send error to AudioManager so source will be BTSA only
	 * And mpres has to send dummy nowPlaying event to HMI so that HMI will show dummy nowPlaying event for BTSA.
	 * This is as per requirement from Ford
	 */
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::resPlay-mSourceSwitchTimerExpired=%d", (int)mSourceSwitchTimerExpired);

	if(mSourceSwitchTimerExpired && (sourceId == am_t::am_sourceID_e::eSRC_BT_A2DP))
	{
		DeviceBase *actDevice = mDetect->getActiveDevice();
		if(actDevice != nullptr)
		{
		  LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::resPlay - clearing out Metadata for BTSA when timer expired");
        const MMPresTypes::tNowPlayingInfo l_BTCurrentNowPlayingInfo = actDevice->getNowPlayingInfo();
        if (0u == l_BTCurrentNowPlayingInfo.getMediumId())
        {
           actDevice->clearMetadata();
        }
		}
		else
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::resPlay - ERROR: Timer expired for BTSA and no activeDevice");
		}
	}
}

void SourceSwitch::startTimer()
{
	//timerId = mTimer.setTimer(CTimerInterface::TIMER_ONESHOT,10000);

	/* below is workaround for BTSA issue where device does not start playback and actually timer should expired after 10 seconds
	 * But AudioManager timer of 10 seconds will be expired early as compared to mpres timer and AudioManager will call doOff for BTSA.
	 * This is not expected as once mpres timer expired, mpres has to inform AM that BTSA is still active source and there is no issue.
	 * This is as per requirement from Ford.
	 * So setting mpres timer is of 9seconds instead of 10 seconds so before AM calls doOff after their 10 seconds, workaround should come into picture
	 * Below workaround will be removed once Wicom/CoC media fix actual issue
	 */
	timerId = mTimer.setTimer(CTimerInterface::TIMER_ONESHOT,9000);

	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::startTimer-timerId=%d", timerId);
	mSourceSwitchTimerExpired = false;
}
void SourceSwitch::cancelTimer()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::cancelTimer");
	mTimer.cancelTimer(timerId);
	timerId = 0;

	mSourceSwitchTimerExpired = false;
}

void SourceSwitch::timerEvent(int timerId)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::timerEvent-timerId=%d expired..so AM goees out of source", timerId);
	mSourceSwitchTimerExpired = true;

	uint64_t id = mDetect->getActiveDeviceId();
	int sourceId = mAudioManager->getSourceID(id);

	/* below is workaround from mpres till Wicom handles actual issue
	 * When source switch to BTSA happens, device does not respond and hence timerexpires.
	 * In this case, mpres used to send error to AudioManager and AudioManager changed source from BTSA to AM/FM
	 * As a workaround, when timer expires and source is BTSA, do not send error to AudioManager so source will be BTSA only
	 * And mpres has to send dummy nowPlaying event to HMI so that HMI will show dummy nowPlaying event for BTSA.
	 * This is as per requirement from Ford.
	 *
	 * For empty USB device, playback is not started and empty device error is sent to HMI.
	 * As per requirement MP-UC-REQ-019925, empty USB should remain as active source and should not change to AM/FM.
	 * Once timer expires, report success to Audio Manager so Audio Manager will not change source to AM/FM
	 */
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::timerEvent-sourceId=%d", sourceId);

	if(sourceId == am_t::am_sourceID_e::eSRC_BT_A2DP)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::timerEvent - timerExpired but BTSA source should remain active");
		resPlay(true);
	}
	else if((sourceId == am_t::am_sourceID_e::eSRC_USB1) || (sourceId == am_t::am_sourceID_e::eSRC_USB2))
	{
		DeviceBase *actDevice = nullptr;
		actDevice = mDetect->getActiveDevice();
		if(actDevice != nullptr)
		{
			if(actDevice->getIsDeviceEmpty())
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::timerEvent - timerExpired but empty USB source should remain active");
				resPlay(true);
			}
			else
			{
				deactivatePlayback(id);
            resPlay(false);
			}
		}
	}
	else
	{
		deactivatePlayback(id);
      resPlay(false);
	}
}

void SourceSwitch::SetretainPauseStateForDevice(const bool pauseFlag)
{
    mRetainPauseStateFlag=pauseFlag;
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::SetretainPauseStateForDevice is true/false i.e 1/0 = %d", mRetainPauseStateFlag);
}

bool SourceSwitch::retainPauseStateForDevice()
{
    return (mRetainPauseStateFlag);
}

bool SourceSwitch::getSourceSwitchTimerExpired()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::getSourceSwitchTimerExpired - value = %d", mSourceSwitchTimerExpired);
    return mSourceSwitchTimerExpired;
}

void SourceSwitch::setSourceSwitchTimerExpired(const bool value)
{
   mSourceSwitchTimerExpired = value;
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SourceSwitch::setSourceSwitchTimerExpired - mSourceSwitchTimerExpired = %d", mSourceSwitchTimerExpired);
}


