#ifndef _REQUEST_PLAY_
#define _REQUEST_PLAY_

#include <core.h>

class RequestPlay : public RequestBase
{
public : 
	RequestPlay(bool isPlay);

	virtual void run(); 
	virtual void cancel(); 

private : 
	bool mIsPlay; 
};


#endif
