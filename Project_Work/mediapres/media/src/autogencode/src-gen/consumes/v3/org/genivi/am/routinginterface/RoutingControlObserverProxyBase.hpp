/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
/**
 * description: Routing Receive sendInterface description.
 * author: lian zhang
 */
#ifndef V3_ORG_GENIVI_AM_ROUTINGINTERFACE_Routing_Control_Observer_PROXY_BASE_HPP_
#define V3_ORG_GENIVI_AM_ROUTINGINTERFACE_Routing_Control_Observer_PROXY_BASE_HPP_

#include <v3/org/genivi/am/routinginterface/RoutingControlObserver.hpp>


#include <v3/org/genivi/am_t.hpp>

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/Deployment.hpp>
#include <CommonAPI/InputStream.hpp>
#include <CommonAPI/OutputStream.hpp>
#include <CommonAPI/Struct.hpp>
#include <cstdint>
#include <string>
#include <vector>

#include <CommonAPI/Attribute.hpp>
#include <CommonAPI/Proxy.hpp>
#include <functional>
#include <future>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v3 {
namespace org {
namespace genivi {
namespace am {
namespace routinginterface {

class RoutingControlObserverProxyBase
    : virtual public CommonAPI::Proxy {
public:
    typedef CommonAPI::ObservableReadonlyAttribute<::v3::org::genivi::am_t::am_RoutingReady_e> RoutingReadyAttribute;

    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_domainID_t&, const ::v3::org::genivi::am_t::am_Error_e&)> RegisterDomainAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> DeregisterDomainAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_domainID_t&, const ::v3::org::genivi::am_t::am_Error_e&)> PeekDomainAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_sinkID_t&, const ::v3::org::genivi::am_t::am_Error_e&)> RegisterSinkAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> DeregisterSinkAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_sinkID_t&, const ::v3::org::genivi::am_t::am_Error_e&)> PeekSinkAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_sinkClass_t&, const ::v3::org::genivi::am_t::am_Error_e&)> PeekSinkClassIDAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> UpdateSinkAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_sourceID_t&, const ::v3::org::genivi::am_t::am_Error_e&)> RegisterSourceAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> DeregisterSourceAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_sourceID_t&, const ::v3::org::genivi::am_t::am_Error_e&)> PeekSourceAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_sourceClass_t&, const ::v3::org::genivi::am_t::am_Error_e&)> PeekSourceClassIDAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> UpdateSourceAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&)> HookInterruptStatusChangeAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&)> HookDomainRegistrationCompleteAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&)> HookSinkAvailablityStatusChangeAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&)> HookSourceAvailablityStatusChangeAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&)> HookDomainStateChangeAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&)> SendChangedDataAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> WriteToDeviceAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const CommonAPI::ByteBuffer&, const ::v3::org::genivi::am_t::am_Error_e&)> ReadFromDeviceAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SetFilterAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const CommonAPI::ByteBuffer&, const ::v3::org::genivi::am_t::am_Error_e&)> GetFilterAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SetLimiterAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const CommonAPI::ByteBuffer&, const ::v3::org::genivi::am_t::am_Error_e&)> GetLimiterAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SetVolumeAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const uint8_t&, const ::v3::org::genivi::am_t::am_Error_e&)> GetVolumeAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SetChannelMuteAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SetChannelPhaseAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SetBMTAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const int8_t&, const int8_t&, const int8_t&, const ::v3::org::genivi::am_t::am_Error_e&)> GetBMTAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SendLimiterAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SendEQFilterAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SendDelayTimeAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SendClariFiAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SendLoudnessAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SendChannelGainAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SendVolumeMaxAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SendVolumeMinAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SendVolumeAbsAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SendVolumeRatioAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v3::org::genivi::am_t::am_Error_e&)> SaveDataToFileAsyncCallback;

    /**
     * description: This attribute signals the current routing state to the clients. A client
     *   should notify himself to this attribute in order to know the current state and
     *   act accordingly.
     */
    virtual RoutingReadyAttribute& getRoutingReadyAttribute() = 0;


    /**
     * description: Registers a domain. (at)return E_OK on success, E_ALREADY_EXISTENT if already
     *   registered E_UNKNOWN on error
     */
    virtual void registerDomain(const ::v3::org::genivi::am_t::am_Domain_s &_domainData, const std::string &_returnBusname, const std::string &_returnInterface, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_domainID_t &_domainID, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> registerDomainAsync(const ::v3::org::genivi::am_t::am_Domain_s &_domainData, const std::string &_returnBusname, const std::string &_returnInterface, RegisterDomainAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Deregisters a domain. All sources, sinks, gateways and crossfaders from that
     *   domain will be removed as well. (at)return E_OK on success, E_NON_EXISTENT if
     *   not found E_UNKNOWN on error
     */
    virtual void deregisterDomain(const ::v3::org::genivi::am_t::am_domainID_t &_domainID, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> deregisterDomainAsync(const ::v3::org::genivi::am_t::am_domainID_t &_domainID, DeregisterDomainAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function returns the ID to the given domain name. If already a domain is
     *   registered with this name, it will return the corresponding ID, if not it will
     *   reserve an ID but not register the domain. The other parameters of the domain
     *   will be overwritten when the domain is registered. (at)return E_OK on success,
     *   E_UNKNOWN on error     please use pulseAudio as the name to peek domainID. now
     *   all the sources and sinks are defined in domain pulseAudio.
     */
    virtual void peekDomain(const std::string &_name, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_domainID_t &_domainID, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> peekDomainAsync(const std::string &_name, PeekDomainAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Registers a sink. If the sink is part of a gateway, the list connection formats
     *   are copied to the gateway information (at)return E_OK on success,
     *   E_ALREADY_EXISTENT if already registered E_UNKNOWN on error
     */
    virtual void registerSink(const ::v3::org::genivi::am_t::am_Sink_s &_sinkData, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> registerSinkAsync(const ::v3::org::genivi::am_t::am_Sink_s &_sinkData, RegisterSinkAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Deregisters a sink. (at)return E_OK on success, E_NON_EXISTENT if not found
     *   E_UNKNOWN on error
     */
    virtual void deregisterSink(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> deregisterSinkAsync(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, DeregisterSinkAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function returns the ID to the given sink name. If already a sink is
     *   registered with this name, it will return the corresponding ID, if not it will
     *   reserve an ID but not register the sink. The other parameters of the sink will
     *   be overwritten when the sink is registered. (at)return E_OK on success,
     *   E_UNKNOWN on error
     */
    virtual void peekSink(const std::string &_name, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> peekSinkAsync(const std::string &_name, PeekSinkAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function peeks a sink class ID. It is used by the RoutingPlugins to
     *   determine the sink class IDs of a sink class. (at)return E_OK on success,
     *   E_DATABASE_ERROR on error
     */
    virtual void peekSinkClassID(const std::string &_name, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_sinkClass_t &_sinkClassID, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> peekSinkClassIDAsync(const std::string &_name, PeekSinkClassIDAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Updates data of a sink. (at)return E_OK on success, E_NON_EXISTENT if the sink
     *   ID is not valid.
     */
    virtual void updateSink(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_sinkClass_t &_sinkClassID, const ::v3::org::genivi::am_t::am_SoundProperty_L &_listSoundProperties, const ::v3::org::genivi::am_t::am_ConnectionFormat_L &_listConnectionFormats, const ::v3::org::genivi::am_t::am_MainSoundProperty_L &_listMainSoundProperties, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> updateSinkAsync(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_sinkClass_t &_sinkClassID, const ::v3::org::genivi::am_t::am_SoundProperty_L &_listSoundProperties, const ::v3::org::genivi::am_t::am_ConnectionFormat_L &_listConnectionFormats, const ::v3::org::genivi::am_t::am_MainSoundProperty_L &_listMainSoundProperties, UpdateSinkAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Registers a source.  If the source is part of a gateway, the list connection
     *   formats are copied to the gateway information (at)return E_OK on success,
     *   E_UNKNOWN on error, E_ALREADY_EXIST if either name or source ID already exists
     */
    virtual void registerSource(const ::v3::org::genivi::am_t::am_Source_s &_sourceData, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> registerSourceAsync(const ::v3::org::genivi::am_t::am_Source_s &_sourceData, RegisterSourceAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Deregisters a source. (at)return E_OK on success, E_NON_EXISTENT if not found
     *   E_UNKNOWN on error
     */
    virtual void deregisterSource(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> deregisterSourceAsync(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, DeregisterSourceAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function returns the ID to the given source name. If already a source is
     *   registered with this name, it will return the corresponding ID, if not it will
     *   reserve an ID but not register the source. The other parameters of the source
     *   will be overwritten when the source is registered. (at)return E_OK on success,
     *   E_UNKNOWN on error
     */
    virtual void peekSource(const std::string &_name, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> peekSourceAsync(const std::string &_name, PeekSourceAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Peeks a source class ID. It is used by the RoutingPlugins to determine the
     *   source class IDs of a source class. (at)return E_OK on success,
     *   E_DATABASE_ERROR on error
     */
    virtual void peekSourceClassID(const std::string &_name, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_sourceClass_t &_sourceClassID, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> peekSourceClassIDAsync(const std::string &_name, PeekSourceClassIDAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Updates data of a source. (at)return E_OK on success, E_NON_EXISTENT if the
     *   source ID in the struct is not valid.  Please note that only the following
     *   data out of am_Source_s have effect when they are changed: sourceClassID,
     *   listSoundProperties, listConnectionFormats, listMainSoundProperties
     */
    virtual void updateSource(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_sourceClass_t &_sourceClassID, const ::v3::org::genivi::am_t::am_SoundProperty_L &_listSoundProperties, const ::v3::org::genivi::am_t::am_ConnectionFormat_L &_listConnectionFormats, const ::v3::org::genivi::am_t::am_MainSoundProperty_L &_listMainSoundProperties, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> updateSourceAsync(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_sourceClass_t &_sourceClassID, const ::v3::org::genivi::am_t::am_SoundProperty_L &_listSoundProperties, const ::v3::org::genivi::am_t::am_ConnectionFormat_L &_listConnectionFormats, const ::v3::org::genivi::am_t::am_MainSoundProperty_L &_listMainSoundProperties, UpdateSourceAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Is called when a low level interrupt changes it status.
     */
    virtual void hookInterruptStatusChange(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_InterruptState_e &_interruptState, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> hookInterruptStatusChangeAsync(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_InterruptState_e &_interruptState, HookInterruptStatusChangeAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This hook is called when all elements from a domain are registered. Is used by
     *   the Controller to know when all expected domains are finally registered.
     */
    virtual void hookDomainRegistrationComplete(const ::v3::org::genivi::am_t::am_domainID_t &_domainID, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> hookDomainRegistrationCompleteAsync(const ::v3::org::genivi::am_t::am_domainID_t &_domainID, HookDomainRegistrationCompleteAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Is called when a sink changes its availability.
     */
    virtual void hookSinkAvailablityStatusChange(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_Availability_s &_availability, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> hookSinkAvailablityStatusChangeAsync(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_Availability_s &_availability, HookSinkAvailablityStatusChangeAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Is called when a source changes its availability.
     */
    virtual void hookSourceAvailablityStatusChange(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_Availability_s &_availability, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> hookSourceAvailablityStatusChangeAsync(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_Availability_s &_availability, HookSourceAvailablityStatusChangeAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Is called when a domain changes its status. This used for early domains only.
     */
    virtual void hookDomainStateChange(const ::v3::org::genivi::am_t::am_domainID_t &_domainID, const ::v3::org::genivi::am_t::am_DomainState_e &_domainState, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> hookDomainStateChangeAsync(const ::v3::org::genivi::am_t::am_domainID_t &_domainID, const ::v3::org::genivi::am_t::am_DomainState_e &_domainState, HookDomainStateChangeAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function is used to send out all data that has been changed in an early
     *   state.
     */
    virtual void sendChangedData(const ::v3::org::genivi::am_t::am_EarlyData_L &_earlyData, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> sendChangedDataAsync(const ::v3::org::genivi::am_t::am_EarlyData_L &_earlyData, SendChangedDataAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void writeToDevice(const uint32_t &_devAddr, const uint32_t &_dataSize, const CommonAPI::ByteBuffer &_data, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> writeToDeviceAsync(const uint32_t &_devAddr, const uint32_t &_dataSize, const CommonAPI::ByteBuffer &_data, WriteToDeviceAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void readFromDevice(const uint32_t &_devAddr, const uint32_t &_dataSize, CommonAPI::CallStatus &_internalCallStatus, CommonAPI::ByteBuffer &_data, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> readFromDeviceAsync(const uint32_t &_devAddr, const uint32_t &_dataSize, ReadFromDeviceAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void setFilter(const CommonAPI::ByteBuffer &_paramData, const uint32_t &_paramSize, const CommonAPI::ByteBuffer &_data, const uint32_t &_dataSize, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> setFilterAsync(const CommonAPI::ByteBuffer &_paramData, const uint32_t &_paramSize, const CommonAPI::ByteBuffer &_data, const uint32_t &_dataSize, SetFilterAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void getFilter(const CommonAPI::ByteBuffer &_paramData, const uint32_t &_paramSize, CommonAPI::CallStatus &_internalCallStatus, CommonAPI::ByteBuffer &_data, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> getFilterAsync(const CommonAPI::ByteBuffer &_paramData, const uint32_t &_paramSize, GetFilterAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void setLimiter(const CommonAPI::ByteBuffer &_paramData, const uint32_t &_paramSize, const CommonAPI::ByteBuffer &_data, const uint32_t &_dataSize, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> setLimiterAsync(const CommonAPI::ByteBuffer &_paramData, const uint32_t &_paramSize, const CommonAPI::ByteBuffer &_data, const uint32_t &_dataSize, SetLimiterAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void getLimiter(const CommonAPI::ByteBuffer &_paramData, const uint32_t &_paramSize, CommonAPI::CallStatus &_internalCallStatus, CommonAPI::ByteBuffer &_data, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> getLimiterAsync(const CommonAPI::ByteBuffer &_paramData, const uint32_t &_paramSize, GetLimiterAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void setVolume(const uint16_t &_sinkID, const uint8_t &_volume, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> setVolumeAsync(const uint16_t &_sinkID, const uint8_t &_volume, SetVolumeAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void getVolume(CommonAPI::CallStatus &_internalCallStatus, uint8_t &_volume, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> getVolumeAsync(GetVolumeAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void setChannelMute(const uint8_t &_channelIndex, const ::v3::org::genivi::am_t::am_MuteState_e &_muteState, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> setChannelMuteAsync(const uint8_t &_channelIndex, const ::v3::org::genivi::am_t::am_MuteState_e &_muteState, SetChannelMuteAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void setChannelPhase(const uint8_t &_channelIndex, const bool &_phase, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> setChannelPhaseAsync(const uint8_t &_channelIndex, const bool &_phase, SetChannelPhaseAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void setBMT(const int8_t &_bass, const int8_t &_mid, const int8_t &_treble, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> setBMTAsync(const int8_t &_bass, const int8_t &_mid, const int8_t &_treble, SetBMTAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual void getBMT(CommonAPI::CallStatus &_internalCallStatus, int8_t &_bass, int8_t &_mid, int8_t &_treble, ::v3::org::genivi::am_t::am_Error_e &_eqToolError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> getBMTAsync(GetBMTAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function send parameters of limiter. If there is no error it will return
     *   0. If not it will return -1.
     */
    virtual void sendLimiter(const ::v3::org::genivi::am_t::hq_limiterStruct_param &_limiter, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_hqError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> sendLimiterAsync(const ::v3::org::genivi::am_t::hq_limiterStruct_param &_limiter, SendLimiterAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function send parameters of EQ filter. If there is no error it will return
     *   0. If not it will return -1.
     */
    virtual void sendEQFilter(const ::v3::org::genivi::am_t::hq_eqFilterStruct_param &_bsFilter, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_qhError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> sendEQFilterAsync(const ::v3::org::genivi::am_t::hq_eqFilterStruct_param &_bsFilter, SendEQFilterAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function send parameters of delay time. If there is no error it will
     *   return 0. If not it will return -1.
     */
    virtual void sendDelayTime(const ::v3::org::genivi::am_t::hq_delayTimeStruct_param &_delayTime, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_qhError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> sendDelayTimeAsync(const ::v3::org::genivi::am_t::hq_delayTimeStruct_param &_delayTime, SendDelayTimeAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function send parameters of ClariFi. If there is no error it will return
     *   0. If not it will return -1.
     */
    virtual void sendClariFi(const ::v3::org::genivi::am_t::hq_ClariFiStruct_param &_bs, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_qhError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> sendClariFiAsync(const ::v3::org::genivi::am_t::hq_ClariFiStruct_param &_bs, SendClariFiAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function send parameters of loudness. If there is no error it will return
     *   0. If not it will return -1.
     */
    virtual void sendLoudness(const ::v3::org::genivi::am_t::hq_londnessStruct_param &_bs, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_qhError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> sendLoudnessAsync(const ::v3::org::genivi::am_t::hq_londnessStruct_param &_bs, SendLoudnessAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function send parameters of channel gain. If there is no error it will
     *   return 0. If not it will return -1.
     */
    virtual void sendChannelGain(const ::v3::org::genivi::am_t::hq_channelGainStruct_param &_bs, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_qhError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> sendChannelGainAsync(const ::v3::org::genivi::am_t::hq_channelGainStruct_param &_bs, SendChannelGainAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function send parameters of maximum volume of source. If there is no error
     *   it will return 0. If not it will return -1.
     */
    virtual void sendVolumeMax(const ::v3::org::genivi::am_t::hq_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::hq_volume_t &_volume, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_qhError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> sendVolumeMaxAsync(const ::v3::org::genivi::am_t::hq_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::hq_volume_t &_volume, SendVolumeMaxAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function send parameters of minimum volume of source. If there is no error
     *   it will return 0. If not it will return -1.
     */
    virtual void sendVolumeMin(const ::v3::org::genivi::am_t::hq_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::hq_volume_t &_volume, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_qhError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> sendVolumeMinAsync(const ::v3::org::genivi::am_t::hq_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::hq_volume_t &_volume, SendVolumeMinAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function send parameters of absolute volume of source. If there is no
     *   error it will return 0. If not it will return -1.
     */
    virtual void sendVolumeAbs(const ::v3::org::genivi::am_t::hq_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::hq_volume_t &_volume, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_qhError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> sendVolumeAbsAsync(const ::v3::org::genivi::am_t::hq_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::hq_volume_t &_volume, SendVolumeAbsAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function send parameters of ratio volume of source. If there is no error
     *   it will return 0. If not it will return -1.
     */
    virtual void sendVolumeRatio(const ::v3::org::genivi::am_t::hq_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::hq_volume_t &_volume, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_qhError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> sendVolumeRatioAsync(const ::v3::org::genivi::am_t::hq_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::hq_volume_t &_volume, SendVolumeRatioAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: This function save tuning data into configuration file. If there is no error it
     *   will return 0. If not it will return -1.
     */
    virtual void saveDataToFile(CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_qhError, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> saveDataToFileAsync(SaveDataToFileAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
};

} // namespace routinginterface
} // namespace am
} // namespace genivi
} // namespace org
} // namespace v3


// Compatibility
namespace v3_7 = v3;

#endif // V3_ORG_GENIVI_AM_ROUTINGINTERFACE_Routing_Control_Observer_PROXY_BASE_HPP_
