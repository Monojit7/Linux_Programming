#include <device/DeviceRemoteCD.h>
#include <browseengine/session/SessionQueryRemoteCDFS.h>

SessionQueryRemoteCDFS::SessionQueryRemoteCDFS(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask, const uint64_t& startIndex, const uint64_t& numItems)
    :SessionQuery(sessionID,_client,std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems), msubscribestatus(false),mTimer(*this), timerId(0), mRequestTimerExpired(false)
{
    Session* sess = mManager->getSession(sessionID);
    if (sess != nullptr)
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::SessionQueryRemoteCDFS:initialize queryId=%lu, mResultType: %d in cache ", mQueryId, mResultType);
        sess->setCurrentQueryId (mQueryId);
        Session::SessionQueryData* data = new Session::SessionQueryData(mQueryId, mMediumId, mFilterIDMap, mFilters, mResultType, mMetadataAttributes, mMetadataCountTypes, mQueryType, mAutoUpdateEnabled, mResetPrevious, mContentMask);
        sess->insertNewQueryToCache(mQueryId, data);
    }


}

SessionQueryRemoteCDFS::SessionQueryRemoteCDFS(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
    :SessionQuery(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems), msubscribestatus(false),mTimer(*this), timerId(0), mRequestTimerExpired(false)
{

}

SessionQueryRemoteCDFS::~SessionQueryRemoteCDFS()
{
    cancelTimer();
    unsubscribeBrowseEvent();
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::~SessionQueryRemoteCDFS");

}
// this is a callback for the browse request to mediaone.This is used for computing the count(currently done only for computing item count of the root folder)

void SessionQueryRemoteCDFS::subscribeBrowseEvent()
{
    if (not msubscribestatus)
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::PRESEVENT_SUBSCRIBE");
        mBrowselistTokenMap [ EVENT_BROWSECONTROL_BROWSEINFO_STATUS ] = PresEventAbstraction::getInstance()->subscribe<browseStatus>(EVENT_BROWSECONTROL_BROWSEINFO_STATUS, std::bind( & SessionQueryRemoteCDFS::queryDataCallback , this,_1, _2));
        
        mBrowselistTokenMap [ EVENT_BROWSECONTROL_TOTAL_FOLDER_COUNT ] = PresEventAbstraction::getInstance()->subscribe<browseTotalCount>(EVENT_BROWSECONTROL_TOTAL_FOLDER_COUNT, std::bind( & SessionQueryRemoteCDFS::queryDataCountCallback , this, _1));

        msubscribestatus = true;
    }
}

void SessionQueryRemoteCDFS::unsubscribeBrowseEvent()
{
    if( msubscribestatus)
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::PRESEVENT_UNSUBSCRIBE");
        PRESEVENT_UNSUBSCRIBE( EVENT_BROWSECONTROL_BROWSEINFO_STATUS, mBrowselistTokenMap [ EVENT_BROWSECONTROL_BROWSEINFO_STATUS ] );
        PRESEVENT_UNSUBSCRIBE( EVENT_BROWSECONTROL_TOTAL_FOLDER_COUNT, mBrowselistTokenMap [ EVENT_BROWSECONTROL_TOTAL_FOLDER_COUNT ] );
        
        msubscribestatus = false;
    }
}

void SessionQueryRemoteCDFS::onRun()
{

    LOG_INFO(LOG_CONTEXT_MEDIA, "<<<SessionQueryRemoteCDFS::onRun <queryId=%lu,mMediumId=%llu, startIndex: %d, NumofItems: %d>", mQueryId ,mMediumId, mStartIndex, mNumItems);

    MMPresTypes::eCallResult callResult;
    MMPresTypes::eQueryStatus queryStatus;


    subscribeBrowseEvent();

    if (validateQueryInput())
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::validateQueryInput");
        
        if (mStartIndex == 0 && mNumItems == 0)
		{
            
			determineCount();
            
		}
		else
        {
           determineQueryItemList();
        }

    }
    else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::validateQueryInput - Not valid");
        
        callResult = MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER;
        queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
        sendQueryCount(callResult, 0);
        MMPresTypes::MetadataItemList list;
        list.clear();
        sendQueryData(queryStatus, list);
        Session* sess = getSession();
        if (sess != nullptr)
        {
            sess->removeQueryFromCache(mQueryId);
        }
        setCompleted();
    }
}

bool SessionQueryRemoteCDFS::validateQueryInput()
{
    bool paramsValid = true;

    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::validateQueryInput-session and device are available");

   if (!validateFilterInput())
   {
      paramsValid = false;
   }

    return paramsValid;
}

