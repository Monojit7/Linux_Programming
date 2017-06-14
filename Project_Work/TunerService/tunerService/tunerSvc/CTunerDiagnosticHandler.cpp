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
 * @file CTunerDiagnosticHandler.cpp
 * @ingroup TunerService
 * @author
 * Public API for the Tuner core service library.
 *
 *---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
 ----------------------------------------------------------------------------------------------------------------------*/

#include "CTunerDiagnosticHandler.hpp"
#include "jsoncpp/json.h"
#include "CTunerRespHandler.hpp"
#include "CTunerDataTypes.hpp"
#include "DMMTunerTypes.h"
#include "DMMAmFmTunerControlCoreTypes.h"
#include <iostream>
#include "stdio.h"
#include "TraceMacros.hpp"

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Defines
 ----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * TraceClient Scope Definitions
 ----------------------------------------------------------------------------------------------------------------------*/
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, execute);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getFieldStrength);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getSoftmuteSwitch);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getHighCutSwitch);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getStereoHighBlendSwitch);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getStereoBlendSwitch);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getQualityThresholdEntry);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getQualityThresholdExit);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getHDAudioMode);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getHDFMBlendTimeAD);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getHDFMBlendTimeDA);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getHDAMBlendTimeAD);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getHDAMBlendTimeDA);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getHDBerMode);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getHDAudioSplitting);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, getHDBlendingMode);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, setSoftmuteSwitch);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, setHighCutSwitch);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, setStereoHighBlendSwitch);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, setStereoBlendSwitch);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, setQualityThresholdEntry);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, setQualityThresholdExit);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, setHDBerMode);
TRC_SCOPE_DEF(TunerService, CTunerDiagnosticHandler, setHDAudioSplitting);

