/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2017
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            CEventManager.cpp
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           EventManager can be used to publish arbitraty events from any thread to the pres control core main loop
*/
#include "CEventManager.hpp"

std::map<uint32_t, std::set<IEventDataHandler*> > CEventManager::mSubscriptionCache;

void CEventManager::subscribe(uint32_t event, IEventDataHandler* listener)
{
	auto it = mSubscriptionCache.find(event); 
	if (mSubscriptionCache.end() != it)
	{
		it->second.insert(listener); 
	}
	else
	{
		std::set<IEventDataHandler*> initset; 
		initset.insert(listener);
		mSubscriptionCache.insert(std::pair<uint32_t, std::set<IEventDataHandler*>>(event, initset));
	}
}

void CEventManager::unsubscribe(uint32_t event, IEventDataHandler* listener)
{
	auto it = mSubscriptionCache.find(event);
	if (mSubscriptionCache.end() != it)
	{
		it->second.erase(listener);
	}
}

void CEventManager::handleEventData(uint32_t event, BaseData* data)
{
	auto it = mSubscriptionCache.find(event);
	if (mSubscriptionCache.end() != it)
	{
		for (auto it1 = it->second.begin(); it1 != it->second.end(); it1++)
		{
			(*it1)->onEventData(event, data); 
		}
	}
	delete data; 
}