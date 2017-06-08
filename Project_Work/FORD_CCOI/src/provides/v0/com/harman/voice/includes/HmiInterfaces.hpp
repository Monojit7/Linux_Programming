#ifndef V0_COM_HARMAN_VOICE_HMIINTERFACES_HPP
#define V0_COM_HARMAN_VOICE_HMIINTERFACES_HPP

#include <thread>
#include <unistd.h>
#include <CommonAPI/CommonAPI.hpp>

#undef STATIC   ///work around for the STATIC definition done in DLT and HMI fidl - both are conflicting with each other
#include "v1/com/harman/hmiDialogueService/hmiDialogueServiceInterface.hpp"
#include "v1/com/harman/hmiDialogueService/hmiDialogueServiceInterfaceProxy.hpp"
#include "com/harman/hmiDialogueServiceTypes/hmiDialogueServiceTypes.hpp"

#endif

