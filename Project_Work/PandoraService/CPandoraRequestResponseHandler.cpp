/*----------------------------------------------------------------------------------------------------------------------
 *
 * Project Harman Mid Systems - Pandora Service
 * (c) copyright 2013
 * Company Harman International
 * All rights reserved
 * Secrecy Level STRICTLY CONFIDENTIAL
 *
 *-----------------------------------------------------------------------------------------------------------------------
 *
 * @file PandoraServiceMain.hpp
 * @ingroup PandoraService
 * @author Monojit Chatterjee
 * Public API for the Tuner core service library.
 *
 *---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
 *----------------------------------------------------------------------------------------------------------------------*/
#include "api/sys/dsi/pf/colibry/src/CDSIConnection.hpp"
#include "api/sys/colibry/pf/oswrapper/src/CHBThreadCtrl.hpp"
#include "api/sys/dsi/pf/colibry/src/CDSIConnection.hpp"
#include "IPandoraListenerImpl.hpp"


#include "CPandoraCmdInvoker.hpp"
#include "CPandoraReqQueue.hpp"
#include "CPandoraRequestResponseHandler.hpp"
#include "IIpodNotificationImpl.hpp"
#include <CPandoraMessageSenderImpl.hpp>

#include "PandoraPersistency.hpp"
#include <fcntl.h>
#include <fstream>
#include "TraceMacros.hpp"
#include "iomedia/api.h"
#include <sys/dcmd_media.h>

#define ONOFF_SERVICE_BUS_NAME "com.harman.service.YAMAHAOnOff"
#define ONOFF_SERVICE_OBJ_NAME "/com/harman/service/YAMAHAOnOff"

#define BLUETOOTH_SERVICE_BUS_NAME "com.harman.service.BluetoothService"
#define BLUETOOTH_SERVICE_OBJ_NAME "/com/harman/service/BluetoothService"

#define AUDIOSERVICE_BUS_NAME  "com.harman.service.AudioManager"
#define AUDIOSERVICE_OBJ_NAME  "/com/harman/service/AudioManager"

#define AUDIOMIXERSERVICE_BUS_NAME  "com.harman.service.AudioMixerManager"
#define AUDIOMIXERSERVICE_OBJ_NAME  "/com/harman/service/AudioMixerManager"

#define CCF_SERVICE_BUS_NAME "com.harman.service.CCFParameters"
#define CCF_SERVICE_OBJ_NAME "/com/harman/service/CCFParameters"

#define PERSISTENCY_DBUS_NAME "com.harman.service.PersistentKeyValue"
#define PERSISTENCY_DBUS_OBJECT_PATH "/com/harman/service/PersistentKeyValue"
#define MEDIA_SERVICE_DBUS_NAME   "com.harman.service.Media"
#define MEDIA_SERVICE_DBUS_OBJECT_PATH "/com/harman/service/Media"

#define MODEMANAGER_SERVICE_OBJ_NAME "/com/harman/service/ModeManager"
#define MODEMANAGER_IGNORE_BT_DISCONNECTION 	"ignoreBtDisconnection"

#define ONOFF_POWERMODE_SIGNAL 				"powerMode"
#define BLUETOOTH_PANDORA_SERVICE_CONNECTED "serviceConnected"
#define BLUETOOTH_PANDORA_SERVICE_DISCONNECTED "serviceDisconnected"
#define BLUETOOTH_SPP_PANDORA_SERVICE_CONNECT_ERROR "serviceConnectError"

#define AUDIOSERVICE_SOURCE_SWITCH_SIGNAL 	"audioSource"
#define AUDIOMIXERSERVICE_INTERRUPT_SIGNAL  "interruptSrc"

#define MEDIA_SERVICE_DEVICE_STATE "deviceState"
#define PM_STANDBY_MODE		 	1
#define	PM_PROTECTION_MODE	 	2
#define	PM_FULLYWORKING_MODE 	3
#define	PM_POWERSAVING_MODE  	4
#define IPOD_USB_0			 	1
#define	IPOD_USB_1			 	2
#define PANDORA_READY_STATE	 	1
#define	PANDORA_APP_STATE	 	2
#define	PANDORA_APP_AVAILABLE 	3
#define PANDORA_SESSION_IPOD	1
#define	PANDORA_SESSION_IPHONE	2
#define	PANDORA_SESSION_ANDROID	3
#define PANDORA_DEVICE_ANDROID  1
#define PANDORA_DEVICE_IPHONE   2
#define PANDORA_DEVICE_IPOD     3


CTunerRequestResponseHandler* CTunerRequestResponseHandler::mpInstance = NULL;
std::uint8_t CTunerRequestResponseHandler::buf[2048] = {NULL};

bool CTunerRequestResponseHandler::mAndroidDeviceConnected = false;
bool CTunerRequestResponseHandler::mIpodDeviceConnected = false;
bool CTunerRequestResponseHandler::mIphoneBluetooth = false;
bool CTunerRequestResponseHandler::mIphoneStartDevCtl = false;
char CTunerRequestResponseHandler::usbInsrtPath = -1;
char CTunerRequestResponseHandler::usbRmvlPath = -1;
char CTunerRequestResponseHandler::IpodPandoraPlayStatus = 0;
char CTunerRequestResponseHandler::pandoraSessionConnectedDevice = 0;
char CTunerRequestResponseHandler::segmentsCount = 0;
bool CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_0 = false;
bool CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_1 = false;
bool CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPHONE = false;
bool CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_ANDROID = false;
bool CTunerRequestResponseHandler::SPP_IAP_Iphone_SERVICE_DISCONNECT_INITIATED = false;
bool CTunerRequestResponseHandler::SPP_PANDORA_SERVICE_DISCONNECT_INITIATED = false;
bool CTunerRequestResponseHandler::setPandoraAudioAutoRecovery = false;
bool CTunerRequestResponseHandler::PandoraDisconnectCheckingFlag = true;
char CTunerRequestResponseHandler::powerModePandoraReadyFalse = 0;
bool CTunerRequestResponseHandler::mStationTokenLists = true;
bool CTunerRequestResponseHandler::mPandoraNoActiveStation = false;
bool CTunerRequestResponseHandler::mPndrStatusInvalidLogin = false;
bool CTunerRequestResponseHandler::mPndrReqPlayTrack = true;
bool CTunerRequestResponseHandler::mPandoraSessionStartupFunctionCalls = true;
bool CTunerRequestResponseHandler::mPandoraAvoidIpodPauseStatus = true;
bool CTunerRequestResponseHandler::mPndrStartSessionReqRcvd = false;
bool CTunerRequestResponseHandler::mIgnoreBtDisconnection = false;
bool CTunerRequestResponseHandler::mIgnoreTerminateSessionCmd = false;
bool CTunerRequestResponseHandler::mIgnoreTerminateSessionCmdOnLaunchAppFailure = false;
int CTunerRequestResponseHandler::mPowerMode = 0;
char CTunerRequestResponseHandler::pandoraConnectedDeviceName = 0;
int channel_id;
pid_t process_id;

pthread_t mPandoraWorkerThreadProxy;


/*----------------------------------------------------------------------------------------------------------------------
 * File scope functions
----------------------------------------------------------------------------------------------------------------------*/

pthread_t   mPandoraReaderThreadProxy;
static void* runPandoraReaderThread(void *arg);
static int notifyUpdateInFile(int fd);
static pthread_mutex_t  mPandoraReaderThreadMutex;
IPandoraProtocolListener *listener = new IPandoraListenerImpl;
CPandoraProtocolMgr* vPandoraProtocolMgr = new CPandoraProtocolMgr(listener);
CIap1Config* vCIap1Config = new CTunerCmdInvoker();

CIap1MgrController* vCIap1MgrController = new CIap1MgrController(vCIap1Config);
IIPodNotification* vIIPodNotification = new IIpodNotificationImpl();
extern CTimer *mTimerInstance;
extern CTimer *mPandoraDeviceCheck;
extern CTimer *mPandoraAudioTimer;

struct usbd_connection    *connection;

/*----------------------------------------------------------------------------------------------------------------------
 * Static attributes
----------------------------------------------------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------------------------------------------------
 * TraceClient Scope Definitions
----------------------------------------------------------------------------------------------------------------------*/
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, Initialize);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, processSvcIpcRequest);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, processSvcIpcSignal);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, sendPresets);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, createTunerStationProxyThread);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, createTunerControlProxyThread);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, createTunerAnnouncementProxyThread);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, CTunerRequestResponseHandler);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, processSvcIpcNameChange);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, createTunerAudioProxyThread);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, processSvcIpcResult);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, processSvcIpcError);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, parseCCFfile);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, createTunerDiranaControlThread);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, runPandoraReaderThread);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, notifyUpdateInFile);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, insertionHndlr);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, removalHndlr);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, startMonitoring);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, stopMonitoring);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, createPandoraWorkerThread);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, setupConnection);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, runPandoraWorkerThread);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, getPandoraDeviceFd);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, afterPandoraDisconnectChecking);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, profileDisconnectMethodCall);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, initPandoraAudioGraphAgain);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, profileConnectMethodCall);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, PandoraSocketDisconnect);
TRC_SCOPE_DEF(PandoraService, CTunerRequestResponseHandler, afterIgnoreBtDisconnectionSignal);

int CTunerRequestResponseHandler::mPandoraDeviceFd = -1;
int CTunerRequestResponseHandler::MsgConnectionId = -1;

int CTunerRequestResponseHandler::channelId = -1;
void insertionHndlr(struct usbd_connection * conn, usbd_device_instance_t *instance);
void removalHndlr(struct usbd_connection * conn, usbd_device_instance_t *instance);

//IPandoraProtocolListener *listener = new IPandoraListenerImpl;
//CPandoraProtocolMgr* vPandoraProtocolMgr = new CPandoraProtocolMgr(listener);

/*......................................................................................................................
 * @brief CTunerServiceMain()- Constructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerRequestResponseHandler::CTunerRequestResponseHandler(): CSvcImplBase("com.harman.service.PandoraService",
		"/com/harman/service/PandoraService", true)
{

	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, CTunerRequestResponseHandler);
	//We want to know if the OnOff Service comes alive or dies.
	//(void)subscribeOwnerChanged( ONOFF_SERVICE_BUS_NAME );

	//We want to know if the CCF Service comes alive or dies.
	(void)subscribeOwnerChanged( CCF_SERVICE_BUS_NAME );
	(void)subscribeOwnerChanged( BLUETOOTH_SERVICE_BUS_NAME );
	(void)subscribeOwnerChanged( MEDIA_SERVICE_DBUS_NAME );
	if(nameHasOwner(PERSISTENCY_DBUS_NAME))
	{
		DBG_MSG(("[CTunerRequestResponseHandler]: Persistency service already available"));
	}
	else
	{
		(void)subscribeOwnerChanged( PERSISTENCY_DBUS_NAME );
		DBG_MSG(("[CTunerRequestResponseHandler]: Subscribed for Persistency service availability"));
	}

	mBluetoothServiceConnected = subscribe( BLUETOOTH_SERVICE_OBJ_NAME, BLUETOOTH_PANDORA_SERVICE_CONNECTED );
	mBluetoothServiceDisConnected = subscribe( BLUETOOTH_SERVICE_OBJ_NAME, BLUETOOTH_PANDORA_SERVICE_DISCONNECTED);
	mMediaDeviceState = subscribe( MEDIA_SERVICE_DBUS_OBJECT_PATH, MEDIA_SERVICE_DEVICE_STATE );
	//Check if the OnOff Service is already alive.
	mPowerModeSignalRegHandle = subscribe( ONOFF_SERVICE_OBJ_NAME, ONOFF_POWERMODE_SIGNAL );
	//Check for SPP_PANDORA serviceConnectError Signal.
	mSPP_PANDORAserviceConnectError = subscribe( BLUETOOTH_SERVICE_OBJ_NAME, BLUETOOTH_SPP_PANDORA_SERVICE_CONNECT_ERROR );
	mIgnoreBtDisconnectionSignal = subscribe( MODEMANAGER_SERVICE_OBJ_NAME, MODEMANAGER_IGNORE_BT_DISCONNECTION );
	//DBG_MSG(("Power Mode signal subscription success"));

	// mSourceChangeRegHandle = subscribe( AUDIOSERVICE_OBJ_NAME, AUDIOSERVICE_SOURCE_SWITCH_SIGNAL );

	//mInterruptSourceRegHandle = subscribe( AUDIOMIXERSERVICE_OBJ_NAME, AUDIOMIXERSERVICE_INTERRUPT_SIGNAL );
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

//Notify if there is update in the provided fd
static int notifyUpdateInFile(int fd)
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, notifyUpdateInFile);
	int     retval = EOK;
	fd_set  rfd;
	int     selectRet = -1;
	struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
	FD_ZERO( &rfd );
	FD_SET( fd, &rfd );

	//DBG_MSG(("\n[PANDORA]Calling select!\n"));
	//selectRet = select( FD_SETSIZE , &rfd, NULL, NULL, &tv );
	selectRet = select( FD_SETSIZE , &rfd, NULL, NULL, NULL );
	switch ( selectRet )
	{
		case -1:
			if (errno == EINTR)
			{
				retval = errno;
				DBG_MSG(("%s","[PANDORA]select() filed; EINTR occured"));
			}
			else
			{
				retval = errno;
				DBG_MSG(("[PANDORA]select() returned -1, error: %s",strerror(errno)));
			}
			break;
		default:
			if ( FD_ISSET( fd, &rfd ) )
			{
				retval = EOK;
			}
			break;
	}
	return retval;
}

static void* runPandoraReaderThread(void *arg)
						{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, runPandoraReaderThread);
	pthread_setname_np (pthread_self(), "PandoraAndroidThread");
	DBG_MSG(("runPandoraReaderThread"));
	ssize_t sReadNoBytes = 0;
	CPandoraProtocolMgr* vPandoraProtocolMgr = (CPandoraProtocolMgr*)arg;
	vPandoraProtocolMgr->setDebugLevel(0);
	while(1)
	{
		//DBG_MSG(("[PANDORA]While mAndroidDeviceConnected=%d,mPandoraDeviceFd=%d",CTunerRequestResponseHandler::mAndroidDeviceConnected,CTunerCmdInvoker::mPandoraDeviceFd));
		if(CTunerRequestResponseHandler::mAndroidDeviceConnected == true && (CTunerCmdInvoker::mPandoraDeviceFd != -1) )//&& (CTunerCmdInvoker::SessionTerminated == 0) )
		{

			///////////////////////////////////
			int retVal = EOK;
			memset( CTunerRequestResponseHandler::buf, 0, 2048 );
			retVal = notifyUpdateInFile(CTunerCmdInvoker::mPandoraDeviceFd);
			//DBG_MSG(("[PANDORA]Before Read mPandoraDeviceFd=%d, retVal:%d\n",CTunerCmdInvoker::mPandoraDeviceFd,retVal));
			if(retVal == EOK)
			{
				retVal = read(CTunerCmdInvoker::mPandoraDeviceFd, CTunerRequestResponseHandler::buf, 2048);
				if(retVal == -1)
				{
					DBG_MSG(("[PANDORA]read failed: %s\n",strerror(errno)));
				}
				else
				{
					//DBG_MSG(("[PANDORA]Number of bytes Read from phone %d",retVal));
					if(retVal > 0)
					{
						//DBG_MSG(("Reading Bytes from the Phone %d",retVal));
						//CTunerCmdInvoker::GetInstance()->printData(CTunerRequestResponseHandler::buf, retVal, eRead);
						vPandoraProtocolMgr->processDataReceived(CTunerRequestResponseHandler::buf, retVal);
					}
				}
			}
			else
			{
				DBG_MSG(("[PANDORA]Retrying for mPandoraDeviceFd\n"));
				CTunerCmdInvoker::mPandoraDeviceFd  = CTunerRequestResponseHandler::mpInstance->getPandoraDeviceFd();
			}
			///////////////////////////////////
			/*DBG_MSG(("Before Mutex"));
			pthread_mutex_lock( &mPandoraReaderThreadMutex );
			DBG_MSG(("Reading started from the Device file"));
			memset( CTunerRequestResponseHandler::buf, 0, 2048 );
			DBG_MSG(("Before Read"));
			sReadNoBytes = read(CTunerCmdInvoker::mPandoraDeviceFd, CTunerRequestResponseHandler::buf, 2048);
			DBG_MSG(("Number of bytes Read from phone %d",sReadNoBytes));

			if(sReadNoBytes > 0)
			{
				DBG_MSG(("Reading Bytes from the Phone %d",sReadNoBytes));
				CTunerCmdInvoker::GetInstance()->printData(CTunerRequestResponseHandler::buf, sReadNoBytes, eRead);
				vPandoraProtocolMgr->processDataReceived(CTunerRequestResponseHandler::buf, sReadNoBytes);
			}
			DBG_MSG(("ProcessDataReceived"));
			pthread_mutex_unlock( &mPandoraReaderThreadMutex );
			DBG_MSG(("Reading ended from the Device file"));*/
		}
		else
		{
			DBG_MSG(("Bluetooth Device disconnected"));
			if(CTunerRequestResponseHandler::mAndroidDeviceConnected == true)
			{
				DBG_MSG(("intiate profileDisconnect mPandoraDeviceFd:%d",CTunerCmdInvoker::mPandoraDeviceFd));
				//CTunerRequestResponseHandler::mpInstance->PandoraSocketDisconnect();
			}
			pthread_exit(NULL);
			break;
		}
	}


	//return 0;
}

