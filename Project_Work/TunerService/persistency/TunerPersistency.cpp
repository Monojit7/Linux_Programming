/**********************************************************************
 *  Project       JLR NLI
 *  (c) copyright 2013
 *  Company       Harman/Becker Automotive Systems GmbH
 *                All rights reserved
 * Secrecy Level STRICTLY CONFIDENTIAL
 **********************************************************************/

/**
 *  @file         TunerPersistency.cpp
 *  @brief
 *  @author
 *  @ingroup      Tuner Subsystem
 */

/*---------------------------------------------------------------------\
 * Preprocessor Includes
\*--------------------------------------------------------------------*/

#include <iostream>
#include "TunerPersistency.hpp"
#include "CTunerDataTypes.hpp"
#include "CTunerPresets.hpp"
#include "CTunerRequestResponseHandler.hpp"
#include "CTunerSeekHandler.hpp"
#include "CTunerAnnouncementHandler.hpp"
#include "TraceMacros.hpp"
#include <CTunerCmdInvoker.hpp>
/*---------------------------------------------------------------------\
 * Preprocessor Defines (Macros, Constants)
\*--------------------------------------------------------------------*/

_Uint32t CHBTunerPersistency::m_readPerDataHandle = 0;
_Uint32t CHBTunerPersistency::id = 1;
UINT32 CHBTunerPersistency::fmFrequency = 0;
UINT32 CHBTunerPersistency::amFrequency = 0;
UINT32 CHBTunerPersistency::count = 0;
eInitProgressStatus CHBTunerPersistency::meInitProgressStatus = eNOT_STARTED;

#define RETRY_COUNT 2

/*----------------------------------------------------------------------------------------------------------------------
 * TraceClient Scope Definitions
 ----------------------------------------------------------------------------------------------------------------------*/
TRC_SCOPE_DEF(TunerService, CHBTunerPersistency, writeToPersistency);
TRC_SCOPE_DEF(TunerService, CHBTunerPersistency, readFromPersistency);
TRC_SCOPE_DEF(TunerService, CHBTunerPersistency, processSvcIpcResult);
TRC_SCOPE_DEF(TunerService, CHBTunerPersistency, processSvcIpcError);
TRC_SCOPE_DEF(TunerService, CHBTunerPersistency, processSvcIpcNameChange);

/*---------------------------------------------------------------------\
 * Type Definitions (Enums, Typedefs, Structs, ...)
\*--------------------------------------------------------------------*/
/*---------------------------------------------------------------------\
 * Constant And External Variables
\*--------------------------------------------------------------------*/
/*---------------------------------------------------------------------\
 * Function Prototypes
\*--------------------------------------------------------------------*/
/*---------------------------------------------------------------------\
 * Local Variables
\*--------------------------------------------------------------------*/
/*---------------------------------------------------------------------\
 * Function Definitions
\*--------------------------------------------------------------------*/


CHBTunerPersistency *CHBTunerPersistency::instance=NULL;

extern SVCIPC_tConnection     gSvcIpcConnection;

CHBTunerPersistency *CHBTunerPersistency::getInstance()
{

   if(instance == NULL)
   {
     instance = new CHBTunerPersistency();
   }
   return(instance);
}

void CHBTunerPersistency::removeInstance()
{
   if(instance != NULL)
   {
     delete instance;
     instance = NULL;
   }

}

