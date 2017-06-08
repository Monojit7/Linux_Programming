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
 * description: Vehicle Settings interface for Master Reset functionality.
 * author: Yogesh Sharma
 */
#include <v0/com/harman/vehicle/MasterResetDBusProxy.hpp>

namespace v0 {
namespace com {
namespace harman {
namespace vehicle {

std::shared_ptr<CommonAPI::DBus::DBusProxy> createMasterResetDBusProxy(
	const CommonAPI::DBus::DBusAddress &_address,
	const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection) {
	return std::make_shared<MasterResetDBusProxy>(_address, _connection);
}

INITIALIZER(registerMasterResetDBusProxy) {
     CommonAPI::DBus::Factory::get()->registerProxyCreateMethod(
		MasterReset::getInterface(),
		&createMasterResetDBusProxy);
}

MasterResetDBusProxy::MasterResetDBusProxy(
	const CommonAPI::DBus::DBusAddress &_address,
	const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection)
	:	CommonAPI::DBus::DBusProxy(_address, _connection)
,		factoryResetNotification_(*this, "FactoryResetNotification", "i", std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr))),
		factoryResetRequestResponse_(*this, "FactoryResetRequestResponse", "ii", std::make_tuple(static_cast<CommonAPI::EmptyDeployment*>(nullptr), static_cast<CommonAPI::EmptyDeployment*>(nullptr)))
{
}


MasterResetDBusProxy::FactoryResetNotificationEvent& MasterResetDBusProxy::getFactoryResetNotificationEvent() {
    return factoryResetNotification_;
}
MasterResetDBusProxy::FactoryResetRequestResponseEvent& MasterResetDBusProxy::getFactoryResetRequestResponseEvent() {
    return factoryResetRequestResponse_;
}
    
