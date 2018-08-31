/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#include <v1/com/harman/audio/audioPresCtrl/audioSourceInfoDBusProxy.hpp>

namespace v1 {
namespace com {
namespace harman {
namespace audio {
namespace audioPresCtrl {

std::shared_ptr<CommonAPI::DBus::DBusProxy> createaudioSourceInfoDBusProxy(
	const CommonAPI::DBus::DBusAddress &_address,
	const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection) {
	return std::make_shared<audioSourceInfoDBusProxy>(_address, _connection);
}

INITIALIZER(registeraudioSourceInfoDBusProxy) {
     CommonAPI::DBus::Factory::get()->registerProxyCreateMethod(
		audioSourceInfo::getInterface(),
		&createaudioSourceInfoDBusProxy);
}

audioSourceInfoDBusProxy::audioSourceInfoDBusProxy(
	const CommonAPI::DBus::DBusAddress &_address,
	const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection)
	:	CommonAPI::DBus::DBusProxy(_address, _connection)
{
}


    
    /**
     * description: This method should be invoked by the MediaPres for Media devices to register   
     *     device information which could be needed by the HMIs     For ex: register
     *   information on media device when they are inserted.  	Please refer
     *   sSourceInfo.eSourceAvailablity to learn about availibility states     Please
     *   refer the sequence diagram to determine when this method should be called    
     *   NOTE: This need not be invoked on device removal
     */
    void audioSourceInfoDBusProxy::RegisterSourceInfo(const ::v3::com::harman::audio::audioPresCtrl::audioPresCtrlTypes::sSourceInfo &_info, CommonAPI::CallStatus &_internalCallStatus) {
        CommonAPI::Deployable<::v3::com::harman::audio::audioPresCtrl::audioPresCtrlTypes::sSourceInfo, ::v3::com::harman::audio::audioPresCtrl::audioPresCtrlTypes_::sSourceInfoDeployment_t> deploy_info(_info, static_cast<::v3::com::harman::audio::audioPresCtrl::audioPresCtrlTypes_::sSourceInfoDeployment_t*>(nullptr));
        CommonAPI::DBus::DBusProxyHelper<
            CommonAPI::DBus::DBusSerializableArguments<
            CommonAPI::Deployable<::v3::com::harman::audio::audioPresCtrl::audioPresCtrlTypes::sSourceInfo, ::v3::com::harman::audio::audioPresCtrl::audioPresCtrlTypes_::sSourceInfoDeployment_t >
            >,
            CommonAPI::DBus::DBusSerializableArguments<
            >
        	>::callMethod(
        *this,
        "RegisterSourceInfo",
        "(iisiys)",
deploy_info,
_internalCallStatus);
}


void audioSourceInfoDBusProxy::getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const {
          ownVersionMajor = 1;
          ownVersionMinor = 0;
      }

      } // namespace audioPresCtrl
      } // namespace audio
      } // namespace harman
      } // namespace com
      } // namespace v1
