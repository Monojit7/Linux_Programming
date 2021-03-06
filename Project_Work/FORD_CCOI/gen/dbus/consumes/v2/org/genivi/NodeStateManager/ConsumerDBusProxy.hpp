/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V2_ORG_GENIVI_NODESTATEMANAGER_Consumer_DBUS_PROXY_HPP_
#define V2_ORG_GENIVI_NODESTATEMANAGER_Consumer_DBUS_PROXY_HPP_

#include <v2/org/genivi/NodeStateManager/ConsumerProxyBase.hpp>
#include "v2/org/genivi/NodeStateManager/ConsumerDBusDeployment.hpp"

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/DBus/DBusAddress.hpp>
#include <CommonAPI/DBus/DBusFactory.hpp>
#include <CommonAPI/DBus/DBusProxy.hpp>
#include <CommonAPI/DBus/DBusAddressTranslator.hpp>
#include <CommonAPI/DBus/DBusAttribute.hpp>
#include <CommonAPI/DBus/DBusEvent.hpp>
#include <CommonAPI/Types.hpp>
#include <CommonAPI/DBus/DBusSelectiveEvent.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

#include <string>

namespace v2 {
namespace org {
namespace genivi {
namespace NodeStateManager {

class ConsumerDBusProxy
    : virtual public ConsumerProxyBase, 
      virtual public CommonAPI::DBus::DBusProxy {
public:
    ConsumerDBusProxy(
        const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection);

    virtual ~ConsumerDBusProxy() { }

    virtual RestartReasonAttribute& getRestartReasonAttribute();
    virtual ShutdownReasonAttribute& getShutdownReasonAttribute();
    virtual WakeUpReasonAttribute& getWakeUpReasonAttribute();
    virtual BootModeAttribute& getBootModeAttribute();
    virtual VehiclePowerModeAttribute& getVehiclePowerModeAttribute();

    virtual NodeStateEvent& getNodeStateEvent();
    virtual NodeApplicationModeEvent& getNodeApplicationModeEvent();
    virtual SessionStateChangedEvent& getSessionStateChangedEvent();
    virtual ShutdownEventSelectiveEvent& getShutdownEventSelectiveEvent();

    /**
     * description: GetNodeState:
        	NodeStateId: Will be based on the NsmNodeState_e.
       
     *   	ErrorCode:   Return value passed to the caller, based upon NsmErrorStatus_e.
    
     *      	The method is used by other applications to get the NodeState without the
     *   need of registration to the signal.
     * author: Giridhar
     */
    virtual void GetNodeState(CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmNodeState &_NodeStateId, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> GetNodeStateAsync(GetNodeStateAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: SetSessionState:
        	SessionName:  The SessionName will be based upon either
     *   the pre-defined platform SessionNames (see NSM content page) or using a newly
     *   added product defined session name.
        	SessionOwner: This parameter defines
     *   the name of the application that is setting the state of the session. This
     *   must be the applications systemd unit filename.
        	SeatID:       This
     *   parameter will be based upon the enum NsmSeat_e
        	SessionState: This
     *   parameter will be based upon the NsmSessionState_e but it will not be bounded
     *   by the values in that enumeration. The listed values are the default values
     *   that are mandatory for platform sessions, but product sessions may have
     *   additional SessionStates.
        	ErrorCode:    Return value passed to the
     *   caller, based upon NsmErrorStatus_e.
        
        	The method is used by
     *   applications to set the state of a session.
     * author: Giridhar
     */
    virtual void SetSessionState(const std::string &_SessionName, const std::string &_SessionOwner, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSeat &_SeatID, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSessionState &_SessionState, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> SetSessionStateAsync(const std::string &_SessionName, const std::string &_SessionOwner, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSeat &_SeatID, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSessionState &_SessionState, SetSessionStateAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: GetSessionState:
        	SessionName:  The SessionName will be based upon either
     *   the pre-defined platform session names (see NSM content page) or using a newly
     *   added product defined SessionName.
        	SeatID:       This parameter will be
     *   based upon the enum NsmSeat_e.
        	SessionState: This parameter will be based
     *   upon the NsmSessionState_e but it will not be bounded by the values in that
     *   enumeration. The listed values are the default values that are mandatory for
     *   platform sessions, but product sessions may have additional SessionStates.
       
     *   	ErrorCode:    Return value passed to the caller, based upon
     *   NsmErrorStatus_e.
        
        	The method is used by applications to get the
     *   state of a session.
     * author: Giridhar
     */
    virtual void GetSessionState(const std::string &_SessionName, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSeat &_SeatID, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSessionState &_SessionState, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> GetSessionStateAsync(const std::string &_SessionName, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSeat &_SeatID, GetSessionStateAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: GetApplicationMode:
    			ApplicationModeId: This parameter will be based upon the
     *   NsmNodeApplicationMode_e.
    			ErrorCode:         Return value passed to the
     *   caller, based upon NsmErrorStatus_e.
    	
    			The method is used by other
     *   applications to get the application mode.
    			The method is used by
     *   applications to get the state of a session.
     * author: Giridhar
     */
    virtual void GetApplicationMode(CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmApplicationMode &_ApplicationModeId, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> GetApplicationModeAsync(GetApplicationModeAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: RegisterShutdownClient:
        	ShutdownMode: Shutdown mode for which client wants
     *   to be informed (i.e normal, fast etc).
        	TimeoutMs:    Max. Timeout to wait
     *   for response from shutdown client in ms.
        	ErrorCode:    Return value
     *   passed to the caller, based upon NsmErrorStatus_e.
        
        	The method is
     *   used by other applications to register themselves as shutdown client. Any
     *   client that registers must provide a method in their D-Bus object called
     *   "LifecycleRequest". This method will take one parameter which is the
     *   RequestType (NSM_SHUTDOWNTYPE_NORMAL, NSM_SHUTDOWNTYPE_FAST). For an example
     *   of the required client interface please see the BootManager component who will
     *   be a client of the NSM.
    		Removed Below Parameters:
     		BusName:      Bus name
     *   of remote application.
        	ObjName:      Object name of remote object that
     *   provides the shutdown interface.
     * author: Giridhar
     */
    virtual void RegisterShutdownClient(const uint32_t &_ShutdownMode, const uint32_t &_TimeoutMs, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> RegisterShutdownClientAsync(const uint32_t &_ShutdownMode, const uint32_t &_TimeoutMs, RegisterShutdownClientAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: UnRegisterShutdownClient:
        	ShutdownMode: Shutdown mode for which client
     *   wants to unregister (NSM_SHUTDOWNTYPE_NORMAL, NSM_SHUTDOWNTYPE_FAST).
       
     *   	ErrorCode:    Return value passed to the caller, based upon
     *   NsmErrorStatus_e.
        
        	The method is used by other applications to
     *   unregister themselves as shutdown client.
     		Removed Below Parameters:
    
     *   		BusName:      Bus name of remote application.
        	ObjName:      Object name
     *   of remote object that provides the shutdown interface.
     * author: Giridhar
     */
    virtual void UnRegisterShutdownClient(const uint32_t &_ShutdownMode, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> UnRegisterShutdownClientAsync(const uint32_t &_ShutdownMode, UnRegisterShutdownClientAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: RegisterSession:
        	SessionName:  The SessionName will be based upon either
     *   the pre-defined platform session names (see NSM content page) or using a newly
     *   added product defined SessionName.
        	SessionOwner: This is the name of the
     *   application that is registering the new session (this must be the applications
     *   systemd unit filename).
        	SeatID:       This parameter will be based upon
     *   the enum NsmSeatId_e
        	SessionState: This parameter will be based upon the
     *   NsmSessionState_e but it will not be bounded by the values in that
     *   enumeration. The listed values are the default values that are mandatory for
     *   platform sessions, but product sessions may have additional session states.
      
     *    	ErrorCode:    Return value passed to the caller, based upon
     *   NsmErrorStatus_e.
        
        	The method is used by other applications to
     *   register a new session whose state should be observed and distributed by the
     *   NSM.
     * author: Giridhar
     */
    virtual void RegisterSession(const std::string &_SessionName, const std::string &_SessionOwner, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSeat &_SeatID, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSessionState &_SessionState, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> RegisterSessionAsync(const std::string &_SessionName, const std::string &_SessionOwner, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSeat &_SeatID, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSessionState &_SessionState, RegisterSessionAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: UnRegisterSession:
        	SessionName:  The SessionName will be based upon either
     *   the pre-defined platform session names (see NSM content page) or using a newly
     *   added product defined SessionName.
        	SessionOwner: This is the name of the
     *   application that originally registered the session. It will be validated that
     *   this value matches the stored value from the registration.
        	SeatID:      
     *   This parameter will be based upon the enum NsmSeat_e.
        	ErrorCode:   
     *   Return value passed to the caller, based upon NsmErrorStatus_e.
        
        	The
     *   method is used by other applications to remove a new session from the session
     *   list hosted by NSM.
     * author: Giridhar
     */
    virtual void UnRegisterSession(const std::string &_SessionName, const std::string &_SessionOwner, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSeat &_SeatID, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> UnRegisterSessionAsync(const std::string &_SessionName, const std::string &_SessionOwner, const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSeat &_SeatID, UnRegisterSessionAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: GetAppHealthCount:
        	Count: Return value passed to the caller. Number of
     *   applications that crashed or terminated accidentally.
        	The method returns
     *   the number of applications that crashed or terminated accidentally, within the
     *   current life cycle. It can be used to observe the system state.
     * author: Giridhar
     */
    virtual void GetAppHealthCount(CommonAPI::CallStatus &_internalCallStatus, uint32_t &_Count, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> GetAppHealthCountAsync(GetAppHealthCountAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: GetInterfaceVersion:
        	Version: Unsigned integer that represents the version
     *   number of the Node State Manager.
        	
        	The method returns the version
     *   number of the Node State Manager. The number is organized in four bytes:
        
    
     *      	Version: VVV.RRR.PPP.BBB
        
        	<literallayout>
        		VVV => Version 
     *   [1..255]
        		RRR => Release  [0..255]
        		PPP => Patch    [0..255]
       
     *   		BBB => Build    [0..255]
        	</literallayout>
     * author: Giridhar
     */
    virtual void GetInterfaceVersion(CommonAPI::CallStatus &_internalCallStatus, uint32_t &_Version, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> GetInterfaceVersionAsync(GetInterfaceVersionAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: LifecycleRequestComplete:
            Status:    The result of the call to
     *   "LifecycleRequest". NsmErrorStatus_Ok: Request successfully processed.
     *   NsmErrorStatus_Error: An error occured while processing the
     *   "LifecycleRequest".
            ErrorCode: Return value passed to the caller,
     *   based upon NsmErrorStatus_e.
            
            The function has to be called by
     *   a "asynchrounous" lifecycle client, when he processed the "LifecycleRequest".
    
     *     		Removed below parameters:
       		RequestId: The request Id of the called
     *   life cycle client. The value has been passed when "LifecycleRequest" was
     *   called.
     * author: Giridhar
     */
    virtual void LifecycleRequestComplete(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_Status, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> LifecycleRequestCompleteAsync(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_Status, LifecycleRequestCompleteAsyncCallback _callback, const CommonAPI::CallInfo *_info);


    virtual void getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const;

private:
   
    class DBusrestartReason_Attribute : public CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<RestartReasonAttribute>> {
    public:
    template <typename... _A>
    	DBusrestartReason_Attribute(DBusProxy &_proxy,
    		_A ... arguments)
    		: CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<RestartReasonAttribute>>(
    			_proxy, arguments...) {}
    };
    DBusrestartReason_Attribute restartReason_;
    class DBusshutdownReason_Attribute : public CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<ShutdownReasonAttribute>> {
    public:
    template <typename... _A>
    	DBusshutdownReason_Attribute(DBusProxy &_proxy,
    		_A ... arguments)
    		: CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<ShutdownReasonAttribute>>(
    			_proxy, arguments...) {}
    };
    DBusshutdownReason_Attribute shutdownReason_;
    class DBuswakeUpReason_Attribute : public CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<WakeUpReasonAttribute>> {
    public:
    template <typename... _A>
    	DBuswakeUpReason_Attribute(DBusProxy &_proxy,
    		_A ... arguments)
    		: CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<WakeUpReasonAttribute>>(
    			_proxy, arguments...) {}
    };
    DBuswakeUpReason_Attribute wakeUpReason_;
    CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<BootModeAttribute>> bootMode_;
    CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusReadonlyAttribute<VehiclePowerModeAttribute, CommonAPI::DBus::StringDeployment>> vehiclePowerMode_;

    CommonAPI::DBus::DBusEvent<NodeStateEvent, CommonAPI::Deployable<::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmNodeState, CommonAPI::EmptyDeployment>> nodeState_;
    CommonAPI::DBus::DBusEvent<NodeApplicationModeEvent, CommonAPI::Deployable<::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmApplicationMode, CommonAPI::EmptyDeployment>> nodeApplicationMode_;
    CommonAPI::DBus::DBusEvent<SessionStateChangedEvent, CommonAPI::Deployable<std::string, CommonAPI::DBus::StringDeployment>, CommonAPI::Deployable<::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSeat, CommonAPI::EmptyDeployment>, CommonAPI::Deployable<::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmSessionState, CommonAPI::EmptyDeployment>> sessionStateChanged_;
    CommonAPI::DBus::DBusSelectiveEvent<ShutdownEventSelectiveEvent, CommonAPI::Deployable<uint32_t, CommonAPI::EmptyDeployment>> shutdownEventSelective_;

};

} // namespace NodeStateManager
} // namespace genivi
} // namespace org
} // namespace v2

#endif // V2_ORG_GENIVI_NODESTATEMANAGER_Consumer_DBUS_PROXY_HPP_