static void* runPandoraWorkerThread(void *arg)
		{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, runPandoraWorkerThread);
	int recv_id;
	struct _pulse recv_msg;

	/*struct sched_param ssParam;
	int    Policy = 0;
	pthread_getschedparam (pthread_self(), &Policy, &ssParam );
	pthread_setschedparam (pthread_self(), Policy, &ssParam );
	pthread_setschedprio (pthread_self(),12);*/
	pthread_setname_np (pthread_self(), "PandoraWorkerThread");

	process_id = getpid();
	//printf("Process id: %d", process_id);

	/* Set up a channel through which clients can connect to me. */
	//CTunerRequestResponseHandler::channelId = ChannelCreate(_NTO_CHF_FIXED_PRIORITY);
	CTunerRequestResponseHandler::channelId = ChannelCreate(0);
	if (CTunerRequestResponseHandler::channelId == -1)
	{
		DBG_MSG(("Channel Creation Failed"));
		return NULL;
	}
	DBG_MSG(("Channel Id %d", CTunerRequestResponseHandler::channelId));

	/* Keep running indefinitely. */
	while (1)
	{
		/* Block until a message is ready for me on the specified channel.
		 * Store the ID of the client, so I can reply to it uniquely. */
		recv_id = MsgReceivePulse( CTunerRequestResponseHandler::channelId, &recv_msg, sizeof(recv_msg), NULL );
		//DBG_MSG(("MsgReceivePulse %d and Msg Type is %d", CTunerRequestResponseHandler::channelId, recv_msg.code));
		switch(recv_msg.code)
		{
		case Pandora_Ack_Received:
		{
			//DBG_MSG(("[ThreadContext]::Pandora_Ack_Received"));
			if(CPandoraMessageSenderImpl::mPandoraRequestQueue.front()->getCommand() == 0x00)
			{
				DBG_MSG(("Ack Received for %s", CPandoraMessage::sendCommandToString(CPandoraMessageSenderImpl::mPandoraRequestQueue.front()->getCommand())));
				CTunerCmdInvoker::meSessionState  = eSessionActive;
				if(CTunerRequestResponseHandler::mIgnoreTerminateSessionCmdOnLaunchAppFailure == true)
				{
					CTunerRequestResponseHandler::mIgnoreTerminateSessionCmdOnLaunchAppFailure = false;
					Json::Value dataAppAvl;
					std::string signalAppAvl = "PandoraReadyState";
					dataAppAvl["appStatus"] = PANDORA_APP_AVAILABLE;
					dataAppAvl["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
					dataAppAvl["value"] = true;
					CTunerRequestResponseHandler::handleNotif(signalAppAvl, dataAppAvl);
					DBG_MSG(("[IPandoraListenerImpl]:IPHONE/IPOD Start Session Ack Received"));
				}
			}
			// If User has request terminate then Stop Protocol Process
			if(CPandoraMessageSenderImpl::mPandoraRequestQueue.front()->getCommand() == 0x05)
			{
				DBG_MSG(("[IPandoraListenerImpl]:Terminate Session Ack Received"));
				CTunerCmdInvoker::sTerminateSession = eRequestNone;
				//DBG_MSG(("[IPandoraListenerImpl]: deInitialize"));
				if(CTunerRequestResponseHandler::mPndrStartSessionReqRcvd == false)
				{
					CPandoraMessageSenderImpl::clearQueue();
				}
				vPandoraProtocolMgr->deInitialize();

				CTunerCmdInvoker::meSessionState = eSessionNone;
				//DBG_MSG(("[IPandoraListenerImpl]:CTunerCmdInvoker::meSessionState %d ", CTunerCmdInvoker::meSessionState));
				//CTunerCmdInvoker::mStationActive == false;

				Json::Value data;
				std::string signalName = "InfoUpdateStationActive";
				data["StationToken"] = 0;
				data["value"] = false;
				CTunerRequestResponseHandler::handleNotif(signalName, data);
				//DBG_MSG(("[runPandoraWorkerThread]: Session Terminated: InforUpdateStationActive"));

				CTunerCmdInvoker::mTerminateSession = false;
                /* The Pandora App will Pause the Currently Playing Track and
				switch to Default Mobile App when Terminate Session is Called if
				application behaves differently then pause command need to be initiated
				explicitly
				*/
				/*if(!CPandoraMessageSenderImpl::mPandoraRequestQueue.empty())
				{
					DBG_MSG(("[IPandoraListenerImpl]: Size of the Queue Before Pop %d" , CPandoraMessageSenderImpl::mPandoraRequestQueue.size()));
					if(CTunerRequestResponseHandler::mPndrStartSessionReqRcvd == false)
					{
						CPandoraMessageSenderImpl::mPandoraRequestQueue.pop();
					}
					DBG_MSG(("[IPandoraListenerImpl]: Size of the Queue After Pop %d" , CPandoraMessageSenderImpl::mPandoraRequestQueue.size()));
				}*/
			}

			// pop the message from queue, received acknowledgment, remove the last successful send message
			if(!CPandoraMessageSenderImpl::mPandoraRequestQueue.empty())
			{
				//DBG_MSG(("[IPandoraListenerImpl]: Size of the Queue Before Pop %d" , CPandoraMessageSenderImpl::mPandoraRequestQueue.size()));
				CPandoraMessageSenderImpl::mPandoraRequestQueue.pop();
				//DBG_MSG(("[IPandoraListenerImpl]: Size of the Queue After Pop %d" , CPandoraMessageSenderImpl::mPandoraRequestQueue.size()));
			}
			else
			{
				DBG_MSG(("[IPandoraListenerImpl]: Queue is empty"));
			}

			//DBG_MSG(("[IPandoraListenerImpl]::CPandoraMessageSenderImpl::mSendInProgress = false"));
			CPandoraMessageSenderImpl::mSendInProgress = false;
			CPandoraMessageSenderImpl::mPandoraAckReceived = true;
			CPandoraMessageSenderImpl::sendData();
			break;
		}
		case Pandora_New_Message_Received:
		{
			//DBG_MSG(("[ThreadContext]::Pandora_New_Message_Received"));
			CPandoraMessageSenderImpl::sendData();
			break;
		}
		case Pandora_Timeout:
		{
			//CTunerRequestResponseHandler::mpInstance->profileDisconnectMethodCall();//Temp
			// Check whether Ack is Received or else resend the last Command
			//DBG_MSG(("[CPandoraRequestResponseHandler]::mPandoraAckReceived:%d, mRetryCounter:%d",CPandoraMessageSenderImpl::mPandoraAckReceived,CPandoraMessageSenderImpl::mRetryCounter));
			if((CPandoraMessageSenderImpl::mPandoraAckReceived != true) && (CPandoraMessageSenderImpl::mRetryCounter <= 8))

			{
				//DBG_MSG(("[CPandoraMessageSenderImpl]::handleTimerEvent::Ack Not Received"));

				if((IIpodNotificationImpl::mSessionId != -1))
				{
					++CPandoraMessageSenderImpl::mRetryCounter;
					//DBG_MSG(("[IPHONE/IPOD]::handleTimerEvent::RetryCounter %d",CPandoraMessageSenderImpl::mRetryCounter));
					//DBG_MSG(("[IPHONE/IPOD]::handleTimerEvent::sendData:mPandoraRequestQueue.front()"));
					CPandoraMessage* msg = CPandoraMessageSenderImpl::mPandoraRequestQueue.front();
					int size = msg->getDataSize();
					//DBG_MSG(("[IPHONE/IPOD]::handleTimerEvent::Iphone/IPOD Connected"));
					//DBG_MSG(("handleTimerEvent::Sending the packet %s", CPandoraMessage::sendCommandToString (msg->getCommand())));
					unsigned int ProtocolID = PROTOCOL_ID;
					if ( Iap1_Types::OK == vCIap1MgrController->requestIpodSendData(ProtocolID, (char*)msg->getData(), size))
					{
						DBG_MSG(("[IPHONE/IPOD]::RetryCounter %d : Sending the packet %s",CPandoraMessageSenderImpl::mRetryCounter, CPandoraMessage::sendCommandToString (msg->getCommand())));
					}
					else
					{
						DBG_MSG(("[IPHONE/IPOD]::Pandora_Timeout::Message written error"));
					}
				}
				else if(CTunerRequestResponseHandler::mAndroidDeviceConnected == true)
				{
					++CPandoraMessageSenderImpl::mRetryCounter;
					//DBG_MSG(("[ANDROID]::handleTimerEvent::RetryCounter %d",CPandoraMessageSenderImpl::mRetryCounter));
					//DBG_MSG(("[ANDROID]::handleTimerEvent::sendData:mPandoraRequestQueue.front()"));
					CPandoraMessage* msg = CPandoraMessageSenderImpl::mPandoraRequestQueue.front();
					//DBG_MSG(("[ANDROID]::handleTimerEvent::Andorid Connected and Sending the data bytes to Phone"));
					ssize_t vNumBytes = write(CTunerCmdInvoker::mPandoraDeviceFd, msg->getData(), msg->getDataSize());
					//DBG_MSG(("Sending the packet %s", CPandoraMessage::sendCommandToString (msg->getCommand())));
					if(vNumBytes > 0)
					{
						DBG_MSG(("[ANDROID]::RetryCounter %d : Sending the packet %s",CPandoraMessageSenderImpl::mRetryCounter,CPandoraMessage::sendCommandToString (msg->getCommand())));
					}
					else
					{
						DBG_MSG(("[ANDROID]::Pandora_Timeout::Message written error"));
					}
				}
				else
				{
					DBG_MSG(("[CPandoraMessageSenderImpl]::handleTimerEvent::Calling StopTimer No Device Connected"));
					CPandoraMessageSenderImpl::mRetryCounter = 0;
					mTimerInstance->stopTimer();
					CPandoraMessageSenderImpl::clearQueue();
					CPandoraMessageSenderImpl::mSendInProgress = false;
				}
			}
			if(CPandoraMessageSenderImpl::mRetryCounter == 9)
			{
				//Do nothing, give extra 750ms time for the ACK of 10th attempt
				++CPandoraMessageSenderImpl::mRetryCounter;
			}
			else if((CPandoraMessageSenderImpl::mRetryCounter >= 10))
			{
				DBG_MSG(("[CPandoraMessageSenderImpl]::handleTimerEvent::Calling StopTimer No Device connected"));
				CPandoraMessageSenderImpl::mRetryCounter = 0;
				mTimerInstance->stopTimer();
				CPandoraMessageSenderImpl::clearQueue();
				/////////////////////
				/*if((CTunerRequestResponseHandler::mAndroidDeviceConnected == true) && (CTunerCmdInvoker::meSessionState == eSessionActive))
				{
					CPandoraMessage *Nxtmsg = new CPandoraMessage;
					Nxtmsg->terminatePandoraSession();
					CPandoraMessageSenderImpl::addSendQueue(Nxtmsg);
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
					DBG_MSG(("[CTunerCmdInvoker]:AndroidDevice terminatePandoraSession bcoz mRetryCounter:%d",CPandoraMessageSenderImpl::mRetryCounter));
				}
				else if((CTunerRequestResponseHandler::mIphoneBluetooth == true) && (CTunerCmdInvoker::meSessionState == eSessionActive))
				{
					CPandoraMessage *Nxtmsg = new CPandoraMessage;
					Nxtmsg->terminatePandoraSession();
					CPandoraMessageSenderImpl::addSendQueue(Nxtmsg);
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
					DBG_MSG(("[CTunerCmdInvoker]:IphoneBluetooth terminatePandoraSession bcoz mRetryCounter:%d",CPandoraMessageSenderImpl::mRetryCounter));
				}
				else if((CTunerRequestResponseHandler::mIpodDeviceConnected == true))
				{
					if(IIpodNotificationImpl::mSessionId != -1)
					{
						CPandoraMessage *Nxtmsg = new CPandoraMessage;
						Nxtmsg->terminatePandoraSession();
						CPandoraMessageSenderImpl::addSendQueue(Nxtmsg);
						MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
						DBG_MSG(("[CTunerCmdInvoker]:IpodDevice terminatePandoraSession bcoz mRetryCounter:%d",CPandoraMessageSenderImpl::mRetryCounter));
					}
					else
					{
						DBG_MSG(("[CTunerCmdInvoker]:Rejected IpodDevice Terminate Session request"));
					}
				}*/
				/////////////////////
				vPandoraProtocolMgr->deInitialize();
				Json::Value data;
				//DBG_MSG(("[IPandoraListenerImpl]: Session Terminated: InforUpdateStationActive"));
				std::string signalName = "InfoUpdateStationActive";
				CTunerCmdInvoker::meSessionState = eSessionNone;
				DBG_MSG(("[IPandoraListenerImpl]:Session Terminated:CTunerCmdInvoker::meSessionState %d ", CTunerCmdInvoker::meSessionState));
				//CTunerCmdInvoker::mStationActive == false;
				data["StationToken"] = 0;
				data["value"] = false;
				CTunerRequestResponseHandler::handleNotif(signalName, data);
				CPandoraMessageSenderImpl::mSendInProgress = false;

				/*Json::Value data3;
				std::string signalName3 = "PandoraReadyState";
				data3["appStatus"] = PANDORA_READY_STATE;
				data3["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
				data3["value"] = false;
				CTunerRequestResponseHandler::handleNotif(signalName3, data3);*/

				//MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_Remove_Audio, 0);
				CTunerRequestResponseHandler::setPandoraAudioAutoRecovery = true;
				//CTunerRequestResponseHandler::mpInstance->PandoraSocketDisconnect();
				if( (CTunerRequestResponseHandler::mIpodDeviceConnected == true) && (IIpodNotificationImpl::mSessionId != -1) )
				{
					CTunerRequestResponseHandler::mIpodDeviceConnected = false;
					CTunerRequestResponseHandler::IpodPandoraPlayStatus = 0;
					if (Iap1_Types::OK == vCIap1MgrController->requestIpodStopDevCtrl())
					{
						IIpodNotificationImpl::mSessionId  = -1;
						Iap1_Types::tError mError =  vCIap1MgrController->requestIpodStopAudio();
						//DBG_MSG(("[CTunerRespHandler]:Bkup Stopping the IpodDevice Graph and return value is : %d",mError));
						vPandoraProtocolMgr->deInitialize();

						DBG_MSG(("[CTunerRespHandler]:Bkup CTunerCmdInvoker::meSessionState %d ", CTunerCmdInvoker::meSessionState));

						if(CTunerRequestResponseHandler::pandoraSessionConnectedDevice == PANDORA_SESSION_IPOD)
						{
							Json::Value data3;
							std::string signalName3 = "PandoraReadyState";
							data3["appStatus"] = PANDORA_READY_STATE;
							data3["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data3["value"] = false;
							CTunerRequestResponseHandler::handleNotif(signalName3, data3);
							DBG_MSG(("[PANDORA]:Bkup USB_IPHONE PandoraReadyState false Bkup"));
						}
					}
				}
				else
				{
					CTunerRequestResponseHandler::mpInstance->profileDisconnectMethodCall();
				}
			}
			else
			{
				DBG_MSG(("[ReqRespHandler]::handleTimerEvent::mRetryCounter value is %d", CPandoraMessageSenderImpl::mRetryCounter));
			}
			break;
		}
		case Pandora_Set_Audio:
		{
			if((CTunerRequestResponseHandler::mIpodDeviceConnected == true))
			{
				std::string mlistener = "/dev/snd/pcmC0D0p";
				Iap1_Types::tError mError = vCIap1MgrController->requestIpodStartAudioGraph(mlistener,Iap1_Types::USBDevice);
				DBG_MSG(("[ReqRespHandler]:Creating Audio graph for IphoneUSB and return value is %d", mError));
				Json::Value data;
				std::string signalName = "PandoraAudio";
				data["value"] = 1;
				CTunerRequestResponseHandler::handleNotif(signalName, data);
			}
			else if(CTunerRequestResponseHandler::mAndroidDeviceConnected == true )
			{
				int fh1 = open("/dev/io-media/graphs/trackplayer/default",O_RDONLY);
				int fh2 = open("/dev/io-media/graphs/mediafs_2wire/default",O_RDONLY);
				if( (fh1 > 0) || (fh2 > 0) )
				{
					DBG_MSG(("[ReqRespHandler]:Creating Audio graph for AndroidDevice Bluetooth"));
					/*if (true == mPandoraAudioTimer->isActive ())
					{
						mPandoraAudioTimer->stopTimer();
						DBG_MSG(("[CTunerCmdInvoker]: mPandoraAudioTimer isActive 2000"));
					}*/
					//mPandoraAudioTimer->stopTimer();
					//mPandoraAudioTimer->startTimer(2000, true);
					if(fh1){
						DBG_MSG(("[Android USB]:/dev/io-media/graphs/trackplayer/default exist fh1:%d",fh1));
						close(fh1);
					}
					if(fh2){
						DBG_MSG(("[Android BT]:/dev/io-media/graphs/mediafs_2wire/default exist fh2:%d",fh2));
						close(fh2);
					}
					Json::Value data;
					std::string signalName = "PandoraAudio";
					data["value"] = 0;
					CTunerRequestResponseHandler::handleNotif(signalName, data);
				}
				else
				{
					DBG_MSG(("[ReqRespHandler]:Creating Audio graph for AndroidDevice Bluetooth"));
					CTunerCmdInvoker::initPandoraAudioGraph();
					Json::Value data;
					std::string signalName = "PandoraAudio";
					data["value"] = 1;
					CTunerRequestResponseHandler::handleNotif(signalName, data);
				}
			}
			else if((CTunerRequestResponseHandler::mIphoneBluetooth == true))
			{
				int fh1 = open("/dev/io-media/graphs/trackplayer/default",O_RDONLY);
				int fh2 = open("/dev/io-media/graphs/mediafs_2wire/default",O_RDONLY);
				if( (fh1 > 0) || (fh2 > 0) )
				{
					DBG_MSG(("[CTunerCmdInvoker]:Creating Audio graph for IPhone Bluetooth"));
					/*if (true == mPandoraAudioTimer->isActive ())
					{
						mPandoraAudioTimer->stopTimer();
						DBG_MSG(("[CTunerCmdInvoker]: mPandoraAudioTimer isActive 2000"));
					}*/
					//mPandoraAudioTimer->stopTimer();
					//mPandoraAudioTimer->startTimer(2000, true);
					if(fh1){
						DBG_MSG(("[IPhone USB]:/dev/io-media/graphs/trackplayer/default exist fh1:%d",fh1));
						close(fh1);
					}
					if(fh2){
						DBG_MSG(("[IPhone BT]:/dev/io-media/graphs/mediafs_2wire/default exist fh2:%d",fh2));
						close(fh2);
					}
					Json::Value data;
					std::string signalName = "PandoraAudio";
					data["value"] = 0;
					CTunerRequestResponseHandler::handleNotif(signalName, data);
				}
				else
				{
					DBG_MSG(("[ReqRespHandler]:Creating Audio graph for IphoneBluetooth"));
					CTunerCmdInvoker::initPandoraAudioGraph();
					Json::Value data;
					std::string signalName = "PandoraAudio";
					data["value"] = 1;
					CTunerRequestResponseHandler::handleNotif(signalName, data);
				}
			}
			break;
		}
		case Pandora_Remove_Audio:
		{
			if((CTunerRequestResponseHandler::mIpodDeviceConnected == true))
			{
				//DBG_MSG(("[CTunerCmdInvoker]:Stopping the IpodDevice Graph"));
				Iap1_Types::tError mError =  vCIap1MgrController->requestIpodStopAudio();
				DBG_MSG(("[CTunerCmdInvoker]:Stopping the IpodDevice Graph and return value is : %d",mError));
				Json::Value data;
				std::string signalName = "PandoraAudio";
				data["value"] = 0;
				CTunerRequestResponseHandler::handleNotif(signalName, data);
			}
			else if(CTunerRequestResponseHandler::mAndroidDeviceConnected == true )
			{
				DBG_MSG(("[CTunerCmdInvoker]:Remove Audio graph for Bluetooth : mGraphid %d",CTunerCmdInvoker::mGraphid));
				if(CTunerCmdInvoker::mGraphid != -1)
				{
					int rs =  iom_stop(CTunerCmdInvoker::mGraphid);
					//DBG_MSG(("[CTunerCmdInvoker]:Graph Stop : return value : %d", rs));
					rs = iom_destroy_graph(CTunerCmdInvoker::mGraphid);
					//DBG_MSG(("[CTunerCmdInvoker]:Graph destroyed : return value : %d", rs));
					if(rs == 0){
						CTunerCmdInvoker::mGraphid = -1;
					}
				}
				Json::Value data;
				std::string signalName = "PandoraAudio";
				data["value"] = 0;
				CTunerRequestResponseHandler::handleNotif(signalName, data);
			}
			else if((CTunerRequestResponseHandler::mIphoneBluetooth == true))
			{
				DBG_MSG(("[CTunerCmdInvoker]:BeforeGraph IphoneBluetooth Stop : mGraphid : %d", CTunerCmdInvoker::mGraphid));
				//Iap1_Types::tError mError =  vCIap1MgrController->requestIpodStopAudio();
				//DBG_MSG(("[CTunerCmdInvoker]:Stopping the IphoneBluetooth Graph and return value is : %d",mError));
				if(CTunerCmdInvoker::mGraphid != -1)
				{
					int rs =  iom_stop(CTunerCmdInvoker::mGraphid);
					//DBG_MSG(("[CTunerCmdInvoker]:IphoneBluetooth Graph Stop : return value : %d", rs));
					rs = iom_destroy_graph(CTunerCmdInvoker::mGraphid);
					//DBG_MSG(("[CTunerCmdInvoker]:IphoneBluetooth Graph destroyed : return value : %d", rs));
					if(rs == 0){
						CTunerCmdInvoker::mGraphid = -1;
					}
				}
				Json::Value data;
				std::string signalName = "PandoraAudio";
				data["value"] = 0;
				CTunerRequestResponseHandler::handleNotif(signalName, data);
			}
			break;
		}
		case Pandora_Start_Timer:
		{
			//DBG_MSG(("[CTunerCmdInvoker]:Starting the Timer"));
			break;
		}
		case Pandora_Stop_Timer:
		{
			//DBG_MSG(("[CTunerCmdInvoker]:Stoping the Timer"));
			break;
		}
		case Pandora_Device_Check_Timer:
		{
			//mPandoraDeviceCheck->stopTimer();
			DBG_MSG(("[CTunerCmdInvoker]:Pandora_Device_Check_Timer"));
			CTunerRequestResponseHandler::mpInstance->afterPandoraDisconnectChecking();
			break;
		}
		case Pandora_Audio_Graph_Timer:
		{
			//mPandoraAudioTimer->stopTimer();
			DBG_MSG(("[CTunerCmdInvoker]:Pandora_Audio_Graph_Timer"));
			CTunerRequestResponseHandler::mpInstance->initPandoraAudioGraphAgain();
			break;
		}
		default:
			break;
		}

		//printf( "nto_srvr: Received pulse ID: %d, code: %d, value: %d\n",recv_id, recv_msg.code, recv_msg.value.sival_int );
	} /* while true */

	/* We'll never get here, but the OS will destroy the channel automatically
	 * when the process is terminated (by a CTRL-C for instance) anyway. */
	ChannelDestroy( CTunerRequestResponseHandler::channelId );
		}

void CTunerRequestResponseHandler::processSvcIpcNameChange( const _Uint32t & nameChgRegHndl, const CSvcIpcNameChange* pNameChange, void* pUserData )
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, processSvcIpcNameChange);
	// DBG_MSG(("******newName: %s, oldOwner: %s, newOwner: %s******", pNameChange->m_newName, pNameChange->m_oldOwner, pNameChange->m_newOwner));


}



