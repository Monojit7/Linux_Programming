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
 * description: "Control" interface of the NodeStateManager.
		This interface contains
 *   functions, which have direct influence on the system state and therefore have
 *   to be handled with care. The interface can only be used by certain clients,
 *   which need to be configured in the D-Bus configuration.
 */
#ifndef V2_ORG_GENIVI_NODESTATEMANAGER_Lifecycle_Control_DBUS_PROXY_HPP_
#define V2_ORG_GENIVI_NODESTATEMANAGER_Lifecycle_Control_DBUS_PROXY_HPP_

#include <v2/org/genivi/NodeStateManager/LifecycleControlProxyBase.hpp>
#include "v2/org/genivi/NodeStateManager/LifecycleControlDBusDeployment.hpp"

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/DBus/DBusAddress.hpp>
#include <CommonAPI/DBus/DBusFactory.hpp>
#include <CommonAPI/DBus/DBusProxy.hpp>
#include <CommonAPI/DBus/DBusAddressTranslator.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

#include <string>

namespace v2 {
namespace org {
namespace genivi {
namespace NodeStateManager {

class LifecycleControlDBusProxy
    : virtual public LifecycleControlProxyBase, 
      virtual public CommonAPI::DBus::DBusProxy {
public:
    LifecycleControlDBusProxy(
        const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection);

    virtual ~LifecycleControlDBusProxy() { }



    /**
     * description: RequestNodeRestart:
        	RestartReason: The passed value will be based upon the
     *   enum NsmRestartReason_e.
        	RestartType:   This is the type of restart that
     *   is required, i.e. during Coding process it is normal for a fast shutdown to
     *   be	requested whereas other Diagnosis restart requests would be normal.
     *   Possible values are NSM_SHUTDOWNTYPE_FAST and NSM_SHUTDOWNTYPE_NORMAL.
       
     *   	ErrorCode:     Return value passed to the caller, based upon
     *   NsmErrorStatus_e.
    
        	The method is used by other applications to request a
     *   restart of the node.
     * author: Giridhar
     */
    virtual void RequestNodeRestart(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmRestartReason &_RestartReason, const uint32_t &_RestartType, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> RequestNodeRestartAsync(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmRestartReason &_RestartReason, const uint32_t &_RestartType, RequestNodeRestartAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: SetApplicationMode:
        	ApplicationModeId: This parameter will be based upon
     *   the NsmNodeApplicationMode_e.
        	ErrorCode:         Return value passed to
     *   the caller, based upon NsmErrorStatus_e.
        	The method is used by other
     *   applications to request a restart of the node.
     * author: Giridhar
     */
    virtual void SetApplicationMode(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmApplicationMode &_ApplicationModeId, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> SetApplicationModeAsync(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmApplicationMode &_ApplicationModeId, SetApplicationModeAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: SetNodeState:
        	NodeStateId: The passed value will be based upon the enum
     *   NsmNodeState_e.
        	ErrorCode:   Return value passed to the caller, based
     *   upon NsmErrorStatus_e.
        
        	The method is used by other applications to
     *   set the NodeState. When this method is called to change the current NodeState
     *   a signal will be sent to notify registered consumers of the new state.
     * author: Giridhar
     */
    virtual void SetNodeState(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmNodeState &_NodeStateId, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> SetNodeStateAsync(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmNodeState &_NodeStateId, SetNodeStateAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: SetBootMode:
        	BootMode:   New BootMode to be set. The values will be
     *   defined by a third party header, which has not been delivered yet. The
     *   description needs to be updated as soon as the header is available.
       
     *   	ErrorCode:  Return value passed to the caller, based upon NsmErrorStatus_e.
     
     *     
        	The method has been introduced, because the property "BootMode" can
     *   only be read by other applications. Nevertheless there are some exceptions
     *   where the property should be set by a restricted set of applications which
     *   will be handled within this method.
     * author: Giridhar
     */
    virtual void SetBootMode(const int32_t &_BootMode, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> SetBootModeAsync(const int32_t &_BootMode, SetBootModeAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: SetAppHealthStatus:
        	AppName:    This parameter can be used to give the
     *   name of the application that has failed (this must be the applications systemd
     *   unit name).
        	AppRunning: The AppState will be FALSE for failed and TRUE
     *   for running.
        	ErrorCode:  Return value passed to the caller, based upon
     *   NsmErrorStatus_e.
        
        	This method will be used by the Node Health
     *   Monitor to report to the NSM if an application has failed and if it is running
     *   again. Internally the NSM will use this information to keep a count of the
     *   number of failed applications within the current lifecycle. Additionally it
     *   will unset any sessions that the failing application may have had active. It
     *   will also be possible for the product node state machine to make a decision on
     *   what to do with this information, i.e. even reset the node or reset the node
     *   if too many applications have failed.
       		The method is used by other
     *   applications to request a restart of the node.
     * author: Giridhar
     */
    virtual void SetAppHealthStatus(const std::string &_AppName, const bool &_AppRunning, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> SetAppHealthStatusAsync(const std::string &_AppName, const bool &_AppRunning, SetAppHealthStatusAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: CheckLucRequired:
        	LucWanted: This will be a simple TRUE or FALSE to define
     *   whether the LUC is wanted in the current Lifecycle.
       
        	This method will
     *   be called exclusivley by the Boot Manager to find out whether the LUC
     *   Applications should be started in the current Lifecycle. This is required
     *   whilst in certain Node Application Modes (i.e. Transport, Factory) we do not
     *   want the LUC Applications started.
     * author: Giridhar
     */
    virtual void CheckLucRequired(CommonAPI::CallStatus &_internalCallStatus, bool &_LucWanted, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> CheckLucRequiredAsync(CheckLucRequiredAsyncCallback _callback, const CommonAPI::CallInfo *_info);


    virtual void getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const;

private:
   


};

} // namespace NodeStateManager
} // namespace genivi
} // namespace org
} // namespace v2

#endif // V2_ORG_GENIVI_NODESTATEMANAGER_Lifecycle_Control_DBUS_PROXY_HPP_

