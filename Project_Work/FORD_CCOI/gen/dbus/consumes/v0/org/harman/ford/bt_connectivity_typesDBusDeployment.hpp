/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/

#ifndef V0_ORG_HARMAN_FORD_BT_CONNECTIVITY_TYPES_DBUS_DEPLOYMENT_HPP_
#define V0_ORG_HARMAN_FORD_BT_CONNECTIVITY_TYPES_DBUS_DEPLOYMENT_HPP_

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif
#include <CommonAPI/DBus/DBusDeployment.hpp>
#undef COMMONAPI_INTERNAL_COMPILATION

namespace v0 {
namespace org {
namespace harman {
namespace ford {
namespace bt_connectivity_types_ {

// typecollection-specific deployment types
typedef CommonAPI::EmptyDeployment BTInCallOptions_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment VRM_BTBatteryLevel_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment VRM_BTSignalStrength_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment VRM_BTNetworkStatus_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment VRM_BlutoothStatus_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment VRM_BTPhoneSts_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ACU_AROperationType_Rsp_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ACU_ARResponse_Rsp_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ACU_ARAudioSource_Rsp_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ACU_ARReqstrSystem_Rsp_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ACU_RUReqstrSystem_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ACU_RUResReqstStatus_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ACU_ARReqstrPriority_Rp_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ACU_RUReqstrPriority_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ACU_RUAudioSource_St_enumDeployment_t;

typedef CommonAPI::EmptyDeployment VRM_BTEndTelService_Rsp_enumDeployment_t;

typedef CommonAPI::EmptyDeployment VRM_BTIncomingCall_Rsp_enumDeployment_t;

typedef CommonAPI::EmptyDeployment Mc_PhMicrophoneMute_Rq_enumDeployment_t;

typedef CommonAPI::EmptyDeployment Mc_BTInCallOptions_Rq_enumDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> HUD_Send_Signals_2_structDeployment_t;

typedef CommonAPI::EmptyDeployment HUD_BTIncomingCall_Rq_enumDeployment_t;

typedef CommonAPI::EmptyDeployment HUD_BTEndTelService_Rq_enumDeployment_t;

typedef CommonAPI::EmptyDeployment HUD_BTInCallOptions_Rq_enumDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> HEC_DispDimm_structDeployment_t;

typedef CommonAPI::EmptyDeployment HEC_BTEndTelService_Rq_enumDeployment_t;

typedef CommonAPI::EmptyDeployment HEC_BTIncomingCall_Rq_enumDeployment_t;


// typecollection-specific deployments

} // namespace bt_connectivity_types_
} // namespace ford
} // namespace harman
} // namespace org
} // namespace v0

#endif // V0_ORG_HARMAN_FORD_BT_CONNECTIVITY_TYPES_DBUS_DEPLOYMENT_HPP_
