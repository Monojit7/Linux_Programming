/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V0_COM_HARMAN_SWDL_Software_Update_Service_DBUS_PROXY_HPP_
#define V0_COM_HARMAN_SWDL_Software_Update_Service_DBUS_PROXY_HPP_

#include <v0/com/harman/swdl/SoftwareUpdateServiceProxyBase.hpp>
#include "v0/com/harman/swdl/SoftwareUpdateServiceDBusDeployment.hpp"

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/DBus/DBusAddress.hpp>
#include <CommonAPI/DBus/DBusFactory.hpp>
#include <CommonAPI/DBus/DBusProxy.hpp>
#include <CommonAPI/DBus/DBusAddressTranslator.hpp>
#include <CommonAPI/DBus/DBusAttribute.hpp>
#include <CommonAPI/DBus/DBusEvent.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

#include <string>

namespace v0 {
namespace com {
namespace harman {
namespace swdl {

class SoftwareUpdateServiceDBusProxy
    : virtual public SoftwareUpdateServiceProxyBase, 
      virtual public CommonAPI::DBus::DBusProxy {
public:
    SoftwareUpdateServiceDBusProxy(
        const CommonAPI::DBus::DBusAddress &_address,
        const std::shared_ptr<CommonAPI::DBus::DBusProxyConnection> &_connection);

    virtual ~SoftwareUpdateServiceDBusProxy() { }

    virtual InstallProgressAttribute& getInstallProgressAttribute();
    virtual DownloadProgressAttribute& getDownloadProgressAttribute();
    virtual ActivationResultAttribute& getActivationResultAttribute();
    virtual SystemUpdateIconStatusAttribute& getSystemUpdateIconStatusAttribute();

    virtual SoftwareUpdateStatusEvent& getSoftwareUpdateStatusEvent();

    /**
     * description: Method to start software update.
     */
    virtual void StartUpdate(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> StartUpdateAsync(StartUpdateAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: To get when last update was checked. The information will include date and
     *   time. The output
    					will be in format Month DD, YYYY at Time
     */
    virtual void GetLastCheckedForUpdate(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, std::string &_lastUpdate, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> GetLastCheckedForUpdateAsync(GetLastCheckedForUpdateAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: Method to Scan For Updates.
     */
    virtual void ScanForUpdates(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, ::v1::com::harman::swdl::SoftwareUpdateTypes::tUpdateSearchResult &_checkResult, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> ScanForUpdatesAsync(ScanForUpdatesAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: To enable disable automatic download. true for enabling automatic download and
     *   false for disabling.
     */
    virtual void SetAutomaticDownload(const bool &_enableAutoDownload, CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> SetAutomaticDownloadAsync(const bool &_enableAutoDownload, SetAutomaticDownloadAsyncCallback _callback, const CommonAPI::CallInfo *_info);
    /**
     * description: To get the enable/disable status of automatic download.
     */
    virtual void GetAutomaticDownloadStatus(CommonAPI::CallStatus &_internalCallStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes::eMethodResult &_error, bool &_enableAutoDownload, const CommonAPI::CallInfo *_info);
    virtual std::future<CommonAPI::CallStatus> GetAutomaticDownloadStatusAsync(GetAutomaticDownloadStatusAsyncCallback _callback, const CommonAPI::CallInfo *_info);


    virtual void getOwnVersion(uint16_t& ownVersionMajor, uint16_t& ownVersionMinor) const;

private:
   
    CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusAttribute<InstallProgressAttribute, ::v1::com::harman::swdl::SoftwareUpdateTypes_::tInstallProgressDeployment_t>> installProgress_;
    CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusAttribute<DownloadProgressAttribute, ::v1::com::harman::swdl::SoftwareUpdateTypes_::tDownloadProgressDeployment_t>> downloadProgress_;
    CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusAttribute<ActivationResultAttribute, ::v1::com::harman::swdl::SoftwareUpdateTypes_::tActivationResultDeployment_t>> activationResult_;
    CommonAPI::DBus::DBusObservableAttribute<CommonAPI::DBus::DBusAttribute<SystemUpdateIconStatusAttribute>> systemUpdateIconStatus_;

    CommonAPI::DBus::DBusEvent<SoftwareUpdateStatusEvent, CommonAPI::Deployable<::v1::com::harman::swdl::SoftwareUpdateTypes::tStatus, ::v1::com::harman::swdl::SoftwareUpdateTypes_::tStatusDeployment_t>, CommonAPI::Deployable<::v1::com::harman::swdl::SoftwareUpdateTypes::eUpdateSource, CommonAPI::EmptyDeployment>> softwareUpdateStatus_;

};

} // namespace swdl
} // namespace harman
} // namespace com
} // namespace v0

#endif // V0_COM_HARMAN_SWDL_Software_Update_Service_DBUS_PROXY_HPP_

