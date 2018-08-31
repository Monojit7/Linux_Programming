/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#include <v1/com/harman/mmpres/MediaSettings.hpp>
#include <v1/com/harman/mmpres/MediaSettingsDBusStubAdapter.hpp>

namespace v1 {
namespace com {
namespace harman {
namespace mmpres {

std::shared_ptr<CommonAPI::DBus::DBusStubAdapter> createMediaSettingsDBusStubAdapter(
                   const CommonAPI::DBus::DBusAddress &_address,
                   const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection,
                   const std::shared_ptr<CommonAPI::StubBase> &_stub) {
    return std::make_shared<MediaSettingsDBusStubAdapter>(_address, _connection, _stub);
}

INITIALIZER(registerMediaSettingsDBusStubAdapter) {
    CommonAPI::DBus::Factory::get()->registerStubAdapterCreateMethod(
    	MediaSettings::getInterface(), &createMediaSettingsDBusStubAdapter);
}

MediaSettingsDBusStubAdapterInternal::~MediaSettingsDBusStubAdapterInternal() {
    deactivateManagedInstances();
    MediaSettingsDBusStubAdapterHelper::deinit();
}

void MediaSettingsDBusStubAdapterInternal::deactivateManagedInstances() {

}

const char* MediaSettingsDBusStubAdapterInternal::getMethodsDBusIntrospectionXmlData() const {
    static const std::string introspectionData =
        "<method name=\"getInterfaceVersion\">\n"
            "<arg name=\"value\" type=\"uu\" direction=\"out\" />"
        "</method>\n"
        "<method name=\"getSubtitleStatusAttribute\">\n"
            "<arg name=\"value\" type=\"i\" direction=\"out\" />"
        "</method>\n"
        "<signal name=\"onSubtitleStatusAttributeChanged\">\n"
            "<arg name=\"changedValue\" type=\"i\" />\n"
        "</signal>\n"
        "<method name=\"getIsFullScreenAttribute\">\n"
            "<arg name=\"value\" type=\"b\" direction=\"out\" />"
        "</method>\n"
        "<signal name=\"onIsFullScreenAttributeChanged\">\n"
            "<arg name=\"changedValue\" type=\"b\" />\n"
        "</signal>\n"
        "<method name=\"getAudioChannelAttribute\">\n"
            "<arg name=\"value\" type=\"i\" direction=\"out\" />"
        "</method>\n"
        "<signal name=\"onAudioChannelAttributeChanged\">\n"
            "<arg name=\"changedValue\" type=\"i\" />\n"
        "</signal>\n"
        "<method name=\"getAppleAuthenticationChipstatusAttribute\">\n"
            "<arg name=\"value\" type=\"b\" direction=\"out\" />"
        "</method>\n"
        "<signal name=\"onAppleAuthenticationChipstatusAttributeChanged\">\n"
            "<arg name=\"changedValue\" type=\"b\" />\n"
        "</signal>\n"
        /**
         * description: reset all the Media related settings to the default. 
        		e: error code.
         */
        "<method name=\"masterMediaReset\">\n"
            "<arg name=\"_e\" type=\"i\" direction=\"out\" />\n"
        "</method>\n"
        /**
         * description: removes the database file of specified device on device ejection.
         * param: slotId: the specified slot id
                returns:	  eCallResult:	Result of the API
         *   call
         */
        "<method name=\"resetMediaIndex\">\n"
            "<arg name=\"_slotId\" type=\"y\" direction=\"in\" />\n"
            "<arg name=\"_e\" type=\"i\" direction=\"out\" />\n"
        "</method>\n"
        /**
         * description: Updates the index of a device
         * param: slotId: the specified slot id
                returns:	  e:	Result of the API call
         */
        "<method name=\"updateIndex\">\n"
            "<arg name=\"_slotId\" type=\"y\" direction=\"in\" />\n"
            "<arg name=\"_e\" type=\"i\" direction=\"out\" />\n"
        "</method>\n"
        /**
         * description: Set Audio channel for playback
         * param: audioChannel - of type eAudioChannelStatus
         */
        "<method name=\"setAudioChannel\">\n"
            "<arg name=\"_audioChannel\" type=\"i\" direction=\"in\" />\n"
            "<arg name=\"_result\" type=\"i\" direction=\"out\" />\n"
        "</method>\n"
        /**
         * description: Set Subtitle ON or OFF
         * param: dispSubtitle - eSubtitleStatus:ON is to enable Subtitle, eSubtitleStatus:OFF is
         *   to disable subtitle
         */
        "<method name=\"setSubtitle\">\n"
            "<arg name=\"_dispSubtitle\" type=\"i\" direction=\"in\" />\n"
            "<arg name=\"_result\" type=\"i\" direction=\"out\" />\n"
        "</method>\n"
        /**
         * description: Set the display window coordinates of video playback
         * param: isFullscreen True is to enable fullscreen, false is to disable fullscreen and
         *   manually assign by screeenDimension
                        screeenDimension  Screen
         *   coordinates
         */
        "<method name=\"setDisplayWindow\">\n"
            "<arg name=\"_isFullscreen\" type=\"b\" direction=\"in\" />\n"
            "<arg name=\"_screeenDimension\" type=\"(uuuu)\" direction=\"in\" />\n"
            "<arg name=\"_e\" type=\"i\" direction=\"out\" />\n"
        "</method>\n"

    ;
    return introspectionData.c_str();
}

CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v1::com::harman::mmpres::MediaSettingsStub,
        CommonAPI::Version
        > MediaSettingsDBusStubAdapterInternal::getMediaSettingsInterfaceVersionStubDispatcher(&MediaSettingsStub::getInterfaceVersion, "uu");

CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v1::com::harman::mmpres::MediaSettingsStub,
        ::v2::com::harman::mmpres::MMPresTypes::eSubtitleStatus
        > MediaSettingsDBusStubAdapterInternal::getSubtitleStatusAttributeStubDispatcher(
            &::v1::com::harman::mmpres::MediaSettingsStub::getSubtitleStatusAttribute
            , "i"
            );
CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v1::com::harman::mmpres::MediaSettingsStub,
        bool
        > MediaSettingsDBusStubAdapterInternal::getIsFullScreenAttributeStubDispatcher(
            &::v1::com::harman::mmpres::MediaSettingsStub::getIsFullScreenAttribute
            , "b"
            );
/**
 * description: Indicates the audio channels for the current track
 */
CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v1::com::harman::mmpres::MediaSettingsStub,
        ::v2::com::harman::mmpres::MMPresTypes::eAudioChannelStatus
        > MediaSettingsDBusStubAdapterInternal::getAudioChannelAttributeStubDispatcher(
            &::v1::com::harman::mmpres::MediaSettingsStub::getAudioChannelAttribute
            , "i"
            );
/**
 * description: this will provide information as to whether the Apple authentication ship is
 *   operational or not
 */
CommonAPI::DBus::DBusGetAttributeStubDispatcher<
        ::v1::com::harman::mmpres::MediaSettingsStub,
        bool
        > MediaSettingsDBusStubAdapterInternal::getAppleAuthenticationChipstatusAttributeStubDispatcher(
            &::v1::com::harman::mmpres::MediaSettingsStub::getAppleAuthenticationChipstatusAttribute
            , "b"
            );



/**
 * description: reset all the Media related settings to the default. 
		e: error code.
 */
CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v1::com::harman::mmpres::MediaSettingsStub,
    std::tuple<>,
    std::tuple<::v2::com::harman::mmpres::MMPresTypes::eCallResult>,
    std::tuple<>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > MediaSettingsDBusStubAdapterInternal::masterMediaResetStubDispatcher(
    &MediaSettingsStub::masterMediaReset, "i",
					std::make_tuple(),
                	std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr)));

/**
 * description: removes the database file of specified device on device ejection.
 * param: slotId: the specified slot id
        returns:	  eCallResult:	Result of the API
 *   call
 */
CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v1::com::harman::mmpres::MediaSettingsStub,
    std::tuple<uint8_t>,
    std::tuple<::v2::com::harman::mmpres::MMPresTypes::eCallResult>,
    std::tuple<CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > MediaSettingsDBusStubAdapterInternal::resetMediaIndexStubDispatcher(
    &MediaSettingsStub::resetMediaIndex, "i",
					std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr)),
                	std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr)));

