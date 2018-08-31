#include <spot/harman_kpimarker.h>
#include<detectionengine/MediaDetectionEngine.h>
#include<device/DeviceUmass.h>
#include<device/DeviceDummy.h>
#include<device/DeviceIAP1.h>
#include<device/DeviceIAP2.h>
#include<device/DeviceBluetooth.h>
#include<device/DeviceMtp.h>
#include<device/DeviceIpod.h>
#include<device/DeviceRemoteCD.h>
#include<MediaPresCore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

//Apple authetication chip Details
#define ACP_ADDR_DEVICE_VERSION       0
#define ACP_ADDR_FIRMWARE_VERSION     1
#define ACP_ADDR_PROTOCOL_MAJOR       2
#define ACP_ADDR_PROTOCOL_MINOR       3
#define ACP_I2C_DEV_PATH              "/dev/i2c-1"
#define ACP_I2C_SLAVE_WRITE_ADDRESS   0x10
#define ACP_I2C_SLAVE_READ_ADDRESS    0x10

std::unordered_map<MMPresTypes::eDeviceType, MMPresTypes::eSlotType> MediaDetectionEngine::mDeviceToSlotMap =
		{ { MMPresTypes::eDeviceType::DEVICETYPE_USB,
				MMPresTypes::eSlotType::SLOTTYPE_USB }, {
				MMPresTypes::eDeviceType::DEVICETYPE_IPOD,
				MMPresTypes::eSlotType::SLOTTYPE_IPOD }, {
				MMPresTypes::eDeviceType::DEVICETYPE_MTP,
				MMPresTypes::eSlotType::SLOTTYPE_USB }, {
				MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH,
				MMPresTypes::eSlotType::SLOTTYPE_BLUETOOTH }, {
				MMPresTypes::eDeviceType::DEVICETYPE_REMOTE_CD,
				MMPresTypes::eSlotType::SLOTTYPE_REMOTE_CD } };

std::unordered_map<BaseType::tBaseError, MMPresTypes::eMediaError> MediaDetectionEngine::mBaseToMediaError =
		{ { BaseType::tBaseError::S_OK,
				MMPresTypes::eMediaError::MEDIAERROR_NO_ERROR }, {
				BaseType::tBaseError::E_UNKNOWN,
				MMPresTypes::eMediaError::MEDIAERROR_UNKNOWN }, {
				BaseType::tBaseError::E_UNSUPPORTED,
				MMPresTypes::eMediaError::MEDIAERROR_UNSUPPORTED }, {
				BaseType::tBaseError::E_BUSY,
				MMPresTypes::eMediaError::MEDIAERROR_BUSY }, {
				BaseType::tBaseError::E_NOMEM,
				MMPresTypes::eMediaError::MEDIAERROR_NO_MEMORY }, {
				BaseType::tBaseError::E_INVALIDPARAMATERS,
				MMPresTypes::eMediaError::MEDIAERROR_INVALIDPARAMATERS }, {
				BaseType::tBaseError::E_WRONGSTATE,
				MMPresTypes::eMediaError::MEDIAERROR_WRONGSTATE }, {
				BaseType::tBaseError::E_FSUNSUPPORTED,
				MMPresTypes::eMediaError::MEDIAERROR_FSUNSUPPORTED }, {
				BaseType::tBaseError::E_FILES_SKIPPED,
				MMPresTypes::eMediaError::MEDIAERROR_FILES_SKIPPED } };

MediaDetectionEngine::MediaDetectionEngine() :
		bTimerTimeExpired(true), bisVRmodeActive(false), isVRActive(false), mSiriEnabled(
				false), mDevManager("local", "inst0"), mTriggerSlotUpdate(
				nullptr), mActiveDeviceId(0u), mDevManagerProxyStatus(
				CommonAPI::AvailabilityStatus::UNKNOWN), mBTSVCHandler("local",
				"com.harman.connectivity.BtService"), mIsAppleAuthenticationFunctional(
				true), mTimer(*this), timerId(0) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::MediaDetectionEngine");

	mRVCStatus = false;
	mAPAStatus = false;
	mPDCStatus = false;
	mPauseForParkingAssistance = false;

	mMediumIdResumePlay.insert(
			pair<MMPresTypes::eDeviceType, uint32_t>(
					MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH, 0));
	mMediumIdResumePlay.insert(
			pair<MMPresTypes::eDeviceType, uint32_t>(
					MMPresTypes::eDeviceType::DEVICETYPE_USB, 0));
	init();
}

void MediaDetectionEngine::init() {
	initSlotsFromConfig();
	mAppleChipAuthenticationCallback = nullptr;
	SUBSCRIBE(mDevManager, getProxyStatusEvent(),
			&MediaDetectionEngine::deviceManagerProxyavailability, _1)
	SUBSCRIBE(mBTSVCHandler, getProxyStatusEvent(),
			&MediaDetectionEngine::btServiceProxyAvailability, _1)

	//subscribe to Mpres plugin specific events.
	initPluginEvents();
}

void MediaDetectionEngine::initPluginEvents() {
	LOG_INFO(LOG_CONTEXT_MEDIA, "MediaDetectionEngine::initPluginEvents");

	//subscribe to RVCDisplay Status specific Events. MPres plugin (Ford-Specific) will publish this event
	PresEventAbstraction::getInstance()->subscribe<eventRVCDisplayStatus>(
			REQ_RVCDISPLAY,
			std::bind(&MediaDetectionEngine::rvcDisplayStatusEvent, this,
					std::placeholders::_1));

	//subscribe to APA Status specific Events. MPres plugin (Ford-Specific) will publish this event
	PresEventAbstraction::getInstance()->subscribe<eventAPAStatus>(REQ_APA,
			std::bind(&MediaDetectionEngine::apaStatusEvent, this,
					std::placeholders::_1));

	//subscribe to PDC Status specific Events. MPres plugin (Ford-Specific) will publish this event
	PresEventAbstraction::getInstance()->subscribe<eventPDCStatus>(REQ_PDC,
			std::bind(&MediaDetectionEngine::pdcStatusEvent, this,
					std::placeholders::_1));

	PresEventAbstraction::getInstance()->subscribe<cdStatus>(EVENT_CD_STATUS,
			std::bind(&MediaDetectionEngine::CDStatusEvent, this,
					std::placeholders::_1));

}

void MediaDetectionEngine::rvcDisplayStatusEvent(const bool& status) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			">>> MediaDetectionEngine::rvcDisplayStatusEvent: status=%d",
			(int )status);

	mRVCStatus = status;
	supportParkingAssistance(mRVCStatus, mAPAStatus, mPDCStatus);
}

void MediaDetectionEngine::apaStatusEvent(const bool& status) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			">>> MediaDetectionEngine::apaStatusEvent: status=%d", (int )status);

	mAPAStatus = status;
	supportParkingAssistance(mRVCStatus, mAPAStatus, mPDCStatus);
}

void MediaDetectionEngine::pdcStatusEvent(const bool& status) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			">>> MediaDetectionEngine::pdcStatusEvent: status=%d", (int )status);

	mPDCStatus = status;
	supportParkingAssistance(mRVCStatus, mAPAStatus, mPDCStatus);
}

