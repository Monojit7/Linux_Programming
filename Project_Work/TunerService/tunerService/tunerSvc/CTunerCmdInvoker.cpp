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
* @file CTunerCmdInvoker.hpp
* @ingroup TunerService
* @author 
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/


#include "CTunerCmdInvoker.hpp"
#include "CTunerSeekHandler.hpp"
#include "CTunerScanHandler.hpp"
#include "CTunerRespHandler.hpp"
#include "CTunerDataTypes.hpp"
#include "CTunerCommand.hpp"
#include "CTunerPresetMgr.hpp"
#include "CTunerPresets.hpp"
#include "CTunerAnnouncementHandler.hpp"
#include "CTunerDiagnosticHandler.hpp"
#include "CDMMAmFmTunerDiranaControlProxy.hpp"
#include <iostream>
#include "TraceMacros.hpp"

TRC_SCOPE_DEF(TunerService, CTunerCmdInvoker, handleTunerRequest);
TRC_SCOPE_DEF(TunerService, CTunerCmdInvoker, getEnumFromMethod);

CTunerCmdInvoker* CTunerCmdInvoker::mpInstance = NULL;
int CTunerCmdInvoker::eListType = -1;
/*......................................................................................................................
 * @brief CTunerCmdInvoker()- Constructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/
CTunerCmdInvoker::CTunerCmdInvoker()
{

}
/*......................................................................................................................
 * @brief CTunerCmdInvoker()- Destructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/
CTunerCmdInvoker::~CTunerCmdInvoker()
{

}
/*......................................................................................................................
 * @brief CTunerCmdInvoker()- Initialize()
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

bool CTunerCmdInvoker::Initialize()
{
  return true;
}
/*......................................................................................................................
 * @brief CTunerCmdInvoker()- deinitialize()
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/
void CTunerCmdInvoker::deinitialize()
{

}
/*......................................................................................................................
 * @brief CTunerCmdInvoker()- GetInstance()
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerCmdInvoker* CTunerCmdInvoker::GetInstance(void)
{
   if(NULL == mpInstance)
   {
     mpInstance = new CTunerCmdInvoker();
     if(mpInstance)
     mpInstance->Initialize();
   }
   return mpInstance;
}


void CTunerCmdInvoker::setPersistencyFlag(bool status)
{
   mPersistencyRead = status;
}

bool CTunerCmdInvoker::GetPersistencyFlag()
{
  return mPersistencyRead;
}
  
/*......................................................................................................................
 * @brief getEnumFromMethod()
 *
 * @param
 *
 * @returns :
 *
 *
 *....................................................................................................................*/

