/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/

#ifndef V1_COM_HARMAN_MEDIA_BASE_TYPE_DBUS_DEPLOYMENT_HPP_
#define V1_COM_HARMAN_MEDIA_BASE_TYPE_DBUS_DEPLOYMENT_HPP_

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif
#include <CommonAPI/DBus/DBusDeployment.hpp>
#undef COMMONAPI_INTERNAL_COMPILATION

namespace v1 {
namespace com {
namespace harman {
namespace media {
namespace BaseType_ {

// typecollection-specific deployment types
typedef CommonAPI::EmptyDeployment tMediumTypeDeployment_t;

typedef CommonAPI::EmptyDeployment tContentTypeDeployment_t;

typedef CommonAPI::EmptyDeployment tMediumStateDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment
> tMediumInfoDeployment_t;

typedef CommonAPI::DBus::ArrayDeployment<
    CommonAPI::DBus::StructDeployment<
        CommonAPI::EmptyDeployment,
        CommonAPI::EmptyDeployment,
        CommonAPI::DBus::StringDeployment,
        CommonAPI::EmptyDeployment,
        CommonAPI::EmptyDeployment,
        CommonAPI::EmptyDeployment,
        CommonAPI::DBus::StringDeployment,
        CommonAPI::DBus::StringDeployment,
        CommonAPI::DBus::StringDeployment,
        CommonAPI::EmptyDeployment,
        CommonAPI::DBus::StringDeployment,
        CommonAPI::DBus::StringDeployment
    >
> tMediumInfoListDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> tItemsInfoDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::DBus::StringDeployment
> tRemotePlayerCapabilityDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment
> tFirmwareInfoDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment
> tHardwareInfoDeployment_t;

typedef CommonAPI::EmptyDeployment tBaseErrorDeployment_t;

typedef CommonAPI::EmptyDeployment tAudioObjectTypeDeployment_t;

typedef CommonAPI::EmptyDeployment eBTProfilesStatusDeployment_t;


// typecollection-specific deployments

} // namespace BaseType_
} // namespace media
} // namespace harman
} // namespace com
} // namespace v1

#endif // V1_COM_HARMAN_MEDIA_BASE_TYPE_DBUS_DEPLOYMENT_HPP_
