/*
*/

#include "CTimerMaster.h"
//#include <sys/netmgr.h>
#include <pthread.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <math.h>
#include <errno.h>

#include "CMutex.h"
#include "TraceMacros.hpp"

TRC_SCOPE_DEF(PandoraService, CTimerMaster, resetTimerData);
TRC_SCOPE_DEF(PandoraService, CTimerMaster, run);
TRC_SCOPE_DEF(PandoraService, CTimerMaster, initializeTimerChannel);
TRC_SCOPE_DEF(PandoraService, CTimerMaster, releaseRefCount);
TRC_SCOPE_DEF(PandoraService, CTimerMaster, addTimer);
TRC_SCOPE_DEF(PandoraService, CTimerMaster, generateCodeId);

CTimerMaster* 	CTimerMaster::m_TimerMaster = 0;
uint32_t 		CTimerMaster::m_RefCount 	= 0;

CMutex gMutex;

// BAAAAAAAAAAAD HACK : Live with it....
void timer_master_signal_handler (int signal)
{
	if (SIGUSR1 == signal)
	{
		// Graceful exit
		//slogf (JLR_SLOG, _SLOG_INFO, "[Timer] signal rcvd to release TimerMaster...");
		CTimerMaster::releaseTimerMaster ();
	}
}

CTimerMaster::CTimerMaster() : IThreadInterface("TimerMaster"), m_ChannelId (0), m_CodeIdIndex (0), m_Active (false)
{
	signal (SIGUSR1, timer_master_signal_handler);
}

CTimerMaster::~CTimerMaster()
{
	resetTimerData ();
	//slogf (JLR_SLOG, _SLOG_INFO, "[Timer]!!! timer master destroyed");
}

void CTimerMaster::resetTimerData ()
{
	TRC_SCOPE(PandoraService, CTimerMaster, resetTimerData);
	// This should make the blocking MsgReceivePulse to come out.
	if (m_Event.sigev_coid > 0)		ConnectDetach (m_Event.sigev_coid);
	if (m_ChannelId > 0)			ChannelDestroy (m_ChannelId);

	m_ChannelId = -1;

	//slogf (JLR_SLOG, _SLOG_INFO, "[Timer] channel destroyed, wait for thread to join");
	//DBG_MSG(("[Timer] channel destroyed, wait for thread to join"));

	int ret = waitStop (); /* CAUTION : blocking call */

	//slogf (JLR_SLOG, _SLOG_INFO, "[Timer]!!! thread [%s] join status[%d]", m_ThreadName, ret);
	//DBG_MSG(("[Timer] !!! thread [%s] join status[%d]", m_ThreadName, ret));

	m_TimerMap.clear ();

	m_Active 	= false;
}

// runs in the thread context
void CTimerMaster::run()
{
	TRC_SCOPE(PandoraService, CTimerMaster, run);
	tTimerMap::iterator 					pos;
	int32_t 								pid = 0;
	struct _pulse 							pulse;

	//slogf (JLR_SLOG, _SLOG_INFO, "[Timer] Running the timer watch thread ...");
	//DBG_MSG(("[Timer] channel destroyed, wait for thread to join"));

	while ((m_ChannelId > 0) && (true == m_Active))
	{
		pid = 0;

		// This is blocking call, one of the way to come out is to destroy the channel
		pid = MsgReceivePulse (m_ChannelId, &pulse, sizeof(pulse), NULL);

		if (!pid)
		{
			pos = m_TimerMap.find (pulse.code);

			CTimer* pTimer = static_cast<CTimer*>(m_TimerMap[pulse.code]);

			if ((0 != pTimer) &&  pTimer->m_sigCode == pulse.code)
			{
				pTimer->m_TimerHndlr->handleTimerEvent (pTimer); // Should not block :-0, and dont call Timer functions from this function
			}
		}
		else
		{
			//slogf (JLR_SLOG, _SLOG_ERROR, "[Timer] MsgRcv failed !!!");
			//DBG_MSG(("[Timer] MsgRcv failed !!!"));
			break;
		}
	}

	//slogf (JLR_SLOG, _SLOG_INFO, "[Timer] exiting timer thread Channel[%d], active[%d]", m_ChannelId, m_Active);
	//DBG_MSG(("[Timer] exiting timer thread Channel[%d], active[%d]", m_ChannelId, m_Active));
}

// get the timerMaster object
CTimerMaster* CTimerMaster::getTimerMaster ()
{
	if (0 == m_TimerMaster)
	{
		gMutex.lock(); // Thread safe block starts

		if (0 == m_TimerMaster)		{
			m_TimerMaster = new CTimerMaster ();	}

		gMutex.unlock(); // Thread safe block ends
	}

	return m_TimerMaster;
}

bool CTimerMaster::initializeTimerChannel ()
{
	TRC_SCOPE(PandoraService, CTimerMaster, initializeTimerChannel);
	bool ret = true;

	if (m_ChannelId > 0){
		//slogf (JLR_SLOG, _SLOG_WARNING, "[Timer] TimerMaster already initialized");
		DBG_MSG(("[Timer] TimerMaster already initialized"));
	}
	else
	{
		// Create channel and remember channel id
		m_ChannelId = ChannelCreate (0);

		if (-1 == m_ChannelId) 	{
			ret = false; 		}
		else
		{
			m_Event.sigev_notify 	= SIGEV_PULSE;
			m_Event.sigev_coid 		= ConnectAttach (ND_LOCAL_NODE, 0, m_ChannelId, 0, 0);
			m_Event.sigev_priority 	= getprio (0);
		}
	}

	return ret;
}

