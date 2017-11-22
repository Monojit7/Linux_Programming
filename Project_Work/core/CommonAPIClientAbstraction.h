/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            CommonAPIClientAbstraction.h
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           Simple wrapper over Common  API proxies. 
*/


#ifndef _COMMON_API_CLIENT_ABSTRACTION_
#define _COMMON_API_CLIENT_ABSTRACTION_

#include <CommonAPI/CommonAPI.hpp>
#include <string>
#include <functional>
#include <set>
#include <core/PresCtrl.h>

/**
* Some helpful Macros for subscribing and invoking functions.
*/
#define SUBSCRIBE(PROXYTYPE,EVENT,CALLBACK,...) (PROXYTYPE->EVENT.subscribe( std::bind (CALLBACK,this, ## __VA_ARGS__) ));

//#define SUBSCRIBE_EVENT(PROXYTYPE,EVENT,CALLBACK,...) (uint32_t token=PROXYTYPE->EVENT.subscribe( std::bind (CALLBACK,this, ## __VA_ARGS__) );return token;);

/**
* Some helpful Macros for subscribing and invoking functions.
*/
#define UNSUBSCRIBE(PROXYTYPE,EVENT,TOKEN) PROXYTYPE->EVENT.unsubscribe(TOKEN);


using namespace std::placeholders;

#define ASYNC_INVOKE_3ARG(P,R,A,I,C,T,...) 	P.setResetListener A (true, I,++P.mSeqNr, std::move(C)); \
						P->T(__VA_ARGS__ std::bind(&CommonAPIClientAbstraction<R>::responseFunc A , &P, I, P.mSeqNr,_1,_2,_3 ));

#define ASYNC_INVOKE_2ARG(P,R,A,I,C,T,...) 	P.setResetListener A (true, I,++P.mSeqNr, std::move(C)); \
						P->T(__VA_ARGS__ std::bind(&CommonAPIClientAbstraction<R>::responseFunc A , &P, I, P.mSeqNr,_1,_2 ));

#define ASYNC_INVOKE_1ARG(P,R,A,I,C,T,...) 	P.setResetListener A (true, I,++P.mSeqNr, std::move(C)); \
						P->T(__VA_ARGS__ std::bind(&CommonAPIClientAbstraction<R>::responseFunc A , &P, I, P.mSeqNr,_1));

template <template<typename ...> class ProxyClass_>
class CommonAPIClientAbstraction
{
public : 

	CommonAPIClientAbstraction(std::string&& domain, std::string&& instance)
		:m_pRuntime(nullptr)
	    , m_pProxy(nullptr)
		,mSeqNr(0u)
	{
		m_pRuntime = CommonAPI::Runtime::get();
		assert(m_pRuntime != nullptr);
		m_pProxy = m_pRuntime->buildProxy<ProxyClass_>(domain, instance, PresCtrl::getMainLoop());
	}

	const std::string getStringAvailibility(const CommonAPI::AvailabilityStatus& avail)
	{
		return (CommonAPI::AvailabilityStatus::AVAILABLE == avail) ? std::string("available") : std::string("not available"); 
	}

	const std::string getStringCallStatus(const CommonAPI::CallStatus & stat)
	{
		return (CommonAPI::CallStatus::SUCCESS == stat) ? std::string("success") : std::string("failed");
	}

	std::shared_ptr<ProxyClass_<>> operator->()
	{
		assert(m_pProxy != nullptr);
		return m_pProxy;
	}


	template <typename ... _attribs>
	void responseFunc(uint32_t ClientID,uint32_t SequenceNumber, const _attribs&... Attributes)
	{
		typedef std::function  <void(const _attribs& ...)> Listener;
		Listener func = setResetListener(false, ClientID,SequenceNumber, (Listener)nullptr);
		if (func)
		{
			func(Attributes...);
		}
		else
		{
			///No Target found. Response Ignored. 
		}

	}

	template <typename ... _attribs>
	std::function  <void(const _attribs& ...)> setResetListener(bool isSet, uint32_t ClientID,uint32_t SeqNum, std::function  <void(const _attribs& ...)>&& func)
	{
		typedef std::function  <void(const _attribs& ...)> Listener;
		static std::unordered_map <uint32_t, Listener> ListenerMap;

		if (isSet)
		{
			ListenerMap.insert(std::make_pair(SeqNum, std::move(func)));
			return std::move(func);
		}
		else
		{
			auto it = ListenerMap.find(SeqNum);
			Listener returnfunc = nullptr; 
			if (it != ListenerMap.end())
			{
				
				if (checkClientExists(ClientID))
				{
					returnfunc = std::move(it->second);
				}
				else
				{
					///Client has gone missing.. maybe destructed.. handle this gracefully and remove the listener mapping withouth removing anything.  
					returnfunc = nullptr; 
				}
				ListenerMap.erase(it);
			}
			return returnfunc;
		}
	}

	///
	// Check if Client still exists, or is missing now. 
	///
	bool checkClientExists(uint32_t ClientID)
	{
		return ( mClientList.find(ClientID)  != mClientList.end() );
	}

	void addClient(uint32_t id)
	{
		mClientList.insert(id); 
	}

	void removeClient(uint32_t id)
	{
		mClientList.erase(id); 
	}

	uint32_t mSeqNr;
private : 
	std::shared_ptr<CommonAPI::Runtime> m_pRuntime;
	std::shared_ptr<ProxyClass_<>> m_pProxy;
	std::set<uint32_t>  mClientList; ///Contains list of all active clients. 
};	

#endif
