#include <device/DeviceIAP1.h>

DeviceIAP1::DeviceIAP1(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo)
	:DeviceBase(mediumInfo)
{

}

uint32_t DeviceIAP1::getDeviceCaps()
{

	return (MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATABROWSE | MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATASEARCH);
}