/**
 * description: Updates the index of a device
 * param: slotId: the specified slot id
        returns:	  e:	Result of the API call
 */
CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v1::com::harman::mmpres::MediaSettingsStub,
    std::tuple<uint8_t>,
    std::tuple<::v2::com::harman::mmpres::MMPresTypes::eMediaError>,
    std::tuple<CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > MediaSettingsDBusStubAdapterInternal::updateIndexStubDispatcher(
    &MediaSettingsStub::updateIndex, "i",
					std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr)),
                	std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr)));

/**
 * description: Set Audio channel for playback
 * param: audioChannel - of type eAudioChannelStatus
 */
CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v1::com::harman::mmpres::MediaSettingsStub,
    std::tuple<::v2::com::harman::mmpres::MMPresTypes::eAudioChannelStatus>,
    std::tuple<::v2::com::harman::mmpres::MMPresTypes::eCallResult>,
    std::tuple<CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > MediaSettingsDBusStubAdapterInternal::setAudioChannelStubDispatcher(
    &MediaSettingsStub::setAudioChannel, "i",
					std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr)),
                	std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr)));

/**
 * description: Set Subtitle ON or OFF
 * param: dispSubtitle - eSubtitleStatus:ON is to enable Subtitle, eSubtitleStatus:OFF is
 *   to disable subtitle
 */
CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v1::com::harman::mmpres::MediaSettingsStub,
    std::tuple<::v2::com::harman::mmpres::MMPresTypes::eSubtitleStatus>,
    std::tuple<::v2::com::harman::mmpres::MMPresTypes::eCallResult>,
    std::tuple<CommonAPI::EmptyDeployment>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > MediaSettingsDBusStubAdapterInternal::setSubtitleStubDispatcher(
    &MediaSettingsStub::setSubtitle, "i",
					std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr)),
                	std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr)));

/**
 * description: Set the display window coordinates of video playback
 * param: isFullscreen True is to enable fullscreen, false is to disable fullscreen and
 *   manually assign by screeenDimension
                screeenDimension  Screen
 *   coordinates
 */
CommonAPI::DBus::DBusMethodWithReplyStubDispatcher<
    ::v1::com::harman::mmpres::MediaSettingsStub,
    std::tuple<bool, ::v2::com::harman::mmpres::MMPresTypes::tScreen>,
    std::tuple<::v2::com::harman::mmpres::MMPresTypes::eMediaError>,
    std::tuple<CommonAPI::EmptyDeployment, ::v2::com::harman::mmpres::MMPresTypes_::tScreenDeployment_t>,
    std::tuple<CommonAPI::EmptyDeployment>
    
    > MediaSettingsDBusStubAdapterInternal::setDisplayWindowStubDispatcher(
    &MediaSettingsStub::setDisplayWindow, "i",
					std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr), static_cast<::v2::com::harman::mmpres::MMPresTypes_::tScreenDeployment_t*>(nullptr)),
                	std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr)));


void MediaSettingsDBusStubAdapterInternal::fireSubtitleStatusAttributeChanged(const ::v2::com::harman::mmpres::MMPresTypes::eSubtitleStatus& value) {
    CommonAPI::DBus::DBusStubSignalHelper<CommonAPI::DBus::DBusSerializableArguments<
    ::v2::com::harman::mmpres::MMPresTypes::eSubtitleStatus
    >>
        ::sendSignal(
            *this,
            "onSubtitleStatusAttributeChanged",
            "i",
            value
    
    );
}
void MediaSettingsDBusStubAdapterInternal::fireIsFullScreenAttributeChanged(const bool& value) {
    CommonAPI::DBus::DBusStubSignalHelper<CommonAPI::DBus::DBusSerializableArguments<
    bool
    >>
        ::sendSignal(
            *this,
            "onIsFullScreenAttributeChanged",
            "b",
            value
    
    );
}
/**
 * description: Indicates the audio channels for the current track
 */
void MediaSettingsDBusStubAdapterInternal::fireAudioChannelAttributeChanged(const ::v2::com::harman::mmpres::MMPresTypes::eAudioChannelStatus& value) {
    CommonAPI::DBus::DBusStubSignalHelper<CommonAPI::DBus::DBusSerializableArguments<
    ::v2::com::harman::mmpres::MMPresTypes::eAudioChannelStatus
    >>
        ::sendSignal(
            *this,
            "onAudioChannelAttributeChanged",
            "i",
            value
    
    );
}
/**
 * description: this will provide information as to whether the Apple authentication ship is
 *   operational or not
 */
