include_directories(./provides)
include_directories(./consumes)

add_library(
mmpres_mmpres_types_static_stub
STATIC
provides/v1/com/harman/mmpres/MediaPlayerStubDefault.cpp
provides/v3/com/harman/mmpres/MediaBrowserStubDefault.cpp
provides/v1/com/harman/mmpres/MediaManagerStubDefault.cpp
provides/v1/com/harman/mmpres/MediaSettingsStubDefault.cpp
provides/v1/org/genivi/am/routinginterface/RoutingControlStubDefault.cpp
)