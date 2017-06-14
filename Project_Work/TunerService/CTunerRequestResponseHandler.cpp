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
* @file TunerServiceMain.hpp
* @ingroup TunerService
* @author 
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
 *----------------------------------------------------------------------------------------------------------------------*/
#include "api/sys/dsi/pf/colibry/src/CDSIConnection.hpp"
#include "api/sys/colibry/pf/oswrapper/src/CHBThreadCtrl.hpp"
#include "api/sys/dsi/pf/colibry/src/CDSIConnection.hpp"
#include "CTunerReqQueue.hpp"
#include "CTunerProxyPool.hpp"
#include "CTunerCmdInvoker.hpp"
#include "CTunerRequestResponseHandler.hpp"
#include "CTunerScanHandler.hpp"
#include "CTunerSeekHandler.hpp"
#include "DMMAmFmTunerControl.h"
#include "TunerPersistency.hpp"
#include "CTunerPresets.hpp"
#include "TraceMacros.hpp"
#include <fstream>

#define ONOFF_SERVICE_BUS_NAME "com.harman.service.YAMAHAOnOff"
#define ONOFF_SERVICE_OBJ_NAME "/com/harman/service/YAMAHAOnOff"

#define AUDIOSERVICE_BUS_NAME  "com.harman.service.AudioManager"
#define AUDIOSERVICE_OBJ_NAME  "/com/harman/service/AudioManager"

#define AUDIOMIXERSERVICE_BUS_NAME  "com.harman.service.AudioMixerManager"
#define AUDIOMIXERSERVICE_OBJ_NAME  "/com/harman/service/AudioMixerManager"

#define CCF_SERVICE_BUS_NAME "com.harman.service.CCFParameters"
#define CCF_SERVICE_OBJ_NAME "/com/harman/service/CCFParameters"

#define PERSISTENCY_DBUS_NAME "com.harman.service.PersistentKeyValue"
#define PERSISTENCY_DBUS_OBJECT_PATH "/com/harman/service/PersistentKeyValue"

#define MODEMANAGER_DBUS_NAME         "com.harman.service.ModeManager"
#define MODEMANAGER_DBUS_OBJECT_PATH   "/com/harman/service/ModeManager"

#define MODEMANAGER_SERVICE_NEW_MODE "currentModeNew"

#define ONOFF_POWERMODE_SIGNAL 				"powerMode"

#define AUDIOSERVICE_SOURCE_SWITCH_SIGNAL 	"audioSource"
#define AUDIOSERVICE_INTERRUPT_SIGNAL  "interruptSrc"

/*----------------------------------------------------------------------------------------------------------------------
 * File scope functions
----------------------------------------------------------------------------------------------------------------------*/

pthread_t                             gProxy;
static void* runTunerStationProxy(void *arg);
static void* runTunerControlProxy(void *arg);
static void* runTunerAnnouncementProxy(void *arg);
static void* runTunerAudioProxy(void *arg);
static void* runTunerDiranaControlProxy(void *arg);
/*----------------------------------------------------------------------------------------------------------------------
 * Static attributes
----------------------------------------------------------------------------------------------------------------------*/
CDMMTunerStationProxy*       CTunerRequestResponseHandler::mStationProxy                = NULL;
CDMMAmFmTunerControlProxy*   CTunerRequestResponseHandler::mControlProxy                = NULL;
CDMMTunerAnnouncementProxy*  CTunerRequestResponseHandler::mAnnouncementProxy           = NULL;
CDMMTunerAudioProxy*         CTunerRequestResponseHandler::mAudioProxy                  = NULL;
CDMMAmFmTunerDiranaControlProxy* CTunerRequestResponseHandler::mDiranaControlProxy      = NULL;
BOOL                         CTunerRequestResponseHandler::controlComponentStatus       = false;
BOOL                         CTunerRequestResponseHandler::stationComponentStatus       = false;
BOOL                         CTunerRequestResponseHandler::announcementComponentStatus  = false;
BOOL                         CTunerRequestResponseHandler::audioComponentStatus         = false;
CTunerRequestResponseHandler* CTunerRequestResponseHandler::mpInstance                  = NULL;
INT32                        CTunerRequestResponseHandler::regionCode                   = 1;
BOOL   						 CTunerRequestResponseHandler::mInitialized					= false;
BOOL   						 CTunerRequestResponseHandler::mHwVariant					= true;
std::string                  CTunerRequestResponseHandler::mLastSource                  = "";
std::string                  CTunerRequestResponseHandler::mCurrentSource               = "";
BOOL                         CTunerRequestResponseHandler::vrStatus                     = false;
BOOL                         CTunerRequestResponseHandler::taStatus                     = false;