int CHBTunerPersistency::writeToPersistency()
{
   TRC_SCOPE(TunerService, CHBTunerPersistency, writeToPersistency);
   DBG_MSG(("[CHBTunerPersistency]: writeToPersistency"));
   tunerPersistencyTypes types = Tuner_Persist_PresetsFM;
   Json::FastWriter mJsonWriter;
   rc = 0;
   CTunerSeekHandler* seek = new CTunerSeekHandler();
   if(types == Tuner_Persist_PresetsFM)
   {
	 Json::Value obj;
	 CTunerPresetFM* mInst = new CTunerPresetFM();
     obj = mInst->retrievePresetFM();
     std::string preset = mJsonWriter.write(obj);
     DBG_MSG(("JSon Writer Value is : %s ",preset.c_str()));
     bool status = obj.isNull();
     if(true == status)
     {
    	 DBG_MSG(("FM preset list is empty hence do not write to persistency !!"));
     }
     else
     {
       rc = invoke( PERSISTENCY_DBUS_NAME, PERSISTENCY_DBUS_OBJECT_PATH, "write",obj,true,&m_resp);
     }
     delete mInst;
     types = Tuner_Persist_PresetsAM;
   }

   if(types == Tuner_Persist_PresetsAM)
   {
	 Json::Value objAm;
	 CTunerPresetAM* mInst = new CTunerPresetAM();
     objAm = mInst->retrievePresetAM();
     std::string preset = mJsonWriter.write(objAm);
     DBG_MSG(("JSon Writer Value is : %s ",preset.c_str()));
     bool status = objAm.isNull();
     if(true == status)
     {
    	DBG_MSG(("AM preset list is empty hence do not write to persistency !!"));
     }
     else
     {
        rc = invoke( PERSISTENCY_DBUS_NAME, PERSISTENCY_DBUS_OBJECT_PATH, "write",objAm,true,&m_resp);
     }
     delete mInst;
     types = Tuner_Persist_PresetsWB;
   }
   if(types == Tuner_Persist_PresetsWB)
   {
	 Json::Value objWb;
	 CTunerPresetWB* mInst = new CTunerPresetWB();
     objWb = mInst->retrievePresetWB();
     std::string preset = mJsonWriter.write(objWb);
     DBG_MSG(("JSon Writer Value is : %s ",preset.c_str()));
     bool status = objWb.isNull();
     if(true == status)
     {
    	DBG_MSG(("WB preset list is empty hence do not write to persistency !!"));
     }
     else
     {
        rc = invoke( PERSISTENCY_DBUS_NAME, PERSISTENCY_DBUS_OBJECT_PATH, "write",objWb,true,&m_resp);
     }
     delete mInst;
     types = Tuner_Persist_LastTunedFreqFM;
   }
   if(types == Tuner_Persist_LastTunedFreqFM)
   {
	  Json::Value objFmFreq;
	  UINT32 freq = seek->getFMLastTunedFreq();
	  if((TUNER_FM_BASE_FREQ_EU == freq)||(TUNER_FM_BASE_FREQ_US == freq)||(TUNER_FM_BASE_FREQ_JPN == freq))
	  {
	    DBG_MSG(("Do not write FM frequency to persistency since it is base frequency"));
	  }
	  else
	  {
		objFmFreq["FmFreq"] = seek->getFMLastTunedFreq();
	    std::string fmFreq = mJsonWriter.write(objFmFreq);
	    DBG_MSG(("JSon Writer Value is : %s ",fmFreq.c_str()));
	    rc = invoke( PERSISTENCY_DBUS_NAME, PERSISTENCY_DBUS_OBJECT_PATH, "write",objFmFreq,true,&m_resp);
	  }
      types = Tuner_Persist_LastTunedFreqAM;
   }

   if(types == Tuner_Persist_LastTunedFreqAM)
   {
	  Json::Value objAmFreq;
	  UINT32 freq = seek->getAMLastTunedFreq();
	  if((TUNER_AM_BASE_FREQ_EU == freq)||(TUNER_AM_BASE_FREQ_US == freq)||(TUNER_AM_BASE_FREQ_JPN == freq))
	  {
	    DBG_MSG(("Do not write AM frequency to persistency since it is base frequency"));
	  }
	  else
	  {
		objAmFreq["AmFreq"] = seek->getAMLastTunedFreq();
	    std::string amFreq = mJsonWriter.write(objAmFreq);
	    DBG_MSG(("JSon Writer Value is : %s ",amFreq.c_str()));
	    rc = invoke( PERSISTENCY_DBUS_NAME, PERSISTENCY_DBUS_OBJECT_PATH, "write",objAmFreq,true,&m_resp);
	   }
       types = Tuner_Persist_LastTunedFreqWB;
   }
   if(types == Tuner_Persist_LastTunedFreqWB)
   {
	  Json::Value objWbFreq;
	  UINT32 freq = seek->getWBLastTunedFreq();
	  if(TUNER_WB_BASE_FREQ_US == freq)
	  {
	    DBG_MSG(("Do not write AM frequency to persistency since it is base frequency"));
	  }
	  else
	  {
		objWbFreq["WbFreq"] = seek->getWBLastTunedFreq();
	    std::string wbFreq = mJsonWriter.write(objWbFreq);
	    DBG_MSG(("JSon Writer Value is : %s ",wbFreq.c_str()));
	    rc = invoke( PERSISTENCY_DBUS_NAME, PERSISTENCY_DBUS_OBJECT_PATH, "write",objWbFreq,true,&m_resp);
	   }
       types = Tuner_Persist_LastHDService;
   }

   if(types == Tuner_Persist_LastHDService)
   {
	   Json::Value ob;
	   ob["HDService"] = seek->getLastTunedHDService();
	   std::string hdService = mJsonWriter.write(ob);
	   DBG_MSG(("JSon Writer Value is : %s ",hdService.c_str()));
	   rc = invoke( PERSISTENCY_DBUS_NAME, PERSISTENCY_DBUS_OBJECT_PATH, "write",ob,true,&m_resp);
	   delete seek;
	   types = Tuner_Persist_TA_Mode;
   }

   if(types == Tuner_Persist_TA_Mode)
   {
   	   Json::Value obTA;
   	   obTA["TAMode"] = CTunerAnnouncementHandler::getAnnouncementHandler()->getTAMode();
   	   std::string TAMode = mJsonWriter.write(obTA);
   	   DBG_MSG(("JSon Writer Value for TA Mode is : %s ",TAMode.c_str()));
   	   rc = invoke( PERSISTENCY_DBUS_NAME, PERSISTENCY_DBUS_OBJECT_PATH, "write",obTA,true,&m_resp);
   }
   else
   {
	   ;
   }
   return 0;

}
int CHBTunerPersistency::deleteFromPersistency(unsigned int presetType)
{
   return (0);
}
int CHBTunerPersistency::readFromPersistency(_Uint32t idValue)
{
   TRC_SCOPE(TunerService, CHBTunerPersistency, readFromPersistency);
   INT32 retval = 0;
   DBG_MSG(("[CHBTunerPersistency]:readFromPersistency"));
   //mBand = band;
   mValue = 0;
   std::string presetliststring;
   char* presetListArray=NULL;
   Json::Value obj;
   Json::Value defaultPackArray(Json::Value::null);

   Json::Value presetListArr(Json::arrayValue);
   Json::Value key_parameters(Json::objectValue);

   //Extract the Array from the object
   Json::Value paramsArray(Json::arrayValue);
   presetListArray=(char *)malloc(20);
   if(idValue == 1)
   {
	  mValue = 1;
	  if(presetListArray != NULL)
	  {
	  sprintf(presetListArray,"presetListFM",NULL);
	  }

   }
   else if(idValue == 2)
   {
	  mValue = 2;
	  if(presetListArray != NULL)
	  {
	  sprintf(presetListArray,"presetListAM",NULL);
	  }
   }
   else if(idValue == 3)
   {
	  mValue = 3;
	  if(presetListArray != NULL)
	  {
	  sprintf(presetListArray,"presetListWB",NULL);
	  }
   }
   else if(idValue == 4)
   {
	  mValue = 4;
	  if(presetListArray != NULL)
	  {
	  sprintf(presetListArray,"FmFreq",NULL);
	  }
   }
   else if(idValue == 5)
   {
	  mValue = 5;
	  if(presetListArray != NULL)
	  {
	  sprintf(presetListArray,"AmFreq",NULL);
	  }
   }
   else if(idValue == 6)
   {
	  mValue = 6;
	  if(presetListArray != NULL)
	  {
	  sprintf(presetListArray,"WbFreq",NULL);
	  }
   }
   else if(idValue == 7)
   {
      mValue = 7;
	  if(presetListArray != NULL)
	  {
	  sprintf(presetListArray,"HDService",NULL);
	  }
   }
   else if(idValue == 8)
   {
      mValue = 8;
	  if(presetListArray != NULL)
	  {
	  sprintf(presetListArray,"TAMode",NULL);
	  }
   }
   else
   {
     retval = -1;
   }
   obj["key"]=presetListArray;
   std::string keyParam=msJsonWriter.write(key_parameters);

   m_readPerDataHandle = invoke( PERSISTENCY_DBUS_NAME, PERSISTENCY_DBUS_OBJECT_PATH, "read",obj,m_resp);
   DBG_MSG(("[CHBTunerPersistency]:m_readPerDataHandle %d ",m_readPerDataHandle));
   std::free(presetListArray);
   return retval;
}


