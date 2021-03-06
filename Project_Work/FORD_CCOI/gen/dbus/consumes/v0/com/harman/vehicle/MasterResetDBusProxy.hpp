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
#ifndef V0_COM_HARMAN_VEHICLE_Master_Reset_DBUS_PROXY_HPP_
#define V0_COM_HARMAN_VEHICLE_Master_Reset_DBUS_PROXY_HPP_

#include <v0/com/harman/vehicle/MasterResetProxyBase.hpp>
#include "v0/com/harman/vehicle/MasterResetDBusDeployment.hpp"

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/DBus/DBusAddress.hpp>
#include <CommonAPI/DBus/DBusFactory.hpp>
#include <CommonAPI/DBus/DBusProxy.hpp>
#include <CommonAPI/DBus/DBusAddressTranslator.hpp>
#include <CommonAPI/DBus/DBusEvent.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

#include <string>

namespace v0 {
namespace com {
namespace harman {
namespace vehicle {

class MasterResetDBusProxy
    : virtual public MasterResetProxyBase, 
      virtual public CommonAPI::DBus::DBusProxy {
public:
    MasterResetDBusProxy(
        const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection);

    virtual ~MasterResetDBusProxy() { }


    virtual FactoryResetNotificationEvent& getFactoryResetNotificationEvent();
    virtual FactoryResetRequestResponseEvent& getFactoryResetRequestResponseEvent();

    /**
     * description: HMI triggers the signal to VehicleSettings to start factory reset
     *   procedure.
    					   VehicleSettings then inform other PresCtrls to reset
     *   factory defauls using broadcast FactoryResetNotification
     */
    virtual void FactoryResetRequest(const ::v1::com::harman::vehicle::ResetType::eFactoryResetParams &_requestCode, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> FactoryResetRequestAsync(const ::v1::com::harman::vehicle::ResetType::eFactoryResetParams &_requestCode, FactoryResetRequestAsyncCallback _callback, const CommonAPI::CallInfo *_info);
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
    virtual void FactoryResetStatus(const ::v1::com::harman::vehicle::ResetType::eModuleID &_moduleID, const ::v1::com::harman::vehicle::ResetType::eFactoryResetStatus &_resetStatus, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> FactoryResetStatusAsync(const ::v1::com::harman::vehicle::ResetType::eModuleID &_moduleID, const ::v1::com::harman::vehicle::ResetType::eFactoryResetStatus &_resetStatus, FactoryResetStatusAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Restart request from HMI to VehicleSettings. HMI could show some warning
     *   message to user and once user confirms,
    	                   send the restart
     *   request to VehicleSettings which in turn informs NSM for restart.
     */
    virtual void requestRestart(CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> requestRestartAsync(RequestRestartAsyncCallback _callback, const CommonAPI::CallInfo *_info);


    virtual void getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const;

private:
   

    CommonAPI::DBus::DBusEvent<FactoryResetNotificationEvent, CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eFactoryResetParams, CommonAPI::EmptyDeployment>> factoryResetNotification_;
    CommonAPI::DBus::DBusEvent<FactoryResetRequestResponseEvent, CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eFactoryResetStatus, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v1::com::harman::vehicle::ResetType::eRestartRequire, CommonAPI::EmptyDeployment>> factoryResetRequestResponse_;

};

} // namespace vehicle
} // namespace harman
} // namespace com
} // namespace v0

#endif // V0_COM_HARMAN_VEHICLE_Master_Reset_DBUS_PROXY_HPP_