/*----------------------------------------------------------------------------------------------------------------------
 * TraceClient Scope Definitions
----------------------------------------------------------------------------------------------------------------------*/
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, Initialize);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, processSvcIpcRequest);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, processSvcIpcSignal);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, sendPresets);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, createTunerStationProxyThread);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, createTunerControlProxyThread);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, createTunerAnnouncementProxyThread);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, CTunerRequestResponseHandler);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, processSvcIpcNameChange);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, createTunerAudioProxyThread);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, processSvcIpcResult);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, processSvcIpcError);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, parseCCFfile);
TRC_SCOPE_DEF(TunerService, CTunerRequestResponseHandler, createTunerDiranaControlThread);

/*......................................................................................................................
 * @brief CTunerServiceMain()- Constructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerRequestResponseHandler::CTunerRequestResponseHandler(): CSvcImplBase("com.harman.service.amfmTunerService",
          "/com/harman/service/amfmTunerService", true)
{

   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, CTunerRequestResponseHandler);
   //We want to know if the OnOff Service comes alive or dies.
   (void)subscribeOwnerChanged( ONOFF_SERVICE_BUS_NAME );

   //We want to know if the CCF Service comes alive or dies.
   (void)subscribeOwnerChanged( CCF_SERVICE_BUS_NAME );
   if(nameHasOwner(PERSISTENCY_DBUS_NAME))
   {
     DBG_MSG(("[CTunerRequestResponseHandler]: Persistency service already available"));
   }
   else
   {
	  (void)subscribeOwnerChanged( PERSISTENCY_DBUS_NAME );
	  DBG_MSG(("[CTunerRequestResponseHandler]: Subscribed for Persistency service availability"));
   }


   //Check if the OnOff Service is already alive.
   mPowerModeSignalRegHandle = subscribe( ONOFF_SERVICE_OBJ_NAME, ONOFF_POWERMODE_SIGNAL );
   DBG_MSG(("Power Mode signal subscription success"));
  
   mSourceChangeRegHandle = subscribe( AUDIOSERVICE_OBJ_NAME, AUDIOSERVICE_SOURCE_SWITCH_SIGNAL );
   DBG_MSG(("[CTunerRequestResponseHandler]: Source change signal %d",mSourceChangeRegHandle ));

   mAudioSourceChange = subscribe( MODEMANAGER_DBUS_OBJECT_PATH, MODEMANAGER_SERVICE_NEW_MODE );
   DBG_MSG(("[CTunerRequestResponseHandler]: Source change signal ModeChange %d",mAudioSourceChange ));

   mInterruptSourceRegHandle = subscribe( AUDIOSERVICE_OBJ_NAME, AUDIOSERVICE_INTERRUPT_SIGNAL );
}

/*......................................................................................................................
 * @brief CTunerServiceMain()- Destructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerRequestResponseHandler::~CTunerRequestResponseHandler()
{
   // TBD
}

void CTunerRequestResponseHandler::setHwVariant(BOOL hwVariant)
{
	printf("hw variant set \n");
   mHwVariant = hwVariant;
}

BOOL CTunerRequestResponseHandler::getHwVariant()
{
   return mHwVariant;
}

//=============================================================================
CDMMTunerStationProxy* CTunerRequestResponseHandler::getStationProxy()
{
   return mStationProxy;
}
CDMMAmFmTunerControlProxy* CTunerRequestResponseHandler::getControlProxy()
{
   return mControlProxy;
}
CDMMTunerAnnouncementProxy* CTunerRequestResponseHandler::getAnnouncementProxy()
{
   return mAnnouncementProxy;
}

CDMMTunerAudioProxy* CTunerRequestResponseHandler::getAudioProxy()
{
   return mAudioProxy;
}

bool CTunerRequestResponseHandler::getStationComponentStatus(void)
{
   return stationComponentStatus;
}

//=============================================================================

void  CTunerRequestResponseHandler::setStationComponentStatus(bool stationStatus)
{
   stationComponentStatus = stationStatus;
}

//=============================================================================

bool CTunerRequestResponseHandler::getControlComponentStatus(void)
{
   return controlComponentStatus;
}

//=============================================================================

void  CTunerRequestResponseHandler::setControlComponentStatus(bool controlStatus)
{
   controlComponentStatus = controlStatus;
}

//=============================================================================
bool CTunerRequestResponseHandler::getAnnouncementComponentStatus(void)
{
   return announcementComponentStatus;
}
//=============================================================================
void  CTunerRequestResponseHandler::setAnnouncementComponentStatus(bool announcementStatus)
{
   announcementComponentStatus = announcementStatus;
}
//=============================================================================
bool CTunerRequestResponseHandler::getAudioComponentStatus(void)
{
   return announcementComponentStatus;
}
//=============================================================================
void  CTunerRequestResponseHandler::setAudioComponentStatus(bool audioStatus)
{
   announcementComponentStatus = audioStatus;
}

void CTunerRequestResponseHandler::processSvcIpcNameChange( const _Uint32t & nameChgRegHndl, const CSvcIpcNameChange* pNameChange, void* pUserData )
{
   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, processSvcIpcNameChange);
   DBG_MSG(("******newName: %s, oldOwner: %s, newOwner: %s******", pNameChange->m_newName, pNameChange->m_oldOwner, pNameChange->m_newOwner));

   if ( 0 == strcmp(ONOFF_SERVICE_BUS_NAME, pNameChange->m_newName) )
   {
     //Subscribe for the power mode signal
     mPowerModeSignalRegHandle = subscribe( ONOFF_SERVICE_OBJ_NAME, ONOFF_POWERMODE_SIGNAL );
     DBG_MSG(("Power Mode signal subscription success in processSvcIpcNameChange"));

   }
   else if(0 == strcmp(PERSISTENCY_DBUS_NAME, pNameChange->m_newName))
   {
	  DBG_MSG(("Persistency service available in processSvcIpcNameChange, reading again"));
	  CHBTunerPersistency::getInstance()->readFromPersistency(1);
   }
   else
   {
     DBG_MSG(("Power Mode signal subscription received for other service in processSvcIpcNameChange"));
   }
}

//=============================================================================
UINT32 CTunerRequestResponseHandler::createTunerStationProxyThread(void)
{
   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, createTunerStationProxyThread);
   pthread_attr_t threadAttr;
   sched_param_t  schedParam;
   // create thread attributes
   DBG_MSG(("Station Proxy Thread"));
   pthread_setname_np(gProxy, "TunerStationThread");
   if( pthread_attr_init(&threadAttr) != EOK )
   {
     return -1;
   }
   if( pthread_attr_getschedparam(&threadAttr,&schedParam) != EOK )
   {
     return -1;
   }
   schedParam.sched_priority = 10;
   if( pthread_attr_setschedparam(&threadAttr,&schedParam) != EOK )
   {
     return -1;
   }
   if( pthread_attr_setinheritsched(&threadAttr,PTHREAD_EXPLICIT_SCHED) != EOK )
   {
     return -1;
   }

   if( pthread_attr_setschedpolicy(&threadAttr,SCHED_RR) != EOK )
   {
     return -1;
   }

   if( pthread_create(&gProxy, &threadAttr, runTunerStationProxy, 0) != EOK)
   {
     DBG_MSG(("failed to create thread"));
     return -1;
   }

   (void) pthread_attr_destroy(&threadAttr);
   //cout << "runTunerStationProxy Thread Created " <<endl;
   return 0;
}

//=============================================================================

UINT32 CTunerRequestResponseHandler::createTunerControlProxyThread(void)
{
   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, createTunerControlProxyThread);
   pthread_attr_t threadAttri;
   sched_param_t  schedParams;
   // create thread attributes
   DBG_MSG(("Tuner Control Proxy Thread"));
   pthread_setname_np(gProxy, "TunerControlThread");
   if( pthread_attr_init(&threadAttri) != EOK )
   {
     return -1;
   }
   if( pthread_attr_getschedparam(&threadAttri,&schedParams) != EOK )
   {
     return -1;
   }
   DBG_MSG(("Changing the Priority to 12"));
   schedParams.sched_priority = 12;
   if( pthread_attr_setschedparam(&threadAttri,&schedParams) != EOK )
   {
     return -1;
   }
   if( pthread_attr_setinheritsched(&threadAttri,PTHREAD_EXPLICIT_SCHED) != EOK )
   {
     return -1;
   }

   if( pthread_attr_setschedpolicy(&threadAttri,SCHED_RR) != EOK )
   {
     return -1;
   }

   if( pthread_create(&gProxy, &threadAttri, runTunerControlProxy, 0) != EOK)
   {
     DBG_MSG(("failed to create Tuner Control thread"));
     return -1;
   }

   (void) pthread_attr_destroy(&threadAttri);
   //cout << "runTunerControlProxy Thread Created " <<endl;
   return 0;
}
//=============================================================================

UINT32 CTunerRequestResponseHandler::createTunerAnnouncementProxyThread(void)
{
   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, createTunerAnnouncementProxyThread);
   pthread_attr_t threadAttribute;
   sched_param_t  schedPara;
   // create thread attributes
   DBG_MSG(("Announcement Proxy Thread"));
   pthread_setname_np(gProxy, "TunerAnnounementThread");
   if( pthread_attr_init(&threadAttribute) != EOK )
   {
     return -1;
   }
   if( pthread_attr_getschedparam(&threadAttribute,&schedPara) != EOK )
   {
     return -1;
   }
   schedPara.sched_priority = 10;
   if( pthread_attr_setschedparam(&threadAttribute,&schedPara) != EOK )
   {
     return -1;
   }
   if( pthread_attr_setinheritsched(&threadAttribute,PTHREAD_EXPLICIT_SCHED) != EOK )
   {
     return -1;
   }

   if( pthread_attr_setschedpolicy(&threadAttribute,SCHED_RR) != EOK )
   {
     return -1;
   }

   if( pthread_create(&gProxy, &threadAttribute, runTunerAnnouncementProxy, 0) != EOK)
   {
     DBG_MSG(("failed to create Tuner Control thread"));
     return -1;
   }

   (void) pthread_attr_destroy(&threadAttribute);
   //cout << "runTunerAnnouncementProxy Thread Created " <<endl;
   return 0;
}

//===============================================================================================

UINT32 CTunerRequestResponseHandler::createTunerAudioProxyThread(void)
{
   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, createTunerAudioProxyThread);
   pthread_attr_t threadAttribute;
   sched_param_t  schedPara;
   DBG_MSG(("Audio Proxy thread"));
   pthread_setname_np(gProxy, "TunerAudioProxyThread");
   // create thread attributes

   if( pthread_attr_init(&threadAttribute) != EOK )
   {
     return -1;
   }
   if( pthread_attr_getschedparam(&threadAttribute,&schedPara) != EOK )
   {
     return -1;
   }
   schedPara.sched_priority = 10;
   if( pthread_attr_setschedparam(&threadAttribute,&schedPara) != EOK )
   {
     return -1;
   }
   if( pthread_attr_setinheritsched(&threadAttribute,PTHREAD_EXPLICIT_SCHED) != EOK )
   {
     return -1;
   }
   if( pthread_attr_setschedpolicy(&threadAttribute,SCHED_RR) != EOK )
   {
     return -1;
   }

   if( pthread_create(&gProxy, &threadAttribute, runTunerAudioProxy, 0) != EOK)
   {
     DBG_MSG(("failed to create Tuner Control thread"));
     return -1;
   }

   (void) pthread_attr_destroy(&threadAttribute);
   //cout << "runTunerAudioProxy Thread Created " <<endl;
   return 0;
}
//===============================================================================================

UINT32 CTunerRequestResponseHandler::createTunerDiranaControlThread(void)
{
   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, createTunerDiranaControlThread);
   pthread_attr_t threadAttribute;
   sched_param_t  schedPara;
   // create thread attributes
   DBG_MSG(("Dirana Proxy Thread"));
   pthread_setname_np(gProxy, "TunerDiranaProxyThread");
   if( pthread_attr_init(&threadAttribute) != EOK )
   {
     return -1;
   }
   if( pthread_attr_getschedparam(&threadAttribute,&schedPara) != EOK )
   {
     return -1;
   }
   schedPara.sched_priority = 10;
   if( pthread_attr_setschedparam(&threadAttribute,&schedPara) != EOK )
   {
     return -1;
   }
   if( pthread_attr_setinheritsched(&threadAttribute,PTHREAD_EXPLICIT_SCHED) != EOK )
   {
     return -1;
   }
   if( pthread_attr_setschedpolicy(&threadAttribute,SCHED_RR) != EOK )
   {
     return -1;
   }

   if( pthread_create(&gProxy, &threadAttribute, runTunerDiranaControlProxy, 0) != EOK)
   {
     DBG_MSG(("failed to create Tuner Dirana Control thread"));
     return -1;
   }

   (void) pthread_attr_destroy(&threadAttribute);
   //cout << "runTunerDiranaControlProxy Thread Created " <<endl;
   return 0;
}

/*......................................................................................................................
 * @brief onTunerRequest()- Global function which is registered for getting the requests on DBUS for Tuner service
 *
 * @param
 *       context         : SVCIPC context
 *       method          : API name
 *       parms           : parameters associated with the request
 *       noReplyExpected : flag to indicate reply is expected or not
 *       token : 
 *
 * @returns None
 *
 *....................................................................................................................*/