int CTunerRequestResponseHandler::createTunerAudioProxyThread(void)
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, createTunerAudioProxyThread);

	return 0;
}
//===============================================================================================



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
	//TRC_SCOPE(TunerService, CTunerRequestResponseHandler, processSvcIpcRequest);
	//DBG_MSG(("CTunerRequestResponseHandler: DBUS request received onTunerRequest"));
	printf("CTunerRequestResponseHandler: DBUS request received onTunerRequest \n");
	CTunerReqData* request;
	request = new CTunerReqData();
	request->mMethod = pRequest->getMethod();
	//request->mContext  = pRequest->getContext();
	request->mUserToken = pRequest->getUserToken();
	request->mNoReplyExpected = pRequest->isReplyExpected();

	//Push the request to Queue
	CTunerResHandler* obj = CTunerResHandler::GetInstance();
	(obj->tunerRequestQueue).push_back(request);
	//Get the instance and propagate the request to Invoker
	CTunerCmdInvoker *cmdInvoker = CTunerCmdInvoker::GetInstance();
	cmdInvoker->handleTunerRequest(pRequest);

}

int CTunerRequestResponseHandler::getPandoraDeviceFd(void)
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, getPandoraDeviceFd);
	int fd = -1;
	char *mount_path = (char *)malloc(100);
	memset (mount_path, 0x00, 100);
	sprintf(mount_path,"%s","/dev/SPP_PANDORA");
	fd = open( mount_path,O_RDWR);
	////////////
	if(fd == -1)
	{
		DBG_MSG(("[getPandoraDeviceFd]file open is failed and errno value is %d",errno));
		return fd;
	}

	int retval;
	//flags = fcntl( STDOUT_FILENO, F_GETFL );
	retval = fcntl(fd, F_SETFL, O_NONBLOCK);
	if( retval == -1 ) {
		DBG_MSG(("error setting mPandoraDeviceFd flags\n"));
	}
	//////////
	return fd;
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
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, processSvcIpcSignal);
	DBG_MSG(("PandoraService::processSvcIpcSignal, sigRegHndl= %d, mBluetoothServiceConnected=%d",sigRegHndl, mBluetoothServiceConnected));
	Json::Reader jReader;
	Json::Value jValue;
	BOOL bSuccess;

	/*int NAMELEN = 30;
    int rc;
    char thread_name[NAMELEN];
    rc = pthread_getname_np(pthread_self(), thread_name, NAMELEN);
	if (rc != 0)
		DBG_MSG(("pthread_getname_np"));
	DBG_MSG(("processSvcIpcSignal thread. name is: %s\n", thread_name));*/

	if(mBluetoothServiceConnected == sigRegHndl)
	{
		DBG_MSG(("BlueToothService Connected Received"));
		bSuccess = jReader.parse(pSignal->m_sigData, jValue);
		if ((bSuccess) && (jValue.isObject()))
		{
			std::string sSPP_Pandora = jValue.get("service", "").asString();
			if(sSPP_Pandora.compare("SPP_PANDORA") == 0)
			{
				CTunerRequestResponseHandler::mIgnoreTerminateSessionCmd = true;
				CTunerRequestResponseHandler::PandoraDisconnectCheckingFlag = true;
				CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_ANDROID = true;
				DBG_MSG(("Android Device is connected which has Pandora Application"));
				if( (CTunerRequestResponseHandler::mIpodDeviceConnected == false) && (CTunerRequestResponseHandler::mIphoneBluetooth == false) )
				{
					CTunerRequestResponseHandler::pandoraConnectedDeviceName = PANDORA_DEVICE_ANDROID;
				}
					CTunerCmdInvoker::mPandoraDeviceFd  = getPandoraDeviceFd();
					if(CTunerCmdInvoker::mPandoraDeviceFd == -1)
					{
						DBG_MSG(("file open is failed and errno value is %d",errno));
						return;
					}
					SPP_Pandora_Connected_DeviceAddress = jValue.get("address", "").asString();

					//if( (CTunerRequestResponseHandler::mIpodDeviceConnected == false) && (CTunerRequestResponseHandler::mIphoneBluetooth == false) )
					//{
						CTunerRequestResponseHandler::mAndroidDeviceConnected = true;
						DBG_MSG(("mAndroidDeviceConnected:%d", CTunerRequestResponseHandler::mAndroidDeviceConnected));
					//}
					DBG_MSG(("File descriptor is opened and value is %d ,DeviceAddress:%s", CTunerCmdInvoker::mPandoraDeviceFd,SPP_Pandora_Connected_DeviceAddress.c_str()));

					pthread_attr_t threadAttr;
					sched_param_t  schedParam;
					// create thread attributes
					if( pthread_attr_init(&threadAttr) != EOK )
					{
						return;
					}
					if( pthread_attr_getschedparam(&threadAttr,&schedParam) != EOK )
					{
						return;
					}
					schedParam.sched_priority = 11;
					if( pthread_attr_setschedparam(&threadAttr,&schedParam) != EOK )
					{
						return;
					}
					if( pthread_attr_setinheritsched(&threadAttr,PTHREAD_EXPLICIT_SCHED) != EOK )
					{
						return;
					}

					if( pthread_attr_setschedpolicy(&threadAttr,SCHED_RR) != EOK )
					{
						return;
					}

					if( pthread_create(&mPandoraReaderThreadProxy, &threadAttr, runPandoraReaderThread, (void*)vPandoraProtocolMgr) != EOK)
					{
						DBG_MSG(("failed to create thread"));
						return;
					}

					(void) pthread_attr_destroy(&threadAttr);
					Json::Value data;
					std::string signalName = "PandoraReadyState";
					data["appStatus"] = PANDORA_READY_STATE;
					data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
					data["value"] = true;
					CTunerRequestResponseHandler::handleNotif(signalName, data);
					DBG_MSG(("[PANDORA]:SPP_PANDORA PandoraReadyState true"));
				//}
			}
			else if(sSPP_Pandora.compare("SPP_IAP_Iphone") == 0)
			{
				DBG_MSG(("IPhone device is connected which has Pandora Applications mIpodDeviceConnected:%d",CTunerRequestResponseHandler::mIpodDeviceConnected));
				CTunerRequestResponseHandler::mIgnoreTerminateSessionCmd = true;
				if( (CTunerRequestResponseHandler::mIpodDeviceConnected == false) && (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) )
				{
					CTunerRequestResponseHandler::PandoraDisconnectCheckingFlag = true;
					CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPHONE = true;
					CTunerRequestResponseHandler::pandoraConnectedDeviceName = PANDORA_DEVICE_IPHONE;
					//////////////////////
					/*FILE *fp;
					const int maxLength_iofs = 512;
					char command_iofs[maxLength_iofs] = {0};
					snprintf(command_iofs, maxLength_iofs, "/bin/io-fs-media -dipod,acp=acp:path=/dev/i2c/i2c_1/acp,fnames=short,transport=ser:nosyncbyte:dev=/dev/SPP_IAP_Iphone,mount=/fs/btipod0,playback,rtimeout=2000,polltimeout=3000,verbose=1,logfile=/dev/console,config=/etc/ipod.cfg &");
					fp = popen(command_iofs, "r");
					if (fp == NULL)
					{
						DBG_MSG(("\n Failed to run io-fs-media\n"));
					}
					else
					{
						pclose(fp);
						DBG_MSG(("\n run io-fs-media Success\n"));
					}*/
					//////////////////////
					char BTIPOD0_Cntr = 0;
					while(!fopen("/fs/btipod0", "rb"))
					{
						DBG_MSG(("Waiting till btipod0 mountpoint is created"));
						sleep(1);
						BTIPOD0_Cntr++;
						if ( BTIPOD0_Cntr >= 20){
								break;	 		}
					}
					if (Iap1_Types::OK == vCIap1MgrController->requestIpodStartDevCtrl("/fs/btipod0"))
					{
						SPP_IAP_Iphone_Connected_DeviceAddress = jValue.get("address", "").asString();
						CTunerRequestResponseHandler::mIphoneBluetooth = true;
						CTunerRequestResponseHandler::mIphoneStartDevCtl = true;
						//CTunerRequestResponseHandler::mIpodDeviceConnected = false;
						DBG_MSG(("requestIpodStartDevCtrl: Success mIphoneBluetooth = %d,mIphoneStartDevCtl=%d",CTunerRequestResponseHandler::mIphoneBluetooth,CTunerRequestResponseHandler::mIphoneStartDevCtl));

						//if(CTunerRequestResponseHandler::mIpodDeviceConnected == false)
						//{
							Json::Value data;
							std::string signalName = "PandoraReadyState";
							data["appStatus"] = PANDORA_READY_STATE;
							data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data["value"] = true;
							CTunerRequestResponseHandler::handleNotif(signalName, data);
							DBG_MSG(("[PANDORA]:SPP_IAP_Iphone PandoraReadyState true"));
						//}
					}
					else
					{
						DBG_MSG(("requestIpodStartDevCtrl: Failed"));
					}
				}
			}
		}
	}
	else if(mBluetoothServiceDisConnected == sigRegHndl)
	{
		DBG_MSG(("Bluetooth is disconnected"));
		CTunerCmdInvoker::mPandoraDeviceFd = -1;
		bSuccess = jReader.parse(pSignal->m_sigData, jValue);
		if ((bSuccess) && (jValue.isObject()))
		{
			std::string sSPP_Pandora = jValue.get("service", "").asString();
			if(sSPP_Pandora.compare("SPP_PANDORA") == 0)
			{
				CTunerRequestResponseHandler::mIgnoreTerminateSessionCmd = false;
				CTunerRequestResponseHandler::PandoraDisconnectCheckingFlag = true;
				CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_ANDROID = false;
				if( (CTunerRequestResponseHandler::mAndroidDeviceConnected == true) && (CTunerRequestResponseHandler::mIphoneBluetooth == false)
																			&& (CTunerRequestResponseHandler::mIpodDeviceConnected == false) )
				{
					//SPP_Pandora_Connected_DeviceAddress = "NULL";
					CTunerRequestResponseHandler::mAndroidDeviceConnected = false;
					////////////////////
					if(CTunerCmdInvoker::mGraphid != -1)
					{
						int rs =  iom_stop(CTunerCmdInvoker::mGraphid);
						DBG_MSG(("[PANDORA]:Graph Stop initPandoraAudioGraph: return value : %d", rs));
						rs = iom_destroy_graph(CTunerCmdInvoker::mGraphid);
						DBG_MSG(("[PANDORA]:Graph destroyed initPandoraAudioGraph: return value : %d", rs));
						if(rs == 0){
							CTunerCmdInvoker::mGraphid = -1;
						}
						Json::Value data;
						std::string signalName = "PandoraAudio";
						data["value"] = 0;
						CTunerRequestResponseHandler::handleNotif(signalName, data);
					}
					////////////////////
						Json::Value data;
						DBG_MSG(("[IPandoraListenerImpl]: Session Terminated: InforUpdateStationActive"));
						std::string signalName = "InfoUpdateStationActive";
						CTunerCmdInvoker::meSessionState = eSessionNone;
						DBG_MSG(("[IPandoraListenerImpl]:CTunerCmdInvoker::meSessionState %d ", CTunerCmdInvoker::meSessionState));
						//CTunerCmdInvoker::mStationActive == false;
						data["StationToken"] = 0;
						data["value"] = false;
						CTunerRequestResponseHandler::handleNotif(signalName, data);

					// pthread_join(mPandoraReaderThreadProxy, NULL);
					DBG_MSG(("Android Device is disconnected which has Pandora App"));

					std::string sDisconnect_Reason = jValue.get("reason", "").asString();
					if(sDisconnect_Reason.compare("remote_disconnect") == 0)
					{
						Json::Value dataPAS;
						std::string signalNamePAS = "PandoraReadyState";
						dataPAS["appStatus"] = PANDORA_APP_STATE;
						dataPAS["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
						dataPAS["value"] = false;
						CTunerRequestResponseHandler::handleNotif(signalNamePAS, dataPAS);
						DBG_MSG(("[PANDORA]:SPP_PANDORA Pandora App State false"));
						/*if( (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) && (CTunerRequestResponseHandler::mIphoneBluetooth == false)
							&& (CTunerRequestResponseHandler::mIpodDeviceConnected == false) )
						{
							if (true == mPandoraDeviceCheck->isActive ())
							{
								mPandoraDeviceCheck->stopTimer();
								DBG_MSG(("[PANDORA]:SPP_PANDORA mPandoraDeviceCheck isActive"));
							}
							mPandoraDeviceCheck->startTimer(2000, true);
						}*/
					}
					else
					{
						Json::Value data2;
						std::string signalName2 = "PandoraReadyState";
						data2["appStatus"] = PANDORA_READY_STATE;
						data2["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
						data2["value"] = false;
						CTunerRequestResponseHandler::handleNotif(signalName2, data2);
						DBG_MSG(("[PANDORA]:SPP_PANDORA PandoraReadyState false"));
					}
					vPandoraProtocolMgr->deInitialize();
					//afterPandoraDisconnectChecking();
					//mPandoraDeviceCheck->startTimer(4000, true);
					if(CTunerRequestResponseHandler::SPP_PANDORA_SERVICE_DISCONNECT_INITIATED == true)
					{
						//CTunerRequestResponseHandler::SPP_PANDORA_SERVICE_DISCONNECT_INITIATED = false;
						profileConnectMethodCall();
					}
				}
			}
			else if(sSPP_Pandora.compare("SPP_IAP_Iphone") == 0)
			{
				CTunerRequestResponseHandler::mIgnoreTerminateSessionCmd = false;
				//CTunerRequestResponseHandler::mIphoneBluetooth = false;
				CTunerRequestResponseHandler::PandoraDisconnectCheckingFlag = true;
				CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPHONE = false;
				DBG_MSG(("iPhone Device SPP_IAP_Iphone disconnecting mIphoneStartDevCtl = %d",CTunerRequestResponseHandler::mIphoneStartDevCtl));
				//if(CTunerRequestResponseHandler::mIphoneBluetooth == true)
				if( (CTunerRequestResponseHandler::mIphoneBluetooth == true) && (CTunerRequestResponseHandler::mAndroidDeviceConnected == false)
																			&& (CTunerRequestResponseHandler::mIpodDeviceConnected == false) )
				{
					if(CTunerRequestResponseHandler::mIphoneStartDevCtl == true)
					{
						CTunerRequestResponseHandler::mIphoneStartDevCtl = false;
						//////////////////////
						/*FILE *fp;
						const int maxLength_iofs = 256;
						char command_iofs[maxLength_iofs] = {0};
						snprintf(command_iofs, maxLength_iofs, "slay -skill io-fs-media &");
						fp = popen(command_iofs, "r");
						if (fp == NULL)
						{
							DBG_MSG(("\n Failed to slay io-fs-media\n"));
						}
						else
						{
							pclose(fp);
							DBG_MSG(("\n slay io-fs-media Success\n"));
						}*/
						//////////////////////
						if (Iap1_Types::OK == vCIap1MgrController->requestIpodStopDevCtrl())
						{
							IIpodNotificationImpl::mSessionId  = -1;
							//SPP_IAP_Iphone_Connected_DeviceAddress = "NULL";
							CTunerRequestResponseHandler::mIphoneBluetooth = false;
							DBG_MSG(("requestIpodStopDevCtrl: Success"));

							if(CTunerCmdInvoker::mGraphid != -1)
							{
								int rs =  iom_stop(CTunerCmdInvoker::mGraphid);
								DBG_MSG(("[PANDORA]:IPhone Graph Stop initPandoraAudioGraph: return value : %d", rs));
								rs = iom_destroy_graph(CTunerCmdInvoker::mGraphid);
								DBG_MSG(("[PANDORA]:IPhone Graph destroyed initPandoraAudioGraph: return value : %d", rs));
								if(rs == 0){
									CTunerCmdInvoker::mGraphid = -1;
								}
								Json::Value data;
								std::string signalName = "PandoraAudio";
								data["value"] = 0;
								CTunerRequestResponseHandler::handleNotif(signalName, data);
							}
							Json::Value dataIUSA;
							std::string signalNameIUSA = "InfoUpdateStationActive";
							CTunerCmdInvoker::meSessionState = eSessionNone;
							DBG_MSG(("[IPandoraListenerImpl]:CTunerCmdInvoker::meSessionState %d ", CTunerCmdInvoker::meSessionState));
							//CTunerCmdInvoker::mStationActive == false;
							dataIUSA["StationToken"] = 0;
							dataIUSA["value"] = false;
							CTunerRequestResponseHandler::handleNotif(signalNameIUSA, dataIUSA);

							DBG_MSG(("iPhone Device is disconnected which has Pandora App"));

							std::string sDisconnect_Reason = jValue.get("reason", "").asString();
							if(sDisconnect_Reason.compare("remote_disconnect") == 0)
							{
								Json::Value dataPAS;
								std::string signalNamePAS = "PandoraReadyState";
								dataPAS["appStatus"] = PANDORA_APP_STATE;
								dataPAS["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
								dataPAS["value"] = false;
								CTunerRequestResponseHandler::handleNotif(signalNamePAS, dataPAS);
								DBG_MSG(("[PANDORA]:SPP_IAP_Iphone Pandora App State false"));

								/*if( (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) && (CTunerRequestResponseHandler::mIphoneBluetooth == false)
									&& (CTunerRequestResponseHandler::mIpodDeviceConnected == false) )
								{
									if (true == mPandoraDeviceCheck->isActive ())
									{
										mPandoraDeviceCheck->stopTimer();
										DBG_MSG(("[PANDORA]:SPP_IAP_Iphone mPandoraDeviceCheck isActive"));
									}
									mPandoraDeviceCheck->startTimer(2000, true);
								}*/
							}
							else
							{
								Json::Value data;
								std::string signalName = "PandoraReadyState";
								data["appStatus"] = PANDORA_READY_STATE;
								data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
								data["value"] = false;
								CTunerRequestResponseHandler::handleNotif(signalName, data);
								DBG_MSG(("[PANDORA]:SPP_IAP_Iphone PandoraReadyState false"));
							}
							//afterPandoraDisconnectChecking();
							//mPandoraDeviceCheck->startTimer(4000, true);
							if(CTunerRequestResponseHandler::SPP_IAP_Iphone_SERVICE_DISCONNECT_INITIATED == true)
							{
								//CTunerRequestResponseHandler::SPP_IAP_Iphone_SERVICE_DISCONNECT_INITIATED = false;
								profileConnectMethodCall();
							}
						}
						else
						{
							DBG_MSG(("requestIpodStopDevCtrl: Failed"));
						}
						if(CTunerRequestResponseHandler::mIgnoreBtDisconnection == true)
						{
							CTunerRequestResponseHandler::mIgnoreBtDisconnection = false;
							afterIgnoreBtDisconnectionSignal();
						}
					}
				}
				else if(CTunerRequestResponseHandler::mIpodDeviceConnected == true)
				{
					if(CTunerRequestResponseHandler::mIgnoreBtDisconnection == true)
					{
						CTunerRequestResponseHandler::mIgnoreBtDisconnection = false;
						afterIgnoreBtDisconnectionSignal();
					}
				}
			}
		}
	}

	if( mMediaDeviceState == sigRegHndl)
	{
		DBG_MSG(("Media Device State  Received"));
		bSuccess = jReader.parse(pSignal->m_sigData, jValue);

		if ((bSuccess) && (jValue.isObject()))
		{
			bool sMediaDeviceState = jValue.get("available", false).asBool();
			if( sMediaDeviceState == true )
			{
				Json::Value mDeviceObj = jValue.get("device","{}");
				std::string mMountPoint = mDeviceObj.get("mountPoint", "").asString();
				if(mMountPoint.compare("/fs/avrcp00") == 0)
				{
					if( (CTunerRequestResponseHandler::mAndroidDeviceConnected == true) || (CTunerRequestResponseHandler::mIphoneBluetooth == true) )
					{
						/*if(CTunerRequestResponseHandler::powerModePandoraReadyFalse)
						{
							CTunerRequestResponseHandler::powerModePandoraReadyFalse = 0;
							if(fopen("/fs/avrcp00/.FS_info./audio", "rb")){
								DBG_MSG(("[PANDORA]:/fs/avrcp00/.FS_info./audio is available"));
							}
							Json::Value data;
							std::string signalName = "PandoraReadyState";
							data["appStatus"] = PANDORA_READY_STATE;
							data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data["value"] = true;
							CTunerRequestResponseHandler::handleNotif(signalName, data);
							DBG_MSG(("[PANDORA]:/fs/avrcp00 PandoraReadyState true"));
						}*/
						DBG_MSG(("Media Device State /fs/avrcp00 Available"));
						if(CTunerCmdInvoker::setPandoraAudioAgainFlagChkOnCmd == true)
						{
							if(CTunerCmdInvoker::setPandoraAudioAgainFlag == true)
							{
								DBG_MSG(("[CTunerCmdInvoker] :/fs/avrcp00 available setPandoraAudioAgainFlag: %d", CTunerCmdInvoker::setPandoraAudioAgainFlag));
								CTunerCmdInvoker::setPandoraAudioAgainFlag = false;
								MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_Set_Audio, 0);
							}
						}
					}
				}
				/*std::string mTypeDevice = mDeviceObj.get("type", "").asString();
				if(mTypeDevice.compare("ipod") == 0 )
				{
					DBG_MSG(("Media Device Type is ipod"));
					std::string mMountPoint = mDeviceObj.get("mountPoint", "").asString();
					if(mMountPoint.compare("/fs/ipod0") == 0)
					{

					}
				}*/
			}
			else if( sMediaDeviceState == false )
			{
				Json::Value mDeviceObj = jValue.get("device","{}");
				std::string mMountPoint = mDeviceObj.get("mountPoint", "").asString();
				if(mMountPoint.compare("/fs/avrcp00") == 0)
				{
					DBG_MSG(("Media Device State /fs/avrcp00 Not Available"));
					CTunerCmdInvoker::setPandoraAudioAgainFlag = true;
				}
			}
			else
			{
				// DBG_MSG(("Media Device is disconnected"));
				Json::Value mDeviceObj = jValue.get("device","{}");
				std::string mTypeDevice = mDeviceObj.get("type", "").asString();
				if(mTypeDevice.compare("ipod") == 0 )
				{

				}
			}
		}
	}

	if( mPowerModeSignalRegHandle == sigRegHndl)
	{
		DBG_MSG(("Power Mode Signal  Received"));
		bSuccess = jReader.parse(pSignal->m_sigData, jValue);

		if ((bSuccess) && (jValue.isObject()))
		{
			int sPowerMode = jValue.get("value", false).asInt();
			CTunerRequestResponseHandler::mPowerMode = sPowerMode;
			if( sPowerMode == PM_STANDBY_MODE )
			{
				if( (CTunerRequestResponseHandler::mAndroidDeviceConnected == true) || (CTunerRequestResponseHandler::mIphoneBluetooth == true) || (CTunerRequestResponseHandler::mIpodDeviceConnected == true) )
				{
					CTunerRequestResponseHandler::powerModePandoraReadyFalse = 1;
					/*Json::Value data;
					std::string signalName = "PandoraReadyState";
					data["appStatus"] = PANDORA_READY_STATE;
					data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
					data["value"] = false;
					CTunerRequestResponseHandler::handleNotif(signalName, data);*/
					DBG_MSG(("[PANDORA]:PowerMode:%d PandoraReadyState false",sPowerMode));
					//MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_Remove_Audio, 0);
				}
			}
			else if( sPowerMode == PM_FULLYWORKING_MODE )
			{
				/*if( (CTunerRequestResponseHandler::mAndroidDeviceConnected == true) || (CTunerRequestResponseHandler::mIphoneBluetooth == true) )
				{
					if(CTunerRequestResponseHandler::powerModePandoraReadyFalse)
					{
						CTunerRequestResponseHandler::powerModePandoraReadyFalse = 0;
						int a = 0;
						while(!fopen("/fs/avrcp00/.FS_info./audio", "rb"))
						{
							sleep(1);
							a++;
							if( a > 15) {
								DBG_MSG(("Waited a:%d sec /fs/avrcp00/.FS_info./audio mountpoint not created",a));
								break;
							}
							DBG_MSG(("Waiting a:%d sec /fs/avrcp00/.FS_info./audio mountpoint is created",a));
						}
						Json::Value data;
						std::string signalName = "PandoraReadyState";
						data["appStatus"] = PANDORA_READY_STATE;
						data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
						data["value"] = true;
						CTunerRequestResponseHandler::handleNotif(signalName, data);
						DBG_MSG(("[PANDORA]:PowerMode:%d PandoraReadyState true",sPowerMode));
					}
				}
				else*/ if(CTunerRequestResponseHandler::mIpodDeviceConnected == true)
				{
					if(CTunerRequestResponseHandler::powerModePandoraReadyFalse)
					{
						CTunerRequestResponseHandler::powerModePandoraReadyFalse = 0;
						Json::Value data;
						std::string signalName = "PandoraReadyState";
						data["appStatus"] = PANDORA_READY_STATE;
						data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
						data["value"] = true;
						CTunerRequestResponseHandler::handleNotif(signalName, data);
						DBG_MSG(("[PANDORA]:IPOD PowerMode:%d PandoraReadyState true",sPowerMode));
					}
				}
			}
			DBG_MSG(("Power Mode Signal  Received sPowerMode:%d",sPowerMode));
		}
	}
	if(mSPP_PANDORAserviceConnectError == sigRegHndl)
	{
		if( (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) && (CTunerRequestResponseHandler::mIpodDeviceConnected == false) && (CTunerRequestResponseHandler::mIphoneBluetooth == false) )
		{
			bSuccess = jReader.parse(pSignal->m_sigData, jValue);
			if ((bSuccess) && (jValue.isObject()))
			{
				std::string sSPP_Pandora = jValue.get("service", "").asString();
				if(sSPP_Pandora.compare("SPP_PANDORA") == 0)
				{
					SPP_Pandora_DeviceAddress = jValue.get("address", "").asString();
				}
				else if(sSPP_Pandora.compare("SPP_IAP_Iphone") == 0)
				{
					SPP_IAP_Iphone_DeviceAddress = jValue.get("address", "").asString();
					if (SPP_IAP_Iphone_DeviceAddress.compare(SPP_Pandora_DeviceAddress) == 0)
					{
						Json::Value dataAppAvl;
						std::string signalAppAvl = "PandoraReadyState";
						dataAppAvl["appStatus"] = PANDORA_APP_AVAILABLE;
						dataAppAvl["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
						dataAppAvl["value"] = false;
						CTunerRequestResponseHandler::handleNotif(signalAppAvl, dataAppAvl);
						DBG_MSG(("[PANDORA]:PandoraAppAvailable false:SPP_Pandora_DeviceAddress=%s,SPP_IAP_Iphone_DeviceAddress=%s",SPP_Pandora_DeviceAddress.c_str(),SPP_IAP_Iphone_DeviceAddress.c_str()));

						SPP_Pandora_DeviceAddress = "NULL";
						SPP_IAP_Iphone_DeviceAddress = "NULL";
					}
				}
			}
		}
	}
	if(mIgnoreBtDisconnectionSignal == sigRegHndl)
	{
		//if( (CTunerRequestResponseHandler::mAndroidDeviceConnected == true) || (CTunerRequestResponseHandler::mIphoneBluetooth == true) )
		//{
			CTunerRequestResponseHandler::mIgnoreBtDisconnection = true;
			DBG_MSG(("mIgnoreBtDisconnectionSignal Received mIgnoreBtDisconnection:%d",CTunerRequestResponseHandler::mIgnoreBtDisconnection));
		//}
	}

	DBG_MSG(("Inside Dbus call back"));

}

void CTunerRequestResponseHandler::processSvcIpcError( const _Uint32t & resultRegHndl, SVCIPC_tErrorCode error, SVCIPC_tConstStr errMsg, SVCIPC_tConstStr errName )
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, processSvcIpcError);
	DBG_WARNING(("processSvcIpcError with error message as %s & error name is %s ************",errMsg,errName));

}

