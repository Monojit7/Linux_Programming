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
 * description: The interface towards the Controlling Instance (e.g. HMI). It handles the
 *   communication towards the HMI and other system components who need to interact
 *   with the audio management. There are two rules that have to be kept in mind
 *   when implementing against this interface:
 * author: Christian Linke
 */
#ifndef V3_ORG_GENIVI_AM_COMMANDINTERFACE_Command_Control_DBUS_PROXY_HPP_
#define V3_ORG_GENIVI_AM_COMMANDINTERFACE_Command_Control_DBUS_PROXY_HPP_

#include <v3/org/genivi/am/commandinterface/CommandControlProxyBase.hpp>
#include "v3/org/genivi/am/commandinterface/CommandControlDBusDeployment.hpp"

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/DBus/DBusAddress.hpp>
#include <CommonAPI/DBus/DBusFactory.hpp>
#include <CommonAPI/DBus/DBusProxy.hpp>
#include <CommonAPI/DBus/DBusAddressTranslator.hpp>
#include <CommonAPI/DBus/DBusAttribute.hpp>
#include <CommonAPI/DBus/DBusEvent.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

#include <string>

namespace v3 {
namespace org {
namespace genivi {
namespace am {
namespace commandinterface {

class CommandControlDBusProxy
    : virtual public CommandControlProxyBase, 
      virtual public CommonAPI::DBus::DBusProxy {
public:
    CommandControlDBusProxy(
        const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection);

    virtual ~CommandControlDBusProxy() { }

    virtual ListOfMainConnetionsAttribute& getListOfMainConnetionsAttribute();

    virtual NewMainConnectionEvent& getNewMainConnectionEvent();
    virtual RemovedMainConnectionEvent& getRemovedMainConnectionEvent();
    virtual NewSinkEvent& getNewSinkEvent();
    virtual RemovedSinkEvent& getRemovedSinkEvent();
    virtual NewSourceEvent& getNewSourceEvent();
    virtual RemovedSourceEvent& getRemovedSourceEvent();
    virtual NumberOfSinkClassesChangedEvent& getNumberOfSinkClassesChangedEvent();
    virtual NumberOfSourceClassesChangedEvent& getNumberOfSourceClassesChangedEvent();
    virtual MainConnectionStateChangedEvent& getMainConnectionStateChangedEvent();
    virtual MainSinkSoundPropertyChangedEvent& getMainSinkSoundPropertyChangedEvent();
    virtual MainSourceSoundPropertyChangedEvent& getMainSourceSoundPropertyChangedEvent();
    virtual SinkAvailabilityChangedEvent& getSinkAvailabilityChangedEvent();
    virtual SourceAvailabilityChangedEvent& getSourceAvailabilityChangedEvent();
    virtual VolumeChangedEvent& getVolumeChangedEvent();
    virtual SinkMuteStateChangedEvent& getSinkMuteStateChangedEvent();
    virtual SystemPropertyChangedEvent& getSystemPropertyChangedEvent();
    virtual TimingInformationChangedEvent& getTimingInformationChangedEvent();
    virtual SinkUpdatedEvent& getSinkUpdatedEvent();
    virtual SourceUpdatedEvent& getSourceUpdatedEvent();
    virtual SinkNotificationEvent& getSinkNotificationEvent();
    virtual SourceNotificationEvent& getSourceNotificationEvent();
    virtual MainSinkNotificationConfigurationChangedEvent& getMainSinkNotificationConfigurationChangedEvent();
    virtual MainSourceNotificationConfigurationChangedEvent& getMainSourceNotificationConfigurationChangedEvent();

    /**
     * description: Connects a source to sink. (at)return E_OK on success, E_NOT_POSSIBLE on
     *   failure, E_ALREADY_EXISTS if the connection does already exists
     */
    virtual void connect(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_mainConnectionID_t &_mainConnectionID, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> connectAsync(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, ConnectAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Disconnects a main connection. (at)return E_OK on successes, E_NON_EXISTENT if
     *   the connection does not exist, E_NOT_POSSIBLE on error.
     */
    virtual void disconnect(const ::v3::org::genivi::am_t::am_mainConnectionID_t &_mainConnectionID, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> disconnectAsync(const ::v3::org::genivi::am_t::am_mainConnectionID_t &_mainConnectionID, DisconnectAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Sets the volume for a sink. (at)return E_OK on success, E_UNKNOWN on error,
     *   E_OUT_OF_RANGE in case the value is out of range
     */
    virtual void setVolume(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_mainVolume_t &_volume, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> setVolumeAsync(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_mainVolume_t &_volume, SetVolumeAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: This function is used to increment or decrement the current volume for a sink.
     *   (at)return E_OK on success, E_UNKNOWN on error and E_OUT_OF_RANGE if the value
     *   is not in the given volume range.
     */
    virtual void volumeStep(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const int16_t &_step, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> volumeStepAsync(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const int16_t &_step, VolumeStepAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Sets the mute state of a sink. (at)return E_OK on success, E_UNKNOWN on error.
     *   If the mute state is already the desired one, the Daemon will return E_OK
     */
    virtual void setSinkMuteState(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_MuteState_e &_muteState, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> setSinkMuteStateAsync(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_MuteState_e &_muteState, SetSinkMuteStateAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: This method is used to set sound properties, e.g. equalizer values. Since the
     *   capabilities of the system can differ, the exact key value pairs can be
     *   extended in each product. (at)return E_OK on success, E_OUT_OF_RANGE if value
     *   exceeds range, E_UNKNOWN in case of an error
     */
    virtual void setMainSinkSoundProperty(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_MainSoundProperty_s &_soundProperty, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> setMainSinkSoundPropertyAsync(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_MainSoundProperty_s &_soundProperty, SetMainSinkSoundPropertyAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: This method is used to set sound properties, e.g. equalizer values. Since the
     *   capabilities of the system can differ, the exact key value pairs can be
     *   extended in each product. (at)return E_OK on success, E_OUT_OF_RANGE if value
     *   exceeds range, E_UNKNOWN in case of an error
     */
    virtual void setMainSourceSoundProperty(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_MainSoundProperty_s &_soundProperty, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> setMainSourceSoundPropertyAsync(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_MainSoundProperty_s &_soundProperty, SetMainSourceSoundPropertyAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Sets a specific system property. (at)return E_OK on success, E_OUT_OF_RANGE if
     *   value exceeds range, E_UNKNOWN in case of an error
     */
    virtual void setSystemProperty(const ::v3::org::genivi::am_t::am_SystemProperty_s &_property, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> setSystemPropertyAsync(const ::v3::org::genivi::am_t::am_SystemProperty_s &_property, SetSystemPropertyAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Returns the current list of main connections. (at)return E_OK on success,
     *   E_DATABASE_ERROR on error
     */
    virtual void getListMainConnections(CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_MainConnection_L &_listConnections, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getListMainConnectionsAsync(GetListMainConnectionsAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Returns the current list of sinks. (at)return E_OK on success, E_DATABASE_ERROR
     *   on error
     */
    virtual void getListMainSinks(CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_SinkType_L &_listMainSinks, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getListMainSinksAsync(GetListMainSinksAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Returns the current list of sources. (at)return E_OK on success,
     *   E_DATABASE_ERROR on error
     */
    virtual void getListMainSources(CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_SourceType_L &_listMainSources, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getListMainSourcesAsync(GetListMainSourcesAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: This is used to retrieve all sink sound properties related to a sink. Returns a
     *   vector of the sound properties and values as pair. (at)return E_OK on success,
     *   E_DATABASE_ERROR on error
     */
    virtual void getListMainSinkSoundProperties(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_MainSoundProperty_L &_listSoundProperties, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getListMainSinkSoundPropertiesAsync(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, GetListMainSinkSoundPropertiesAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: This is used to retrieve all source sound properties related to a source.
     *   Returns a vector of the sound properties and values as pair. (at)return E_OK
     *   on success, E_DATABASE_ERROR on error
     */
    virtual void getListMainSourceSoundProperties(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_MainSoundProperty_L &_listSoundProperties, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getListMainSourceSoundPropertiesAsync(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, GetListMainSourceSoundPropertiesAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: This is used to retrieve sink class information of all sink classes. (at)return
     *   E_OK on success, E_DATABASE_ERROR on error
     */
    virtual void getListSinkClasses(CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_SinkClass_L &_listSinkClasses, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getListSinkClassesAsync(GetListSinkClassesAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: This is used to retrieve source class information of all source classes.
     *   (at)return E_OK on success, E_DATABASE_ERROR on error
     */
    virtual void getListSourceClasses(CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_SourceClass_L &_listSourceClasses, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getListSourceClassesAsync(GetListSourceClassesAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Retrieves a complete list of all system properties. (at)return E_OK on success,
     *   E_DATABASE_ERROR on error
     */
    virtual void getListSystemProperties(CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_SystemProperty_L &_listSystemProperties, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getListSystemPropertiesAsync(GetListSystemPropertiesAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Returns the delay in ms that the audio path for the given main connection.
     *   (at)return E_OK on success, E_NOT_POSSIBLE if timing information is not yet
     *   retrieved, E_DATABASE_ERROR on read error on the database
     */
    virtual void getTimingInformation(const ::v3::org::genivi::am_t::am_mainConnectionID_t &_mainConnectionID, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_timeSync_t &_delay, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getTimingInformationAsync(const ::v3::org::genivi::am_t::am_mainConnectionID_t &_mainConnectionID, GetTimingInformationAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Retrieves the list of main notifications for a sink. Does not return the
     *   possible ones.
     */
    virtual void getListMainSinkNotificationConfigurations(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_NotificationConfiguration_L &_listMainNotificationConfigurations, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getListMainSinkNotificationConfigurationsAsync(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, GetListMainSinkNotificationConfigurationsAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Retrieves the list of main notifications for a source. Does not return the
     *   possible ones.
     */
    virtual void getListMainSourceNotificationConfigurations(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_NotificationConfiguration_L &_listMainNotificationConfigurations, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> getListMainSourceNotificationConfigurationsAsync(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, GetListMainSourceNotificationConfigurationsAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Sets a MainNotificationConfiuration. This can be used to turn on an off
     *   notifications an to change the mode of the configuration. (at)return E_OK on
     *   success, E_NON_EXISTENT if sink ID does not exists, E_DATABASE_ERROR on error
     */
    virtual void setMainSinkNotificationConfiguration(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_NotificationConfiguration_s &_mainNotificationConfiguration, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> setMainSinkNotificationConfigurationAsync(const ::v3::org::genivi::am_t::am_sinkID_t &_sinkID, const ::v3::org::genivi::am_t::am_NotificationConfiguration_s &_mainNotificationConfiguration, SetMainSinkNotificationConfigurationAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Sets a MainNotificationConfiuration. This can be used to turn on an off
     *   notifications an to change the mode of the configuration. (at)return E_OK on
     *   success, E_NON_EXISTENT if source ID does not exists, E_DATABASE_ERROR on error
     */
    virtual void setMainSourceNotificationConfiguration(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_NotificationConfiguration_s &_mainNotificationConfiguration, CommonAPI::CallStatus &_internalCallStatus, ::v3::org::genivi::am_t::am_Error_e &_amError, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> setMainSourceNotificationConfigurationAsync(const ::v3::org::genivi::am_t::am_sourceID_t &_sourceID, const ::v3::org::genivi::am_t::am_NotificationConfiguration_s &_mainNotificationConfiguration, SetMainSourceNotificationConfigurationAsyncCallback _callback, const CommonAPI::CallInfo *_info);


    virtual void getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const;

private:
   
    CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<ListOfMainConnetionsAttribute, ::v3::org::genivi::am_t_::am_MainConnection_LDeployment_t>> listOfMainConnetions_;

    CommonAPI::DBus::DBusEvent<NewMainConnectionEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_MainConnectionType_s, ::v3::org::genivi::am_t_::am_MainConnectionType_sDeployment_t>> newMainConnection_;
    CommonAPI::DBus::DBusEvent<RemovedMainConnectionEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_mainConnectionID_t, CommonAPI::EmptyDeployment>> removedMainConnection_;
    CommonAPI::DBus::DBusEvent<NewSinkEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_SinkType_s, ::v3::org::genivi::am_t_::am_SinkType_sDeployment_t>> newSink_;
    CommonAPI::DBus::DBusEvent<RemovedSinkEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sinkID_t, CommonAPI::EmptyDeployment>> removedSink_;
    CommonAPI::DBus::DBusEvent<NewSourceEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_SourceType_s, ::v3::org::genivi::am_t_::am_SourceType_sDeployment_t>> newSource_;
    CommonAPI::DBus::DBusEvent<RemovedSourceEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sourceID_t, CommonAPI::EmptyDeployment>> removedSource_;
    CommonAPI::DBus::DBusEvent<NumberOfSinkClassesChangedEvent> numberOfSinkClassesChanged_;
    CommonAPI::DBus::DBusEvent<NumberOfSourceClassesChangedEvent> numberOfSourceClassesChanged_;
    CommonAPI::DBus::DBusEvent<MainConnectionStateChangedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_mainConnectionID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_ConnectionState_e, CommonAPI::EmptyDeployment>> mainConnectionStateChanged_;
    CommonAPI::DBus::DBusEvent<MainSinkSoundPropertyChangedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sinkID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_MainSoundProperty_s, ::v3::org::genivi::am_t_::am_MainSoundProperty_sDeployment_t>> mainSinkSoundPropertyChanged_;
    CommonAPI::DBus::DBusEvent<MainSourceSoundPropertyChangedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sourceID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_MainSoundProperty_s, ::v3::org::genivi::am_t_::am_MainSoundProperty_sDeployment_t>> mainSourceSoundPropertyChanged_;
    CommonAPI::DBus::DBusEvent<SinkAvailabilityChangedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sinkID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_Availability_s, ::v3::org::genivi::am_t_::am_Availability_sDeployment_t>> sinkAvailabilityChanged_;
    CommonAPI::DBus::DBusEvent<SourceAvailabilityChangedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sourceID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_Availability_s, ::v3::org::genivi::am_t_::am_Availability_sDeployment_t>> sourceAvailabilityChanged_;
    CommonAPI::DBus::DBusEvent<VolumeChangedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sinkID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_mainVolume_t, CommonAPI::EmptyDeployment>> volumeChanged_;
    CommonAPI::DBus::DBusEvent<SinkMuteStateChangedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sinkID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_MuteState_e, CommonAPI::EmptyDeployment>> sinkMuteStateChanged_;
    CommonAPI::DBus::DBusEvent<SystemPropertyChangedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_SystemProperty_s, ::v3::org::genivi::am_t_::am_SystemProperty_sDeployment_t>> systemPropertyChanged_;
    CommonAPI::DBus::DBusEvent<TimingInformationChangedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_mainConnectionID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_timeSync_t, CommonAPI::EmptyDeployment>> timingInformationChanged_;
    CommonAPI::DBus::DBusEvent<SinkUpdatedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sinkID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sinkClass_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_MainSoundProperty_L, ::v3::org::genivi::am_t_::am_MainSoundProperty_LDeployment_t>> sinkUpdated_;
    CommonAPI::DBus::DBusEvent<SourceUpdatedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sourceID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sourceClass_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_MainSoundProperty_L, ::v3::org::genivi::am_t_::am_MainSoundProperty_LDeployment_t>> sourceUpdated_;
    CommonAPI::DBus::DBusEvent<SinkNotificationEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sinkID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_NotificationPayload_s, ::v3::org::genivi::am_t_::am_NotificationPayload_sDeployment_t>> sinkNotification_;
    CommonAPI::DBus::DBusEvent<SourceNotificationEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sourceID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_NotificationPayload_s, ::v3::org::genivi::am_t_::am_NotificationPayload_sDeployment_t>> sourceNotification_;
    CommonAPI::DBus::DBusEvent<MainSinkNotificationConfigurationChangedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sinkID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_NotificationConfiguration_s, ::v3::org::genivi::am_t_::am_NotificationConfiguration_sDeployment_t>> mainSinkNotificationConfigurationChanged_;
    CommonAPI::DBus::DBusEvent<MainSourceNotificationConfigurationChangedEvent, CommonAPI::Deployable<::v3::org::genivi::am_t::am_sourceID_t, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v3::org::genivi::am_t::am_NotificationConfiguration_s, ::v3::org::genivi::am_t_::am_NotificationConfiguration_sDeployment_t>> mainSourceNotificationConfigurationChanged_;

};

} // namespace commandinterface
} // namespace am
} // namespace genivi
} // namespace org
} // namespace v3

#endif // V3_ORG_GENIVI_AM_COMMANDINTERFACE_Command_Control_DBUS_PROXY_HPP_

