/*----------------------------------------------------------------------------------------------------------------------
*
*
*
*---------------------------------------------------------------------------------------------------------------------*/


#ifndef PANDORA_IIPOD_NOTIFICATION_HPP_
#define PANDORA_IIPOD_NOTIFICATION_HPP_
#include "CPandoraMessageSenderImpl.hpp"
#include "CPandoraRequestResponseHandler.hpp"
#include "api/mm/iap1devctrl/pf/src/CIAP1_Types.hpp"
#define INVALID_IPOD_INVALID_STATE 100

class IIpodNotificationImpl : public IIPodNotification
{
public:
	   /**
	     *@brief  Constructor
	     *@param
	     *@return
	     */
	IIpodNotificationImpl();

	   /**
	       *@brief    On updation of protocol list
	       *@param1:pnFr->getvalue() will return protocol List (tProtocolList)
	       *@return
	     */
	   void onProtocolListUpdate(Iap1_Types::tProtocolList& protocolList,Iap1_Types::tError& error);
	   /**
	       *@brief    On updation of current session
	       *@param1:pnFr->getvalue()  will return current session ID (tSession)
	       *@return
	     */
	   void onCurrentSessionUpdate(Iap1_Types::tSession& session,Iap1_Types::tError& error);
	   /**
	       *@brief    On updation of app launch allowed
	       *@param1:pnFr->getvalue() will return whether app is launched or not. Returns a boolean value
	       *@return
	     */
	   void onAppLaunchAllowedUpdate(bool& value);
	   /**
	       *@brief    On update the  list of sessionIDs which have data.
	       *@param1:pnFr->getValue() will return DataList(tDataList)
	       *@return
	     */
	   void onDataListUpdate(Iap1_Types::tDataList& datalist);
	   /**
	       *@brief    On Update ipod State
	       *@param1:pnFr->getDeviceState() will return device state(tDevice State)
	       *@return
	     */
	   void oniPodStateUpdate(Iap1_Types::tDeviceState& deviceState);
	   /**
	       *@brief   On update session list
	       *@param1:pnFr->getSessionList() will return session list(tSessionList)
	       *@return
	     */
	   void onSessionListUpdate(Iap1_Types::tSessionList& sessionList);
	   /**
	       *@brief   On update swhether data is available
	       *@param1:pnFr->getValue() will return boolean value
	       *@return
	     */
	   void onDataAvailableUpdate(bool& dataAvailable );
	   /**
	      *@brief   On update of Bundle Name
	      *@param1:BundleId
	      *@return
	      */
	   void onUpdateBundleName(const char* budleName );
	   /**
	      *@brief   On update of App Name
	      *@param1:App Name
	      *@return
	      */
	   void onUpdateAppName(const char* appName);
	   /**
	       *@brief    Response for sending data
	       *@param1:pnFr->getValue() will return error code (tError)
	       *@return
	     */
	   void responseSendData(Iap1_Types::tError& error);
	   /**
	       *@brief    Response for Accept session request
	       *@param1:pnFr->getError() will return error code (tError)
	                      pnFr->getSessionID() will return the  sessionId
	       *@return
	     */
	   void responseAcceptSession(uint32_t& sessionId,Iap1_Types::tError& error);
	   /**
	      *@brief   OResponse for requestRecieveData()
	      *@param1: data content ,length etc
	      @param3 Session Id
	      *@return
	      */
	   void responseReceiveData(const unsigned char* buffer,std::size_t& len,int32_t& sessionId,Iap1_Types::tError& error);
	   /**
	      *@brief   OResponse for closing the session
	      *@param1:tError
	      *@return
	      */
	   void responseCloseSession(Iap1_Types::tError& error);
	   static uint32_t mSessionId;
	   static Iap1_Types::tSessionState mPandoraSessionState;
	   static bool mRequestAcceptSession;
};




























#endif