//         SVCIPC_tReqContext  context,
//         SVCIPC_tConstStr    method,
//         SVCIPC_tConstStr    parms,
//         SVCIPC_tBool        noReplyExpected,
//         SVCIPC_tUserToken   token)

void CTunerRequestResponseHandler::processSvcIpcRequest( CSvcIpcRequest* pRequest )
{
   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, processSvcIpcRequest);
   DBG_MSG(("CTunerRequestResponseHandler: DBUS request received onTunerRequest"));
   //printf("CTunerRequestResponseHandler: DBUS request received onTunerRequest \n");
   if(strcmp("startDSI", pRequest->getMethod()) == 0)
   {
     // Handle shutdown request
   }
   else
   {
     CTunerReqData* request;
     request = new CTunerReqData();
     request->mMethod = pRequest->getMethod();
    // request->mContext  = pRequest->getContext();
     request->mUserToken = pRequest->getUserToken();
     request->mNoReplyExpected = pRequest->isReplyExpected();

     //Push the request to Queue
     CTunerResHandler* obj = CTunerResHandler::GetInstance();
     (obj->tunerRequestQueue).push_back(request);
     //Get the instance and propagate the request to Invoker
     CTunerCmdInvoker *cmdInvoker = CTunerCmdInvoker::GetInstance();
     cmdInvoker->handleTunerRequest(pRequest);
   }
}

