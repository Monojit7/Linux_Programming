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
 * @file CTunerSeekHandler.cpp
 * @ingroup TunerService
 * @author 
 * Public API for the Tuner core service library.
 *
 *---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
 ----------------------------------------------------------------------------------------------------------------------*/

#include "DMMTunerStationTypes.h"
#include "CTunerSeekHandler.hpp"
#include "jsoncpp/json.h"
#include "CTunerRespHandler.hpp"
#include "DMMTunerTypes.h"
#include <iostream>
#include "stdio.h"
#include "TraceMacros.hpp"
#include "DMMTunerStationTypes.h"
#include "CTunerPresets.hpp"



/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Defines
 ----------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * TraceClient Scope Definitions
 ----------------------------------------------------------------------------------------------------------------------*/
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, execute);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, seekUp);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, setFrequency);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, getFrequency);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, setAF);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, setWaveband);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, seekDown);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, manualSeekUp);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, manualSeekDown);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, abortSeek);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, setLastTunerAMFreq);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, setLastTunerFMFreq);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, setHDMode);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, setRegMode);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, stnListSortingbyFreq);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, stnListSortingbyAlpb);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, sendStationList);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, informationStnListforFreqSorting);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, informationStnListforAlphaSorting);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, getStationList);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, setStationSortingFlag);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, tuneLastFMStation);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, tuneLastAMStation);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, updateFrequency);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, setLastTunedHDService);
TRC_SCOPE_DEF(TunerService, CTunerSeekHandler, getInfoCurrentStation);


