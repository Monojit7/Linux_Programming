/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core .
* Used org.franca.core 0.9.1.201412191134.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V2_COM_HARMAN_BTPRES_CONNECTION_MANAGER_HPP_
#define V2_COM_HARMAN_BTPRES_CONNECTION_MANAGER_HPP_




#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/Types.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v2 {
namespace com {
namespace harman {
namespace btpres {

class ConnectionManager {
public:
    virtual ~ConnectionManager() { }

    static inline const char* getInterface();
    static inline CommonAPI::Version getInterfaceVersion();
};

const char* ConnectionManager::getInterface() {
    return ("com.harman.btpres.ConnectionManager");
}

CommonAPI::Version ConnectionManager::getInterfaceVersion() {
    return CommonAPI::Version(2, 0);
}


} // namespace btpres
} // namespace harman
} // namespace com
} // namespace v2

namespace CommonAPI {
}


// Compatibility
namespace v2_0 = v2;

#endif // V2_COM_HARMAN_BTPRES_CONNECTION_MANAGER_HPP_
