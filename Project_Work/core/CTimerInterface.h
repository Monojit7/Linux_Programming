/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            CHBTimerInterface.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)

* @brief           CHBTimerInterface can be used by clients for creating and cancelling timer.Client can create an
                   object of CHBTimerInterface for doing various timer operations. Also client can use the same 
				   CTimerInterface object for creating multiple independent timers. The client will be provided a 
				   unique ID for each timer which is created.
				   
				   CHBTimerInterface abstarcts the underying OS (windows/linux) for which the timer is created.Also client
				   should register a listener object (implemented from ITimerEventListener) while creating an object of 
				   CHBTimerInterface. This is used for dispatching the timer callback event to the listener. In the listner callback, client will be notified with the timerID.
*/

#ifndef _CTIMERINTERFACE_HPP_
#define _CTIMERINTERFACE_HPP_
//#ifdef PLATFORM_WIN32
#include <core/PresCtrlTypes.h>
#include <core/ITimerEventListener.h>

#include<list>

class CTimerManager; 

class DLL_EXPORT CTimerInterface
{
public:

	enum timerType
	{
		TIMER_ONESHOT = 0u,
		TIMER_PERIODIC
	};


	/**
	* @brief Sets the timer for the mentioned type (either TIMER_ONESHOT or TIMER_PERIODIC) and duration in milliseconds.
	  Returns an unique timer id.
	*/

	int setTimer(timerType type, int duration_ms);

	/**
	* @brief Cancels the timer corresponding to the provided timer id.
	*/

	void cancelTimer(int timerId);

	CTimerInterface(ITimerEventListener& listener);
	~CTimerInterface();

private:
	std::list<int> mTimerIdList;
	static CTimerManager* mTimerManager; 
	ITimerEventListener& mListener;
};
#endif