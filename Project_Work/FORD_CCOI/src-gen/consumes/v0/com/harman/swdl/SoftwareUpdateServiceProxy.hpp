/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V0_COM_HARMAN_SWDL_Software_Update_Service_PROXY_HPP_
#define V0_COM_HARMAN_SWDL_Software_Update_Service_PROXY_HPP_

#include <v0/com/harman/swdl/SoftwareUpdateServiceProxyBase.hpp>


#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/AttributeExtension.hpp>
#include <CommonAPI/Factory.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v0 {
namespace com {
namespace harman {
namespace swdl {

template <typename ... _AttributeExtensions>
class SoftwareUpdateServiceProxy
    : virtual public SoftwareUpdateService, 
      virtual public SoftwareUpdateServiceProxyBase,
      virtual public _AttributeExtensions... {
public:
    SoftwareUpdateServiceProxy(std::shared_ptr<CommonAPI::Proxy> delegate);
    ~SoftwareUpdateServiceProxy();

    typedef SoftwareUpdateService InterfaceType;


    /**
     * description: Software package installation progress information.
     */
    /**
     * Returns the wrapper class that provides access to the attribute installProgress.
     */
    virtual InstallProgressAttribute& getInstallProgressAttribute() {
        return delegate_->getInstallProgressAttribute();
    }
    /**
     * description: Software package download progress information.
     */
    /**
     * Returns the wrapper class that provides access to the attribute downloadProgress.
     */
    virtual DownloadProgressAttribute& getDownloadProgressAttribute() {
        return delegate_->getDownloadProgressAttribute();
    }
    /**
     * description: activation status of previously installed software.
     */
    /**
     * Returns the wrapper class that provides access to the attribute activationResult.
     */
    virtual ActivationResultAttribute& getActivationResultAttribute() {
        return delegate_->getActivationResultAttribute();
    }
    /**
     * description: icon to display status of lastvsoftware update.
        	It has to be shown for two
     *   cycles post update.
     */
    /**
     * Returns the wrapper class that provides access to the attribute systemUpdateIconStatus.
     */
    virtual SystemUpdateIconStatusAttribute& getSystemUpdateIconStatusAttribute() {
        return delegate_->getSystemUpdateIconStatusAttribute();
    }

    /**
     * description: Broadcast signals for indication of OTA package download or install complete or
     *   failed.
     */
    /**
     * Returns the wrapper class that provides access to the broadcast SoftwareUpdateStatus.
     */
    virtual SoftwareUpdateStatusEvent& getSoftwareUpdateStatusEvent() {
        return delegate_->getSoftwareUpdateStatusEvent();
    }

    /**
     * description: Method to start software update.
     * Calls StartUpdate with synchronous semantics.
     * 
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void StartUpdate(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls StartUpdate with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> StartUpdateAsync(StartUpdateAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: To get when last update was checked. The information will include date and
     *   time. The output
    					will be in format Month DD, YYYY at Time
     * Calls GetLastCheckedForUpdate with synchronous semantics.
     * 
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void GetLastCheckedForUpdate(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, std::string &_lastUpdate, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls GetLastCheckedForUpdate with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> GetLastCheckedForUpdateAsync(GetLastCheckedForUpdateAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: Method to Scan For Updates.
     * Calls ScanForUpdates with synchronous semantics.
     * 
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void ScanForUpdates(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, ::v1::com::harman::swdl::SoftwareUpdateTypes::tUpdateSearchResult &_checkResult, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls ScanForUpdates with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> ScanForUpdatesAsync(ScanForUpdatesAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: To enable disable automatic download. true for enabling automatic download and
     *   false for disabling.
     * Calls SetAutomaticDownload with synchronous semantics.
     * 
    * All const parameters are input parameters to this method.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void SetAutomaticDownload(const bool &_enableAutoDownload, CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls SetAutomaticDownload with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> SetAutomaticDownloadAsync(const bool &_enableAutoDownload, SetAutomaticDownloadAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * description: To get the enable/disable status of automatic download.
     * Calls GetAutomaticDownloadStatus with synchronous semantics.
     * 
    * All non-const parameters will be filled with the returned values.
     * The CallStatus will be filled when the method returns and indicate either
     * "SUCCESS" or which type of error has occurred. In case of an error, ONLY the CallStatus
     * will be set.
     */
    virtual void GetAutomaticDownloadStatus(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, bool &_enableAutoDownload, const CommonAPI::CallInfo *_info = nullptr);
    /**
     * Calls GetAutomaticDownloadStatus with asynchronous semantics.
     * 
     * The provided callback will be called when the reply to this call arrives or
     * an error occurs during the call. The CallStatus will indicate either "SUCCESS"
     * or which type of error has occurred. In case of any error, ONLY the CallStatus
     * will have a defined value.
     * The std::future returned by this method will be fulfilled at arrival of the reply.
     * It will provide the same value for CallStatus as will be handed to the callback.
     */
    virtual std::future<CommonAPI::CallStatus> GetAutomaticDownloadStatusAsync(GetAutomaticDownloadStatusAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr);


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
    std::shared_ptr<SoftwareUpdateServiceProxyBase> delegate_;
};

typedef SoftwareUpdateServiceProxy<> SoftwareUpdateServiceProxyDefault;

namespace SoftwareUpdateServiceExtensions {
    /**
     * description: Software package installation progress information.
     */
    template <template <typename > class _ExtensionType>
    class InstallProgressAttributeExtension {
     public:
        typedef _ExtensionType<SoftwareUpdateServiceProxyBase::InstallProgressAttribute> extension_type;
    