void CHBTunerPersistency::processSvcIpcResult( const _Uint32t & resultRegHndl, SVCIPC_tConstStr result, void* pUserData )
{
   TRC_SCOPE(TunerService, CHBTunerPersistency, processSvcIpcResult);
   DBG_MSG(("Tuner processSvcIpcResult handle: %d",resultRegHndl));

   if(resultRegHndl == CHBTunerPersistency::m_readPerDataHandle)
   {
     bool bSuccess;
     Json::Reader jReader;
     Json::Value jValue;

     bSuccess = jReader.parse(result, jValue);
     //DBG_MSG(("bSuccess: %d", bSuccess));
     Json::FastWriter mJsonWriter;
     std::string preset = mJsonWriter.write(jValue);

     Json::Value presetJsonList(Json::arrayValue);

     if((mValue == 1) || (mValue == 2) || (mValue == 3))
     {
     presetJsonList=jValue.get("res", "{}" );
     if(presetJsonList.isNull() != true)
     {
       DBG_MSG(("presetJsonList.size(%d)",presetJsonList.size()));
       for(unsigned int i = 0; i < presetJsonList.size(); i++)
       {
         Json::Value tempObject(Json::objectValue);
         Json::Value defarray;
         tempObject = presetJsonList.get(i, defarray);
         if(tempObject.isNull()!=true)
         {
           if(mValue == 1)
           {
        	    CTunerPresetFM* presetData;
			    presetData = new CTunerPresetFM();
				presetData->frequency = tempObject.get("frequency", Json::Value(0)).asUInt();
				DBG_MSG(("Frequency in read is : %d ",tempObject.get("frequency", Json::Value(0)).asUInt()));
				presetData->presetNo = tempObject.get("presetNo", Json::Value(0)).asInt();
				presetData->psName = tempObject.get("psName", Json::Value("")).asString();
//				presetData->spsNo = tempObject.get("spsNo", Json::Value(0)).asInt();
				presetData->piCode = tempObject.get("piCode", Json::Value(0)).asUInt();

				DBG_MSG(("Before Pushed preset : %d",i));

				(CTunerPresetFM::presetListFM).push_back(presetData);
				DBG_MSG(("Pushed  preset :%d ",i));
           }
           else if(mValue == 2)
           {
        	   CTunerPresetAM* presetInfo;
               presetInfo = new CTunerPresetAM();
               presetInfo->frequency =  tempObject.get("frequency", Json::Value(0)).asUInt();
               DBG_MSG(("Frequency in read is : %d ",tempObject.get("frequency", Json::Value(0)).asUInt()));
               presetInfo->presetNo = tempObject.get("presetNo", Json::Value(0)).asInt();
               presetInfo->psName = tempObject.get("psName", Json::Value("")).asString();
               presetInfo->spsNo = tempObject.get("spsNo",Json::Value(0)).asInt();
               presetInfo->piCode = tempObject.get("piCode", Json::Value(0)).asUInt();

               DBG_MSG(("Before Pushed  preset : %d",i));

               (CTunerPresetAM::presetListAM).push_back(*presetInfo);
                DBG_MSG(("Pushed  preset : %d",i));

           }

           else if(mValue == 3)
           {
        	   CTunerPresetWB* presetInfo;
               presetInfo = new CTunerPresetWB();
               presetInfo->frequency =  tempObject.get("frequency", Json::Value(0)).asUInt();
               DBG_MSG(("Frequency in read is : %d ",tempObject.get("frequency", Json::Value(0)).asUInt()));
               presetInfo->presetNo = tempObject.get("presetNo", Json::Value(0)).asInt();
               presetInfo->psName = tempObject.get("psName", Json::Value("")).asString();
               presetInfo->spsNo = tempObject.get("spsNo",Json::Value(0)).asInt();
               presetInfo->piCode = tempObject.get("piCode", Json::Value(0)).asUInt();

               DBG_MSG(("Before Pushed  preset : %d",i));

               (CTunerPresetWB::presetListWB).push_back(*presetInfo);
                DBG_MSG(("Pushed  preset : %d",i));

           }
           else
           {
               ;
           }
         }
         }
       }
     }
     else
     {
         if(mValue == 4)
	     {
		  CTunerSeekHandler* seek = new CTunerSeekHandler();
		  UINT32 freq = jValue.get("res",Json::Value(0)).asUInt();
		  seek->setFMLastTunedFreq(freq);
		  DBG_MSG(("FM Frequency read is : %d",seek->getFMLastTunedFreq()));
	      //seek->tuneLastFMStation();
	      delete seek;
		  fmFrequency = freq;
	      }
    	 else if(mValue == 5)
		 {
    	  bool status = jValue.get("res", Json::Value(0)).isUInt();
    	  CTunerSeekHandler* seek = new CTunerSeekHandler();
    	  if(true == status)
    	  {
    		  seek->setAMLastTunedFreq(jValue.get("res", Json::Value(0)).asUInt());
    		  DBG_MSG(("AM Frequency read is : %d",seek->getAMLastTunedFreq()));
    	  }
          delete seek;
		 }
    	 else if(mValue == 6)
		 {
      	  CTunerSeekHandler* seek = new CTunerSeekHandler();
      	  seek->setWBLastTunedFreq(jValue.get("res", Json::Value(0)).asUInt());
      	  DBG_MSG(("WB Frequency read is : %d",seek->getWBLastTunedFreq()));
          delete seek;
		 }
    	 else if(mValue == 7)
    	 {
   		    CTunerSeekHandler* seek = new CTunerSeekHandler();
   		    UINT32 service = jValue.get("res",Json::Value(0)).asInt();
   		    seek->setLastTunedHDService(service);
   		    DBG_MSG(("HD Service in read is : %d",seek->getLastTunedHDService()));
   	        delete seek;
    	 }
    	 else if(mValue == 8)
		 {
	    	//bool mode = jValue.get("res",Json::Value(0)).asBool();
			//CTunerAnnouncementHandler::TAmode = mode;
			//DBG_MSG(("TA mode in read is : %d",CTunerAnnouncementHandler::getAnnouncementHandler()->getTAMode()));
		 }

     }
   }
   else
   {
	   DBG_MSG(("RC to invoke is : %d",m_readPerDataHandle));
   }
   count = 0;
   if(id < 6)
   {
	 id++;
	 if(CHBTunerPersistency::getInstance()->readFromPersistency(id) != 0)
     {
    	 DBG_MSG(("Read presets from persistency failed ...."));
     }
     else
     {

     }
   }
   else
   {
     DBG_MSG(("Read from persistency complete !!"));
  	   pthread_mutex_lock(&CHBTunerPersistency::getInstance()->mAvailBandMSignalmutex);
  	   if(CHBTunerPersistency::meInitProgressStatus == eSTATION_INTERFACE_CONNECTED)
  	   {

  		   Json::Value availableBands (Json::arrayValue);
  		   Json::Value waveBand;
  		   unsigned int j = 0;
  		   availableBands[j] = "fm";
  		   j++;
  		   availableBands[j] = "am";
  		   j++;
  		 availableBands[j] = "wb";
  		   waveBand["value"] = availableBands;
  		   DBG_MSG(("[CDMMTunerStationProxy]::GetProperties: sending available Band inside Persistency read completion"));
  		   CTunerRequestResponseHandler::handleNotif("availableBands",waveBand);
  		   DBG_MSG(("Setting the persistency flag with true value"));
  		   CTunerCmdInvoker::GetInstance()->setPersistencyFlag(true);

  	   }
  	   else
  	   {
  		   DBG_MSG(("setting CDMMTunerStationProxy::ePERSISTENCY_COMPLETION !!!\n"));
  	   CHBTunerPersistency::meInitProgressStatus = ePERSISTENCY_COMPLETION;
  	   }
       DBG_MSG(("Read from persistency complete !!"));
       pthread_mutex_unlock(&CHBTunerPersistency::getInstance()->mAvailBandMSignalmutex);
     }

}

