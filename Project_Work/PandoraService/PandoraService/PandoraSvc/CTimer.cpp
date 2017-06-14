/*
 * CTimer.cpp
 *
 *  Created on: Feb 12, 2013
 *      Author: Raj
 */

#include "CTimer.h"
#include "CTimerMaster.h"
#include "TraceMacros.hpp"

TRC_SCOPE_DEF(PandoraService, CTimer, startTimer);
TRC_SCOPE_DEF(PandoraService, CTimer, stopTimer);

CTimer::CTimer() { }

CTimer::~CTimer()
{
	stopTimer ();
}

CTimer::CTimer(ITimerHandler* pTimerHandler):
		m_active (false), m_timeOutVal (0), m_Repeat(false), m_TimerHndlr(pTimerHandler), m_TimerId(0), m_sigCode(0)
{
}

bool CTimer::startTimer (uint64_t interval /*in milli seconds*/, bool repeat)
{
	TRC_SCOPE(PandoraService, CTimer, startTimer);
	bool ret = false;
	if (false == m_active) // add only if it is not activated before
	{
		m_active 	 = true;
		m_timeOutVal = interval;
		m_Repeat	 = repeat;

		//slogf (JLR_SLOG, _SLOG_INFO, "[CTimer] startTimer Timer!!!" );
		////DBG_MSG(("[CTimer]: startTimer Timer!!!"));
		if ( false == CTimerMaster::getTimerMaster ()->addTimer (this, m_Repeat))
			m_active = false;

		ret = m_active;
	}
	else
	{
		//slogf (JLR_SLOG, _SLOG_ERROR, "[CTimer] ERROR : Timer already active!!!");
		//DBG_MSG(("[CTimer]: ERROR : Timer already active!!!"));
	}
	return ret;
}

bool CTimer::stopTimer ()
{
	TRC_SCOPE(PandoraService, CTimer, stopTimer);
	if (true ==  m_active)
	{
		//slogf (JLR_SLOG, _SLOG_INFO, "[CTimer] stopping Timer [%d]!!!", m_TimerId);
		////DBG_MSG(("[CTimer]: stopping Timer [%d]!!!", m_TimerId));
		CTimerMaster::getTimerMaster ()->removeTimer (this);
		m_active = false;
	}

	return true;
}