void CTunerRequestResponseHandler::processSvcIpcResult( const _Uint32t & resultRegHndl, SVCIPC_tConstStr result, void* pUserData )
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, processSvcIpcResult);
	DBG_MSG(("Tuner processSvcIpcResult handle in CTunerRequestResponseHandler : %d",resultRegHndl));
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

bool CTunerRequestResponseHandler::Initialize()
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, Initialize);

	DBG_MSG(("CTunerRequestResponseHandler::Initialize"));
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_protocols, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_appLaunchAllowed, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_capabilities, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_currentSession, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_dataAvailable, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_dataList, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_deviceState, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_mountPath, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_responseAcceptSession, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_responseCloseSession, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_responseGetNowPlayingApp, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_responseLaunchApplication, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_responseReceiveData, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_responseSendData, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_sessions, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_updateAppName, vIIPodNotification);
	vCIap1MgrController->registerNotification(Iap1_Types::UPD_ID_updateBundleName, vIIPodNotification);

	vCIap1MgrController->disableAlternateIpodAudiocfg(true);
	CPandoraMessageSenderImpl::clearQueue();
	createPandoraWorkerThread();
	setupConnection();
	startMonitoring();
}





int CTunerRequestResponseHandler::createPandoraWorkerThread()
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, createPandoraWorkerThread);
	DBG_MSG(("CTunerRequestResponseHandler::createPandoraWorkerThread"));
	pthread_attr_t threadAttr;
	sched_param_t  schedParam;
	// create thread attributes
	if( pthread_attr_init(&threadAttr) != EOK )
	{
		return 0;
	}
	if( pthread_attr_getschedparam(&threadAttr,&schedParam) != EOK )
	{
		return 0;
	}
	schedParam.sched_priority = 11;
	if( pthread_attr_setschedparam(&threadAttr,&schedParam) != EOK )
	{
		return 0;
	}
	if( pthread_attr_setinheritsched(&threadAttr,PTHREAD_EXPLICIT_SCHED) != EOK )
	{
		return 0;
	}

	if( pthread_attr_setschedpolicy(&threadAttr,SCHED_RR) != EOK )
	{
		return 0;
	}

	if( pthread_create(&mPandoraWorkerThreadProxy, &threadAttr, runPandoraWorkerThread, NULL) != EOK)
	{
		return 0;
	}

	(void) pthread_attr_destroy(&threadAttr);
}

