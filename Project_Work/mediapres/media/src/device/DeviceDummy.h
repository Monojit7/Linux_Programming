/*********************************************************************
* Project         Media  Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            DeviceDummy.h
* @ingroup         media
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           The Base Class abstracting a "device" functionality.
*				   Just a dummy.. will be used when no device is available for the slot. 
*/
#include "DeviceBase.h"

class DeviceDummy : public DeviceBase
{
public:
	DeviceDummy(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo);

protected:
	virtual uint32_t getDeviceCaps();
};