/*......................................................................................................................
 * @brief processSvcIpcSignal()- Global function which is registered for getting the requests on DBUS for Tuner service
 *
 * @param
 *       context         : SVCIPC context
 *       method          : API name
 *       parms           : parameters associated with the request
 *       noReplyExpected : flag to indicate reply is expected or not
 *       token :
 *
 * @returns None
 *
 *....................................................................................................................*/

void CTunerRequestResponseHandler::processSvcIpcSignal( const _Uint32t & sigRegHndl, const CSvcIpcSignal* pSignal, void* pUserData )
{
   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, processSvcIpcSignal);

   Json::Reader jReader;
   Json::Value jValue;
   BOOL bSuccess;

   // Process the shutdown signal from ON/OFF service
   if (sigRegHndl == mPowerModeSignalRegHandle )
   {
     DBG_MSG(("Received Power Mode signal from ON/OFF service"));
     //cout << "Received Power Mode signal from ON/OFF service" << endl;
     bSuccess = jReader.parse(pSignal->m_sigData, jValue);
     if ((bSuccess) && (jValue.isObject()))
     {
       INT32 powerModeValue = jValue.get("value", -1).asInt();
       if(powerModeValue == 1 || powerModeValue == 2)
       {
         DBG_MSG(("shutdown signal !!!! persist presets now"));
         // save to device
         FILE *fp = fopen("/dev/tuner/status/amfm","wb");
         if(fp != NULL)
         {
        	 DBG_MSG(("shutdown signal !!!! writing into tuner device now"));
        	 //cout << "shutdown signal !!!! writing into tuner device now" << endl;
        	 fprintf( fp, "save" );
             fclose(fp);
         }
         //CTunerPresetFM::writePreset();
         CHBTunerPersistency::getInstance()->writeToPersistency();
       }
       else
       {
    	 DBG_ERROR(("Power mode received is %d !!!! cannot persist presets",powerModeValue));
       }
     }
   }
   else if (sigRegHndl == mAudioSourceChange )
   {
     DBG_MSG(("Received Source change signal from Mode manager service with uniquename"));

     bSuccess = jReader.parse(pSignal->m_sigData, jValue);
     if ((bSuccess) && (jValue.isObject()))
     {
       std::string currentSource;
       proxy data;
       data.token = TOKEN;
       bool status = jValue.get("uniqueName", "").isString();
       if(status == true)
       {
    	 currentSource = jValue.get("uniqueName", "").asString();
       }
       DBG_MSG(("UniqueName from ModeManager %s", currentSource.c_str()));
       mLastSource = currentSource;
       mCurrentSource = currentSource;
       //DBG_MSG(("Current source is %s",currentSource));

       if((currentSource == "fm1"))
       {
         DBG_MSG(("Inform AM/FM resource manager regarding the source switch to Entertainment source"));
         //cout << "Inform AM/FM resource manager regarding the source switch to Entertainment source from FM" << endl;
        // DMMTunerTypes_StationSelector station;
        // station.frequency = CTunerSeekHandler::mLastTunedFMFrequency;
        // station.id  = 0;
        // mStationProxy->requestStartTune(station, 1, DMMTunerStation_SELECT_DIRECT);
         data.snkSpeakerStatus = true;
         CDMMTunerAudioProxy::spkStatus = true;
         data.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
         DBG_MSG(("Address being sent for sink in fm source is %x",&data));
         mAudioProxy->postInternalEvent(DMMTunerAudio_UPD_ID_requestSetCurrentSinks, (void*)&data);

        // sleep(1);
        // CTunerSeekHandler* mSeek = new CTunerSeekHandler();
        // mSeek->tuneLastFMStation();
        // delete mSeek;
       }
       else if((currentSource == "am1"))
       {
         DBG_MSG(("Inform AM/FM resource manager regarding the source switch to Entertainment source from AM"));
         //cout << "Inform AM/FM resource manager regarding the source switch to Entertainment source from AM" << endl;
        // DMMTunerTypes_StationSelector station;
        // station.frequency = CTunerSeekHandler::mLastTunedAMFrequency;
        // station.id  = 0;
       //  mStationProxy->requestStartTune(station, 1, DMMTunerStation_SELECT_DIRECT);
         data.snkSpeakerStatus = true;
         CDMMTunerAudioProxy::spkStatus = true;
         data.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
         DBG_MSG(("Address being sent for sink in am source is %x",&data));
         mAudioProxy->postInternalEvent(DMMTunerAudio_UPD_ID_requestSetCurrentSinks, (void*)&data);
       }
       else if((currentSource == "wb1"))
       {
         DBG_MSG(("Inform AM/FM resource manager regarding the source switch to Entertainment source from WB"));
         //cout << "Inform AM/FM resource manager regarding the source switch to Entertainment source from WB" << endl;
        // DMMTunerTypes_StationSelector station;
        // station.frequency = CTunerSeekHandler::mLastTunedAMFrequency;
        // station.id  = 0;
       //  mStationProxy->requestStartTune(station, 1, DMMTunerStation_SELECT_DIRECT);
         data.snkSpeakerStatus = true;
         CDMMTunerAudioProxy::spkStatus = true;
         data.srcType = DMMTunerAudio_SRC_ENTERTAINMENT;
         DBG_MSG(("Address being sent for sink in am source is %x",&data));
         mAudioProxy->postInternalEvent(DMMTunerAudio_UPD_ID_requestSetCurrentSinks, (void*)&data);
       }
       else if(currentSource == "ta")
       {
    	 //  cout << "Inform AM/FM resource manager regarding the source switch to Announcement source from TA" << endl;
         data.srcType = DMMTunerAudio_SRC_ANNOUNCEMENT;
         data.snkSpeakerStatus = true;
         CDMMTunerAudioProxy::spkStatus = true;
         mAudioProxy->postInternalEvent(DMMTunerAudio_UPD_ID_requestSetCurrentSinks, (void*)&data);
         DBG_MSG(("Inform AM/FM resource manager regarding the source switch to Announcement source"));
       }
       else if(currentSource == "dab")
       {
         data.srcType = (DMMTunerAudio_SourceType)0xFF;
         CDMMTunerAudioProxy::spkStatus = false;
         mAudioProxy->postInternalEvent(DMMTunerAudio_UPD_ID_requestSetCurrentSinks, (void*)&data);
         //cout << "Inform AM/FM resource manager regarding the source switch to dab source from dab" << endl;
         DBG_MSG(("Inform AM/FM resource manager regarding the source switch to dab source"));
       }
     }
   }
   else if(sigRegHndl == mInterruptSourceRegHandle)
   {
	   DBG_MSG(("Received Interrupt signal from Mode manager service"));
	   bSuccess = jReader.parse(pSignal->m_sigData, jValue);
	   std::string interruptSource ;
	   std::string action ;
	   if ((bSuccess) && (jValue.isObject()))
	   {
	     proxy data;
	     data.token = TOKEN;

	     bool status = jValue.get("source", "").isString();
	     bool stat = jValue.get("action", "").isString();
	     if((status & stat)== true)
	     {
			 interruptSource = jValue.get("source", "").asString();
			 std::cout<<"Current interrupt source is "<<interruptSource<<std::endl;

			 action = jValue.get("action", "").asString();
			 //DBG_MSG(("Current action of interrupt source is %s",action));
	     }
	     else
	     {
	    	 ;
	     }

	     if((interruptSource == "traffic") && (action == "added"))
	     {
	    	 taStatus = false;
	    	 data.srcType = DMMTunerAudio_SRC_ANNOUNCEMENT;
	    	 data.snkSpeakerStatus = true;
	    	 CDMMTunerAudioProxy::spkStatus = true;
	    	 mAudioProxy->postInternalEvent(DMMTunerAudio_UPD_ID_requestSetCurrentSinks, (void*)&data);
	    	 DBG_MSG(("Inform AM/FM resource manager regarding the source switch to Announcement source"));
	     }
	     else if((interruptSource == "traffic") && (action == "removed"))
	     {
	         if((mCurrentSource == "fm") || (mCurrentSource == "am"))
	         {
	        	 DBG_MSG(("TA exited !! "));
	        	 taStatus = true;
	         }
	         else
	         {
	        	DBG_MSG(("Current source was not Am/Fm when Traffic Announcement exited"));
	         }


	    	 data.srcType = DMMTunerAudio_SRC_ANNOUNCEMENT;
	    	 data.snkSpeakerStatus = false;
	    	 CDMMTunerAudioProxy::spkStatus = false;
	    	 mAudioProxy->postInternalEvent(DMMTunerAudio_UPD_ID_requestSetCurrentSinks, (void*)&data);
	    	 DBG_MSG(("Inform AM/FM resource manager regarding the source switch to Entertainment source after TA"));
             //sleep(2);
	    	 if(mLastSource == "dab")
			 {
	    		sleep(2);
	    		data.srcType = (DMMTunerAudio_SourceType)0xFF;
	    		CDMMTunerAudioProxy::spkStatus = false;
				mAudioProxy->postInternalEvent(DMMTunerAudio_UPD_ID_requestSetCurrentSinks, (void*)&data);
				DBG_MSG(("Inform AM/FM resource manager regarding the source switch to dab source after TA"));
			 }
			 else
	    	 {
	    	   //data.srcType = DMMTunerAudio_SRC_ANNOUNCEMENT;
	    	   //data.taStatus = false;
	    	  // data.snkSpeakerStatus = false;
	    	  // mAudioProxy->postInternalEvent(DMMTunerAudio_UPD_ID_requestSetCurrentSinks, (void*)&data);
	    	  // DBG_MSG(("Inform AM/FM resource manager regarding the source switch to Entertainment source after TA"));
	    	 }
	     }
	     else if((interruptSource == "speech") && (action == "removed"))
	     {
	         if((mCurrentSource == "fm") || (mCurrentSource == "am"))
	         {
	        	 DBG_MSG(("VR exited !! "));
	        	 vrStatus = true;
	         }
	         else
	         {
	        	DBG_MSG(("Current source was not Am/Fm when VR exited"));
	         }
	     }
	     else if((interruptSource == "speech") && (action == "added"))
	     {
	    	 vrStatus = false;
	     }
      }
	  else
	  {
	     ;
	  }
   }
}

