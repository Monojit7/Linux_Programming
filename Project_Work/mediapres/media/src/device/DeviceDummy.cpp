#include "DeviceDummy.h"

DeviceDummy::DeviceDummy(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo)
	:DeviceBase(mediumInfo)
{

}

uint32_t DeviceDummy::getDeviceCaps()
{

	return (MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATABROWSE | MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATASEARCH);
}