tunerReqTypes CTunerCmdInvoker::getEnumFromMethod(char* method)
{
   TRC_SCOPE(TunerService, CTunerCmdInvoker, getEnumFromMethod);
   DBG_MSG(("[CTunerCmdInvoker]:Method value in getEnumFromMethod is : %s",method));

   tunerReqTypes request = Tuner_Req_Invalid;
   if(strcmp(method,"reqStartSeek") == 0)
   {
     request = Tuner_Start_Seek;
   }
   else if(strcmp(method,"reqStopSeek") == 0)
   {
     request = Tuner_Req_AbortSeek;
   }
   else if(strcmp(method,"reqSetFrequency") == 0)
   {
     request = Tuner_Req_Set_Frequency;
   }
   else if(strcmp(method,"reqStartScan") == 0)
   {
     request = Tuner_Req_Scan;
   }
   else if(strcmp(method,"reqStopScan") == 0)
   {
     request = Tuner_Req_AbortScan;
   }
   else if(strcmp(method,"reqGetFrequency") == 0)
   {
     request = Tuner_Req_Get_Frequency;
   }
   else if(strcmp(method,"reqSetAF") == 0)
   {
     request = Tuner_Req_Set_AFSwitch;
   }
   else if(strcmp(method,"setWaveband") == 0)
   {
     request = Tuner_Req_Set_Waveband;
   }
   else if(strcmp(method,"reqStorePreset") == 0)
   {
     request = Tuner_Req_Store_Preset;
   }
   else if(strcmp(method, "reqAutoStorePreset") == 0)
   {
      request = Tuner_Req_Auto_Store_Preset;
   }
   else if(strcmp(method,"reqSelectPreset") == 0)
   {
     request = Tuner_Req_Select_Preset;
   }
   else if(strcmp(method,"reqGetPresetList") == 0)
   {
     request = Tuner_Req_Get_Preset;
   }
   else if(strcmp(method,"getProperties") == 0)
   {
     DBG_MSG(("Invoker: GetProperties ............\n"));
     request = Tuner_Req_Get_Properties;
   }
   else if(strcmp(method,"reqSetTAMode") == 0)
   {
     request = Tuner_Req_Set_TAMode;
   }
   else if(strcmp(method,"reqCancelTA") == 0)
   {
     request = Tuner_Req_Cancel_TA;
   }
   else if(strcmp(method,"reqGetSignalQuality") == 0)
   {
     request = Tuner_Req_Get_SignalQuality;
   }
   else if(strcmp(method,"reqSetHDMode") == 0)
   {
     request = Tuner_Req_Set_HDMode;
   }
   else if(strcmp(method,"reqSetBallgameMode") == 0)
   {
     request = Tuner_Req_Set_BallgameMode;
   }
   else if(strcmp(method,"reqSetRegMode") == 0)
   {
     request = Tuner_Req_Set_RegMode;
   }
   else if(strcmp(method,"reqSetStationSortingFlag") == 0)
   {
     request = Tuner_Req_Set_StationSortingFlag;
   }
   else if(strcmp(method,"reqGetStationList") == 0)
   {
     request = Tuner_Req_Get_StationList;
   }
   else if(strcmp(method,"reqGetFieldStrength") == 0)
   {
     request = Tuner_Req_Get_FieldStrength;
   }
   else if(strcmp(method,"reqGetSoftmuteSwitch") == 0)
   {
     request = Tuner_Req_Get_SoftMute_Switch;
   }
   else if(strcmp(method,"reqGetHighCutSwitch") == 0)
   {
     request = Tuner_Req_Get_High_Cut_Switch;
   }
   else if(strcmp(method,"reqGetStereoHighBlendSwitch") == 0)
   {
     request = Tuner_Req_Get_Stereo_High_Blend_Switch;
   }
   else if(strcmp(method,"reqGetStereoBlendSwitch") == 0)
   {
     request = Tuner_Req_Get_Stereo_Blend_Switch;
   }
   else if(strcmp(method,"reqGetQualityThresholdEntry") == 0)
   {
        request = Tuner_Req_Get_Quality_Threshold_Entry;
   }
   else if(strcmp(method,"reqGetQualityThresholdExit") == 0)
   {
        request = Tuner_Req_Get_Quality_Threshold_Exit;
   }
   else if(strcmp(method,"reqGetHDAudioMode") == 0)
   {
        request = Tuner_Req_Get_HD_Audio_Mode;
   }
   else if(strcmp(method,"reqGetHDFMBlendTimeAD") == 0)
   {
        request = Tuner_Req_Get_HD_FM_BlendTime_AD;
   }
   else if(strcmp(method,"reqGetHDFMBlendTimeDA") == 0)
   {
        request = Tuner_Req_Get_HD_FM_BlendTime_DA;
   }
   else if(strcmp(method,"reqGetHDAMBlendTimeAD") == 0)
   {
        request = Tuner_Req_Get_HD_AM_BlendTime_AD;
   }
   else if(strcmp(method,"reqGetHDAMBlendTimeDA") == 0)
   {
        request = Tuner_Req_Get_HD_AM_BlendTime_DA;
   }
   else if(strcmp(method,"reqGetHDBerMode") == 0)
   {
        request = Tuner_Req_Get_HD_BER_Mode;
   }
   else if(strcmp(method,"reqGetHDAudioSplitting") == 0)
   {
        request = Tuner_Req_Get_HD_Audio_Splitting;
   }
   else if(strcmp(method,"reqGetHDBlendingMode") == 0)
   {
        request = Tuner_Req_Get_HD_Blending_Mode;
   }
   else if(strcmp(method,"reqSetSoftmuteSwitch") == 0)
   {
        request = Tuner_Req_Set_SoftmuteSwitch;
   }
   else if(strcmp(method,"reqSetHighCutSwitch") == 0)
   {
        request = Tuner_Req_Set_High_Cut_Switch;
   }
   else if(strcmp(method,"reqSetStereoHighBlendSwitch") == 0)
   {
        request = Tuner_Req_Set_Stereo_High_Blend_Switch;
   }
   else if(strcmp(method,"reqSetStereoBlendSwitch") == 0)
   {
        request = Tuner_Req_Set_Stereo_Blend_Switch;
   }
   else if(strcmp(method,"reqSetQualityThresholdEntry") == 0)
   {
        request = Tuner_Req_Set_Quality_Threshold_Entry;
   }
   else if(strcmp(method,"reqSetQualityThresholdExit") == 0)
   {
        request = Tuner_Req_Set_Quality_Threshold_Exit;
   }
   else if(strcmp(method,"reqSetHDBerMode") == 0)
   {
        request = Tuner_Req_Set_HD_BER_Mode;
   }
   else if(strcmp(method,"reqSetHDAudioSplitting") == 0)
   {
        request = Tuner_Req_Set_HD_Audio_Splitting;
   }
   else if(strcmp(method,"reqNextPreset") == 0)
   {
        request = Tuner_Req_Skip_Preset_Up;
   }
   else if(strcmp(method,"reqPreviousPreset") == 0)
   {
        request = Tuner_Req_Skip_Preset_Down;
   }
   else if(strcmp(method,"reqGetD3Signal") == 0)
   {
	   request = Tuner_Req_Get_D3_Signal;
   }
   else if(strcmp(method,"reqGetInfoCurrentStation") == 0)
   {
	   request = Tuner_Req_Get_Info_Current_Station;
   }
   else if(strcmp(method,"DeleteTunerData") == 0)
   {
	   request = Tuner_Req_Clear_Presets;
   }
   else
   {
     ;
   }
   return request;
}


