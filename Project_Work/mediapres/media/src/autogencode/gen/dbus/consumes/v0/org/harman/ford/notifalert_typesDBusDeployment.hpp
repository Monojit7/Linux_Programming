/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/

#ifndef V0_ORG_HARMAN_FORD_NOTIFALERT_TYPES_DBUS_DEPLOYMENT_HPP_
#define V0_ORG_HARMAN_FORD_NOTIFALERT_TYPES_DBUS_DEPLOYMENT_HPP_

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif
#include <CommonAPI/DBus/DBusDeployment.hpp>
#undef COMMONAPI_INTERNAL_COMPILATION

namespace v0 {
namespace org {
namespace harman {
namespace ford {
namespace notifalert_types_ {

// typecollection-specific deployment types
typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> CtaAlrtRight_D_Stat_structDeployment_t;

typedef CommonAPI::EmptyDeployment CCOI_PolicyClient_St_enumDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> IPC_Chime_structDeployment_t;

typedef CommonAPI::EmptyDeployment Chime_Time_Criticality_enumDeployment_t;

typedef CommonAPI::EmptyDeployment Chime_Directionality_enumDeployment_t;

typedef CommonAPI::EmptyDeployment Chime_enumDeployment_t;

typedef CommonAPI::EmptyDeployment Chime_Occurence_enumDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> CtaAlrtLeft_D_Stat_structDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> IPC_Chime2_structDeployment_t;

typedef CommonAPI::EmptyDeployment Chime_Time_Criticality2_enumDeployment_t;

typedef CommonAPI::EmptyDeployment Chime_Directionality2_enumDeployment_t;

typedef CommonAPI::EmptyDeployment Chime2_enumDeployment_t;

typedef CommonAPI::EmptyDeployment Chime_Occurence2_enumDeployment_t;

typedef CommonAPI::EmptyDeployment AHU_Chime_Supported_enumDeployment_t;

typedef CommonAPI::EmptyDeployment AHU_Chime_Active_enumDeployment_t;

typedef CommonAPI::EmptyDeployment AHU_Chime2_Active_enumDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> GWM_Send_Signals_8_HS3_sig_4_structDeployment_t;

typedef CommonAPI::EmptyDeployment CCOI_PolicyServer_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment CCOI_PolicyServer_St_UB_enumDeployment_t;

typedef CommonAPI::EmptyDeployment CDWarnChime_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment CDWarnChime_Rq_enumDeployment_t;


// typecollection-specific deployments

} // namespace notifalert_types_
} // namespace ford
} // namespace harman
} // namespace org
} // namespace v0

#endif // V0_ORG_HARMAN_FORD_NOTIFALERT_TYPES_DBUS_DEPLOYMENT_HPP_
