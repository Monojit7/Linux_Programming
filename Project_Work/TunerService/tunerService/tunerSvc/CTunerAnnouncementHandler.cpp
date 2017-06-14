/*----------------------------------------------------------------------------------------------------------------------
 *
 * Project Harman Mid Systems - Tuner Service
 * (c) copyright 2013
 * Company Harman Internation
 * All rights reserved
 * Secrecy Level STRICTLY CONFIDENTIAL
 *
 *-----------------------------------------------------------------------------------------------------------------------
 *
 * @file CTunerAnnouncementHandler.cpp
 * @ingroup TunerService
 * @author
 * Public API for the Tuner core service library.
 *
 *---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
 ----------------------------------------------------------------------------------------------------------------------*/

#include "CTunerAnnouncementHandler.hpp"
#include "CTunerRespHandler.hpp"
#include "jsoncpp/json.h"
#include "CTunerDataTypes.hpp"
#include "DMMTunerTypes.h"
#include <iostream>
#include "stdio.h"
#include "TraceMacros.hpp"

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Defines
 ----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * TraceClient Scope Definitions
 ----------------------------------------------------------------------------------------------------------------------*/
TRC_SCOPE_DEF(TunerService, CTunerAnnouncementHandler, execute);
TRC_SCOPE_DEF(TunerService, CTunerAnnouncementHandler, setTAMode);
TRC_SCOPE_DEF(TunerService, CTunerAnnouncementHandler, cancelTA);
/*......................................................................................................................
 * @brief CTunerAnnouncementHandler()- Constructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/
CTunerAnnouncementHandler* CTunerAnnouncementHandler::mAnnouncementHandler = NULL;
bool CTunerAnnouncementHandler::mAnnouncementState = false;
bool CTunerAnnouncementHandler::TAmode = false;
UINT32 CTunerAnnouncementHandler::TAfreq = 0;

CTunerAnnouncementHandler::CTunerAnnouncementHandler()
{

}

/*......................................................................................................................
 * @brief CTunerAnnouncementHandler()- Destructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerAnnouncementHandler::~CTunerAnnouncementHandler()
{
   ;
}
CTunerAnnouncementHandler* CTunerAnnouncementHandler::getAnnouncementHandler()
{
   if(NULL == mAnnouncementHandler)
   {
     mAnnouncementHandler = new CTunerAnnouncementHandler();
   }
   return mAnnouncementHandler;
}
bool CTunerAnnouncementHandler::getTAMode(void)
{
   return TAmode;
}
void CTunerAnnouncementHandler::execute(CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerAnnouncementHandler, execute);
   DBG_MSG(("[CTunerAnnouncementHandler]: In execute !!! /n"));
   SVCIPC_tConstStr method = reqData->getMethod();
   SVCIPC_tUserToken token = reqData->getUserToken();

   DBG_MSG(("[CTunerAnnouncementHandler]:execute and method is: %s,",method));

   if (strcmp(method, "reqCancelTA") == 0)
   {
     cancelTA(token, reqData);
   }
   else if (strcmp (method, "reqSetTAMode") == 0)
   {
     setTAMode(token, reqData);
   }
   else
   {
     DBG_MSG(("[CTunerAnnouncementHandler]: Request Error"));
     CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Request);
   }
}
/*......................................................................................................................
 * @brief setTAMode()- Function to change TA mode for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerAnnouncementHandler::setTAMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerAnnouncementHandler, setTAMode);
   BOOL mode = reqData->getParms().get("TAMode", Json::Value(false)).asBool();
   DBG_MSG(("[CTunerAnnouncementHandler] : TA mode received to be set is %d",mode));
   TAmode = mode;
   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
/*......................................................................................................................
 * @brief cancelTA()- Function to cancel traffic announcement for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerAnnouncementHandler::cancelTA(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerAnnouncementHandler, cancelTA);
   DBG_MSG(("[CTunerAnnouncementHandler]:cancelTA"));
   mAnnouncementState = false;
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   //tunerData.mode = DMMTunerAnnouncement_UPD_ID_requestAbort;

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
   //Call DSI Interface

   //Call normal DSI seek function
   (CTunerRequestResponseHandler::GetInstance()->getAnnouncementProxy())->postInternalEvent(DMMTunerAnnouncement_UPD_ID_requestAbort,(void*)&tunerData);

}
//----------------------------------------------------- END OF FILE ---------------------------------------------------
