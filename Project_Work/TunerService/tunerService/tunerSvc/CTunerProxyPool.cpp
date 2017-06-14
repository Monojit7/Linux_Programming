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
* @file CTunerCmdInvoker.hpp
* @ingroup TunerService
* @author Reshma Roy
* Public API for the Tuner core service library.
*
*---------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------
 * Preprocessor Includes
----------------------------------------------------------------------------------------------------------------------*/


#include "CTunerProxyPool.hpp"

CTunerProxyPool::CTunerProxyPool()
{

}
CTunerProxyPool::~CTunerProxyPool()
{

}

//bool CTunerProxyPool::Initialize()

/*......................................................................................................................
 * @brief handleTunerRequest()
 *
 * @param
 *
 * @returns 0 : Success
 *         -1 : Failure
 *
 *....................................................................................................................*/

// this needs to e changed - create an array of methods and write a compare fn.

CTunerStationProxy* CTunerProxyPool::GetTunerStationProxy()
{
	return mpTunerStationProxy;
}




//===================================================== End Of File ===================================================
