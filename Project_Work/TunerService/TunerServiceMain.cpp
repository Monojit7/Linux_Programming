/*----------------------------------------------------------------------------------------------------------------------
*
* Project Harman Mid Systems - Tuner Service
* (c) copyright 2013
* Company Harman Internation
* All rights reserved
* Secrecy Level STRICTLY CONFIDENTIAL
*
*-----------------------------------------------------------------------------------------------------------------------
*
* @file TunerServiceMain.hpp
* @ingroup TunerService
* @author Sachin Athanikar
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "api/sys/dsi/pf/colibry/src/CDSIConnection.hpp"
#include "api/sys/colibry/pf/oswrapper/src/CHBThreadCtrl.hpp"
#include "api/sys/dsi/pf/colibry/src/CDSIConnection.hpp"
#include "TunerServiceMain.hpp"
#include "CTunerRequestResponseHandler.hpp"
#include "CTunerDataTypes.hpp"
#include "TraceMacros.hpp"

CTunerServiceMain* CTunerServiceMain::mpInstance                  = NULL;
BOOL CTunerServiceMain::hwVariantType = true;
bool variantCheck(Int32 argc, char **argv);

/*----------------------------------------------------------------------------------------------------------------------
 * TraceClient Scope Definitions
----------------------------------------------------------------------------------------------------------------------*/
//TBD

/*......................................................................................................................
 * @brief CTunerServiceMain()- Constructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerServiceMain::CTunerServiceMain()
{

}

/*......................................................................................................................
 * @brief CTunerServiceMain()- Destructor
 *
 * @param
 *
 * @returns None
 *
 *....................................................................................................................*/

CTunerServiceMain::~CTunerServiceMain()
{
  // TBD
}

BOOL CTunerServiceMain::getHwVariant()
{
	//printf(" \ncall the get variant function \n");
	   return hwVariantType;
}

/*......................................................................................................................
 * @brief main()
 *
 * @param
 *       
 *
 * @returns 
 *
 *....................................................................................................................*/

INT32 main( INT32 argc, STRING argv[] )
{ 
	/*std::string configFile  ;
    INT32  opt = -1;

    //Check the variant information to route the audio path
    if(argc > 1)
	{
	    while ((opt = getopt(argc, argv, "b-tp")) != -1)
	    {
           switch (opt)
	       {
	         case 'b':
	         CTunerServiceMain::hwVariantType = false; // Level B
	         printf("\n Level B board ....................\n");
	         //DBG_MSG(("Config File Path = %s",configFile.c_str()));
	         break;
	         case -1:
	         break;
	         default:
	         //DBG_ERROR(("Invalid Param to the application..."));
	         printf( "\TunerService (%s, %s)\n", __DATE__, __TIME__ );
	         break;
	       }
	     }
	   }*/
	CTunerServiceMain::hwVariantType = variantCheck(argc, argv);

   // Launch trace client
   CTraceThread::CHBConfig cfg(argc, argv, "TunerService");
   CTraceThread traceThread(cfg);

   // start th e Trace Client control thread
   HBOSTypes::TID traceClientTreadID = CHBThreadCtrl::startThread(traceThread);
   CHBThreadCtrl::setThreadName(traceClientTreadID,"TraceThread");
   // This will create the main instance and call Initialize function.
   CTunerRequestResponseHandler::GetInstance();

   // launch the DBUS router. Should never return
   CSvcIpcEventRouter *pSvcIpc = CSvcIpcEventRouter::getMe();
   pSvcIpc->start();

}
bool variantCheck(Int32 argc, char **argv)
{
   //printf("Variant Check \n");

   INT32 res = true;

   for (int index = optind; index < argc; index++)
   {
     if((strcmp(argv[index],"-b") == 0))
     {
    	// printf("Variant is Level B !!! \n");
    	 res = false;
    	 break;
     }
     else
     {
    	 //printf("Variant is Level C !!! \n");
    	 res = true;
     }
   }

   return res;
}


//----------------------------------------------------- END OF FILE ---------------------------------------------------
