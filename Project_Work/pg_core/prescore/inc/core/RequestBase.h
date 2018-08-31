/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            RequestBase.h
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           RequestBase is the super class for all Requests. 
*/

#ifndef _REQUEST_BASE_
#define _REQUEST_BASE_

#include <stdint.h>
#include <string>
#include <core/PresCtrlTypes.h>

class RequestManager; 
class DLL_EXPORT RequestBase
{
public:
	RequestBase(uint32_t,const char* pName);


	virtual ~RequestBase();
	
	/**
	* All the business logic triggered by the request must go in this method, and not in the constructor. 
	* Run is automatically called for newly created requests, if they are eligible to run as per the RequestTable / Status of other requests. 
	* Run will be called again and again for a request if the request state is set to ready. 
	* So, if you called a service api here, and are waiting for the response, you must change the request state to Waiting. 
	*/
	virtual void onRun() = 0;

	/**
	* cancel will be immediately followed by destructor invocation for the Request. 
	* This can be called in multiple cases, including the request being cancelled because of a higher priority request as per the table. 
	* You might use it to send error responses to the appropriate layer. 
	* Indicates that the request was cancelled abruptly. 
	*/
	virtual void onCancelled() = 0;


protected:

	/**
	Ignored if the current state is not idle or ready. 
	*/
	void setCompleted(); 

	/**
	* Only takes effect if the current state is idle. 
	*/
	void setReady();

	/**
	* Only takes effect if the current state is ready. 
	*/
	void setIdle();
	

	void requestQueueRun(); 

	/**
	* To be called only when request handles its own deletion and wants to cleanup the associated request manager resources.
	*/
	void removeMe();


private : 

	void setCurrentState(e_RequestState state);

	void setRequestGroup(uint64_t GroupID,uint64_t DynGrpID = 0u); 

	friend RequestManager; 

	uint64_t mRequestID;
	uint64_t mGrpID;
	uint64_t mDynGrpID;
	e_RequestState mCurrentState;

	const char* mName; // Mainly for Debug Purposes. 
};


#endif