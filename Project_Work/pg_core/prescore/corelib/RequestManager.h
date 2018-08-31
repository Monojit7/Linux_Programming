/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            RequestManager.h
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           RequestManager manages and runs all the requests in the Queue.
*
*/

#include "core/PresCtrlTypes.h"
#include <core/RequestBase.h>
#include <core/LinkedList.h>
#include <map>
#include <unordered_map>
using namespace std; 

class PresCtrlCore; 

/**
* \brief  RequestManager manages and runs all the requests in the Queue.
*/
class RequestManager {

public:

private:

	struct RequestKey
	{
		RequestKey(uint64_t groupid, uint64_t dyngrpid = 0u)
			:mGroupID(groupid)
			,mGroupDynID(dyngrpid)
		{

		}

		bool operator<(const RequestKey& rhs) const
		{
			bool c1 = (mGroupID == rhs.mGroupID); 
			bool c2 = (mGroupDynID == rhs.mGroupDynID);

			if (c1)
			{
				if (c2)
				{
					return false; 
				}
				else
				{
					return (mGroupDynID < rhs.mGroupDynID) ;
				}
			}
			else
			{
				return (mGroupID < rhs.mGroupID);
			}
		}

		uint64_t mGroupID;
		uint64_t mGroupDynID;
	};

	struct KeyComparator
	{
		bool operator() (const RequestKey& a, const RequestKey& b) const 
		{
			return a < b; 
		}
	};

	/**
	* \param  reqTable       Table containing all possible Requests
	* \param  size           Size of Table
	* \brief  RequestManager constructor
	*/
	RequestManager(const std::unordered_map <uint64_t, Request> & ReqTable);

	void cancelAllinGroup(const RequestKey&);

	void cancelIntermediateRequests(const RequestKey&);

	bool isRequestAllowed(uint64_t ReqID,const Request*,const RequestKey&, e_RequestState & startingState);

	RequestBase*  findReqAndGetActiveRequests(uint64_t RequestID, const RequestKey&,uint64_t & RequestMask);

	void  getActiveRequests(const RequestKey& key, uint64_t & RequestMask);

	e_AddReqResponse addRequest(uint64_t RequestID, std::function<RequestBase*()>&& requestFunc, uint64_t DynamicGroupID = 0u);

	void runQueue();

	void findAndRemoveRequestIntern(uint64_t GrpID, uint64_t DynGrpID, RequestBase*);

	static void enableQueueRun(); 

	static void findAndRemoveRequest(uint64_t GrpID, uint64_t DynGrpID, RequestBase*); 

	friend PresCtrlCore; 
	friend RequestBase; 

	std::map<RequestKey, LinkedList<RequestBase>, KeyComparator> mReqList;

	const std::unordered_map <uint64_t,Request> & mReqTable;
	uint32_t		mReqTableSize;
	bool			mRunQueue; /// Will be set if the Queue needs to be run at the end of event loop. 
	static RequestManager* mpMyself; 
};