    /**
     * description: HMI triggers the signal to VehicleSettings to start factory reset
     *   procedure.
    					   VehicleSettings then inform other PresCtrls to reset
     *   factory defauls using broadcast FactoryResetNotification
     */
    void MasterResetDBusProxy::FactoryResetRequest(const ::v1::com::harman::vehicle::ResetType::eFactoryResetParams &_requestCode, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info) {
        CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eFactoryResetParams, CommonAPI::EmptyDeployment> deploy_requestCode(_requestCode, static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        CommonAPI::DBus::DBusProxyHelper<
            CommonAPI::DBus::DBusSerializableArguments<
            CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eFactoryResetParams, CommonAPI::EmptyDeployment >
            >,
            CommonAPI::DBus::DBusSerializableArguments<
            >
        	>::callMethodWithReply(
        *this,
        "FactoryResetRequest",
        "i",
(_info ? _info : &CommonAPI::DBus::defaultCallInfo),
deploy_requestCode,
_internalCallStatus);
}
    std::future<CommonAPI::CallStatus> MasterResetDBusProxy::FactoryResetRequestAsync(const ::v1::com::harman::vehicle::ResetType::eFactoryResetParams &_requestCode, FactoryResetRequestAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
        CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eFactoryResetParams, CommonAPI::EmptyDeployment> deploy_requestCode(_requestCode, static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        return CommonAPI::DBus::DBusProxyHelper<
            CommonAPI::DBus::DBusSerializableArguments<
            CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eFactoryResetParams, CommonAPI::EmptyDeployment >
            >,
            CommonAPI::DBus::DBusSerializableArguments<
            >
        	>::callMethodAsync(
        *this,
        "FactoryResetRequest",
        "i",
        (_info ? _info : &CommonAPI::DBus::defaultCallInfo),
        deploy_requestCode,
        [_callback] (CommonAPI::CallStatus _internalCallStatus) {
        	if (_callback)
        		_callback(_internalCallStatus);
        },
        std::make_tuple());
    }
    /**
     * description: Factory Reset Status from PresCtrls to VehicleSettings to inform the status of
     *   the FactoryReset.
    					   VehicleSetings would track the return status of all
     *   the components.
    					   In case some component doesn't come back with in
     *   pre-defined time, VehicleSettings would continue with the reset.
    					   This
     *   is just for VehicleSettings internal bookkeeping purpose. Also would be used
     *   to fine tune the timeout time.
     */
    void MasterResetDBusProxy::FactoryResetStatus(const ::v1::com::harman::vehicle::ResetType::eModuleID &_moduleID, const ::v1::com::harman::vehicle::ResetType::eFactoryResetStatus &_resetStatus, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info) {
        CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eModuleID, CommonAPI::EmptyDeployment> deploy_moduleID(_moduleID, static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eFactoryResetStatus, CommonAPI::EmptyDeployment> deploy_resetStatus(_resetStatus, static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        CommonAPI::DBus::DBusProxyHelper<
            CommonAPI::DBus::DBusSerializableArguments<
            CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eModuleID, CommonAPI::EmptyDeployment >,
            CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eFactoryResetStatus, CommonAPI::EmptyDeployment >
            >,
            CommonAPI::DBus::DBusSerializableArguments<
            >
        	>::callMethodWithReply(
        *this,
        "FactoryResetStatus",
        "ii",
(_info ? _info : &CommonAPI::DBus::defaultCallInfo),
deploy_moduleID, deploy_resetStatus,
_internalCallStatus);
}
    std::future<CommonAPI::CallStatus> MasterResetDBusProxy::FactoryResetStatusAsync(const ::v1::com::harman::vehicle::ResetType::eModuleID &_moduleID, const ::v1::com::harman::vehicle::ResetType::eFactoryResetStatus &_resetStatus, FactoryResetStatusAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
        CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eModuleID, CommonAPI::EmptyDeployment> deploy_moduleID(_moduleID, static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eFactoryResetStatus, CommonAPI::EmptyDeployment> deploy_resetStatus(_resetStatus, static_cast<CommonAPI::EmptyDeployment*>(nullptr));
        return CommonAPI::DBus::DBusProxyHelper<
            CommonAPI::DBus::DBusSerializableArguments<
            CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eModuleID, CommonAPI::EmptyDeployment >,
            CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eFactoryResetStatus, CommonAPI::EmptyDeployment >
            >,
            CommonAPI::DBus::DBusSerializableArguments<
            >
        	>::callMethodAsync(
        *this,
        "FactoryResetStatus",
        "ii",
        (_info ? _info : &CommonAPI::DBus::defaultCallInfo),
        deploy_moduleID, deploy_resetStatus,
        [_callback] (CommonAPI::CallStatus _internalCallStatus) {
        	if (_callback)
        		_callback(_internalCallStatus);
        },
        std::make_tuple());
    }
    /**
     * description: Restart request from HMI to VehicleSettings. HMI could show some warning
     *   message to user and once user confirms,
    	                   send the restart
     *   request to VehicleSettings which in turn informs NSM for restart.
     */
    void MasterResetDBusProxy::requestRestart(CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info) {
        CommonAPI::DBus::DBusProxyHelper<
            CommonAPI::DBus::DBusSerializableArguments<
            >,
            CommonAPI::DBus::DBusSerializableArguments<
            >
        	>::callMethodWithReply(
        *this,
        "requestRestart",
        "",
(_info ? _info : &CommonAPI::DBus::defaultCallInfo),
_internalCallStatus);
}
    std::future<CommonAPI::CallStatus> MasterResetDBusProxy::requestRestartAsync(RequestRestartAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
        return CommonAPI::DBus::DBusProxyHelper<
            CommonAPI::DBus::DBusSerializableArguments<
            >,
            CommonAPI::DBus::DBusSerializableArguments<
            >
        	>::callMethodAsync(
        *this,
        "requestRestart",
        "",
        (_info ? _info : &CommonAPI::DBus::defaultCallInfo),
        [_callback] (CommonAPI::CallStatus _internalCallStatus) {
        	if (_callback)
        		_callback(_internalCallStatus);
        },
        std::make_tuple());
    }


void MasterResetDBusProxy::getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const {
          ownVersionMajor = 0;
          ownVersionMinor = 1;
      }

      } // namespace vehicle
      } // namespace harman
      } // namespace com
      } // namespace v0