// Setting the Channel for Event Messaging with Pandora Worker thread

int CTunerRequestResponseHandler::setupConnection()
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, setupConnection);
	DBG_MSG(("CTunerRequestResponseHandler::setupConnection"));

	CTunerRequestResponseHandler::MsgConnectionId = ConnectAttach(0, getpid(), CTunerRequestResponseHandler::channelId, 0, 0);
	DBG_MSG(("CTunerRequestResponseHandler::MsgConnectionId %d", CTunerRequestResponseHandler::MsgConnectionId));
	if (CTunerRequestResponseHandler::MsgConnectionId == -1)
	{
		DBG_MSG(("CTunerRequestResponseHandler::MsgConnectionId failed"));
		printf("Connection Id is invalid");

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
void CTunerRequestResponseHandler::profileDisconnectMethodCall()
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, profileDisconnectMethodCall);
	DBG_MSG(("[PANDORA]:profileDisconnectMethodCall mAndroidDeviceConnected:%d, mIphoneBluetooth:%d",CTunerRequestResponseHandler::mAndroidDeviceConnected,CTunerRequestResponseHandler::mIphoneBluetooth));
	if(CTunerRequestResponseHandler::mAndroidDeviceConnected == true)
	{
		CTunerRequestResponseHandler::SPP_PANDORA_SERVICE_DISCONNECT_INITIATED = true;
		Json::Value obj;
		obj["address"] = CTunerRequestResponseHandler::mpInstance->SPP_Pandora_Connected_DeviceAddress;
		obj["service"] = "SPP_PANDORA";
		CTunerRequestResponseHandler::mpInstance->invoke( BLUETOOTH_SERVICE_BUS_NAME, BLUETOOTH_SERVICE_OBJ_NAME, "startServiceDisconnect",obj,CTunerRequestResponseHandler::mpInstance->m_resp);
	}
	else if(CTunerRequestResponseHandler::mIphoneBluetooth == true)
	{
		CTunerRequestResponseHandler::SPP_IAP_Iphone_SERVICE_DISCONNECT_INITIATED = true;
		Json::Value obj;
		obj["address"] = CTunerRequestResponseHandler::mpInstance->SPP_IAP_Iphone_Connected_DeviceAddress;
		obj["service"] = "SPP_IAP_Iphone";
		CTunerRequestResponseHandler::mpInstance->invoke( BLUETOOTH_SERVICE_BUS_NAME, BLUETOOTH_SERVICE_OBJ_NAME, "startServiceDisconnect",obj,CTunerRequestResponseHandler::mpInstance->m_resp);
	}
}
void CTunerRequestResponseHandler::profileConnectMethodCall()
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, profileConnectMethodCall);
	DBG_MSG(("[PANDORA]:profileConnectMethodCall SPP_PANDORA_SERVICE_DISCONNECT_INITIATED:%d, SPP_IAP_Iphone_SERVICE_DISCONNECT_INITIATED:%d",CTunerRequestResponseHandler::SPP_PANDORA_SERVICE_DISCONNECT_INITIATED,CTunerRequestResponseHandler::SPP_IAP_Iphone_SERVICE_DISCONNECT_INITIATED));
	//if(CTunerRequestResponseHandler::mAndroidDeviceConnected == true)
	if(CTunerRequestResponseHandler::SPP_PANDORA_SERVICE_DISCONNECT_INITIATED == true)
	{
		CTunerRequestResponseHandler::SPP_PANDORA_SERVICE_DISCONNECT_INITIATED = false;
		Json::Value obj;
		obj["address"] = CTunerRequestResponseHandler::mpInstance->SPP_Pandora_Connected_DeviceAddress;
		obj["service"] = "SPP_PANDORA";
		CTunerRequestResponseHandler::mpInstance->invoke( BLUETOOTH_SERVICE_BUS_NAME, BLUETOOTH_SERVICE_OBJ_NAME, "startServiceConnect",obj,CTunerRequestResponseHandler::mpInstance->m_resp);
	}
	//else if(CTunerRequestResponseHandler::mIphoneBluetooth == true)
	else if(CTunerRequestResponseHandler::SPP_IAP_Iphone_SERVICE_DISCONNECT_INITIATED == true)
	{
		CTunerRequestResponseHandler::SPP_IAP_Iphone_SERVICE_DISCONNECT_INITIATED = false;
		Json::Value obj;
		obj["address"] = CTunerRequestResponseHandler::mpInstance->SPP_IAP_Iphone_Connected_DeviceAddress;
		obj["service"] = "SPP_IAP_Iphone";
		CTunerRequestResponseHandler::mpInstance->invoke( BLUETOOTH_SERVICE_BUS_NAME, BLUETOOTH_SERVICE_OBJ_NAME, "startServiceConnect",obj,CTunerRequestResponseHandler::mpInstance->m_resp);
	}
}
void CTunerRequestResponseHandler::sendResponse(CSvcIpcRequest* reqData, PandoraErrorTypes error)
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
		reqData->returnReply(mJsonWriter.write(ret));
	}

}
void CTunerRequestResponseHandler::sendPresets(CSvcIpcRequest* reqData, Json::Value res)
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, sendPresets);
	DBG_MSG(("[CTunerRespHandler]: Presets sent !!"));
	Json::FastWriter mJsonWriter;
	reqData->returnReply(mJsonWriter.write(res));
}

