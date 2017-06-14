#include "CTunerReqQueue.hpp"


CTunerReqData::CTunerReqData()
{

  mContext = NULL;
  mMethod = "";
  mParms = "";
  mNoReplyExpected = false;
  mUserToken = 0;
}
CTunerResHandler::CTunerResHandler()
{
}
CTunerResHandler::~CTunerResHandler()
{

}
CTunerResHandler* CTunerResHandler::mpInst;
/*CTunerResHandler* CTunerResHandler::GetInstance(void)
{
	  if(NULL == mpInstance)
	  {
		 mpInstance = new CTunerResHandler();
	  }
	  return mpInstance;
}*/
/*
BOOL CTunerRespHandler::enQueueRequest(CTunerReqData *tunerRequest)
{
	return true;
}

BOOL CTunerRespHandler::deQueueRequest(INT32 token)
		{
	return true;
}

BOOL CTunerRespHandler::isRequestValid(INT32 token)
{
	return true;
}
*/
