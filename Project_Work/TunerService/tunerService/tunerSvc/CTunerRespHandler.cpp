/**********************************************************************
 *  Project
 *  (c) copyright 2011
 *  Company       Harman/Becker Automotive Systems GmbH
 *                All rights reserved
 * Secrecy Level  STRICTLY CONFIDENTIAL
 **********************************************************************/

/**
 *  @file         CTunerRespHandler.cpp
 *  @brief
 *  @author
 *  @ingroup
 */

/*---------------------------------------------------------------------\
 * Preprocessor Includes
 *--------------------------------------------------------------------*/
#include "CTunerRespHandler.hpp"
#include "CTunerReqQueue.hpp"
#include "CTunerRequestResponseHandler.hpp"
#include "svcIpcEventHandling/CSvcIpcAccessoryEventTarget.hpp"
#include "svcIpcEventHandling/CSvcIpcEventRouter.hpp"
#include "svcIpcEventHandling/CSvcImplBase.hpp"
#include <iostream>
#include "stdio.h"
#include "jsoncpp/json.h"

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Defines
----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * TraceClient Scope Definitions
----------------------------------------------------------------------------------------------------------------------*/


/*......................................................................................................................
 * @brief CTunerRespHandler()- Constructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerRespHandler::CTunerRespHandler()
{
}
/*......................................................................................................................
 * @brief CTunerRespHandler()- Destructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerRespHandler::~CTunerRespHandler()
{
}

void clearQueue(SVCIPC_tUserToken token)
{
	 //printf("******CTunerRespHandler::In clearQueue*****\n");
	//Remove from queue
}

/*void CTunerRespHandler::handleRes(unsigned int token, Json::Value res)
{
	 // printf("******CTunerRespHandler::In handleResp*****\n");
	 //clearQueue(token);
     //Access the list for the queued object
	  CTunerResHandler* obj = CTunerResHandler::GetInstance();
	  list<CTunerReqData*>::iterator p = ((obj->tunerRequestQueue).begin());
	  CTunerReqData* data = new CTunerReqData();
	  SVCIPC_tReqContext svcipcContext = NULL ;
	  while(p != ((obj->tunerRequestQueue).end()))
	  {
		  std::cout<<"Token in resp is : "<<(*p)->mUserToken<<std::endl;
		  data = *p;
		  SVCIPC_tUserToken tkn = data->mUserToken ;

		  if(tkn == (SVCIPC_tUserToken)token)
		  {
			  svcipcContext = data->mContext;
			  //std::cout<<"List size before deleting is: "<<(obj->tunerRequestQueue).size()<<std::endl;
			  //(obj->tunerRequestQueue).remove(*p);
			  //std::cout<<"List size after deleting is: "<<(obj->tunerRequestQueue).size()<<std::endl;
			  break;
		  }
		  p++;

	  }
	 //std::cout<<"Context in response is : "<<svcipcContext<<std::endl;
	 //Send response to DBUS
	 SVCIPC_tError errorCode = SVCIPC_ERROR_NONE;
	 Json::FastWriter mJsonWriter;
	 std::string replyStr  = mJsonWriter.write(res);
	 //errorCode = SVCIPC_returnResult(svcipcContext,replyStr.c_str());
	 errorCode = CSvcImplBase::returnReply(svcipcContext,replyStr.c_str(),true );
		if(SVCIPC_IS_ERROR( errorCode ))
			   {
				  printf("SVCIPC_returnResult return errorCode %u", errorCode );
			   }
		 //std::cout<<"Text in resp is : "<<replyStr<<std::endl;


}*/
/*void CTunerRespHandler::handleNotif(std::string signalName,Json::Value res)
{
	 //printf("******CTunerRespHandler::In handleNotif*****\n");
	 Json::FastWriter mJsonWriter;
	 std::string signalStr  = mJsonWriter.write(res);
	 SVCIPC_tError errorCode = SVCIPC_ERROR_NONE;

	 //Now emit the signal in synchronous way (blocking call)
	 //errorCode = SVCIPC_emit(svcipcHandle, signalName.c_str(), signalStr.c_str());
	// SVCIPC_tError emit( const char* sigName, const char* params );

	// Example:
	  errorCode = CSvcImplBase::emit(signalName.c_str(), signalStr.c_str());

	 //std::cout<<"Error is notif is: "<<errorCode<<std::endl;

}


void CTunerRespHandler::sendResponse(CSvcIpcRequest* reqData, tunerErrorTypes error)
{
	Json::FastWriter mJsonWriter;
  if (0 != reqData)
  {
	Json::Value ret;
	switch(error)
	{
		case 0:
			ret["description"] = "Success";
			break;
		case 1:
			ret["description"] = "Error";
			break;
		case 2:
			ret["description"] = "Invalid Request";
			break;
		case 3:
			ret["description"] = "Invalid Parameter";
			break;
		case 4:
			ret["description"] = "Invalid";
			break;
		default:
			ret["description"] = "Invalid";
			break;
	}
	ret["code"] = error;
	reqData->returnReply(mJsonWriter.write(ret), true );
  }

}
void CTunerRespHandler::sendPresets(CSvcIpcRequest* reqData, Json::Value res)
{
	std::cout<<"[CTunerRespHandler]: Presets sent !!"<<std::endl;
	Json::FastWriter mJsonWriter;
	reqData->returnReply(mJsonWriter.write(res), false );

}*/
