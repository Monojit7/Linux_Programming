/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/

#ifndef V1_COM_HARMAN_VEHICLE_VEHICLE_TYPE_DBUS_DEPLOYMENT_HPP_
#define V1_COM_HARMAN_VEHICLE_VEHICLE_TYPE_DBUS_DEPLOYMENT_HPP_

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif
#include <CommonAPI/DBus/DBusDeployment.hpp>
#undef COMMONAPI_INTERNAL_COMPILATION

namespace v1 {
namespace com {
namespace harman {
namespace vehicle {
namespace VehicleType_ {

// typecollection-specific deployment types
typedef CommonAPI::EmptyDeployment eBezelBeepsDeployment_t;

typedef CommonAPI::EmptyDeployment eDisplayLangDeployment_t;

typedef CommonAPI::EmptyDeployment eDisplayLangResponseDeployment_t;

typedef CommonAPI::EmptyDeployment eDistanceUnitsDeployment_t;

typedef CommonAPI::EmptyDeployment eTempreatureUnitsDeployment_t;

typedef CommonAPI::EmptyDeployment eTimeFormatDeployment_t;

typedef CommonAPI::EmptyDeployment eAmbientLightingColorDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> AmbientLightSettingDeployment_t;

typedef CommonAPI::DBus::ArrayDeployment<
    CommonAPI::DBus::StructDeployment<
        CommonAPI::EmptyDeployment,
        CommonAPI::EmptyDeployment
    >
> ListAmbLightSettingsDeployment_t;

typedef CommonAPI::EmptyDeployment eWirelessChargingStatusDeployment_t;

typedef CommonAPI::EmptyDeployment EGenericVehRespDeployment_t;

typedef CommonAPI::EmptyDeployment eCountryRegionDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment,
    CommonAPI::DBus::StringDeployment
> VersionInfoListDeployment_t;


// typecollection-specific deployments

} // namespace VehicleType_
} // namespace vehicle
} // namespace harman
} // namespace com
} // namespace v1

#endif // V1_COM_HARMAN_VEHICLE_VEHICLE_TYPE_DBUS_DEPLOYMENT_HPP_
