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

#include "CTunerPresets.hpp"
#include "jsoncpp/json.h"
#include"TunerPersistency.hpp"
#include <iostream>
#include "TunerPersistency.hpp"
#include "TraceMacros.hpp"
#include "CTunerSeekHandler.hpp"
#include <algorithm>
std::list<CTunerPresetFM*> CTunerPresetFM::presetListFM;
std::list<CTunerPresetAM> CTunerPresetAM::presetListAM;

std::list<CTunerPresetWB> CTunerPresetWB::presetListWB;

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Defines
 ----------------------------------------------------------------------------------------------------------------------*/
unsigned int CTunerPresetFM::currentFrequency = 0;
int CTunerPresetFM::currentPresetNo = 0;
unsigned int CTunerPresetAM::currentFrequencyAM = 0;
stationInformation CTunerPresetAM::value;
int CTunerPresetAM::currentPresetNoAM = 0;
bool CTunerPresetFM::reqStatus = false;
int CTunerPresetFM::nextPresetNo = 0;
bool CTunerPresetAM::reqStatusAM = false;
int CTunerPresetAM::nextPresetNoAM = 0;

unsigned int CTunerPresetWB::currentFrequencyWB = 0;

int CTunerPresetWB::currentPresetNoWB = 0;
bool CTunerPresetWB::reqStatusWB = false;
int CTunerPresetWB::nextPresetNoWB = 0;

/*----------------------------------------------------------------------------------------------------------------------
 * TraceClient Scope Definitions
 ----------------------------------------------------------------------------------------------------------------------*/
TRC_SCOPE_DEF(TunerService, CTunerPresetFM, storePresets);
TRC_SCOPE_DEF(TunerService, CTunerPresetFM, selectPresets);
TRC_SCOPE_DEF(TunerService, CTunerPresetFM, retrievePresetFM);
TRC_SCOPE_DEF(TunerService, CTunerPresetAM, storePresets);
TRC_SCOPE_DEF(TunerService, CTunerPresetAM, selectPresets);
TRC_SCOPE_DEF(TunerService, CTunerPresetAM, retrievePresetAM);
TRC_SCOPE_DEF(TunerService, CTunerPresetAM, AutoStorePresetListUpdateAM);
TRC_SCOPE_DEF(TunerService, CTunerPresetFM, updatePresetNoFM);
TRC_SCOPE_DEF(TunerService, CTunerPresetAM, updatePresetNoAM);
TRC_SCOPE_DEF(TunerService, CTunerPresetFM, nextPreset);
TRC_SCOPE_DEF(TunerService, CTunerPresetAM, nextPreset);
TRC_SCOPE_DEF(TunerService, CTunerPresetFM, previousPreset);
TRC_SCOPE_DEF(TunerService, CTunerPresetAM, previousPreset);
TRC_SCOPE_DEF(TunerService, CTunerPresetWB, updatePresetNoWB);
TRC_SCOPE_DEF(TunerService, CTunerPresetWB, storePresets);
TRC_SCOPE_DEF(TunerService, CTunerPresetWB, selectPresets);
TRC_SCOPE_DEF(TunerService, CTunerPresetWB, nextPreset);
TRC_SCOPE_DEF(TunerService, CTunerPresetWB, previousPreset);
TRC_SCOPE_DEF(TunerService, CTunerPresetWB, getPresets);
TRC_SCOPE_DEF(TunerService, CTunerPresetWB, retrievePresetWB);