void MediaDetectionEngine::supportParkingAssistance(const bool& RVCstatus,
		const bool& APAStatus, const bool& PDCStataus) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"MediaDetectionEngine::supportParkingAssistance: RVCstatus=%d , APAStatus=%d, PDCStataus=%d, mPauseForParkingAssistance=%d",
			(int )RVCstatus, (int )APAStatus, (int )PDCStataus,
			(int )mPauseForParkingAssistance);

	DeviceBase* pDevice = getActiveDevice();
	if (pDevice != nullptr) {
		if (true == pDevice->getVideoPlayStatus()) {
			if ((RVCstatus || APAStatus || PDCStataus)
					&& (mPauseForParkingAssistance == false)) {
				//pause active device if any of PDC/APA/RVC is ON from VehicleSettings & video playback is not paused
				LOG_INFO(LOG_CONTEXT_MEDIA,
						"MediaDetectionEngine::supportParkingAssistance: pausing ActiveDevice");
				pDevice->pause();
				mPauseForParkingAssistance = true;
			} else if (!RVCstatus && !APAStatus && !PDCStataus) {
				//resume active device only when RVC, PDC & APA are OFF and video playabck state is not in pause state.

				FacadeHelper<SourceSwitch> mSourceSwitch;

				/* If PDC, RVC & APA are OFF and video playback state is Pause state then do not resume video playback.
				 * This is as per assembla 2290
				 */
				if(mSourceSwitch->retainPauseStateForDevice() == false)
				{
					LOG_INFO(LOG_CONTEXT_MEDIA,
							"MediaDetectionEngine::supportParkingAssistance: resuming ActiveDevice");
					pDevice->resume();
				}
				mPauseForParkingAssistance = false;
			} else
				LOG_INFO(LOG_CONTEXT_MEDIA,
						"MediaDetectionEngine::supportParkingAssistance: No Pause or Resume: RVCstatus=%d , APAStatus=%d, PDCStataus=%d, mPauseForParkingAssistance=%d",
						(int )RVCstatus, (int )APAStatus, (int )PDCStataus,
						(int )mPauseForParkingAssistance);

		}
	} else {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"MediaDetectionEngine::supportParkingAssistance - No active device found");
	}
}

void MediaDetectionEngine::mediumChangedCallback(
		const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo) {
	//LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaDetectionEngine::mediumChangedCallback: mediumId:%lu,mediumName:%s,mountPath:%s,deviceUri:%s,deviceBrowsePath:%s,browseable:%d",mediumInfo.getMMediumId(),mediumInfo.getMMediumName().c_str(),mediumInfo.getMMountPath().c_str(),mediumInfo.getMDeviceUri().c_str(),mediumInfo.getMDeviceBrowsePath().c_str(),mediumInfo.getMBrowseAble());
	LOG_INFO(LOG_CONTEXT_MEDIA,
			">>> MediaDetectionEngine::mediumChangedCallback :mediumId:%lu,mediumType:%d,contentType:%d,mediumState:%d,handleid:%d",
			mediumInfo.getMMediumId(), (int32_t)(mediumInfo.getMMediumType()),
			(int32_t)(mediumInfo.getMContentType()),
			(int32_t)(mediumInfo.getMMediumState()), mediumInfo.getMHandleId());

	bool isDeviceAlreadyAvailable = false;
	auto it = mDeviceList.find(mediumInfo.getMMediumId());

	bool isValidMediumType = false;
	if ((v1::com::harman::media::BaseType::tMediumType::MT_UMASS
			== mediumInfo.getMMediumType())
			|| (v1::com::harman::media::BaseType::tMediumType::MT_BLUETOOTH
					== mediumInfo.getMMediumType())
			|| (v1::com::harman::media::BaseType::tMediumType::MT_IPOD
					== mediumInfo.getMMediumType())
			|| (v1::com::harman::media::BaseType::tMediumType::MT_MTP
					== mediumInfo.getMMediumType())) {
		isValidMediumType = true;
	}
	if ((it != mDeviceList.end()) && (mediumInfo.getMMediumType())
			&& (it->second->getMediumInfo().getMHandleId()
					== mediumInfo.getMHandleId())
			&& (it->second->getMediumInfo().getMMediumType()
					== mediumInfo.getMMediumType())) {
		isDeviceAlreadyAvailable = true;
	}
	if (true == isValidMediumType) {
		/************ MS_AVAILABLE ==> MS_READY_TO_PLAY ==>MS_FIRST_PASS_FIN ==>MS_SECOND_PASS_FIN ==>MS_ACTIVE *************/
		switch (mediumInfo.getMMediumState()) {
		case v1::com::harman::media::BaseType::tMediumState::MS_AVAILABLE: {
			if (true == isDeviceAlreadyAvailable) {
				LOG_INFO(LOG_CONTEXT_MEDIA,
						"MediaDetectionEngine::mediumChangedCallback-Device with the msid %lu already exists!! Ignored",
						mediumInfo.getMMediumId());
			} else {
				KPIMARKER(306, "AV_DEVICE_INSERTED");
				PresCtrl::addRequest(e_REQUEST_DEVICEINFO_TASK,
						std::bind(&MediaDetectionEngine::createDevice, this,
								mediumInfo));
				;
			}
		}
			break;
		case v1::com::harman::media::BaseType::tMediumState::MS_FIRST_PASS_FIN:
		case v1::com::harman::media::BaseType::tMediumState::MS_SECOND_PASS_FIN:
		case v1::com::harman::media::BaseType::tMediumState::MS_ACTIVE:
		case v1::com::harman::media::BaseType::tMediumState::MS_READY_TO_PLAY: {
			if (true == isDeviceAlreadyAvailable) {
				it->second->mediumInfoUpdate(mediumInfo); ///Device already exists, so update with the medium information.
			} else {
				///For some reason we might have missed the availibility update.. 
				///Create a new device now !
				PresCtrl::addRequest(e_REQUEST_DEVICEINFO_TASK,
						std::bind(&MediaDetectionEngine::createDevice, this,
								mediumInfo));
			}
		}
			break;
		case v1::com::harman::media::BaseType::tMediumState::MS_UNAVAILABLE:
		case v1::com::harman::media::BaseType::tMediumState::MS_UNKNOWN: {
			if (true == isDeviceAlreadyAvailable) {
				if (getActiveDevice() != nullptr) {
					if (mediumInfo.getMMediumId()
							== getActiveDevice()->getMediumInfo().getMMediumId()) {
						getActiveDevice()->clearMetadata();
					}
				}
				BaseType::tMediumInfo temp_mediuminfo =
						it->second->getMediumInfo();
				LOG_INFO(LOG_CONTEXT_MEDIA,
						">>> MediaDetectionEngine::mediumChangedCallback : Stored mediumType:%d,handleid:%lu!!!",
						(int32_t)(temp_mediuminfo.getMMediumType()),
						temp_mediuminfo.getMHandleId());
				KPIMARKER(306, "AV_DEVICE_REMOVED");
				// unsubscribe the player events.Removed this from the destuctor as derived class virtual functions are used.
				it->second->unSubscribeToPlayerEvents();
				delete it->second;
				mDeviceList.erase(it);
				int8_t slotId = findMatchingSlot(mediumInfo.getMMediumId());
				if (slotId > 0) {
					cleanupSlot(slotId, temp_mediuminfo);
				}
			} else {
				LOG_INFO(LOG_CONTEXT_MEDIA,
						"Got invalid MediaState removal for non existant device, medium id : %lu",
						mediumInfo.getMMediumId());
			}
		}
			break;
		default:
			break;
		}
	} else {
		//Do Nothing
		LOG_INFO(LOG_CONTEXT_MEDIA, "Got invalid medium Type %d. Ignore!");
	}
}

