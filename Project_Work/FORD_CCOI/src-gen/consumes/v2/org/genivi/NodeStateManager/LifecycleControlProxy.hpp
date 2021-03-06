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
 * description: "Control" interface of the NodeStateManager.
		This interface contains
 *   functions, which have direct influence on the system state and therefore have
 *   to be handled with care. The interface can only be used by certain clients,
 *   which need to be configured in the D-Bus configuration.
 */
#ifndef V2_ORG_GENIVI_NODESTATEMANAGER_Lifecycle_Control_PROXY_HPP_
#define V2_ORG_GENIVI_NODESTATEMANAGER_Lifecycle_Control_PROXY_HPP_

#include <v2/org/genivi/NodeStateManager/LifecycleControlProxyBase.hpp>


#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif


#undef COMMONAPI_INTERNAL_COMPILATION

namespace v2 {
namespace org {
namespace genivi {
namespace NodeStateManager {

template <typename ... _AttributeExtensions>
class LifecycleControlProxy
    : virtual public LifecycleControl, 
      virtual public LifecycleControlProxyBase,
      virtual public _AttributeExtensions... {
public:
    LifecycleControlProxy(std::shared_ptr<CommonAPI::Proxy> delegate);
    ~LifecycleControlProxy();

    typedef LifecycleControl InterfaceType;




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
     * Calls RequestNodeRestart with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void RequestNodeRestart(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmRestartReason &_RestartReason, const uint32_t &_RestartType, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls RequestNodeRestart with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> RequestNodeRestartAsync(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmRestartReason &_RestartReason, const uint32_t &_RestartType, RequestNodeRestartAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: SetApplicationMode:
        	ApplicationModeId: This parameter will be based upon
     *   the NsmNodeApplicationMode_e.
        	ErrorCode:         Return value passed to
     *   the caller, based upon NsmErrorStatus_e.
        	The method is used by other
     *   applications to request a restart of the node.
     * author: Giridhar
     * Calls SetApplicationMode with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void SetApplicationMode(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmApplicationMode &_ApplicationModeId, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls SetApplicationMode with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> SetApplicationModeAsync(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmApplicationMode &_ApplicationModeId, SetApplicationModeAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
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
     * Calls SetNodeState with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void SetNodeState(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmNodeState &_NodeStateId, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls SetNodeState with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> SetNodeStateAsync(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmNodeState &_NodeStateId, SetNodeStateAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
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
     * Calls SetBootMode with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void SetBootMode(const int32_t &_BootMode, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls SetBootMode with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> SetBootModeAsync(const int32_t &_BootMode, SetBootModeAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
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
     * Calls SetAppHealthStatus with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void SetAppHealthStatus(const std::string &_AppName, const bool &_AppRunning, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls SetAppHealthStatus with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> SetAppHealthStatusAsync(const std::string &_AppName, const bool &_AppRunning, SetAppHealthStatusAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
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
     * Calls CheckLucRequired with synchronous semantics.
     * 
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void CheckLucRequired(CommonAPI::CallStatus &_internalCallStatus, bool &_LucWanted, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls CheckLucRequired with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> CheckLucRequiredAsync(CheckLucRequiredAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);


    /**
     * Returns the CommonAPI address of the remote partner this proxy communicates with.
     */
    virtual const CommonAPI::Address &getAddress() const;

    /**
     * Returns true if the remote partner for this proxy is currently known to be available.
     */
    virtual bool isAvailable() const;

    /**
     * Returns true if the remote partner for this proxy is available.
     */
    virtual bool isAvailableBlocking() const;

    /**
     * Returns the wrapper class that is used to (de-)register for notifications about
     * the availability of the remote partner of this proxy.
     */
    virtual CommonAPI::ProxyStatusEvent& getProxyStatusEvent();

    /**
     * Returns the wrapper class that is used to access version information of the remote
     * partner of this proxy.
     */
    virtual CommonAPI::InterfaceVersionAttribute& getInterfaceVersionAttribute();

 private:
    std::shared_ptr<LifecycleControlProxyBase> delegate_;
};

typedef LifecycleControlProxy<> LifecycleControlProxyDefault;


//
// LifecycleControlProxy Implementation
//
template <typename ... _AttributeExtensions>
LifecycleControlProxy<_AttributeExtensions...>::LifecycleControlProxy(std::shared_ptr<CommonAPI::Proxy> delegate):
        _AttributeExtensions(*(std::dynamic_pointer_cast<LifecycleControlProxyBase>(delegate)))...,
        delegate_(std::dynamic_pointer_cast<LifecycleControlProxyBase>(delegate)) {
}

template <typename ... _AttributeExtensions>
LifecycleControlProxy<_AttributeExtensions...>::~LifecycleControlProxy() {
}

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
template <typename ... _AttributeExtensions>
void LifecycleControlProxy<_AttributeExtensions...>::RequestNodeRestart(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmRestartReason &_RestartReason, const uint32_t &_RestartType, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info) {
    if (!_RestartReason.validate()) {
        _internalCallStatus = CommonAPI::CallStatus::INVALID_VALUE;
        return;
    }
    delegate_->RequestNodeRestart(_RestartReason, _RestartType, _internalCallStatus, _ErrorCode, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> LifecycleControlProxy<_AttributeExtensions...>::RequestNodeRestartAsync(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmRestartReason &_RestartReason, const uint32_t &_RestartType, RequestNodeRestartAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    if (!_RestartReason.validate()) {
        ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus ErrorCode = ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus::NsmErrorStatus_NotSet;
        _callback(CommonAPI::CallStatus::INVALID_VALUE, ErrorCode);
        std::promise<CommonAPI::CallStatus> promise;
        promise.set_value(CommonAPI::CallStatus::INVALID_VALUE);
        return promise.get_future();
    }
    return delegate_->RequestNodeRestartAsync(_RestartReason, _RestartType, _callback, _info);
}
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
template <typename ... _AttributeExtensions>
void LifecycleControlProxy<_AttributeExtensions...>::SetApplicationMode(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmApplicationMode &_ApplicationModeId, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info) {
    if (!_ApplicationModeId.validate()) {
        _internalCallStatus = CommonAPI::CallStatus::INVALID_VALUE;
        return;
    }
    delegate_->SetApplicationMode(_ApplicationModeId, _internalCallStatus, _ErrorCode, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> LifecycleControlProxy<_AttributeExtensions...>::SetApplicationModeAsync(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmApplicationMode &_ApplicationModeId, SetApplicationModeAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    if (!_ApplicationModeId.validate()) {
        ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus ErrorCode = ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus::NsmErrorStatus_NotSet;
        _callback(CommonAPI::CallStatus::INVALID_VALUE, ErrorCode);
        std::promise<CommonAPI::CallStatus> promise;
        promise.set_value(CommonAPI::CallStatus::INVALID_VALUE);
        return promise.get_future();
    }
    return delegate_->SetApplicationModeAsync(_ApplicationModeId, _callback, _info);
}
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
template <typename ... _AttributeExtensions>
void LifecycleControlProxy<_AttributeExtensions...>::SetNodeState(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmNodeState &_NodeStateId, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info) {
    if (!_NodeStateId.validate()) {
        _internalCallStatus = CommonAPI::CallStatus::INVALID_VALUE;
        return;
    }
    delegate_->SetNodeState(_NodeStateId, _internalCallStatus, _ErrorCode, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> LifecycleControlProxy<_AttributeExtensions...>::SetNodeStateAsync(const ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmNodeState &_NodeStateId, SetNodeStateAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    if (!_NodeStateId.validate()) {
        ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus ErrorCode = ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus::NsmErrorStatus_NotSet;
        _callback(CommonAPI::CallStatus::INVALID_VALUE, ErrorCode);
        std::promise<CommonAPI::CallStatus> promise;
        promise.set_value(CommonAPI::CallStatus::INVALID_VALUE);
        return promise.get_future();
    }
    return delegate_->SetNodeStateAsync(_NodeStateId, _callback, _info);
}
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
template <typename ... _AttributeExtensions>
void LifecycleControlProxy<_AttributeExtensions...>::SetBootMode(const int32_t &_BootMode, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info) {
    delegate_->SetBootMode(_BootMode, _internalCallStatus, _ErrorCode, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> LifecycleControlProxy<_AttributeExtensions...>::SetBootModeAsync(const int32_t &_BootMode, SetBootModeAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->SetBootModeAsync(_BootMode, _callback, _info);
}
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
template <typename ... _AttributeExtensions>
void LifecycleControlProxy<_AttributeExtensions...>::SetAppHealthStatus(const std::string &_AppName, const bool &_AppRunning, CommonAPI::CallStatus &_internalCallStatus, ::v2::org::genivi::NodeStateManager::NodeStateTypes::NsmErrorStatus &_ErrorCode, const CommonAPI::CallInfo *_info) {
    delegate_->SetAppHealthStatus(_AppName, _AppRunning, _internalCallStatus, _ErrorCode, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> LifecycleControlProxy<_AttributeExtensions...>::SetAppHealthStatusAsync(const std::string &_AppName, const bool &_AppRunning, SetAppHealthStatusAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->SetAppHealthStatusAsync(_AppName, _AppRunning, _callback, _info);
}
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
template <typename ... _AttributeExtensions>
void LifecycleControlProxy<_AttributeExtensions...>::CheckLucRequired(CommonAPI::CallStatus &_internalCallStatus, bool &_LucWanted, const CommonAPI::CallInfo *_info) {
    delegate_->CheckLucRequired(_internalCallStatus, _LucWanted, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> LifecycleControlProxy<_AttributeExtensions...>::CheckLucRequiredAsync(CheckLucRequiredAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->CheckLucRequiredAsync(_callback, _info);
}

template <typename ... _AttributeExtensions>
const CommonAPI::Address &LifecycleControlProxy<_AttributeExtensions...>::getAddress() const {
    return delegate_->getAddress();
}

template <typename ... _AttributeExtensions>
bool LifecycleControlProxy<_AttributeExtensions...>::isAvailable() const {
    return delegate_->isAvailable();
}

template <typename ... _AttributeExtensions>
bool LifecycleControlProxy<_AttributeExtensions...>::isAvailableBlocking() const {
    return delegate_->isAvailableBlocking();
}

template <typename ... _AttributeExtensions>
CommonAPI::ProxyStatusEvent& LifecycleControlProxy<_AttributeExtensions...>::getProxyStatusEvent() {
    return delegate_->getProxyStatusEvent();
}

template <typename ... _AttributeExtensions>
CommonAPI::InterfaceVersionAttribute& LifecycleControlProxy<_AttributeExtensions...>::getInterfaceVersionAttribute() {
    return delegate_->getInterfaceVersionAttribute();
}


} // namespace NodeStateManager
} // namespace genivi
} // namespace org
} // namespace v2



// Compatibility
namespace v2_0 = v2;

#endif // V2_ORG_GENIVI_NODESTATEMANAGER_Lifecycle_Control_PROXY_HPP_