void insertionHndlr(struct usbd_connection * conn, usbd_device_instance_t *instance)
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, insertionHndlr);
	char IPOD0_Cntr;
	char IPOD1_Cntr;
	DBG_MSG(("[CTunerRequestResponseHandler::insertionHndlr] instance->path=%x,instance->devno=%x,instance->ident.vendor=%x,instance->ident.device=%x\n",instance->path,instance->devno,instance->ident.vendor,instance->ident.device));
	if((instance->ident.vendor == 0x05ac) && (instance->ident.device == 0x12a8))
	{
		CTunerRequestResponseHandler::usbRmvlPath = -1;
		DBG_MSG(("[CTunerRespHandler]: insertionHndlr:Media Connected !!,usbInsrtPath:%d,usbRmvlPath:%d,IpodPandoraPlayStatus:%d",CTunerRequestResponseHandler::usbInsrtPath,CTunerRequestResponseHandler::usbRmvlPath,CTunerRequestResponseHandler::IpodPandoraPlayStatus));
		DBG_MSG(("[CTunerRespHandler]: mIphoneBluetooth:%d, mAndroidDeviceConnected:%d",CTunerRequestResponseHandler::mIphoneBluetooth,CTunerRequestResponseHandler::mAndroidDeviceConnected));
		if(CTunerRequestResponseHandler::usbInsrtPath != instance->path)
		{
			CTunerRequestResponseHandler::usbInsrtPath = instance->path;
			if(instance->path == 0)
				CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_0 = true;
			else if(instance->path == 1)
				CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_1 = true;
			if( (CTunerRequestResponseHandler::mIphoneBluetooth == false) && (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) )
			{
				CTunerRequestResponseHandler::mIgnoreTerminateSessionCmd = true;
				/*if(instance->path == 0)
					CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_0 = true;
				else if(instance->path == 1)
					CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_1 = true;*/
				CTunerRequestResponseHandler::PandoraDisconnectCheckingFlag = true;
				CTunerRequestResponseHandler::pandoraConnectedDeviceName = PANDORA_DEVICE_IPOD;
				if(!CTunerRequestResponseHandler::IpodPandoraPlayStatus)
				{
					if(instance->path == 0)
					{
						IPOD0_Cntr = 0;
						while(!fopen("/fs/ipod0", "rb"))
						{
							DBG_MSG(("Waiting till ipod0 mountpoint is created:%d",IPOD0_Cntr));
							sleep(1);
							IPOD0_Cntr++;
							if ( IPOD0_Cntr >= 20){
									break;			}
						}
						if (Iap1_Types::OK == vCIap1MgrController->requestIpodStartDevCtrl("/fs/ipod0"))
						{
							CTunerRequestResponseHandler::IpodPandoraPlayStatus = IPOD_USB_0;
							CTunerRequestResponseHandler::mIpodDeviceConnected = true;
							//CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_0 = true;
							DBG_MSG(("requestIpodStartDevCtrl: Success mIpod0DeviceConnected %d",CTunerRequestResponseHandler::mIpodDeviceConnected));

							Json::Value data;
							std::string signalName = "PandoraReadyState";
							data["appStatus"] = PANDORA_READY_STATE;
							data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data["value"] = true;
							CTunerRequestResponseHandler::handleNotif(signalName, data);
							DBG_MSG(("[PANDORA]:USB_IPHONE0 PandoraReadyState true"));
						}
						else
						{
							DBG_MSG(("requestIpod0StartDevCtrl: Failed IPOD0_Cntr:%d",IPOD0_Cntr));
						}
					}
					else if(instance->path == 1)
					{
						IPOD1_Cntr = 0;
						while(!fopen("/fs/ipod1", "rb"))
						{
							DBG_MSG(("Waiting till ipod1 mountpoint is created :%d",IPOD1_Cntr));
							sleep(1);
							IPOD1_Cntr++;
							if ( IPOD1_Cntr >= 20){
									break;	 		}
						}
						if (Iap1_Types::OK == vCIap1MgrController->requestIpodStartDevCtrl("/fs/ipod1"))
						{
							CTunerRequestResponseHandler::IpodPandoraPlayStatus = IPOD_USB_1;
							CTunerRequestResponseHandler::mIpodDeviceConnected = true;
							//CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_1 = true;
							DBG_MSG(("requestIpodStartDevCtrl: Success mIpod1DeviceConnected %d",CTunerRequestResponseHandler::mIpodDeviceConnected));

							Json::Value data;
							std::string signalName = "PandoraReadyState";
							data["appStatus"] = PANDORA_READY_STATE;
							data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data["value"] = true;
							CTunerRequestResponseHandler::handleNotif(signalName, data);
							DBG_MSG(("[PANDORA]:USB_IPHONE1 PandoraReadyState true"));
						}
						else
						{
							DBG_MSG(("requestIpod1StartDevCtrl: Failed IPOD1_Cntr:%d",IPOD1_Cntr));
						}
					}
				}
			}
		}
	}
}

void removalHndlr(struct usbd_connection * conn, usbd_device_instance_t *instance)
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, removalHndlr);
	//DBG_MSG(("[CTunerRespHandler]: removalHndlr !!"));
	DBG_MSG(("[CTunerRequestResponseHandler::removalHndlr] instance->path=%x,instance->devno=%x,instance->ident.vendor=%x,instance->ident.device=%x\n",instance->path,instance->devno,instance->ident.vendor,instance->ident.device));
	if((instance->ident.vendor == 0x05ac) && (instance->ident.device == 0x12a8))
	{
		if(instance->path == 0)
			CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_0 = false;
		else if(instance->path == 1)
			CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_1 = false;

		CTunerRequestResponseHandler::usbInsrtPath = -1;

		if( instance->path == (CTunerRequestResponseHandler::IpodPandoraPlayStatus - 1) )
		{
			DBG_MSG(("[CTunerRespHandler]: removalHndlr:Media DisConnected mIpodDeviceConnected=%d,mAndroidDeviceConnected=%d,IpodPandoraPlayStatus=%d",CTunerRequestResponseHandler::mIpodDeviceConnected,CTunerRequestResponseHandler::mAndroidDeviceConnected,CTunerRequestResponseHandler::IpodPandoraPlayStatus));
			//if( (CTunerRequestResponseHandler::mIpodDeviceConnected == true) && (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) )
			if( (CTunerRequestResponseHandler::mIpodDeviceConnected == true) && (CTunerRequestResponseHandler::IpodPandoraPlayStatus) )
			{
				CTunerRequestResponseHandler::PandoraDisconnectCheckingFlag = true;
				CTunerRequestResponseHandler::mIpodDeviceConnected = false;
				CTunerRequestResponseHandler::IpodPandoraPlayStatus = 0;
				CTunerRequestResponseHandler::mIgnoreTerminateSessionCmd = false;
				DBG_MSG(("[CTunerRespHandler]: In mIpodDeviceConnected=%d,mAndroidDeviceConnected=%d,usbInsrtPath:%d,usbRmvlPath:%d",CTunerRequestResponseHandler::mIpodDeviceConnected,CTunerRequestResponseHandler::mAndroidDeviceConnected,CTunerRequestResponseHandler::usbInsrtPath,CTunerRequestResponseHandler::usbRmvlPath));
				if(CTunerRequestResponseHandler::usbRmvlPath != instance->path)
				{
					CTunerRequestResponseHandler::usbRmvlPath = instance->path;
					if (Iap1_Types::OK == vCIap1MgrController->requestIpodStopDevCtrl())
					{
						IIpodNotificationImpl::mSessionId  = -1;
						Iap1_Types::tError mError =  vCIap1MgrController->requestIpodStopAudio();
						DBG_MSG(("[CTunerRespHandler]:removalHndlr Stopping the IpodDevice Graph and return value is : %d",mError));
						vPandoraProtocolMgr->deInitialize();

						//CTunerCmdInvoker::meSessionState  = eSessionNone;
						DBG_MSG(("[CTunerRespHandler]:CTunerCmdInvoker::meSessionState %d ", CTunerCmdInvoker::meSessionState));
						//CTunerCmdInvoker::mStationActive == false;
						//if( (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) && (CTunerRequestResponseHandler::mIphoneBluetooth == false) )
						if(CTunerRequestResponseHandler::pandoraSessionConnectedDevice == PANDORA_SESSION_IPOD)
						{
							Json::Value data3;
							std::string signalName3 = "PandoraReadyState";
							data3["appStatus"] = PANDORA_READY_STATE;
							data3["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data3["value"] = false;
							CTunerRequestResponseHandler::handleNotif(signalName3, data3);
							DBG_MSG(("[PANDORA]:USB_IPHONE PandoraReadyState false"));

							Json::Value data;
							std::string signalName = "PandoraAudio";
							data["value"] = 0;
							CTunerRequestResponseHandler::handleNotif(signalName, data);

							Json::Value data2;
							std::string signalName2 = "InfoUpdateStationActive";
							data2["StationToken"] = 0;
							data2["value"] = false;
							CTunerRequestResponseHandler::handleNotif(signalName2, data2);

							/*if(CTunerRequestResponseHandler::mAndroidDeviceConnected == true)
							{
								Json::Value data4;
								std::string signalName4 = "PandoraReadyState";
								data4["appStatus"] = PANDORA_READY_STATE;
								data4["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
								data4["value"] = true;
								CTunerRequestResponseHandler::handleNotif(signalName4, data4);
								DBG_MSG(("[PANDORA]:USB_IPHONE AndroidDevice PandoraReadyState true"));
							}*/
						}
						//CTunerRequestResponseHandler::mpInstance->afterPandoraDisconnectChecking();
						/*if (true == mPandoraDeviceCheck->isActive ())
						{
							mPandoraDeviceCheck->stopTimer();
							DBG_MSG(("[PANDORA]:IPOD mPandoraDeviceCheck isActive"));
						}
						mPandoraDeviceCheck->startTimer(2000, true);*/
					}
					else
					{
						DBG_MSG(("requestIpodStopDevCtrl: Failed"));
					}
				}
			}
		}
		/*if(CTunerRequestResponseHandler::mIpodDeviceConnected == true)
		{
			CTunerRequestResponseHandler::mIpodDeviceConnected = false;
			DBG_MSG(("[CTunerRespHandler]: setting mIpodDeviceConnected=%d",CTunerRequestResponseHandler::mIpodDeviceConnected));
		}*/
	}
}

