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
 * description: The VersionManager component is responsible for gathering version information
 *   of the different modules at run time as well as managing
    a set of required
 *   version for a certain release, the release configuration that is defined at
 *   integration time.


    Version: 1.0
    Reason:
        - Initial Version
   
 *   Changes:
        1. readonly attributes: versionlist
        2. methods:
 *   GetVersion, SetVersion
        3. signal : versioninfochanged
 * author: Dinesh D
 */
#ifndef V1_COM_HARMAN_INFOTAINMENT_SYSTEMINFRASTRUCTURE_VERSIONMANAGER_VERSIONMANAGER_Version_Manager_PROXY_HPP_
#define V1_COM_HARMAN_INFOTAINMENT_SYSTEMINFRASTRUCTURE_VERSIONMANAGER_VERSIONMANAGER_Version_Manager_PROXY_HPP_

#include <v1/com/harman/infotainment/systeminfrastructure/versionmanager/versionmanager/VersionManagerProxyBase.hpp>


#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/AttributeExtension.hpp>
#include <CommonAPI/Factory.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v1 {
namespace com {
namespace harman {
namespace infotainment {
namespace systeminfrastructure {
namespace versionmanager {
namespace versionmanager {

template <typename ... _AttributeExtensions>
class VersionManagerProxy
    : virtual public VersionManager, 
      virtual public VersionManagerProxyBase,
      virtual public _AttributeExtensions... {
public:
    VersionManagerProxy(std::shared_ptr<CommonAPI::Proxy> delegate);
    ~VersionManagerProxy();

    typedef VersionManager InterfaceType;


    /**
     * description: list of all known version informations at runtime
     */
    /**
     * Returns the wrapper class that provides access to the attribute allVersions.
     */
    virtual AllVersionsAttribute& getAllVersionsAttribute() {
        return delegate_->getAllVersionsAttribute();
    }

    /**
     * description: signals availabilty/update of a version entry in the runtime
     */
    /**
     * Returns the wrapper class that provides access to the broadcast versionupdated.
     */
    virtual VersionupdatedSelectiveEvent& getVersionupdatedSelectiveEvent() {
        return delegate_->getVersionupdatedSelectiveEvent();
    }

    /**
     * description: a method to retrieve known version information of a specific component
           
     *   to retrieve all versions "attribute VersionInfoList allVersions" can be used
     * Calls getVersion with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void getVersion(const std::string &_versionName, CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanagertypes::versionmanagertypes::VersionInfo &_versionInfo, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls getVersion with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> getVersionAsync(const std::string &_versionName, GetVersionAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: a method to update version list from specific client
     * Calls setVersion with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void setVersion(const ::v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanagertypes::versionmanagertypes::VersionInfoList &_versions, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls setVersion with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> setVersionAsync(const ::v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanagertypes::versionmanagertypes::VersionInfoList &_versions, SetVersionAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);


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
    std::shared_ptr<VersionManagerProxyBase> delegate_;
};

typedef VersionManagerProxy<> VersionManagerProxyDefault;

namespace VersionManagerExtensions {
    /**
     * description: list of all known version informations at runtime
     */
    template <template <typename > class _ExtensionType>
    class AllVersionsAttributeExtension {
     public:
        typedef _ExtensionType<VersionManagerProxyBase::AllVersionsAttribute> extension_type;
    
        static_assert(std::is_base_of<typename CommonAPI::AttributeExtension<VersionManagerProxyBase::AllVersionsAttribute>, extension_type>::value,
                      "Not CommonAPI Attribute Extension!");
    #ifdef WIN32
        AllVersionsAttributeExtension() {}
    #endif
    
        AllVersionsAttributeExtension(VersionManagerProxyBase& proxy): attributeExtension_(proxy.getAllVersionsAttribute()) {
        }
    
        inline extension_type& getAllVersionsAttributeExtension() {
            return attributeExtension_;
        }
    
     private:
        extension_type attributeExtension_;
    };

} // namespace VersionManagerExtensions

//
// VersionManagerProxy Implementation
//
template <typename ... _AttributeExtensions>
VersionManagerProxy<_AttributeExtensions...>::VersionManagerProxy(std::shared_ptr<CommonAPI::Proxy> delegate):
        _AttributeExtensions(*(std::dynamic_pointer_cast<VersionManagerProxyBase>(delegate)))...,
        delegate_(std::dynamic_pointer_cast<VersionManagerProxyBase>(delegate)) {
}

template <typename ... _AttributeExtensions>
VersionManagerProxy<_AttributeExtensions...>::~VersionManagerProxy() {
}

/**
 * description: a method to retrieve known version information of a specific component
       
 *   to retrieve all versions "attribute VersionInfoList allVersions" can be used
 */
template <typename ... _AttributeExtensions>
void VersionManagerProxy<_AttributeExtensions...>::getVersion(const std::string &_versionName, CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanagertypes::versionmanagertypes::VersionInfo &_versionInfo, const CommonAPI::CallInfo *_info) {
    delegate_->getVersion(_versionName, _internalCallStatus, _versionInfo, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> VersionManagerProxy<_AttributeExtensions...>::getVersionAsync(const std::string &_versionName, GetVersionAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->getVersionAsync(_versionName, _callback, _info);
}
/**
 * description: a method to update version list from specific client
 */
template <typename ... _AttributeExtensions>
void VersionManagerProxy<_AttributeExtensions...>::setVersion(const ::v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanagertypes::versionmanagertypes::VersionInfoList &_versions, CommonAPI::CallStatus &_internalCallStatus, const CommonAPI::CallInfo *_info) {
    delegate_->setVersion(_versions, _internalCallStatus, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> VersionManagerProxy<_AttributeExtensions...>::setVersionAsync(const ::v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanagertypes::versionmanagertypes::VersionInfoList &_versions, SetVersionAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->setVersionAsync(_versions, _callback, _info);
}

template <typename ... _AttributeExtensions>
const CommonAPI::Address &VersionManagerProxy<_AttributeExtensions...>::getAddress() const {
    return delegate_->getAddress();
}

template <typename ... _AttributeExtensions>
bool VersionManagerProxy<_AttributeExtensions...>::isAvailable() const {
    return delegate_->isAvailable();
}

template <typename ... _AttributeExtensions>
bool VersionManagerProxy<_AttributeExtensions...>::isAvailableBlocking() const {
    return delegate_->isAvailableBlocking();
}

template <typename ... _AttributeExtensions>
CommonAPI::ProxyStatusEvent& VersionManagerProxy<_AttributeExtensions...>::getProxyStatusEvent() {
    return delegate_->getProxyStatusEvent();
}

template <typename ... _AttributeExtensions>
CommonAPI::InterfaceVersionAttribute& VersionManagerProxy<_AttributeExtensions...>::getInterfaceVersionAttribute() {
    return delegate_->getInterfaceVersionAttribute();
}


} // namespace versionmanager
} // namespace versionmanager
} // namespace systeminfrastructure
} // namespace infotainment
} // namespace harman
} // namespace com
} // namespace v1

namespace CommonAPI {
template<template<typename > class _AttributeExtension>
struct DefaultAttributeProxyHelper<::v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanager::VersionManagerProxy,
    _AttributeExtension> {
    typedef typename ::v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanager::VersionManagerProxy<
            ::v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanager::VersionManagerExtensions::AllVersionsAttributeExtension<_AttributeExtension>
    > class_t;
};
}


// Compatibility
namespace v1_0 = v1;

#endif // V1_COM_HARMAN_INFOTAINMENT_SYSTEMINFRASTRUCTURE_VERSIONMANAGER_VERSIONMANAGER_Version_Manager_PROXY_HPP_
