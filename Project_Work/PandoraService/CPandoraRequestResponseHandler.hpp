#ifndef _TUNER_REQUEST_RESPONSE_HANDLER_HPP_
#define _TUNER_REQUEST_RESPONSE_HANDLER_HPP_

/**********************************************************************
 */

/*-----------------------------------------------------------------------------\
 * Preprocessor Includes
\*----------------------------------------------------------------------------*/
#include "svcIpcEventHandling/CSvcIpcAccessoryEventTarget.hpp"
#include "svcIpcEventHandling/CSvcIpcEventRouter.hpp"
#include "svcIpcEventHandling/CSvcImplBase.hpp"
#include "svcIpcEventHandling/CSvcClientBase.hpp"
#include <iostream>
#include <sys/stat.h>
#include <CPandoraDataTypes.hpp>
#include <sys/usbdi.h>
#include <sys/select.h>


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


	 static std::string mLastSource;
	 static std::string mCurrentSource;
	 std::string SPP_Pandora_DeviceAddress;
	 std::string SPP_IAP_Iphone_DeviceAddress;
	 std::string SPP_Pandora_Connected_DeviceAddress;
	 std::string SPP_IAP_Iphone_Connected_DeviceAddress;

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
   static void profileDisconnectMethodCall();
   static void profileConnectMethodCall();
   //static void sendResponse(CSvcIpcRequest* reqData);
   static void sendResponse(CSvcIpcRequest* reqData, PandoraErrorTypes error);
   static void sendPresets(CSvcIpcRequest* reqData, Json::Value res);

   void startMonitoring ();
   void stopMonitoring ();
   int createPandoraWorkerThread();
   int setupConnection();
   SVCIPC_tResponse* m_resp;


protected:

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

  bool Initialize(void);

/*..............................................................................
 * @brief deinitialize()- Init the Tuner service component
 *
 * Input Parameters:
 *    @param void None
 *
 * Return Value:
 *    @retval rval true on success or else false
 *............................................................................*/
  bool deinitialize(void);

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

      int createTunerAudioProxyThread(void);








private:

   unsigned int mBluetoothServiceConnected;
   unsigned int mMediaDeviceState;
   unsigned int mBluetoothServiceDisConnected;
   unsigned int mPowerModeSignalRegHandle;
   unsigned int mSPP_PANDORAserviceConnectError;
   unsigned int mIgnoreBtDisconnectionSignal;
   struct usbd_connection *mConnection;


public:
   static std::uint8_t buf[2048];
   static int mPandoraDeviceFd;
   static bool mAndroidDeviceConnected;
   static bool mIpodDeviceConnected;
   static bool mIphoneBluetooth;
   static bool mIphoneStartDevCtl;
   static int MsgConnectionId;
   static int channelId;
   static int mPowerMode;
   static CTunerRequestResponseHandler* mpInstance;
   int getPandoraDeviceFd();
   void afterPandoraDisconnectChecking();
   void afterIgnoreBtDisconnectionSignal();
   void PandoraSocketDisconnect();
   void initPandoraAudioGraphAgain();
   static char 	usbInsrtPath;
   static char 	usbRmvlPath;
   static char 	pandoraSessionConnectedDevice;
   static char 	IpodPandoraPlayStatus;
   static char 	segmentsCount;
   static bool 	PANDORA_CONNECTED_DEVICE_IPOD_0;
   static bool 	PANDORA_CONNECTED_DEVICE_IPOD_1;
   static bool 	PANDORA_CONNECTED_DEVICE_IPHONE;
   static bool 	PANDORA_CONNECTED_DEVICE_ANDROID;
   static bool 	SPP_IAP_Iphone_SERVICE_DISCONNECT_INITIATED;
   static bool 	SPP_PANDORA_SERVICE_DISCONNECT_INITIATED;
   static bool 	setPandoraAudioAutoRecovery;
   static bool 	PandoraDisconnectCheckingFlag;
   static char 	powerModePandoraReadyFalse;
   static bool  mStationTokenLists;
   static bool  mPandoraNoActiveStation;
   static bool  mPndrStatusInvalidLogin;
   static bool  mPndrReqPlayTrack;
   static bool  mPndrStartSessionReqRcvd;
   static bool  mPandoraSessionStartupFunctionCalls;
   static bool  mPandoraAvoidIpodPauseStatus;//workaround
   static bool  mIgnoreBtDisconnection;
   static bool  mIgnoreTerminateSessionCmd;
   static bool  mIgnoreTerminateSessionCmdOnLaunchAppFailure;
   static char 	pandoraConnectedDeviceName;
};

#endif /* _TUNER_REQUEST_RESPONSE_HANDLER_HPP_ */

/*--------------------------------- EOF --------------------------------------*/