void CTunerRequestResponseHandler::processSvcIpcError( const _Uint32t & resultRegHndl, SVCIPC_tErrorCode error, SVCIPC_tConstStr errMsg, SVCIPC_tConstStr errName )
{
	TRC_SCOPE(TunerService, CTunerRequestResponseHandler, processSvcIpcError);
	DBG_WARNING(("Default Region code value Setting with handles recieved as %d & stored value as %d!!!!!!!!!!!!!!!!!!!!!!!",resultRegHndl,mCCFDataHandle));
	DBG_WARNING(("processSvcIpcError with error message as %s & error name is %s ************",errMsg,errName));
	/*if(mCCFDataHandle == resultRegHndl)
    {
       regionCode = 1;
       CTunerSeekHandler* mSeek = new CTunerSeekHandler();
	   if(mControlProxy != NULL)
	   {
		 mControlProxy->processInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetRegionCode,(void*)&regionCode);
	   }
	   DBG_WARNING(("Default Region code value Set due to CCF Error!!!"));
	   mSeek->updateFrequency(regionCode);
	   delete(mSeek);
    }*/
}

void CTunerRequestResponseHandler::processSvcIpcResult( const _Uint32t & resultRegHndl, SVCIPC_tConstStr result, void* pUserData )
{
   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, processSvcIpcResult);
   DBG_MSG(("Tuner processSvcIpcResult handle in CTunerRequestResponseHandler : %d",resultRegHndl));
   //Get the corresponding region from CCF
  /* if(resultRegHndl == mCCFDataHandle)
   {
     bool bSuccess;
     Json::Reader jReader;
     Json::Value jValue;
     bSuccess = jReader.parse(result, jValue);
     regionCode = jValue.get("Value", Json::Value(1)).asInt();
     DBG_MSG(("Region code value read from CCF is %d ",regionCode));
     std::string key = jValue.get("Attribute", Json::Value("empty")).asString();
     if((regionCode < 1) || (regionCode > 6))
     {
       regionCode = 1;
     }
     else
     {
       DBG_MSG(("Main Region code value is %d ",regionCode));
     }
   CTunerSeekHandler* mSeek = new CTunerSeekHandler();
   if(mControlProxy != NULL)
   {
     mControlProxy->processInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetRegionCode,(void*)&regionCode);
   }
   DBG_MSG(("Main Region code value is outside %d ",regionCode));
   mSeek->updateFrequency(regionCode);
   delete(mSeek);
   }*/
}
/*......................................................................................................................
 * @brief Initialize()
 *
 * @param
 *
 * @returns TRUE  : Success
 *          FALSE : Failure
 *
 *....................................................................................................................*/

