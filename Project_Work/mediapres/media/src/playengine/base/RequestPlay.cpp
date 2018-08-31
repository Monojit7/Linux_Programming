#include <core.h>
#include "RequestPlay.h"

RequestPlay::RequestPlay(bool isPlay)
	:RequestBase(e_REQUEST_PLAY,"RequestPlay")
	, mIsPlay(isPlay)
{

}

void RequestPlay::run()
{
	LOG_DEBUG(LOG_CONTEXT_MEDIA, "Play Request Running !"); 
}

void RequestPlay::cancel()
{

}