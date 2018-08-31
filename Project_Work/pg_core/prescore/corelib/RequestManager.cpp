/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            RequestManager.cpp
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           RequestManager manages and runs all the requests in the Queue. 
*/

#include "RequestManager.h"
#include <functional>
#include <core/RequestBase.h>
#include <core/logging.h>
#include <core/CommonAPIClientAbstraction.h>

RequestManager* RequestManager::mpMyself = nullptr; 

RequestManager::RequestManager(const std::unordered_map <uint64_t, Request> & ReqTable)
	:mReqTable(ReqTable)
	,mReqTableSize(ReqTable.size())
	, mRunQueue(false)
{
	mpMyself = this; 
}

void RequestManager::enableQueueRun()
{
	mpMyself->mRunQueue = true; 
}

e_AddReqResponse RequestManager::addRequest(uint64_t RequestID, std::function<RequestBase*()>&& requestFunc, uint64_t DynamicGroupID)
{

	e_AddReqResponse response = e_REQUEST_INVALID; 

	/**
	* Check Request ID and process
	*/
		const Request* pNewReq= &mReqTable.at(RequestID);	
		RequestKey key(pNewReq->RequestGroupID, DynamicGroupID);

		/**
		* Check if we are even allowed to add this request. 
		*/
		e_RequestState startingstate; 

		if (isRequestAllowed(RequestID,pNewReq,key, startingstate))
		{
			RequestBase* pRequest = requestFunc();
			pRequest->setRequestGroup(pNewReq->RequestGroupID, DynamicGroupID);
			pRequest->setCurrentState(startingstate); 
			mReqList[key].push_tail(pRequest);
			response = e_REQUEST_ADDED;

			enableQueueRun(); /// Explicitly enabling queue run because new request has been added. 
			runQueue();
		}
		else
		{
			response = e_REQUEST_NOT_ALLOWED;
		}

	return response; 

}


void RequestManager::cancelAllinGroup(const RequestKey& key)
{
	RequestBase* pReq = nullptr;
	mReqList[key].initializeCurrent();
	while (nullptr != (pReq = mReqList[key].peekNext()))
	{
		pReq->setCurrentState(e_REQUEST_STATE_CANCELLED);
	}
}

void RequestManager::cancelIntermediateRequests(const RequestKey& key)
{
	RequestBase* pReq = nullptr;
	mReqList[key].initializeCurrent();
	while (nullptr != (pReq = mReqList[key].peekNext()))
	{
		if (pReq->mCurrentState == e_REQUEST_STATE_WAITING_SAME_REQUEST_FINISH)
		{
			pReq->setCurrentState(e_REQUEST_STATE_CANCELLED);
			LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "RequestName %s , status : CANCELLED", pReq->mName);
		}
	}
}

RequestBase*  RequestManager::findReqAndGetActiveRequests(uint64_t RequestID, const RequestKey& key, uint64_t & RequestMask)
{
	RequestBase* tmp = nullptr;
	RequestBase* pReqReturn = nullptr;
	mReqList[key].initializeCurrent();
	while (nullptr != (tmp = mReqList[key].peekNext()))
	{
		if ( ( nullptr == pReqReturn ) &&
			(tmp->mRequestID == RequestID) &&
			(e_REQUEST_STATE_COMPLETED != tmp->mCurrentState) &&
			(e_REQUEST_STATE_CANCELLED != tmp->mCurrentState)
			)
		{
			pReqReturn = tmp; 
		}
		if ((e_REQUEST_STATE_COMPLETED != tmp->mCurrentState)
			&& (e_REQUEST_STATE_CANCELLED != tmp->mCurrentState)
			)
		{
			RequestMask |= tmp->mRequestID;
		}
		
	}

	return pReqReturn;
}

void  RequestManager::getActiveRequests(const RequestKey& key, uint64_t & RequestMask)
{
	RequestBase* tmp = nullptr;
	mReqList[key].initializeCurrent();
	while (nullptr != (tmp = mReqList[key].peekNext()))
	{
		if ( (e_REQUEST_STATE_COMPLETED != tmp->mCurrentState ) 
			&& (e_REQUEST_STATE_CANCELLED != tmp->mCurrentState)
			)
		{
			RequestMask |= tmp->mRequestID;
		}

	}
}

bool RequestManager::isRequestAllowed(uint64_t ReqID, const Request* pReq,const RequestKey& key, e_RequestState & startingState)
{
	/**
	* Check if the same request is already found in the Queue. 
	*/
	bool bAddToQueue = true; 
	uint64_t nExistingRequestsPending = 0u; 

	RequestBase* pExisting = findReqAndGetActiveRequests(ReqID, key, nExistingRequestsPending);

	startingState = e_REQUEST_STATE_READY;

	if (pReq->RequestDependencies & nExistingRequestsPending)
	{
		startingState = e_REQUEST_STATE_WAITING_DEPENDENCIES;
	}

	if (pReq->RequestBehavior & e_REQUEST_CANCEL_ALL_GRP)
	{
		/**
		* Whole group needs to be cleared. Other flags become irrelevant here. 
		* If this flag is used, other flags are ignored. 
		* Do cleanup and return. 
		*/
		cancelAllinGroup(key);
		startingState = e_REQUEST_STATE_READY;
	}
	else if (pReq->RequestBehavior & e_REQUEST_CANCEL_OLD_REQ)
	{
		if (nullptr != pExisting)
		{
			pExisting->setCurrentState(e_REQUEST_STATE_CANCELLED);
		}
	}
	else if ( pReq->RequestBehavior & e_REQUEST_IGNORE_THIS_IF_OLD_ACTIVE  )
	{
		
		if (nullptr != pExisting)
		{
			/**
			* Will not add this to Queue. Inform the requestor that they have been ignored. 
			*/
			bAddToQueue = false; 
		}
	}
	else if (pReq->RequestBehavior & e_REQUEST_WAIT_TILL_SAME_REQ_FINISHES)
	{
		/**
		* Other handling. 
		*/
		if (nullptr != pExisting)
		{
			startingState = e_REQUEST_STATE_WAITING_SAME_REQUEST_FINISH;
		}
	}
	else if (pReq->RequestBehavior & e_REQUEST_CANCEL_INTERMEDIATE_REQUESTS)
	{
		cancelIntermediateRequests(key);

		if (nullptr != pExisting)
		{
			startingState = e_REQUEST_STATE_WAITING_SAME_REQUEST_FINISH;
		}
	}

	return bAddToQueue;
}