/*......................................................................................................................
 * @brief CTunerPresetFM()- Constructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerPresetFM::CTunerPresetFM() {
	frequency = 0;
	psName = "";
	presetNo = 0;
	//spsNo = -1;
	piCode = 0;
	hdDispStatus = 3;
}

/*......................................................................................................................
 * @brief CTunerPresetFM()- Destructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerPresetFM::~CTunerPresetFM() {
}
/*......................................................................................................................
 * @brief updatePresetNoFM()- Function to obtain the current preset No
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerPresetFM::updatePresetNoFM(unsigned int freq,INT32 spsNo, UINT32 piCode)
{
   TRC_SCOPE(TunerService, CTunerPresetFM, updatePresetNoFM);
   DBG_MSG(("[CTunerPresetFM]:updatePresetNoFM"));
   currentFrequency = freq;
   DBG_MSG(("[CTunerPresetFM]: Current frequency is %d",currentFrequency));
   currentPresetNo = 0;
   int size = (CTunerPresetFM::presetListFM).size();
   int prevPresetNo = 0;
   if(size != 0)
   {
	   std::list<CTunerPresetFM*>::iterator it ;
	   for(it = (CTunerPresetFM::presetListFM).begin();it!=((CTunerPresetFM::presetListFM).end());++it)
	   {
		 if(((*it)->frequency == currentFrequency) &&  ((*it)->piCode == piCode))
		 {
		   currentPresetNo = (*it)->presetNo;
		   if(reqStatus == true)
		   {
			 if(nextPresetNo != currentPresetNo)
		     {
		       currentPresetNo = nextPresetNo;
		       DBG_MSG(("[CTunerPresetFM]: Current preset No in condition check is %d",currentPresetNo));
		     }
		     else
		     {
			   ;
		     }
		   }
		   else
		   {
			   if((prevPresetNo > currentPresetNo) || (prevPresetNo == 0))
			   {
			      prevPresetNo = currentPresetNo;
			      DBG_MSG(("[CTunerPresetFM]: prevPresetNo is %d",prevPresetNo));
			   }
			   else
			   {
				   ;
			   }
		   }
		   DBG_MSG(("[CTunerPresetFM]: Current preset No is %d",currentPresetNo));
		   //return;
		 }
		 else
		 {
			DBG_MSG(("[CTunerPresetFM]: Stored preset does not match currently tuned station"));
			//currentPresetNo = 0;
		 }
	   }
	   if(prevPresetNo != 0)
	   {
		  currentPresetNo = prevPresetNo ;
	   }
	   else
	   {
		  ;
	   }

	  DBG_MSG(("[CTunerPresetFM]: Final Current preset No is %d",currentPresetNo));
   }
   else if(size == 0)
   {
	 DBG_MSG(("[CTunerPresetFM]: Empty list !!"));
   }
   else
   {
	 ;
   }

}

bool comp(const int& num1, const int& num2)
{
	return num1 < num2;
}


/*......................................................................................................................
 * @brief storePresets()- Function to initiate the preset store functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerPresetFM::storePresets(CSvcIpcRequest* reqData) {

   TRC_SCOPE(TunerService, CTunerPresetFM, storePresets);
   DBG_MSG(("[CTunerPresetFM]:storePreset"));
   CTunerPresetFM::reqStatus = false;
   int presetNo = reqData->getParms().get("presetNo", Json::Value(0)).asInt();
   unsigned int frequency = reqData->getParms().get("frequency", Json::Value(0)).asUInt();
   std::string psName = reqData->getParms().get("psName", Json::Value("")).asString();

   unsigned int piCode = CTunerSeekHandler::mPIcode;

   DBG_MSG(("[CTunerPresetFM]: presetNo & frequency & piCode is : %d & %d & %d",presetNo,frequency,piCode));
   //Populate data object into preset list

   CTunerPresetFM* presetData;
   presetData = new CTunerPresetFM();
   presetData->frequency = frequency;
   presetData->presetNo = presetNo;
   presetData->psName = psName;

   presetData->piCode = piCode;
   int size = (CTunerPresetFM::presetListFM).size();
   DBG_MSG(("[CTunerPresetFM]: Size of the list is --->%d",size));
   if((presetNo <= PRESET_MAX)&&(presetNo != 0))
    {
      if(size != 0)
      {
        bool status = false;
        std::list<CTunerPresetFM*>::iterator it ;
        for(it = (CTunerPresetFM::presetListFM).begin();it!=((CTunerPresetFM::presetListFM).end());++it)
        {
          if((*it)->presetNo == presetNo)
          {
             delete (*it);
        	 *it = presetData;
             status = true;
             currentPresetNo = presetNo;
             CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
             DBG_MSG(("[CTunerPresetFM]: Over write the preset no %d with new value %d",presetNo,(*it)->frequency));
          }
        }
       if(false == status)
       {
         DBG_MSG(("[CTunerPresetFM]: No match hence fill"));
         (CTunerPresetFM::presetListFM).push_back(presetData);
         currentPresetNo = presetNo;
         CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
       }
       else if(true == status)
       {
         DBG_MSG(("[CTunerPresetFM]: Preset filled"));
       }
       else
       {
         ;
       }
    }
    else if(size == 0)
    {
      DBG_MSG(("[CTunerPresetFM]: Empty list hence fill"));
      (CTunerPresetFM::presetListFM).push_back(presetData);
      currentPresetNo = presetNo;
      CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
    }
    else
    {
      ;
    }
   }
   else
   {
     DBG_MSG(("[CTunerPresetFM]: Incorrect preset number !"));
     CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Error);
   }
}
/*......................................................................................................................
 * @brief selectPresets()- Function to initiate the preset selection functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerPresetFM::selectPresets(CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerPresetFM, selectPresets);
   DBG_MSG(("[CTunerPresetFM]:selectPresets\n"));
   CTunerPresetFM::reqStatus = true;
   bool resp = reqData->getParms().get("presetNo", Json::Value(0)).isInt();

   if(true == resp)
   {
	   int presetNo = reqData->getParms().get("presetNo", Json::Value(0)).asInt();
	   //SVCIPC_tUserToken token = reqData->getUserToken();
	   int size = (CTunerPresetFM::presetListFM).size();
	   DBG_MSG(("[CTunerPresetFM]: Size of the list is --->%d",size));
	   if(size != 0 && size <= 18)
	   {
		 std::list<CTunerPresetFM*>::iterator it ;
		 bool status = false;
		 for(it = (CTunerPresetFM::presetListFM).begin();it!=((CTunerPresetFM::presetListFM).end());++it)
		 {
		   DBG_MSG(("[CTunerPresetFM]: Preset number from the list is %d ",(*it)->presetNo));
		   if((*it)->presetNo == presetNo)
		   {
			 nextPresetNo = presetNo;
			 //Copy data to be send into a structure
			 proxy tunerData;
			 tunerData.frequency = ((*it)->frequency);
			 tunerData.piCode = ((*it)->piCode);
			 tunerData.selectionType = Tuner_Select_Preset_Tune;
			 tunerData.token = TOKEN;
			 status = true;
			 DBG_MSG(("[CTunerPresetFM]: Frequency selected is %d",((*it)->frequency)));
			// DBG_MSG(("[CTunerPresetFM]: SPS No for selected frequency is %d",((*it)->spsNo)));

			 //Get HD setting status
			 bool hdStatus =  CTunerSeekHandler::getHDStatus();
			 //Check for HD setting and tune to selected preset

			   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
			   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

		   }
		   else
		   {
			 ;
		   }
		 }
		 if(false == status)
		 {
		   DBG_MSG(("[CTunerPresetFM]: Preset not available !!"));
		   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Error);
		 }
	   }
	   else if(size == 0)
	   {
		 DBG_MSG(("[CTunerPresetFM]: Empty list !!"));
		 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Error);
	   }
   }
   else
   {
	 DBG_MSG(("[CTunerPresetFM]: Invalid type of parameter"));
     CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid_Param);
   }
}

void CTunerPresetFM::nextPreset(CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerPresetFM, nextPreset);
   DBG_MSG(("[CTunerPresetFM]:nextPreset"));
   reqStatus = true;
   if(currentPresetNo != 0)
   {
	   int presetNum;
	   if(currentPresetNo != 18)
	   {
	     presetNum = currentPresetNo + 1;
	   }
	   else
	   {
		 presetNum = 1;
	   }
	   std::list<CTunerPresetFM*>::iterator it;
	   bool status = true;
	   DBG_MSG(("[CTunerPresetFM]: nextPreset Current Preset Number is %d",currentPresetNo));
	   while(status)
	   {
		   for(it = (CTunerPresetFM::presetListFM).begin();it!=((CTunerPresetFM::presetListFM).end());++it)
		   {
			   DBG_MSG(("[CTunerPresetFM]: nextPreset Preset Number in loop is %d",presetNum));
			   if(presetNum == (*it)->presetNo)
			   {
				   if(reqStatus == true)
				   {
				      nextPresetNo = presetNum;
				   }
				   else
				   {
					  nextPresetNo = 0;
				   }
				   proxy tunerData;
				   tunerData.frequency = ((*it)->frequency);
				   tunerData.piCode = ((*it)->piCode);

				   //tunerData.token = (unsigned int)token;
				   tunerData.token = TOKEN;
				   DBG_MSG(("[CTunerPresetFM]: nextPreset Frequency selected in next preset is %d",(*it)->frequency));
				 //  DBG_MSG(("[CTunerPresetFM]: nextPreset SPS No for selected in next preset frequency is %d",(*it)->spsNo));

				   //Get HD setting status
				   bool hdStatus =  CTunerSeekHandler::getHDStatus();

					 (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
					 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

     			    status = false;
			   }
			   else
			   {
				   ;
			   }
		   }
		   if(presetNum <= 18)
		   {
			 presetNum = presetNum + 1;
			 DBG_MSG(("[CTunerPresetFM]: nextPreset Preset Number after incrementing is %d",presetNum));
		   }
		   else
		   {
			 presetNum = 1;
		   }
	   }
   }
   else
   {
	   DBG_MSG(("Current Preset No in FM is 0 hence no next preset available !"));
	   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid);
   }
}

void CTunerPresetFM::previousPreset(CSvcIpcRequest* reqData)
{
	TRC_SCOPE(TunerService, CTunerPresetFM, previousPreset);
	DBG_MSG(("[CTunerPresetFM]:previousPreset"));
	reqStatus = true;
	if(currentPresetNo != 0)
	   {
		  int presetNum;
		  if(currentPresetNo != 1)
		   {
		      presetNum = currentPresetNo - 1;
		   }
		   else
		   {
			  presetNum = 18;
		   }
		   std::list<CTunerPresetFM*>::iterator it;
		   bool status = true;
		   DBG_MSG(("[CTunerPresetFM]:previousPreset Current Preset Number is %d",currentPresetNo));
		   while(status)
		   {
			   for(it = (CTunerPresetFM::presetListFM).begin();it!=((CTunerPresetFM::presetListFM).end());++it)
			   {
				   DBG_MSG(("[CTunerPresetFM]:previousPreset Preset Number in loop is %d",presetNum));
				   if(presetNum == (*it)->presetNo)
				   {
					   if(reqStatus == true)
					   {
					      nextPresetNo = presetNum;
					   }
					   else
					   {
						  nextPresetNo = 0;
					   }
					   proxy tunerData;
					   tunerData.frequency = ((*it)->frequency);
					   tunerData.piCode = ((*it)->piCode);
                       //tunerData.token = (unsigned int)token;
					   tunerData.token = TOKEN;
					   DBG_MSG(("[CTunerPresetFM]:previousPreset Frequency selected in next preset is %d",(*it)->frequency));
					   //DBG_MSG(("[CTunerPresetFM]:previousPreset SPS No for selected in next preset frequency is %d",(*it)->spsNo));





						 (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
						 CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

					   status = false;
				   }
				   else
				   {
					   ;
				   }
			   }
			   if(presetNum > 1)
			   {
				 presetNum = presetNum - 1;
				 DBG_MSG(("[CTunerPresetFM]:previousPreset Preset Number after decrementing is %d",presetNum));
			   }
			   else
			   {
				 presetNum = 18;
			   }
		   }
	   }
	   else
	   {
		   DBG_MSG(("[CTunerPresetFM]:previousPreset Current Preset No in FM is 0 hence no next preset available !"));
		   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid);
	   }
}
/*......................................................................................................................
 * @brief getPresets()- Function to get the presets for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerPresetFM::getPresets(CSvcIpcRequest* reqData)
{
   Json::Value obj = retrievePresetFM();
   CTunerRequestResponseHandler::sendPresets(reqData, obj);
}
/*......................................................................................................................
 * @brief retrievePresetFM()- Function to get the presets for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
Json::Value CTunerPresetFM::retrievePresetFM()
{
   TRC_SCOPE(TunerService, CTunerPresetFM, retrievePresetFM);
   int size = (CTunerPresetFM::presetListFM).size();
   DBG_MSG(("[CTunerPresetFM]: Size of the list is --->%d",size));
   if(size != 0)
   {
     std::list<CTunerPresetFM*>::iterator it ;
     Json::Value obj;
     //Json::Value freqobj;
     Json::Value presetsArray;
     Json::Value data;
     int i = 0;
     for(it = (CTunerPresetFM::presetListFM).begin();it!=((CTunerPresetFM::presetListFM).end());++it)
     {
       if(i <= 17)
       {
         data["frequency"] = ((*it)->frequency);
         data["presetNo"] = ((*it)->presetNo);
         data["psName"]	= ((*it)->psName);
         data["piCode"] = ((*it)->piCode);
         presetsArray[i] = data;
         i++;
       }
       else
       {
    	 DBG_MSG(("[CTunerPresetFM]: Size of the list is more than 18, truncating the preset list to MAX"));
       }
     }

     //This is to persist Last tuned station
     // Since we are persisting presets, along with that last tuned station is persisted.
     //freqobj["FmFreq"] = CTunerSeekHandler::mLastTunedFMFrequency;
     //CTunerSeekHandler* seek = new CTunerSeekHandler();
     //freqobj["FmFreq"] = seek->getFMLastTunedFreq();
     //presetsArray[i] = freqobj;

     obj["presetListFM"] = presetsArray;
     obj["presetSizeFM"] = size;

     //Add the last tuned station
    // obj["FmFreq"] = CTunerSeekHandler::mLastTunedFMFrequency;
     //CTunerSeekHandler *seek = new CTunerSeekHandler();
     //obj["FmFreq"] = seek->getFMLastTunedFreq();
     return obj;
   }
   else
   {
     Json::Value obj;
    //obj["presetListFM"] = "";
    //obj["presetSizeFM"] = 0;
     return obj;
   }
}
/*......................................................................................................................
 * @brief readPreset()- Function to get the presets for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
INT32 CTunerPresetFM::readPreset()
{
   INT32 retval1 = 0;

   if(CHBTunerPersistency::getInstance()->readFromPersistency(Tuner_Persist_PresetsFM) != 0)
   {
     retval1 = -1;
   }

   if((retval1 != 0 ))
   {
     return -1;
   }
   return 0;
}
/*......................................................................................................................
 * @brief writePreset()- Function to write the presets for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerPresetFM::writePreset()
{
   //CHBTunerPersistency::getInstance()->writeToPersistency(Tuner_Persist_PresetsFM);
   //sleep(2);
   //CHBTunerPersistency::getInstance()->writeToPersistency(Tuner_Persist_PresetsAM);
}
//AM Presets

/*......................................................................................................................
 * @brief CTunerPresetAM()- Constructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerPresetAM::CTunerPresetAM() {
	frequency = 0;
	psName = "";
	presetNo = 0;
	spsNo = -1;
	piCode = 0;
	hdDispStatus = 3;
	pty = 0;

}

/*......................................................................................................................
 * @brief CTunerPresetAM()- Destructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerPresetAM::~CTunerPresetAM() {
}

/*......................................................................................................................
 * @brief updatePresetNoAM()- Function to obtain the current preset No
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerPresetAM::updatePresetNoAM(unsigned int freq,INT32 spsNo)
{
   TRC_SCOPE(TunerService, CTunerPresetAM, updatePresetNoAM);
   DBG_MSG(("[CTunerPresetAM]:updatePresetNoAM"));
   currentFrequencyAM = freq;
   DBG_MSG(("[CTunerPresetAM]: Current frequency is %d",currentFrequencyAM));
   int prePresetNo = -1;
   currentPresetNoAM = -1;
   int size = (CTunerPresetAM::presetListAM).size();
   if(size != 0)
   {
	   std::list<CTunerPresetAM>::iterator it ;
	   for(it = (CTunerPresetAM::presetListAM).begin();it!=((CTunerPresetAM::presetListAM).end());++it)
	   {
		 if((*it).frequency == currentFrequencyAM)
		 {
		   currentPresetNoAM = (*it).presetNo;
		   if(reqStatusAM == true)
		   {
		   	 if(nextPresetNoAM != currentPresetNoAM)
		     {
		         currentPresetNoAM = nextPresetNoAM;
		         DBG_MSG(("[CTunerPresetAM]: Current preset No in condition check is %d",currentPresetNoAM));
		     }
		     else
		     {
		       ;
		   	 }
		   	}
		   	else
		   	{
			   if((prePresetNo > currentPresetNoAM) || (prePresetNo == -1))
			   {
				  prePresetNo = currentPresetNoAM;
			   }
			   else
			   {
				 ;
			   }
		   	}
		   DBG_MSG(("[CTunerPresetAM]: Current preset No is %d",currentPresetNoAM));
		   //return;
		 }
		 else
		 {
		   //currentPresetNoAM = 0 ;
		 }
	   }
	   if(prePresetNo != -1)
	   {
	   	 currentPresetNoAM = prePresetNo ;
	   }
	   else
	   {
	   		  ;
	   }

	   DBG_MSG(("[CTunerPresetAM]: Final Current preset No is %d",currentPresetNoAM));
   }
   else if(size == 0)
   {
	 DBG_MSG(("[CTunerPresetAM]: Empty list !!"));
   }
   else
   {
	 ;
   }

}
/*......................................................................................................................
 * @brief storePresets()- Function to initiate the preset store functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerPresetAM::storePresets(CSvcIpcRequest* reqData) {

   TRC_SCOPE(TunerService, CTunerPresetAM, storePresets);
   DBG_MSG(("[CTunerPresetAM]:storePreset"));
   CTunerPresetFM::reqStatus = false;
   int presetNo = reqData->getParms().get("presetNo", Json::Value(0)).asInt();
   unsigned int frequency = reqData->getParms().get("frequency", Json::Value(0)).asUInt();
   std::string psName = reqData->getParms().get("psName", Json::Value("")).asString();
   unsigned int piCode = CTunerSeekHandler::mPIcode;
   DBG_MSG(("[CTunerPresetAM]: presetNo & frequency & spsNo: %d, %d , %d",presetNo,frequency,spsNo));
   //Populate data object into preset list

   CTunerPresetAM presetData;
   //presetData = new CTunerPresetAM();
   presetData.frequency = frequency;
   presetData.presetNo = presetNo;
   presetData.psName = psName;
   presetData.piCode = piCode;
   int size = (CTunerPresetAM::presetListAM).size();
   DBG_MSG(("[CTunerPresetAM]: Size of the list is --->%d",size));

   if((presetNo <= PRESET_MAX))
   {
     if(size != 0)
     {
       bool status = false;
       std::list<CTunerPresetAM>::iterator it ;
       for(it = (CTunerPresetAM::presetListAM).begin();it!=((CTunerPresetAM::presetListAM).end());++it)
       {
         DBG_MSG(("[CTunerPresetAM1]: Preset number from the list is %d",(*it).presetNo));
         if((*it).presetNo == presetNo)
         {
            //delete (*it);
           *it = presetData;
           status = true;
           currentPresetNoAM = presetNo;
           CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
           DBG_MSG(("[CTunerPresetAM]: Over write the preset no %d with new value %d",presetNo,(*it).frequency));
         }
       }
       if(false == status)
       {
         DBG_MSG(("[CTunerPresetAM]: No match hence fill"));
         (CTunerPresetAM::presetListAM).push_back(presetData);
         currentPresetNoAM = presetNo;
         CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
       }
       else if(true == status)
       {
         DBG_MSG(("[CTunerPresetAM]: Preset filled"));
       }
       else
       {
         ;
       }
      }
      else if(size == 0)
      {
         DBG_MSG(("[CTunerPresetAM]: Empty list hence fill"));
         (CTunerPresetAM::presetListAM).push_back(presetData);
         currentPresetNoAM = presetNo;
         CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
      }
      else
      {
         ;
      }
   }
   else
   {
     DBG_MSG(("[CTunerPresetAM]: Incorrect preset number !"));
     CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Error);
   }
}

/*......................................................................................................................
 * @brief selectPresets()- Function to initiate the preset selection functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerPresetAM::selectPresets(CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerPresetAM, selectPresets);
   DBG_MSG(("[CTunerPresetAM]:selectPresets\n"));
   reqStatusAM = true;
   int presetNo = reqData->getParms().get("presetNo", Json::Value(0)).asInt();
   //SVCIPC_tUserToken token = reqData->getUserToken();
   int size = (CTunerPresetAM::presetListAM).size();
   DBG_MSG(("[CTunerPresetAM]: Size of the list is ---> %d",size));

   if(size != 0 && size <= PRESET_MAX)
   {
     bool status = false;
     std::list<CTunerPresetAM>::iterator it ;
     for(it = (CTunerPresetAM::presetListAM).begin();it!=((CTunerPresetAM::presetListAM).end());++it)
     {
       DBG_MSG(("[CTunerPresetAM]: Preset number from the list is %d",(*it).presetNo));
       if((*it).presetNo == presetNo)
       {
    	 nextPresetNoAM = presetNo;
    	 //Copy data to be send into a structure
         proxy tunerData;
         tunerData.frequency = ((*it).frequency);
         tunerData.piCode = ((*it).piCode);
         tunerData.selectionType = Tuner_Select_Direct_Tune;
         //tunerData.token = (unsigned int)token;
         tunerData.token = TOKEN;
         status = true;
         DBG_MSG(("[CTunerPresetAM]: Frequency selected is %d",((*it).frequency)));
         DBG_MSG(("[CTunerPresetAM]: SPS No for selected frequency is %d",((*it).spsNo)));

         (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
         CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

       }
       else
       {
         ;
       }
     }
     if(false == status)
     {
       DBG_MSG(("[CTunerPresetAM]: Preset not available !!"));
       CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Error);
     }

   }
   else if(size == 0)
   {
     DBG_MSG(("[CTunerPresetAM]: Empty list !!"));
     CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Error);
   }
}

void CTunerPresetAM::nextPreset(CSvcIpcRequest* reqData)
{
	TRC_SCOPE(TunerService, CTunerPresetAM, nextPreset);
	DBG_MSG(("[CTunerPresetAM]:nextPreset"));
	reqStatusAM = true;
	if(currentPresetNoAM != -1)
	   {
		   int presetNum;
		   if(currentPresetNoAM != 5)
		   {
		     presetNum = currentPresetNoAM + 1;
		   }
		   else
		   {
			 presetNum = 0;
		   }
		   std::list<CTunerPresetAM>::iterator it;
		   bool status = true;
		   DBG_MSG(("[CTunerPresetAM]:nextPreset Current Preset Number is %d",currentPresetNoAM));
		   while(status)
		   {
			   for(it = (CTunerPresetAM::presetListAM).begin();it!=((CTunerPresetAM::presetListAM).end());++it)
			   {
				   DBG_MSG(("[CTunerPresetAM]:nextPreset Preset Number in loop is %d",presetNum));
				   if(presetNum == (*it).presetNo)
				   {
					   if(reqStatusAM == true)
					   {
						  nextPresetNoAM = presetNum;
					   }
					   else
					   {
						  nextPresetNoAM = -1;
					   }
					   proxy tunerData;
					   tunerData.frequency = ((*it).frequency);
					   tunerData.piCode = ((*it).piCode);

					   //tunerData.token = (unsigned int)token;
					   tunerData.token = TOKEN;
					   DBG_MSG(("[CTunerPresetAM]:nextPreset Frequency selected in next preset is %d",(*it).frequency));

					   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
					   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
					   status = false;
				   }
				   else
				   {
					   ;
				   }
			   }
			   if(presetNum <= 5)
			   {
				 presetNum = presetNum + 1;
				 DBG_MSG(("[CTunerPresetAM]:nextPreset Preset Number after incrementing is %d",presetNum));
			   }
			   else
			   {
				 presetNum = -1;
			   }
		   }
	   }
	   else
	   {
		   DBG_MSG(("[CTunerPresetAM]:nextPreset Current Preset No in AM is 0 hence no next preset available !"));
		   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid);
	   }
}

void CTunerPresetAM::previousPreset(CSvcIpcRequest* reqData)
{
	 TRC_SCOPE(TunerService, CTunerPresetAM, previousPreset);
	 DBG_MSG(("[CTunerPresetAM]:previousPreset"));
	 reqStatusAM = true;
	 if(currentPresetNoAM != -1)
	   {
		 DBG_MSG(("[CTunerPresetAM]:previousPreset:Current Preset No is %d",currentPresetNoAM));
		   int presetNum;
		   if(currentPresetNoAM != 0)
		   {
		     presetNum = currentPresetNoAM - 1;
		   }
		   else
		   {
			 presetNum = 5;
		   }
		   std::list<CTunerPresetAM>::iterator it;
		   bool status = true;
		   DBG_MSG(("[CTunerPresetAM]:previousPreset Current Preset Number is %d",currentPresetNoAM));
		   while(status)
		   {
			   for(it = (CTunerPresetAM::presetListAM).begin();it!=((CTunerPresetAM::presetListAM).end());++it)
			   {
				   DBG_MSG(("Preset Number in loop is %d",presetNum));
				   if(presetNum == (*it).presetNo)
				   {
					   if(reqStatusAM == true)
					   {
						  nextPresetNoAM = presetNum;
					   }
					   else
					   {
						  nextPresetNoAM = -1;
					   }
					   proxy tunerData;
					   tunerData.frequency = ((*it).frequency);
					   tunerData.piCode = ((*it).piCode);

					   //tunerData.token = (unsigned int)token;
					   tunerData.token = TOKEN;
					   DBG_MSG(("[CTunerPresetAM]:previousPreset Frequency selected in next preset is %d",(*it).frequency));
					   DBG_MSG(("[CTunerPresetAM]:previousPreset SPS No for selected in next preset frequency is %d",(*it).spsNo));

					   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
					   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
					   status = false;
				   }
				   else
				   {
					   ;
				   }
			   }
			   if(presetNum > 0)
			   {
				 presetNum = presetNum - 1;
				 DBG_MSG(("[CTunerPresetAM]:previousPreset Preset Number after decrementing is %d",presetNum));
			   }
			   else
			   {
				 presetNum = 5;
			   }
		   }
	   }
	   else
	   {
		   DBG_MSG(("[CTunerPresetAM]:previousPreset Current Preset No in AM is 0 hence no next preset available !"));
		   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid);
	   }
}

/*......................................................................................................................
 * @brief getPresets()- Function to get the presets for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerPresetAM::getPresets(CSvcIpcRequest* reqData)
{
   Json::Value obj = retrievePresetAM();
   CTunerRequestResponseHandler::sendPresets(reqData, obj);
}
/*......................................................................................................................
 * @brief retrievePresetAM()- Function to get the presets for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
Json::Value CTunerPresetAM::retrievePresetAM()
{
   TRC_SCOPE(TunerService, CTunerPresetAM, retrievePresetAM);
   int size = (CTunerPresetAM::presetListAM).size();
   DBG_MSG(("[CTunerPresetAM]: Size of the list is --->%d",size));
   if(size != 0)
   {
     std::list<CTunerPresetAM>::iterator it ;
     Json::Value obj;
     Json::Value arr_value(Json::arrayValue);
     Json::Value data(Json::objectValue);
     int i = 0;

     for(it = (CTunerPresetAM::presetListAM).begin();it!=((CTunerPresetAM::presetListAM).end());++it)
     {

       	DBG_MSG((" Preset Station and Frequency is %d",(*it).frequency));
       	DBG_MSG(("Preset Station and Preset is %d",(*it).presetNo));
       	DBG_MSG(("Preset Station and psName is %s",((*it).psName.c_str())));
       	DBG_MSG(("Preset Station and pty is %d",((*it).pty)));
     	DBG_MSG(("Preset Station and piCode is %d",((*it).piCode)));

     	data["frequency"] = (*it).frequency;
     	data["presetNo"] = (*it).presetNo;
     	data["psName"] = (*it).psName.c_str();
     	data["pty"] = (*it).pty;
     	data["piCode"] = (*it).piCode;
         DBG_MSG((" Preset List %d updated",((*it).presetNo)));
         arr_value[i] = data;

         i++;

   }

     obj["presetListAM"] = arr_value;
     obj["presetSizeAM"] = size;
     DBG_MSG(("[CTunerPresetAM]: Size of the list is more than 18, truncating the preset list to MAX"));
     return obj;
   }
   else
   {
     DBG_MSG(("[CTunerPresetAM]:AM preset list is empty "));
     Json::Value obj;
     printf("Preset List is Empty");
     obj["presetListAM"] = "";
     obj["presetSizeAM"] = 0;
     return obj;
   }
}





//  -------------------------------------- Weather Band Preset Implementation ----------------------------------------


CTunerPresetWB::CTunerPresetWB() {
	frequency = 0;
	psName = "";
	presetNo = 0;
	spsNo = -1;
	piCode = 0;
	hdDispStatus = 3;

}

/*......................................................................................................................
 * @brief CTunerPresetAM()- Destructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerPresetWB::~CTunerPresetWB() {
}

/*......................................................................................................................
 * @brief updatePresetNoAM()- Function to obtain the current preset No
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerPresetWB::updatePresetNoWB(unsigned int freq,INT32 spsNo)
{
   TRC_SCOPE(TunerService, CTunerPresetWB, updatePresetNoWB);
   DBG_MSG(("[CTunerPresetWB]:updatePresetNoWB"));
   currentFrequencyWB = freq;
   DBG_MSG(("[CTunerPresetWB]: Current frequency is %d",currentFrequencyWB));
   int prePresetNo = 0;
   currentPresetNoWB = 0;
   int size = (CTunerPresetWB::presetListWB).size();
   if(size != 0)
   {
	   std::list<CTunerPresetWB>::iterator it ;
	   for(it = (CTunerPresetWB::presetListWB).begin();it!=((CTunerPresetWB::presetListWB).end());++it)
	   {
		 if((*it).frequency == currentFrequencyWB)
		 {
		   currentPresetNoWB = (*it).presetNo;
		   if(reqStatusWB == true)
		   {
		   	 if(nextPresetNoWB != currentPresetNoWB)
		     {
		         currentPresetNoWB = nextPresetNoWB;
		         DBG_MSG(("[CTunerPresetWB]: Current preset No in condition check is %d",currentPresetNoWB));
		     }
		     else
		     {
		       ;
		   	 }
		   	}
		   	else
		   	{
			   if((prePresetNo > currentPresetNoWB) || (prePresetNo == 0))
			   {
				  prePresetNo = currentPresetNoWB;
			   }
			   else
			   {
				 ;
			   }
		   	}
		   DBG_MSG(("[CTunerPresetWB]: Current preset No is %d",currentPresetNoWB));
		   //return;
		 }
		 else
		 {
		   //currentPresetNoAM = 0 ;
		 }
	   }
	   if(prePresetNo != 0)
	   {
	   	 currentPresetNoWB = prePresetNo ;
	   }
	   else
	   {
	   		  ;
	   }

	   DBG_MSG(("[CTunerPresetWB]: Final Current preset No is %d",currentPresetNoWB));
   }
   else if(size == 0)
   {
	 DBG_MSG(("[CTunerPresetWB]: Empty list !!"));
   }
   else
   {
	 ;
   }

}
/*......................................................................................................................
 * @brief storePresets()- Function to initiate the preset store functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
void CTunerPresetWB::storePresets(CSvcIpcRequest* reqData) {

   TRC_SCOPE(TunerService, CTunerPresetWB, storePresets);
   DBG_MSG(("[CTunerPresetWB]:storePreset"));
   CTunerPresetFM::reqStatus = false;
   int presetNo = reqData->getParms().get("presetNo", Json::Value(0)).asInt();
   unsigned int frequency = reqData->getParms().get("frequency", Json::Value(0)).asUInt();
   std::string psName = reqData->getParms().get("psName", Json::Value("")).asString();
   unsigned int piCode = CTunerSeekHandler::mPIcode;
   DBG_MSG(("[CTunerPresetWB]: presetNo & frequency & spsNo: %d, %d , %d",presetNo,frequency,spsNo));
   //Populate data object into preset list

   CTunerPresetWB presetData;
   //presetData = new CTunerPresetAM();
   presetData.frequency = frequency;
   presetData.presetNo = presetNo;
   presetData.psName = psName;
   presetData.piCode = piCode;
   int size = (CTunerPresetWB::presetListWB).size();
   DBG_MSG(("[CTunerPresetWB]: Size of the list is --->%d",size));

   if((presetNo <= PRESET_MAX_WB))
   {
     if(size != 0)
     {
       bool status = false;
       std::list<CTunerPresetWB>::iterator it ;
       for(it = (CTunerPresetWB::presetListWB).begin();it!=((CTunerPresetWB::presetListWB).end());++it)
       {
         DBG_MSG(("[CTunerPresetWB]: Preset number from the list is %d",(*it).presetNo));
         if((*it).presetNo == presetNo)
         {
            //delete (*it);
           *it = presetData;
           status = true;
           currentPresetNoWB = presetNo;
           CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
           DBG_MSG(("[CTunerPresetWB]: Over write the preset no %d with new value %d",presetNo,(*it).frequency));
         }
       }
       if(false == status)
       {
         DBG_MSG(("[CTunerPresetWB]: No match hence fill"));
         (CTunerPresetWB::presetListWB).push_back(presetData);
         currentPresetNoWB = presetNo;
         CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
       }
       else if(true == status)
       {
         DBG_MSG(("[CTunerPresetWB]: Preset filled"));
       }
       else
       {
         ;
       }
      }
      else if(size == 0)
      {
         DBG_MSG(("[CTunerPresetWB]: Empty list hence fill"));
         (CTunerPresetWB::presetListWB).push_back(presetData);
         currentPresetNoWB = presetNo;
         CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
      }
      else
      {
         ;
      }
   }
   else
   {
     DBG_MSG(("[CTunerPresetWB]: Incorrect preset number !"));
     CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Error);
   }
}

/*......................................................................................................................
 * @brief selectPresets()- Function to initiate the preset selection functionality for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerPresetWB::selectPresets(CSvcIpcRequest* reqData)
{
   TRC_SCOPE(TunerService, CTunerPresetWB, selectPresets);
   DBG_MSG(("[CTunerPresetWB]:selectPresets\n"));
   reqStatusWB = true;
   int presetNo = reqData->getParms().get("presetNo", Json::Value(0)).asInt();
   //SVCIPC_tUserToken token = reqData->getUserToken();
   int size = (CTunerPresetWB::presetListWB).size();
   DBG_MSG(("[CTunerPresetWB]: Size of the list is ---> %d",size));

   if(size != 0 && size <= PRESET_MAX_WB)
   {
     bool status = false;
     std::list<CTunerPresetWB>::iterator it ;
     for(it = (CTunerPresetWB::presetListWB).begin();it!=((CTunerPresetWB::presetListWB).end());++it)
     {
       DBG_MSG(("[CTunerPresetWB]: Preset number from the list is %d",(*it).presetNo));
       if((*it).presetNo == presetNo)
       {
    	 nextPresetNoWB = presetNo;
    	 //Copy data to be send into a structure
         proxy tunerData;
         tunerData.frequency = ((*it).frequency);
         tunerData.piCode = ((*it).piCode);
         tunerData.selectionType = Tuner_Select_Direct_Tune;
         //tunerData.token = (unsigned int)token;
         tunerData.token = TOKEN;
         status = true;
         DBG_MSG(("[CTunerPresetAM]: Frequency selected is %d",((*it).frequency)));
         DBG_MSG(("[CTunerPresetAM]: SPS No for selected frequency is %d",((*it).spsNo)));

         (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
         CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);

       }
       else
       {
         ;
       }
     }
     if(false == status)
     {
       DBG_MSG(("[CTunerPresetWB]: Preset not available !!"));
       CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Error);
     }

   }
   else if(size == 0)
   {
     DBG_MSG(("[CTunerPresetWB]: Empty list !!"));
     CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Error);
   }
}
void CTunerPresetWB::nextPreset(CSvcIpcRequest* reqData)
{
	TRC_SCOPE(TunerService, CTunerPresetWB, nextPreset);
	DBG_MSG(("[CTunerPresetWB]:nextPreset"));
	reqStatusWB = true;
	if(currentPresetNoWB != -1)
	   {
		   int presetNum;
		   if(currentPresetNoWB != 5)
		   {
		     presetNum = currentPresetNoWB + 1;
		   }
		   else
		   {
			 presetNum = 0;
		   }
		   std::list<CTunerPresetWB>::iterator it;
		   bool status = true;
		   DBG_MSG(("[CTunerPresetWB]:nextPreset Current Preset Number is %d",currentPresetNoWB));
		   while(status)
		   {
			   for(it = (CTunerPresetWB::presetListWB).begin();it!=((CTunerPresetWB::presetListWB).end());++it)
			   {
				   DBG_MSG(("[CTunerPresetWB]:nextPreset Preset Number in loop is %d",presetNum));
				   if(presetNum == (*it).presetNo)
				   {
					   if(reqStatusWB == true)
					   {
						  nextPresetNoWB = presetNum;
					   }
					   else
					   {
						  nextPresetNoWB = 0;
					   }
					   proxy tunerData;
					   tunerData.frequency = ((*it).frequency);
					   tunerData.piCode = ((*it).piCode);

					   //tunerData.token = (unsigned int)token;
					   tunerData.token = TOKEN;
					   DBG_MSG(("[CTunerPresetWB]:nextPreset Frequency selected in next preset is %d",(*it).frequency));

					   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
					   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
					   status = false;
				   }
				   else
				   {
					   ;
				   }
			   }
			   if(presetNum <= 5)
			   {
				 presetNum = presetNum + 1;
				 DBG_MSG(("[CTunerPresetWB]:nextPreset Preset Number after incrementing is %d",presetNum));
			   }
			   else
			   {
				 presetNum = -1;
			   }
		   }
	   }
	   else
	   {
		   DBG_MSG(("[CTunerPresetWB]:nextPreset Current Preset No in WB is 0 hence no next preset available !"));
		   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid);
	   }
}

void CTunerPresetWB::previousPreset(CSvcIpcRequest* reqData)
{
	 TRC_SCOPE(TunerService, CTunerPresetWB, previousPreset);
	 DBG_MSG(("[CTunerPresetWB]:previousPreset"));
	 reqStatusWB = true;
	 if(currentPresetNoWB != -1)
	   {
		   int presetNum;
		   if(currentPresetNoWB != 0)
		   {
		     presetNum = currentPresetNoWB - 1;
		   }
		   else
		   {
			 presetNum = 5;
		   }
		   std::list<CTunerPresetWB>::iterator it;
		   bool status = true;
		   DBG_MSG(("[CTunerPresetWB]:previousPreset Current Preset Number is %d",currentPresetNoWB));
		   while(status)
		   {
			   for(it = (CTunerPresetWB::presetListWB).begin();it!=((CTunerPresetWB::presetListWB).end());++it)
			   {
				   DBG_MSG(("Preset Number in loop is %d",presetNum));
				   if(presetNum == (*it).presetNo)
				   {
					   if(reqStatusWB == true)
					   {
						  nextPresetNoWB = presetNum;
					   }
					   else
					   {
						  nextPresetNoWB = 0;
					   }
					   proxy tunerData;
					   tunerData.frequency = ((*it).frequency);
					   tunerData.piCode = ((*it).piCode);

					   //tunerData.token = (unsigned int)token;
					   tunerData.token = TOKEN;
					   DBG_MSG(("[CTunerPresetWB]:previousPreset Frequency selected in next preset is %d",(*it).frequency));
					   DBG_MSG(("[CTunerPresetWB]:previousPreset SPS No for selected in next preset frequency is %d",(*it).spsNo));

					   (CTunerRequestResponseHandler::GetInstance()->getStationProxy())->postInternalEvent(DMMTunerStation_UPD_ID_requestStartTune, (void*) &tunerData);
					   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Success);
					   status = false;
				   }
				   else
				   {
					   ;
				   }
			   }
			   if(presetNum > 0)
			   {
				 presetNum = presetNum - 1;
				 DBG_MSG(("[CTunerPresetAM]:previousPreset Preset Number after decrementing is %d",presetNum));
			   }
			   else
			   {
				 presetNum = 5;
			   }
		   }
	   }
	   else
	   {
		   DBG_MSG(("[CTunerPresetWB]:previousPreset Current Preset No in WB is 0 hence no next preset available !"));
		   CTunerRequestResponseHandler::sendResponse(reqData, Tuner_Response_Invalid);
	   }
}
/*......................................................................................................................
 * @brief getPresets()- Function to get the presets for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/

void CTunerPresetWB::getPresets(CSvcIpcRequest* reqData)
{
   Json::Value obj = retrievePresetWB();
   CTunerRequestResponseHandler::sendPresets(reqData, obj);
}
/*......................................................................................................................
 * @brief retrievePresetAM()- Function to get the presets for the tuner service.
 *
 * @param
 *
 *
 *
 *
 *....................................................................................................................*/