void SessionQueryRemoteCDFS::determineCount()
{
    DLT_LOGS_UPDATED();
    if(getSession() != nullptr)
    {
       Session::SessionQueryData* currData = nullptr;
    	currData = getSession()->getSessionQueryData(mQueryId);
    	if (currData != nullptr)
    	{
           // this->setItemRequestData();
    		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::determineQueryItemList-mContainerId=%llu,mStartIndex=%llu,mNumItems=%llu", mContainerId, mStartIndex, mNumItems);
    		startTimer();
    		if ( mRemoteCDPlayBackEngine->getPluginInstance() != nullptr) mRemoteCDPlayBackEngine->getPluginInstance()->getFolderItemCount(mContainerId);

    		setIdle();
    	}
    }
}


bool SessionQueryRemoteCDFS::CheckIfWindowCachedCD (  MMPresTypes::MetadataItemList& itemList )
{
    bool ret = false;
	uint64_t endIndex = 0;
	uint64_t i = mStartIndex;
	std::unordered_map<uint64_t, Session::ItemData>::iterator itr;
	Session::SessionQueryData* currData = nullptr;
   if (nullptr != getSession())
   {
      currData = getSession()->getSessionQueryData(mQueryId);
   }

	if (currData != nullptr)
	{
		endIndex = mStartIndex + mNumItems - 1;
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::checkIfWindowDataCached-browsePath=%s,startIndex=%llu,endIndex=%llu,total Query count=%llu", (currData->mBrowsePath).c_str(), mStartIndex, endIndex, currData->mNumResults);
		if ((currData->mNumResults) > 0)
		{
			if (endIndex > (currData->mNumResults))
			{
				endIndex = currData->mNumResults;
			}

			while (i <= endIndex)
			{
				itr = currData->itemList.find(i);
				if (itr != currData->itemList.end())
				{
					ret = true;
					itemList.push_back(itr->second.item);
				}
				else
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::checkIfWindowDataCached-key:%llu is not cached", i);
					ret = false;
					itemList.clear();
					break;
				}
				i++;
			}
		}
	}

	return ret;
}

bool SessionQueryRemoteCDFS::validateResultType()
{
   return BrowseHelper::checkIfFileSystemBrowseItemSupported(mResultType);
}

bool SessionQueryRemoteCDFS::validateFilterInput()
{
    bool ret = true;
    bool inputMatched = false;

    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::validateFilterInput-filterIDMap size=%d", (int)(mFilterIDMap.size()));
    std::unordered_map<MMPresTypes::eMetadataType, uint64_t>::const_iterator itr;

    if (!mFilterIDMap.empty())
    {
        itr = mFilterIDMap.begin();
        while (itr != mFilterIDMap.end())
        {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::validateFilterInput-filterIDMap key=%d,value=%d", (int)(itr->first), (int)(itr->second));
            if ((itr->first)==(MMPresTypes::eMetadataType::METADATATYPE_FOLDER))
            {
                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::validateFilterInput-filterID map contains a valid key");
                mContainerId = itr->second;
                inputMatched = true;
                break;
            }
            itr++;
        }
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::validateFilterInput-mContainerId=%llu", mContainerId);
        if (!inputMatched)
        {
            ret = false;
        }
    }
    else
    {
        mContainerId = 0;
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::validateFilterInput-mFilterIDMap is empty...so equivalent to obtaining root level data");
    }
    return ret;
}

void SessionQueryRemoteCDFS::determineQueryItemList()
{
    MMPresTypes::MetadataItemList itemList;
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineData:retrieve the item list for queryId=%lu,startIndex=%llu,numItems=%llu", mQueryId, mStartIndex, mNumItems);
	bool ret = CheckIfWindowCachedCD(itemList);
    
    if (ret)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<<SessionQuery::determineData:send data from cache");
		MMPresTypes::eQueryStatus stat = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;
		sendQueryData(stat, itemList);
        setCompleted();
    }
    
    else
    {
        Session::SessionQueryData* currData = nullptr;

        if(getSession() != nullptr)
            currData = getSession()->getSessionQueryData(mQueryId);

        mFolderRequest = make_shared <FolderRequestedData> ();
        if (currData != nullptr)
        {
            // this->setItemRequestData();
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::determineQueryItemList-mContainerId=%llu,mStartIndex=%llu,mNumItems=%llu", mContainerId, mStartIndex, mNumItems);
            startTimer();
            this->setItemRequestData();
            if ( mRemoteCDPlayBackEngine->getPluginInstance() != nullptr) mRemoteCDPlayBackEngine->getPluginInstance()->getBrowseInfo(getItemRequestData());
            setIdle();

        }
    }
}

FolderRequest SessionQueryRemoteCDFS::getItemRequestData()
{
    return mFolderRequest;
}

