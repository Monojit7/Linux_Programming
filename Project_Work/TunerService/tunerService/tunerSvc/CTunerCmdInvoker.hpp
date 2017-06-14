#ifndef TUNER_CMDINVOKER_HPP_
#define TUNER_CMDINVOKER_HPP_

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
#include "CTunerReqQueue.hpp"
#include "CTunerCommand.hpp"
#include "CTunerProxyPool.hpp"
#include "CTunerdataTypes.hpp"

class CTunerCmdInvoker
{
public:

   CTunerCmdInvoker();

   ~CTunerCmdInvoker();

   //static CTunerCmdInvoker* GetInstance(void);
   static CTunerCmdInvoker* GetInstance(void);
   bool handleTunerRequest(CSvcIpcRequest* request);
   bool Initialize();
   void deinitialize();
   void setPersistencyFlag(bool status);
   bool GetPersistencyFlag();
   static int eListType;

private:
   tunerReqTypes getEnumFromMethod(char* method);
   static CTunerCmdInvoker* mpInstance;

   bool mPersistencyRead;
};

#endif