/*......................................................................................................................
 * @brief handleTunerRequest()
 *
 * @param
 *
 * @returns 0 : Success
 *         -1 : Failure
 *
 *....................................................................................................................*/

bool CTunerCmdInvoker::handleTunerRequest(CSvcIpcRequest* reqCommand)
{

   TRC_SCOPE(TunerService, CTunerCmdInvoker, handleTunerRequest);
   DBG_MSG(("[CTunerCmdInvoker]:handleTunerRequest"));

   tunerReqTypes tunerReq = Tuner_Req_Invalid;
   tunerReq = getEnumFromMethod((char*)(reqCommand->getMethod()));
   switch(tunerReq)
   {
     case Tuner_Start_Seek:
     case Tuner_Req_AbortSeek:
     case Tuner_Req_Set_Frequency:
     case Tuner_Req_Get_Frequency:
     case Tuner_Req_Set_AFSwitch:
     case Tuner_Req_Set_Waveband:
     case Tuner_Req_Set_HDMode:
     case Tuner_Req_Set_BallgameMode:
     case Tuner_Req_Set_RegMode:
     case Tuner_Req_Set_StationSortingFlag:
     case Tuner_Req_Get_StationList:
     case Tuner_Req_Get_Info_Current_Station:
     {
       //Corresponding concrete command object created
       CTunerSeekHandler* seek = new CTunerSeekHandler();
       //Proxy pointer is created
       CTunerCommand* cmd = NULL;
       cmd = seek;
       cmd->execute(reqCommand);
       delete cmd;
       break;
     }
     case Tuner_Req_Store_Preset:
     {
       bool status = reqCommand->getParms().get("waveband", Json::Value("")).isString();

       if(true == status)
       {
		   std::string band = reqCommand->getParms().get("waveband", Json::Value("")).asString();
		   if("FM" == band)
		   {
			 CTunerPresetMgr* mPreset = new CTunerPresetFM();
			 mPreset->storePresets(reqCommand);
			 delete mPreset;
		   }
		   else if("AM" == band)
		   {
			 CTunerPresetMgr* mPreset = new CTunerPresetAM();
			 mPreset->storePresets(reqCommand);
			 delete mPreset;
		   }
		   else if("WB" == band)
		   {
			 CTunerPresetMgr* mPreset = new CTunerPresetWB();
			 mPreset->storePresets(reqCommand);
			 delete mPreset;
		   }
		   else
		   {
			 CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Invalid_Param);
		   }
       }
       else
       {
    	  CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Invalid_Param);
       }
       break;
     }
   case Tuner_Req_Select_Preset:
   {
	 bool status = reqCommand->getParms().get("waveband", Json::Value("")).isString();

	 if(true == status)
	 {
		 std::string band = reqCommand->getParms().get("waveband", Json::Value("")).asString();
		 if("FM" == band)
		 {
		   CTunerPresetMgr* mPreset = new CTunerPresetFM();
		   mPreset->selectPresets(reqCommand);
		   delete mPreset;
		 }
		 else if("AM" == band)
		 {
		   CTunerPresetMgr* mPreset = new CTunerPresetAM();
		   mPreset->selectPresets(reqCommand);
		   delete mPreset;
		 }
		 else if("WB" == band)
		 {
		   CTunerPresetMgr* mPreset = new CTunerPresetWB();
		   mPreset->selectPresets(reqCommand);
		   delete mPreset;
		 }

		 else
		 {
		   CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Invalid_Param);
		 }
	 }
	 else
	 {
       CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Invalid_Param);
	 }
     break;
   }

   case Tuner_Req_Auto_Store_Preset:
   {
       bool status = reqCommand->getParms().get("waveband", Json::Value("")).isString();

       if(true == status)
       {
    	   proxy tunerData;
		   std::string band = reqCommand->getParms().get("waveband", Json::Value("")).asString();
		   CTunerCmdInvoker::eListType = reqCommand->getParms().get("eListType", Json::Value("")).asInt();
		   DBG_MSG(("Tuner_Req_Auto_Store_Preset:eListType:%d",CTunerCmdInvoker::eListType));
		   if("AM" == band)
		   {
			   DBG_MSG(("AM AUTO STORE"));
			   //tunerData.token = (unsigned int)token;
			   tunerData.token = TOKEN;
			   tunerData.mode = DMMTunerStation_SEEK_AUTOSTORE;
			   DBG_MSG(("1"));
			   //DMMTunerStation_SeekMode mode = DMMTunerStation_SEEK_AUTO_UP;
			   CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Success);
			   DBG_MSG(("2"));
			   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);
		   }
		   else if("WB" == band)
		   {
			   DBG_MSG(("WB AUTO STORE"));
			   //tunerData.token = (unsigned int)token;
			   tunerData.token = TOKEN;
			   tunerData.mode = DMMTunerStation_SEEK_AUTOSTORE;

			   //DMMTunerStation_SeekMode mode = DMMTunerStation_SEEK_AUTO_UP;
			   CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Success);
			   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);
		   }
		   else
		   {
			 CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Invalid_Param);
		   }
       }
       else
       {
    	  CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Invalid_Param);
       }
       break;

   }
   case Tuner_Req_Get_Preset:
   {
     bool status = reqCommand->getParms().get("waveband", Json::Value("")).isString();
     if(true == status)
     {
		 std::string band = reqCommand->getParms().get("waveband", Json::Value("")).asString();
		 if("FM" == band)
		 {
		   CTunerPresetMgr* mPreset = new CTunerPresetFM();
		   mPreset->getPresets(reqCommand);
		   delete mPreset;
		 }
		 else if("AM" == band)
		 {
		   CTunerPresetMgr* mPreset = new CTunerPresetAM();
		   mPreset->getPresets(reqCommand);
		   delete mPreset;
		 }
		 else if("WB" == band)
		 {
		   CTunerPresetMgr* mPreset = new CTunerPresetWB();
		   mPreset->getPresets(reqCommand);
		   delete mPreset;
		 }
		 else
		 {
		   CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Invalid_Param);
		 }
     }
     else
     {
    	 CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Invalid_Param);
     }
     break;
   }
   case Tuner_Req_Skip_Preset_Up:
   {
	     tunerBandTypes band = CTunerSeekHandler::mCurrentTunerBand;
   		 if(Tuner_Band_FM == band)
   		 {
   		   CTunerPresetMgr* mPreset = new CTunerPresetFM();
   		   mPreset->nextPreset(reqCommand);
   		   delete mPreset;
   		 }
   		 else if(Tuner_Band_AM == band)
   		 {
   		   CTunerPresetMgr* mPreset = new CTunerPresetAM();
   		   mPreset->nextPreset(reqCommand);
   		   delete mPreset;
   		 }
   		 else if(Tuner_Band_WB == band)
   		 {
   		   CTunerPresetMgr* mPreset = new CTunerPresetWB();
   		   mPreset->nextPreset(reqCommand);
   		   delete mPreset;
   		 }
   		 else
   		 {
   		   CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Invalid_Param);
   		 }
   		 break;

   }
   case Tuner_Req_Skip_Preset_Down:
   {
   	     tunerBandTypes band = CTunerSeekHandler::mCurrentTunerBand;
		 if(Tuner_Band_FM == band)
		 {
		   CTunerPresetMgr* mPreset = new CTunerPresetFM();
		   mPreset->previousPreset(reqCommand);
		   delete mPreset;
		 }
		 else if(Tuner_Band_AM == band)
		 {
		   CTunerPresetMgr* mPreset = new CTunerPresetAM();
		   mPreset->previousPreset(reqCommand);
		   delete mPreset;
		 }
		 else if(Tuner_Band_WB == band)
		 {
		   CTunerPresetMgr* mPreset = new CTunerPresetWB();
		   mPreset->previousPreset(reqCommand);
		   delete mPreset;
		 }
		 else
		 {
		   CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Invalid_Param);
		 }
		 break;

      }
   case Tuner_Req_Scan:
   {
     //Corresponding concrete command object created
     CTunerScanHandler* scan = CTunerScanHandler::getScanHandler();
     //Proxy pointer is created
     CTunerCommand* cmd;
     cmd = scan;
     cmd->execute(reqCommand);
     break;
   }
   case Tuner_Req_AbortScan:
   {
     //Corresponding concrete command object created
     CTunerScanHandler* scan = CTunerScanHandler::getScanHandler();
     //Proxy pointer is created
     CTunerCommand* cmd;
     cmd = scan;
     cmd->execute(reqCommand);
     break;
   }
   case Tuner_Req_Get_Properties:
   {
     Json::Value availableBands (Json::arrayValue);
     Json::Value propsRqArray ( Json::arrayValue );
     Json::Value waveBand;
     Json::Value Bands;
     propsRqArray = reqCommand->getParms( ).get( "props", Json::Value::null );
     std::string propRq;
     unsigned int j = 0;
     Json::FastWriter mJsonWriter;
     int size = propsRqArray.size( );
     for ( int i = 0; i < size; i++ )
     {
       propRq = propsRqArray[ i ].asString( );
       if ( 0 == propRq.compare("availableBands") )
       {
         availableBands[j] = "fm";
         j++;
         availableBands[j] = "am";
         j++;
         availableBands[j] = "wb";
         Bands["availableBands"] = availableBands;
         waveBand["value"] = availableBands;
        bool status = CTunerCmdInvoker::GetInstance()->GetPersistencyFlag();
		if(status == true)
		{
		DBG_MSG(("[CTunerCmdInvoker]::GetProperties: sending available Band signal since persistency read completed"));
		CTunerRequestResponseHandler::handleNotif("availableBands",waveBand);
		}
		else
		{
		DBG_MSG(("[CTunerCmdInvoker]::GetProperties: Not sending available Band signal till Persistency Read completion"));
		}
         reqCommand->returnReply(Bands);
        }
        else if(0 == propRq.compare("stationList"))
        {
          CTunerSeekHandler* pSeek = new CTunerSeekHandler();
          Json::Value obj = pSeek->sendStationList();
          reqCommand->returnReply(obj);
          delete pSeek;
        }
        else if(0 == propRq.compare("currentWaveband"))
	    {
		   std::string signalName = "waveband";
		   Json::Value data;
		   if(CTunerSeekHandler::getCurrBand() == Tuner_Band_FM)
		   {
			 data["waveband"] = "fm";
		   }
		   else if(CTunerSeekHandler::getCurrBand() == Tuner_Band_AM)
		   {
			 data["waveband"] = "am";
		   }
		   else if(CTunerSeekHandler::getCurrBand() == Tuner_Band_WB)
		   {
			 data["waveband"] = "wb";
		   }
		   else
		   {
			  ;
		   }

		   CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Success);
		   CTunerRequestResponseHandler::handleNotif(signalName,data);
	   }
       else
       {
        	;
       }
      }
     //return isSuccess;
     // get the properties
     // return the response
     break;
   }
   case Tuner_Req_Set_TAMode:
   {
     //Corresponding concrete command object created
     CTunerAnnouncementHandler* announcement = CTunerAnnouncementHandler::getAnnouncementHandler();
     //Proxy pointer is created
     CTunerCommand* cmd;
     cmd = announcement;
     cmd->execute(reqCommand);
     break;
   }
   case Tuner_Req_Cancel_TA:
   {
     //Corresponding concrete command object created
     CTunerAnnouncementHandler* announcement = CTunerAnnouncementHandler::getAnnouncementHandler();
     //Proxy pointer is created
     CTunerCommand* cmd;
     cmd = announcement;
     cmd->execute(reqCommand);
     break;
   }
   case Tuner_Req_Get_SignalQuality:
   {
     Json::Value signalQuality;
     signalQuality["signalQuality"] = CTunerSeekHandler::getCurrentstationQuality();
     CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Success);
     CTunerRequestResponseHandler::handleNotif("signalQuality",signalQuality);
     break;
   }
   case Tuner_Req_Get_FieldStrength:
   case Tuner_Req_Get_SoftMute_Switch:
   case Tuner_Req_Get_High_Cut_Switch:
   case Tuner_Req_Get_Stereo_High_Blend_Switch:
   case Tuner_Req_Get_Stereo_Blend_Switch:
   case Tuner_Req_Get_Quality_Threshold_Entry:
   case Tuner_Req_Get_Quality_Threshold_Exit:
   case Tuner_Req_Get_HD_Audio_Mode:
   case Tuner_Req_Get_HD_FM_BlendTime_AD:
   case Tuner_Req_Get_HD_FM_BlendTime_DA:
   case Tuner_Req_Get_HD_AM_BlendTime_AD:
   case Tuner_Req_Get_HD_AM_BlendTime_DA:
   case Tuner_Req_Get_HD_BER_Mode:
   case Tuner_Req_Get_HD_Audio_Splitting:
   case Tuner_Req_Get_HD_Blending_Mode:
   case Tuner_Req_Set_SoftmuteSwitch:
   case Tuner_Req_Set_High_Cut_Switch:
   case Tuner_Req_Set_Stereo_High_Blend_Switch:
   case Tuner_Req_Set_Stereo_Blend_Switch:
   case Tuner_Req_Set_Quality_Threshold_Entry:
   case Tuner_Req_Set_Quality_Threshold_Exit:
   case Tuner_Req_Set_HD_BER_Mode:
   case Tuner_Req_Set_HD_Audio_Splitting:
   {
     //Corresponding concrete command object created
     CTunerDiagnosticHandler* diagnostic = new CTunerDiagnosticHandler();
     //Proxy pointer is created
     CTunerCommand* cmd;
     cmd = diagnostic;
     cmd->execute(reqCommand);
     delete cmd;
     break;
   }
   case Tuner_Req_Get_D3_Signal:
   {
	if(true == (CDMMAmFmTunerDiranaControlProxy::D3status))
	{
	   CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Success);
	   std::string sig = "diranaFlashStatus";
	   Json::Value obj;
	   obj[""] = "";
	   CTunerRequestResponseHandler::handleNotif(sig,obj);
	}
	else
	{
	   CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Error);
	}
	break;
   }
   case Tuner_Req_Clear_Presets:
   {
	   DBG_MSG(("[CTunerCmdInvoker]::Clearing the presets for AM/FM/WB"));
	   CTunerPresetAM::presetListAM.clear();
	   CTunerPresetFM::presetListFM.clear();
	   CTunerPresetWB::presetListWB.clear();
	   std::string sig = "DeletedTunerData";
	   Json::Value obj;
	   obj[""] = "";
	   CTunerRequestResponseHandler::handleNotif(sig,obj);
	   DBG_MSG(("[CTunerCmdInvoker]::Cleared the presets for AM/FM/WB"));

   }
   default:
     DBG_MSG(("[CTunerCmdInvoker]: Request Error"));
     CTunerRequestResponseHandler::sendResponse(reqCommand, Tuner_Response_Invalid_Request);
     break;
   }
   return true;
}

//===================================================== End Of File ===================================================
