/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            DeviceBluetooth.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           Implements common functionality expected from a bluetooth device.
*/

#ifndef _DEVICE_MTP_H_
#define _DEVICE_MTP_H_

#include <device/DeviceBase.h>

class MediaDetectionEngine;
class DeviceMtp : public DeviceBase, public ITimerEventListener
{
public:
	DeviceMtp(const v1::com::harman::media::BaseType::tMediumInfo& MediumInfo);
	void timerEvent(int);
	MMPresTypes::eCallResult fastForward();
	MMPresTypes::eCallResult fastReverse();

protected:
	virtual uint32_t getDeviceCaps();
	void fastForwardCallback(const CommonAPI::CallStatus& status, const double& rate);
	void fastReverseCallback(const CommonAPI::CallStatus& status, const double& rate);
	int determineTimerValue();
	void cancelSeekTimer();
	void setSeekTimer();

private:
	CTimerInterface mSeekTimer;
	int mSeekTimerId;
	double mRateInvoked;
	bool mffwdRequested;
	bool mfrwdRequested;
	std::map<double, int> mRateTimeMap;
	std::map<double, int> mRateTimeMapVideo;
	std::map<double, int> mRateTimeMapForAud_Podcast;
};

#endif
