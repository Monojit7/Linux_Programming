/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            audiomanagerinterface.h
* @ingroup         mediapresctrl
* @author          sriram srivatsan (sriram.srivatsan@harman.com)
* @brief           This contains ths interfaces for communicating with AM(Audio Manager) for various functionality such as 
                   registering/deregistering a media source/device,notifying media device availability/unavailability to AM.
*/

#ifndef _AUDIOMANAGERINTERFACE_H
#define _AUDIOMANAGERINTERFACE_H

#include<config/MediaPresSettings.h>
#include<detectionengine/MediaDetectionEngine.h>


using namespace v3::org::genivi::am::routinginterface;
using namespace v3::org::genivi::am::commandinterface;
using namespace v3::org::genivi;
using namespace v3::com::harman::mmpres;
using namespace v1::com::harman::audio::audioPresCtrl;
using namespace v3::com::harman::audio::audioPresCtrl;

typedef map<uint16_t, uint16_t> SOURCELIST;

#define DOMAIN_NAME "org.genivi.am"
#define BUS_NAME "mmpres.routingControl"
#define ABSOLUTEVOLUME 0.033
#define MAXMEDIAVOLUME 1.0
#define MAXAUDVOLUME 30

class MediaPresSettings;
class AudioManagerInterface:public ITimerEventListener
{
public:

   // update media device availability/unavailability to AM.
   bool updateSourceAvailabilitytoAM(const uint64_t& deviceId);
   bool updateSourceUnavailabilitytoAM(const uint64_t& deviceId);

   // get the deviceId for the particular sourceID.
   uint64_t getDeviceId(const am_t::am_sourceID_t& sourceId);
   
   // sets the asynchronous broadcast informing the state(SS_ON,SS_PAUSED,SS_OFF) of media source.This is invoked from RoutingControlStubImpl during the construction/registration of routing control interface.
   void setAsyncSetSourceStateFunction(std::function<void(const ::v3::org::genivi::am_t::am_Handle_s &_handle, const ::v3::org::genivi::am_t::am_Error_e &_amError, const std::shared_ptr<CommonAPI::ClientIdList> _receivers)> func);
   
   // this func adds the incoming request handle from AM to the source structure.This is just to track what all requests belong to a particular source.
   void updateRequestInfo(am_t::am_sourceID_t sourceID,am_t::am_t::am_Handle_s handle, am_t::am_SourceState_e sourceState);
   
   // update the ACK info for the source switch(handle,source id,source state)  request from AM.
   void updateError(am_t::am_sourceID_t sourceID, am_t::am_Error_e error);
   void updateError(uint64_t deviceId, am_t::am_Error_e error);

   void updateAudioSourceInfotoAudioPres(const uint64_t& deviceId);
   void startAMProxyTasks();
   void startAudioPresProxyTasks();
   void startCommandControlProxyTasks();
   am_t::am_sourceID_e getSourceID(const uint64_t& deviceId);
   void setBTDisconnectReason(const am_t::am_Error_e disconnectReason);
   void setBTAvailabilityReason(const am_t::am_AvailabilityReason_e reason);

   void doSourceSwitch(const ::v3::org::genivi::am_t::am_sourceID_t _sourceID, const ::v3::org::genivi::am_t::am_sinkID_t _sinkID);
   void CommandControlConnect();
    bool isCallOnGoing();
    void startTimer();
    void cancelTimer();
    void timerEvent(int timerid);
    void setActiveDeviceBTLinkLossStubCallback(std::function<void(const bool&)> func);
    void setmbTimerTimeExpired(bool);
    const bool getmbTimerTimeExpired();
    u_int16_t getActiveSourceId();
    void setPerformBTSASourceSwitch(bool);
    const bool getPerformBTSASourceSwitch();

private:

	u_int16_t activeSourceId = 0;

    bool onGoingCall;
    bool mbdoSourceSwitch = false;
    bool mMainSourceSoundPropertyChangedSignalHandle;
    bool mbTimerTimeExpired;
    int mtimerId;
    CTimerInterface mTimer;
    std::function<void(const bool&)> mActiveDeviceBTLinkLossStubCallback;

   // this describes the requestinfo from AM for a source switch
   struct RequestInfo
   {
      am_t::am_Handle_s mSourceHandle;
      am_t::am_SourceState_e mRequestedSourceState;
      RequestInfo()
      {

      }
      RequestInfo(const am_t::am_Handle_s& sourceHandle, const am_t::am_SourceState_e& requestedSourceState)
         :mSourceHandle(sourceHandle)
         ,mRequestedSourceState(requestedSourceState)
      {
      }
   };

   // this represents the audio source maintained by pres.This is initialised based on project config.
   struct Source
   {
    // static params
    const am_t::am_sourceID_e& mSourceIdEnum; // manadatory param for source registration
    const MMPresTypes::eSlotType& mSlotType;
    const std::string mName; // mandatory param for source registration

    // variable params
    am_t::am_sourceID_t mSourceIdValue;
    am_t::am_Availability_s mAvailability;
    am_t::am_SourceState_e mSourceState;
    uint64_t mDeviceId;
    bool registered;
    RequestInfo mRequestInfo;

    Source(const am_t::am_sourceID_e& sourceIdEnum, const MMPresTypes::eSlotType& slotType,const std::string name)
       :mSourceIdEnum(sourceIdEnum)
       ,mSlotType(slotType)
       ,mName(name)
       ,mSourceIdValue(0u)
       ,mSourceState(am_t::am_SourceState_e::SS_UNKNNOWN)
       ,mDeviceId(0u)
       ,registered(false)
       {
         mAvailability.setAvailability(am_t::am_Availability_e::A_UNKNOWN);
       }
   };

   AudioManagerInterface();
   ~AudioManagerInterface();

