/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
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
#ifndef V3_ORG_GENIVI_AM_ROUTINGINTERFACE_Routing_Control_Observer_DBUS_STUB_ADAPTER_HPP_
#define V3_ORG_GENIVI_AM_ROUTINGINTERFACE_Routing_Control_Observer_DBUS_STUB_ADAPTER_HPP_

#include <v3/org/genivi/am/routinginterface/RoutingControlObserverStub.hpp>
#include "v3/org/genivi/am/routinginterface/RoutingControlObserverDBusDeployment.hpp"        
#include <v3/org/genivi/am_tDBusDeployment.hpp>        

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/DBus/DBusAddressTranslator.hpp>
#include <CommonAPI/DBus/DBusFactory.hpp>
#include <CommonAPI/DBus/DBusStubAdapterHelper.hpp>
#include <CommonAPI/DBus/DBusStubAdapter.hpp>
#include <CommonAPI/DBus/DBusDeployment.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v3 {
namespace org {
namespace genivi {
namespace am {
namespace routinginterface {

typedef CommonAPI::DBus::DBusStubAdapterHelper<RoutingControlObserverStub> RoutingControlObserverDBusStubAdapterHelper;

class RoutingControlObserverDBusStubAdapterInternal
    : public virtual RoutingControlObserverStubAdapter,
      public RoutingControlObserverDBusStubAdapterHelper
{
public:
    RoutingControlObserverDBusStubAdapterInternal(
            const CommonAPI::DBus::DBusAddress &_address,
            const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection,
            const std::shared_ptr<CommonAPI::StubBase> &_stub);

    ~RoutingControlObserverDBusStubAdapterInternal();

    virtual bool hasFreedesktopProperties();

    inline static const char* getInterface() {
        return RoutingControlObserver::getInterface();
    }

    /**
     * description: This attribute signals the current routing state to the clients. A client
     *   should notify himself to this attribute in order to know the current state and
     *   act accordingly.
     */
    void fireRoutingReadyAttributeChanged(const ::v3::org::genivi::am_t::am_RoutingReady_e& value);



    const RoutingControlObserverDBusStubAdapterHelper::StubDispatcherTable& getStubDispatcherTable();
    const CommonAPI::DBus::StubAttributeTable& getStubAttributeTable();

    void deactivateManagedInstances();
    

static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
        CommonAPI::Version
        > getRoutingControlObserverInterfaceVersionStubDispatcher;

/**
 * description: This attribute signals the current routing state to the clients. A client
 *   should notify himself to this attribute in order to know the current state and
 *   act accordingly.
 */
static CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
        ::v3::org::genivi::am_t::am_RoutingReady_e                
        > getRoutingReadyAttributeStubDispatcher;


/**
 * description: Registers a domain. (at)return E_OK on success, E_ALREADY_EXISTENT if already
 *   registered E_UNKNOWN on error
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_Domain_s, std::string, std::string>,
    std::tuple<::v3::org::genivi::am_t::am_domainID_t, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<::v3::org::genivi::am_t_::am_Domain_sDeployment_t, CommonAPI::DBus::StringDeployment, CommonAPI::DBus::StringDeployment>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > registerDomainStubDispatcher;
/**
 * description: Deregisters a domain. All sources, sinks, gateways and crossfaders from that
 *   domain will be removed as well. (at)return E_OK on success, E_NON_EXISTENT if
 *   not found E_UNKNOWN on error
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_domainID_t>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > deregisterDomainStubDispatcher;
/**
 * description: This function returns the ID to the given domain name. If already a domain is
 *   registered with this name, it will return the corresponding ID, if not it will
 *   reserve an ID but not register the domain. The other parameters of the domain
 *   will be overwritten when the domain is registered. (at)return E_OK on success,
 *   E_UNKNOWN on error     please use pulseAudio as the name to peek domainID. now
 *   all the sources and sinks are defined in domain pulseAudio.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<std::string>,
    std::tuple<::v3::org::genivi::am_t::am_domainID_t, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::DBus::StringDeployment>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > peekDomainStubDispatcher;
/**
 * description: Registers a sink. If the sink is part of a gateway, the list connection formats
 *   are copied to the gateway information (at)return E_OK on success,
 *   E_ALREADY_EXISTENT if already registered E_UNKNOWN on error
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_Sink_s>,
    std::tuple<::v3::org::genivi::am_t::am_sinkID_t, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<::v3::org::genivi::am_t_::am_Sink_sDeployment_t>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > registerSinkStubDispatcher;
/**
 * description: Deregisters a sink. (at)return E_OK on success, E_NON_EXISTENT if not found
 *   E_UNKNOWN on error
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_sinkID_t>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > deregisterSinkStubDispatcher;
/**
 * description: This function returns the ID to the given sink name. If already a sink is
 *   registered with this name, it will return the corresponding ID, if not it will
 *   reserve an ID but not register the sink. The other parameters of the sink will
 *   be overwritten when the sink is registered. (at)return E_OK on success,
 *   E_UNKNOWN on error
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<std::string>,
    std::tuple<::v3::org::genivi::am_t::am_sinkID_t, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::DBus::StringDeployment>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > peekSinkStubDispatcher;
/**
 * description: This function peeks a sink class ID. It is used by the RoutingPlugins to
 *   determine the sink class IDs of a sink class. (at)return E_OK on success,
 *   E_DATABASE_ERROR on error
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<std::string>,
    std::tuple<::v3::org::genivi::am_t::am_sinkClass_t, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::DBus::StringDeployment>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > peekSinkClassIDStubDispatcher;
/**
 * description: Updates data of a sink. (at)return E_OK on success, E_NON_EXISTENT if the sink
 *   ID is not valid.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_sinkID_t, ::v3::org::genivi::am_t::am_sinkClass_t, ::v3::org::genivi::am_t::am_SoundProperty_L, ::v3::org::genivi::am_t::am_ConnectionFormat_L, ::v3::org::genivi::am_t::am_MainSoundProperty_L>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment, ::v3::org::genivi::am_t_::am_SoundProperty_LDeployment_t, ::v3::org::genivi::am_t_::am_ConnectionFormat_LDeployment_t, ::v3::org::genivi::am_t_::am_MainSoundProperty_LDeployment_t>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > updateSinkStubDispatcher;
/**
 * description: Registers a source.  If the source is part of a gateway, the list connection
 *   formats are copied to the gateway information (at)return E_OK on success,
 *   E_UNKNOWN on error, E_ALREADY_EXIST if either name or source ID already exists
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_Source_s>,
    std::tuple<::v3::org::genivi::am_t::am_sourceID_t, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<::v3::org::genivi::am_t_::am_Source_sDeployment_t>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > registerSourceStubDispatcher;
/**
 * description: Deregisters a source. (at)return E_OK on success, E_NON_EXISTENT if not found
 *   E_UNKNOWN on error
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_sourceID_t>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > deregisterSourceStubDispatcher;
/**
 * description: This function returns the ID to the given source name. If already a source is
 *   registered with this name, it will return the corresponding ID, if not it will
 *   reserve an ID but not register the source. The other parameters of the source
 *   will be overwritten when the source is registered. (at)return E_OK on success,
 *   E_UNKNOWN on error
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<std::string>,
    std::tuple<::v3::org::genivi::am_t::am_sourceID_t, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::DBus::StringDeployment>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > peekSourceStubDispatcher;
/**
 * description: Peeks a source class ID. It is used by the RoutingPlugins to determine the
 *   source class IDs of a source class. (at)return E_OK on success,
 *   E_DATABASE_ERROR on error
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<std::string>,
    std::tuple<::v3::org::genivi::am_t::am_sourceClass_t, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::DBus::StringDeployment>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > peekSourceClassIDStubDispatcher;
/**
 * description: Updates data of a source. (at)return E_OK on success, E_NON_EXISTENT if the
 *   source ID in the struct is not valid.  Please note that only the following
 *   data out of am_Source_s have effect when they are changed: sourceClassID,
 *   listSoundProperties, listConnectionFormats, listMainSoundProperties
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_sourceID_t, ::v3::org::genivi::am_t::am_sourceClass_t, ::v3::org::genivi::am_t::am_SoundProperty_L, ::v3::org::genivi::am_t::am_ConnectionFormat_L, ::v3::org::genivi::am_t::am_MainSoundProperty_L>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment, ::v3::org::genivi::am_t_::am_SoundProperty_LDeployment_t, ::v3::org::genivi::am_t_::am_ConnectionFormat_LDeployment_t, ::v3::org::genivi::am_t_::am_MainSoundProperty_LDeployment_t>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > updateSourceStubDispatcher;
/**
 * description: Is called when a low level interrupt changes it status.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_sourceID_t, ::v3::org::genivi::am_t::am_InterruptState_e>,
    std::tuple<>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<>
    
    > hookInterruptStatusChangeStubDispatcher;
/**
 * description: This hook is called when all elements from a domain are registered. Is used by
 *   the Controller to know when all expected domains are finally registered.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_domainID_t>,
    std::tuple<>,
    std::tuple<CommonAPI::EmptyDeployment>,
    std::tuple<>
    
    > hookDomainRegistrationCompleteStubDispatcher;
/**
 * description: Is called when a sink changes its availability.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_sinkID_t, ::v3::org::genivi::am_t::am_Availability_s>,
    std::tuple<>,
    std::tuple<CommonAPI::EmptyDeployment, ::v3::org::genivi::am_t_::am_Availability_sDeployment_t>,
    std::tuple<>
    
    > hookSinkAvailablityStatusChangeStubDispatcher;
/**
 * description: Is called when a source changes its availability.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_sourceID_t, ::v3::org::genivi::am_t::am_Availability_s>,
    std::tuple<>,
    std::tuple<CommonAPI::EmptyDeployment, ::v3::org::genivi::am_t_::am_Availability_sDeployment_t>,
    std::tuple<>
    
    > hookSourceAvailablityStatusChangeStubDispatcher;
/**
 * description: Is called when a domain changes its status. This used for early domains only.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_domainID_t, ::v3::org::genivi::am_t::am_DomainState_e>,
    std::tuple<>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<>
    
    > hookDomainStateChangeStubDispatcher;
/**
 * description: This function is used to send out all data that has been changed in an early
 *   state.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::am_EarlyData_L>,
    std::tuple<>,
    std::tuple<::v3::org::genivi::am_t_::am_EarlyData_LDeployment_t>,
    std::tuple<>
    
    > sendChangedDataStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<uint32_t, uint32_t, CommonAPI::ByteBuffer>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > writeToDeviceStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<uint32_t, uint32_t>,
    std::tuple<CommonAPI::ByteBuffer, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > readFromDeviceStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<CommonAPI::ByteBuffer, uint32_t, CommonAPI::ByteBuffer, uint32_t>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > setFilterStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<CommonAPI::ByteBuffer, uint32_t>,
    std::tuple<CommonAPI::ByteBuffer, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > getFilterStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<CommonAPI::ByteBuffer, uint32_t, CommonAPI::ByteBuffer, uint32_t>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > setLimiterStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<CommonAPI::ByteBuffer, uint32_t>,
    std::tuple<CommonAPI::ByteBuffer, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > getLimiterStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<uint16_t, uint8_t>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > setVolumeStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<>,
    std::tuple<uint8_t, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > getVolumeStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<uint8_t, ::v3::org::genivi::am_t::am_MuteState_e>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > setChannelMuteStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<uint8_t, bool>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > setChannelPhaseStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<int8_t, int8_t, int8_t>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > setBMTStubDispatcher;

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<>,
    std::tuple<int8_t, int8_t, int8_t, ::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>
    
    > getBMTStubDispatcher;
/**
 * description: This function send parameters of limiter. If there is no error it will return
 *   0. If not it will return -1.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::hq_limiterStruct_param>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<::v3::org::genivi::am_t_::hq_limiterStruct_paramDeployment_t>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > sendLimiterStubDispatcher;
/**
 * description: This function send parameters of EQ filter. If there is no error it will return
 *   0. If not it will return -1.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::hq_eqFilterStruct_param>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<::v3::org::genivi::am_t_::hq_eqFilterStruct_paramDeployment_t>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > sendEQFilterStubDispatcher;
/**
 * description: This function send parameters of delay time. If there is no error it will
 *   return 0. If not it will return -1.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::hq_delayTimeStruct_param>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<::v3::org::genivi::am_t_::hq_delayTimeStruct_paramDeployment_t>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > sendDelayTimeStubDispatcher;
/**
 * description: This function send parameters of ClariFi. If there is no error it will return
 *   0. If not it will return -1.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::hq_ClariFiStruct_param>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<::v3::org::genivi::am_t_::hq_ClariFiStruct_paramDeployment_t>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > sendClariFiStubDispatcher;
/**
 * description: This function send parameters of loudness. If there is no error it will return
 *   0. If not it will return -1.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::hq_londnessStruct_param>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<::v3::org::genivi::am_t_::hq_londnessStruct_paramDeployment_t>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > sendLoudnessStubDispatcher;
/**
 * description: This function send parameters of channel gain. If there is no error it will
 *   return 0. If not it will return -1.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::hq_channelGainStruct_param>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<::v3::org::genivi::am_t_::hq_channelGainStruct_paramDeployment_t>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > sendChannelGainStubDispatcher;
/**
 * description: This function send parameters of maximum volume of source. If there is no error
 *   it will return 0. If not it will return -1.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::hq_sourceID_t, ::v3::org::genivi::am_t::hq_volume_t>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > sendVolumeMaxStubDispatcher;
/**
 * description: This function send parameters of minimum volume of source. If there is no error
 *   it will return 0. If not it will return -1.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::hq_sourceID_t, ::v3::org::genivi::am_t::hq_volume_t>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > sendVolumeMinStubDispatcher;
/**
 * description: This function send parameters of absolute volume of source. If there is no
 *   error it will return 0. If not it will return -1.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::hq_sourceID_t, ::v3::org::genivi::am_t::hq_volume_t>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > sendVolumeAbsStubDispatcher;
/**
 * description: This function send parameters of ratio volume of source. If there is no error
 *   it will return 0. If not it will return -1.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<::v3::org::genivi::am_t::hq_sourceID_t, ::v3::org::genivi::am_t::hq_volume_t>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<CommonAPI::EmptyDeployment, CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > sendVolumeRatioStubDispatcher;
/**
 * description: This function save tuning data into configuration file. If there is no error it
 *   will return 0. If not it will return -1.
 */

static CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v3::org::genivi::am::routinginterface::RoutingControlObserverStub,
    std::tuple<>,
    std::tuple<::v3::org::genivi::am_t::am_Error_e>,
    std::tuple<>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > saveDataToFileStubDispatcher;




 protected:
    virtual const char* getMethodsDBusIntrospectionXmlData() const;

 private:
    RoutingControlObserverDBusStubAdapterHelper::StubDispatcherTable stubDispatcherTable_;
    CommonAPI::DBus::StubAttributeTable stubAttributeTable_;
};

class RoutingControlObserverDBusStubAdapter
    : public RoutingControlObserverDBusStubAdapterInternal,
      public std::enable_shared_from_this<RoutingControlObserverDBusStubAdapter> {
public:
    RoutingControlObserverDBusStubAdapter(
    	const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection,
        const std::shared_ptr<CommonAPI::StubBase> &_stub)
    	: CommonAPI::DBus::DBusStubAdapter(
    		_address, 
    		_connection,
            false),
          RoutingControlObserverDBusStubAdapterInternal(
          	_address, 
          	_connection, 
          	_stub) { 
    }
};

} // namespace routinginterface
} // namespace am
} // namespace genivi
} // namespace org
} // namespace v3

#endif // V3_ORG_GENIVI_AM_ROUTINGINTERFACE_Routing_Control_Observer_DBUS_STUB_ADAPTER_HPP_
