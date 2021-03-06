/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V0_ORG_HARMAN_FORD_notifalert_PROXY_BASE_HPP_
#define V0_ORG_HARMAN_FORD_notifalert_PROXY_BASE_HPP_

#include <v0/org/harman/ford/notifalert.hpp>


#include <v0/org/harman/ford/notifalert_types.hpp>

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/Deployment.hpp>
#include <CommonAPI/InputStream.hpp>
#include <CommonAPI/OutputStream.hpp>
#include <CommonAPI/Struct.hpp>
#include <cstdint>

#include <CommonAPI/Attribute.hpp>
#include <CommonAPI/Proxy.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v0 {
namespace org {
namespace harman {
namespace ford {

class notifalertProxyBase
    : virtual public CommonAPI::Proxy {
public:
    typedef CommonAPI::ObservableAttribute<::v0::org::harman::ford::notifalert_types::CCOI_PolicyClient_St_enum> CCOI_PolicyClient_St_enumAttribute;
    typedef CommonAPI::ObservableAttribute<::v0::org::harman::ford::notifalert_types::AHU_Chime_Not_Recgnizd_enum> AHU_Chime_Not_Recgnizd_enumAttribute;
    typedef CommonAPI::ObservableAttribute<::v0::org::harman::ford::notifalert_types::AHU_Chime2_Not_Recgnizd_enum> AHU_Chime2_Not_Recgnizd_enumAttribute;
    typedef CommonAPI::ObservableAttribute<::v0::org::harman::ford::notifalert_types::AHU_Chime_Supported_enum> AHU_Chime_Supported_enumAttribute;
    typedef CommonAPI::ObservableAttribute<::v0::org::harman::ford::notifalert_types::AHU_Chime_Active_enum> AHU_Chime_Active_enumAttribute;
    typedef CommonAPI::ObservableAttribute<::v0::org::harman::ford::notifalert_types::AHU_Chime2_Active_enum> AHU_Chime2_Active_enumAttribute;
    typedef CommonAPI::ObservableAttribute<::v0::org::harman::ford::notifalert_types::CDWarnChime_St_enum> CDWarnChime_St_enumAttribute;
    typedef CommonAPI::ObservableAttribute<::v0::org::harman::ford::notifalert_types::CDWarnChime_Rq_enum> CDWarnChime_Rq_enumAttribute;
    typedef CommonAPI::ObservableReadonlyAttribute<::v0::org::harman::ford::notifalert_types::IPC_Chime_struct> IPC_ChimeAttribute;
    typedef CommonAPI::ObservableReadonlyAttribute<::v0::org::harman::ford::notifalert_types::IPC_Chime2_struct> IPC_Chime2Attribute;
    typedef CommonAPI::ObservableReadonlyAttribute<::v0::org::harman::ford::notifalert_types::GWM_Send_Signals_8_HS3_sig_4_struct> GWM_Send_Signals_8_HS3_sig_4Attribute;


    /**
     * description: No Information Available
     */
    virtual CCOI_PolicyClient_St_enumAttribute& getCCOI_PolicyClient_St_enumAttribute() = 0;
    /**
     * description: No Information Available
     */
    virtual AHU_Chime_Not_Recgnizd_enumAttribute& getAHU_Chime_Not_Recgnizd_enumAttribute() = 0;
    /**
     * description: No Information Available
     */
    virtual AHU_Chime2_Not_Recgnizd_enumAttribute& getAHU_Chime2_Not_Recgnizd_enumAttribute() = 0;
    /**
     * description: No Information Available
     */
    virtual AHU_Chime_Supported_enumAttribute& getAHU_Chime_Supported_enumAttribute() = 0;
    /**
     * description: No Information Available
     */
    virtual AHU_Chime_Active_enumAttribute& getAHU_Chime_Active_enumAttribute() = 0;
    /**
     * description: No Information Available
     */
    virtual AHU_Chime2_Active_enumAttribute& getAHU_Chime2_Active_enumAttribute() = 0;
    /**
     * description: No Information Available
     */
    virtual CDWarnChime_St_enumAttribute& getCDWarnChime_St_enumAttribute() = 0;
    /**
     * description: No Information Available
     */
    virtual CDWarnChime_Rq_enumAttribute& getCDWarnChime_Rq_enumAttribute() = 0;
    virtual IPC_ChimeAttribute& getIPC_ChimeAttribute() = 0;
    virtual IPC_Chime2Attribute& getIPC_Chime2Attribute() = 0;
    virtual GWM_Send_Signals_8_HS3_sig_4Attribute& getGWM_Send_Signals_8_HS3_sig_4Attribute() = 0;


};

} // namespace ford
} // namespace harman
} // namespace org
} // namespace v0


// Compatibility
namespace v0_1 = v0;

#endif // V0_ORG_HARMAN_FORD_notifalert_PROXY_BASE_HPP_
