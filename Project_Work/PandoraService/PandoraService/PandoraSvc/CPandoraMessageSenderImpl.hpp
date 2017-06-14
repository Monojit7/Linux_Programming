/*----------------------------------------------------------------------------------------------------------------------
*
* Project Harman Mid Systems - Pandora Service
* (c) copyright 2013
* Company Harman Internation
* All rights reserved
* Secrecy Level STRICTLY CONFIDENTIAL
*
*-----------------------------------------------------------------------------------------------------------------------
*
* @file CPandoraMessageSenderImpl.cpp
* @ingroup PandoraService
* @author Monojit Chatterjee
* Public API for the Pandora Service.
*
*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#ifndef PANDORA_MESSAGE_SENDER_HPP_
#define PANDORA_MESSAGE_SENDER_HPP_

#include "IPandoraListenerImpl.hpp"
#include "api/mm/iap1devctrl/pf/src/CIAP1_Types.hpp"
#include "api/mm/iap1devctrl/pf/src/CIAP1Config.hpp"
#include "api/mm/iap1devctrl/pf/src/CIAP1MgrController.hpp"
#include "api/mm/iap1devctrl/pf/src/IIpodNotification.hpp"
#include <unistd.h>
#include <list>
#include <sys/types.h>
#include <queue>
#include <pthread.h>
#include "CTimer.h"

class CPandoraMessageSenderImpl : public ITimerHandler
{
public:

	CPandoraMessageSenderImpl();
	static void sendData();
	static void addSendQueue(CPandoraMessage* data);
	static void clearQueue();
	void handleTimerEvent(CTimer* pTimer);

    static std::queue<CPandoraMessage*> mPandoraRequestQueue;
    static bool mSendInProgress;
    static bool mPandoraAckReceived;
    static int mRetryCounter;
   // static RtTimer_t mRt_timer;

};

#endif