void CTunerRequestResponseHandler::initPandoraAudioGraphAgain()
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, initPandoraAudioGraphAgain);
	DBG_MSG(("[CTunerRespHandler] :initPandoraAudioGraphAgain:Start"));

	int fh1 = open("/dev/io-media/graphs/trackplayer/default",O_RDONLY);
	int fh2 = open("/dev/io-media/graphs/mediafs_2wire/default",O_RDONLY);
	if( (fh1 > 0) || (fh2 > 0) )
	{
		if(fh1){
			DBG_MSG(("/dev/io-media/graphs/trackplayer/default exist fh1:%d",fh1));
			close(fh1);
		}
		if(fh2){
			DBG_MSG(("/dev/io-media/graphs/mediafs_2wire/default exist fh2:%d",fh2));
			close(fh2);
		}
		Json::Value data;
		std::string signalName = "PandoraAudio";
		data["value"] = 0;
		CTunerRequestResponseHandler::handleNotif(signalName, data);
	}
	else
	{
		if(CTunerCmdInvoker::mGraphid != -1)
		{
			CTunerCmdInvoker::SessionTerminated = 1;
			int rs =  iom_stop(CTunerCmdInvoker::mGraphid);
			DBG_MSG(("[CTunerRespHandler]:Graph Stop initPandoraAudioGraph: return value : %d", rs));
			rs = iom_destroy_graph(CTunerCmdInvoker::mGraphid);
			DBG_MSG(("[CTunerRespHandler]:Graph destroyed initPandoraAudioGraph: return value : %d", rs));
			if(rs == 0){
				CTunerCmdInvoker::mGraphid = -1;
			}
		}

		CTunerCmdInvoker::mGraphid = iom_create_graph( "/dev/io-media/graphs/trackplayer", "PandoraAudio", 0);

		int fh = open("/fs/avrcp00/.FS_info./audio",O_RDONLY);
		if (fh < 0)
		{
			DBG_ERROR(("[CTunerRespHandler]:MediaMirrorLink::  failed errno: %s",  strerror(errno)));
		}
		else
		{
			DBG_MSG(("[CTunerRespHandler]:MediaMirrorLink:: succeeded"));
			char tmp[100];
			int ret = devctl(fh, DCMD_MEDIA_URL, (void*)tmp, sizeof(tmp) - 1, NULL);
			DBG_MSG(("[CTunerRespHandler] :devctl:tmp: %s", (char*)tmp));
			string inputURL = tmp;
			close(fh);
			if (ret != EOK)
			{
				DBG_ERROR(("[CTunerRespHandler]:MediaMirrorLink:: devctl failed errno: %s",strerror(errno)));
			}

			DBG_MSG(("[CTunerRespHandler] :File descriptor value: %d", CTunerCmdInvoker::mGraphid));
			if(CTunerCmdInvoker::mGraphid > 0)
			{
				DBG_MSG(("[CTunerRespHandler] :Audio Graph is created successfully"));
				int rc = iom_attach_output( CTunerCmdInvoker::mGraphid, "snd:/dev/snd/pcmC0D0p", (IOM_OF_AUDIO) );
				DBG_MSG(("[CTunerRespHandler] :iom attach output%d", rc));

				rc = iom_attach_input(CTunerCmdInvoker::mGraphid, "wms:/fs/avrcp00/.FS_info./audio", 1);
				DBG_MSG(("[CTunerRespHandler] :input URL Path %s", inputURL.c_str()));
				DBG_MSG(("[CTunerRespHandler] :iom attach input%d", rc));
				DBG_ERROR(("[CTunerRespHandler]:MediaMirrorLink::  iom attach input: %s",  strerror(errno)));
				rc = iom_play(CTunerCmdInvoker::mGraphid);
				DBG_MSG(("[CTunerRespHandler] :iom play %d", rc));
				CTunerCmdInvoker::SessionTerminated = 0;
			}
			DBG_MSG(("[CTunerRespHandler] :initPandoraAudioGraph:End"));
		}
		Json::Value data;
		std::string signalName = "PandoraAudio";
		data["value"] = 1;
		CTunerRequestResponseHandler::handleNotif(signalName, data);
	}
}

