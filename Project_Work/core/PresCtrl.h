/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            PresCtrl.h
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           The main interface file for the presentation control core. 
*/


#ifndef _PRES_CTRL_H_
#define _PRES_CTRL_H_

#include <core/PresCtrlTypes.h>
#include <CommonAPI/CommonAPI.hpp>
#include <memory>
#include <core/IDataHandler.h>
#include <core/Lifecycle.h>

class PresCtrlCore; 
class RequestBase; 


class DLL_EXPORT PresCtrl
{
public:
	/**
	* @brief Init pres control
	* callback should be passed if the client expects a callback on the shutdown type
	*/
	static void initPresControl(const std::unordered_map <uint64_t, Request> & ,ShutdownCallback && callback=nullptr);

	/**
	* @brief Init pres control without CAPI support. CAPI can be later enabled with delayedInitCAPI call. 
	* callback should be passed if the client expects a callback on the shutdown type
	*/
	static void initPresControlNoCAPI(const std::unordered_map <uint64_t, Request> &, ShutdownCallback && callback = nullptr);


	static void delayedInitCAPI(); 

	/**
	* @brief Pres Ctrl Main Loop. Will not return
	*
	* initNotify can be used to enable/disable 'notify' feature to startup component
	*
	* Currently prescore supports only systemd(linux) for notification of start-up completion
	*
	* If Presentation controller chooses true/default param , app should ensure that
	* all initializations are complete before the mainLoop invoke
	* If Presentation controller chooses false , the app is responsible to notify completion
	*/
	static void mainLoop(bool initNotify=true);

	/**
	* @brief Cleansup the core. Call this if application is terminating.
	*/
	static void shutDown();


	/**
	*  Request Management Functions
	*/

	static e_AddReqResponse addRequest(uint32_t RequestID, std::function<RequestBase*()>&& requestFunc, uint64_t DynamicGroupID = 0u);

	/**
	* @brief  Removes all requests belonging to a specific group.
	*/
	void purgeAllRequestsInGroup(uint64_t GroupID, uint64_t GroupDynamicID = 0u);

	static void addWorkerThreadData(int threadId,BaseData* threadData);

	/**
	* @brief  API to support invoking a callback in the mainloop thread (hence making it thread safe)
	* API need not be used with CAPI / timer / thread events as core implicitly takes care of executing 
	* their callbaks in main thread
	* API can be typically used within pres controllers that get callbacks / events in a different thread 
	* (for ex: thread created within a utility lib or because a poll / select is needed on an fd) 
	* and yet want to handle in a thread safe approach without using mutexes / condvars
	*/
	static void addExternalEvent(std::function<void()> callback);

	static void publishEvent(uint32_t EventID, BaseData* pData); 

private : 

	static std::shared_ptr<CommonAPI::MainLoopContext> getMainLoop();
	
	template <template<typename ...> class  T>
	friend class CommonAPIClientAbstraction; 
	template <class  T>
	friend class CommonAPIServiceAbstraction;
	template <class  T>
	friend class CommonAPIServiceAbstractionV2;

	static PresCtrlCore* mpCore;

};

#endif