BOOL CTunerRequestResponseHandler::Initialize()
{
   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, Initialize);
   DBG_MSG(("In CTunerServiceMain::Initialize function ...."));
   //printf("In CTunerServiceMain::Initialize function ....");
   if( mInitialized == false )
   {
     DBG_MSG(("Read preset list from persistency"));
    /* if(CTunerPresetFM::readPreset() != 0)
     {
       DBG_MSG(("Preset read from persistency failed !!!"));
     }
     else
     {
       DBG_MSG(("Preset read from persistency succeeded !!!"));
     }*/

     DBG_MSG(("Setting the persistency flag as false"));
     CTunerCmdInvoker::GetInstance()->setPersistencyFlag(false);
      if(CHBTunerPersistency::getInstance()->readFromPersistency(1)!= 0)
      {
        DBG_MSG(("Preset read from persistency failed !!!"));
      }
      else
      {
        DBG_MSG(("Preset read from persistency succeeded !!!"));
      }

     //If we are here then it means that everything was created with success
     mInitialized = true;
   }//if( mInitialized == false )

   //start the Tuner station proxy thread
   (mpInstance->GetInstance())->createTunerControlProxyThread();
   // sleep(2);
   (mpInstance->GetInstance())->createTunerDiranaControlThread();

   //start the Tuner station proxy thread
   (mpInstance->GetInstance())->createTunerStationProxyThread();
   //sleep(2);
   //start the Tuner announcement proxy thread
   (mpInstance->GetInstance())->createTunerAnnouncementProxyThread();
   //sleep(2);
   //start the Tuner Audio proxy thread
   (mpInstance->GetInstance())->createTunerAudioProxyThread();

   //create Scan Handler Instance
   CTunerScanHandler::mScanHandler = new CTunerScanHandler();

   string str;

   // Initializing with US Region
   regionCode = 2;
   //printf("Initializing for US Region \n");
   //Read the HD CCF value, to determine if HD should be available for the current configuration

   if(mControlProxy != NULL)
   {
	 //printf(" Calling  processInternalEvent to Set Region for US\n");
     mControlProxy->processInternalEvent(DMMAmFmTunerControl_UPD_ID_requestSetRegionCode,(void*)&regionCode);
   }
   CTunerSeekHandler* mSeek = new CTunerSeekHandler();

   DBG_MSG(("Main Region code value is outside %d ",regionCode));
   mSeek->updateFrequency(regionCode);
   delete(mSeek);
   return mInitialized;
  
}

