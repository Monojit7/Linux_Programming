include_directories(./dbus/consumes)




add_library(mmprestest-dbus
SHARED 
dbus/consumes/v1/com/harman/mmpres/MediaPlayerDBusProxy.cpp
dbus/consumes/v1/com/harman/mmpres/MediaPlayerDBusDeployment.cpp
dbus/consumes/v3/com/harman/mmpres/MMPresTypesDBusDeployment.cpp
dbus/consumes/v3/com/harman/mmpres/MediaBrowserDBusProxy.cpp
dbus/consumes/v3/com/harman/mmpres/MediaBrowserDBusDeployment.cpp
dbus/consumes/v3/com/harman/mmpres/MMPresTypesDBusDeployment.cpp
dbus/consumes/v1/org/genivi/am/routinginterface/RoutingControlDBusProxy.cpp
dbus/consumes/v1/org/genivi/am/routinginterface/RoutingControlDBusDeployment.cpp
dbus/consumes/v3/org/genivi/am_tDBusDeployment.cpp
dbus/consumes/v1/com/harman/mmpres/MediaSettingsDBusProxy.cpp
dbus/consumes/v1/com/harman/mmpres/MediaSettingsDBusDeployment.cpp
dbus/consumes/v3/com/harman/mmpres/MMPresTypesDBusDeployment.cpp
dbus/consumes/v1/com/harman/mmpres/MediaManagerDBusProxy.cpp
dbus/consumes/v1/com/harman/mmpres/MediaManagerDBusDeployment.cpp
dbus/consumes/v3/com/harman/mmpres/MMPresTypesDBusDeployment.cpp
)
target_link_libraries(mmprestest-dbus CommonAPI CommonAPI-DBus)