void MediaDetectionEngine::remotePlayerCallback(const uint32_t& mediumId, const ::v1::com::harman::media::BaseType::tRemotePlayerCapability& capability) 
{
   LOG_INFO(LOG_CONTEXT_MEDIA,">>> MediaDetectionEngine::remotePlayerCallback-play:%d,pause:%d,next:%d,prev:%d,ffwd:%d,frwd:%d",
         capability.getSupportPlay(), capability.getSupportPause(),
         capability.getSupportNext(), capability.getSSupportPrevious(),
         capability.getSupportFastForward(),
         capability.getSupportFastRewind());
   LOG_INFO(LOG_CONTEXT_MEDIA,">>> MediaDetectionEngine::remotePlayerCallback-rptOff:%d,rptSingle:%d,rptAll:%d,shuffle:%d,unshuffle:%d,browse:%d", capability.getSupportRepeatOff(),
         capability.getSupportRepeatSingle(),
         capability.getSupportRepeatAll(), capability.getSupportShuffle(),
         capability.getSupportUnShuffle(), capability.getSupportBrowsing());
   DeviceBase* pDevice = getDevice((uint64_t) mediumId);
   if ((pDevice != nullptr)&& (MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH== pDevice->getDevType())) 
   {
      LOG_INFO(LOG_CONTEXT_MEDIA,">>> MediaDetectionEngine::remotePlayerCallback-mediumId: %d.",mediumId);
      pDevice->onRemotePlayerChanged();
   }
}

DeviceBase* MediaDetectionEngine::createDevice(
		const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"MediaDetectionEngine::createDevice-mediumId=%lu,medium Type=%d",
			mediumInfo.getMMediumId(), (int32_t )mediumInfo.getMMediumType());
	DeviceBase* pNewDevice = nullptr;

	switch (mediumInfo.getMMediumType()) {
	case v1::com::harman::media::BaseType::tMediumType::MT_UMASS: {
		pNewDevice = new DeviceUmass(mediumInfo);
		if (nullptr != pNewDevice) {
			pNewDevice->subscribeToPlayerEvents();
		}
	}
		break;
	case v1::com::harman::media::BaseType::tMediumType::MT_BLUETOOTH: {
		pNewDevice = new DeviceBluetooth(mediumInfo);
		if (nullptr != pNewDevice) {
			pNewDevice->subscribeToPlayerEvents();
		}
	}
		break;
	case v1::com::harman::media::BaseType::tMediumType::MT_IPOD: {
		pNewDevice = new DeviceIpod(mediumInfo);
		if (nullptr != pNewDevice) {
			pNewDevice->subscribeToPlayerEvents();
		}
	}
		break;
	case v1::com::harman::media::BaseType::tMediumType::MT_MTP: {
		pNewDevice = new DeviceMtp(mediumInfo);
		if (nullptr != pNewDevice) {
			pNewDevice->subscribeToPlayerEvents();
		}
	}
		break;
	default: {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"MediaDetectionEngine::createDevice-Unsupported Device Type : %d",
				mediumInfo.getMMediumType());
		pNewDevice = new DeviceDummy(mediumInfo);
	}
		break;
	}

	if (nullptr == pNewDevice) {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"MediaDetectionEngine::createDevice-Unsupported Device  Name : %s , Medium state %d , Mountpath : %s , Browse Path : %s",
				mediumInfo.getMMediumName().c_str(),
				mediumInfo.getMMediumState(),
				mediumInfo.getMMountPath().c_str(),
				mediumInfo.getMDeviceBrowsePath().c_str());
	} else {
		mDeviceList[mediumInfo.getMMediumId()] = pNewDevice;
	}
	return pNewDevice;
}

void MediaDetectionEngine::getDevicesCallback(
		const CommonAPI::CallStatus & stat,
		const ::v1::com::harman::media::BaseType::tBaseError&error,
		const ::v1::com::harman::media::BaseType::tMediumInfoList& mediumInfolist) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			">>> MediaDetectionEngine::getDevicesCallback-callStatus:%d,base error:%d",
			(int32_t )stat, (int32_t )error);
	if ((CommonAPI::CallStatus::SUCCESS == stat)
			&& (v1::com::harman::media::BaseType::tBaseError::S_OK == error)) {
		uint16_t listSize = mediumInfolist.size();
		for (uint16_t i = 0; i < listSize; i++) {
			mediumChangedCallback(mediumInfolist[i]);
		}
	}
}

void MediaDetectionEngine::deviceManagerProxyavailability(
		const CommonAPI::AvailabilityStatus& avail) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			">>> MediaDetectionEngine::deviceManagerProxyavailability : %s",
			mDevManager.getStringAvailibility(avail).c_str());
	mDevManagerProxyStatus = avail;
	if (CommonAPI::AvailabilityStatus::AVAILABLE == avail) {
		//	startProxyTasks();
		FacadeHelper<MediaPresCore>()->setDevManagerAvailable();
	}
}

void MediaDetectionEngine::startProxyTasks() {
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaDetectionEngine::startProxyTasks");
	mDevManager->getDevicesAsync(
			std::bind(&MediaDetectionEngine::getDevicesCallback, this, _1, _2,
					_3));
	SUBSCRIBE(mDevManager, getOnMediumChangedEvent(),
			&MediaDetectionEngine::mediumChangedCallback, _1)
	SUBSCRIBE(mDevManager, getSyncProgressEvent(),
			&MediaDetectionEngine::syncProgressCallback, _1, _2)
	SUBSCRIBE(mDevManager, getDeviceErrorEvent(),
			&MediaDetectionEngine::deviceErrorCallback, _1, _2)
	SUBSCRIBE(mDevManager, getOnRemotePlayerChangedEvent(),
			&MediaDetectionEngine::remotePlayerCallback, _1, _2)
}

void MediaDetectionEngine::btServiceProxyAvailability(
		const CommonAPI::AvailabilityStatus& avail) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			">>> MediaDetectionEngine::btServiceProxyAvailability : %s",
			mBTSVCHandler.getStringAvailibility(avail).c_str());
	if (CommonAPI::AvailabilityStatus::AVAILABLE == avail) {
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d ", __FUNCTION__, __LINE__);
		FacadeHelper<MediaPresCore>()->setBtServiceProxyAvailable();
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d ", __FUNCTION__, __LINE__);
		SUBSCRIBE(mBTSVCHandler, getServiceConnectedEvent(),
				&MediaDetectionEngine::serviceConnectedCallback, _1, _2, _3, _4)
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d ", __FUNCTION__, __LINE__);
		SUBSCRIBE(mBTSVCHandler, getServiceDisconnectedEvent(),
				&MediaDetectionEngine::serviceDisconnectedCallback, _1, _2, _3,
				_4)
		SUBSCRIBE(mBTSVCHandler, getBondedDeviceProfileListChangedEvent(),
				&MediaDetectionEngine::bondedDeviceProfileListChangedCallback,
				_1, _2)
		SUBSCRIBE(mBTSVCHandler, getVrModeEvent(),
				&MediaDetectionEngine::vrModeEventCallback, _1, _2, _3)
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d ", __FUNCTION__, __LINE__);
		SUBSCRIBE(mBTSVCHandler, getSiriStatusEvent(),
				&MediaDetectionEngine::siriStatusEventCallback, _1, _2, _3)
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d ", __FUNCTION__, __LINE__);
	}

	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d ", __FUNCTION__, __LINE__);
}