string CTunerRequestResponseHandler::parseCCFfile(char * attribName)
{
	  TRC_SCOPE(TunerService, CTunerRequestResponseHandler, parseCCFfile);
	  string readString, value;
      ifstream fp("/fs/etfs/CCFParameters.ccf");
      if (fp.is_open())
      {
            DBG_MSG(("Opened file\n"));
            getline (fp,readString);
            std::size_t found, foundEnd;
            found = readString.find(attribName);
            if(found != std::string::npos)
            {
                  found = readString.find(":",found+1,1);
                  if(found != std::string::npos)
                  {
                	  DBG_MSG(("read string found at %d",found));
                  }
                  else
                  {
                	    DBG_MSG((": not found"));
                        fp.close();
                        return NULL;
                  }
                  foundEnd = readString.find(",",found++,1);
                  if(foundEnd != std::string::npos)
                  {
                        while(found != foundEnd)
                              value = value + (readString.c_str()[found++]);
                        value = value + '\0';
                        DBG_MSG(("read string %s",value.c_str()));
                  }
                  else
                  {
                	    DBG_MSG((", not found\n"));
                        value = "";
                  }
            }
            fp.close();
      }
      else
      {
    	    DBG_MSG(("ReadFile: Unable to open file\n"));
            value = "";
      }
      return value;
}