Json::Value CTunerPresetWB::retrievePresetWB()
{
   TRC_SCOPE(TunerService, CTunerPresetWB, retrievePresetWB);
   int size = (CTunerPresetWB::presetListWB).size();
   DBG_MSG(("[CTunerPresetWB]: Size of the list is --->%d",size));
   if(size != 0)
   {
     std::list<CTunerPresetWB>::iterator it ;
     Json::Value obj;
     Json::Value arr_value(Json::arrayValue);
     Json::Value data(Json::objectValue);
     int i = 0;

     for(it = (CTunerPresetWB::presetListWB).begin();it!=((CTunerPresetWB::presetListWB).end());++it)
     {

       	DBG_MSG((" Preset Station and Frequency is %d",(*it).frequency));
       	DBG_MSG(("Preset Station and Preset is %d",(*it).presetNo));
       	DBG_MSG(("Preset Station and psName is %s",((*it).psName.c_str())));
       	DBG_MSG(("Preset Station and pty is %d",((*it).pty)));
     	DBG_MSG(("Preset Station and piCode is %d",((*it).piCode)));

     	data["frequency"] = (*it).frequency;
     	data["presetNo"] = (*it).presetNo;
     	data["psName"] = (*it).psName.c_str();
     	data["pty"] = (*it).pty;
     	data["piCode"] = (*it).piCode;
         DBG_MSG((" Preset List %d updated",((*it).presetNo)));
         arr_value[i] = data;

         i++;

   }

     obj["presetListWB"] = arr_value;
     obj["presetSizeWB"] = size;
     DBG_MSG(("[CTunerPresetWB]: Size of the list is more than 18, truncating the preset list to MAX"));
     return obj;
   }
   else
   {
     DBG_MSG(("[CTunerPresetWB]:WB preset list is empty "));
     Json::Value obj;
     printf("Preset List is Empty");
     obj["presetListWB"] = "";
     obj["presetSizeWB"] = 0;
     return obj;
   }
}

//----------------------------------------------------- END OF FILE ---------------------------------------------------