void CTunerRequestResponseHandler::PandoraSocketDisconnect()
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, PandoraSocketDisconnect);
	DBG_MSG(("[PandoraSocketDisconnect]: mAndroidDeviceConnected:%d, mIphoneBluetooth:%d, mIpodDeviceConnected:%d,PandoraDisconnectCheckingFlag:%d",CTunerRequestResponseHandler::mAndroidDeviceConnected,
			CTunerRequestResponseHandler::mIphoneBluetooth,CTunerRequestResponseHandler::mIpodDeviceConnected,CTunerRequestResponseHandler::PandoraDisconnectCheckingFlag));
	if(CTunerRequestResponseHandler::mAndroidDeviceConnected == true)
	{
		CTunerRequestResponseHandler::mAndroidDeviceConnected = false;
		if(CTunerCmdInvoker::mGraphid != -1)
		{
			int rs =  iom_stop(CTunerCmdInvoker::mGraphid);
			DBG_MSG(("[PANDORA]:Graph Stop initPandoraAudioGraph: return value : %d", rs));
			rs = iom_destroy_graph(CTunerCmdInvoker::mGraphid);
			DBG_MSG(("[PANDORA]:Graph destroyed initPandoraAudioGraph: return value : %d", rs));
			if(rs == 0){
				CTunerCmdInvoker::mGraphid = -1;
			}
			Json::Value data;
			std::string signalName = "PandoraAudio";
			data["value"] = 0;
			CTunerRequestResponseHandler::handleNotif(signalName, data);
		}
		Json::Value data;
		DBG_MSG(("[PandoraSocketDisconnect]: Session Terminated: InforUpdateStationActive"));
		std::string signalName = "InfoUpdateStationActive";
		CTunerCmdInvoker::meSessionState = eSessionNone;
		DBG_MSG(("[PandoraSocketDisconnect]:CTunerCmdInvoker::meSessionState %d ", CTunerCmdInvoker::meSessionState));
		//CTunerCmdInvoker::mStationActive == false;
		data["StationToken"] = 0;
		data["value"] = false;
		CTunerRequestResponseHandler::handleNotif(signalName, data);

		DBG_MSG(("[PandoraSocketDisconnect]Android Device is disconnected which has Pandora App"));
		Json::Value data2;
		std::string signalName2 = "PandoraReadyState";
		data2["appStatus"] = PANDORA_READY_STATE;
		data2["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
		data2["value"] = false;
		CTunerRequestResponseHandler::handleNotif(signalName2, data2);
		DBG_MSG(("[PandoraSocketDisconnect]:SPP_PANDORA PandoraReadyState false"));
		vPandoraProtocolMgr->deInitialize();
	}
	else if(CTunerRequestResponseHandler::mIphoneBluetooth == true)
	{
		if(CTunerRequestResponseHandler::mIphoneStartDevCtl == true)
		{
			CTunerRequestResponseHandler::mIphoneStartDevCtl = false;
			if (Iap1_Types::OK == vCIap1MgrController->requestIpodStopDevCtrl())
			{
				IIpodNotificationImpl::mSessionId  = -1;
				CTunerRequestResponseHandler::mIphoneBluetooth = false;
				DBG_MSG(("[PandoraSocketDisconnect]requestIpodStopDevCtrl: Success"));

				if(CTunerCmdInvoker::mGraphid != -1)
				{
					int rs =  iom_stop(CTunerCmdInvoker::mGraphid);
					DBG_MSG(("[PandoraSocketDisconnect]:IPhone Graph Stop initPandoraAudioGraph: return value : %d", rs));
					rs = iom_destroy_graph(CTunerCmdInvoker::mGraphid);
					DBG_MSG(("[PandoraSocketDisconnect]:IPhone Graph destroyed initPandoraAudioGraph: return value : %d", rs));
					if(rs == 0){
						CTunerCmdInvoker::mGraphid = -1;
					}
					Json::Value data;
					std::string signalName = "PandoraAudio";
					data["value"] = 0;
					CTunerRequestResponseHandler::handleNotif(signalName, data);
				}
				Json::Value dataIUSA;
				std::string signalNameIUSA = "InfoUpdateStationActive";
				CTunerCmdInvoker::meSessionState = eSessionNone;
				DBG_MSG(("[PandoraSocketDisconnect]:CTunerCmdInvoker::meSessionState %d ", CTunerCmdInvoker::meSessionState));
				//CTunerCmdInvoker::mStationActive == false;
				dataIUSA["StationToken"] = 0;
				dataIUSA["value"] = false;
				CTunerRequestResponseHandler::handleNotif(signalNameIUSA, dataIUSA);
				DBG_MSG(("[PandoraSocketDisconnect]iPhone Device is disconnected which has Pandora App"));

				Json::Value data;
				std::string signalName = "PandoraReadyState";
				data["appStatus"] = PANDORA_READY_STATE;
				data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
				data["value"] = false;
				CTunerRequestResponseHandler::handleNotif(signalName, data);
				DBG_MSG(("[PandoraSocketDisconnect]:SPP_IAP_Iphone PandoraReadyState false"));
			}
			else
			{
				DBG_MSG(("[PandoraSocketDisconnect]requestIpodStopDevCtrl: Failed"));
			}
		}
	}
	else if(CTunerRequestResponseHandler::mIpodDeviceConnected == true)
	{
		CTunerRequestResponseHandler::mIpodDeviceConnected = false;
		CTunerRequestResponseHandler::IpodPandoraPlayStatus = 0;
		if (Iap1_Types::OK == vCIap1MgrController->requestIpodStopDevCtrl())
		{
			IIpodNotificationImpl::mSessionId  = -1;
			Iap1_Types::tError mError =  vCIap1MgrController->requestIpodStopAudio();
			DBG_MSG(("[PandoraSocketDisconnect]:Bkup Stopping the IpodDevice Graph and return value is : %d",mError));
			vPandoraProtocolMgr->deInitialize();

			DBG_MSG(("[PandoraSocketDisconnect]:Bkup CTunerCmdInvoker::meSessionState %d ", CTunerCmdInvoker::meSessionState));

			if(CTunerRequestResponseHandler::pandoraSessionConnectedDevice == PANDORA_SESSION_IPOD)
			{
				Json::Value data3;
				std::string signalName3 = "PandoraReadyState";
				data3["appStatus"] = PANDORA_READY_STATE;
				data3["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
				data3["value"] = false;
				CTunerRequestResponseHandler::handleNotif(signalName3, data3);
				DBG_MSG(("[PandoraSocketDisconnect]:Bkup USB_IPHONE PandoraReadyState false Bkup"));
				Json::Value data;
				std::string signalName = "PandoraAudio";
				data["value"] = 0;
				CTunerRequestResponseHandler::handleNotif(signalName, data);

				Json::Value data2;
				std::string signalName2 = "InfoUpdateStationActive";
				data2["StationToken"] = 0;
				data2["value"] = false;
				CTunerRequestResponseHandler::handleNotif(signalName2, data2);
			}
		}
	}
	/*if(CTunerRequestResponseHandler::PandoraDisconnectCheckingFlag == true)
	{
		CTunerRequestResponseHandler::PandoraDisconnectCheckingFlag = false;
		afterPandoraDisconnectChecking();
	}*/
}

void CTunerRequestResponseHandler::afterPandoraDisconnectChecking()
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, afterPandoraDisconnectChecking);
	DBG_MSG(("afterPandoraDisconnectChecking mIphoneBluetooth:%d,mAndroidDeviceConnected:%d,mIpodDeviceConnected:%d,IpodPandoraPlayStatus:%d",CTunerRequestResponseHandler::mIphoneBluetooth,
						CTunerRequestResponseHandler::mAndroidDeviceConnected,CTunerRequestResponseHandler::mIpodDeviceConnected,CTunerRequestResponseHandler::IpodPandoraPlayStatus));
	DBG_MSG(("PANDORA_CONNECTED_DEVICE_IPOD_0:%d,PANDORA_CONNECTED_DEVICE_IPOD_1:%d,PANDORA_CONNECTED_DEVICE_ANDROID:%d,PANDORA_CONNECTED_DEVICE_IPHONE:%d",CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_0,
						CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_1,CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_ANDROID,CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPHONE));
	//if( (CTunerRequestResponseHandler::mIpodDeviceConnected == false) && (CTunerRequestResponseHandler::mIphoneBluetooth == false) && (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) )
	//{
		if( (CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_0 == true) || (CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_1 == true) )
		{
			char IPOD0_Cntr;
			char IPOD1_Cntr;
			if( (CTunerRequestResponseHandler::mIphoneBluetooth == false) && (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) )
			{
				if(!CTunerRequestResponseHandler::IpodPandoraPlayStatus)
				{
					if(CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_0)
					{
						IPOD0_Cntr = 0;
						while(!fopen("/fs/ipod0", "rb"))
						{
							DBG_MSG(("Waiting till ipod0 mountpoint is created Chk:%d",IPOD0_Cntr));
							sleep(1);
							IPOD0_Cntr++;
							if ( IPOD0_Cntr >= 20){
									break;			}
						}
						if (Iap1_Types::OK == vCIap1MgrController->requestIpodStartDevCtrl("/fs/ipod0"))
						{
							CTunerRequestResponseHandler::IpodPandoraPlayStatus = IPOD_USB_0;
							CTunerRequestResponseHandler::mIpodDeviceConnected = true;
							DBG_MSG(("requestIpodStartDevCtrl: Success mIpod0DeviceConnected %d Chk",CTunerRequestResponseHandler::mIpodDeviceConnected));

							Json::Value data;
							std::string signalName = "PandoraReadyState";
							data["appStatus"] = PANDORA_READY_STATE;
							data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data["value"] = true;
							CTunerRequestResponseHandler::handleNotif(signalName, data);
							CTunerRequestResponseHandler::setPandoraAudioAutoRecovery = true;
							DBG_MSG(("[PANDORA]:USB_IPHONE0 PandoraReadyState true Chk"));
						}
						else
						{
							DBG_MSG(("requestIpod0StartDevCtrl: Failed IPOD0_Cntr:%d Chk",IPOD0_Cntr));
						}
					}
					else if(CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_1)
					{
						IPOD1_Cntr = 0;
						while(!fopen("/fs/ipod1", "rb"))
						{
							DBG_MSG(("Waiting till ipod1 mountpoint is created Chk:%d",IPOD1_Cntr));
							sleep(1);
							IPOD1_Cntr++;
							if ( IPOD1_Cntr >= 20){
									break;	 		}
						}
						if (Iap1_Types::OK == vCIap1MgrController->requestIpodStartDevCtrl("/fs/ipod1"))
						{
							CTunerRequestResponseHandler::IpodPandoraPlayStatus = IPOD_USB_1;
							CTunerRequestResponseHandler::mIpodDeviceConnected = true;
							DBG_MSG(("requestIpodStartDevCtrl: Success mIpod1DeviceConnected %d Chk",CTunerRequestResponseHandler::mIpodDeviceConnected));

							Json::Value data;
							std::string signalName = "PandoraReadyState";
							data["appStatus"] = PANDORA_READY_STATE;
							data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data["value"] = true;
							CTunerRequestResponseHandler::handleNotif(signalName, data);
							CTunerRequestResponseHandler::setPandoraAudioAutoRecovery = true;
							DBG_MSG(("[PANDORA]:USB_IPHONE1 PandoraReadyState true Chk"));
						}
						else
						{
							DBG_MSG(("requestIpod1StartDevCtrl: Failed IPOD1_Cntr:%d Chk",IPOD1_Cntr));
						}
					}
				}
			}
		}
		else if(CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_ANDROID == true)
		{
			if( (CTunerRequestResponseHandler::mIpodDeviceConnected == false) && (CTunerRequestResponseHandler::mIphoneBluetooth == false) )
			{
				CTunerCmdInvoker::mPandoraDeviceFd  = getPandoraDeviceFd();
				if(CTunerCmdInvoker::mPandoraDeviceFd == -1)
				{
					DBG_MSG(("file open is failed and errno value is %d",errno));
					return;
				}
				CTunerRequestResponseHandler::mAndroidDeviceConnected = true;
				DBG_MSG(("mAndroidDeviceConnected:%d", CTunerRequestResponseHandler::mAndroidDeviceConnected));
				DBG_MSG(("File descriptor is opened and value is %d", CTunerCmdInvoker::mPandoraDeviceFd));
				pthread_attr_t threadAttr;
				sched_param_t  schedParam;
				// create thread attributes
				if( pthread_attr_init(&threadAttr) != EOK )
				{
					return;
				}
				if( pthread_attr_getschedparam(&threadAttr,&schedParam) != EOK )
				{
					return;
				}
				schedParam.sched_priority = 11;
				if( pthread_attr_setschedparam(&threadAttr,&schedParam) != EOK )
				{
					return;
				}
				if( pthread_attr_setinheritsched(&threadAttr,PTHREAD_EXPLICIT_SCHED) != EOK )
				{
					return;
				}
				if( pthread_attr_setschedpolicy(&threadAttr,SCHED_RR) != EOK )
				{
					return;
				}
				if( pthread_create(&mPandoraReaderThreadProxy, &threadAttr, runPandoraReaderThread, (void*)vPandoraProtocolMgr) != EOK)
				{
					DBG_MSG(("failed to create thread"));
					return;
				}
				(void) pthread_attr_destroy(&threadAttr);

				//CTunerRequestResponseHandler::mAndroidDeviceConnected = true;
				Json::Value data;
				std::string signalName = "PandoraReadyState";
				data["appStatus"] = PANDORA_READY_STATE;
				data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
				data["value"] = true;
				CTunerRequestResponseHandler::handleNotif(signalName, data);
				CTunerRequestResponseHandler::setPandoraAudioAutoRecovery = true;
				DBG_MSG(("[PANDORA]:SPP_PANDORA PandoraReadyState true chk"));
			}
		}
		else if(CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPHONE == true)
		{
			if( (CTunerRequestResponseHandler::mIpodDeviceConnected == false) && (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) )
			{
				char BTIPOD0_Cntr = 0;
				while(!fopen("/fs/btipod0", "rb"))
				{
					DBG_MSG(("Waiting till btipod0 mountpoint is created chk"));
					sleep(1);
					BTIPOD0_Cntr++;
					if ( BTIPOD0_Cntr >= 20){
							break;	 		}
				}
				if (Iap1_Types::OK == vCIap1MgrController->requestIpodStartDevCtrl("/fs/btipod0"))
				{
					CTunerRequestResponseHandler::mIphoneBluetooth = true;
					CTunerRequestResponseHandler::mIphoneStartDevCtl = true;
					//CTunerRequestResponseHandler::mIpodDeviceConnected = false;
					DBG_MSG(("requestIpodStartDevCtrl: Success mIphoneBluetooth = %d,mIphoneStartDevCtl=%d chk",CTunerRequestResponseHandler::mIphoneBluetooth,CTunerRequestResponseHandler::mIphoneStartDevCtl));

					//if(CTunerRequestResponseHandler::mIpodDeviceConnected == false)
					//{
						Json::Value data;
						std::string signalName = "PandoraReadyState";
						data["appStatus"] = PANDORA_READY_STATE;
						data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
						data["value"] = true;
						CTunerRequestResponseHandler::handleNotif(signalName, data);
						CTunerRequestResponseHandler::setPandoraAudioAutoRecovery = true;
						DBG_MSG(("[PANDORA]:SPP_IAP_Iphone PandoraReadyState true chk"));
					//}
				}
				else
				{
					DBG_MSG(("requestIpodStartDevCtrl: Failed chk"));
				}
			}
		}
	//}
}

void CTunerRequestResponseHandler::afterIgnoreBtDisconnectionSignal()
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, afterIgnoreBtDisconnectionSignal);
	DBG_MSG(("afterIgnoreBtDisconnectionSignal mIphoneBluetooth:%d,mAndroidDeviceConnected:%d,mIpodDeviceConnected:%d,IpodPandoraPlayStatus:%d",CTunerRequestResponseHandler::mIphoneBluetooth,
						CTunerRequestResponseHandler::mAndroidDeviceConnected,CTunerRequestResponseHandler::mIpodDeviceConnected,CTunerRequestResponseHandler::IpodPandoraPlayStatus));
	DBG_MSG(("PANDORA_CONNECTED_DEVICE_IPOD_0:%d,PANDORA_CONNECTED_DEVICE_IPOD_1:%d,PANDORA_CONNECTED_DEVICE_ANDROID:%d,PANDORA_CONNECTED_DEVICE_IPHONE:%d",CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_0,
						CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_1,CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_ANDROID,CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPHONE));
	if( (CTunerRequestResponseHandler::mIpodDeviceConnected == false) && (CTunerRequestResponseHandler::mIphoneBluetooth == false) && (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) )
	{
		if( (CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_0 == true) || (CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_1 == true) )
		{
			char IPOD0_Cntr;
			char IPOD1_Cntr;
			//if( (CTunerRequestResponseHandler::mIphoneBluetooth == false) && (CTunerRequestResponseHandler::mAndroidDeviceConnected == false) )
			//{
				if(!CTunerRequestResponseHandler::IpodPandoraPlayStatus)
				{
					if(CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_0)
					{
						IPOD0_Cntr = 0;
						while(!fopen("/fs/ipod0", "rb"))
						{
							DBG_MSG(("Waiting till ipod0 mountpoint is created Chk:%d",IPOD0_Cntr));
							sleep(1);
							IPOD0_Cntr++;
							if ( IPOD0_Cntr >= 20){
									break;			}
						}
						if (Iap1_Types::OK == vCIap1MgrController->requestIpodStartDevCtrl("/fs/ipod0"))
						{
							CTunerRequestResponseHandler::IpodPandoraPlayStatus = IPOD_USB_0;
							CTunerRequestResponseHandler::mIpodDeviceConnected = true;
							DBG_MSG(("requestIpodStartDevCtrl: Success mIpod0DeviceConnected %d Chk",CTunerRequestResponseHandler::mIpodDeviceConnected));

							Json::Value data2;
							std::string signalName2 = "PandoraReadyState";
							data2["appStatus"] = PANDORA_READY_STATE;
							data2["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data2["value"] = false;
							CTunerRequestResponseHandler::handleNotif(signalName2, data2);

							Json::Value data;
							std::string signalName = "PandoraReadyState";
							data["appStatus"] = PANDORA_READY_STATE;
							data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data["value"] = true;
							CTunerRequestResponseHandler::handleNotif(signalName, data);
							CTunerRequestResponseHandler::setPandoraAudioAutoRecovery = true;
							DBG_MSG(("[PANDORA]:USB_IPHONE0 PandoraReadyState true Chk"));
						}
						else
						{
							DBG_MSG(("requestIpod0StartDevCtrl: Failed IPOD0_Cntr:%d Chk",IPOD0_Cntr));
						}
					}
					else if(CTunerRequestResponseHandler::PANDORA_CONNECTED_DEVICE_IPOD_1)
					{
						IPOD1_Cntr = 0;
						while(!fopen("/fs/ipod1", "rb"))
						{
							DBG_MSG(("Waiting till ipod1 mountpoint is created Chk:%d",IPOD1_Cntr));
							sleep(1);
							IPOD1_Cntr++;
							if ( IPOD1_Cntr >= 20){
									break;	 		}
						}
						if (Iap1_Types::OK == vCIap1MgrController->requestIpodStartDevCtrl("/fs/ipod1"))
						{
							CTunerRequestResponseHandler::IpodPandoraPlayStatus = IPOD_USB_1;
							CTunerRequestResponseHandler::mIpodDeviceConnected = true;
							DBG_MSG(("requestIpodStartDevCtrl: Success mIpod1DeviceConnected %d Chk",CTunerRequestResponseHandler::mIpodDeviceConnected));

							Json::Value data2;
							std::string signalName2 = "PandoraReadyState";
							data2["appStatus"] = PANDORA_READY_STATE;
							data2["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data2["value"] = false;
							CTunerRequestResponseHandler::handleNotif(signalName2, data2);

							Json::Value data;
							std::string signalName = "PandoraReadyState";
							data["appStatus"] = PANDORA_READY_STATE;
							data["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							data["value"] = true;
							CTunerRequestResponseHandler::handleNotif(signalName, data);
							CTunerRequestResponseHandler::setPandoraAudioAutoRecovery = true;
							DBG_MSG(("[PANDORA]:USB_IPHONE1 PandoraReadyState true Chk"));
						}
						else
						{
							DBG_MSG(("requestIpod1StartDevCtrl: Failed IPOD1_Cntr:%d Chk",IPOD1_Cntr));
						}
					}
				}
			//}
		}
	}
}

void CTunerRequestResponseHandler::startMonitoring ()
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, startMonitoring);
	DBG_MSG(("[CTunerRespHandler]: startMonitoring !!"));
	usbd_funcs_t 		funcs 		= {_USBDI_NFUNCS, insertionHndlr, removalHndlr, NULL};
	usbd_connect_parm_t parm 		= {"/dev/io-usb/io-usb", USB_VERSION, USBD_VERSION, 0, 0, NULL, 0, NULL, &funcs,0 };
	int	 error 	= 0;
	struct usbd_connection    *connection;
	if ((error = usbd_connect (&parm, &connection)) != EOK)
	{
		DBG_MSG(("\n[SYSTEMMgr] Unable to connect to USB Server!\n"));
		//mConnection = 0;
	}
	else
	{
		DBG_MSG(("\n[SYSTEMMgr]  connect to USB Server!\n"));
		//mConnection = 1;
	}
}

void CTunerRequestResponseHandler::stopMonitoring ()
{
	TRC_SCOPE(PandoraService, CTunerRequestResponseHandler, stopMonitoring);
	DBG_MSG(("[CTunerRespHandler]: stopMonitoring !!"));
	//if (0 != mConnection)
	//usbd_disconnect(mConnection);

}
//----------------------------------------------------- END OF FILE ---------------------------------------------------