void SessionQueryRemoteCDFS::setItemRequestData()
{
    DLT_LOGS_UPDATED()
    mFolderRequest->setNumber(getRequestedFolderNumber());
    mFolderRequest->setItemNameLen(20); // maximum 20 characters are supported in CD player for each item name
    mFolderRequest->setCount(mNumItems);
    mFolderRequest->setStartIndex( mStartIndex - 1);
    mFolderRequest->setFolderContent(FolderRequestedData::eRequestedFolder);
    DLT_LOGS_UPDATED()
}

short SessionQueryRemoteCDFS::getRequestedFolderNumber()
{
    return (short)mContainerId;
}
void SessionQueryRemoteCDFS::queryDataCallback(Folder mFolderData, bool status)
{
    
    cancelTimer();
    MMPresTypes::MetadataItemList metadataItemList;
    metadataItemList.clear();
    MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
    Session* sess = mManager->getSession(mSessionId);
    DeviceBase* device = mDetect->getDevice((uint64_t)mMediumId);
    uint64_t itemid = 0;

    if (device == nullptr)
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::queryDataCallback :device not available: medium id : %d", mMediumId);
        if (sess != nullptr)
        {
            sess->removeQueryFromCache(mQueryId);
        }
    }
    else
    {
        if (status == true)
        {
            if (sess != nullptr)
            {
                queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;
                short FolderNum = mFolderData->getNumber ();
                short totalItems = mFolderData->getTotalItems ();
                mRemoteCDPlayBackEngine -> insertFolderItemCount ( (int ) FolderNum, (int ) totalItems );
                 LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::queryDataCallback  FolderItemCount : %d ",mRemoteCDPlayBackEngine ->getFolderItemCount ( FolderNum )  );
                mRemoteCDPlayBackEngine -> setCurrentFolder ( FolderNum );
               /* if( ( totalItems > 0 ) && ( mStartIndex == 0 ) && ( mNumItems == 0 ) )
                {
                    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::queryDataCallback :Sending Query Count : %d", totalItems);
                    MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
                    sendQueryCount ( result , totalItems ) ; 
                    setCompleted();
                    return;
                }*/
                
                short parentFolderNum = mFolderData->getParent ();
                string FolderName = mFolderData->getName ();
                ItemList mItemList = mFolderData->getItemsList ();

                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::queryDataCallback :FolderNum: %d, totalItems: %d, parentFolderNum: %d", FolderNum, totalItems, parentFolderNum );

              //  if ( mFolderData->getNumber () == getRequestedFolderNumber() )
               // {

                    MMPresTypes::MetadataCountMap metadataCountMap;
                    MMPresTypes::MetadataStringMap metadataStringMap;
                    uint64_t childcount = 0;
                    for ( auto it = mItemList.begin(); it != mItemList.end(); it++ )
                    {
                        childcount++;
                        TrackNumber mTrackNumber = (*it)->getNumber ();
                        MMPresTypes::tMetadataItem metadataItem;
                        metadataItem.setPlayable(false);
                        metadataItem.setBrowsable(false);
                        if ( mediaSettings -> is_valid_utf8 (  (*it)->getName().c_str () ) )
                        {
                            LOG_INFO( LOG_CONTEXT_MEDIA, ">>> %s %d valid File Name %s  ", __FUNCTION__ , __LINE__ , (*it)->getName().c_str () );
                           metadataItem.setValue((*it)->getName());
                        }
                        else
                        {
                             LOG_INFO(LOG_CONTEXT_MEDIA, ">>> %s %d invalid File Name   ", __FUNCTION__ , __LINE__ );
                             metadataItem.setValue ( mediaSettings -> getInvalidString () );
                        }
                        switch((*it)->getType ())
                        {
                        case eFile:
                        {
                            metadataItem.setID(getTrackId(mTrackNumber));
                            metadataItem.setPlayable(true);
                            metadataItem.setFolderType(MMPresTypes::eFileSystemContentType::Literal::FS_CONTENT_TYPE_FILE);
                            metadataItem.setContentType(MMPresTypes::eTitleContentType::Literal::TITLETYPE_AUDIO);
                            metadataItem.setItemType(MMPresTypes::eMetadataType::METADATATYPE_FILE);
                            metadataStringMap[MMPresTypes::eMetadataType::METADATATYPE_FILE] = metadataItem.getValue ();
                            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::queryDataCallback: Track num :%d filename : %s ", getTrackId(mTrackNumber), metadataItem.getValue ().c_str() );
                            DLT_LOGS_UPDATED();
                        }
                        break;
                        case eFolder:
                        {
                            metadataItem.setID(mTrackNumber.first);
                            metadataItem.setBrowsable(true);
                            metadataItem.setFolderType(MMPresTypes::eFileSystemContentType::Literal::FS_CONTENT_TYPE_FOLDER);
                            metadataItem.setItemType(MMPresTypes::eMetadataType::METADATATYPE_FOLDER);
                            metadataItem.setContentType(MMPresTypes::eTitleContentType::TITLETYPE_UNKNOWN);
                            metadataStringMap[MMPresTypes::eMetadataType::METADATATYPE_FOLDER] = metadataItem.getValue () ;
                            mRemoteCDPlayBackEngine -> insertFolderName ( mTrackNumber.first , (*it)->getName() ); 
                            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::queryDataCallback: Track num :%d , FolderName : %s ", mTrackNumber.first, metadataItem.getValue ().c_str() );
                            DLT_LOGS_UPDATED();
                        }
                        break;
                        case ePlayList:
                        {
                        }
                        break;
                        default:
                            break;
                        }

                        metadataItem.setMetadataMap(metadataStringMap);
                        metadataItemList.push_back(metadataItem);
                        Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
                        if (sessionQueryData != nullptr)
                        {
                            sessionQueryData->mNumResults = totalItems;
                            sessionQueryData->mContainerId = mContainerId;
                            // Level ID needs to be added
                            //sessionQueryData->mLevelId = levelId;
                            std::pair<std::unordered_map<uint64_t, Session::ItemData>::iterator, bool> res = sessionQueryData->itemList.insert(std::make_pair( static_cast < uint64_t > ( (*it)->getIndex () ) , Session::ItemData("", "", (uint64_t)childcount,itemid, metadataItem)));
                            if (res.second)
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::queryDataCallback :updated query data to Media Cache");
                                //	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::queryCallback :updated query data info successfully to cache");
                            }
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::queryDataCallback :updated query data info to cache failed");
                            }
                        }
    
                    }
            }// end of device check
        }
    }
    LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d sending the QueryData to HMI ",  __FUNCTION__, __LINE__ );
    sendQueryData(queryStatus, metadataItemList);
    setCompleted();
}

