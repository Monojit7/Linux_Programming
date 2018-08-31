/*
 * IEventPublisherBase.cpp
 *
 *  Created on: Jan 4 2017
 *      Author: VNandakumar
 */
#include "core/PresEventAbstraction.h"


/**
* @breif : Internal: Used by PresEventSubscriber and PresEventPublisher
*/
PresEventAbstraction* PresEventAbstraction::instance = NULL;

PresEventAbstraction* PresEventAbstraction::getInstance(){
	if (instance == NULL){
		static PresEventAbstraction obj;
		instance = &obj;
	}
	return instance;
}

PresEventAbstraction::~PresEventAbstraction()
{
	for(auto itMap = eventQueue.begin(); itMap != eventQueue.end(); itMap++)
	{
		std::vector<EventBase*> list =  itMap->second;
		auto it = list.begin();
		while(it != list.end()){
			delete(*it);
			it++;
		}
		list.clear();
	}
	eventQueue.clear();
}
