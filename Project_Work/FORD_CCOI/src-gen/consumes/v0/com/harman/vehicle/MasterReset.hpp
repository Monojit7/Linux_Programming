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
 * description: Vehicle Settings interface for Master Reset functionality.
 * author: Yogesh Sharma
 */
#ifndef V0_COM_HARMAN_VEHICLE_MASTER_RESET_HPP_
#define V0_COM_HARMAN_VEHICLE_MASTER_RESET_HPP_




#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/Types.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v0 {
namespace com {
namespace harman {
namespace vehicle {

class MasterReset {
public:
    virtual ~MasterReset() { }

    static inline const char* getInterface();
    static inline CommonAPI::Version getInterfaceVersion();
};

const char* MasterReset::getInterface() {
    return ("com.harman.vehicle.MasterReset");
}

CommonAPI::Version MasterReset::getInterfaceVersion() {
    return CommonAPI::Version(0, 1);
}


} // namespace vehicle
} // namespace harman
} // namespace com
} // namespace v0

namespace CommonAPI {
}


// Compatibility
namespace v0_1 = v0;

#endif // V0_COM_HARMAN_VEHICLE_MASTER_RESET_HPP_
