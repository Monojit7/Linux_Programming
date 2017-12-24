/*----------------------------------------------------------------------------------------------------------------------
 *
 * 
 *
 *---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/


#include "CPandoraCmdInvoker.hpp"
#include "CPandoraDataTypes.hpp"
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <sys/dcmd_media.h>
#include "CPandoraRequestResponseHandler.hpp"
#include "IIpodNotificationImpl.hpp"
#include "iomedia/api.h"

#define	PANDORA_APP_AVAILABLE 	3
#define PANDORA_SESSION_IPOD	1
#define	PANDORA_SESSION_IPHONE	2
#define	PANDORA_SESSION_ANDROID	3
#define PM_STANDBY_MODE		 	1
#define	PM_PROTECTION_MODE	 	2
#define	PM_FULLYWORKING_MODE 	3
#define	PM_POWERSAVING_MODE  	4
#define PANDORA_DEVICE_ANDROID  1
#define PANDORA_DEVICE_IPHONE   2
#define PANDORA_DEVICE_IPOD     3

TRC_SCOPE_DEF(PandoraService, CTunerCmdInvoker, handleTunerRequest);
TRC_SCOPE_DEF(PandoraService, CTunerCmdInvoker, getEnumFromMethod);
TRC_SCOPE_DEF(PandoraService, CTunerCmdInvoker, printData);
TRC_SCOPE_DEF(PandoraService, CTunerCmdInvoker, initAudioGraph);
TRC_SCOPE_DEF(PandoraService, CTunerCmdInvoker, AudioThread);
TRC_SCOPE_DEF(PandoraService, CTunerCmdInvoker, updateBrandingImage);

CTunerCmdInvoker* CTunerCmdInvoker::mpInstance = NULL;
//CPandoraProtocolMgr* CTunerCmdInvoker::vPandoraProtocolMgr = NULL;
CPandoraMessageSenderImpl *CTunerCmdInvoker::mPandoraMessageSenderImpl = NULL;
int CTunerCmdInvoker::mGraphid = -1;

//pthread_t   mPandoraReaderThreadProxy;
//static void* runPandoraReaderThread(void *arg);
//static pthread_mutex_t  mPandoraReaderThreadMutex;
//extern CPandoraProtocolMgr* vPandoraProtocolMgr;

//void printData(const uint8_t *data, const uint32_t dataSize);
int CTunerCmdInvoker::mPandoraDeviceFd = 0;
bool CTunerCmdInvoker::mIpodDeviceSupport = false;
bool CTunerCmdInvoker::mTerminateSession = false;
eSessionState CTunerCmdInvoker::meSessionState = eSessionNone;
unsigned int CTunerCmdInvoker::mStationActiveToken = -1;
eRequestState CTunerCmdInvoker::mPauseSession = eRequestNone;
eRequestState CTunerCmdInvoker::sTerminateSession = eRequestNone;
bool CTunerCmdInvoker::SessionTerminated = 0;
bool CTunerCmdInvoker::currentStationIsPlaying = true;
bool CTunerCmdInvoker::setPandoraAudioAgainFlag = true;
bool CTunerCmdInvoker::setPandoraAudioAgainFlagChkOnCmd = false;
pthread_t   mPandoraAudioReaderThreadProxy;
static void* runPandoraReaderAudioThread(void *arg);
//IPandoraProtocolListener *listener = new IPandoraListenerImpl;
///CPandoraProtocolMgr* vPandoraProtocolMgr = new CPandoraProtocolMgr(listener);
extern CPandoraProtocolMgr* vPandoraProtocolMgr;
extern CIap1MgrController* vCIap1MgrController;
static pthread_mutex_t  mPandoraGraphThreadMutex;
extern CTimer *mPandoraDeviceCheck;
extern CTimer *mPandoraAudioTimer;

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
	cout << "CTunerCmdInvoker" << endl;
	this->mGraphName = "PandoraUSBAudioGraph";
	this->mIOMGraphPath = "/dev/io-media/graphs/trackplayer";


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
		mPandoraMessageSenderImpl = new CPandoraMessageSenderImpl;
		mpInstance = new CTunerCmdInvoker();
		if(mpInstance)
			mpInstance->Initialize();
	}
	return mpInstance;
}

void CTunerCmdInvoker::printData(const uint8_t *data, const int dataSize, eCommunicationType mtype)
{
	TRC_SCOPE(PandoraService, CTunerCmdInvoker, printData);
	//DBG_MSG(("[CPandora]:printData"));
	if ((data == NULL) || (dataSize <= 0))
	{
		printf("[ printData ] : Data is NULL");
		return;
		// DBG_MSG(("[ printData ] : Data is NULL"));
	}
	//
	char temp[256];
	int len = 0;

	for (uint32_t i = 0; i < dataSize; i++)
	{
		len = len + sprintf (temp + len, "%.2X ", data[i]);

		if ( ((i%70) == 0) && (i > 0) )
		{
			temp[len] = '\0';
			DBG_MSG(("[Pandora Bytes] : %s", temp));
			len = 0;
		}
	}

	temp[len] = '\0';


	if( mtype == eRead)
	{
		DBG_MSG(("[Pandora Bytes][Rx] :%s",temp));
	}
	if(mtype == eWrite)
	{
		DBG_MSG(("[Pandora Bytes][Tx] :%s",temp));
		//printf("[[Pandora Bytes] [Tx]  %s \n", temp);
	}
}

static void* runPandoraReaderAudioThread(void *arg)
{
	TRC_SCOPE(PandoraService, CTunerCmdInvoker, AudioThread);
	DBG_MSG(("[CTunerCmdInvoker]:Initializing the Audio Graph inside the thread:Start"));
	CTunerCmdInvoker::initPandoraAudioGraph();
	DBG_MSG(("[CTunerCmdInvoker]:Initializing the Audio Graph inside the thread:End"));
}

bool CTunerCmdInvoker::initPandoraAudioGraph()
{
	//delay(500);
	bool minitAudioGraph = false;
	TRC_SCOPE(PandoraService, CTunerCmdInvoker, initAudioGraph);
	DBG_MSG(("[CTunerCmdInvoker] :initPandoraAudioGraph:Start"));
	if(CTunerCmdInvoker::mGraphid != -1)
	{
		CTunerCmdInvoker::SessionTerminated = 1;
		int rs =  iom_stop(CTunerCmdInvoker::mGraphid);
		DBG_MSG(("[PANDORA]:Graph Stop initPandoraAudioGraph: return value : %d", rs));
		rs = iom_destroy_graph(CTunerCmdInvoker::mGraphid);
		DBG_MSG(("[PANDORA]:Graph destroyed initPandoraAudioGraph: return value : %d", rs));
		if(rs == 0){
			CTunerCmdInvoker::mGraphid = -1;
		}
	}
	pthread_mutex_lock( &mPandoraGraphThreadMutex );
	CTunerCmdInvoker::mGraphid = iom_create_graph( "/dev/io-media/graphs/trackplayer", "PandoraAudio", 0);
	pthread_mutex_unlock( &mPandoraGraphThreadMutex);

	int fh = open("/fs/avrcp00/.FS_info./audio",O_RDONLY);
	if (fh < 0)
	{
		DBG_ERROR(("MediaMirrorLink::  failed errno: %s",  strerror(errno)));
		/*if (true == mPandoraAudioTimer->isActive ())
		{
			mPandoraAudioTimer->stopTimer();
			DBG_MSG(("[CTunerCmdInvoker]: mPandoraAudioTimer isActive"));
		}
		mPandoraAudioTimer->startTimer(3000, true);*/
	}
	else
	{
		DBG_MSG(("MediaMirrorLink:: succeeded fh::%d",fh));
	//}
		char tmp[100];// = (char*)malloc(sizeof(char)*100);
		int ret = devctl(fh, DCMD_MEDIA_URL, (void*)tmp, sizeof(tmp) - 1, NULL);
		DBG_MSG(("[CTunerCmdInvoker] :devctl:tmp: %s", (char*)tmp));
		string inputURL = tmp;
		close(fh);
		if (ret != EOK)
		{
			DBG_ERROR(("MediaMirrorLink:: devctl failed errno: %s",strerror(errno)));

		}

		DBG_MSG(("[CTunerCmdInvoker] :File descriptor value mGraphid: %d", CTunerCmdInvoker::mGraphid));
		if(CTunerCmdInvoker::mGraphid > 0)
		{
			DBG_MSG(("[CTunerCmdInvoker] :Audio Graph is created successfully"));
			int rc = iom_attach_output( CTunerCmdInvoker::mGraphid, "snd:/dev/snd/pcmC0D0p", (IOM_OF_AUDIO) );
			DBG_MSG(("[CTunerCmdInvoker] :iom attach output%d", rc));

			rc = iom_attach_input(CTunerCmdInvoker::mGraphid, "wms:/fs/avrcp00/.FS_info./audio", 1);
			DBG_MSG(("[CTunerCmdInvoker] :input URL Path %s", inputURL.c_str()));
			DBG_MSG(("[CTunerCmdInvoker] :iom attach input%d", rc));
			DBG_ERROR(("MediaMirrorLink::  iom attach input: %s",  strerror(errno)));
			rc = iom_play(CTunerCmdInvoker::mGraphid);
			DBG_MSG(("[CTunerCmdInvoker] :iom play %d", rc));
			CTunerCmdInvoker::SessionTerminated = 0;
		}
		DBG_MSG(("[CTunerCmdInvoker] :initPandoraAudioGraph:End"));
		/*if(CTunerCmdInvoker::setPandoraAudioAgainFlag == true)
		{
			DBG_MSG(("[CTunerCmdInvoker] :setPandoraAudioAgainFlag: %d", setPandoraAudioAgainFlag));
			CTunerCmdInvoker::setPandoraAudioAgainFlag = false;
			MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_Set_Audio, 0);
		}*/
	}
	return minitAudioGraph;
}

