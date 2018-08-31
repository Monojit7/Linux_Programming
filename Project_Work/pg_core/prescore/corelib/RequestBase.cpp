/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            RequestBase.cpp
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           RequestBase is the super class for all Requests.
*/

#include <core/RequestBase.h>
#include "RequestManager.h"


RequestBase::RequestBase(uint32_t ReqID, const char* pName)
	: mRequestID(ReqID)
	, mGrpID(0u)
	, mDynGrpID(0u)
	, mCurrentState(e_REQUEST_STATE_CREATED)
	, mName(pName)
{

}

void RequestBase::removeMe()
{
	RequestManager::findAndRemoveRequest(mGrpID, mDynGrpID, this); 
}

void RequestBase::setRequestGroup(uint64_t GroupID, uint64_t DynGrpID)
{
	mGrpID = GroupID; 
	mDynGrpID = DynGrpID;
}

/**
Ignored if the current state is not idle or ready.
*/
void RequestBase::setCompleted()
{
	if ( (e_REQUEST_STATE_READY == mCurrentState)
		|| ( e_REQUEST_STATE_IDLE == mCurrentState )
		)
	{
		setCurrentState(e_REQUEST_STATE_COMPLETED);
		requestQueueRun(); 
	}
}

/**
* Only takes effect if the current state is idle.
*/
void RequestBase::setReady()
{
	if ( e_REQUEST_STATE_IDLE == mCurrentState	)
	{
		setCurrentState(e_REQUEST_STATE_READY);
		requestQueueRun();
	}
}

/**
* Only takes effect if the current state is ready.
*/
void RequestBase::setIdle()
{
	if (e_REQUEST_STATE_READY == mCurrentState)
	{
		setCurrentState(e_REQUEST_STATE_IDLE);
	}
}

void RequestBase::setCurrentState(e_RequestState state)
{
	/**
	* Once cancelled, do not allow a state change. 
	*/
	if (e_REQUEST_STATE_CANCELLED != mCurrentState)
	{
		mCurrentState = state;
	}
	
}


void RequestBase::requestQueueRun()
{
	if (e_REQUEST_STATE_CANCELLED != mCurrentState)
	{
		RequestManager::enableQueueRun();
	}
}

RequestBase::~RequestBase()
{

}
