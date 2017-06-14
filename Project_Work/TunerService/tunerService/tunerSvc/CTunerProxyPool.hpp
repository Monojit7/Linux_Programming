#ifndef TUNER_PROXYPOOL_HPP_
#define TUNER_PROXYPOOL_HPP_

/*----------------------------------------------------------------------------------------------------------------------
*
* Project Harman Mid Systems - Tuner Service
* (c) copyright 2010
* Company Harman Internation
* All rights reserved
* Secrecy Level STRICTLY CONFIDENTIAL
*
*-----------------------------------------------------------------------------------------------------------------------
*
* @file CTunerCmdInvoker.hpp
* @ingroup Tunerservice
* @author Reshma Roy
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "CTunerDataTypes.hpp"
#include "CTunerStationProxy.hpp"


class CTunerProxyPool
{
public:

   static CTunerProxyPool* GetInstance(void)
   {
      if(NULL == mpInstance)
      {
         mpInstance = new CTunerProxyPool();
         
        // if(mpInstance)
        //    mpInstance->initialize();
      }
      return mpInstance;
   }
   
   static void DeleteInstance(void)
   {
      if(NULL != mpInstance)
      {
        // mpInstance->deinitialize();
         
         delete mpInstance;
         mpInstance = NULL;
      }
   }

	CTunerProxyPool();
	~CTunerProxyPool();


	CTunerStationProxy* GetTunerStationProxy(void);
private:

	CTunerStationProxy* mpTunerStationProxy;
	static CTunerProxyPool* mpInstance;

};

#endif
