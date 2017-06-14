/*
 * IThreadInterface.h
 *
 *  Created on: Dec 14, 2012
 *      Author: MPrasad/Raj
 */
/**
* @brief      Interface for using threads with C++ objects
*
* @details    The classes wishing to implement a thread interface should
*             derive and implement the pure virtual run method and call
*             createThread
*
* @note       This interface doesn't provide any locking mechanism.
*             Classes implementing this interface should take care of locking.
*/

#ifndef CPPTHREAD_H_
#define CPPTHREAD_H_

#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <cstdio>

class IThreadInterface
{
protected:
    pthread_t 			m_ThreadId;
    char				m_ThreadName[64];
    static void* 		threadFunction (void *pObj);

	IThreadInterface (const char* pThreadName = 0);
	virtual 			~IThreadInterface ();

    void 				createThread ();
    virtual void 		run () = 0;

public:
    void				stop ();
    int					waitStop ();
};
#endif /* CPPTHREAD_H_ */
