#pragma once

// plugin interface
#include <core/IPluginBase.h>

// prescore helpers
#include <core/CommonAPIClientAbstraction.h>
#include <core/PresEventAbstraction.h>
#include <core/ITimerEventListener.h>
#include <core/CTimerInterface.h>

// proxy interfaces
// commonAPI
#include <CommonAPI/CommonAPI.hpp>

// Header required for persistency and system config requirement 
#include <persistence_client_library_file.h>
#include <persistence_client_library_key.h>
#include <persistence_client_library.h>
#include <SysetIds.h>

// mediaplayback
#include "autogencode/src-gen/consumes/v0/org/harman/ford/mediaplaybackProxy.hpp"


// namespace declaratives
using namespace std ;
using namespace v0::org::harman::ford ;

