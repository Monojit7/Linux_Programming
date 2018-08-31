#include <core/PresCtrl.h>
#include "PresCtrlCore.h"


PresCtrlCore* PresCtrl::mpCore = nullptr;


void PresCtrl::initPresControl(const std::unordered_map <uint64_t, Request> & reqTable,ShutdownCallback && sCallback)
{
	if (nullptr == mpCore)
	{
		mpCore = new PresCtrlCore(reqTable, std::move(sCallback), false);
		mpCore->initLCM();
	}
}


void PresCtrl::initPresControlNoCAPI(const std::unordered_map <uint64_t, Request> & reqTable,ShutdownCallback && sCallback )
{
	if (nullptr == mpCore)
	{
		mpCore = new PresCtrlCore(reqTable, std::move(sCallback), true);
	}
}

void PresCtrl::delayedInitCAPI()
{
	if (nullptr != mpCore)
	{
		mpCore->delayedCAPIInit();
		mpCore->initLCM();
	}
}

/**
*  Request Management Functions
*/

e_AddReqResponse PresCtrl::addRequest(uint32_t RequestID, std::function<RequestBase*()>&& requestFunc, uint64_t DynamicGroupID)
{
	return mpCore->addRequest(RequestID, std::move(requestFunc),DynamicGroupID);
}

std::shared_ptr<CommonAPI::MainLoopContext> PresCtrl::getMainLoop()
{
	return mpCore->getMainLoop();
}

void PresCtrl::addWorkerThreadData(int threadId,BaseData* threadData)
{
	mpCore->addWorkerThreadData(threadId, threadData);
}

void PresCtrl::publishEvent(uint32_t EventID, BaseData* pData)
{
	mpCore->publishEvent(EventID, pData);

}

void PresCtrl::addExternalEvent(std::function<void()>&& callback)
{
	mpCore->addExternalEvent(std::move(callback));
}

/**
* @brief Pres Ctrl Main Loop. Will not return
*/
void PresCtrl::mainLoop(bool initNotify)
{
	mpCore->mainLoop(initNotify);
	//cleanup on exit
	delete mpCore;
}

void PresCtrl::shutDown()
{
	mpCore->shutDown();
}
