/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V0_COM_HARMAN_SWDL_Software_Update_Service_PROXY_BASE_HPP_
#define V0_COM_HARMAN_SWDL_Software_Update_Service_PROXY_BASE_HPP_

#include <v0/com/harman/swdl/SoftwareUpdateService.hpp>


#include <v1/com/harman/swdl/SoftwareUpdateTypes.hpp>

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/Deployment.hpp>
#include <CommonAPI/InputStream.hpp>
#include <CommonAPI/OutputStream.hpp>
#include <CommonAPI/Struct.hpp>
#include <cstdint>
#include <string>
#include <vector>

#include <CommonAPI/Attribute.hpp>
#include <CommonAPI/Event.hpp>
#include <CommonAPI/Proxy.hpp>
#include <functional>
#include <future>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v0 {
namespace com {
namespace harman {
namespace swdl {

class SoftwareUpdateServiceProxyBase
    : virtual public CommonAPI::Proxy {
public:
    typedef CommonAPI::ObservableAttribute<::v1::com::harman::swdl::SoftwareUpdateTypes::tInstallProgress> InstallProgressAttribute;
    typedef CommonAPI::ObservableAttribute<::v1::com::harman::swdl::SoftwareUpdateTypes::tDownloadProgress> DownloadProgressAttribute;
    typedef CommonAPI::ObservableAttribute<::v1::com::harman::swdl::SoftwareUpdateTypes::tActivationResult> ActivationResultAttribute;
    typedef CommonAPI::ObservableAttribute<bool> SystemUpdateIconStatusAttribute;
    typedef CommonAPI::Event<
        ::v1::com::harman::swdl::SoftwareUpdateTypes::tStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eUpdateSource
    > SoftwareUpdateStatusEvent;

    typedef std::function<void(const CommonAPI::CallStatus&, const ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult&)> StartUpdateAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult&, const std::string&)> GetLastCheckedForUpdateAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult&, const ::v1::com::harman::swdl::SoftwareUpdateTypes::tUpdateSearchResult&)> ScanForUpdatesAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult&)> SetAutomaticDownloadAsyncCallback;
    typedef std::function<void(const CommonAPI::CallStatus&, const ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult&, const bool&)> GetAutomaticDownloadStatusAsyncCallback;

    /**
     * description: Software package installation progress information.
     */
    virtual InstallProgressAttribute& getInstallProgressAttribute() = 0;
    /**
     * description: Software package download progress information.
     */
    virtual DownloadProgressAttribute& getDownloadProgressAttribute() = 0;
    /**
     * description: activation status of previously installed software.
     */
    virtual ActivationResultAttribute& getActivationResultAttribute() = 0;
    /**
     * description: icon to display status of lastvsoftware update.
        	It has to be shown for two
     *   cycles post update.
     */
    virtual SystemUpdateIconStatusAttribute& getSystemUpdateIconStatusAttribute() = 0;

    /**
     * description: Broadcast signals for indication of OTA package download or install complete or
     *   failed.
     */
    virtual SoftwareUpdateStatusEvent& getSoftwareUpdateStatusEvent() = 0;

    /**
     * description: Method to start software update.
     */
    virtual void StartUpdate(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> StartUpdateAsync(StartUpdateAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: To get when last update was checked. The information will include date and
     *   time. The output
    					will be in format Month DD, YYYY at Time
     */
    virtual void GetLastCheckedForUpdate(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, std::string &_lastUpdate, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> GetLastCheckedForUpdateAsync(GetLastCheckedForUpdateAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: Method to Scan For Updates.
     */
    virtual void ScanForUpdates(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, ::v1::com::harman::swdl::SoftwareUpdateTypes::tUpdateSearchResult &_checkResult, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> ScanForUpdatesAsync(ScanForUpdatesAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: To enable disable automatic download. true for enabling automatic download and
     *   false for disabling.
     */
    virtual void SetAutomaticDownload(const bool &_enableAutoDownload, CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> SetAutomaticDownloadAsync(const bool &_enableAutoDownload, SetAutomaticDownloadAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
    /**
     * description: To get the enable/disable status of automatic download.
     */
    virtual void GetAutomaticDownloadStatus(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, bool &_enableAutoDownload, const CommonAPI::CallInfo *_info = nullptr) = 0;
    virtual std::future<CommonAPI::CallStatus> GetAutomaticDownloadStatusAsync(GetAutomaticDownloadStatusAsyncCallback _callback = nullptr, const CommonAPI::CallInfo *_info = nullptr) = 0;
};

} // namespace swdl
} // namespace harman
} // namespace com
} // namespace v0


// Compatibility
namespace v0_2 = v0;

#endif // V0_COM_HARMAN_SWDL_Software_Update_Service_PROXY_BASE_HPP_
