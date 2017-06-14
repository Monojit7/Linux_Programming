#ifndef TUNER_CMDINVOKER_HPP_
#define TUNER_CMDINVOKER_HPP_

/*----------------------------------------------------------------------------------------------------------------------
*
* Project Harman Mid Systems - Tuner Service
* (c) copyright 2010
* Company Harman Internation
* All rights reserved
* Secrecy Level STRICTLY CONFIDENTIAL
*
*-----------------------------------------------------------------------------------------------------------------------
*
* @file CTunerCmdInvoker.hpp
* @ingroup Pandoraservice
* @author Monojit Chatterjee
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/

#include "CPandoraDataTypes.hpp"
#include "svcIpcEventHandling/CSvcImplBase.hpp"
#include "CPandoraMessageSenderImpl.hpp"
#include "TraceMacros.hpp"
//#include "api/mm/pandoralib/pf/src/CPandoraDataTypes.hpp"
//#include "api/mm/pandoralib/pf/src/CPandoraMessage.hpp"
//#include "api/mm/pandoralib/pf/src/CPandoraProtocolMgr.hpp"
//#include "api/mm/pandoralib/pf/src/IPandoraProtocolListener.hpp"
//TRC_SCOPE_DEF(PandoraService, CTunerCmdInvoker, printData);
typedef enum
{
	eRead,
	eWrite

}eCommunicationType;

typedef enum
{
	eRequestNone,
	eRequestReceived,
	eAckReceived

}eRequestState;

typedef enum
{
	eSessionNone,
	eSessionOpenInProgress,
	eSessionActive

}eSessionState;
#define PROTOCOL_ID 1

class CTunerCmdInvoker : public CIap1Config
{
public:

   CTunerCmdInvoker();

   ~CTunerCmdInvoker();

   //static CTunerCmdInvoker* GetInstance(void);
   static CTunerCmdInvoker* GetInstance(void);
   static CPandoraProtocolMgr* GetInstanceProtocolManager(void);
   static CTunerCmdInvoker* GetInstanceSenderImpl(void);
   bool handleTunerRequest(CSvcIpcRequest* request);
   static bool initPandoraAudioGraph();
   static int mGraphid;
   static unsigned int mStationActiveToken;
   static bool currentStationIsPlaying;
   static bool setPandoraAudioAgainFlag;
   static bool setPandoraAudioAgainFlagChkOnCmd;
   bool Initialize();
   void deinitialize();
   static int mPandoraDeviceFd;
   void printData(const uint8_t *data, const int dataSize, eCommunicationType mtype);
   void updateBrandingImage();

private:
   pandoraReqTypes getEnumFromMethod(char* method);
   static CTunerCmdInvoker* mpInstance;

   //static CPandoraProtocolMgr* vPandoraProtocolMgr;

   static CPandoraMessageSenderImpl *mPandoraMessageSenderImpl;
public:
   static bool mIpodDeviceSupport;
   static bool mTerminateSession;
   static bool SessionTerminated;
   static  eRequestState mPauseSession,sTerminateSession;
   //static eRequestState sTerminateSession;
   static eSessionState meSessionState;
   static bool mStartSession;
};

#endif