void MediaDetectionEngine::setActiveDevice(uint64_t deviceId) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			">>> MediaDetectionEngine::setActiveDevice- setting active device as: %llu",
			deviceId);
	mActiveDeviceId = deviceId;
}

DeviceBase* MediaDetectionEngine::getActiveDevice() {
	DeviceBase* pNewDevice = nullptr;
	std::unordered_map<uint64_t, DeviceBase*>::const_iterator itr;
	LOG_INFO(LOG_CONTEXT_MEDIA,
			">>> MediaDetectionEngine::ActiveDevice ID -: %llu size = %d",
			mActiveDeviceId, mDeviceList.size());

	if (mActiveDeviceId > 0) {
		itr = mDeviceList.find(mActiveDeviceId);
		if (itr != mDeviceList.end()) {
			pNewDevice = itr->second;
		}
	}

	return pNewDevice;
}

DeviceBase* MediaDetectionEngine::getDevice(uint64_t deviceId) {
	DeviceBase* pNewDevice = nullptr;
	auto itr = mDeviceList.find(deviceId);
	if (itr != mDeviceList.end()) {
		pNewDevice = itr->second;
	} else {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				">>> MediaDetectionEngine::Device not found for : %llu",
				deviceId);
	}

	return pNewDevice;
}

std::unordered_map<uint64_t, DeviceBase*> MediaDetectionEngine::getDeviceList() {
	return mDeviceList;
}

void MediaDetectionEngine::deviceInformationUpdate(uint64_t mediumID,
		MMPresTypes::eDeviceType type, const std::string& deviceName,
		uint32_t DeviceCaps) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::deviceInformationUpdate for updating device availability: medium id=%llu,device Type=%d",
			mediumID, (int )(type));

	int8_t SlotID = 0;

	SlotID = assignSlotToDevice(mediumID, type);

	if (SlotID <= 0) {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::deviceInformationUpdate updating device availability: there is no available slot for inserting a new medium with type=%ld",
				(int32_t )type);
	} else {
		for (unsigned int i = 0; i < mSlotList.size(); i++) {
			if (mSlotList[i].getSlotId() == SlotID) {
				mSlotList[i].setSlotState(
						MMPresTypes::eSlotState::SLOTSTATE_READY);
				MMPresTypes::tDevice device = mSlotList[i].getDevice();
				device.setDeviceId(mediumID);
				device.setDeviceType(type);
				device.setDeviceName(deviceName.c_str());
				device.setDeviceCaps(DeviceCaps);
				mSlotList[i].setDevice(device);
				LOG_INFO(LOG_CONTEXT_MEDIA,
						"<<< MediaDetectionEngine::deviceInformationUpdate updating device availability:deviceName=%s, DeviceCaps=%d",
						deviceName.c_str(), DeviceCaps);
				LOG_INFO(LOG_CONTEXT_MEDIA,
						"<<< MediaDetectionEngine::deviceInformationUpdate updating device availability:slotNumber=%d, device id=%llu",
						mSlotList[i].getSlotId(), device.getDeviceId());
				if (mTriggerSlotUpdate != nullptr) {
					mTriggerSlotUpdate(mSlotList[i]);
				}
				break;
			}
		}
	}

}

void MediaDetectionEngine::deviceInformationUpdate(uint64_t mediumID,
		MMPresTypes::tBTStatus status) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::deviceInformationUpdate for BT status: medium id=%llu",
			mediumID);
	int8_t SlotID = findMatchingSlot(mediumID);

	if (SlotID <= 0) {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::deviceInformationUpdate for BT status: no availablr slot");
	} else {
		for (unsigned int i = 0; i < mSlotList.size(); i++) {
			if (mSlotList[i].getSlotId() == SlotID) {
				MMPresTypes::tDevice device = mSlotList[i].getDevice();
				device.setBtstatus(status);
				mSlotList[i].setDevice(device);
				LOG_INFO(LOG_CONTEXT_MEDIA,
						"<<< MediaDetectionEngine::deviceInformationUpdate for BT status: slotNumber=%d device id=%llu",
						mSlotList[i].getSlotId(), device.getDeviceId());
				if (mTriggerSlotUpdate != nullptr) {
					mTriggerSlotUpdate(mSlotList[i]);
				}
				break;
			}
		}
	}
}

void MediaDetectionEngine::deviceInformationUpdate(uint64_t mediumID,
		MMPresTypes::ContentInfoMap contentInfoMap) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::deviceInformationUpdate for contentinfo : medium id=%llu",
			mediumID);
	int8_t SlotID = findMatchingSlot(mediumID);

	if (SlotID <= 0) {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::deviceInformationUpdate for contentinfo : caps: no availablr slot");
	} else {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::deviceInformationUpdate for contentinfo- contentinfo map is");
		// Video EOL is set to disable. hence setting video count to 0
		if (false == mMediaSettings->getIsVideoSupported()) {
			MMPresTypes::ContentInfoMap::iterator itr = contentInfoMap.find(
					MMPresTypes::eTitleContentType::TITLETYPE_VIDEO);
			if (itr != contentInfoMap.end())
				itr->second = 0;
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"<<< MediaDetectionEngine::Setting video count to 0 since Video EOL is set to disable");
		}
		for (MMPresTypes::ContentInfoMap::const_iterator ctr =
				contentInfoMap.begin(); ctr != contentInfoMap.end(); ctr++) {
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"<<< MediaDetectionEngine::deviceInformationUpdate- content type=%d,count=%llu",
					(int )(ctr->first), ctr->second);
		}

		for (unsigned int i = 0; i < mSlotList.size(); i++) {
			if (mSlotList[i].getSlotId() == SlotID) {
				MMPresTypes::tDevice device = mSlotList[i].getDevice();
				device.setContentInfo(contentInfoMap);
				mSlotList[i].setDevice(device);
				LOG_INFO(LOG_CONTEXT_MEDIA,
						"<<< MediaDetectionEngine::deviceInformationUpdate for content info: slotNumber=%d device id=%llu",
						mSlotList[i].getSlotId(), device.getDeviceId());
				if (mTriggerSlotUpdate != nullptr) {
					mTriggerSlotUpdate(mSlotList[i]);
				}
				break;
			}
		}
	}
}

void MediaDetectionEngine::deviceInformationUpdateRemoteCD(int deviceID,
		pRemoteCDSlot mRemoteCDSlot, int DeviceCaps, string DeviceName) {
	int8_t SlotID = findMatchingSlot(deviceID);

	if (SlotID <= 0) {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::deviceInformationUpdate for Remote CD status: no availablr slot");
		SlotID = assignSlotToDevice(deviceID, mRemoteCDSlot.first);
	}

	for (unsigned int i = 0; i < mSlotList.size(); i++) {
		if (mSlotList[i].getSlotId() == SlotID) {
			mSlotList[i].setSlotState(MMPresTypes::eSlotState::SLOTSTATE_READY);
			MMPresTypes::tDevice device = mSlotList[i].getDevice();
			device.setDeviceId(deviceID);
			device.setDeviceType(mRemoteCDSlot.first);
			device.setDeviceName(DeviceName.c_str());
			device.setDeviceCaps(DeviceCaps);
			mSlotList[i].setDevice(device);
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"<<< MediaDetectionEngine::deviceInformationUpdate updating Reemote CD device availability:deviceName=%s, DeviceCaps=%d",
					DeviceName.c_str(), DeviceCaps);
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"<<< MediaDetectionEngine::deviceInformationUpdate updating Reemote CD device availability:slotNumber=%d, device id=%llu",
					mSlotList[i].getSlotId(), device.getDeviceId());
			if (mTriggerSlotUpdate != nullptr) {
				mTriggerSlotUpdate(mSlotList[i]);
			}
		}
	}

}