static void* runTunerStationProxy(void *arg)
{
	//cout << "runTunerStationProxy " << endl;
   CTunerRequestResponseHandler::mStationProxy = new CDMMTunerStationProxy();
   if(NULL != CTunerRequestResponseHandler::mStationProxy)
   {
     CTunerRequestResponseHandler::mStationProxy->start("AMFMTunerDSIDevice");
   }

   return 0;
}

static void* runTunerControlProxy(void *arg)
{
	//cout << "runTunerControlProxy " << endl;
   INT32 returnvalue = 0;
   CTunerRequestResponseHandler::mControlProxy = new CDMMAmFmTunerControlProxy();
   if(NULL != CTunerRequestResponseHandler::mControlProxy)
   {
     returnvalue = CTunerRequestResponseHandler::mControlProxy->start("AMFMTunerDSIDevice");
   }

   return 0;
}
static void* runTunerAnnouncementProxy(void *arg)
{
	//cout << "runTunerAnnouncementProxy " << endl;
   INT32 returnvalue = 0;
   CTunerRequestResponseHandler::mAnnouncementProxy = new CDMMTunerAnnouncementProxy();
   if(NULL != CTunerRequestResponseHandler::mAnnouncementProxy)
   {
     returnvalue = CTunerRequestResponseHandler::mAnnouncementProxy->start("AMFMTunerDSIDevice");
   }
   return 0;
}

static void* runTunerAudioProxy(void *arg)
{
	//cout << "runTunerAudioProxy " << endl;
   INT32 returnvalue = 0;
   CTunerRequestResponseHandler::mAudioProxy = new CDMMTunerAudioProxy();
   if(NULL != CTunerRequestResponseHandler::mAudioProxy)
   {
     returnvalue = CTunerRequestResponseHandler::mAudioProxy->start("AMFMTunerDSIDevice");
   }
   return 0;
}

static void* runTunerDiranaControlProxy(void *arg)
{
   INT32 returnvalue = 0;

   //printf("\n runTunerDiranaControlProxy \n");
   CTunerRequestResponseHandler::mDiranaControlProxy = new CDMMAmFmTunerDiranaControlProxy();
   if(NULL != CTunerRequestResponseHandler::mDiranaControlProxy)
   {
     returnvalue = CTunerRequestResponseHandler::mDiranaControlProxy->start("AMFMTunerDSIDevice");
   }
   return 0;
}


void CTunerRequestResponseHandler::handleNotif(std::string signalName,Json::Value res)
{
   Json::FastWriter mJsonWriter;
   std::string signalStr  = mJsonWriter.write(res);
   SVCIPC_tError errorCode = SVCIPC_ERROR_NONE;

   CTunerRequestResponseHandler* inst = CTunerRequestResponseHandler::GetInstance();
   errorCode = inst->emit(signalName.c_str(), signalStr.c_str());
}
void CTunerRequestResponseHandler::sendResponse(CSvcIpcRequest* reqData, tunerErrorTypes error)
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
     reqData->returnReply(ret);
   }

}
void CTunerRequestResponseHandler::sendPresets(CSvcIpcRequest* reqData, Json::Value res)
{
   TRC_SCOPE(TunerService, CTunerRequestResponseHandler, sendPresets);
   DBG_MSG(("[CTunerRespHandler]: Presets sent !!"));
   Json::FastWriter mJsonWriter;
   reqData->returnReply(res);
}
//----------------------------------------------------- END OF FILE ---------------------------------------------------
