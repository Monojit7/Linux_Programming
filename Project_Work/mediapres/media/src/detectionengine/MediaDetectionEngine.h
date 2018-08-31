/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            MediaDetectionEngine.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           The heart of the MediaDetectionEngine. 
*/

#ifndef _MEDIA_DETECTION_ENGINE_H
#define _MEDIA_DETECTION_ENGINE_H

#include<core.h>
#include <unordered_map>
#include<config/MediaPresSettings.h>
#include <v1/com/harman/mmpres/MediaSettingsStub.hpp>
//#include<audiomanagerinterface/audiomanagerinterface.h>
//#include <v1/com/harman/mmpres/MediaSettingsStubDefault.hpp>

using namespace v3::com::harman::mmpres; 
using namespace v1::com::harman::media;
using namespace v4::com::harman::connectivity;
using namespace com::harman::connectivity;

#define ARG_LIST_GETDEVICES_RESPONSE <CommonAPI::CallStatus,::v1::com::harman::media::BaseType::tBaseError,::v1::com::harman::media::BaseType::tMediumInfoList>
#define ARG_LIST_GETDEVICEITEMSINFO_RESPONSE <CommonAPI::CallStatus,::v1::com::harman::media::BaseType::tItemsInfo,::v1::com::harman::media::BaseType::tBaseError>
#define ARG_LIST_GETDEVICEHARDWAREINFO_RESPONSE <CommonAPI::CallStatus,::v1::com::harman::media::BaseType::tHardwareInfo,::v1::com::harman::media::BaseType::tBaseError>
#define ARG_LIST_GETDEVICEFIRMWAREINFO_RESPONSE <CommonAPI::CallStatus,::v1::com::harman::media::BaseType::tFirmwareInfo,::v1::com::harman::media::BaseType::tBaseError>
#define ARG_LIST_GETREMOTEPLAYERCAPABILITY_RESPONSE <CommonAPI::CallStatus,::v1::com::harman::media::BaseType::tRemotePlayerCapability,::v1::com::harman::media::BaseType::tBaseError>

typedef std::pair <MMPresTypes::eDeviceType, int> pRemoteCDSlot;

class DeviceBase; 
class MediaPresSettings;
class AudioManagerInterface;
class MediaDetectionEngine:public ITimerEventListener
{
public:

	MediaDetectionEngine();
	DeviceBase* getActiveDevice();
	DeviceBase* getDevice(uint64_t deviceId);
	void setActiveDevice(uint64_t deviceId);
	std::unordered_map<uint64_t, DeviceBase*> getDeviceList();
	uint64_t getDevice(uint8_t slotId);
    MMPresTypes::eDeviceType getDeviceType(uint8_t slotId);
	const MMPresTypes::tSlotList& getSlotList(); 
	const MMPresTypes::tAdditionalDeviceInfo getAdditionalDevDetails(uint8_t slotId);
	std::unordered_map<std::string, btTypedefs::profileList> getBondedDeviceList();


	/**
	* Basic Bare minimum device information expected from a valid device. This must be the first function called during early detection. 
	*/
	void deviceInformationUpdate(uint64_t mediumID, MMPresTypes::eDeviceType type, const std::string& deviceName, uint32_t DeviceCaps);

	void deviceInformationUpdate(uint64_t mediumID, MMPresTypes::ContentInfoMap contentInfoMap);

	void deviceInformationUpdate(uint64_t mediumID, MMPresTypes::tBTStatus);
    
    void deviceInformationUpdateRemoteCD(int deviceID, pRemoteCDSlot mRemoteCDSlot  , int DeviceCaps, string DeviceName);

	inline void setSlotUpdateHandler(std::function<void(const ::v3::com::harman::mmpres::MMPresTypes::tSlot &)> && triggerFunc)
	{
		mTriggerSlotUpdate = std::move( triggerFunc);
	}

	void cleanupSlot(uint8_t slotID,BaseType::tMediumInfo& mediuminfo);
    
    void cleanupSlotRemoteCD(uint8_t slotID);

	int8_t findMatchingSlot(uint32_t mediumID);

	void getDeviceItemsInfo(uint32_t clientID,uint32_t mediumId,DeviceManagerProxyBase::GetDeviceItemsInfoAsyncCallback);
	void getDeviceHardwareInfo(uint32_t clientID, uint32_t mediumId,DeviceManagerProxyBase::GetDeviceHarwareInfoAsyncCallback);
	void getDeviceFirmwareInfo(uint32_t clientID, uint32_t mediumId,DeviceManagerProxyBase::GetDeviceFirmwareInfoAsyncCallback);
	void getRemotePlayerCapability(uint32_t clientID, uint32_t mediumId,DeviceManagerProxyBase::GetRemotePlayerCapabilityAsyncCallback);
	void startProxyTasks();
	uint64_t getActiveDeviceId();

	void rvcDisplayStatusEvent(const bool& status);
	void apaStatusEvent(const bool& status);
	void pdcStatusEvent(const bool& status);
	void supportParkingAssistance(const bool& RVCstatus, const bool& APAStatus, const bool& PDCStatus);