void MediaSettingsDBusStubAdapterInternal::fireAppleAuthenticationChipstatusAttributeChanged(const bool& value) {
    CommonAPI::DBus::DBusStubSignalHelper<CommonAPI::DBus::DBusSerializableArguments<
    bool
    >>
        ::sendSignal(
            *this,
            "onAppleAuthenticationChipstatusAttributeChanged",
            "b",
            value
    
    );
}



const MediaSettingsDBusStubAdapterHelper::StubDispatcherTable& MediaSettingsDBusStubAdapterInternal::getStubDispatcherTable() {
    return stubDispatcherTable_;
}

const CommonAPI::DBus::StubAttributeTable& MediaSettingsDBusStubAdapterInternal::getStubAttributeTable() {
    return stubAttributeTable_;
}

MediaSettingsDBusStubAdapterInternal::MediaSettingsDBusStubAdapterInternal(
        const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection,
        const std::shared_ptr<CommonAPI::StubBase> &_stub)
	: CommonAPI::DBus::DBusStubAdapter(_address, _connection,false),
      MediaSettingsDBusStubAdapterHelper(_address, _connection, std::dynamic_pointer_cast<MediaSettingsStub>(_stub), false),
      stubDispatcherTable_({
            { { "getSubtitleStatusAttribute", "" }, &com::harman::mmpres::MediaSettingsDBusStubAdapterInternal::getSubtitleStatusAttributeStubDispatcher },
            { { "getIsFullScreenAttribute", "" }, &com::harman::mmpres::MediaSettingsDBusStubAdapterInternal::getIsFullScreenAttributeStubDispatcher },
            /**
             * description: Indicates the audio channels for the current track
             */
            { { "getAudioChannelAttribute", "" }, &com::harman::mmpres::MediaSettingsDBusStubAdapterInternal::getAudioChannelAttributeStubDispatcher },
            /**
             * description: this will provide information as to whether the Apple authentication ship is
             *   operational or not
             */
            { { "getAppleAuthenticationChipstatusAttribute", "" }, &com::harman::mmpres::MediaSettingsDBusStubAdapterInternal::getAppleAuthenticationChipstatusAttributeStubDispatcher }
            ,
            /**
             * description: reset all the Media related settings to the default. 
            		e: error code.
             */
            { { "masterMediaReset", "" }, &com::harman::mmpres::MediaSettingsDBusStubAdapterInternal::masterMediaResetStubDispatcher },
            /**
             * description: removes the database file of specified device on device ejection.
             * param: slotId: the specified slot id
                    returns:	  eCallResult:	Result of the API
             *   call
             */
            { { "resetMediaIndex", "y" }, &com::harman::mmpres::MediaSettingsDBusStubAdapterInternal::resetMediaIndexStubDispatcher },
            /**
             * description: Updates the index of a device
             * param: slotId: the specified slot id
                    returns:	  e:	Result of the API call
             */
            { { "updateIndex", "y" }, &com::harman::mmpres::MediaSettingsDBusStubAdapterInternal::updateIndexStubDispatcher },
            /**
             * description: Set Audio channel for playback
             * param: audioChannel - of type eAudioChannelStatus
             */
            { { "setAudioChannel", "i" }, &com::harman::mmpres::MediaSettingsDBusStubAdapterInternal::setAudioChannelStubDispatcher },
            /**
             * description: Set Subtitle ON or OFF
             * param: dispSubtitle - eSubtitleStatus:ON is to enable Subtitle, eSubtitleStatus:OFF is
             *   to disable subtitle
             */
            { { "setSubtitle", "i" }, &com::harman::mmpres::MediaSettingsDBusStubAdapterInternal::setSubtitleStubDispatcher },
            /**
             * description: Set the display window coordinates of video playback
             * param: isFullscreen True is to enable fullscreen, false is to disable fullscreen and
             *   manually assign by screeenDimension
                            screeenDimension  Screen
             *   coordinates
             */
            { { "setDisplayWindow", "b(uuuu)" }, &com::harman::mmpres::MediaSettingsDBusStubAdapterInternal::setDisplayWindowStubDispatcher }
            }),
        stubAttributeTable_() {

    stubDispatcherTable_.insert({ { "getInterfaceVersion", "" }, &com::harman::mmpres::MediaSettingsDBusStubAdapterInternal::getMediaSettingsInterfaceVersionStubDispatcher });
}

bool MediaSettingsDBusStubAdapterInternal::hasFreedesktopProperties() {
    return false;
}

} // namespace mmpres
} // namespace harman
} // namespace com
} // namespace v1
