/*----------------------------------------------------------------------------------------------------------------------
*
*
*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/
#include "api/sys/dsi/pf/colibry/src/CDSIConnection.hpp"
#include "api/sys/colibry/pf/oswrapper/src/CHBThreadCtrl.hpp"
#include "api/sys/dsi/pf/colibry/src/CDSIConnection.hpp"
#include "PandoraServiceMain.hpp"
#include "CPandoraRequestResponseHandler.hpp"
#include "CPandoraDataTypes.hpp"
#include "TraceMacros.hpp"

CTunerServiceMain* CTunerServiceMain::mpInstance                  = NULL;


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

	struct sched_param ssParam;
	int    Policy = 0;

	pthread_getschedparam (pthread_self(), &Policy, &ssParam );

   // Launch trace client
   CTraceThread::CHBConfig cfg(argc, argv, "PandoraService");
   CTraceThread traceThread(cfg);

   // start th e Trace Client control thread
   HBOSTypes::TID traceClientTreadID = CHBThreadCtrl::startThread(traceThread);
   CHBThreadCtrl::setThreadName(traceClientTreadID,"TraceThread");

	/* Change the priority of OnOff to high, even the child threads should get this prio */
   ssParam.sched_curpriority = ssParam.sched_priority = 11;

   pthread_setschedparam (pthread_self(), Policy, &ssParam );

   pthread_setschedprio (pthread_self(),11);
   pthread_setname_np (pthread_self(), "PandoraMain");
   // This will create the main instance and call Initialize function.
   CTunerRequestResponseHandler::GetInstance();

   // launch the DBUS router. Should never return
   CSvcIpcEventRouter *pSvcIpc = CSvcIpcEventRouter::getMe();
   pSvcIpc->start();

}



//----------------------------------------------------- END OF FILE ---------------------------------------------------