void SessionQueryRemoteCDFS::queryDataCountCallback (  short  TotalCount )
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d Sending Query Count to HMI <TotalCount: %d>", __FUNCTION__ , __LINE__, TotalCount );
    MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
    Session* sess = mManager->getSession(mSessionId);
    
    cancelTimer();
   if (sess != nullptr)
	{
		Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
		if (sessionQueryData != nullptr)
      {
         sessionQueryData->mNumResults = TotalCount;
         
       //  if ( mContainerId != 0 )
             mBreadCrumbPath = mRemoteCDPlayBackEngine -> getFolderName ( mContainerId ) ;
        // else
          //   mBreadCrumbPath = "";
         LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d  <FolderName :%s >",__FUNCTION__,__LINE__ ,mBreadCrumbPath.c_str () );
			sessionQueryData->mContainerId = mContainerId;
         if ( TotalCount > 0 )
         {
            sess->insertToQueryList(mQueryId);
            sess->setCurrentQueryId(mQueryId);
         }
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::queryCountCallback :updated query parameter info to cache");
		}
	}
    
    sendQueryCount(result, TotalCount);
    setCompleted();
}

void SessionQueryRemoteCDFS::startTimer()
{
	/*
	 * This timer is introduced to handle the following case
	 * If a corrupted CD is inserted and user tries to browse in it, We are not getting the response
	 * from the player with in 5 and also sometimes onrun() of SessionQueryRemoteCDFS is not called as expected,
	 * Since we didn't get the response and a request is in process other requests following are cancelled and entire browse
	 * is not functional for other media as well.
	 *
	 * So to avoid the above case this timer has been introduced, If we didn't get the response for the browse request
	 * with 4 seconds we will sent a empty reply to HMI.
	 */
	timerId = mTimer.setTimer(CTimerInterface::TIMER_ONESHOT,10000);

	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SessionQueryRemoteCDFS::startTimer-timerId=%d", timerId);
	mRequestTimerExpired = false;
}
void SessionQueryRemoteCDFS::cancelTimer()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SessionQueryRemoteCDFS::cancelTimer");
	mTimer.cancelTimer(timerId);
	timerId = 0;

	mRequestTimerExpired = false;
}

void SessionQueryRemoteCDFS::timerEvent(int timerId)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SessionQueryRemoteCDFS::timerEvent-timerId=%d expired", timerId);
   mRequestTimerExpired = true;
   if (mStartIndex == 0 && mNumItems == 0)
   {
      queryDataCountCallback(0);
   }
   else
   {
      MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;
      MMPresTypes::MetadataItemList metadataItemList;
      metadataItemList.clear();
      sendQueryData(queryStatus, metadataItemList);
      setCompleted();
   }
}

int SessionQueryRemoteCDFS::getTrackId(TrackNumber mTrackNumber)
{
    return (((mTrackNumber.first) << 16 ) | (mTrackNumber.second));
}
