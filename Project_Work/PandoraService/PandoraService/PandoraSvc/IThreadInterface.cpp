/* ---------------------------------------------------------------
 * IThreadInterface.h
 *
 *  Created on: Dec 14, 2012
 *      Author: MPrasad/ Raj
 *--------------------------------------------------------------- */

#include "IThreadInterface.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

#define JLR_SLOG _SLOG_SETCODE(_SLOG_SYSLOG, 1)

IThreadInterface::IThreadInterface (const char* pThreadName) : m_ThreadId (0)
{
	if (0 != pThreadName)
	{	strcpy (m_ThreadName, pThreadName);			}
	else
	{	strcpy (m_ThreadName, (char*)"JLRThread");	}
}

IThreadInterface::~IThreadInterface() { }

static void unwind(__attribute__ ((unused)) void *pObj)
{
	// do some cleanup if u want
}

void* IThreadInterface::threadFunction (void *pObj)
{
    pthread_cleanup_push(unwind, pObj);

	IThreadInterface* pThread = reinterpret_cast <IThreadInterface*>(pObj);

	// call delegated to specific active object, runs in this thread context
	if (0 != pThread)
	{	pThread->run (); 																	}
	else
	{	slogf (JLR_SLOG, _SLOG_ERROR, "[GEN] ThreadFun failed!!! -> You passed null object!!!"); }

	pthread_cleanup_pop (1);

	pthread_exit (0); //  ready to join

	return 0;
}

// CAUTION : this is a blocking call and Should not be called from the same thread context
int IThreadInterface::waitStop ()
{
	int ret = EOK ;

	if ((0 != m_ThreadId) && (pthread_self () != m_ThreadId) )
		ret = pthread_join (m_ThreadId, NULL);
	else
		slogf (JLR_SLOG, _SLOG_WARNING, "[GEN] waitstop : join call from the same thread");

	m_ThreadId = 0;

	return ret;
}

// CAUTION !!! Should not be called from the same thread context
void IThreadInterface::stop ()
{
	if ((0 != m_ThreadId) && (pthread_self () != m_ThreadId) )
	{
		// just cancel the thread, does not matter what it does :-)
		if (EOK == pthread_cancel (m_ThreadId))
		{
			slogf (JLR_SLOG, _SLOG_INFO, "[GEN] waiting for thread[%s] to join", m_ThreadName);

			// just blocks, face the music
			int ret = waitStop ();

			if (EOK == ret)
				slogf (JLR_SLOG, _SLOG_INFO, "[GEN] Thread[%s] joined", m_ThreadName);
			else
				slogf (JLR_SLOG, _SLOG_ERROR, "[GEN] failed to join Thread[%s] Err[%d]", m_ThreadName, ret);
		}
	}
	else
	{
		slogf (JLR_SLOG, _SLOG_ERROR, "[GEN] Bad caller, pthread cancel from the same thread");
	}
}

void IThreadInterface::createThread ()
{
	if (0 == m_ThreadId)
	{
		int retVal = pthread_create (&m_ThreadId, 0, threadFunction, this);

		if (retVal != 0)
			slogf (JLR_SLOG, _SLOG_ERROR, "[GEN] [%s] pthread_create failed", m_ThreadName);
		else
			pthread_setname_np (m_ThreadId, m_ThreadName);
	}
	else
	{
		slogf (JLR_SLOG, _SLOG_WARNING, "[GEN] WARNING, pthread_already created!!!");
	}
}