_Int8t CTimerMaster::generateCodeId()
{
	TRC_SCOPE(PandoraService, CTimerMaster, generateCodeId);
	_Int8t codeId = 0;

	codeId = (4 + _PULSE_CODE_MINAVAIL + (++m_CodeIdIndex));

	//slogf (JLR_SLOG, _SLOG_INFO, "[Timer] generated CodeId %d", codeId);

	if (_PULSE_CODE_MAXAVAIL < codeId )
	{
		--m_CodeIdIndex;
		codeId =  0;
	}
	//DBG_MSG(("Timer CodeId %d", codeId));
	return (codeId );
}
/*_Int8t CTimerMaster::generateCodeId()
{
	TRC_SCOPE(PandoraService, CTimerMaster, generateCodeId);
    bool found = false;
	tTimerMap::iterator 					pos;

	_Int8t codeId = _PULSE_CODE_MINAVAIL;
	for( ; ( (codeId < _PULSE_CODE_MAXAVAIL) && (found == false) ); codeId++ )
	{
		pos = m_TimerMap.find (codeId);
		if (pos == m_TimerMap.end ())
		{
			DBG_MSG(("Timer CodeId %d", codeId));
			found = true;
		}
		//else{
		//DBG_MSG(("[%s] else Timer CodeId %d", m_LogTxt, codeId));}
	}
	if(found == true)
	{
		codeId = codeId -1;
	}
	else if(found == false)
	{
		codeId = -1;
	}
	return (codeId );
}*/

// release Timer master object
void CTimerMaster::releaseTimerMaster ()
{
	gMutex.lock(); // Thread safe block starts
	if ((0 == m_RefCount) && (0 != m_TimerMaster))
	{
		delete m_TimerMaster;
		m_TimerMaster = 0;
	}
	gMutex.unlock(); // Thread safe block ends
}

void CTimerMaster::addRefCount ()
{
	++m_RefCount;
}

void CTimerMaster::releaseRefCount()
{
	TRC_SCOPE(PandoraService, CTimerMaster, releaseRefCount);
	if (m_RefCount > 0) // Don't mess up please :-)
	--m_RefCount;

	if (0 == m_RefCount)
	{
		// Bad caller, but i have to take care
		if (pthread_self () == m_ThreadId)
		{
			//slogf (JLR_SLOG, _SLOG_WARNING, "[Timer] Call from same thread!!! so, Signal to destroy");
			DBG_MSG(("[Timer] Call from same thread!!! so, Signal to destroy"));
			kill (getpid(), SIGUSR1);
		}
		else
		{	releaseTimerMaster ();	}
	}

	// CAUTION !!! don't do anything beyond this. You will have to face the music
}

bool CTimerMaster::addTimer (CTimer* pTimer, bool repeat)
{
	TRC_SCOPE(PandoraService, CTimerMaster, addTimer);
	bool status = false;
	if (0 != pTimer)
	{
		gMutex.lock (); // Thread safe block starts

		_Int8t codeId = -1;
		initializeTimerChannel ();
		codeId =  generateCodeId ();
		if(-1 == codeId)
		{
			DBG_MSG(("codeId creation failed!!!"));
		}
		else
		{

			m_Event.sigev_code	= pTimer->m_sigCode = codeId;

			int32_t ret = timer_create (CLOCK_MONOTONIC, &m_Event, &pTimer->m_TimerId);

			if (ret != 0)
			{
				//slogf (JLR_SLOG, _SLOG_ERROR, "[Timer] creation failed!!!");
				DBG_MSG(("[Timer] creation failed!!!"));
			}
			else
			{
				status = true;

				uint64_t nSeconds 		= pTimer->m_timeOutVal / 1000;
				long double nRemaining	= fmodl (static_cast<long double>(pTimer->m_timeOutVal), static_cast<long double>(1000) );

				pTimer->m_TimerSpec.it_value.tv_sec 	= nSeconds;
				pTimer->m_TimerSpec.it_value.tv_nsec 	= nRemaining * 1000000;

				//slogf (JLR_SLOG, _SLOG_INFO, "[Timer] [%d] created, Event[%d], Sec[%Lu], Nano[%lu]!!!", pTimer->m_TimerId, m_Event.sigev_code, nSeconds, pTimer->m_TimerSpec.it_value.tv_nsec);
				//DBG_MSG(("[Timer] [%d] created, Event[%d], Sec[%Lu], Nano[%lu]!!!", pTimer->m_TimerId, m_Event.sigev_code, nSeconds, pTimer->m_TimerSpec.it_value.tv_nsec));

				// If the requester has opted for repeat
				if (true == repeat)
				{
					pTimer->m_TimerSpec.it_interval.tv_sec 	= nSeconds;
					pTimer->m_TimerSpec.it_interval.tv_nsec = nRemaining * 1000000;
				}

				timer_settime (pTimer->m_TimerId, 0, &pTimer->m_TimerSpec, NULL);

				m_TimerMap.insert (tTimerMap::value_type (m_Event.sigev_code, pTimer));

				addRefCount ();

				if (false == m_Active)
				{
					m_Active = true;
					createThread (); // Do it only once
				}
			}
		}
		gMutex.unlock (); // Thread safe block ends
	}

	return status;
}

bool CTimerMaster::removeTimer (CTimer* pTimer)
{
	bool ret = true;

	if (0 != pTimer)
	{
		pTimer->m_TimerSpec.it_value.tv_sec 	= 0;
		pTimer->m_TimerSpec.it_value.tv_nsec 	= 0;
		pTimer->m_TimerSpec.it_interval.tv_sec 	= 0;
		pTimer->m_TimerSpec.it_interval.tv_nsec = 0;

		timer_settime (pTimer->m_TimerId, 0, &pTimer->m_TimerSpec, NULL);

		timer_delete (pTimer->m_TimerId);

		m_TimerMap.erase (pTimer->m_sigCode);

		releaseRefCount ();
	}
	return ret;
}
