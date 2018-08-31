/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            PresCtrlCore.h
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           The core internal framework class for the Presentation Control Core
*				   
*/

#ifndef _PRES_CTRL_CORE_
#define _PRES_CTRL_CORE_

#include "core/PresCtrlTypes.h"
#include <functional>
#include "RequestManager.h"
#include "CommonAPIMainLoopEngine.h"
#include "EventQueueAbstraction.h"
#include "WdgManager.h"
#include "core/IDataHandler.h"
#include <core/Lifecycle.h>
#include "CTimerManager.h"
#include <core/CThreadManager.h>
#ifndef PLATFORM_WIN32
#include "CMainLoop.h"
#endif

/**
* \brief  The core and entry point of the PresCtrl. 
*/

class LifeCycleManager;
class  PresCtrlCore
{
public : 

	/**
	* \brief  The core and entry point of the PresCtrl.
	*/
	PresCtrlCore(const std::unordered_map <uint64_t, Request> &, ShutdownCallback && sCallback,bool bDelayedCAPIInit = false);

	~PresCtrlCore();

	void initLCM();

	void delayedCAPIInit();

	e_AddReqResponse addRequest(uint32_t RequestID, std::function<RequestBase*()>&& requestFunc,uint64_t DynamicGroupID = 0u);

	static void addCAPIEvent(CommonAPI::Watch* watch,uint32_t revents);

	static void addWorkerThreadData(int threadId, BaseData* threadData);

	static void publishEvent(uint32_t EventID, BaseData* pData);
	static void addExternalEvent(std::function<void()>&& callback);

	static PresCtrlCore& getCore();

	/**
	* \brief  Cleans up everything from the Queue. 
	*/
	void purgeAllRequests();

	std::shared_ptr<CommonAPI::MainLoopContext> getMainLoop()
	{
		return mCAPIEngine->getMainLoop(); 
	}
	
	/**
	* @brief  Removes all requests belonging to a specific group. 
	*/
	void purgeAllRequestsInGroup(uint64_t GroupID,uint64_t GroupDynamicID = 0u); 

	/**
	* @brief Cleansup the core. Call this if application is terminating. 
	*/
	void shutDown(); 


	/**
	* @brief Pres Ctrl Main Loop. Will not return
	*/
	void mainLoop(bool initNotify);

	void runQueue();
	
private:

	static PresCtrlCore* mCore; 

	void decodeAndHandleEventIntern(EventQueueAbstraction::InternalEventItem *);
	void createWdg();

#ifndef PLATFORM_WIN32
	CMainLoop mLinuxMainloop;
#endif
	RequestManager  mManager;
	ShutdownCallback msCallback;
	CTimerManager mTManager;
	static LifeCycleManager* mlifeCycleManager;
	static WdgManager* mWdgManager;
	CommonAPIMainLoopEngine* mCAPIEngine;
	CThreadManager* mThreadManager;
	bool mRunMainLoop;
};


#endif
