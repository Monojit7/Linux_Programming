#include <core/IDataHandler.h>
#include "CEventManager.hpp"

IEventDataHandler::~IEventDataHandler()
{
	for (auto it = mEventSubscription.begin(); it != mEventSubscription.end(); it++)
	{
		CEventManager::unsubscribe(*it, this); 
	}
	mEventSubscription.clear(); 
}

void IEventDataHandler::subscribeEventData(uint32_t eventID)
{
	auto result = mEventSubscription.insert(eventID); 
	if (result.second)
	{
		CEventManager::subscribe(eventID, this); 
	}
}

void IEventDataHandler::unsubscribeEventData(uint32_t eventID)
{
	uint32_t numRemoved = mEventSubscription.erase(eventID); 
	if (1 <= numRemoved)
	{
		CEventManager::unsubscribe(eventID, this);
	}
}