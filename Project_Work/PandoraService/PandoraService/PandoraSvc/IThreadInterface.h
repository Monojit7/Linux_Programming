/*
 * 
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
