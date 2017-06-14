#ifndef TUNER_RESPHANDLER_HPP_
#define TUNER_RESPHANDLER_HPP_

/*----------------------------------------------------------------------------------------------------------------------
*
* Project Harman Mid Systems - Tuner Service
* (c) copyright 2010
* Company Harman Internation
* All rights reserved
* Secrecy Level STRICTLY CONFIDENTIAL
*
*-----------------------------------------------------------------------------------------------------------------------
*
* @file CTunerRespHandler.hpp
* @ingroup Tunerservice
* @author
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "jsoncpp/json.h"
#include "CTunerReqQueue.hpp"
#include "svcIpcEventHandling/CSvcIpcAccessoryEventTarget.hpp"
#include "svcIpcEventHandling/CSvcIpcEventRouter.hpp"
#include "svcIpcEventHandling/CSvcImplBase.hpp"
#include "svcIpcEventHandling/CSvcIpcEventTypes.hpp"


class CTunerRespHandler
{
 public:
	    CTunerRespHandler();
	   ~CTunerRespHandler();

	  // void handleRes(unsigned int token, Json::Value res);
	   //void handleNotif(std::string signalName,Json::Value res);
	  // void sendResponse(CSvcIpcRequest* reqData, tunerErrorTypes error);
	  // void sendPresets(CSvcIpcRequest* reqData, Json::Value res);

};

#endif
