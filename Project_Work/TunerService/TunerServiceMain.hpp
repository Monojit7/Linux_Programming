#ifndef _TUNER_SERVICE_MAIN_HPP_
#define _TUNER_SERVICE_MAIN_HPP_

/*******************************************************************************
 *  Project       JLR NLI
 *  (c) copyright 2013
 *  Company       Harman International
 *                All rights reserved
 * Secrecy Level STRICTLY CONFIDENTIAL
 ******************************************************************************/

/**
 *  @file         TunerServiceMain.hpp
 *  @brief        This is the main class of Tuner Service and it is a singleton
 *  @authors      Sachin Athanikar
 *  @ingroup      Tuner Service
 */

/*-----------------------------------------------------------------------------\
 * Preprocessor Includes
\*----------------------------------------------------------------------------*/
//#include "svcIpcEventHandling/CSvcIpcAccessoryEventTarget.hpp"
//#include "svcIpcEventHandling/CSvcIpcEventRouter.hpp"
//#include "svcIpcEventHandling/CSvcImplBase.hpp"
//#include "svcIpcEventHandling/CSvcClientBase.hpp"
#include <iostream>
#include "CTunerDataTypes.hpp"

using namespace std;

/*-----------------------------------------------------------------------------\
 * Preprocessor Defines (Macros, Constants)
\*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------\
 * Type Definitions (Enums, Typedefs, Structs, ...)
\*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------\
 * Constant And External Variables
\*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------\
 * Function Prototypes
\*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------\
 * Class Declarations
\*----------------------------------------------------------------------------*/

class CTunerServiceMain	/*Singleton Pattern*/
{
/*.............................................................................\
 * Public Members
\*............................................................................*/
public:
	 CTunerServiceMain();
	 ~CTunerServiceMain();

   static CTunerServiceMain* GetInstance(void)
   {
      if(NULL == mpInstance)
      {
         mpInstance = new CTunerServiceMain();
         
       /*  if(mpInstance)
            mpInstance->Initialize();*/
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

   BOOL getHwVariant();

   static BOOL hwVariantType;

private:
   static CTunerServiceMain* mpInstance;

};

#endif /* _TUNER_SERVICE_MAIN_HPP_ */

/*--------------------------------- EOF --------------------------------------*/