/*......................................................................................................................
 * @brief CTunerDiagnosticHandler()- Constructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerDiagnosticHandler::CTunerDiagnosticHandler()
{
}

/*......................................................................................................................
 * @brief CTunerDiagnosticHandler()- Destructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerDiagnosticHandler::~CTunerDiagnosticHandler() {
}
/*......................................................................................................................
 * @brief execute()
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::execute(CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, execute);
   SVCIPC_tConstStr method = reqData->getMethod();
   SVCIPC_tUserToken token = reqData->getUserToken();
   DBG_MSG(("[CTunerDiagnosticHandler]:execute and method is: %s",method));

   if (strcmp(method, "reqGetFieldStrength") == 0)
   {
     getFieldStrength(token, reqData);
   }
   else if (strcmp(method, "reqGetSoftmuteSwitch") == 0)
   {
     getSoftmuteSwitch(token, reqData);
   }
   else if (strcmp(method, "reqGetHighCutSwitch") == 0)
   {
     getHighCutSwitch(token, reqData);
   }
   else if (strcmp(method, "reqGetStereoHighBlendSwitch") == 0)
   {
     getStereoHighBlendSwitch(token, reqData);
   }
   else if (strcmp(method, "reqGetStereoBlendSwitch") == 0)
   {
     getStereoBlendSwitch(token, reqData);
   }
   else if (strcmp(method, "reqGetQualityThresholdEntry") == 0)
   {
     getQualityThresholdEntry(token, reqData);
   }
   else if (strcmp(method, "reqGetQualityThresholdExit") == 0)
   {
     getQualityThresholdExit(token, reqData);
   }
   else if (strcmp(method, "reqGetHDAudioMode") == 0)
   {
	   getHDAudioMode(token, reqData);
   }
   else if (strcmp(method, "reqGetHDFMBlendTimeAD") == 0)
   {
	   getHDFMBlendTimeAD(token, reqData);
   }
   else if (strcmp(method, "reqGetHDFMBlendTimeDA") == 0)
   {
	   getHDFMBlendTimeDA(token, reqData);
   }
   else if (strcmp(method, "reqGetHDAMBlendTimeAD") == 0)
   {
	   getHDAMBlendTimeAD(token, reqData);
   }
   else if (strcmp(method, "reqGetHDAMBlendTimeDA") == 0)
   {
	   getHDAMBlendTimeDA(token, reqData);
   }
   else if (strcmp(method, "reqGetHDBerMode") == 0)
   {
	   getHDBerMode(token, reqData);
   }
   else if (strcmp(method, "reqGetHDAudioSplitting") == 0)
   {
	   getHDAudioSplitting(token, reqData);
   }
   else if (strcmp(method, "reqGetHDBlendingMode") == 0)
   {
	   getHDBlendingMode(token, reqData);
   }
   else if(strcmp(method,"reqSetSoftmuteSwitch") == 0)
   {
     setSoftmuteSwitch(token, reqData);
   }
   else if(strcmp(method,"reqSetHighCutSwitch") == 0)
   {
     setHighCutSwitch(token, reqData);
   }
   else if(strcmp(method,"reqSetStereoHighBlendSwitch") == 0)
   {
     setStereoHighBlendSwitch(token, reqData);
   }
   else if(strcmp(method,"reqSetStereoBlendSwitch") == 0)
   {
     setStereoBlendSwitch(token, reqData);
   }
   else if(strcmp(method,"reqSetQualityThresholdEntry") == 0)
   {
     setQualityThresholdEntry(token, reqData);
   }
   else if(strcmp(method,"reqSetQualityThresholdExit") == 0)
   {
     setQualityThresholdExit(token, reqData);
   }
   else if(strcmp(method,"reqSetHDBerMode") == 0)
   {
	 setHDBerMode(token, reqData);
   }
   else if(strcmp(method,"reqSetHDAudioSplitting") == 0)
   {
	 setHDAudioSplitting(token, reqData);
   }
   else
   {
     DBG_MSG(("[CTunerDiagnosticHandler]: Request Tuner_Response_Invalid_Request"));
     CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Request);
   }
}

/*......................................................................................................................
 * @brief getFieldStrength()- Function to get the antenna field strength for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerDiagnosticHandler::getFieldStrength(SVCIPC_tUserToken token, CSvcIpcRequest* reqData) {

   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getFieldStrength);
   unsigned char antennaNo = reqData->getParms().get("antennaNo", Json::Value(1)).asInt();
   DBG_MSG(("[CTunerDiagnosticHandler]:getFieldStrength antenna number is %d", antennaNo));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   if(antennaNo == 1)
   {
     tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_FIELDSTRENGTH_ANTENNA1;
	  //tunerData.antennaNo = antennaNo;
     (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

     CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
   }
   else if(antennaNo == 2)
   {
	 tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_FIELDSTRENGTH_ANTENNA2;
	  // tunerData.antennaNo = antennaNo;
	 (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

	  CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
   }
   else
   {
	 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }


}
/*......................................................................................................................
 * @brief getSoftmuteSwitch()- Function to get the softmute switch for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getSoftmuteSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getSoftmuteSwitch);
   DBG_MSG(("[CTunerDiagnosticHandler]:getSoftmuteSwitch"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_SWITCH_SOFTMUTE;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
/*......................................................................................................................
 * @brief getHighCutSwitch()- Function to get the high cut switch for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getHighCutSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getHighCutSwitch);
   DBG_MSG(("[CTunerDiagnosticHandler]:getHighCutSwitch"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_SWITCH_HIGH_CUT;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
/*......................................................................................................................
 * @brief getStereoHighBlendSwitch()- Function to get the stereo high blend switch for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getStereoHighBlendSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getStereoHighBlendSwitch);
   DBG_MSG(("[CTunerDiagnosticHandler]:getStereoHighBlendSwitch"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_SWITCH_STEREO_HIGH_BLEND;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
/*......................................................................................................................
 * @brief getStereoBlendSwitch()- Function to get the stereo blend switch for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getStereoBlendSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getStereoBlendSwitch);
   DBG_MSG(("[CTunerDiagnosticHandler]:getStereoBlendSwitch"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_SWITCH_STEREO_BLEND;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

}
/*......................................................................................................................
 * @brief getQualityThresholdEntry()- Function to get the quality threshold for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getQualityThresholdEntry(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getQualityThresholdEntry);
   DBG_MSG(("[CTunerDiagnosticHandler]:getQualityThresholdEntry"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_Q_IN_LIST;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

}
/*......................................................................................................................
 * @brief getQualityThresholdExit()- Function to get the quality threshold for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getQualityThresholdExit(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getQualityThresholdExit);
   DBG_MSG(("[CTunerDiagnosticHandler]:getQualityThresholdExit"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_Q_OUT_LIST;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

}
/*......................................................................................................................
 * @brief getHDAudioMode()- Function to get the HD audioMode for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getHDAudioMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getHDAudioMode);
   DBG_MSG(("[CTunerDiagnosticHandler]:getHDAudioMode"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_HD_AUDIO_MODE;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

}
/*......................................................................................................................
 * @brief getHDFMBlendTimeAD()- Function to get the HD FM Blendtime Analog 2 Digital for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getHDFMBlendTimeAD(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getHDFMBlendTimeAD);
   DBG_MSG(("[CTunerDiagnosticHandler]:getHDFMBlendTimeAD"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_HD_FM_BLENDTIME_AD;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

}
/*......................................................................................................................
 * @brief getHDFMBlendTimeDA()- Function to get the HD FM Blendtime Digital 2 Analog for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getHDFMBlendTimeDA(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getHDFMBlendTimeDA);
   DBG_MSG(("[CTunerDiagnosticHandler]:getHDFMBlendTimeDA"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_HD_FM_BLENDTIME_DA;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

}
/*......................................................................................................................
 * @brief getHDAMBlendTimeAD()- Function to get the HD AM Blendtime Analog 2 Digital for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getHDAMBlendTimeAD(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getHDAMBlendTimeAD);
   DBG_MSG(("[CTunerDiagnosticHandler]:getHDAMBlendTimeAD"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_HD_AM_BLENDTIME_AD;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

}
/*......................................................................................................................
 * @brief getHDAMBlendTimeDA()- Function to get the HD AM Blendtime Digital 2 Analog for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getHDAMBlendTimeDA(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getHDAMBlendTimeDA);
   DBG_MSG(("[CTunerDiagnosticHandler]:getHDAMBlendTimeDA"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_HD_AM_BLENDTIME_DA;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

}
/*......................................................................................................................
 * @brief getHDBerMode()- Function to get the HD BER (bit error rate) mode for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getHDBerMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getHDBerMode);
   DBG_MSG(("[CTunerDiagnosticHandler]:getHDBerMode"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_HD_BER_MODE;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

}
/*......................................................................................................................
 * @brief getHDAudioSplitting()- Function to get the HD audio splitting for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getHDAudioSplitting(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getHDAudioSplitting);
   DBG_MSG(("[CTunerDiagnosticHandler]:getHDAudioSplitting"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_HD_AUDIO_SPLITTING;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

}
/*......................................................................................................................
 * @brief getHDBlendingMode()- Function to get the HD blending mode for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::getHDBlendingMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, getHDBlendingMode);
   DBG_MSG(("[CTunerDiagnosticHandler]:getHDBlendingMode"));
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_HD_BLENDING;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestGetTunerParameter,(void*)&tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

}
/*......................................................................................................................
 * @brief setSoftmuteSwitch()- Function to set the softmute switch for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::setSoftmuteSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, setSoftmuteSwitch);

   bool status = reqData->getParms().get("switch", Json::Value(0)).isInt();
   if(true == status)
   {
	   int flag = reqData->getParms().get("switch", Json::Value(0)).asInt();
	   DBG_MSG(("[CTunerDiagnosticHandler]:setSoftmuteSwitch flag is %d", flag));
	   proxy tunerData;
	   //tunerData.token = (unsigned int)token;
	   tunerData.token = TOKEN;
	   tunerData.value = flag;
	   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_SWITCH_SOFTMUTE;

	   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetTunerParameter,(void*)&tunerData);
	   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
   }
   else
   {
	 DBG_MSG(("[CTunerDiagnosticHandler]:Invalid Parameter Type"));
	 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }
}
/*......................................................................................................................
 * @brief setHighCutSwitch()- Function to set the high cut switch for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::setHighCutSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, setHighCutSwitch);

   bool status = reqData->getParms().get("switch", Json::Value(0)).isInt();
   if(status == true)
   {
	   int flag = reqData->getParms().get("switch", Json::Value(0)).asInt();
	   DBG_MSG(("[CTunerDiagnosticHandler]:setHighCutSwitch flag is %d", flag));
	   proxy tunerData;
	   //tunerData.token = (unsigned int)token;
	   tunerData.token = TOKEN;
	   tunerData.value = flag;
	   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_SWITCH_HIGH_CUT;
	   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetTunerParameter,(void*)&tunerData);
	   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
   }
   else
   {
	 DBG_MSG(("[CTunerDiagnosticHandler]:Invalid Parameter Type"));
     CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }
}
/*......................................................................................................................
 * @brief setStereoHighBlendSwitch()- Function to set the stereo high blend switch for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::setStereoHighBlendSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, setStereoHighBlendSwitch);
   bool status = reqData->getParms().get("switch", Json::Value(0)).isInt();
   if(status == true)
   {
	   int flag = reqData->getParms().get("switch", Json::Value(0)).asInt();
	   DBG_MSG(("[CTunerDiagnosticHandler]:setStereoHighBlendSwitch flag is %d", flag));
	   proxy tunerData;
	   //tunerData.token = (unsigned int)token;
	   tunerData.token = TOKEN;
	   tunerData.value = flag;
	   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_SWITCH_STEREO_HIGH_BLEND;
	   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetTunerParameter,(void*)&tunerData);
	   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
   }
   else
   {
	 DBG_MSG(("[CTunerDiagnosticHandler]:Invalid Parameter Type"));
	 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }
}
/*......................................................................................................................
 * @brief setStereoBlendSwitch()- Function to set the stereo blend switch for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::setStereoBlendSwitch(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, setStereoBlendSwitch);
   bool status = reqData->getParms().get("switch", Json::Value(0)).isInt();

   if(true == status)
   {
	   int flag = reqData->getParms().get("switch", Json::Value(0)).asInt();
	   DBG_MSG(("[CTunerDiagnosticHandler]:setStereoBlendSwitch flag is %d", flag));
	   proxy tunerData;
	   //tunerData.token = (unsigned int)token;
	   tunerData.token = TOKEN;
	   tunerData.value = flag;
	   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_SWITCH_STEREO_BLEND;

	   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetTunerParameter,(void*)&tunerData);
	   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
   }
   else
   {
	  DBG_MSG(("[CTunerDiagnosticHandler]:Invalid Parameter Type"));
	  CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }

}
/*......................................................................................................................
 * @brief setQualityThresholdEntry()- Function to set the quality threshold for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::setQualityThresholdEntry(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, setQualityThresholdEntry);
   bool status = reqData->getParms().get("value", Json::Value(0)).isInt();

   if(true == status)
   {
	   int thresholdValue = reqData->getParms().get("value", Json::Value(0)).asInt();
	   DBG_MSG(("[CTunerDiagnosticHandler]:setQualityThresholdEntry value is %d", thresholdValue));
	   proxy tunerData;
	   //tunerData.token = (unsigned int)token;
	   tunerData.token = TOKEN;
	   tunerData.value = thresholdValue;
	   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_Q_IN_LIST;
	   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetTunerParameter,(void*)&tunerData);
	   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
   }
   else
   {
     DBG_MSG(("[CTunerDiagnosticHandler]:Invalid Parameter Type"));
	 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }

}
/*......................................................................................................................
 * @brief setQualityThresholdExit()- Function to set the quality threshold for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::setQualityThresholdExit(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, setQualityThresholdExit);
   bool status = reqData->getParms().get("value", Json::Value(0)).isInt();

   if(true == status)
   {
	   int thresholdValue = reqData->getParms().get("value", Json::Value(0)).asInt();
	   DBG_MSG(("[CTunerDiagnosticHandler]:setQualityThresholdExit value is %d", thresholdValue));
	   proxy tunerData;
	   //tunerData.token = (unsigned int)token;
	   tunerData.token = TOKEN;
	   tunerData.value = thresholdValue;
	   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_Q_OUT_LIST;
	   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetTunerParameter,(void*)&tunerData);
	   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
   }
   else
   {
     DBG_MSG(("[CTunerDiagnosticHandler]:Invalid Parameter Type"));
	 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }

}
/*......................................................................................................................
 * @brief setHDBerMode()- Function to set the HD BER Mode for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::setHDBerMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, setHDBerMode);
   bool status = reqData->getParms().get("value", Json::Value(0)).isInt();

   if(true == status)
   {
	   int BERvalue = reqData->getParms().get("value", Json::Value(0)).asInt();
	   DBG_MSG(("[CTunerDiagnosticHandler]:setHDBerMode value is %d", BERvalue));
	   proxy tunerData;
	   //tunerData.token = (unsigned int)token;
	   tunerData.token = TOKEN;
	   tunerData.value = BERvalue;
	   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_HD_BER_MODE;
	   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetTunerParameter,(void*)&tunerData);
	   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
   }
   else
   {
     DBG_MSG(("[CTunerDiagnosticHandler]:Invalid Parameter Type"));
	 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }

}
/*......................................................................................................................
 * @brief setHDAudioSplitting()- Function to set the quality threshold for the tuner service
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerDiagnosticHandler::setHDAudioSplitting(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerDiagnosticHandler, setHDAudioSplitting);
   bool status = reqData->getParms().get("value", Json::Value(0)).isInt();

   if(true == status)
   {
	   int hdAudioSplitValue = reqData->getParms().get("value", Json::Value(0)).asInt();
	   DBG_MSG(("[CTunerDiagnosticHandler]:setHDAudioSplitting value is %d", hdAudioSplitValue));
	   proxy tunerData;
	   //tunerData.token = (unsigned int)token;
	   tunerData.token = TOKEN;
	   tunerData.value = hdAudioSplitValue;
	   tunerData.reqType = DMMAmFmTunerControlCoreTypes_PARAM_HD_AUDIO_SPLITTING;
	   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetTunerParameter,(void*)&tunerData);
	   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
   }
   else
   {
     DBG_MSG(("[CTunerDiagnosticHandler]:Invalid Parameter Type"));
	 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }

}
//----------------------------------------------------- END OF FILE ---------------------------------------------------
