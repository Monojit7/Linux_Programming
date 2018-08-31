/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            LifeCycleManager.cpp
* @ingroup         presctrlcore
* @author          Amit Yatheesh (amit.yatheesh@harman.com)
*/

#include "LifeCycleManager.h"

LifeCycleManager::LifeCycleManager(ShutdownCallback&& callback) :
	mShutdownSignalHandle(0),
	mNsmproxy("local", "NodeStateManager.inst0"),
	mSCallback(std::move(callback))
{
	SUBSCRIBE(mNsmproxy, getProxyStatusEvent(), &LifeCycleManager::statusListener, _1);
}

/* This is the Callback of getProxyStatusEvent signal */
void LifeCycleManager::statusListener(CommonAPI::AvailabilityStatus status)
{
	LOG_INFO(Log::e_LOG_CONTEXT_CORE, "NSM statusListener: signal status %d", (int)status);
	if (CommonAPI::AvailabilityStatus::AVAILABLE == status)
	{
		NodeStateTypes::NsmErrorStatus eCode;
                mShutdownSignalHandle = SUBSCRIBE(mNsmproxy, getShutdownEventSelectiveEvent(), &LifeCycleManager::shutdownevent, _1);

		/*
		* 3u = NodeStateTypes::NsmShutdownType::NSM_SHUTDOWNTYPE_NORMAL | NodeStateTypes::NsmShutdownType::NSM_SHUTDOWNTYPE_FAST
		* Above because we seem to have an issue with codegen when parsing x00000002U
		*/
		mNsmproxy->RegisterShutdownClientAsync(3U, 2000);
	}
	else
	{
                if ( 0 != mShutdownSignalHandle ) 
                   UNSUBSCRIBE(mNsmproxy, getShutdownEventSelectiveEvent(), mShutdownSignalHandle);
		mShutdownSignalHandle = 0;
	}
}

void LifeCycleManager::shutdownevent(const uint32_t &shutdownType)
{
	LOG_INFO(Log::e_LOG_CONTEXT_CORE, "shutdownevent type %d", shutdownType);
	mNsmproxy->LifecycleRequestCompleteAsync(mSCallback(static_cast<NodeStateTypes::NsmShutdownType::Literal>(shutdownType)));
}



