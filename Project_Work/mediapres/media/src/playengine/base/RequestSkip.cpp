#include "core.h"
#include "RequestSkip.h"


RequestSkip::RequestSkip(uint32_t SkipCount)
	:RequestBase(e_REQUEST_SKIP, "RequestSkip")
	,mSkipCount(SkipCount)
{

}

void RequestSkip::run()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "Skip Request Running !");
}

void RequestSkip::cancel()
{

}