	void startTimer();
	void cancelTimer();
	void timerEvent(int timerid);

	bool iPodNbluetoothCheck();
    void initAppleAuthenticationCheck();
    void setAppleChipAuthenticationCallBack(std::function<void(const bool&)>);
    std::function<void(const bool&)>& getAppleChipAuthenticationCallback();
    uint64_t getMediumIdForResumePlay(MMPresTypes::eDeviceType type);
    void setMediumIdForResumePlay(MMPresTypes::eDeviceType type,const uint64_t& mediumId);
    void setVRmodeActive(bool ismodeVR);
    bool getVRmodeActive();
    void setbTimerTimeExpired(bool isexpired);
    bool getbTimerTimeExpired();
    void setmSiriEnabled(bool isEnabled);
    bool getmSiriEnabled();
protected:

	void deviceManagerProxyavailability(const CommonAPI::AvailabilityStatus& avail);
	void getDevicesCallback(const CommonAPI::CallStatus& , const ::v1::com::harman::media::BaseType::tBaseError&,const ::v1::com::harman::media::BaseType::tMediumInfoList&);
	void mediumChangedCallback(const v1::com::harman::media::BaseType::tMediumInfo& MediumInfo);
	void syncProgressCallback(const uint32_t&,const double&);
	void deviceErrorCallback(const uint32_t&, const ::v1::com::harman::media::BaseType::tBaseError&);
   void remotePlayerCallback(const uint32_t&, const ::v1::com::harman::media::BaseType::tRemotePlayerCapability&);

	void btServiceProxyAvailability(const CommonAPI::AvailabilityStatus& avail);
	void serviceConnectedCallback(const std::string&, const std::string&, const std::string&, const bool&);
	void serviceDisconnectedCallback(const std::string&, const std::string&, const std::string&, const std::string&);
	void bondedDeviceProfileListChangedCallback(const std::string&, const ::com::harman::connectivity::btTypedefs::profileList&);
	void vrModeEventCallback(const std::string&, const bool&, const bool&);
	void siriStatusEventCallback(const std::string&, const bool&, const bool&);
    
    DeviceBase* createDeviceRemoteCD( eCDStatus mCDStatusEvent);
    int getMatchingSlotsRemoteCD();
    void CDStatusEvent(eCDStatus mCDStatusEvent);


private:

	void init();

	uint32_t registerClient()
	{
		mDevManager.addClient(++mNumClients);
		return mNumClients;
	}
	void deregisterClient(uint32_t ClientID)
	{
		mDevManager.removeClient(ClientID);
	}
	DeviceBase* createDevice(const v1::com::harman::media::BaseType::tMediumInfo& MediumInfo);

	int8_t assignSlotToDevice(uint32_t mediumID, MMPresTypes::eDeviceType);

	// this actually returns a slot number if it is in ejected state.It means that, a new compatible device can be inserted in that slot.
	int8_t findEmptySlot(const MMPresTypes::eSlotType& slotType);

	// check if a slot for the DeviceType is supported.If so,also obtain the slotType.
	bool checkForSupportedSlot(const MMPresTypes::eDeviceType& devType, MMPresTypes::eSlotType& slotType);
    void invalidParameterDoSourceSwitch(const uint64_t& mediumId);

	friend class FacadeHelper<MediaDetectionEngine>;

	void initSlotsFromConfig(); 	
	void initPluginEvents();

	int acpDeviceRead(unsigned char data[], unsigned int dataLength, const int ifdACPDevice);
   bool bTimerTimeExpired;
   bool bisVRmodeActive;
   bool isVRActive;
   bool mSiriEnabled;
   bool mPauseForParkingAssistance;
   uint32_t mNumClients;
	MMPresTypes::tSlotList		   mSlotList; 
	std::unordered_map<uint64_t,DeviceBase*> mDeviceList; 
	CommonAPIClientAbstraction<DeviceManagerProxy> mDevManager;
	std::function<void(const ::v3::com::harman::mmpres::MMPresTypes::tSlot &)> mTriggerSlotUpdate; 
	uint64_t mActiveDeviceId; 
    std::unordered_map<MMPresTypes::eDeviceType, uint64_t > mMediumIdResumePlay;

    //uint32_t mMediumIdResumePlay;
	FacadeHelper<MediaPresSettings> mMediaSettings;
    FacadeHelper<AudioManagerInterface> mAudioInterface;
	static std::unordered_map<MMPresTypes::eDeviceType, MMPresTypes::eSlotType> mDeviceToSlotMap;
	static std::unordered_map<BaseType::tBaseError, MMPresTypes::eMediaError> mBaseToMediaError;
	std::unordered_map<std::string, btTypedefs::profileList> mBtBondedDevList;
	CommonAPI::AvailabilityStatus mDevManagerProxyStatus;
	CommonAPIClientAbstraction<BtServiceProxy> mBTSVCHandler;
	bool mIsAppleAuthenticationFunctional;
    CTimerInterface mTimer;
    int timerId;
    std::function<void(const bool&)> mAppleChipAuthenticationCallback;

    bool mRVCStatus;
    bool mAPAStatus;
    bool mPDCStatus;
};

#endif