void MediaDetectionEngine::getDeviceItemsInfo(uint32_t ClientID,
		uint32_t mediumId,
		DeviceManagerProxyBase::GetDeviceItemsInfoAsyncCallback callback) {
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaDetectionEngine::getDeviceItemsInfo");
	ASYNC_INVOKE_3ARG(mDevManager, DeviceManagerProxy,
			ARG_LIST_GETDEVICEITEMSINFO_RESPONSE, ClientID, callback,
			getDeviceItemsInfoAsync, mediumId,)
}

void MediaDetectionEngine::getDeviceHardwareInfo(uint32_t ClientID,
		uint32_t mediumId,
		DeviceManagerProxyBase::GetDeviceHarwareInfoAsyncCallback callback) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::getDeviceHardwareInfo");
	ASYNC_INVOKE_3ARG(mDevManager, DeviceManagerProxy,
			ARG_LIST_GETDEVICEHARDWAREINFO_RESPONSE, ClientID, callback,
			getDeviceHarwareInfoAsync, mediumId,)
}

void MediaDetectionEngine::getDeviceFirmwareInfo(uint32_t ClientID,
		uint32_t mediumId,
		DeviceManagerProxyBase::GetDeviceFirmwareInfoAsyncCallback callback) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::getDeviceFirmwareInfo");
	ASYNC_INVOKE_3ARG(mDevManager, DeviceManagerProxy,
			ARG_LIST_GETDEVICEFIRMWAREINFO_RESPONSE, ClientID, callback,
			getDeviceFirmwareInfoAsync, mediumId,)
}

void MediaDetectionEngine::getRemotePlayerCapability(uint32_t clientID,
		uint32_t mediumId,
		DeviceManagerProxyBase::GetRemotePlayerCapabilityAsyncCallback callback) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::getRemotePlayerCapability");
	ASYNC_INVOKE_3ARG(mDevManager, DeviceManagerProxy,
			ARG_LIST_GETREMOTEPLAYERCAPABILITY_RESPONSE, clientID, callback,
			getRemotePlayerCapabilityAsync, mediumId,)
}

void MediaDetectionEngine::initSlotsFromConfig() {
	mSlotList = mMediaSettings->getSlots();
}

void MediaDetectionEngine::setAppleChipAuthenticationCallBack(
		std::function<void(const bool&)> func) {
	mAppleChipAuthenticationCallback = func;
}

std::function<void(const bool&)>& MediaDetectionEngine::getAppleChipAuthenticationCallback() {
	return mAppleChipAuthenticationCallback;
}

void MediaDetectionEngine::initAppleAuthenticationCheck() {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::initAppleAuthenticationCheck");

	if (true == mMediaSettings->getAppleAuthenticationChipFunctionalCheck()) {
		//Attempt to read the registeries of the Apple authentication chip 
		int ifdACPDevice = -1;
		unsigned char data[1];

		//Open ACP Chip I2C bus
		ifdACPDevice = open(ACP_I2C_DEV_PATH, O_RDWR);

		if (ifdACPDevice <= 0) // On error, open will return -1.
				{
			LOG_INFO(LOG_CONTEXT_MEDIA, "Opening ACP chip I2C bus failed");
			mIsAppleAuthenticationFunctional = false;
		} else {
			data[0] = ACP_ADDR_DEVICE_VERSION;

			if (acpDeviceRead(&data[0], 1, ifdACPDevice) != 1) {
				LOG_INFO(LOG_CONTEXT_MEDIA, "I2C subaddress 0x00 read failed");
				mIsAppleAuthenticationFunctional = false;
			}
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"MediaDetectionEngine::initAppleAuthenticationChipFunctionalCheck-:Device Version [0x%02x]",
					data[0]);

			data[0] = ACP_ADDR_FIRMWARE_VERSION;

			if (acpDeviceRead(&data[0], 1, ifdACPDevice) != 1) {
				LOG_INFO(LOG_CONTEXT_MEDIA, "I2C subaddress 0x00 read failed");
				mIsAppleAuthenticationFunctional = false;
			}

			LOG_INFO(LOG_CONTEXT_MEDIA,
					"MediaDetectionEngine::initAppleAuthenticationChipFunctionalCheck-:Firmware Version [0x%02x]",
					data[0]);

			data[0] = ACP_ADDR_PROTOCOL_MAJOR;

			if (acpDeviceRead(&data[0], 1, ifdACPDevice) != 1) {
				LOG_INFO(LOG_CONTEXT_MEDIA, "I2C subaddress 0x00 read failed");
				mIsAppleAuthenticationFunctional = false;
			}

			LOG_INFO(LOG_CONTEXT_MEDIA,
					"MediaDetectionEngine::initAppleAuthenticationChipFunctionalCheck-:Protocol Major Version [0x%02x]",
					data[0]);

			data[0] = ACP_ADDR_PROTOCOL_MINOR;

			if (acpDeviceRead(&data[0], 1, ifdACPDevice) != 1) {
				LOG_INFO(LOG_CONTEXT_MEDIA, "I2C subaddress 0x00 read failed");
				mIsAppleAuthenticationFunctional = false;
			}
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"MediaDetectionEngine::initAppleAuthenticationChipFunctionalCheck-:Protocol Minor Version [0x%02x]",
					data[0]);

			/*			//Close the device
			 if (ifdACPDevice  > 0)
			 {
			 close(ifdACPDevice);
			 ifdACPDevice = -1;
			 }
			 */
			if (false == mIsAppleAuthenticationFunctional) {
				LOG_INFO(LOG_CONTEXT_MEDIA,
						"Apple Authentication chip Unaccessible!!");
			}
		}

		//Close the device
		if (ifdACPDevice > 0) {
			close(ifdACPDevice);
			ifdACPDevice = -1;
		}
		std::function<void(const bool&)> callback =
				getAppleChipAuthenticationCallback();

		if (callback != nullptr) {
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"MediaDetectionEngine::Apple Authentication chip Changed - firing AppleAuthentication");
			callback(mIsAppleAuthenticationFunctional);
		}
	}

}

int MediaDetectionEngine::acpDeviceRead(unsigned char data[],
		unsigned int dataLength, const int ifdACPDevice) {
	// Sanity check
	if (dataLength <= 0 || data == NULL || data[0] < ACP_ADDR_DEVICE_VERSION
			|| data[0] > ACP_ADDR_PROTOCOL_MINOR || ifdACPDevice <= 0) {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"MediaDetectionEngine::acpDeviceRead-Invalid args acpDeviceRead");

		//printf("Invalid args acpDeviceRead");
		return -1;
	}

	struct i2c_rdwr_ioctl_data rdWrData = { 0 };
	struct i2c_msg msg[1] = { 0 };

	// Write the i2c subaddress, you want to read

	rdWrData.nmsgs = 1;
	rdWrData.msgs = msg;
	msg[0].addr = ACP_I2C_SLAVE_WRITE_ADDRESS;
	// msg[0].flags = 0;  // Array of msg[] = {0} Code Sonaer warning
	msg[0].buf = data;
	msg[0].len = 0x01;

	int status = -1;
	unsigned int uRetry = 0;
	while ((status = ioctl(ifdACPDevice, I2C_RDWR, &rdWrData)) < 0
			&& uRetry < 10) {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"MediaDetectionEngine::acpDeviceRead-Subaddress write failed acpDeviceRead");
		usleep(500);
		uRetry++;
	}
	if (status < 0) {
		return -1;
	}

	// Echoed read

	rdWrData.nmsgs = 1;
	rdWrData.msgs = msg;
	// msg[0].addr = ACP_I2C_SLAVE_READ_ADDRESS;   //Repeated code. CodeSonar Warning
	msg[0].flags = I2C_M_RD;
	//msg[0].buf = data; //data;                  //Repeated code. CodeSonar Warning
	//msg[0].len = 0x01; //dataLength;            //Repeated code. CodeSonar Warning

	status = -1;
	uRetry = 0;

	// Apple Authtentication Coprocessor expects a retry if a read failed.
	// We will try a total of 10 times max which should waste around 5ms worst case.
	while (((status = ioctl(ifdACPDevice, I2C_RDWR, &rdWrData)) < 0)
			&& (uRetry < 10)) {
		usleep(500);
		uRetry++;
	}

	if (status < 0) {
		return -1;
	}

	return dataLength;
}

