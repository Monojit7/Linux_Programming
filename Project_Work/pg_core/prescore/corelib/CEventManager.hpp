/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2017
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            CEventManager.hpp
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           EventManager can be used to publish arbitraty events from any thread to the pres control core main loop
*/
#include <map>
#include <set>
#include <core/IDataHandler.h>

class CEventManager
{
public : 
	static void subscribe(uint32_t event, IEventDataHandler*); 
	static void unsubscribe(uint32_t event, IEventDataHandler*);

	static void handleEventData(uint32_t event, BaseData*);

private : 

	static std::map<uint32_t, std::set<IEventDataHandler*> > mSubscriptionCache; 
};