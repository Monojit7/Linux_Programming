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

template <class StubClass_,typename... TArgs>
class CommonAPIServiceAbstraction
{
public : 

	CommonAPIServiceAbstraction(std::string&& domain, std::string&& instance,TArgs... args)
        :m_pStub(std::make_shared<StubClass_>(args...)) 
	{
                
		m_pRuntime = CommonAPI::Runtime::get(); 
		assert(m_pRuntime != nullptr);
                m_pRuntime->registerService(domain, instance, m_pStub, PresCtrl::getMainLoop());
 
	}

	std::shared_ptr<StubClass_> operator->()
	{
		assert(m_pStub != nullptr);
		return m_pStub;
	}


private : 
	std::shared_ptr<CommonAPI::Runtime> m_pRuntime;
	std::shared_ptr<StubClass_> m_pStub;
};	

#endif
