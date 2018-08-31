/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            sourceswitch.h
* @ingroup         mediapresctrl
* @author          sriram srivatsan (sriram.srivatsan@harman.com)
* @brief           This contains the interaction with the source switch statemachine impemented using yakindu statechart tool.
                    This basically contains the implementation of pure virtual methods whose signature is defined in the state machine.
					These methods are invoked from the state machine based on the source switch request, source states and error info triggered from either a
					request from AM or from the other device classes in the media pres.
*/

/*

#ifndef _SOURCE_SWITCH_H_
#define _SOURCE_SWITCH_H_

#include<core.h>
#include<statemachine/MediaSwitch7.h>
#include<detectionengine/MediaDetectionEngine.h>
#include<device/DeviceBase.h>
#include<audiomanagerinterface/audiomanagerinterface.h>
#include<playengine/MediaPlaybackEngine.h>

class AudioManagerInterface;
class SourceSwitch;
class MediaDetectionEngine;
class SourceOperation :public MediaSwitch7::SCI_Media_OCB, public ITimerEventListener
{
public:
	void timerEvent(int timerid);
private:
	SourceOperation();
	~SourceOperation();

	// initiate a play for the media source.This method is triggered from the state machine.
	sc_boolean doPlay(sc_integer sourceId);

	// initiate a pause for the media source.This method is triggered from the state machine.
	sc_boolean doPause(sc_integer sourceId);

	// initiate a off for the media source.This method is triggered from the state machine.
	sc_boolean doOff(sc_integer sourceId);

	// inform the status  of the  play request for the media source.This method is triggered from the state machine.
	void resPlay(sc_integer sourceId,sc_boolean status);

	// inform the status  of the  pause request for the media source.This method is triggered from the state machine.
	void resPause(sc_integer sourceId,sc_boolean status);

	// inform the status  of the  off request for the media source.This method is triggered from the state machine.
	void resOff(sc_integer sourceId,sc_boolean status);

	// print the input state
	void print(sc_string name);

	bool deactivatePlayback(uint64_t deviceId);

	FacadeHelper<MediaDetectionEngine> mDetect;
	friend class SourceSwitch;
	FacadeHelper<AudioManagerInterface> mAudioManager;

	CTimerInterface mTimer;
	int timerId;
	void startTimer();
	void cancelTimer();
};

class SourceSwitch :public MediaSwitch7
{
public:
	void startTimer();
	void cancelTimer();
private:
	uint32_t registerClient()
	{
		return 0u;
	}
	void deregisterClient(uint32_t ClientID)
	{

	}
	SourceSwitch();
	~SourceSwitch();
	friend class FacadeHelper<SourceSwitch>;
	SourceOperation* operation;
};
#endif
*/

#ifndef _SOURCE_SWITCH_H_
#define _SOURCE_SWITCH_H_

#include<core.h>
#include<detectionengine/MediaDetectionEngine.h>
#include<audiomanagerinterface/audiomanagerinterface.h>

class AudioManagerInterface;
class MediaDetectionEngine;
class SourceSwitch:public ITimerEventListener
{
public:

	// initiate a play for the media source.
	bool doPlay(int sourceId);

	// initiate a pause for the media source.
	bool doPause(int sourceId);

	// initiate a off for the media source.
	bool doOff(int sourceId);

	bool deactivatePlayback(uint64_t deviceId);

    bool retainPauseStateForDevice();

	void startTimer();
	void cancelTimer();

	void timerEvent(int timerid);

	void resPlay(bool val);
	
    void SetretainPauseStateForDevice(const bool);
    bool getSourceSwitchTimerExpired();
    void setSourceSwitchTimerExpired(const bool);

private:
	uint32_t registerClient()
	{
		return 0u;
	}
	void deregisterClient(uint32_t ClientID)
	{

	}
	SourceSwitch();
	~SourceSwitch();
	friend class FacadeHelper<SourceSwitch>;
	FacadeHelper<MediaDetectionEngine> mDetect;
	FacadeHelper<AudioManagerInterface> mAudioManager;
	FacadeHelper<MediaPresSettings> mSettings;
    bool clear_queue_status_ = true ;
	CTimerInterface mTimer;
	int timerId;
    bool mRetainPauseStateFlag;
    bool mSourceSwitchTimerExpired;
};
#endif