   uint32_t registerClient()
   {
      mRoutingControlObserver.addClient(++mNumClients);
      return mNumClients;
   }
   void deregisterClient(uint32_t ClientID)
   {
      mRoutingControlObserver.removeClient(ClientID);
   }

   // check if the proxy to the AM becomes available
   void RoutingControlObserverProxyavailibility(const CommonAPI::AvailabilityStatus& avail);

   // check if the proxy to the AudioPres source info becomes available
   void AudioSourceInfoProxyAvailibility(const CommonAPI::AvailabilityStatus& avail);

    void AudioPresProxyAvailibility(const CommonAPI::AvailabilityStatus& avail);

    void onMainConnectionStateChanged(const am_t::am_mainConnectionID_t & connectionID,const am_t::am_ConnectionState_e & connectionState);

    void onNewMainConnection(const am_t::am_MainConnectionType_s & mainConnection);
    void onMainSoundPropertyChanged( const am_t::am_sourceID_t & sourceID, const am_t::am_MainSoundProperty_s & sound_property );
   
   // check if the proxy to the CommandControl becomes available
   void CommandControlProxyAvailibility(const CommonAPI::AvailabilityStatus& avail);

   // callback for register domain call to AM
   void registerDomainAsyncCallback(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_domainID_t&, const ::v3::org::genivi::am_t::am_Error_e&);
   
   // callback for deregister domain call to AM
   void deRegisterDomainAsyncCallback(const CommonAPI::CallStatus&);
   
   // callback for register source call to AM
   void registerSourceCallback(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_sourceID_t&, const ::v3::org::genivi::am_t::am_Error_e&);
   
   // callback for deregister source call to AM
   void deRegisterSourceCallback(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&);
   
   // callback for routing ready status from AM
   void RoutingReadyAttributeCallback(const ::v3::org::genivi::am_t::am_RoutingReady_e&);
   
   // callback for domain registration complete status from AM
   void hookDomainRegistrationCompleteCallback(const CommonAPI::CallStatus&);
   
   // callback for source availability information update to AM
   void hookSourceAvailablityStatusChangeCallback(const am_t::am_sourceID_t& sourceId,bool availableStatus,const CommonAPI::CallStatus&);

   // initialiase the domain
   void initDomain();
   
   // initialise the list of supported sources from pres config
   void initSources();
   
   // register the list of supported sources with AM
   void registerSources();
   
   // deregister the list of supported sources with AM
   void deRegisterSources();

   // subscribe to the AM proxy available status
   void subscribetoProxyStatus(bool);

   // subscribe to the routing ready status
   void subscribeToRoutingReadyStatus(bool);

   void subscribeToAudioSourceInfoProxy(bool);
   void subscribeToCommandControlProxy(bool);

   bool updateAvailabilitytoAMIfAlreadyDetected(am_t::am_sourceID_e sourceID);

   bool updateAvailabilitytoAudioPresIfAlreadyDetected(am_t::am_sourceID_e sourceID);


   am_t::am_sourceID_e assignSourceIdToMediaDevice(const uint64_t& deviceId);
   am_t::am_sourceID_e assignSourceIdToMediaDevice(const uint64_t& deviceId,const MMPresTypes::eSlotType& slotType);

   //am_t::am_sourceID_e getSourceID(const uint64_t& deviceId);

   bool checkIfDeviceAlreadyAssignedToSource(const uint64_t& deviceId);
//using namespace v1::com::harman::audio::audioPresCtrl;
   uint32_t mNumClients;
   friend class FacadeHelper<AudioManagerInterface>;
   CommonAPIClientAbstraction<RoutingControlObserverProxy> mRoutingControlObserver;
   CommonAPIClientAbstraction<audioSourceInfoProxy> mAudioSoureInfo;
   CommonAPIClientAbstraction<CommandControlProxy> mCommandControl;

   CommonAPI::AvailabilityStatus mRoutingControlObserverAvailableStatus,mAudioSourceInfoAvailabilityStatus, mCommandControlAvailabilityStatus,mAudioPresInfoAvailabilityStatus;
   FacadeHelper<MediaPresSettings> mediaSettings;

   std::string domainName = DOMAIN_NAME;

   am_t::am_Domain_s mDomainData;
   std::string mBusName;
   am_t::am_domainID_t mDomainId;
   am_t::am_DomainState_e mDomainState = am_t::am_DomainState_e::DS_INDEPENDENT_STARTUP; 
   am_t::am_domainID_t domainId_ = 0;
   bool mDomainRegistratioComplete;
   bool mSourceRegistrationComplete;
   am_t::am_AvailabilityReason_e mBTAvailabilityReason;
   am_t::am_Error_e mBTDisconnectReason;
   SOURCELIST	mActiveSourceInfo;

   std::list<Source> mSourceList;
   static std::unordered_map<std::string, am_t::am_sourceID_e> sourceEnumMap;
   static std::unordered_map<am_t::am_sourceID_e,MMPresTypes::eSlotType> sourceToSlotMap;
   static std::unordered_map<am_t::am_sourceID_e,audioPresCtrlTypes::eEntertainmentSrcs> AMtoAudioPresSourceMap;
   static std::unordered_map<MMPresTypes::eDeviceType,audioPresCtrlTypes::eEntSourceType> DeviceTypetoAudioSourceType;

   std::function<void(const ::v3::org::genivi::am_t::am_Handle_s &_handle, const ::v3::org::genivi::am_t::am_Error_e &_amError, const std::shared_ptr<CommonAPI::ClientIdList> _receivers)> asyncSourceStateCallback;
   
   // This is used to read EOL data 
   unsigned char  ReadEOLData ( unsigned long sysSetID , unsigned long sysIDSize ) ;
           

};

#endif