        static_assert(std::is_base_of<typename CommonAPI::AttributeExtension<SoftwareUpdateServiceProxyBase::InstallProgressAttribute>, extension_type>::value,
                      "Not CommonAPI Attribute Extension!");
    #ifdef WIN32
        InstallProgressAttributeExtension() {}
    #endif
    
        InstallProgressAttributeExtension(SoftwareUpdateServiceProxyBase& proxy): attributeExtension_(proxy.getInstallProgressAttribute()) {
        }
    
        inline extension_type& getInstallProgressAttributeExtension() {
            return attributeExtension_;
        }
    
     private:
        extension_type attributeExtension_;
    };

    /**
     * description: Software package download progress information.
     */
    template <template <typename > class _ExtensionType>
    class DownloadProgressAttributeExtension {
     public:
        typedef _ExtensionType<SoftwareUpdateServiceProxyBase::DownloadProgressAttribute> extension_type;
    
        static_assert(std::is_base_of<typename CommonAPI::AttributeExtension<SoftwareUpdateServiceProxyBase::DownloadProgressAttribute>, extension_type>::value,
                      "Not CommonAPI Attribute Extension!");
    #ifdef WIN32
        DownloadProgressAttributeExtension() {}
    #endif
    
        DownloadProgressAttributeExtension(SoftwareUpdateServiceProxyBase& proxy): attributeExtension_(proxy.getDownloadProgressAttribute()) {
        }
    
        inline extension_type& getDownloadProgressAttributeExtension() {
            return attributeExtension_;
        }
    
     private:
        extension_type attributeExtension_;
    };

    /**
     * description: activation status of previously installed software.
     */
    template <template <typename > class _ExtensionType>
    class ActivationResultAttributeExtension {
     public:
        typedef _ExtensionType<SoftwareUpdateServiceProxyBase::ActivationResultAttribute> extension_type;
    
        static_assert(std::is_base_of<typename CommonAPI::AttributeExtension<SoftwareUpdateServiceProxyBase::ActivationResultAttribute>, extension_type>::value,
                      "Not CommonAPI Attribute Extension!");
    #ifdef WIN32
        ActivationResultAttributeExtension() {}
    #endif
    
