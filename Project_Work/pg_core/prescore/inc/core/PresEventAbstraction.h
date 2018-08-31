/*
 * PresEventAbstraction.hpp
 *
 *  Created on: Jan 4 2017
 *  Author: VNandakumar
 * @brief Framework implementing an observer pattern
 * Following can be used for event notifications with a pres ctrl 
 * or from a pres ctrl to a runtime plugin loaded by the pres ctrl
 * This can be used escpecially when a project specific behaviour needs to be
 * achieved on an event triggered by the pres ctrl
 */
#ifndef PRESEVENTABSTRATION_H
#define PRESEVENTABSTRATION_H

#include<functional>
#include<map>
#include<vector>
#include<core/PresCtrlTypes.h>

////////////////////////////////Public: Helper  macros////////////////////////////////////////////////////////////////////

/**
 * @breif : Helper macros for subscribe
 * Caller has to provide REPLY -> Function signature defined by the publisher ( refer publisher header file)
 * returns token
 */
#define PRESEVENT_SUBSCRIBE(REPLY,EVENT,CALLBACK,...)\
	PresEventAbstraction::getInstance()->subscribe<REPLY>(EVENT, std::bind (CALLBACK,this, ## __VA_ARGS__));
 /**
 * @breif : Helper macros for unsunscribe
 * Caller has to provide EVENT,TOKEN(returns from PRESEVENT_SUBSCRIBE)
 * returns token
 */
#define PRESEVENT_UNSUBSCRIBE(EVENT,TOKEN) PresEventAbstraction::getInstance()->unSubscribe(EVENT, TOKEN);
 /**
 * @breif : Helper macro for publisher
 * Caller must define REPLY type and EVENT enum its header and share with subscriber
 * Caller has to provide REPLY type & EVENT in this
 */
#define PRESEVENT_PUBLISH(REPLY,EVENT,...) PresEventAbstraction::getInstance()->publish<REPLY>((uint32_t)EVENT, ## __VA_ARGS__);

////////////////////////////////////////// INTERNAL USE/////////////////////////////////////////////////////////////////////

/* EventBase header used internal only */
class EventBase{
public:
	uint32_t eventType;
	EventBase(uint32_t val) :eventType(val){}
	virtual ~EventBase(){
	}
};

//////////////////////////////////////////////////////////// PUBLIC interface starts //////////////////////////////////////////////////////////

/**
* breif : Subscriber object class used Subscriber 
*/
template<typename T>
class DLL_EXPORT PresSubscribeObject:public EventBase
{
	virtual ~PresSubscribeObject(){
	}
public:
	T  callback;
	PresSubscribeObject(uint32_t eventType, T callback):EventBase(eventType),callback(callback){
	};
};


/**
 * @breif : PresEventAbstraction:: Abstraction class
 */
class DLL_EXPORT PresEventAbstraction
{
	std::map<uint32_t, std::vector<EventBase*>> eventQueue;
	static PresEventAbstraction* instance;

	PresEventAbstraction(){
	}
public:
	static PresEventAbstraction* getInstance();

	 /**
	 * @breif: subscribe(): Caller must pass the PresSubscribeObject with REPLY type (Defined by the publisher) and
	 * it must be created with a EVENT(uint32) defined by the publisher
	 * return token ( for unsubscribe)
	 */
	template<typename REPLY>
	uint32_t subscribe(uint32_t event, REPLY obj) {
		PresSubscribeObject<REPLY>* eventObj = new PresSubscribeObject<REPLY>(event, std::move(obj));
		std::vector<EventBase*>& List = eventQueue[event];
		List.push_back(eventObj);
		return List.size();
	}
	/**
	 * @breif: unSubscribe(): Caller must pass token(from subscribe) and event for unsubscribe
	 */
	void unSubscribe(uint32_t event, uint32_t token) {
		std::vector<EventBase*>& List = eventQueue[event];
		if (token > List.size()){
		    return;
		}
		auto it = List.begin()+token-1;
		delete (*it);
		List.erase(it);
	}

	 /**
	 * @breif: publish(): Publish the events to its subscribers
	 * Caller must pass the event enum(id) and args
	 */

	template<typename REPLY, typename ... ARGS>
	void publish(uint32_t event, ARGS...args) {
		std::vector<EventBase*> list = eventQueue[event];//copy allow unsubscribe from callback
		auto it = list.begin();
		while(it != list.end()){
			PresSubscribeObject<REPLY> *obj = dynamic_cast<PresSubscribeObject<REPLY>*>(*it);
			it++;//allow callback to unsubscribe
			obj->callback(args...);
		}
	}
	~PresEventAbstraction()	;

};


#endif

