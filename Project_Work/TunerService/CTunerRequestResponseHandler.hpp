#ifndef _TUNER_REQUEST_RESPONSE_HANDLER_HPP_
#define _TUNER_REQUEST_RESPONSE_HANDLER_HPP_

/*******************************************************************************
 *  Project       JLR NLI
 *  (c) copyright 2013
 *  Company       Harman International
 *                All rights reserved
 * Secrecy Level STRICTLY CONFIDENTIAL
 ******************************************************************************/

/**
 *  @file         TunerServiceMain.hpp
 *  @brief        This is the main class of Tuner Service and it is a singleton
 *  @authors
 *  @ingroup      Tuner Service
 */

/*-----------------------------------------------------------------------------\
 * Preprocessor Includes
\*----------------------------------------------------------------------------*/
#include "svcIpcEventHandling/CSvcIpcAccessoryEventTarget.hpp"
#include "svcIpcEventHandling/CSvcIpcEventRouter.hpp"
#include "svcIpcEventHandling/CSvcImplBase.hpp"
#include "svcIpcEventHandling/CSvcClientBase.hpp"
#include "CTunerDataTypes.hpp"
#include "CTunerReqQueue.hpp"
#include "CDMMTunerStationProxy.hpp"
#include "CDMMAmFmTunerControlProxy.hpp"
#include "CDMMTunerAnnouncementProxy.hpp"
#include "CDMMTunerAudioProxy.hpp"
#include "CDMMAmFmTunerDiranaControlProxy.hpp"
#include <iostream>

using namespace std;

/*-----------------------------------------------------------------------------\
 * Preprocessor Defines (Macros, Constants)
\*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------\
 * Type Definitions (Enums, Typedefs, Structs, ...)
\*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------\
 * Constant And External Variables
\*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------\
 * Function Prototypes
\*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------\
 * Class Declarations
\*----------------------------------------------------------------------------*/

class CTunerRequestResponseHandler: public CSvcImplBase, public CSvcClientBase /*Singleton Pattern*/
{
/*.............................................................................\
 * Public Members
\*............................................................................*/
public:
	 static CDMMTunerStationProxy* mStationProxy;
	 static CDMMAmFmTunerControlProxy* mControlProxy;
	 static CDMMTunerAnnouncementProxy* mAnnouncementProxy;
	 static CDMMTunerAudioProxy*        mAudioProxy;
	 static CDMMAmFmTunerDiranaControlProxy*        mDiranaControlProxy;
	 static INT32 regionCode;
	 static BOOL   mInitialized;
	 static std::string mLastSource;
	 static std::string mCurrentSource;
	 static BOOL vrStatus;
	 static BOOL taStatus;

public:
	 CTunerRequestResponseHandler();
	 ~CTunerRequestResponseHandler();

   static CTunerRequestResponseHandler* GetInstance(void)
   {
      if(NULL == mpInstance)
      {
         mpInstance = new CTunerRequestResponseHandler();

         if(mpInstance)
            mpInstance->Initialize();
      }
      return mpInstance;
   }
   
   static void DeleteInstance(void)
   {
      if(NULL != mpInstance)
      {
         mpInstance->deinitialize();
         
         delete mpInstance;
         mpInstance = NULL;
      }
   }

   static void handleNotif(std::string signalName,Json::Value res);
   static void sendResponse(CSvcIpcRequest* reqData, tunerErrorTypes error);
   static void sendPresets(CSvcIpcRequest* reqData, Json::Value res);
   static bool getStationComponentStatus(void);
   static void setStationComponentStatus(bool stationStatus);

   static bool getControlComponentStatus(void);
   static void setControlComponentStatus(bool controlStatus);

   static bool getAnnouncementComponentStatus(void);
   static void setAnnouncementComponentStatus(bool announcementStatus);

   static bool getAudioComponentStatus(void);
   static void setAudioComponentStatus(bool audioStatus);

   static CDMMTunerStationProxy* getStationProxy();
   static CDMMAmFmTunerControlProxy* getControlProxy();
   static CDMMTunerAnnouncementProxy* getAnnouncementProxy();
   static CDMMTunerAudioProxy* getAudioProxy();
   string parseCCFfile(char * attribName);
  // void startUp();
   	//static void Inv(void);
   void setHwVariant(BOOL hwVariant);
   BOOL getHwVariant(void);
protected:
     SVCIPC_tResponse* m_resp;

     void processSvcIpcRequest( CSvcIpcRequest* pRequest );
	 void processSvcIpcSignal( const _Uint32t & sigRegHndl, const CSvcIpcSignal* pSignal, void* pUserData );
	 void processSvcIpcNameChange( const _Uint32t & nameChgRegHndl, const CSvcIpcNameChange* pNameChange, void* pUserData );
	 void processSvcIpcResult( const _Uint32t & resultRegHndl, SVCIPC_tConstStr result, void* pUserData );
	 void processSvcIpcError( const _Uint32t & resultRegHndl, SVCIPC_tErrorCode error, SVCIPC_tConstStr errMsg, SVCIPC_tConstStr errName );
/*.............................................................................\
 * Private Members
\*............................................................................*/
private:
    
	//SVCIPC_tConnection 		gSvcIpcConnection;
	//SVCIPC_tSvcRegHnd       gSvcIpcRegHdlr;
/*.............................................................................\
 * Private Constructor and Destructor, because this is singleton class
\*............................................................................*/

      
/*.............................................................................\
 * Copy Constructor and overloaded = operator
\*............................................................................*/

      //CTunerServiceMain(const CTunerServiceMain& copyFrom);
     // CTunerServiceMain& operator= (CTunerServiceMain& copyFrom);

/*..............................................................................
 * @brief initialize()- Init the Tuner Service
 *
 * Input Parameters:
 *    @param void None
 *
 * Return Value:
 *    @retval rval true on success or else false
 *............................................................................*/

  BOOL Initialize(void);

/*..............................................................................
 * @brief deinitialize()- Init the Tuner service component
 *
 * Input Parameters:
 *    @param void None
 *
 * Return Value:
 *    @retval rval true on success or else false
 *............................................................................*/
      BOOL deinitialize(void);

/*..............................................................................
 * @brief registerTunerService()- Init the Tuner service component
 *
 * Input Parameters:
 *    @param void None
 *
 * Return Value:
 *    @retval rval true on success or else false
 *............................................................................*/
	//  BOOL registerTunerService(void);
	//  INT32 createRequestQueueMutex(void);
	//  INT32 createWorkerSemaphore(void);
    //  INT32 createTunerWorkerThread(void);
      UINT32 createTunerStationProxyThread(void);
      UINT32 createTunerControlProxyThread(void);
      UINT32 createTunerAnnouncementProxyThread(void);
      UINT32 createTunerAudioProxyThread(void);
      UINT32 createTunerDiranaControlThread(void);


private:
   static CTunerRequestResponseHandler* mpInstance;
   static BOOL controlComponentStatus;
   static BOOL stationComponentStatus;
   static BOOL announcementComponentStatus;
   static BOOL audioComponentStatus;
   static BOOL mHwVariant;

   //BOOL   mInitialized;
   UINT32 mPowerModeSignalRegHandle;
   UINT32 mSourceChangeRegHandle;
   UINT32 mInterruptSourceRegHandle;
   UINT32 mAudioSourceChange;
   _Uint32t mCCFDataHandle;


   /*inline BOOL getTunerInitializedStatus()
   {
      return mInitialized;
   }*/

};

#endif /* _TUNER_REQUEST_RESPONSE_HANDLER_HPP_ */

/*--------------------------------- EOF --------------------------------------*/
