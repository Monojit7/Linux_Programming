/*
 * CMutex.cpp
 *
 *  Created on: Feb 28, 2013
 *      Author: Raj
 */

#include "CMutex.h"
#include <stdio.h>

CMutex::CMutex () : m_MutexAccessor (PTHREAD_MUTEX_INITIALIZER), m_condVar (PTHREAD_COND_INITIALIZER)
{
	pthread_mutex_init (&m_MutexAccessor, NULL);
}

CMutex::~CMutex ()
{
	pthread_mutex_destroy (&m_MutexAccessor);
}

void CMutex::lock ()
{
	pthread_mutex_lock (&m_MutexAccessor);
}

void CMutex::unlock ()
{
	pthread_mutex_unlock (&m_MutexAccessor);
}

void CMutex::condWait ()
{
	pthread_cond_wait( &m_condVar, &m_MutexAccessor );
}

void CMutex::condSignal ()
{
	pthread_cond_signal( &m_condVar );
}