        ActivationResultAttributeExtension(SoftwareUpdateServiceProxyBase& proxy): attributeExtension_(proxy.getActivationResultAttribute()) {
        }
    
        inline extension_type& getActivationResultAttributeExtension() {
            return attributeExtension_;
        }
    
     private:
        extension_type attributeExtension_;
    };

    /**
     * description: icon to display status of lastvsoftware update.
        	It has to be shown for two
     *   cycles post update.
     */
    template <template <typename > class _ExtensionType>
    class SystemUpdateIconStatusAttributeExtension {
     public:
        typedef _ExtensionType<SoftwareUpdateServiceProxyBase::SystemUpdateIconStatusAttribute> extension_type;
    
        static_assert(std::is_base_of<typename CommonAPI::AttributeExtension<SoftwareUpdateServiceProxyBase::SystemUpdateIconStatusAttribute>, extension_type>::value,
                      "Not CommonAPI Attribute Extension!");
    #ifdef WIN32
        SystemUpdateIconStatusAttributeExtension() {}
    #endif
    
        SystemUpdateIconStatusAttributeExtension(SoftwareUpdateServiceProxyBase& proxy): attributeExtension_(proxy.getSystemUpdateIconStatusAttribute()) {
        }
    
        inline extension_type& getSystemUpdateIconStatusAttributeExtension() {
            return attributeExtension_;
        }
    