void CHBTunerPersistency::processSvcIpcError( const _Uint32t & resultRegHndl, SVCIPC_tErrorCode error, SVCIPC_tConstStr errMsg, SVCIPC_tConstStr errName )
{
	TRC_SCOPE(TunerService, CHBTunerPersistency, processSvcIpcError);
	DBG_MSG(("[CHBTunerPersistency]: processSvcIpcError - %d ", error));
	DBG_MSG(("Error string is %s",(char*)errMsg));
	DBG_MSG(("Error name is %s ",(char*)errName));

	if(resultRegHndl == CHBTunerPersistency::m_readPerDataHandle)
	{
		DBG_MSG(("[CHBTunerPersistency]: processSvcIpcError while trying to invoke Persistency Read"));

		if(count < RETRY_COUNT)
		{
		  count++;
		  DBG_MSG(("[CHBTunerPersistency]: processSvcIpcError trying to read for %d time",count));
		  readFromPersistency(id);
		}
		else
		{
			count = 0;

			if (8 > id)
			{
				id++;
				readFromPersistency(id);
			}
		}

		//(void)subscribeOwnerChanged( PERSISTENCY_DBUS_NAME );

	}

}
void CHBTunerPersistency::processSvcIpcNameChange( const _Uint32t & nameChgRegHndl, const CSvcIpcNameChange* pNameChange, void* pUserData )
{
	TRC_SCOPE(TunerService, CHBTunerPersistency, processSvcIpcNameChange);
	DBG_MSG(("[CHBTunerPersistency]: processSvcIpcNameChange"));
	if ( 0 == strcmp(PERSISTENCY_DBUS_NAME, pNameChange->m_newName) )
	{
		  // readFromPersistency(1);
	}
}
