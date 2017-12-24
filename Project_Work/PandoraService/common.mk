# This is an automatically generated record.
# The area between QNX Internal Start and QNX Internal End is controlled by
# the QNX IDE properties.

ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

define PINFO
PINFO DESCRIPTION=Pandora Radio Service
PINFO STATE=Release
PINFO VERSION=1.0.0
endef

#===== USEFILE - the file containing the usage message for the application. 
USEFILE=

# Next lines are for C++ projects only

EXTRA_SUFFIXES+=cxx cpp

#===== LDFLAGS - add the flags to the linker command line.
LDFLAGS+=-lang-c++

VFLAG_g=-gstabs+

#===== EXTRA_INCVPATH - a space-separated list of directories to search for include files.
EXTRA_INCVPATH+=$(PROJECT_ROOT)/../target/qnx6/usr/include  \
	$(PROJECT_ROOT)/../deliveries/fwcommon  \
	$(PROJECT_ROOT)/../deliveries\Pandora  \
	$(PROJECT_ROOT)/../deliveries\IPhone  \
	$(PROJECT_ROOT)/../PandoraService  \
	$(PROJECT_ROOT)/../common/inc  \
	$(PROJECT_ROOT)/../PandoraService/PandoraService/PandoraSvc  \
	$(PROJECT_ROOT)/../PandoraService/persistency  \
	$(PROJECT_ROOT)/../deliveries/IOMEDIA

#===== EXTRA_LIBVPATH - a space-separated list of directories to search for library files.
EXTRA_LIBVPATH+=$(PROJECT_ROOT)/../target/qnx6/armle-v7/lib  \
	$(PROJECT_ROOT)/../target/qnx6/armle-v7/usr/lib  \
	$(PROJECT_ROOT)/../deliveries/fwcommon/lib/servicebroker  \
	$(PROJECT_ROOT)/../deliveries/Pandora/bin/mm/pandoralib/armv7-qnx-m650sp1-4.4.2-osz-rel-rtti-cpp/lib/  \
	$(PROJECT_ROOT)/../deliveries\IPhone\bin\mm\iap1devctrl\armv7-qnx-m650sp1-442-osz-rel-cpp\lib  \
	$(PROJECT_ROOT)/../deliveries/MME/Trunk/bin/mm/mme/target/qnx6/armle-v7/lib

#===== NAME - name of the project (default - name of project directory).
NAME=PandoraService

#===== LIBS - a space-separated list of library items to be included in the link.
LIBS+=svcIpcCPP jsoncpp svcipc dbus-1 socket bessel m  \
	sys_trace_clienttrace -Bstatic mm_pandoralib  \
	mm_iap1devctrl -Bdynamic PocoFoundation ipod iomedia usbdi

	#mm_pandoralib ph

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PROJECT_ROOT)/../PandoraService  \
	$(PROJECT_ROOT)/../PandoraService/PandoraService/PandoraSvc  \
	$(PROJECT_ROOT)/../PandoraService/persistency

include $(MKFILES_ROOT)/qmacros.mk
ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif
include $(QNX_INTERNAL)

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))

