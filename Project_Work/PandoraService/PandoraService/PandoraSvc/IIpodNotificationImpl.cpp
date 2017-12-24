/*----------------------------------------------------------------------------------------------------------------------
*
* 
*
*
*---------------------------------------------------------------------------------------------------------------------*/



#include "IIpodNotificationImpl.hpp"
#include "TraceMacros.hpp"
#include "CPandoraCmdInvoker.hpp"
#include "iomedia/api.h"
#define	PANDORA_APP_STATE	 	2
#define PANDORA_SESSION_IPOD	1
#define	PANDORA_SESSION_IPHONE	2
#define	PANDORA_SESSION_ANDROID	3
#define PANDORA_DEVICE_ANDROID  1
#define PANDORA_DEVICE_IPHONE   2
#define PANDORA_DEVICE_IPOD     3

TRC_SCOPE_DEF(TunerService, IIpodNotificationImpl, onCurrentSessionUpdate);
TRC_SCOPE_DEF(TunerService, IIpodNotificationImpl, onDataListUpdate);
TRC_SCOPE_DEF(PandoraService, IIpodNotificationImpl, onSessionListUpdate);
TRC_SCOPE_DEF(PandoraService, IIpodNotificationImpl, responseAcceptSession);
TRC_SCOPE_DEF(PandoraService, IIpodNotificationImpl, responseReceiveData);
TRC_SCOPE_DEF(PandoraService, IIpodNotificationImpl, oniPodStateUpdate);
TRC_SCOPE_DEF(PandoraService, IIpodNotificationImpl, responseSendData);
TRC_SCOPE_DEF(PandoraService, IIpodNotificationImpl, onProtocolListUpdate);
extern CIap1MgrController* vCIap1MgrController;
extern CPandoraProtocolMgr* vPandoraProtocolMgr;
extern CTimer *mPandoraDeviceCheck;
uint32_t IIpodNotificationImpl::mSessionId = -1;
bool IIpodNotificationImpl::mRequestAcceptSession = false;
Iap1_Types::tSessionState IIpodNotificationImpl::mPandoraSessionState = (Iap1_Types::tSessionState)INVALID_IPOD_INVALID_STATE;

	   /**
	     *@brief  Constructor
	     *@param
	     *@return
	     */
	IIpodNotificationImpl::IIpodNotificationImpl()
	{

	}

	   /**
	       *@brief    On updation of protocol list
	       *@param1:pnFr->getvalue() will return protocol List (tProtocolList)
	       *@return
	     */
	   void IIpodNotificationImpl::onProtocolListUpdate(Iap1_Types::tProtocolList& protocolList,Iap1_Types::tError& error)
	   {
		   TRC_SCOPE(PandoraService, IIpodNotificationImpl, onProtocolListUpdate);
		   //DBG_MSG(("[IIpodNotificationImpl]::onProtocolListUpdate"));
		   Iap1_Types::tProtocolList::iterator vprotocolList;
		   for(vprotocolList = protocolList.begin(); vprotocolList != protocolList.end(); vprotocolList++)
		   {
		   //DBG_MSG(("[IIpodNotificationImpl]::ProtocolId %d",vprotocolList->id));
		   //DBG_MSG(("[IIpodNotificationImpl]::State is %d",vprotocolList->state));
		   //DBG_MSG(("[IIpodNotificationImpl]::Protocol name is %s",vprotocolList->dnsString.c_str()));
		   }
	   }
	   /**
	       *@brief    On updation of current session
	       *@param1:pnFr->getvalue()  will return current session ID (tSession)
	       *@return
	     */
	   void IIpodNotificationImpl::onCurrentSessionUpdate(Iap1_Types::tSession& session,Iap1_Types::tError& error)
	   {
		   TRC_SCOPE(TunerService, IIpodNotificationImpl, onCurrentSessionUpdate);
		   //DBG_MSG(("[IIpodNotificationImpl]::onCurrentSessionUpdate"));

		   if (session.sessionId == IIpodNotificationImpl::mSessionId)
		   {
		   //DBG_MSG(("[IIpodNotificationImpl]::onCurrentSessionUpdate: Session is Pandora"));
		   if (Iap1_Types::OK == error)
		   {
			   //DBG_MSG(("[IIpodNotificationImpl]::Iap1_Types::OK "));
              if(session.sessionState == Iap1_Types::SESSION_CLOSING)
              {
            	  //DBG_MSG(("[IIpodNotificationImpl]::Iap1_Types::SESSION_CLOSING "));
            	  //cout << "Current Session is Closed" << endl;
            	  vCIap1MgrController->requestIpodStopAudio();
              }
		   }
		   }
	   }
	   /**
	       *@brief    On updation of app launch allowed
	       *@param1:pnFr->getvalue() will return whether app is launched or not. Returns a boolean value
	       *@return
	     */
	   void IIpodNotificationImpl::onAppLaunchAllowedUpdate(bool& value)
	   {

	   }
	   /**
	       *@brief    On update the  list of sessionIDs which have data.
	       *@param1:pnFr->getValue() will return DataList(tDataList)
	       *@return
	     */
	   void IIpodNotificationImpl::onDataListUpdate(Iap1_Types::tDataList& datalist)
	   {
		   TRC_SCOPE(TunerService, IIpodNotificationImpl, onDataListUpdate);
		   //DBG_MSG(("[IIpodNotificationImpl]::onDataListUpdate"));
		   Iap1_Types::tDataList::iterator vIterator;
		   for (vIterator = datalist.begin(); vIterator != datalist.end(); vIterator++)
		   {
			 if (((Poco::UInt32)IIpodNotificationImpl::mSessionId) == (*vIterator))
			 {
				 //DBG_MSG(("[IIpodNotificationImpl]::onDataListUpdate:: SessionId %d", IIpodNotificationImpl::mSessionId));

				 unsigned int mProtocolId = PROTOCOL_ID;
				   if (Iap1_Types::OK == vCIap1MgrController->requestIpodReceiveData(mProtocolId))
				   {
					           //DBG_MSG(("[IIpodNotificationImpl]::onDataListUpdate:: requestIpodReceiveData Success"));
				   }
				   else
				   {
					   //DBG_MSG(("[IIpodNotificationImpl]::onDataListUpdate:: requestIpodReceiveData Failed"));
				   }
			 }
			 else
			 {
				 cout << "Session Id is not present in iterator" << endl;
			 }
		   }

	   }
	   /**
	       *@brief    On Update ipod State
	       *@param1:pnFr->getDeviceState() will return device state(tDevice State)
	       *@return
	     */
	   void IIpodNotificationImpl::oniPodStateUpdate(Iap1_Types::tDeviceState& deviceState)
	   {

	   }
	   /**
	       *@brief   On update session list
	       *@param1:pnFr->getSessionList() will return session list(tSessionList)
	       *@return
	     */
	   void IIpodNotificationImpl::onSessionListUpdate(Iap1_Types::tSessionList& sessionList)
	   {
		   TRC_SCOPE(PandoraService, IIpodNotificationImpl, onSessionListUpdate);
		   //DBG_MSG(("[IIpodNotificationImpl]::onSessionListUpdate"));
		   Iap1_Types::tSessionList::iterator vIterator;
		   cout << "Number of elements in the SessionList" << sessionList.size() << endl;
		   int index = 0;
		   for (vIterator = sessionList.begin(); vIterator != sessionList.end(); vIterator++)
		   {
			   //DBG_MSG(("[DEBUG] OnSessionList update and the element position is %d",index));
			   //DBG_MSG(("[DEBUG] protocolID %d",vIterator->protocolId));
			   //DBG_MSG(("[DEBUG] SessionID %d",vIterator->sessionId));
			   //DBG_MSG(("[DEBUG] SessionState %d",vIterator->sessionState));

			   //DBG_MSG(("[IIpodNotificationImpl]::protocolId: %d",vIterator->protocolId));
			   if(vIterator->protocolId == PROTOCOL_ID)
			   {
				  // IIpodNotificationImpl::mPandoraSessionState = vIterator->sessionState;
				   //DBG_MSG(("[IIpodNotificationImpl]::sessionState: %d",vIterator->sessionState));
				   /*if(vIterator->sessionState == 2)//SESSION_CLOSING = 2//App kill at Phone check//
				   {
					   Json::Value dataPAS;
					   std::string signalNamePAS = "PandoraReadyState";
					   dataPAS["appStatus"] = PANDORA_APP_STATE;
					   dataPAS["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
					   dataPAS["value"] = false;
					   CTunerRequestResponseHandler::handleNotif(signalNamePAS, dataPAS);
					   //DBG_MSG(("[PANDORA]:IPHONE/IPOD Pandora App State false"));
					   //////////Below is Temp Fix for AutoPandoraChecking////////////
					   if( CTunerRequestResponseHandler::mIpodDeviceConnected == true)
					   {
						   CTunerRequestResponseHandler::mIpodDeviceConnected = false;
						   CTunerRequestResponseHandler::IpodPandoraPlayStatus = 0;
						   if (Iap1_Types::OK == vCIap1MgrController->requestIpodStopDevCtrl())
						   {
							   IIpodNotificationImpl::mSessionId  = -1;
							   Iap1_Types::tError mError =  vCIap1MgrController->requestIpodStopAudio();
							   //DBG_MSG(("[IIpodNotificationImpl]: Stopping the IpodDevice Graph and return value is : %d",mError));
							   vPandoraProtocolMgr->deInitialize();
						   }
					   }
					   if(CTunerRequestResponseHandler::mIphoneBluetooth == true)
					   {
						   CTunerRequestResponseHandler::mIphoneBluetooth = false;
					   }
					   mPandoraDeviceCheck->startTimer(4000, true);
					   //////////////////////////////////////////////////
				   }*/
			   }
			   //cout << "[IIpodNotificationImpl]::protocolId:"
			   if( (vIterator->protocolId == PROTOCOL_ID) && (vIterator->sessionState == Iap1_Types::SESSION_REQUESTED))
			   {
				   DBG_MSG(("[IIpodNotificationImpl]::sessionState: %d",vIterator->sessionState));
				   //cout << "[IIpodNotificationImpl]::sessionState: " << (int)vIterator->sessionState << endl;
				  // cout << "Session ID " << vIterator->sessionId << endl;
				   vPandoraProtocolMgr->setDebugLevel(0);
				   if (Iap1_Types::OK == vCIap1MgrController->requestIpodAcceptSession(vIterator->protocolId))
						   {
					   //DBG_MSG(("[IIpodNotificationImpl]::requestIpodAcceptSession: Success"));
					  // IIpodNotificationImpl::mRequestAcceptSession = true;
						   }
				   else
				   {
					   //DBG_MSG(("[IIpodNotificationImpl]::requestIpodAcceptSession: Failed"));
				   }
			   }

			   if(vIterator->sessionState == Iap1_Types::SESSION_CLOSING)
			   {
				   //DBG_MSG(("[DEBUG] protocolID %d",vIterator->protocolId));
				   //DBG_MSG(("[DEBUG] SessionID %d",vIterator->sessionId));
				   //DBG_MSG(("[DEBUG] SessionState %d",vIterator->sessionState));
				   if (vIterator->protocolId == PROTOCOL_ID)
				   {
					   IIpodNotificationImpl::mSessionId = -1;
						vPandoraProtocolMgr->deInitialize();
						////////////////////
						/*if(CTunerCmdInvoker::mGraphid != -1)
						{
							int rs =  iom_stop(CTunerCmdInvoker::mGraphid);
							//DBG_MSG(("[PANDORA]:Graph Stop initPandoraAudioGraph: return value : %d", rs));
							rs = iom_destroy_graph(CTunerCmdInvoker::mGraphid);
							//DBG_MSG(("[PANDORA]:Graph destroyed initPandoraAudioGraph: return value : %d", rs));
							if(rs == 0){
								CTunerCmdInvoker::mGraphid = -1;
							}
						}*/
						////////////////////
						Json::Value data;
						//DBG_MSG(("[IPandoraListenerImpl]: Session Terminated: InforUpdateStationActive"));
						std::string signalName = "InfoUpdateStationActive";
						//CTunerCmdInvoker::mStationActive = false;
						CTunerCmdInvoker::meSessionState = eSessionNone;
						//DBG_MSG(("[IPandoraListenerImpl]:CTunerCmdInvoker::meSessionState %d ", CTunerCmdInvoker::meSessionState));
						//CTunerCmdInvoker::mStationActive == false;
						data["StationToken"] = 0;
						data["value"] = false;
						CTunerRequestResponseHandler::handleNotif(signalName, data);
						//if(CTunerRequestResponseHandler::mIphoneBluetooth == true)
					    //{
						   //CTunerRequestResponseHandler::mIphoneBluetooth = false;
						   //mPandoraDeviceCheck->startTimer(3000, true);
							//CTunerRequestResponseHandler::mpInstance->PandoraSocketDisconnect();
							Json::Value dataPAS;
							std::string signalNamePAS = "PandoraReadyState";
							dataPAS["appStatus"] = PANDORA_APP_STATE;
							dataPAS["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							dataPAS["value"] = false;
							CTunerRequestResponseHandler::handleNotif(signalNamePAS, dataPAS);
							//DBG_MSG(("[PANDORA]:IPHONE/IPOD Pandora App State false"));

							Json::Value dataPAS2;
							std::string signalNamePAS2 = "PandoraReadyState";
							dataPAS2["appStatus"] = PANDORA_APP_STATE;
							dataPAS2["device"] = CTunerRequestResponseHandler::pandoraConnectedDeviceName;
							dataPAS2["value"] = true;
							CTunerRequestResponseHandler::handleNotif(signalNamePAS2, dataPAS2);
							//DBG_MSG(("[PANDORA]:IPHONE/IPOD Pandora App State true"));
					    //}
				   }
				   //CTunerCmdInvoker::mIpodDeviceSupport = false;
				   vCIap1MgrController->requestIpodCloseSession(vIterator->sessionId);
			   }
			   ++index;
		   }
	   }
	   /**
	       *@brief   On update swhether data is available
	       *@param1:pnFr->getValue() will return boolean value
	       *@return
	     */
	   void IIpodNotificationImpl::onDataAvailableUpdate(bool& dataAvailable )
	   {

	   }
	   /**
	      *@brief   On update of Bundle Name
	      *@param1:BundleId
	      *@return
	      */
	   void IIpodNotificationImpl::onUpdateBundleName(const char* budleName )
	   {

	   }
	   /**
	      *@brief   On update of App Name
	      *@param1:App Name
	      *@return
	      */
	   void IIpodNotificationImpl::onUpdateAppName(const char* appName)
	   {

	   }
	   /**
	       *@brief    Response for sending data
	       *@param1:pnFr->getValue() will return error code (tError)
	       *@return
	     */
	   void IIpodNotificationImpl::responseSendData(Iap1_Types::tError& error)
	   {
		   TRC_SCOPE(PandoraService, IIpodNotificationImpl, responseSendData);
		   //DBG_MSG(("[IIpodNotificationImpl]:responseSendData:: : Iap1_Types::OK"));
	   }
	   /**
	       *@brief    Response for Accept session request
	       *@param1:pnFr->getError() will return error code (tError)
	                      pnFr->getSessionID() will return the  sessionId
	       *@return
	     */
	   void IIpodNotificationImpl::responseAcceptSession(uint32_t& sessionId,Iap1_Types::tError& error)
	   {
		   TRC_SCOPE(PandoraService, IIpodNotificationImpl, responseAcceptSession);
		   if (Iap1_Types::OK == error)
		   {
			   //DBG_MSG(("[IIpodNotificationImpl]:responseAcceptSession:: : Iap1_Types::OK"));
			   //DBG_MSG(("[IIpodNotificationImpl]:responseAcceptSession:: : SessionId from Library %d", sessionId));
			   //DBG_MSG(("[IIpodNotificationImpl]:responseAcceptSession:: : SessionId %d", IIpodNotificationImpl::mSessionId));

			   IIpodNotificationImpl::mSessionId = sessionId;
			   if(CTunerCmdInvoker::mIpodDeviceSupport == true)
               {
				    //CTunerCmdInvoker::mIpodDeviceSupport = true;
				    CTunerCmdInvoker::mIpodDeviceSupport = false;
					CPandoraMessage *msg = new CPandoraMessage;
					pndr_connect_params connect;
					connect.api_version = 3;
					strcpy (connect.accessory_id, "4588D35A");
					connect.album_art_dimension = 218;
					connect.album_art_type = 1;
					connect.station_art_dimension = 75;
					connect.flags = 0x00;
					vPandoraProtocolMgr->initialize();
					//DBG_MSG(("[IIpodNotificationImpl]::Calling Start Session"));
					msg->startPandoraSession(connect);
					CPandoraMessageSenderImpl::addSendQueue(msg);
					MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, getprio(0), Pandora_New_Message_Received, 0);
					//DBG_MSG(("[IIpodNotificationImpl]:pandoraSessionConnectedDevice:%d", CTunerRequestResponseHandler::pandoraSessionConnectedDevice));
			   }
		   }
	   }
	   /**
	      *@brief   OResponse for requestRecieveData()
	      *@param1: data content ,length etc
	      @param3 Session Id
	      *@return
	      */
	   void IIpodNotificationImpl::responseReceiveData(const unsigned char* buffer,std::size_t& len,int32_t& sessionId,Iap1_Types::tError& error)
	   {
		   TRC_SCOPE(PandoraService, IIpodNotificationImpl, responseReceiveData);
		   //DBG_MSG(("[IIpodNotificationImpl]::responseReceiveData"));
		   if (Iap1_Types::OK == error)
		   {
			   //DBG_MSG(("[IIpodNotificationImpl]::responseReceiveData::Iap1_Types::OK"));
			   if (sessionId == IIpodNotificationImpl::mSessionId)
			   {
				   //DBG_MSG(("[IIpodNotificationImpl]::responseReceiveData::Session Id: %d", sessionId));
				   //CTunerCmdInvoker::GetInstance()->printData(buffer, len, eRead);
				   vPandoraProtocolMgr->processDataReceived(buffer, len);
			   }
		   }
	   }
	   /**
	      *@brief   OResponse for closing the session
	      *@param1:tError
	      *@return
	      */
	   void IIpodNotificationImpl::responseCloseSession(Iap1_Types::tError& error)
	   {

	   }