/**
* Run the internal queue. 
*/
void RequestManager::runQueue()
{
	if (!mRunQueue)
	{
		return; 
	}
	mRunQueue = false; 

	LOG_DEBUG(Log::e_LOG_CONTEXT_CORE,"******************Debug Queue Run Status Output!********************")
	auto it = mReqList.begin();
	while (mReqList.end() != it)
	{
		uint64_t nActiveRequests = 0u; 
		std::unordered_map<uint64_t, int8_t> nRequestsUnderProcessing;

		getActiveRequests(it->first, nActiveRequests);

		LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "Request Group Begin Group ID %llu : Dynamic Group ID :%llu , Active Requests in Group : %llu", it->first.mGroupID, it->first.mGroupDynID, nActiveRequests);
		RequestBase* pReq = nullptr; 
		it->second.initializeCurrent();
		while (nullptr != (pReq = it->second.peekNext()))
		{
			switch (pReq->mCurrentState)
			{
				case e_REQUEST_STATE_CREATED:
				{
					LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "RequestName %s , status : CREATED", pReq->mName);
					nRequestsUnderProcessing[pReq->mRequestID];
				}
				break;
				case e_REQUEST_STATE_CANCELLED:
				{
					LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "RequestName %s , status : Cancelled", pReq->mName);
					pReq->onCancelled(); 
					delete pReq;
					it->second.deleteCurrentAndSetPrev(); 
					break;
				}
				
				case e_REQUEST_STATE_READY:
				{
					LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "RequestName %s , status : Ready", pReq->mName);
					pReq->setIdle();
					pReq->onRun();
					if(pReq->mCurrentState != e_REQUEST_STATE_COMPLETED)
						nRequestsUnderProcessing[pReq->mRequestID];
					break;
				}
				


				case e_REQUEST_STATE_WAITING_DEPENDENCIES:
				{
					/**
					* This will only work if request IDs are power of 2
					*/
					if ( !  (mReqTable.at(pReq->mRequestID).RequestDependencies & nActiveRequests ))
					{
						LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "RequestName %s , status : READY", pReq->mName);
						pReq->setCurrentState(e_REQUEST_STATE_READY);
						pReq->onRun(); 
					}
					else
					{
						LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "RequestName %s , status : WAIT_DEPENDS", pReq->mName);
						
					}
					if (pReq->mCurrentState != e_REQUEST_STATE_COMPLETED)
						nRequestsUnderProcessing[pReq->mRequestID];
				}
				break;
				case e_REQUEST_STATE_WAITING_SAME_REQUEST_FINISH:
				{		
					if (nRequestsUnderProcessing.find(pReq->mRequestID) == nRequestsUnderProcessing.end())
					{
						/**
						* A request with this ID finished processing. 
						* Since the list are in order of creation, set this request to the next state. 
						*/
						if ( mReqTable.at(pReq->mRequestID).RequestDependencies & nActiveRequests)
						{
							LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "RequestName %s , status : WAIT_DEPENDS", pReq->mName);
							pReq->setCurrentState(e_REQUEST_STATE_WAITING_DEPENDENCIES);
						}
						else
						{
							LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "RequestName %s , status : Ready", pReq->mName);
							pReq->setCurrentState(e_REQUEST_STATE_READY);
							pReq->onRun();
						}
						if (pReq->mCurrentState != e_REQUEST_STATE_COMPLETED)
							nRequestsUnderProcessing[pReq->mRequestID];
					}
					else
					{
						LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "RequestName %s , status : WAIT_SAME_FINISH", pReq->mName);
					}
				}
				break;

				case e_REQUEST_STATE_IDLE:
				{
					LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "RequestName %s , status : Idle", pReq->mName);
					nRequestsUnderProcessing[pReq->mRequestID];
					break;
				}


				case e_REQUEST_STATE_COMPLETED:
				{
					LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "RequestName %s , status : COMPLETED", pReq->mName);
					delete pReq; 
					it->second.deleteCurrentAndSetPrev(); 
				}
				break;
				default : 
					LOG_ERROR(Log::e_LOG_CONTEXT_CORE, "Undefined Request State"); 
					break;
			}


		}
		
		LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "Request Group End Group ");
		it++;
	}
	LOG_DEBUG(Log::e_LOG_CONTEXT_CORE, "******************Debug Queue Run Status Output Finished!**********")
}

void RequestManager::findAndRemoveRequestIntern(uint64_t GrpID, uint64_t DynGrpID, RequestBase* req)
{
	RequestKey key(GrpID, DynGrpID);
	mReqList[key].initializeCurrent();
	RequestBase* pReq = nullptr; 
	while (nullptr != (pReq = mReqList[key].peekNext()))
	{
		if (pReq == req)
		{
			//Found, break. 
			mReqList[key].searchAndDestroy(req); 
			break; 
		}
	}
}

void RequestManager::findAndRemoveRequest(uint64_t GrpID, uint64_t DynGrpID, RequestBase* req)
{
	mpMyself->findAndRemoveRequestIntern(GrpID,DynGrpID, req);
}