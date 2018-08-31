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
 * description: This is a Common API DBUS control interface of audio presentation controller
 *   for Orinoco 2.0
 * author: AYatheesh
 */
#ifndef V2_COM_HARMAN_AUDIO_AUDIOPRESCTRL_AUDIO_PRES_CTRL_CONTROLS_HPP_
#define V2_COM_HARMAN_AUDIO_AUDIOPRESCTRL_AUDIO_PRES_CTRL_CONTROLS_HPP_




#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/Types.hpp>

#undef COMMONAPI_INTERNAL_COMPILATION

namespace v2 {
namespace com {
namespace harman {
namespace audio {
namespace audioPresCtrl {

class audioPresCtrlControls {
public:
    virtual ~audioPresCtrlControls() { }

    static inline const char* getInterface();
    static inline CommonAPI::Version getInterfaceVersion();
};

const char* audioPresCtrlControls::getInterface() {
    return ("com.harman.audio.audioPresCtrl.audioPresCtrlControls");
}

CommonAPI::Version audioPresCtrlControls::getInterfaceVersion() {
    return CommonAPI::Version(2, 2);
}


} // namespace audioPresCtrl
} // namespace audio
} // namespace harman
} // namespace com
} // namespace v2

namespace CommonAPI {
}


// Compatibility
namespace v2_2 = v2;

#endif // V2_COM_HARMAN_AUDIO_AUDIOPRESCTRL_AUDIO_PRES_CTRL_CONTROLS_HPP_
