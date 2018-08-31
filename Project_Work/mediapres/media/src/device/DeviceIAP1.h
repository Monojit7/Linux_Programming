/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            DeviceIAP1.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           Implements common functionality expected from an IAP1 device.
*/

#ifndef _DEVICE_IAP1_H_
#define _DEVICE_IAP1_H_

#include <device/DeviceBase.h>

class DeviceIAP1 : public DeviceBase
{
public:
	DeviceIAP1(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo);

protected:
	virtual uint32_t getDeviceCaps();
};

#endif

