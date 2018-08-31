#ifndef _REQUEST_SKIP_
#define _REQUEST_SKIP_

#include <core.h>

class RequestSkip : public RequestBase
{
public:
	RequestSkip(uint32_t SkipCount);

	virtual void run();
	virtual void cancel();

private : 

	uint32_t mSkipCount; 
};


#endif
