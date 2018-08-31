/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.dbus .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/

#ifndef V0_ORG_HARMAN_FORD_KEY_INPUT_TYPES_DBUS_DEPLOYMENT_HPP_
#define V0_ORG_HARMAN_FORD_KEY_INPUT_TYPES_DBUS_DEPLOYMENT_HPP_

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif
#include <CommonAPI/DBus/DBusDeployment.hpp>
#undef COMMONAPI_INTERNAL_COMPILATION

namespace v0 {
namespace org {
namespace harman {
namespace ford {
namespace key_input_types_ {

// typecollection-specific deployment types
typedef CommonAPI::EmptyDeployment KeyMykeyTot_No_Cnt_enumDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment
> Personality_BCM2_Data_HS3_structDeployment_t;

typedef CommonAPI::EmptyDeployment AssocConfirm_D_Actl_enumDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> EFP_Button_Press_structDeployment_t;

typedef CommonAPI::EmptyDeployment Coding_BtnID_A_enumDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> MC_Send_Signals_1_structDeployment_t;

typedef CommonAPI::EmptyDeployment ICI_Coding_BtnID_B_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ICI_Coding_BtnID_A_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ICI_Coding_BtnID_D_enumDeployment_t;

typedef CommonAPI::EmptyDeployment ICI_Coding_BtnID_C_enumDeployment_t;

typedef CommonAPI::DBus::StructDeployment<
    CommonAPI::EmptyDeployment,
    CommonAPI::EmptyDeployment
> EFP2_Button_Press_structDeployment_t;

typedef CommonAPI::EmptyDeployment Coding_BtnID_A_EFP2_enumDeployment_t;

typedef CommonAPI::EmptyDeployment KeyOffMde_D_Actl_enumDeployment_t;


// typecollection-specific deployments

} // namespace key_input_types_
} // namespace ford
} // namespace harman
} // namespace org
} // namespace v0

#endif // V0_ORG_HARMAN_FORD_KEY_INPUT_TYPES_DBUS_DEPLOYMENT_HPP_
