/*
 * CMutex.h
 *
 *  Created on: Feb 28, 2013
 *      Author: Raj
 */

#include <pthread.h>

#ifndef CMUTEX_H_
#define CMUTEX_H_

class CMutex
{
public:
	CMutex();
	~CMutex();

	void lock();
	void unlock();

	void condWait ();
	void condSignal ();

	pthread_mutex_t* operator () (void)	{ return &m_MutexAccessor; }

private:
	pthread_mutex_t						m_MutexAccessor;
	pthread_cond_t 						m_condVar;
};

#endif /* CMUTEX_H_ */
