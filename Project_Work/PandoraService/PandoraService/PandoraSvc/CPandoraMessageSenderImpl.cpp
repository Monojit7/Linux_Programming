/*----------------------------------------------------------------------------------------------------------------------
*
* 
*
*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "CPandoraMessageSenderImpl.hpp"
#include "CPandoraRequestResponseHandler.hpp"
#include "IIpodNotificationImpl.hpp"
#include <iostream>
#include "CPandoraCmdInvoker.hpp"
#include "TraceMacros.hpp"
#include "CTimer.h"
pthread_mutex_t vSendDataWritemutex = PTHREAD_MUTEX_INITIALIZER;

//#include <photon/realtime/RtTimer.h>
extern CPandoraProtocolMgr* vPandoraProtocolMgr;
extern CIap1MgrController* vCIap1MgrController;


//extern IPandoraProtocolListener *listener;
extern CPandoraProtocolMgr* vPandoraProtocolMgr;
pthread_mutex_t vqueuelock;
std::queue<CPandoraMessage*> CPandoraMessageSenderImpl::mPandoraRequestQueue;
bool CPandoraMessageSenderImpl::mSendInProgress = false;
bool CPandoraMessageSenderImpl::mPandoraAckReceived = false;
int CPandoraMessageSenderImpl::mRetryCounter = -1;

ITimerHandler* pTimerHandler = new CPandoraMessageSenderImpl();
CTimer *mTimerInstance = new CTimer(pTimerHandler);
CTimer *mPandoraDeviceCheck = new CTimer(pTimerHandler);
CTimer *mPandoraAudioTimer = new CTimer(pTimerHandler);

TRC_SCOPE_DEF(PandoraService, CPandoraMessageSenderImpl, sendData);
TRC_SCOPE_DEF(PandoraService, CPandoraMessageSenderImpl, addSendQueue);
TRC_SCOPE_DEF(PandoraService, CPandoraMessageSenderImpl, handleTimerEvent);

CPandoraMessageSenderImpl::CPandoraMessageSenderImpl()
{

}

void CPandoraMessageSenderImpl::handleTimerEvent(CTimer* pTimer)
{
	TRC_SCOPE(PandoraService, CPandoraMessageSenderImpl, handleTimerEvent);
	//DBG_MSG(("[CPandoraMessageSenderImpl]::handleTimerEvent"));
	int priority = getprio(0);

	if (pTimer == mTimerInstance)
	{
		////DBG_MSG(("[CPandoraMessageSenderImpl]::Pandora_Timeout"));
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, priority, Pandora_Timeout, 0);
	}
	else if (pTimer == mPandoraDeviceCheck)
	{
		mPandoraDeviceCheck->stopTimer();
		//DBG_MSG(("[CPandoraMessageSenderImpl]::mPandoraDeviceCheck"));
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, priority, Pandora_Device_Check_Timer, 0);
	}
	else if (pTimer == mPandoraAudioTimer)
	{
		mPandoraAudioTimer->stopTimer();
		//DBG_MSG(("[CPandoraMessageSenderImpl]::mPandoraAudioTimer"));
		MsgSendPulse( CTunerRequestResponseHandler::MsgConnectionId, priority, Pandora_Audio_Graph_Timer, 0);
	}
}

void CPandoraMessageSenderImpl::addSendQueue(CPandoraMessage* data)
{
	TRC_SCOPE(PandoraService, CPandoraMessageSenderImpl, addSendQueue);
	//DBG_MSG(("[CPandoraMessageSenderImpl]::addSendQueue"));
	if(data)
	{
		pthread_mutex_lock(&vqueuelock);
		CPandoraMessageSenderImpl::mPandoraRequestQueue.push(data);
		pthread_mutex_unlock(&vqueuelock);
	}

}

void CPandoraMessageSenderImpl::clearQueue()
{
    while (!CPandoraMessageSenderImpl::mPandoraRequestQueue.empty())
    {
    	CPandoraMessageSenderImpl::mPandoraRequestQueue.pop();
    }
    ////DBG_MSG(("[CPandoraMessageSenderImpl]::queue is cleared now"));
}
void CPandoraMessageSenderImpl::sendData() {
	TRC_SCOPE(PandoraService, CPandoraMessageSenderImpl, sendData);
	//DBG_MSG(("[CPandoraMessageSenderImpl]::sendData"));
	if ((false == CPandoraMessageSenderImpl::mSendInProgress))
	{
		//DBG_MSG(("[CPandoraMessageSenderImpl]::sendData:mSendInProgress = false"));

		if (!CPandoraMessageSenderImpl::mPandoraRequestQueue.empty())
		{
			//DBG_MSG(("[CPandoraMessageSenderImpl]:Queue is not Empty and Size is %d",CPandoraMessageSenderImpl::mPandoraRequestQueue.size()));

			CPandoraMessage* msg = CPandoraMessageSenderImpl::mPandoraRequestQueue.front();
			//DBG_MSG(("[CPandoraMessageSenderImpl]::sendData:mPandoraRequestQueue.front():getDataSize()=%d",msg->getDataSize()));
			if ((msg->getDataSize() > 0))
			{
				//DBG_MSG(("[CPandoraMessageSenderImpl]::Message written is more than o bytes", msg->getDataSize()));

				if ((IIpodNotificationImpl::mSessionId != -1))
				{
					int size = msg->getDataSize();
					unsigned int ProtocolID = PROTOCOL_ID;
					//CPandoraMessageSenderImpl::mRetryCounter = 0;
					vPandoraProtocolMgr->prepareMessageToSend(msg);
					//DBG_MSG(("[CPandoraMessageSenderImpl]::CPandoraMessageSenderImpl::mSendInProgress = true"));


					//DBG_MSG(("Sending the packet %s", CPandoraMessage::sendCommandToString (msg->getCommand())));

					if (Iap1_Types::OK == vCIap1MgrController->requestIpodSendData( ProtocolID, (char*) msg->getData(), size))
					{
						if(msg->getCommand() == 0x00)
						{
							//DBG_MSG(("[CPandoraMessageSenderImpl]::Sending the Packet for StartSession"));
							CTunerCmdInvoker::meSessionState = eSessionOpenInProgress;
						}

						if(msg->getCommand() == 0x05)
						{
							//DBG_MSG(("[CPandoraMessageSenderImpl]::Sending the Packet for TerminateSession"));
							CTunerCmdInvoker::sTerminateSession = eRequestReceived;
							//CTunerCmdInvoker::meSessionState = eSessionOpenInProgress;
						}
						CPandoraMessageSenderImpl::mSendInProgress = true;
						//DBG_MSG(("[CPandoraMessageSenderImpl]::Calling Stop Timer"));
						mTimerInstance->stopTimer();
						CPandoraMessageSenderImpl::mRetryCounter = 0;
						CPandoraMessageSenderImpl::mPandoraAckReceived = false;
						//DBG_MSG(("[CPandoraMessageSenderImpl]::Calling Start Timer"));
						mTimerInstance->startTimer(750, true);
						//DBG_MSG(("[CPandoraMessageSenderImpl]::sendData: Sending to ipod"));
					} else
					{
						//Queue may have more commands to send this portion
						//This scenario should not happen.
						//DBG_MSG(("[CPandoraMessageSenderImpl]::sendData: Sending failed to ipod"));
						//DBG_MSG(("[CPandoraMessageSenderImpl]::Message written less than or equal to 0 bytes"));
						CPandoraMessageSenderImpl::mSendInProgress = false;
					}

				}
				else if (CTunerRequestResponseHandler::mAndroidDeviceConnected == true)
				{
					//DBG_MSG(("[CPandoraMessageSenderImpl]::prepareMessageToSend"));
					CPandoraMessageSenderImpl::mPandoraAckReceived = false;

					vPandoraProtocolMgr->prepareMessageToSend(msg);
					ssize_t vNumBytes = write(
							CTunerCmdInvoker::mPandoraDeviceFd, msg->getData(),
							msg->getDataSize());
					//DBG_MSG(("Sending the packet %s", CPandoraMessage::sendCommandToString (msg->getCommand())));

					//CTunerCmdInvoker::GetInstance()->printData(msg->getData(),msg->getDataSize(), eWrite);
					if (vNumBytes > 0)
					{
						if(msg->getCommand() == 0x00)
						{
							//DBG_MSG(("[CPandoraMessageSenderImpl]::Sending the Packet for StartSession"));
							CTunerCmdInvoker::meSessionState = eSessionOpenInProgress;
							//CTunerCmdInvoker::SessionTerminated = 0;
						}

						if(msg->getCommand() == 0x05)
						{
							//DBG_MSG(("[CPandoraMessageSenderImpl]::Sending the Packet for TerminateSession"));
							CTunerCmdInvoker::sTerminateSession = eRequestReceived;
							//CTunerCmdInvoker::SessionTerminated = 1;
							//CTunerCmdInvoker::meSessionState = eSessionOpenInProgress;
						}
						//
						CPandoraMessageSenderImpl::mSendInProgress = true;
						//DBG_MSG(("[CPandoraMessageSenderImpl]::Calling Stop Timer"));
						mTimerInstance->stopTimer();
						CPandoraMessageSenderImpl::mRetryCounter = 0;
						CPandoraMessageSenderImpl::mPandoraAckReceived = false;
						//DBG_MSG(("[CPandoraMessageSenderImpl]::Calling Start Timer"));
						mTimerInstance->startTimer(750, true);
						//DBG_MSG(("[CPandoraMessageSenderImpl]::sendData: Sending to Android"));
						//DBG_MSG(("[CPandoraMessageSenderImpl]::written bytes is more than o bytes", vNumBytes));
					} else {
						//This is communication failure. This is not handled.
						//DBG_MSG(("[CPandoraMessageSenderImpl]::Message written less than or equal to 0 bytes"));
						CPandoraMessageSenderImpl::mSendInProgress = false;
					}
				}
			}
		} else {
			//DBG_MSG(("[CPandoraMessageSenderImpl] Queue is Empty"));
		}
	} else {
		//DBG_MSG(("[CPandoraMessageSenderImpl]::sendData:mSendInProgress:true"));
	}

}