const MMPresTypes::tSlotList& MediaDetectionEngine::getSlotList() {
	return mSlotList;
}

int8_t MediaDetectionEngine::assignSlotToDevice(uint32_t mediumID,
		MMPresTypes::eDeviceType type) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::assignSlotToDevice: medium id=%lu,device type=%ld",
			mediumID, (int32_t )type);
	MMPresTypes::eSlotType slotType;
	int8_t slotNumber = 0;
	// check if a slot for the DeviceType is supported.If so,also obtain the slotType.
	bool ret = checkForSupportedSlot(type, slotType);
	if (ret == true) {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::assignSlotToDevice: slot type=%ld",
				(int32_t )slotType);

		// if slot is supported for the device type,then check if a slot already contains the mediumID.If the medium is already available, then it is an error scenario.
		// if the mediumID is not available in any of the slots,then return the slotID for the slotType which can contain the medium.
		int8_t slot = findMatchingSlot(mediumID);
		if (slot <= 0) {
			slotNumber = findEmptySlot(slotType);
		} else {
			slotNumber = slot;
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"<<< MediaDetectionEngine::assignSlotToDevice: a slot with slot id=%d and mediumid=%lu is already available",
					slot, mediumID);
		}
	} else {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::assignSlotToDevice: for medium id=%lu,device type=%ld, there is no supported slot",
				mediumID, (int32_t )type);
	}
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::assignSlotToDevice: slotNumber=%d",
			slotNumber);
	return slotNumber;
}

// this actually returns a slot number if it is in ejected state.It means that, a new compatible device can be inserted in that slot.
int8_t MediaDetectionEngine::findEmptySlot(
		const MMPresTypes::eSlotType& slotType) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::findEmptySlot: slot type=%ld", slotType);
	uint8_t size = mSlotList.size();
	for (uint8_t i = 0; i < size; i++) {
		if (mSlotList[i].getSlotType() == slotType
				&& mSlotList[i].getSlotState()
						== MMPresTypes::eSlotState::SLOTSTATE_EJECTED) {
			return mSlotList[i].getSlotId();
		}
	}
	return 0;
}

// this actually returns the slot number which contains the mediumID..
int8_t MediaDetectionEngine::findMatchingSlot(uint32_t mediumID) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::findMatchingSlot: medium id=%lu",
			mediumID);
	uint8_t size = mSlotList.size();
	for (uint8_t i = 0; i < size; i++) {
		if (mSlotList[i].getDevice().getDeviceId() == mediumID) {
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"<<< MediaDetectionEngine::findMatchingSlot: (slotnumber)=%d,device Id=%llu",
					mSlotList[i].getSlotId(),
					mSlotList[i].getDevice().getDeviceId());
			return mSlotList[i].getSlotId();
		}
	}
	return 0; // Not found. 
}

bool MediaDetectionEngine::checkForSupportedSlot(
		const MMPresTypes::eDeviceType& devType,
		MMPresTypes::eSlotType& slotType) {
	bool ret = false;
	slotType = MMPresTypes::eSlotType::SLOTTYPE_UNKNOWN;
	std::unordered_map<MMPresTypes::eDeviceType, MMPresTypes::eSlotType>::const_iterator itr =
			mDeviceToSlotMap.find(devType);
	if (itr != mDeviceToSlotMap.end()) {
		ret = true;
		slotType = itr->second;
	}
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::checkForSupportedSlot: ret=%d", ret);
	return ret;
}

void MediaDetectionEngine::cleanupSlot(uint8_t slotID,
		BaseType::tMediumInfo& mediuminfo) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::cleanupSlot: clean up slotId=%d", slotID);
	for (unsigned int i = 0; i < mSlotList.size(); i++) {
		if (mSlotList[i].getSlotId() == slotID) {
			MMPresTypes::tDevice device(0u,
					MMPresTypes::eDeviceType::DEVICETYPE_UKNOWN, 0u, 0u, 0u, "",
					MMPresTypes::ContentInfoMap(), MMPresTypes::tBTStatus());
			if (mediuminfo.getMMediumType() == BaseType::tMediumType::MT_HDD) {
				device.setDeviceType(MMPresTypes::eDeviceType::DEVICETYPE_HDD);
			} else if (mediuminfo.getMMediumType()
					== BaseType::tMediumType::MT_DISCSINGLE) {
				device.setDeviceType(
						MMPresTypes::eDeviceType::DEVICETYPE_DISCSINGLE);
			} else if (mediuminfo.getMMediumType()
					== BaseType::tMediumType::MT_DISCCHANGER) {
				device.setDeviceType(
						MMPresTypes::eDeviceType::DEVICETYPE_DISCCHANGER);
			} else if (mediuminfo.getMMediumType()
					== BaseType::tMediumType::MT_UMASS) {
				device.setDeviceType(MMPresTypes::eDeviceType::DEVICETYPE_USB);
			} else if (mediuminfo.getMMediumType()
					== BaseType::tMediumType::MT_IPOD) {
				device.setDeviceType(MMPresTypes::eDeviceType::DEVICETYPE_IPOD);
			} else if (mediuminfo.getMMediumType()
					== BaseType::tMediumType::MT_MTP) {
				device.setDeviceType(MMPresTypes::eDeviceType::DEVICETYPE_MTP);
			} else if (mediuminfo.getMMediumType()
					== BaseType::tMediumType::MT_BLUETOOTH) {
				device.setDeviceType(
						MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH);
			}

			mSlotList[i].setDevice(device);
			mSlotList[i].setSlotState(
					MMPresTypes::eSlotState::SLOTSTATE_EJECTED);
			if (mTriggerSlotUpdate != nullptr) {
				LOG_INFO(LOG_CONTEXT_MEDIA,
						"<<< MediaDetectionEngine::cleanupSlot: trigger slotupdate for unavailability of slotId=%d",
						slotID);
				mTriggerSlotUpdate(mSlotList[i]);
			}
			break;
		}
	}
}

// this will return the contained device if the slot is in READY state.
uint64_t MediaDetectionEngine::getDevice(uint8_t slotId) {
	uint64_t deviceId = 0;
	for (unsigned int i = 0; i < mSlotList.size(); i++) {
		if (mSlotList[i].getSlotId() == slotId
				&& mSlotList[i].getSlotState()
						== MMPresTypes::eSlotState::SLOTSTATE_READY) {
			deviceId = mSlotList[i].getDevice().getDeviceId();
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"<<< MediaDetectionEngine::getDevice: slotIdId=%d,mediumId=%d",
					slotId, deviceId);
			return deviceId;
		}
	}
	return deviceId;
}

