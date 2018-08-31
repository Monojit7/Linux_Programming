/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            DeviceIAP2.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           Implements common functionality expected from an IAP2 device.
*/

#ifndef _DEVICE_IAP2_H_
#define _DEVICE_IAP2_H_

#include <device/DeviceBase.h>

class DeviceIAP2 : public DeviceBase
{
public:
	DeviceIAP2(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo);
//	void setRepeat(MMPresTypes::eRepeatMode);

protected:
	virtual uint32_t getDeviceCaps();
	//void setRepeatCallback(const CommonAPI::CallStatus& status, const PlayerTypes::RepeatStatus& mode);

private:
	//FacadeHelper<MediaPlaybackEngine> mPlayEngine;
};

#endif
