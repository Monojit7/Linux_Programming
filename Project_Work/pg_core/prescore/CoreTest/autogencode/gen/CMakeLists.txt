include_directories(./dbus/consumes)
include_directories(../src-gen/consumes)

add_library(keyprestest-dbus
SHARED 
dbus/consumes/v1/com/harman/pres/keypresDBusProxy.cpp
dbus/consumes/v1/com/harman/pres/keypresDBusDeployment.cpp
dbus/consumes/v1/com/harman/pres/keyprestypesDBusDeployment.cpp
../src-gen/consumes/v1/com/harman/pres/keyprestypes.cpp
)
target_link_libraries(keyprestest-dbus
CommonAPI-DBus
CommonAPI)

