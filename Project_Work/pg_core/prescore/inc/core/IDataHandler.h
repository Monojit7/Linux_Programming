/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            IDataHandler.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)

*/

#ifndef _IDATAHANDLER_HPP_
#define _IDATAHANDLER_HPP_

#include"logging.h"
#include <set>

class DLL_EXPORT BaseData
{
public:

	BaseData()
	{

	}

	virtual ~BaseData()
	{

	}
};

class DLL_EXPORT IThreadDataHandler
{
public:

	virtual ~IThreadDataHandler()
	{
	}

	typedef std::function<void(BaseData*)> SendDataFunction;

	virtual void onData(int threadId, BaseData* data = nullptr) = 0;
};

class DLL_EXPORT IEventDataHandler
{
public:

	virtual ~IEventDataHandler();
	

protected : 

	/**
	* Subscribe to receive event
	*/
	void subscribeEventData(uint32_t eventID); 

	/**
	* Unsubscribe to receive event
	*/
	void unsubscribeEventData(uint32_t eventID);

	virtual void onEventData(uint32_t eventID, BaseData* data = nullptr) = 0;

	std::set<uint32_t> mEventSubscription; 

	friend class CEventManager; 
};

#endif
