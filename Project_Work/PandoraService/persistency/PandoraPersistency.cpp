/**********************************************************************
 * 
 */

/*---------------------------------------------------------------------\
 * Preprocessor Includes
\*--------------------------------------------------------------------*/

#include <iostream>
#include "PandoraPersistency.hpp"
#include "CPandoraRequestResponseHandler.hpp"
#include "TraceMacros.hpp"
/*---------------------------------------------------------------------\
 * Preprocessor Defines (Macros, Constants)
\*--------------------------------------------------------------------*/



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


//CHBTunerPersistency *CHBTunerPersistency::instance=NULL;

extern SVCIPC_tConnection     gSvcIpcConnection;

/*CHBTunerPersistency *CHBTunerPersistency::getInstance()
{

   if(instance == NULL)
   {
     instance = new CHBTunerPersistency();
   }
   return(instance);
}
*/
/*
void CHBTunerPersistency::removeInstance()
{
   if(instance != NULL)
   {
     delete instance;
     instance = NULL;
   }

}*/

int CHBTunerPersistency::writeToPersistency()
{
   TRC_SCOPE(TunerService, CHBTunerPersistency, writeToPersistency);
   DBG_MSG(("[CHBTunerPersistency]: writeToPersistency"));
  // tunerPersistencyTypes types = Tuner_Persist_PresetsFM;
   Json::FastWriter mJsonWriter;
   rc = 0;

   return 0;

}
int CHBTunerPersistency::deleteFromPersistency(unsigned int presetType)
{
   return (0);
}
int CHBTunerPersistency::readFromPersistency(_Uint32t idValue)
{
   TRC_SCOPE(TunerService, CHBTunerPersistency, readFromPersistency);
   int retval = 0;
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
	  sprintf(presetListArray,"presetListFM",NULL);

   }
   else if(idValue == 2)
   {
	  mValue = 2;
	  sprintf(presetListArray,"presetListAM",NULL);
   }
   else if(idValue == 3)
   {
	  mValue = 3;
	  sprintf(presetListArray,"presetListWB",NULL);
   }
   else if(idValue == 4)
   {
	  mValue = 4;
	  sprintf(presetListArray,"FmFreq",NULL);
   }
   else if(idValue == 5)
   {
	  mValue = 5;
	  sprintf(presetListArray,"AmFreq",NULL);
   }
   else if(idValue == 6)
   {
	  mValue = 6;
	  sprintf(presetListArray,"WbFreq",NULL);
   }
   else if(idValue == 7)
   {
      mValue = 7;
	  sprintf(presetListArray,"HDService",NULL);
   }
   else if(idValue == 8)
   {
      mValue = 8;
	  sprintf(presetListArray,"TAMode",NULL);
   }
   else
   {
     retval = -1;
   }
   obj["key"]=presetListArray;
   std::string keyParam=msJsonWriter.write(key_parameters);

   invoke( PERSISTENCY_DBUS_NAME, PERSISTENCY_DBUS_OBJECT_PATH, "read",obj,m_resp);
  // DBG_MSG(("[CHBTunerPersistency]:m_readPerDataHandle %d ",m_readPerDataHandle));
   std::free(presetListArray);
   return retval;
}


void CHBTunerPersistency::processSvcIpcResult( const _Uint32t & resultRegHndl, SVCIPC_tConstStr result, void* pUserData )
{
   TRC_SCOPE(TunerService, CHBTunerPersistency, processSvcIpcResult);
   DBG_MSG(("Tuner processSvcIpcResult handle: %d",resultRegHndl));


}

void CHBTunerPersistency::processSvcIpcError( const _Uint32t & resultRegHndl, SVCIPC_tErrorCode error, SVCIPC_tConstStr errMsg, SVCIPC_tConstStr errName )
{
	int count = 0;
	TRC_SCOPE(TunerService, CHBTunerPersistency, processSvcIpcError);
	DBG_MSG(("[CHBTunerPersistency]: processSvcIpcError - %d ", error));
	DBG_MSG(("Error string is %s",(char*)errMsg));
	DBG_MSG(("Error name is %s ",(char*)errName));



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
