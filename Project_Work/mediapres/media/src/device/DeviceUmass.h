/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            DeviceUmass.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           Implements common functionality expected from a USB MSD device.
*/

#ifndef _DEVICE_UMASS_H_
#define _DEVICE_UMASS_H_

#include <device/DeviceBase.h>
#include <config/MediaPresSettings.h>


class MediaPresSettings;
class DeviceUmass : public DeviceBase, public ITimerEventListener
{
public:
	DeviceUmass(const v1::com::harman::media::BaseType::tMediumInfo& MediumInfo);
	~DeviceUmass();
	void timerEvent(int);
	MMPresTypes::eCallResult fastForward();
	MMPresTypes::eCallResult fastReverse();
	virtual void setRepeat(MMPresTypes::eRepeatMode);
	virtual void onRepeatChanged(const PlayerTypes::RepeatStatus&);
	virtual void subscribeToRepeatStatusUpdate(bool subscribe);
	virtual void next();
	virtual void previous();

protected:
	virtual uint32_t getDeviceCaps();
	int determineTimerValue();
	void cancelSeekTimer();
	void setSeekTimer();
private:
	//FacadeHelper<MediaPlaybackEngine> mPlayEngine;
	CTimerInterface mSeekTimer;
	int mSeekTimerId;
	double mRateInvoked;
	bool mffwdRequested;
	bool mfrwdRequested;
	//FacadeHelper<MediaPresSettings> mSettings;
	std::map<double, int> mRateTimeMap;
	std::map<double, int> mRateTimeMapVideo;
	std::map<double, int> mRateTimeMapForAud_Podcast;
	PlayerTypes::RepeatStatus mMediaOneRepeatStatus;
	bool mExposeRepeatOffToHMI;
	uint32_t mRepeatToken;
};

#endif
