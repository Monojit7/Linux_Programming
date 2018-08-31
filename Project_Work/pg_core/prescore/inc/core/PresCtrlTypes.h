/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            PresCtrlTypes.h
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           User defined types used by Pres Ctrl
*/

#ifndef _PRESCTRLTYPES_H_
#define _PRESCTRLTYPES_H_

#include <stdint.h>
#include <functional>

#ifdef PLATFORM_WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT 
#endif


enum e_RequestBehavior
{
	 e_REQUEST_IGNORE_THIS_IF_OLD_ACTIVE			= 0x0000000000000001u // Request will not be added and will fail with an error if same request is pending in same group. 
	,e_REQUEST_WAIT_TILL_SAME_REQ_FINISHES			= 0x0000000000000002u // The request will be in blocked state till the old request in the same group can finish. 
	,e_REQUEST_CANCEL_OLD_REQ						= 0x0000000000000004u  // Discard any previous request with the same ID
	,e_REQUEST_CANCEL_ALL_GRP						= 0x0000000000000008u // Discard all previous requests in the same group as the request. 
	,e_REQUEST_CANCEL_INTERMEDIATE_REQUESTS         = 0x0000000000000010u // Discard the pending queue requests except the current active/executing request
};

enum e_AddReqResponse
{
	e_REQUEST_ADDED,
	e_REQUEST_NOT_ALLOWED,
	e_REQUEST_INVALID
};

enum e_RequestState
{
	e_REQUEST_STATE_CREATED=0u,//Default State during creation. 
	e_REQUEST_STATE_READY, // This state is internally set by the RequestManager if all criteria is met as per the table. This State can also be by the user when coming out of idle mode. 
	e_REQUEST_STATE_WAITING_DEPENDENCIES, // RequestManager is waiting for the dependency requests to finish 
	e_REQUEST_STATE_WAITING_SAME_REQUEST_FINISH,//Request Manager is waiting for the same request to finish
	e_REQUEST_STATE_IDLE,//The requests in this state are still considered for request management, but RM will not call the Run method if Idle. Requests in this state will still block other requests as per the table. 
	e_REQUEST_STATE_CANCELLED,//Request has been cancelled, calls onCancelled and destroys. 
	e_REQUEST_STATE_COMPLETED//Request completed, followed by destruction. 
};

/**
* \brief  This struct denotes each table entry in the RequestTable
*/
struct Request
{
	Request(const char* name,uint64_t grpid,uint64_t behav,uint64_t deps)
		:mName(name)
		,RequestGroupID(grpid)
		,RequestBehavior(behav)
		,RequestDependencies(deps)
	{

	}

	const char* mName; 	
	uint64_t	RequestGroupID;/// A static grouping of requests into different groups. 
	uint64_t	RequestBehavior;///Behavior denoting the impact of the request on group and existing requests of the same type. 
	uint64_t    RequestDependencies;/// Bitmask combination of all the requests that this request depends on. In this case, the request will be put in wait state until these requests finish. 
};

#endif