CPandoraProtocolMgr* CTunerCmdInvoker::GetInstanceProtocolManager(void)
{
	if(NULL == vPandoraProtocolMgr)
	{
		IPandoraProtocolListener *listener = new IPandoraListenerImpl;
		CPandoraProtocolMgr* vPandoraProtocolMgr = new CPandoraProtocolMgr(listener);
	}
	return vPandoraProtocolMgr;
}

void CTunerCmdInvoker::updateBrandingImage()
{
	TRC_SCOPE(PandoraService, CTunerCmdInvoker, updateBrandingImage);
        DBG_MSG(("[CTunerCmdInvoker]:Method updateBrandingImage invoked "));
	//unsigned int BufLegth = 8192;//Should be multiple of 1K bytes, Max 8K can be sent in a msg
    unsigned int BufLegth = 1002;
	CPandoraMessage *msgBrImg = new CPandoraMessage;
	msgBrImg->updateBrandingImage(BufLegth);
	CPandoraMessageSenderImpl::addSendQueue(msgBrImg);
	MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
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

pandoraReqTypes CTunerCmdInvoker::getEnumFromMethod(char* method)
{
	TRC_SCOPE(PandoraService, CTunerCmdInvoker, getEnumFromMethod);
	DBG_MSG(("[CTunerCmdInvoker]:Method value in getEnumFromMethod is : %s",method));

	pandoraReqTypes request = Pandora_Req_Invalid;
	if(strcmp(method,"reqStartSession") == 0)
	{
		request = Pandora_Start_Session;
	}
	else if(strcmp(method,"reqTerminateSession") == 0)
	{
		request = Pandora_Terminate_Session;
	}
	else if(strcmp(method,"reqSetElapsedpolling") == 0)
	{
		request = Pandora_Set_ElapsedPolling;
	}
	else if(strcmp(method,"getStatus") == 0)
	{
		request = Pandora_Get_Status;
	}
	else if(strcmp(method,"reqPlayTrack") == 0)
	{
		request = Pandora_Play_Track;
	}
	else if(strcmp(method,"reqPauseTrack") == 0)
	{
		request = Pandora_Pause_Track;
	}
	else if(strcmp(method,"reqSkipTrack") == 0)
	{
		request = Pandora_Skip_Track;
	}
	else if(strcmp(method,"reqTrackRatePositive") == 0)
	{
		request = Pandora_TrackRate_Positive;
	}
	else if(strcmp(method,"reqTrackRateNegative") == 0)
	{
		request = Pandora_TrackRate_Negative;
	}
	else if(strcmp(method,"reqTrackRateNegative") == 0)
	{
		request = Pandora_TrackRate_Negative;
	}
	else if(strcmp(method,"getTrackinfoExtended") == 0)
	{
		request = Pandora_TrackInfo_Extended;
	}
	else if(strcmp(method,"reqGetStationList") == 0)
	{
		request = Pandora_Get_StationList;
	}
	else if(strcmp(method,"reqSelectStation") == 0)
	{
		request = Pandora_Select_Station;
	}
	else if(strcmp(method, "reqGetTrackAlbumArt") == 0)
	{
		request = Pandora_AlBum_Art;
	}
	else if(strcmp(method, "reqBookMarkCurrentArtist") == 0)
	{
		request = Pandora_BookMark_Current_Artist;
	}
	else if(strcmp(method, "reqBookMarkCurrentTrack") == 0)
	{
		request = Pandora_BookMark_Current_Track;
	}
	else if(strcmp(method, "reqGetStationCount") == 0)
	{
		request = Pandora_Get_Station_Count;
	}
	else if(strcmp(method, "reqGetStationToken") == 0)
	{
		request = Pandora_Get_Station_Tokens;
	}
	else if(strcmp(method, "reqGetStationInfo") == 0)
	{
		request = Pandora_Get_Station_Info;
	}
	else if(strcmp(method, "SetPandoraAudio") == 0)
	{
		request = Pandora_Set_Audio;
	}
	else if(strcmp(method, "RemovePandoraAudio") == 0)
	{
		request = Pandora_Remove_Audio;
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

	TRC_SCOPE(PandoraService, CTunerCmdInvoker, handleTunerRequest);
	//DBG_MSG(("[CTunerCmdInvoker]:handleTunerRequest"));

	pandoraReqTypes pandoraReq = Pandora_Req_Invalid;
	pandoraReq = getEnumFromMethod((char*)(reqCommand->getMethod()));
	//DBG_MSG(("[CTunerCmdInvoker]:pandoraReq %d", pandoraReq));
	switch(pandoraReq)
	{
	case Pandora_Start_Session:
	{
		IIpodNotificationImpl::mRequestAcceptSession = false;
		CTunerCmdInvoker::sTerminateSession = eRequestNone;
		CTunerCmdInvoker::mPauseSession = eRequestNone;
		CTunerRequestResponseHandler::mPndrStatusInvalidLogin = false;
		CTunerRequestResponseHandler::mPndrReqPlayTrack = true;
		CTunerRequestResponseHandler::mPandoraSessionStartupFunctionCalls = true;
		CTunerRequestResponseHandler::mPandoraAvoidIpodPauseStatus = true;
		CTunerRequestResponseHandler::pandoraSessionConnectedDevice = 0;
		CTunerRequestResponseHandler::mPndrStartSessionReqRcvd = true;
		CTunerRequestResponseHandler::segmentsCount = 0;
		CTunerCmdInvoker::mIpodDeviceSupport = false;
		CTunerRequestResponseHandler::mPandoraNoActiveStation = false;
		CTunerRequestResponseHandler::mStationTokenLists = true;
		CTunerRequestResponseHandler::mIgnoreTerminateSessionCmd = false;
		CTunerRequestResponseHandler::mIgnoreTerminateSessionCmdOnLaunchAppFailure = false;
		if((CTunerRequestResponseHandler::mIpodDeviceConnected == true))
		{
			if(IIpodNotificationImpl::mSessionId == -1)
			{
				std::string vApplicationID = "com.pandora";
				DBG_MSG(("[CTunerCmdInvoker]:requestIpodLaunchApp"));
				if( Iap1_Types::OK ==  vCIap1MgrController->requestIpodLaunchApp(vApplicationID))
				{
					CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
					DBG_MSG(("[CTunerCmdInvoker]:requestIpodLaunchApp:Success"));
					CTunerCmdInvoker::mIpodDeviceSupport = true;
					CTunerRequestResponseHandler::pandoraSessionConnectedDevice = PANDORA_SESSION_IPOD;
					CPandoraMessage *msg = new CPandoraMessage;
					pndr_connect_params connect;
					connect.api_version = 3;
					strcpy (connect.accessory_id, "4588D35A");
					connect.album_art_dimension = 218;
					connect.album_art_type = 1;
					connect.station_art_dimension = 75;
					connect.flags = 0x00;
					vPandoraProtocolMgr->initialize();
					msg->startPandoraSession(connect);
					CPandoraMessageSenderImpl::addSendQueue(msg);
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
					// CPandoraMessageSenderImpl::sendData();
					/*CPandoraMessage *msgElp = new CPandoraMessage;
					msgElp->setTrackElapsedPolling(1);
					CPandoraMessageSenderImpl::addSendQueue(msgElp);
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);*/

					/*CPandoraMessage *msgAS = new CPandoraMessage;
					msgAS->getStationActive();
					CPandoraMessageSenderImpl::addSendQueue(msgAS);
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
					updateBrandingImage();*/
				}
				else
				{
						DBG_MSG(("[CTunerCmdInvoker]:handleTunerRequest: IPOD Device Does not Support Pandora"));
						CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Error);
						//////////////////////////////Workaround for IOS10//////
						CTunerRequestResponseHandler::mIgnoreTerminateSessionCmdOnLaunchAppFailure = true;
						CTunerRequestResponseHandler::pandoraSessionConnectedDevice = PANDORA_SESSION_IPOD;
						CTunerCmdInvoker::mIpodDeviceSupport = true;
						CPandoraMessage *msg = new CPandoraMessage;
						pndr_connect_params connect;
						connect.api_version = 3;
						strcpy (connect.accessory_id, "4588D35A");
						connect.album_art_dimension = 218;
						connect.album_art_type = 1;
						connect.station_art_dimension = 75;
						connect.flags = 0x00;
						vPandoraProtocolMgr->initialize();
						msg->startPandoraSession(connect);
						CPandoraMessageSenderImpl::addSendQueue(msg);
						MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
						//////////////////////////////
						/*Json::Value dataAppAvl;
						std::string signalAppAvl = "PandoraReadyState";
						dataAppAvl["appStatus"] = PANDORA_APP_AVAILABLE;
						dataAppAvl["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
						dataAppAvl["value"] = false;
						CTunerRequestResponseHandler::handleNotif(signalAppAvl, dataAppAvl);*/
						//CTunerRequestResponseHandler::profileDisconnectMethodCall();
				}
			}
			else
			{
				CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
				DBG_MSG(("[CTunerCmdInvoker]: Session is already launched, therefore sending startSession to IpodDevice"));
				CTunerRequestResponseHandler::pandoraSessionConnectedDevice = PANDORA_SESSION_IPOD;
				vPandoraProtocolMgr->initialize();
				//DBG_MSG(("[CTunerCmdInvoker]:Pandora_Start_Session"));
				CPandoraMessage *msg = new CPandoraMessage;
				pndr_connect_params connect;
				connect.api_version = 3;
				strcpy (connect.accessory_id, "4588D35A");
				connect.album_art_dimension = 218;
				connect.album_art_type = 1;
				connect.station_art_dimension = 75;
				connect.flags = 0x00;

				msg->startPandoraSession(connect);
				if(msg == NULL)
				{
					DBG_MSG(("[IIpodNotificationImpl]:responseAcceptSession:: :Message is NULL in StartSession"));
					cout << "Message is NULL in StartSession" << endl;
				}
				CPandoraMessageSenderImpl::addSendQueue(msg);
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
				//CPandoraMessageSenderImpl::sendData();
				/*CPandoraMessage *msgElp = new CPandoraMessage;
				msgElp->setTrackElapsedPolling(1);
				CPandoraMessageSenderImpl::addSendQueue(msgElp);
				int priority = getprio(0);
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, priority, Pandora_New_Message_Received, 0);*/

				/*CPandoraMessage *msgAS = new CPandoraMessage;
				msgAS->getStationActive();
				CPandoraMessageSenderImpl::addSendQueue(msgAS);
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
				updateBrandingImage();*/
			}
		}
		else if(CTunerRequestResponseHandler::mAndroidDeviceConnected == true )
		{
			CTunerCmdInvoker::mPandoraDeviceFd  = open("/dev/SPP_PANDORA",O_RDWR);
			if(-1== CTunerCmdInvoker::mPandoraDeviceFd)
			{
				DBG_MSG(("[CTunerCmdInvoker]:open /dev/SPP_PANDORA failed"));
			}
			else
			{
				//CTunerCmdInvoker::SessionTerminated = 0;
				CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
				CTunerRequestResponseHandler::pandoraSessionConnectedDevice = PANDORA_SESSION_ANDROID;
				vPandoraProtocolMgr->initialize();
				DBG_MSG(("[CTunerCmdInvoker]:Pandora_Start_Session"));
				CPandoraMessage *msg = new CPandoraMessage;
				pndr_connect_params connect;
				connect.api_version = 3;
				strcpy (connect.accessory_id, "4588D35A");
				connect.album_art_dimension = 218;
				connect.album_art_type = 1;
				connect.station_art_dimension = 75;
				connect.flags = 0x00;

				msg->startPandoraSession(connect);
				CPandoraMessageSenderImpl::addSendQueue(msg);
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
				// CPandoraMessageSenderImpl::sendData();

				/*CPandoraMessage *msgElp = new CPandoraMessage;
				msgElp->setTrackElapsedPolling(1);
				CPandoraMessageSenderImpl::addSendQueue(msgElp);
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);*/

				/*CPandoraMessage *msgAS = new CPandoraMessage;
				msgAS->getStationActive();
				CPandoraMessageSenderImpl::addSendQueue(msgAS);
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
				updateBrandingImage();*/
			}
		}
		else if(CTunerRequestResponseHandler::mIphoneBluetooth == true)
		{
			DBG_MSG(("[CTunerCmdInvoker]:mIphoneBluetooth: StartSession for Bluetooth"));
			if(IIpodNotificationImpl::mSessionId == -1)
			{
				std::string vApplicationID = "com.pandora";
				DBG_MSG(("[CTunerCmdInvoker]:requestIpodLaunchApp"));
				if( Iap1_Types::OK ==  vCIap1MgrController->requestIpodLaunchApp(vApplicationID))
				{
					// std::string mlistener = "/dev/snd/pcmC0D0p";
					//sleep(2);
					// vCIap1MgrController->requestIpodStartAudioGraph(mlistener,Iap1_Types::USBDevice);
					//CTunerRequestResponseHandler::mIpodDeviceConnected = false;
					CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
					DBG_MSG(("[CTunerCmdInvoker]:requestIpodLaunchApp:Success"));
					CTunerRequestResponseHandler::pandoraSessionConnectedDevice = PANDORA_SESSION_IPHONE;
					CTunerCmdInvoker::mIpodDeviceSupport = true;
					CPandoraMessage *msg = new CPandoraMessage;
					pndr_connect_params connect;
					connect.api_version = 3;
					strcpy (connect.accessory_id, "4588D35A");
					connect.album_art_dimension = 218;
					connect.album_art_type = 1;
					connect.station_art_dimension = 75;
					connect.flags = 0x00;
					vPandoraProtocolMgr->initialize();
					msg->startPandoraSession(connect);
					CPandoraMessageSenderImpl::addSendQueue(msg);
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
					//CPandoraMessageSenderImpl::sendData();
					/*CPandoraMessage *msgElp = new CPandoraMessage;
					msgElp->setTrackElapsedPolling(1);
					CPandoraMessageSenderImpl::addSendQueue(msgElp);
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);*/

					/*CPandoraMessage *msgAS = new CPandoraMessage;
					msgAS->getStationActive();
					CPandoraMessageSenderImpl::addSendQueue(msgAS);
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
					updateBrandingImage();*/
				}
				else
				{
						DBG_MSG(("[CTunerCmdInvoker]:handleTunerRequest: IPhone Device Does not Support Pandora"));
						CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Error);
						//////////////////////////////Workaround for IOS10//////
						CTunerRequestResponseHandler::mIgnoreTerminateSessionCmdOnLaunchAppFailure = true;
						CTunerRequestResponseHandler::pandoraSessionConnectedDevice = PANDORA_SESSION_IPHONE;
						CTunerCmdInvoker::mIpodDeviceSupport = true;
						CPandoraMessage *msg = new CPandoraMessage;
						pndr_connect_params connect;
						connect.api_version = 3;
						strcpy (connect.accessory_id, "4588D35A");
						connect.album_art_dimension = 218;
						connect.album_art_type = 1;
						connect.station_art_dimension = 75;
						connect.flags = 0x00;
						vPandoraProtocolMgr->initialize();
						msg->startPandoraSession(connect);
						CPandoraMessageSenderImpl::addSendQueue(msg);
						MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
						//////////////////////////////
						/*Json::Value dataAppAvl;
						std::string signalAppAvl = "PandoraReadyState";
						dataAppAvl["appStatus"] = PANDORA_APP_AVAILABLE;
						dataAppAvl["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
						dataAppAvl["value"] = false;
						CTunerRequestResponseHandler::handleNotif(signalAppAvl, dataAppAvl);*/

						//CTunerRequestResponseHandler::profileDisconnectMethodCall();

						//CTunerRequestResponseHandler::profileConnectMethodCall();
					}
				}
			else
			{
				CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
				DBG_MSG(("[CTunerCmdInvoker]: Session is already launched, therefore sending startSession to iphone"));
				CTunerRequestResponseHandler::pandoraSessionConnectedDevice = PANDORA_SESSION_IPHONE;
				vPandoraProtocolMgr->initialize();

				CPandoraMessage *msg = new CPandoraMessage;
				pndr_connect_params connect;
				connect.api_version = 3;
				strcpy (connect.accessory_id, "4588D35A");
				connect.album_art_dimension = 218;
				connect.album_art_type = 1;
				connect.station_art_dimension = 75;
				connect.flags = 0x00;

				msg->startPandoraSession(connect);
				CPandoraMessageSenderImpl::addSendQueue(msg);
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
				//CPandoraMessageSenderImpl::sendData();
				/*CPandoraMessage *msgElp = new CPandoraMessage;
				msgElp->setTrackElapsedPolling(1);
				CPandoraMessageSenderImpl::addSendQueue(msgElp);
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);*/

				/*CPandoraMessage *msgAS = new CPandoraMessage;
				msgAS->getStationActive();
				CPandoraMessageSenderImpl::addSendQueue(msgAS);
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
				updateBrandingImage();*/
			}
		}
		if(CTunerRequestResponseHandler::setPandoraAudioAutoRecovery == true)
		{
			DBG_MSG(("[CTunerCmdInvoker]: setPandoraAudioAutoRecovery:%d, mPndrStatusInvalidLogin:%d",CTunerRequestResponseHandler::setPandoraAudioAutoRecovery,
																									CTunerRequestResponseHandler::mPndrStatusInvalidLogin));
			CTunerRequestResponseHandler::setPandoraAudioAutoRecovery = false;
			if(CTunerRequestResponseHandler::mPndrStatusInvalidLogin == false)
			{
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_Set_Audio, 0);
			}
		}
		break;
	}
	case Pandora_Terminate_Session:
	{
		CTunerRequestResponseHandler::mPndrStartSessionReqRcvd = false;
		DBG_MSG(("[CTunerCmdInvoker]:Pandora_Terminate_Session"));
		DBG_MSG(("[CTunerCmdInvoker]:meSessionState:%d, mIgnoreTerminateSessionCmd:%d, mIgnoreTerminateSessionCmdOnLaunchAppFailure:%d",CTunerCmdInvoker::meSessionState,CTunerRequestResponseHandler::mIgnoreTerminateSessionCmd,CTunerRequestResponseHandler::mIgnoreTerminateSessionCmdOnLaunchAppFailure));

		if( (CTunerRequestResponseHandler::mIgnoreTerminateSessionCmd == false) || (CTunerRequestResponseHandler::mIgnoreTerminateSessionCmdOnLaunchAppFailure == false) )
		{
			Json::Value data;
			if(CTunerRequestResponseHandler::setPandoraAudioAutoRecovery == false)
			{
				if((CTunerRequestResponseHandler::mIpodDeviceConnected == true))
				{
					DBG_MSG(("[CTunerCmdInvoker]:Ipod device connected"));
					DBG_MSG(("[CTunerCmdInvoker]IIpodNotificationImpl::mSessionId:%d,CTunerCmdInvoker::meSessionState:%d",IIpodNotificationImpl::mSessionId,CTunerCmdInvoker::meSessionState));
					//if((IIpodNotificationImpl::mSessionId != -1) && (CTunerCmdInvoker::meSessionState == eSessionActive))
					CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
					if(IIpodNotificationImpl::mSessionId != -1)
					{
						CPandoraMessage *Nxtmsg = new CPandoraMessage;
						Nxtmsg->terminatePandoraSession();
						CPandoraMessageSenderImpl::addSendQueue(Nxtmsg);
						DBG_MSG(("[CTunerCmdInvoker]:Nxtmsg terminatePandoraSession"));
						MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
						//vPandoraProtocolMgr->deInitialize();
					}
					else
					{
						DBG_MSG(("[CTunerCmdInvoker]:Rejected Terminate Session request"));
						CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Error);
					}
				}
				else if((CTunerRequestResponseHandler::mAndroidDeviceConnected == true) && (CTunerCmdInvoker::meSessionState == eSessionActive)
						&& (CTunerRequestResponseHandler::pandoraSessionConnectedDevice == PANDORA_SESSION_ANDROID) )
				{
					DBG_MSG(("[CTunerCmdInvoker]:Android Device Connected and Session is also Active"));
					DBG_MSG(("[CTunerCmdInvoker]:Setting the flag true"));
					CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
					CPandoraMessage *Nxtmsg = new CPandoraMessage;
					Nxtmsg->terminatePandoraSession();
					CPandoraMessageSenderImpl::addSendQueue(Nxtmsg);
					DBG_MSG(("[CTunerCmdInvoker]:Nxtmsg terminatePandoraSession"));
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
					//vPandoraProtocolMgr->deInitialize();
				}
				else if( (CTunerRequestResponseHandler::mIphoneBluetooth == true) && (CTunerCmdInvoker::meSessionState == eSessionActive)
						&& (CTunerRequestResponseHandler::pandoraSessionConnectedDevice == PANDORA_SESSION_IPHONE) )
				{
					DBG_MSG(("[CTunerCmdInvoker]:Iphone Device Connected and Session is also Active"));
					DBG_MSG(("[CTunerCmdInvoker]:Setting the flag true"));
					CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
					CTunerCmdInvoker::mPauseSession = eRequestReceived;
					CPandoraMessage *Nxtmsg = new CPandoraMessage;
					Nxtmsg->terminatePandoraSession();
					CPandoraMessageSenderImpl::addSendQueue(Nxtmsg);
					DBG_MSG(("[CTunerCmdInvoker]:Nxtmsg terminatePandoraSession"));
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
					//vPandoraProtocolMgr->deInitialize();
				}
				else
				{
					DBG_MSG(("[CTunerCmdInvoker]:Neither Android not iPhone Connected"));
					CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Invalid);
					if( (CTunerRequestResponseHandler::mStationTokenLists == false) && ( (CTunerRequestResponseHandler::mIpodDeviceConnected == true) ||
							(CTunerRequestResponseHandler::mAndroidDeviceConnected == true) || (CTunerRequestResponseHandler::mIphoneBluetooth == true)	) )
					{
						CPandoraMessage *Nxtmsg = new CPandoraMessage;
						Nxtmsg->terminatePandoraSession();
						CPandoraMessageSenderImpl::addSendQueue(Nxtmsg);
						DBG_MSG(("[CTunerCmdInvoker]:Nxtmsg terminatePandoraSession mStationTokenLists : %d",CTunerRequestResponseHandler::mStationTokenLists));
						MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
					}
				}
			}
			/*if (true == mPandoraAudioTimer->isActive ())
			{
				mPandoraAudioTimer->stopTimer();
				DBG_MSG(("[CTunerCmdInvoker]: mPandoraAudioTimer isActive chk"));
			}*/
			MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_Remove_Audio, 0);
		}
		else
		{
			DBG_MSG(("[CTunerCmdInvoker]: mIgnoreTerminateSessionCmd : %d",CTunerRequestResponseHandler::mIgnoreTerminateSessionCmd));
		}
		break;
	}
	case Pandora_Set_ElapsedPolling:
	{
		DBG_MSG(("[CTunerCmdInvoker]:Pandora_Set_ElapsedPolling"));
		CPandoraMessage *msg = new CPandoraMessage;
		bool mMode =  reqCommand->getParms().get("mode", Json::Value(0)).asBool();
		msg->setTrackElapsedPolling(mMode);
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		break;
	}
	case Pandora_Get_Status:
	{
		DBG_MSG(("[CTunerCmdInvoker]:Pandora_Get_Status"));

		CPandoraMessage *msg = new CPandoraMessage;
		msg->getStatus();
		if(msg == NULL)
		{

		}
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);

		break;
	}
	case Pandora_TrackInfo_Extended:
	{
		//DBG_MSG(("[CTunerCmdInvoker]:Pandora_TrackInfo_Extended"));
		//cout << "[CTunerCmdInvoker]:Pandora_TrackInfo_Extended" << endl;
		CPandoraMessage *msg = new CPandoraMessage;
		msg->getTrackInfoExtended();
		// printData(msg->getData(), msg->getDataSize(), eWrite);
		if(msg == NULL)
		{
			//cout << "Message is NULL in StartSession" << endl;
		}
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		break;
	}
	case Pandora_Select_Station:
	{
		//DBG_MSG(("[CTunerCmdInvoker]:Pandora_Select_Station"));
		//cout << "[CTunerCmdInvoker]:Pandora_TrackInfo_Extended" << endl;
		UINT32 station_token =  reqCommand->getParms().get("station_token", Json::Value(0)).asUInt();
		DBG_MSG(("[CTunerCmdInvoker]:Pandora Station Token %d", station_token));
		if(station_token)
		{
			CPandoraMessage *msg = new CPandoraMessage;
			msg->eventStationSelect(station_token);
			CPandoraMessageSenderImpl::addSendQueue(msg);
			// Inform Pandora Worker thread to process the New Request
			MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		}
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		break;
	}
	case Pandora_Play_Track:
	{
		//DBG_MSG(("[CTunerCmdInvoker]:Pandora_Play_Track MsgConnectionId:%d",CTunerRequestResponseHandler::MsgConnectionId));
		//cout << "[CTunerCmdInvoker]:Pandora_Play_Track" << endl;
		CPandoraMessage *msg = new CPandoraMessage;
		msg->eventPlayTrack();
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		break;
	}
	case Pandora_Pause_Track:
	{
		//DBG_MSG(("[CTunerCmdInvoker]:Pandora_Pause_Track MsgConnectionId:%d",CTunerRequestResponseHandler::MsgConnectionId));
		CTunerRequestResponseHandler::mPandoraAvoidIpodPauseStatus = false;
		CTunerRequestResponseHandler::mPndrReqPlayTrack = false;
		CPandoraMessage *msg = new CPandoraMessage;
		msg->eventPauseTrack();
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		break;
	}
	case Pandora_TrackRate_Positive:
	{
		DBG_MSG(("[CTunerCmdInvoker]:Pandora_TrackRate_Positive"));
		//cout << "[CTunerCmdInvoker]:Pandora_TrackRate_Positive" << endl;
		CPandoraMessage *msg = new CPandoraMessage;
		msg->eventTrackRatePositive();
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		break;
	}
	case Pandora_TrackRate_Negative:
	{
		DBG_MSG(("[CTunerCmdInvoker]:Pandora_TrackRate_Negative"));
		//cout << "[CTunerCmdInvoker]:Pandora_TrackRate_Negative" << endl;
		CPandoraMessage *msg = new CPandoraMessage;
		msg->eventTrackRateNegative();
		//printData(msg->getData(), msg->getDataSize(), eWrite);
		if(msg == NULL)
		{
			//cout << "Message is NULL in StartSession" << endl;
		}
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		break;
	}

	case Pandora_Skip_Track:
	{
		DBG_MSG(("[CTunerCmdInvoker]:Pandora_TrackRate_Positive"));
		CPandoraMessage *msg = new CPandoraMessage;
		msg->eventSkipTrack();
		CPandoraMessageSenderImpl::addSendQueue(msg);
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		break;
	}
	case Pandora_Get_StationList:
	{
		DBG_MSG(("[CTunerCmdInvoker]:Pandora_Get_StationList"));
		if(CTunerRequestResponseHandler::mStationTokenLists == true)
		{
			CPandoraMessage *msg = new CPandoraMessage;
			msg->getAllStationTokens();
			CPandoraMessageSenderImpl::addSendQueue(msg);
			// Inform Pandora Worker thread to process the New Request
			MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
			CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		}
		else
		{
			DBG_MSG(("[CTunerCmdInvoker]:Pandora_Get_StationList mStationTokenLists:%d",CTunerRequestResponseHandler::mStationTokenLists));
		}

		break;
	}
	case Pandora_AlBum_Art:
	{
		//DBG_MSG(("[CTunerCmdInvoker]:Pandora_AlBum_Art"));
		//CPandoraMessage *msg = new CPandoraMessage;
		//INT32 AlbumArtLength =  reqCommand->getParms().get("AlbumArtLength", Json::Value(0)).asInt();
		//DBG_MSG(("[CTunerCmdInvoker]:AlbumArtLength: %d",AlbumArtLength));
		//AlbumArtLength = 2048;
		//msg->getTrackAlbumArt(AlbumArtLength);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Error);
		//CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		//MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(1), Pandora_New_Message_Received, 0);
		break;
	}
	case Pandora_BookMark_Current_Artist:
	{
		DBG_MSG(("[CTunerCmdInvoker]:Pandora_BookMark_Current_Artist"));
		CPandoraMessage *msg = new CPandoraMessage;

		msg->eventBookMarkCurrentArtist();
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		break;
	}
	case Pandora_BookMark_Current_Track:
	{
		DBG_MSG(("[CTunerCmdInvoker]:Pandora_BookMark_Current_Track"));
		CPandoraMessage *msg = new CPandoraMessage;

		msg->eventBookMarkCurrentTrack();
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		break;
	}
	case Pandora_Get_Station_Count:
	{
		DBG_MSG(("[CTunerCmdInvoker]:Pandora_Get_Station_Count"));
		CPandoraMessage *msg = new CPandoraMessage;
		msg->getStationCount();
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		break;
	}
	case Pandora_Get_Station_Info:
	{
		DBG_MSG(("[CTunerCmdInvoker]:Pandora_Get_Station_Tokens"));
		CPandoraMessage *msg = new CPandoraMessage;
		unsigned int startIndex = reqCommand->getParms().get("startIndex", Json::Value(0)).asUInt();
		unsigned int Count = reqCommand->getParms().get("Count", Json::Value(0)).asUInt();
		msg->getStationTokens(startIndex, Count);
		CPandoraMessageSenderImpl::addSendQueue(msg);
		// Inform Pandora Worker thread to process the New Request
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		break;
	}
	case Pandora_Get_Station_Tokens:
	{
		break;
	}
	case Pandora_Set_Audio:
	{
		if(CTunerRequestResponseHandler::mPndrStatusInvalidLogin == false)
		{
			/*if (true == mPandoraAudioTimer->isActive ())
			{
				mPandoraAudioTimer->stopTimer();
				DBG_MSG(("[CTunerCmdInvoker]: Pandora_Set_Audio mPandoraAudioTimer isActive"));
			}*/
			//CTunerCmdInvoker::initPandoraAudioGraph();
			CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
			MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_Set_Audio, 0);
			//updateBrandingImage();
			//Below is Workaround for sometime track Pause issue fix
			if(CTunerRequestResponseHandler::mIpodDeviceConnected == true)
			{
				CPandoraMessage *msg = new CPandoraMessage;
				msg->eventPlayTrack();
				CPandoraMessageSenderImpl::addSendQueue(msg);
				//CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
				MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
			}
			CTunerCmdInvoker::setPandoraAudioAgainFlagChkOnCmd = true;
		}
		break;
	}

	case Pandora_Remove_Audio:
	{
		CTunerCmdInvoker::setPandoraAudioAgainFlagChkOnCmd = false;
		CTunerRequestResponseHandler::setPandoraAudioAutoRecovery = false;
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_Remove_Audio, 0);
		CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Success);
		break;
	}
	CTunerRequestResponseHandler::sendResponse(reqCommand, Pandora_Response_Invalid_Param);

	}
	return true;
}




//===================================================== End Of File ===================================================