/*......................................................................................................................
 * @brief CTunerSeekHandler()- Constructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/
UINT32 CTunerSeekHandler::regionCode = 1;
bool CTunerSeekHandler::scanStatus = false;
bool CTunerSeekHandler::hdStatus = false;
bool CTunerSeekHandler::hdMode = true;
UINT32 CTunerSeekHandler::mLastTunedAMFrequency = CTunerSeekHandler::mAMMinFrequency;
UINT32 CTunerSeekHandler::mLastTunedFMFrequency = CTunerSeekHandler::mFMMinFrequency;
UINT32 CTunerSeekHandler::mLastTunedWBFrequency = CTunerSeekHandler::mWBMinFrequency;
tunerBandTypes CTunerSeekHandler::mCurrentTunerBand = Tuner_Band_Invalid;
UINT32 CTunerSeekHandler::mStationQuality = 0;
CDMMTunerStationProxy* CTunerSeekHandler::mProxy = NULL;
UINT32 CTunerSeekHandler::mFMMinFrequency = 0;
UINT32 CTunerSeekHandler::mFMMaxFrequency = 0;
UINT32 CTunerSeekHandler::mAMMinFrequency = 0;
UINT32 CTunerSeekHandler::mAMMaxFrequency = 0;
UINT32 CTunerSeekHandler::mWBMinFrequency = 0;
UINT32 CTunerSeekHandler::mWBMaxFrequency = 0;
UINT32 CTunerSeekHandler::mPIcode = 0;
INT32 CTunerSeekHandler::mCurrentHDService = -1;
INT32 CTunerSeekHandler::mLastHDService  = -1;
UINT32 CTunerSeekHandler::mTotalHDService = 0;
bool   CTunerSeekHandler::mStationSortingFlag = false;
struct stationInformation CTunerSeekHandler::stndata[48];
struct stationInformation CTunerSeekHandler::stndataAutoStore[18];
struct currentStationInformation CTunerSeekHandler::stnInfo;
int CTunerSeekHandler::stnSize=0;
std::list<int> CTunerSeekHandler::spsList;


CTunerSeekHandler::CTunerSeekHandler()
{
}

/*......................................................................................................................
 * @brief CTunerSeekHandler()- Destructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerSeekHandler::~CTunerSeekHandler() {
}

/*......................................................................................................................
 * @brief execute()
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

void CTunerSeekHandler::execute(CSvcIpcRequest* reqData) {
   TRC_SCOPE(TunerService, CTunerSeekHandler, execute);
   SVCIPC_tConstStr method = reqData->getMethod();
   SVCIPC_tUserToken token = reqData->getUserToken();
   int req = reqData->getParms().get("seek", Json::Value(Tuner_Req_Invalid)).asInt();
   DBG_MSG(("[CTunerSeekHandler]:execute and method is: %s",method));
   DBG_MSG(("[CTunerSeekHandler]:execute and req is:%d",req));

   if (strcmp(method, "reqStartSeek") == 0)
    {
       switch (req)
        {
            case 1: {
                manualSeekUp(token, reqData);
                break;
            }
            case 2: {
                manualSeekDown(token, reqData);
                break;
            }
            case 3: {
                seekUp(token, reqData);
                break;
            }
            case 4: {
                seekDown(token, reqData);
                break;
            }
            default: {
                CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Request);
                break;
            }


        }
    }
    else if (strcmp(method, "reqStopSeek") == 0) {
        abortSeek(token, reqData);
    } else if (strcmp(method, "reqSetFrequency") == 0) {
        setFrequency(token, reqData);
    } else if (strcmp(method, "reqGetFrequency") == 0) {
        getFrequency(token, reqData);
    } else if (strcmp(method, "reqSetAF") == 0) {
        setAF(token, reqData);
    } else if (strcmp (method, "setWaveband") == 0)	{
        setWaveband(token, reqData);
    } else if (strcmp (method, "reqSetHDMode") == 0) {
        setHDMode(token, reqData);
    }else if (strcmp (method, "reqSetBallgameMode") == 0) {
        setBallgameMode(token, reqData);
    }else if (strcmp (method, "reqSetRegMode") == 0) {
        setRegMode(token, reqData);
    }else if (strcmp (method, "reqSetStationSortingFlag") == 0) {
    	setStationSortingFlag(token, reqData);
    }else if (strcmp (method, "reqGetStationList") == 0) {
    	getStationList(token, reqData);
    }else if(strcmp (method, "reqGetInfoCurrentStation") == 0) {
        getInfoCurrentStation(token, reqData);
    }
    else
    {
        DBG_MSG(("[CTunerSeekHandler]: Request Tuner_Response_Invalid_Request"));
        CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Request);
    }
}
/*......................................................................................................................
 * @brief updateFrequency() - This function updates the min and max frequencies for AM and FM band depending on the region
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/
void CTunerSeekHandler::updateFrequency(INT32 region)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, updateFrequency);
   switch(region)
   {

     case 2:
     mAMMinFrequency = TUNER_AM_BASE_FREQ_US;
     mAMMaxFrequency = TUNER_AM_END_FREQ_US;
     mFMMinFrequency = TUNER_FM_BASE_FREQ_US;
     mFMMaxFrequency = TUNER_FM_END_FREQ_US;
     mWBMinFrequency = TUNER_WB_BASE_FREQ_US;
     mWBMaxFrequency = TUNER_WB_END_FREQ_US;

     if(mLastTunedFMFrequency > TUNER_FM_BASE_FREQ_US)
     {
    	 DBG_MSG(("Attribute already initialized in FM with %d",mLastTunedFMFrequency));
     }
     else
     {
    	 mLastTunedFMFrequency = TUNER_FM_BASE_FREQ_US;
     }
     if(mLastTunedAMFrequency > TUNER_AM_BASE_FREQ_US)
     {
    	 DBG_MSG(("Attribute already initialized in AM with %d",mLastTunedAMFrequency));
     }
     else
     {
    	 mLastTunedAMFrequency = TUNER_AM_BASE_FREQ_US;
     }
     if(mLastTunedWBFrequency > TUNER_WB_BASE_FREQ_US)
     {
    	 DBG_MSG(("Attribute already initialized in AM with %d",mLastTunedWBFrequency));
     }
     else
     {
    	 mLastTunedWBFrequency = TUNER_WB_BASE_FREQ_US;
     }
     break;



     default:
         mAMMinFrequency = TUNER_AM_BASE_FREQ_US;
         mAMMaxFrequency = TUNER_AM_END_FREQ_US;
         mFMMinFrequency = TUNER_FM_BASE_FREQ_US;
         mFMMaxFrequency = TUNER_FM_END_FREQ_US;
         mWBMinFrequency = TUNER_WB_BASE_FREQ_US;
         mWBMaxFrequency = TUNER_WB_END_FREQ_US;
       	 mLastTunedFMFrequency = TUNER_FM_BASE_FREQ_US;
         mLastTunedAMFrequency = TUNER_AM_BASE_FREQ_US;
         mLastTunedWBFrequency = TUNER_WB_BASE_FREQ_US;
         break;
   }
}
/*......................................................................................................................
 * @brief seekUp()- Function to initiate the seek up functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::seekUp(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, seekUp);
   DBG_MSG(("[CTunerSeekHandler]:seekUp"));
   CTunerPresetFM::reqStatus = false;
   CTunerPresetAM::reqStatusAM = false;
   //Copy data to be send into a structure
   proxy tunerData;
   tunerData.spsNo = -1;
   if(mCurrentTunerBand == Tuner_Band_FM)
   {

		   DBG_MSG(("Perform seek to next valid frequency"));
		   //tunerData.token = (unsigned int)token;
		   tunerData.token = TOKEN;
		   tunerData.mode = DMMTunerStation_SEEK_AUTO_UP;
		   tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;

		   //DMMTunerStation_SeekMode mode = DMMTunerStation_SEEK_AUTO_UP;

		   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);

   }
   else
   {
	   DBG_MSG(("Current band is not FM hence perform seek to next valid frequency"));
	   //tunerData.token = (unsigned int)token;
	   tunerData.token = TOKEN;
	   tunerData.mode = DMMTunerStation_SEEK_AUTO_UP;

	   //DMMTunerStation_SeekMode mode = DMMTunerStation_SEEK_AUTO_UP;

	   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);
   }

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}

/*......................................................................................................................
 * @brief setFrequency()- Function to tune to a frequency
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::setFrequency(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, setFrequency);
   CTunerPresetFM::reqStatus = false;
   CTunerPresetAM::reqStatusAM = false;
   bool status = reqData->getParms().get("frequency", Json::Value(0)).isInt();
   bool stat = reqData->getParms().get("piCode", Json::Value(0)).isInt();
   printf("Calling SetFrequency \n");
   if((true == status) && (true == stat))
   {
	   UINT32 frequency = reqData->getParms().get("frequency", Json::Value(0)).asInt();
       UINT32 piCode = reqData->getParms().get("piCode", Json::Value(0)).asUInt();
	   DBG_MSG(("[CTunerSeekHandler]: setFrequency with : %d and piCode as : %d",frequency,piCode));
	   printf("[CTunerSeekHandler]: setFrequency with : %d and piCode as : %d\n",frequency,piCode);

	   if(((frequency >= mFMMinFrequency) && (frequency <= mFMMaxFrequency)))
	   {
		   printf("Calling SetFrequency for FM \n");
		     mCurrentTunerBand = Tuner_Band_FM;
		     //Copy data to be send into a structure
		     proxy tunerData;
			 tunerData.frequency = frequency;
			 tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
			 tunerData.spsNo = 0;
			 //tunerData.token = (unsigned int)token;
			 tunerData.token = TOKEN;
             tunerData.piCode = piCode;
             if(0 != piCode)
             {
            	 tunerData.selectionType = Tuner_Select_Station_List_Tune;
             }
             else if(0 == piCode)
             {
            	 tunerData.selectionType = Tuner_Select_Direct_Tune;
             }
             printf("Calling postInternalEvent for FM \n");
             (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);

			 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
	   }
	   else if(((frequency >= mAMMinFrequency) && (frequency <= mAMMaxFrequency)))
	   {
		    mCurrentTunerBand = Tuner_Band_AM;
		    printf("Calling postInternalEvent for AM \n");
		    //Copy data to be send into a structure
		     proxy tunerData;
			 tunerData.frequency = frequency;
			 tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
			 tunerData.spsNo = 0;
			 //tunerData.token = (unsigned int)token;
			 tunerData.token = TOKEN;
             tunerData.piCode = 0;
             tunerData.selectionType = Tuner_Select_Direct_Tune;
             printf("Calling postInternalEvent for FM \n");
			 (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);

			 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
	   }
	   else if(((frequency >= mWBMinFrequency) && (frequency <= mWBMaxFrequency)))
	   {
		    mCurrentTunerBand = Tuner_Band_WB;
		    printf("Calling postInternalEvent for WB \n");
		    //Copy data to be send into a structure
		     proxy tunerData;
			 tunerData.frequency = frequency;
			 tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
			 tunerData.spsNo = 0;
			 //tunerData.token = (unsigned int)token;
			 tunerData.token = TOKEN;
             tunerData.piCode = 0;
             tunerData.selectionType = Tuner_Select_Direct_Tune;
             printf("Calling postInternalEvent for WB \n");
			 (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);

			 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
	   }
	   else
	   {
		 DBG_MSG(("[CTunerSeekHandler]: Frequency Range Error"));
		 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
		 return;
	   }

   }
   else
   {
	  DBG_MSG(("[CTunerSeekHandler]: Invalid type of parameter"));
	  CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }
}
/*......................................................................................................................
 * @brief getFrequency()- Function to get the lasted tuned frequency
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::getFrequency(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, getFrequency);
   DBG_MSG(("[CTunerSeekHandler]: getFrequency"));
   std::string signalName = "frequency";
   Json::Value data;

   if(mCurrentTunerBand == Tuner_Band_FM)
    {
	  DBG_MSG(("[CTunerSeekHandler]: getFrequencyFM : %d",mLastTunedFMFrequency));
      data["frequency"] = mLastTunedFMFrequency;
      data["waveband"] = "fm";
    }
   else if(mCurrentTunerBand == Tuner_Band_AM)
    {
	   DBG_MSG(("[CTunerSeekHandler]: getFrequencyAM : %d",mLastTunedAMFrequency));
      data["frequency"] = mLastTunedAMFrequency;
      data["waveband"] = "am";
    }
   else if(mCurrentTunerBand == Tuner_Band_WB)
    {
	   DBG_MSG(("[CTunerSeekHandler]: getFrequencyWB %d",mLastTunedWBFrequency));
      data["frequency"] = mLastTunedWBFrequency;
      data["waveband"] = "wb";
    }
   else
   {
     data["frequency"] = TUNER_FM_BASE_FREQ_US;
   }

  // data["frequency"] = mLastTunedFMFrequency;
   //CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

   Json::FastWriter mJsonWriter;

   CTunerRequestResponseHandler::handleNotif(signalName,data);

     reqData->returnReply(data);

}
/*......................................................................................................................
 * @brief setAF()- Function to get the set the alternate frequency setting
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerSeekHandler::setAF(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, setAF);
   BOOL mode = reqData->getParms().get("mode", Json::Value(false)).asBool();

   DBG_MSG(("[CTunerSeekHandler]: setAF"));

   //Copy data to be send into a structure
   proxy tunerData;
   tunerData.switchFlag = mode;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;

   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetAfSwitch, (void*) &tunerData);

   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}

/*......................................................................................................................
 * @brief setWaveband()- Function to set the waveband for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::setWaveband(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, setWaveband);
   //INT32 waveBand = reqData->getParms().get("waveband", Json::Value(Tuner_Req_Invalid)).as asInt();
   CTunerPresetFM::reqStatus = false;
   CTunerPresetAM::reqStatusAM = false;
   bool status = reqData->getParms().get( "waveband", Json::Value(Json::Value::null)).isString();

   if(true == status)
   {
	   std::string waveBand = reqData->getParms().get( "waveband", Json::Value(Json::Value::null)).asString( );
	   Json::Value data;
	   proxy tunerData;

	   DBG_MSG(("[CTunerSeekHandler]: setWaveband"));

	   //tunerData.token = (unsigned int)token;
	   tunerData.token = TOKEN;
	   tunerData.selectionType = Tuner_Select_Direct_Tune;
	   tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;

	  if(CTunerRequestResponseHandler::vrStatus == true)
	  {
		 DBG_MSG(("[CTunerSeekHandler]: setWaveband not invoked since previous interrupt was VR"));
		 CTunerRequestResponseHandler::vrStatus = false;
	  }
	  else if(CTunerRequestResponseHandler::taStatus == true)
	  {
	     DBG_MSG(("[CTunerSeekHandler]: setWaveband not invoked since previous interrupt was TA"));
		 CTunerRequestResponseHandler::taStatus = false;
	  }
	  else
	  {
	   if(waveBand == "AM")
	   {
		 DBG_MSG(("[setWaveband]: Setting the AM waveband with frequency %d",mLastTunedAMFrequency));
		 tunerData.frequency = mLastTunedAMFrequency;
		 tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
		 mCurrentTunerBand = Tuner_Band_AM;
		 data["waveband"] = "am";
		 (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->sendInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
		 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
		 CTunerRequestResponseHandler::handleNotif("waveband",data);
	   }
	   else if(waveBand == "FM")
	   {
		 DBG_MSG(("[setWaveband]: Setting the FM waveband with frequency %d",mLastTunedFMFrequency));
		 tunerData.frequency = mLastTunedFMFrequency;
		 tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
		 mCurrentTunerBand = Tuner_Band_FM;
		 data["waveband"] = "fm";
		 (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->sendInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
		 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
		 CTunerRequestResponseHandler::handleNotif("waveband",data);
	   }
	   else if(waveBand == "WB")
	   {
		 DBG_MSG(("[setWaveband]: Setting the WB waveband with frequency %d",mLastTunedWBFrequency));
		 tunerData.frequency = mLastTunedWBFrequency;
		 //tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
		 mCurrentTunerBand = Tuner_Band_WB;
		 data["waveband"] = "wb";
		 (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->sendInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
		 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
		 CTunerRequestResponseHandler::handleNotif("waveband",data);
	   }
	   else
	   {
		 DBG_MSG(("Invalid Parameter"));
		 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
	   }

     }
   }
   else
   {
     DBG_MSG(("Invalid Parameter Type"));
	 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }

}

/*......................................................................................................................
 * @brief seekDown()- Function to initiate the seek down functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerSeekHandler::seekDown(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, seekDown);
   DBG_MSG(("[CTunerSeekHandler]:seekDown"));
   CTunerPresetFM::reqStatus = false;
   CTunerPresetAM::reqStatusAM = false;
   //Copy data to be send into a structure
   proxy tunerData;
   tunerData.spsNo = -1;
   if(mCurrentTunerBand == Tuner_Band_FM)
   {

		   DBG_MSG(("Perform seek to previous valid frequency"));
		   //tunerData.token = (unsigned int)token;
		   tunerData.token = TOKEN;
		   tunerData.mode = DMMTunerStation_SEEK_AUTO_DOWN;
		   tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;

		   //DMMTunerStation_SeekMode mode = DMMTunerStation_SEEK_AUTO_UP;

		   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);

   }
   else
   {
	   DBG_MSG(("Current band is not FM hence perform seek to previous valid frequency"));
	   //tunerData.token = (unsigned int)token;
	   tunerData.token = TOKEN;
	   tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
	   tunerData.mode = DMMTunerStation_SEEK_AUTO_DOWN;

	   //DMMTunerStation_SeekMode mode = DMMTunerStation_SEEK_AUTO_UP;

	   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);
   }

   //Send response to HMI
   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
/*......................................................................................................................
 * @brief manualSeekUp()- Function to initiate the manual seek up functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerSeekHandler::manualSeekUp(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, manualSeekUp);
   DBG_MSG(("[CTunerSeekHandler]:manualSeekUp"));
   CTunerPresetFM::reqStatus = false;
   CTunerPresetAM::reqStatusAM = false;
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
   tunerData.mode = DMMTunerStation_SEEK_MAN_UP;

   //Call DSI Interface
   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);

   //Send response to HMI
   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
/*......................................................................................................................
 * @brief manualSeekDown()- Function to initiate the manual seek down functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerSeekHandler::manualSeekDown(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, manualSeekDown);
   DBG_MSG(("[CTunerSeekHandler]: manualSeekDown"));
   CTunerPresetFM::reqStatus = false;
   CTunerPresetAM::reqStatusAM = false;

   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
   tunerData.mode = DMMTunerStation_SEEK_MAN_DOWN;

   //Call DSI Interface
   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);

   //Send response to HMI
   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}

/*......................................................................................................................
 * @brief abortSeek()- Function to initiate the abort seek functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerSeekHandler::abortSeek(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, abortSeek);
   DBG_MSG(("[CTunerSeekHandler]: abortSeek"));
   CTunerPresetFM::reqStatus = false;
   CTunerPresetAM::reqStatusAM = false;
   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.mode = DMMTunerStation_SEEK_OFF;

   //Call DSI Interface
   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestControlSeek,(void*)&tunerData);

   //Send response to HMI
   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}

/*......................................................................................................................
 * @brief setHDMode()- Function to set the HD functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerSeekHandler::setHDMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, setHDMode);
   DBG_MSG(("[CTunerSeekHandler]: reqSetHDMode"));
   BOOL mode = reqData->getParms().get("hdMode", Json::Value(false)).asBool();

   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;

   if(true == mode)
   {
     tunerData.hdMode = DMMAmFmTunerControl_HDMODE_AUTOMATIC;
     hdMode = true;
   }
   else
   {
     tunerData.hdMode = DMMAmFmTunerControl_HDMODE_ANALOG;
     hdMode = false;
     std::string signalName = "infoHDStatus";
     Json::Value data;
     data["hdStatus"] = 0;
     CTunerRequestResponseHandler::handleNotif(signalName,data);
   }

   //Call DSI Interface
   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetHdMode,(void*)&tunerData);

   //Send response to HMI
   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
/*......................................................................................................................
 * @brief setBallgameMode()- Function to set the ballgame mode functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerSeekHandler::setBallgameMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, setHDMode);
   DBG_MSG(("[CTunerSeekHandler]: setBallgameMode"));
   BOOL mode = reqData->getParms().get("bgMode", Json::Value(false)).asBool();

   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   if(true == mode)
   {
     tunerData.bgMode = DMMAmFmTunerControl_HDBGMODE_ALWAYS_ANALOG;
   }
   else
   {
     tunerData.bgMode = DMMAmFmTunerControl_HDBGMODE_ALWAYS_DIGITAL;
   }

   //Call DSI Interface
  // (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetHdBallGameMode,(void*)&tunerData);

   //Send response to HMI
   //CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
/*......................................................................................................................
 * @brief setStationSortingFlag()- Function to set the flag to identify the type of sorting required for the station list in the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::setStationSortingFlag(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
  TRC_SCOPE(TunerService, CTunerSeekHandler, setStationSortingFlag);
  DBG_MSG(("[CTunerSeekHandler]: setStationSortingFlag"));
  BOOL mode = reqData->getParms().get("sortingMode", Json::Value(false)).asBool();
  DBG_MSG(("[CTunerSeekHandler]: setStationSortingFlag flag recieved is %d",mode));
  mStationSortingFlag = mode;
  //Send response to HMI
  CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
/*......................................................................................................................
 * @brief getStationList()- Function to provide the sorted station list to the HMI.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::getStationList(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
	TRC_SCOPE(TunerService, CTunerSeekHandler, getStationList);
	DBG_MSG(("[CTunerSeekHandler]: getStationList"));

	//Send response to HMI
	CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);


		DBG_MSG(("[CTunerSeekHandler]:getStationList Sorted by Frequency"));
		stnListSortingbyFreq();


	Json::Value obj = sendStationList();
	std::string signalName = "stationList";
	CTunerRequestResponseHandler::handleNotif(signalName,obj);
}

/*......................................................................................................................
 * @brief setRegMode()- Function to set the Regionalization mode functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerSeekHandler::setRegMode(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, setRegMode);
   DBG_MSG(("[CTunerSeekHandler]: setRegMode"));
   BOOL mode = reqData->getParms().get("regMode", Json::Value(false)).asBool();

   //Copy data to be send into a structure
   proxy tunerData;
   //tunerData.token = (unsigned int)token;
   tunerData.token = TOKEN;
   tunerData.switchFlag = mode;


   //Call DSI Interface
   (CTunerRequestResponseHandler::GetInstance()->getControlProxy())->postInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetRegMode,(void*)&tunerData);

   //Send response to HMI
   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
/*......................................................................................................................
 * @brief setLastTunerAMFreq()- Function to store the last tuned AM frequency for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::setAMLastTunedFreq(UINT32 freq)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, setLastTunerAMFreq);
   DBG_MSG(("Last tuned AM frequency is %d", freq));
   if((freq >= mAMMinFrequency) && (freq <= mAMMaxFrequency))
   {
      mLastTunedAMFrequency = freq;
   }
   else
   {
	   DBG_WARNING(("Invalid AM frequency is received...."));
	   DBG_MSG(("Current Last tuned AM frequency is ----------> %d", mLastTunedAMFrequency));
   }
}
/*......................................................................................................................
 * @brief setLastTunerFMFreq()- Function to store the last tuned FM frequency for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::setFMLastTunedFreq(UINT32 freq)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, setLastTunerFMFreq);
   DBG_MSG(("Last tuned FM frequency is %d", freq));
   if((freq >= mFMMinFrequency) && (freq <= mFMMaxFrequency))
   {
      mLastTunedFMFrequency = freq;
   }
   else
   {
	   DBG_WARNING(("Invalid FM frequency is received...."));
	   DBG_MSG(("Current Last tuned FM frequency is ----------> %d", mLastTunedFMFrequency));
   }
}


/*......................................................................................................................
 * @brief setLastTunerWBFreq()- Function to store the last tuned WB frequency for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::setWBLastTunedFreq(UINT32 freq)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, setLastTunerFMFreq);
   DBG_MSG(("Last tuned FM frequency is %d", freq));
   if((freq >= mWBMinFrequency) && (freq <= mWBMaxFrequency))
   {
      mLastTunedWBFrequency = freq;
   }
   else
   {
	   DBG_WARNING(("Invalid FM frequency is received...."));
	   DBG_MSG(("Current Last tuned FM frequency is ----------> %d", mLastTunedWBFrequency));
   }
}

/*......................................................................................................................
 * @brief setLastTunerFMFreq()- Function to store the last tuned FM frequency for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::setLastTunedHDService(INT32 service)
{
   TRC_SCOPE(TunerService, CTunerSeekHandler, setLastTunedHDService);
   DBG_MSG(("Last tuned HD service is %d", service));
   mLastHDService = service;
   mCurrentHDService = service;
}

/*......................................................................................................................
 * @brief getHDStatus- Function to get the latest HD setting status for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

bool CTunerSeekHandler::getHDStatus()
{
   return hdMode;
}

/*......................................................................................................................
 * @brief getInfoCurrentStation - Function to get the information current station for the currently tuned station for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerSeekHandler::getInfoCurrentStation(SVCIPC_tUserToken token, CSvcIpcRequest* reqData)
{
	TRC_SCOPE(TunerService, CTunerSeekHandler, getInfoCurrentStation);
	DBG_MSG(("[CTunerSeekHandler] :getInfoCurrentStation"));
	cout << "[CTunerSeekHandler] :getInfoCurrentStation" << endl;
	Json::Value data;
	std::string signalName = "infoCurrentStation";
	data["currentService"] = mCurrentHDService;
	data["pi"] = mPIcode;
	if(Tuner_Band_FM == mCurrentTunerBand)
	{
       data["frequency"] = mLastTunedFMFrequency;
	}
	else if(Tuner_Band_AM == mCurrentTunerBand)
	{
	   data["frequency"] = mLastTunedAMFrequency;
	}
	data["psName"] = stnInfo.psName;
	data["radioText"] = stnInfo.radioText;
	//data["hdShortName"] = stnInfo.hdShortName;
	//data["hdSongName"] = stnInfo.hdSongName;
	//data["hdArtistName"] = stnInfo.hdArtistName;
 	CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
	CTunerRequestResponseHandler::handleNotif(signalName,data);
}
/*......................................................................................................................
 * @brief stnListSortingbyFreq- Function to sort the station list by frequency
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::stnListSortingbyFreq()
{
	TRC_SCOPE(TunerService, CTunerSeekHandler, stnListSortingbyFreq);
	DBG_MSG(("[CDMMTunerStationProxy]: stnListSortingbyFreq \n"));

	int k=0;
	struct stationInformation temp;

	if(stnSize > 0)
	{
		for (  int i = 1 ; i <= stnSize-1 ; i++ )
		{
			for (int j = 0 ; j < i ; j++ )
			{
				if ( stndata[j].freq > stndata[i].freq )
				{
					temp = stndata[j] ;
					stndata[j] = stndata[i] ;
					for ( k = i ; k > j ; k-- )
						stndata[k] = stndata[k - 1] ;

					stndata[k + 1] = temp ;
				}
			}
		}
    }
	else
	{
		DBG_MSG(("[CDMMTunerStationProxy]: stnListSortingbyFreq  Empty station list, no sorting performed !!"));
	}
}

/*......................................................................................................................
 * @brief stnListSortingbyAlpb- Function to sort the station list alphabetically
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerSeekHandler::stnListSortingbyAlpb()
{
	TRC_SCOPE(TunerService, CTunerSeekHandler, stnListSortingbyAlpb);
	DBG_MSG(("[CDMMTunerStationProxy]: stnListSortingbyAlpb \n"));

	int k=0;
	int c, d;
	struct stationInformation temp;
	char text[20];
	int flag;


	if(stnSize > 0)
	{
		for (int i=0;  i <= stnSize-1 ; i++ )
		{
			//char text[20];
			c=0;
			d=0;
			flag=0;
            //Checking for blank spaces that may be present in a station name
			while((stndata[i].name)[c] == ' ')
			{
				c++;
				flag=1;
			}

			if(flag==1)
			{
				//Copying the station name without the blank spaces into a temporary array
				while ((stndata[i].name)[c] != '\0')
				{
					text[d] = (stndata[i].name)[c];
					d++;
					c++;
				}
			}

			if(d>0)
			{
				//Copying the station name in the temporary array back into the structure
				text[d] = '\0';
				strcpy((stndata[i].name),text);
			}
		}

        //Alphabetical sorting is performed
		for (  int i = 1 ; i <= stnSize-1 ; i++ )
		{
			for (int j = 0 ; j < i ; j++ )
			{
				if (strcasecmp((stndata[j].name), (stndata[i].name))>0 )
				{
					temp = stndata[j] ;
					stndata[j] = stndata[i] ;
					for ( k = i ; k > j ; k-- )
						stndata[k] = stndata[k - 1] ;

					stndata[k + 1] = temp ;
				}
			}
		}
	}
	else
	{
		DBG_MSG(("[CDMMTunerStationProxy]: stnListSortingbyAlpb  Empty station list, no sorting performed !!"));
	}
}

/*......................................................................................................................
 * @brief sendStationList- Function to send the station list to HMI
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

Json::Value CTunerSeekHandler::sendStationList()
{
	TRC_SCOPE(TunerService, CTunerSeekHandler, sendStationList);
	DBG_MSG(("[CDMMTunerStationProxy]: sendStationList \n"));

	Json::Value obj(Json::objectValue);
	Json::Value stationList(Json::arrayValue);
	Json::Value data;
	//std::string signalName = "stationList";

	if(stnSize > 0)
	{
		for(int m=0; m < stnSize; m++)
		{
			data["stationName"] =stndata[m].name ;
			data["frequency"] =stndata[m].freq ;
			data["PiCode"] =stndata[m].picode ;
			data["pty"] = stndata[m].pty;
			DBG_MSG(("[CTunerSeekHandler]: sendStationList: [Frequency]  %d",stndata[m].freq));
			DBG_MSG(("[CTunerSeekHandler]: sendStationList [name] %s", stndata[m].name));
			DBG_MSG(("[CTunerSeekHandler]: sendStationList [pty] %d",stndata[m].pty));
			DBG_MSG(("[CTunerSeekHandler]: sendStationList [picode] %d",stndata[m].picode));
			stationList[m] = data;
			obj["stationList"] = stationList;
		}
	}
	else
	{
		obj["stationList"] = "";
	}

	obj["numOfStations"] = stnSize;
    return obj;
	//CTunerRequestResponseHandler::handleNotif(signalName,obj);
}

void CTunerSeekHandler::tuneLastFMStation()
{
	TRC_SCOPE(TunerService, CTunerSeekHandler, tuneLastFMStation);
	//Copy data to be send into a structure
	proxy tunerData;
	tunerData.token = TOKEN;
	DBG_MSG(("tuneLastFMStation: Setting the FM frequency %d ------------> ",mLastTunedFMFrequency));
	tunerData.frequency = mLastTunedFMFrequency;
	tunerData.spsNo = 0;
	(CTunerRequestResponseHandler::GetInstance()->getStationProxy())->sendInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
	//CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
void CTunerSeekHandler::tuneLastAMStation()
{
	TRC_SCOPE(TunerService, CTunerSeekHandler, tuneLastAMStation);
	//Copy data to be send into a structure
	proxy tunerData;
	tunerData.token = TOKEN;
	DBG_MSG(("tuneLastAMStation: Setting the AM frequency %d ------------> ",mLastTunedAMFrequency));
	tunerData.frequency = mLastTunedAMFrequency;
	tunerData.spsNo = 0;
	(CTunerRequestResponseHandler::GetInstance()->getStationProxy())->sendInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
	//CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
}
//----------------------------------------------------- END OF FILE ---------------------------------------------------
