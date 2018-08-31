add_library( nsmconsumer-dbus
SHARED 
gen/dbus/consumes/v2/org/genivi/NodeStateManager/ConsumerDBusProxy.cpp
gen/dbus/consumes/v2/org/genivi/NodeStateManager/ConsumerDBusDeployment.cpp
gen/dbus/consumes/v2/org/genivi/NodeStateManager/NodeStateTypesDBusDeployment.cpp
)

target_link_libraries(nsmconsumer-dbus CommonAPI CommonAPI-DBus)