     private:
        extension_type attributeExtension_;
    };

} // namespace SoftwareUpdateServiceExtensions

//
// SoftwareUpdateServiceProxy Implementation
//
template <typename ... _AttributeExtensions>
SoftwareUpdateServiceProxy<_AttributeExtensions...>::SoftwareUpdateServiceProxy(std::shared_ptr<CommonAPI::Proxy> delegate):
        _AttributeExtensions(*(std::dynamic_pointer_cast<SoftwareUpdateServiceProxyBase>(delegate)))...,
        delegate_(std::dynamic_pointer_cast<SoftwareUpdateServiceProxyBase>(delegate)) {
}

template <typename ... _AttributeExtensions>
SoftwareUpdateServiceProxy<_AttributeExtensions...>::~SoftwareUpdateServiceProxy() {
}

/**
 * description: Method to start software update.
 */
template <typename ... _AttributeExtensions>
void SoftwareUpdateServiceProxy<_AttributeExtensions...>::StartUpdate(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, const CommonAPI::CallInfo *_info) {
    delegate_->StartUpdate(_internalCallStatus, _error, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> SoftwareUpdateServiceProxy<_AttributeExtensions...>::StartUpdateAsync(StartUpdateAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->StartUpdateAsync(_callback, _info);
}
/**
 * description: To get when last update was checked. The information will include date and
 *   time. The output
					will be in format Month DD, YYYY at Time
 */
template <typename ... _AttributeExtensions>
void SoftwareUpdateServiceProxy<_AttributeExtensions...>::GetLastCheckedForUpdate(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, std::string &_lastUpdate, const CommonAPI::CallInfo *_info) {
    delegate_->GetLastCheckedForUpdate(_internalCallStatus, _error, _lastUpdate, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> SoftwareUpdateServiceProxy<_AttributeExtensions...>::GetLastCheckedForUpdateAsync(GetLastCheckedForUpdateAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->GetLastCheckedForUpdateAsync(_callback, _info);
}
/**
 * description: Method to Scan For Updates.
 */
template <typename ... _AttributeExtensions>
void SoftwareUpdateServiceProxy<_AttributeExtensions...>::ScanForUpdates(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, ::v1::com::harman::swdl::SoftwareUpdateTypes::tUpdateSearchResult &_checkResult, const CommonAPI::CallInfo *_info) {
    delegate_->ScanForUpdates(_internalCallStatus, _error, _checkResult, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> SoftwareUpdateServiceProxy<_AttributeExtensions...>::ScanForUpdatesAsync(ScanForUpdatesAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->ScanForUpdatesAsync(_callback, _info);
}
/**
 * description: To enable disable automatic download. true for enabling automatic download and
 *   false for disabling.
 */
template <typename ... _AttributeExtensions>
void SoftwareUpdateServiceProxy<_AttributeExtensions...>::SetAutomaticDownload(const bool &_enableAutoDownload, CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, const CommonAPI::CallInfo *_info) {
    delegate_->SetAutomaticDownload(_enableAutoDownload, _internalCallStatus, _error, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> SoftwareUpdateServiceProxy<_AttributeExtensions...>::SetAutomaticDownloadAsync(const bool &_enableAutoDownload, SetAutomaticDownloadAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->SetAutomaticDownloadAsync(_enableAutoDownload, _callback, _info);
}
/**
 * description: To get the enable/disable status of automatic download.
 */
template <typename ... _AttributeExtensions>
void SoftwareUpdateServiceProxy<_AttributeExtensions...>::GetAutomaticDownloadStatus(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, bool &_enableAutoDownload, const CommonAPI::CallInfo *_info) {
    delegate_->GetAutomaticDownloadStatus(_internalCallStatus, _error, _enableAutoDownload, _info);
}

template <typename ... _AttributeExtensions>
std::future<CommonAPI::CallStatus> SoftwareUpdateServiceProxy<_AttributeExtensions...>::GetAutomaticDownloadStatusAsync(GetAutomaticDownloadStatusAsyncCallback _callback, const CommonAPI::CallInfo *_info) {
    return delegate_->GetAutomaticDownloadStatusAsync(_callback, _info);
}

template <typename ... _AttributeExtensions>
const CommonAPI::Address &SoftwareUpdateServiceProxy<_AttributeExtensions...>::getAddress() const {
    return delegate_->getAddress();
}

template <typename ... _AttributeExtensions>
bool SoftwareUpdateServiceProxy<_AttributeExtensions...>::isAvailable() const {
    return delegate_->isAvailable();
}

template <typename ... _AttributeExtensions>
bool SoftwareUpdateServiceProxy<_AttributeExtensions...>::isAvailableBlocking() const {
    return delegate_->isAvailableBlocking();
}

template <typename ... _AttributeExtensions>
CommonAPI::ProxyStatusEvent& SoftwareUpdateServiceProxy<_AttributeExtensions...>::getProxyStatusEvent() {
    return delegate_->getProxyStatusEvent();
}

template <typename ... _AttributeExtensions>
CommonAPI::InterfaceVersionAttribute& SoftwareUpdateServiceProxy<_AttributeExtensions...>::getInterfaceVersionAttribute() {
    return delegate_->getInterfaceVersionAttribute();
}


} // namespace swdl
} // namespace harman
} // namespace com
} // namespace v0

namespace CommonAPI {
template<template<typename > class _AttributeExtension>
struct DefaultAttributeProxyHelper<::v0::com::harman::swdl::SoftwareUpdateServiceProxy,
    _AttributeExtension> {
    typedef typename ::v0::com::harman::swdl::SoftwareUpdateServiceProxy<
            ::v0::com::harman::swdl::SoftwareUpdateServiceExtensions::InstallProgressAttributeExtension<_AttributeExtension>, 
            ::v0::com::harman::swdl::SoftwareUpdateServiceExtensions::DownloadProgressAttributeExtension<_AttributeExtension>, 
            ::v0::com::harman::swdl::SoftwareUpdateServiceExtensions::ActivationResultAttributeExtension<_AttributeExtension>, 
            ::v0::com::harman::swdl::SoftwareUpdateServiceExtensions::SystemUpdateIconStatusAttributeExtension<_AttributeExtension>
    > class_t;
};
}


// Compatibility
namespace v0_2 = v0;

#endif // V0_COM_HARMAN_SWDL_Software_Update_Service_PROXY_HPP_