MMPresTypes::eDeviceType MediaDetectionEngine::getDeviceType(uint8_t slotId) {
	MMPresTypes::eDeviceType deviceType =
			MMPresTypes::eDeviceType::DEVICETYPE_UKNOWN;
	for (unsigned int i = 0; i < mSlotList.size(); i++) {
		if (mSlotList[i].getSlotId() == slotId) {
			//deviceType = mSlotList[i].getDevice().getDeviceId();
			deviceType = mSlotList[i].getDevice().getDeviceType();
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"<<< MediaDetectionEngine::getDevice: slotIdId=%d,deviceType=%d",
					slotId, deviceType);
			return deviceType;
		}
	}
	return deviceType;
}

void MediaDetectionEngine::syncProgressCallback(const uint32_t& mediumId,
		const double& progress) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::syncProgressCallback: mediumId=%d,progress=%f",
			mediumId, progress);
}

void MediaDetectionEngine::deviceErrorCallback(const uint32_t& mediumId,
		const ::v1::com::harman::media::BaseType::tBaseError& error) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::deviceErrorCallback: mediumId=%d,error=%d",
			mediumId, (int32_t )error);
	auto it = mBaseToMediaError.find(error);
	if (it != mBaseToMediaError.end()) {
		uint8_t slotId = findMatchingSlot(mediumId);
		uint64_t temp_deviceId = getDevice(slotId);
		DeviceBase* temp_device = getDevice(temp_deviceId);
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::deviceErrorCallback: mediumId=%d,it->second=%d",
				mediumId, (int32_t )it->second);
		if (it->second
				== MMPresTypes::eMediaError::MEDIAERROR_INVALIDPARAMATERS) {
			setMediumIdForResumePlay(MMPresTypes::eDeviceType::DEVICETYPE_USB,
					mediumId);
		} else if (nullptr != temp_device) {
			if (it->second == MMPresTypes::eMediaError::MEDIAERROR_WRONGSTATE) {
				setMediumIdForResumePlay(
						MMPresTypes::eDeviceType::DEVICETYPE_USB, mediumId);
			}
			temp_device->triggerMediaErrorToDevice(slotId, it->second);
		} else {
			//Do Nothing
		}

	} else {
		LOG_INFO(LOG_CONTEXT_MEDIA, "Got invalid DeviceError");
	}
}

void MediaDetectionEngine::serviceConnectedCallback(
		const std::string& macAddress, const std::string& name,
		const std::string& service, const bool& userInitiatedConn) {

}

void MediaDetectionEngine::serviceDisconnectedCallback(
		const std::string& macAddress, const std::string& name,
		const std::string& service, const std::string& reason) {

}

void MediaDetectionEngine::bondedDeviceProfileListChangedCallback(const std::string& macAddress, const ::com::harman::connectivity::btTypedefs::profileList& profileList) 
{
	//LOG_INFO(LOG_CONTEXT_MEDIA,"<<< MediaDetectionEngine::bondedDeviceProfileListChangedCallback: macAddress=%s",macAddress.c_str());
	std::unordered_map<std::string, btTypedefs::profileList>::iterator it;
	it = mBtBondedDevList.find(macAddress);
	if (it != mBtBondedDevList.end()) 
   {
		it->second.clear();
		it->second = profileList;
	} 
   else 
   {
		mBtBondedDevList.insert(
				std::pair<std::string, btTypedefs::profileList>(macAddress,
						profileList));
	}
}

void MediaDetectionEngine::vrModeEventCallback(const std::string& macAddress,
		const bool& modeVr, const bool& active) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::vrModeEventCallback - modeVr = %d, active = %d", (int )modeVr, (int )active);
	isVRActive = active;
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::vrModeEventCallback isVRActive = %d",
			isVRActive);
	/* start timer */

	if (getVRmodeActive() != modeVr) {
		setVRmodeActive(modeVr);
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::vrModeEventCallback VRmode = %d",
				getVRmodeActive());
		if (false == getVRmodeActive()) {
			startTimer();
		} else {
			cancelTimer();
		}
	}
}

void MediaDetectionEngine::siriStatusEventCallback(
		const std::string& macAddress, const bool& siriAvailable,
		const bool& siriEnabled) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::siriStatusEventCallback - siriAvailable = %d, siriEnabled = %d", (int )siriAvailable, (int )siriEnabled);
	//mSiriEnabled=siriEnabled;
	setmSiriEnabled(siriEnabled);
	/* start timer */
	/*if(siriAvailable && siriEnabled)
	 startTimer();
	 */
}

void MediaDetectionEngine::startTimer() {
	/* start timer of 5 seconds */
	timerId = mTimer.setTimer(CTimerInterface::TIMER_ONESHOT, 5000);
	LOG_INFO(LOG_CONTEXT_MEDIA,
			">>> MediaDetectionEngine::startTimer-timerId=%d", timerId);
	setbTimerTimeExpired(false);
	//bTimerTimeExpired = false;
}

void MediaDetectionEngine::cancelTimer() {
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaDetectionEngine::cancelTimer");

	mTimer.cancelTimer(timerId);
	timerId = 0;
	setbTimerTimeExpired(true);
	mAudioInterface->setPerformBTSASourceSwitch(false);
	//bTimerTimeExpired=true;
}

void MediaDetectionEngine::timerEvent(int timerId) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::timerEvent - timerId = %d", timerId);

	setbTimerTimeExpired(true);
   mAudioInterface->setPerformBTSASourceSwitch(false);
	//bTimerTimeExpired = true;
}

std::unordered_map<std::string, btTypedefs::profileList> MediaDetectionEngine::getBondedDeviceList() {
	return mBtBondedDevList;
}

uint64_t MediaDetectionEngine::getActiveDeviceId() {
	return mActiveDeviceId;
}
const MMPresTypes::tAdditionalDeviceInfo MediaDetectionEngine::getAdditionalDevDetails(
		uint8_t slotId) {
	uint64_t temp_deviceId = getDevice(slotId);
	DeviceBase* temp_device = getDevice(temp_deviceId);
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::getAdditionalDevDetails: slotid:%d,deviceid:%llu",
			slotId, temp_deviceId);
	MMPresTypes::tAdditionalDeviceInfo temp_info;
	if (temp_device != nullptr) {
		temp_info = temp_device->getAdditionalDeviceInfo();
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::getAdditionalDevDetails: device not empty !!!");
	}
	return temp_info;
}

bool MediaDetectionEngine::iPodNbluetoothCheck() {
	std::string blueThoothDeviceName;
	std::string iPodDeviceName;
   LOG_INFO(LOG_CONTEXT_MEDIA,"<<< MediaDetectionEngine::mSlotList size: :%d", mSlotList.size());

	for (unsigned int i = 0; i < mSlotList.size(); i++) {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::Device Name %s >>> Type %d",
				mSlotList.at(i).getDevice().getDeviceName().c_str(),
				mSlotList.at(i).getDevice().getDeviceType());
		if (mSlotList.at(i).getDevice().getDeviceType()
				== MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH) {
			blueThoothDeviceName =
					mSlotList.at(i).getDevice().getDeviceName().c_str();
		}
		if (mSlotList.at(i).getDevice().getDeviceType()
				== MMPresTypes::eDeviceType::DEVICETYPE_IPOD) {
			iPodDeviceName =
					mSlotList.at(i).getDevice().getDeviceName().c_str();
		}
	}
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::Device Name %s >>> %s iPodNbluetoothCheck %d",
			blueThoothDeviceName.c_str(), iPodDeviceName.c_str(), (blueThoothDeviceName.c_str() == iPodDeviceName.c_str()));
   if (!(blueThoothDeviceName.c_str() == iPodDeviceName.c_str())
      && blueThoothDeviceName.length() && iPodDeviceName.length())
   {
      return true;
   }
   else
   {
      return false;
   }
}

