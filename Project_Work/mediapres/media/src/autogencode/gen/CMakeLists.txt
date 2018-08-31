include_directories(./dbus/provides)
include_directories(./dbus/consumes)



add_library(mmpres-dbus
SHARED 
dbus/provides/v1/com/harman/mmpres/MediaPlayerDBusStubAdapter.cpp
dbus/provides/v1/com/harman/mmpres/MediaPlayerDBusDeployment.cpp
dbus/provides/v3/com/harman/mmpres/MediaBrowserDBusStubAdapter.cpp
dbus/provides/v3/com/harman/mmpres/MediaBrowserDBusDeployment.cpp
dbus/provides/v1/org/genivi/am/routinginterface/RoutingControlDBusStubAdapter.cpp
dbus/provides/v1/org/genivi/am/routinginterface/RoutingControlDBusDeployment.cpp
dbus/provides/v3/org/genivi/am_tDBusDeployment.cpp
dbus/provides/v1/com/harman/mmpres/MediaSettingsDBusStubAdapter.cpp
dbus/provides/v1/com/harman/mmpres/MediaSettingsDBusDeployment.cpp
dbus/provides/v3/com/harman/mmpres/MMPresTypesDBusDeployment.cpp
dbus/provides/v1/com/harman/mmpres/MediaManagerDBusStubAdapter.cpp
dbus/provides/v1/com/harman/mmpres/MediaManagerDBusDeployment.cpp
dbus/consumes/v1/com/harman/media/ImageProcessDBusProxy.cpp
dbus/consumes/v1/com/harman/media/ImageProcessDBusDeployment.cpp
dbus/consumes/v1/org/genivi/mediamanager/BrowserDBusProxy.cpp
dbus/consumes/v1/org/genivi/mediamanager/BrowserDBusDeployment.cpp
dbus/consumes/v1/org/genivi/mediamanager/MediaTypesDBusDeployment.cpp
dbus/consumes/v1/org/genivi/mediamanager/IndexerDBusProxy.cpp
dbus/consumes/v1/org/genivi/mediamanager/IndexerDBusDeployment.cpp
dbus/consumes/v3/org/genivi/am/commandinterface/CommandControlDBusProxy.cpp
dbus/consumes/v3/org/genivi/am/commandinterface/CommandControlDBusDeployment.cpp
dbus/consumes/v1/com/harman/media/PlayerExtDBusProxy.cpp
dbus/consumes/v1/com/harman/media/PlayerExtDBusDeployment.cpp
dbus/consumes/v1/com/harman/media/BaseTypeDBusDeployment.cpp
dbus/consumes/v1/org/genivi/mediamanager/BrowserTypesDBusDeployment.cpp
dbus/consumes/v3/org/genivi/am/routinginterface/RoutingControlObserverDBusProxy.cpp
dbus/consumes/v3/org/genivi/am/routinginterface/RoutingControlObserverDBusDeployment.cpp
dbus/consumes/v3/org/genivi/am_tDBusDeployment.cpp
dbus/consumes/v4/com/harman/connectivity/BtServiceDBusProxy.cpp
dbus/consumes/v4/com/harman/connectivity/BtServiceDBusDeployment.cpp
dbus/consumes/com/harman/connectivity/btTypedefsDBusDeployment.cpp
dbus/consumes/v2/com/harman/btpres/ConnectionManagerDBusProxy.cpp
dbus/consumes/v2/com/harman/btpres/ConnectionManagerDBusDeployment.cpp
dbus/consumes/v4/com/harman/btpres/BTPresTypesDBusDeployment.cpp
dbus/consumes/v1/com/harman/audio/audioPresCtrl/audioSourceInfoDBusProxy.cpp
dbus/consumes/v1/com/harman/audio/audioPresCtrl/audioSourceInfoDBusDeployment.cpp
dbus/consumes/v3/com/harman/audio/audioPresCtrl/audioPresCtrlTypesDBusDeployment.cpp
dbus/consumes/v1/org/genivi/mediamanager/PlayerDBusProxy.cpp
dbus/consumes/v1/org/genivi/mediamanager/PlayerDBusDeployment.cpp
dbus/consumes/v1/org/genivi/mediamanager/PlayerTypesDBusDeployment.cpp
dbus/consumes/v1/com/harman/media/DeviceManagerDBusProxy.cpp
dbus/consumes/v1/com/harman/media/DeviceManagerDBusDeployment.cpp
dbus/consumes/v1/com/harman/media/BaseTypeDBusDeployment.cpp
dbus/consumes/v1/com/harman/media/MediaSettingDBusProxy.cpp
dbus/consumes/v1/com/harman/media/MediaSettingDBusDeployment.cpp
dbus/consumes/v0/org/harman/ford/mediaplaybackDBusProxy.cpp
)

target_link_libraries(mmpres-dbus CommonAPI CommonAPI-DBus)

