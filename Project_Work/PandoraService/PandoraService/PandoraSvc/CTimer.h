/*
 * CTimer.h
 *
 *  Created on: Feb 12, 2013
 *      Author: Raj
 */

#ifndef CTIMER_H_
#define CTIMER_H_

#include <time.h>
#include <unistd.h>
#include <stdint.h>

class CTimer;
class CTimerMaster;

class ITimerHandler
{
public:
	// Should not block :-0, and dont call Timer functions from this function
	virtual void handleTimerEvent (CTimer* pTimer) = 0;
	virtual ~ITimerHandler() {};
};

class CTimer
{
	friend class CTimerMaster;

private: // No i will tell you how to create me
	CTimer();
	bool				m_active;
	uint64_t 			m_timeOutVal;
	bool				m_Repeat;
	ITimerHandler*		m_TimerHndlr;
	timer_t 			m_TimerId;
	itimerspec 			m_TimerSpec;
	short				m_sigCode;

public:
	// No goof ups please
	explicit CTimer(ITimerHandler* pTimerHandler = 0);
	~CTimer();

	bool 				stopTimer ();
	bool				isActive () {	return m_active;	};
	bool 				startTimer (uint64_t interval /*in milli seconds*/, bool repeat = false /*true if you want to repeat the timer*/);
	short				getTimerId () {return m_sigCode; };
};

#endif /* CTIMER_H_ */
