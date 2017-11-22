/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            CommonAPIServiceAbstraction.h
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           Simple wrapper over Common  API Stubs.
*/

#ifndef _COMMON_API_SERVICE_ABSTRACTION_
#define _COMMON_API_SERVICE_ABSTRACTION_

#include <CommonAPI/CommonAPI.hpp>
#include <string>
#include <functional>
#include <core/PresCtrl.h>

template <class T>
class CommonAPIServiceAbstraction
{
public : 

	CommonAPIServiceAbstraction(std::string&& domain, std::string&& instance)
		:m_pStub(std::make_shared<T>())
	{
		m_pRuntime = CommonAPI::Runtime::get(); 
		assert(m_pRuntime != nullptr);
		m_pRuntime->registerService(domain, instance, m_pStub, PresCtrl::getMainLoop());
	}


	std::shared_ptr<T> operator->()
	{
		assert(m_pStub != nullptr);
		return m_pStub;
	}


private : 
	std::shared_ptr<CommonAPI::Runtime> m_pRuntime;
	std::shared_ptr<T> m_pStub;
};	

// Template class to be used if automatic unregister of service required on object destruction
template <class T>
class CommonAPIServiceAbstractionV2
{
public:

	CommonAPIServiceAbstractionV2(std::string&& domain, std::string&& instance, std::string&& interface)
		:m_pStub(std::make_shared<T>()),
		m_domain(domain),
		m_instance(instance),
		m_interface(interface)
	{
		m_pRuntime = CommonAPI::Runtime::get();
		assert(m_pRuntime != nullptr);
		m_pRuntime->registerService(domain, instance, m_pStub, PresCtrl::getMainLoop());
	}

	~CommonAPIServiceAbstractionV2()
	{
		m_pRuntime->unregisterService(m_domain, m_interface, m_instance);
	}

	std::shared_ptr<T> operator->()
	{
		assert(m_pStub != nullptr);
		return m_pStub;
	}


private:
	std::shared_ptr<CommonAPI::Runtime> m_pRuntime;
	std::shared_ptr<T> m_pStub;

	std::string m_domain;
	std::string m_instance;
	std::string m_interface;
};

#endif