DeviceBase* MediaDetectionEngine::createDeviceRemoteCD(
		eCDStatus mCDStatusEvent) {
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< createDeviceRemoteCD  >>> %d ",
			(int )mCDStatusEvent);
	DeviceBase* pNewDevice = nullptr;
	switch (mCDStatusEvent) {

	case eLoaded: {
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< create RemoteCD device");
		pNewDevice = new DeviceRemoteCD(mCDStatusEvent);
		if (pNewDevice)
			pNewDevice->subscribeToPlayerEvents();
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< Called subscribeToPlayerEvents");

	}
		break;

	default:
		break;

	}

	if (nullptr == pNewDevice) {

	} else {

		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< Adding  RemoteCD device in the devicelist");
		mDeviceList[REMOTE_CD_DEVICE_ID] = pNewDevice;

	}

	return pNewDevice;
}

void MediaDetectionEngine::CDStatusEvent(eCDStatus mCDStatusEvent) {
	// LOG_INFO(LOG_CONTEXT_MEDIA, "<<< CDStatusEvent");
	LOG_INFO(LOG_CONTEXT_MEDIA, "MediaDetectionEngine::CD StatusEvent:%d",
			mCDStatusEvent);

	auto it = mDeviceList.find(REMOTE_CD_DEVICE_ID);

	switch (mCDStatusEvent) {
	case eLoaded: {
		if (it != mDeviceList.end()) {
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"MediaDetectionEngine::Before DeviceRemoteCD SubscribeToPlayerEvents");

			LOG_INFO(LOG_CONTEXT_MEDIA,
					"MediaDetectionEngine::After DeviceRemoteCD SubscribeToPlayerEvents");
		} else {
			LOG_INFO(LOG_CONTEXT_MEDIA,
					"MediaDetectionEngine::creating Device for RemoteCD:%d",
					mCDStatusEvent);
			PresCtrl::addRequest(e_REQUEST_DEVICEINFO_TASK,
					std::bind(&MediaDetectionEngine::createDeviceRemoteCD, this,
							mCDStatusEvent));
			;
		}
	}
		break;

	case ePrepareEject: {
		if (it != mDeviceList.end()) {

			// unsubscribe the player events.Removed this from the destuctor as derived class virtual functions are used.
			it->second->unSubscribeToPlayerEvents();
			delete it->second;
			mDeviceList.erase(it);
			int8_t slotId = findMatchingSlot(REMOTE_CD_DEVICE_ID);
			if (slotId > 0) {
				cleanupSlotRemoteCD(slotId);
				//need to write slot cleanup logic
				//cleanupSlot(slotId,temp_mediuminfo);
			}
		}

	}
		break;
	default:
		break;
	}

}

void MediaDetectionEngine::cleanupSlotRemoteCD(uint8_t slotID) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::cleanupSlot: clean up slotId=%d", slotID);
	for (unsigned int i = 0; i < mSlotList.size(); i++) {
		if (mSlotList[i].getSlotId() == slotID) {
			MMPresTypes::tDevice device(0u,
					MMPresTypes::eDeviceType::DEVICETYPE_UKNOWN, 0u, 0u, 0u, "",
					MMPresTypes::ContentInfoMap(), MMPresTypes::tBTStatus());

			// Here need to add the device type
			/*  if (mediuminfo.getMMediumType() == BaseType::tMediumType::MT_HDD) {
			 device.setDeviceType(MMPresTypes::eDeviceType::DEVICETYPE_HDD);
			 }*/

			device.setDeviceType(
					MMPresTypes::eDeviceType::DEVICETYPE_REMOTE_CD);
			mSlotList[i].setDevice(device);
			mSlotList[i].setSlotState(
					MMPresTypes::eSlotState::SLOTSTATE_EJECTED);
			if (mTriggerSlotUpdate != nullptr) {
				LOG_INFO(LOG_CONTEXT_MEDIA,
						"<<< MediaDetectionEngine::cleanupSlot: trigger slotupdate for unavailability of slotId=%d",
						slotID);
				mTriggerSlotUpdate(mSlotList[i]);
			}
			break;
		}
	}
}
void MediaDetectionEngine::setMediumIdForResumePlay(
		MMPresTypes::eDeviceType type, const uint64_t& mediumId) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::SetMediumIdForResumePlay Devicetype=%d, SetmediumId=%d",
			type, mediumId);
	auto itr = mMediumIdResumePlay.find(type);
	if (itr != mMediumIdResumePlay.end()) {
		itr->second = mediumId;
	}

}
uint64_t MediaDetectionEngine::getMediumIdForResumePlay(
		MMPresTypes::eDeviceType type) {
	uint64_t mediumId = 0;
	auto itr = mMediumIdResumePlay.find(type);
	if (itr != mMediumIdResumePlay.end()) {
		mediumId = itr->second;
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::getMediumIdForResumePlay Devicetye found,for Device type=%d,Get mediumId=%d",
				type, mediumId);
	} else {
		LOG_INFO(LOG_CONTEXT_MEDIA,
				"<<< MediaDetectionEngine::getMediumIdForResumePlay Devicetye not found");
	}
	return (mediumId);
}

void MediaDetectionEngine::invalidParameterDoSourceSwitch(
		const uint64_t& mediumId) {
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::invalidParameterDoSourceSwitch:mediumId=%d",
			mediumId);
	uint32_t SlotID = findMatchingSlot(mediumId);
	//MMPresTypes::tSlotList mSlotList = mDetect->getSlotList();
	::v3::org::genivi::am_t::am_sourceID_t targetSourceID =
			am_t::am_sourceID_e::eSRC_UNKNOWN;
	if (SlotID == 1) {
		targetSourceID = am_t::am_sourceID_e::eSRC_USB1;
	} else if (SlotID == 2) {
		targetSourceID = am_t::am_sourceID_e::eSRC_USB2;
	}
	LOG_INFO(LOG_CONTEXT_MEDIA,
			"<<< MediaDetectionEngine::invalidParameterDoSourceSwitch:SlotID=%d,targetSourceID=%d",
			SlotID, targetSourceID);
	mAudioInterface->doSourceSwitch(targetSourceID,
			am_t::am_sinkID_e::eSNK_SPKS);
}
void MediaDetectionEngine::setVRmodeActive(bool isModeVR) {
	bisVRmodeActive = isModeVR;
}
bool MediaDetectionEngine::getVRmodeActive() {
	return (bisVRmodeActive);
}

void MediaDetectionEngine::setbTimerTimeExpired(bool isexpired) {
	bTimerTimeExpired = isexpired;
}
bool MediaDetectionEngine::getbTimerTimeExpired() {
	return (bTimerTimeExpired);
}
void MediaDetectionEngine::setmSiriEnabled(bool isEnabled) {
	mSiriEnabled = isEnabled;
}
bool MediaDetectionEngine::getmSiriEnabled() {
	return (mSiriEnabled);
}
