#include <device/DeviceRemoteCD.h>
#include <browseengine/session/SessionQueryRemoteCDNowPlaying.h>

SessionQueryRemoteCDNowPlaying::SessionQueryRemoteCDNowPlaying(const uint32_t& sessionID,const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex,const uint64_t& numItems)
	:SessionQuery(sessionID,_client,std::move(reply),std::move(gotoPreviousReply),std::move(jumpToQueryReply),mediumID,filterIDs,filters,resultType,metadataAttributes,metadataCountTypes,type,resetPrevious,AutoUpdateEnabled,ContentMask,startIndex,numItems), msubscribestatus(false),mTimer(*this), timerId(0), mRequestTimerExpired(false)
{
	Session* sess = mManager->getSession(mSessionId);
	if (sess != nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::SessionQueryRemoteCDNowPlaying:initialize queryId=%lu in cache",mQueryId);
		Session::SessionQueryData* data = new Session::SessionQueryData(mQueryId, mMediumId, mFilterIDMap, mFilters, mResultType, mMetadataAttributes, mMetadataCountTypes, mQueryType, mAutoUpdateEnabled, mResetPrevious, mContentMask);
		sess->insertNewQueryToCache(mQueryId,data);
	}
	
}

SessionQueryRemoteCDNowPlaying::SessionQueryRemoteCDNowPlaying(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
	:SessionQuery(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems), msubscribestatus(false),mTimer(*this), timerId(0), mRequestTimerExpired(false)
{

}

SessionQueryRemoteCDNowPlaying::~SessionQueryRemoteCDNowPlaying()
{
    cancelTimer();
    unsubscribeBrowseEvent();
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::~SessionQueryRemoteCDNowPlaying");
}



void SessionQueryRemoteCDNowPlaying::subscribeBrowseEvent()
{
    if (not msubscribestatus)
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::subscribeBrowseEvent");
        mBrowselistTokenMap [ EVENT_BROWSECONTROL_BROWSEINFO_STATUS ] = PresEventAbstraction::getInstance()->subscribe<browseStatus>(EVENT_BROWSECONTROL_BROWSEINFO_STATUS, std::bind(&SessionQueryRemoteCDNowPlaying::queryDataCallback, this,_1, _2));

        mBrowselistTokenMap [ EVENT_BROWSECONTROL_TOTAL_FOLDER_COUNT ] = PresEventAbstraction::getInstance()->subscribe<browseTotalCount>(EVENT_BROWSECONTROL_TOTAL_FOLDER_COUNT, std::bind( & SessionQueryRemoteCDNowPlaying::queryDataCountCallback , this, _1));
        

        msubscribestatus = true;
    }
}

void SessionQueryRemoteCDNowPlaying::unsubscribeBrowseEvent()
{
    if( msubscribestatus)
    {
        //std::pair <RemoteCDEvents, uint32_t>::iterator it = mTockenList.find(EVENT_BROWSECONTROL_BROWSEINFO_STATUS);
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::PRESEVENT_UNSUBSCRIBE");
        PRESEVENT_UNSUBSCRIBE( EVENT_BROWSECONTROL_BROWSEINFO_STATUS, mBrowselistTokenMap [ EVENT_BROWSECONTROL_BROWSEINFO_STATUS ] );
        PRESEVENT_UNSUBSCRIBE( EVENT_BROWSECONTROL_TOTAL_FOLDER_COUNT, mBrowselistTokenMap [ EVENT_BROWSECONTROL_TOTAL_FOLDER_COUNT ] );
        msubscribestatus = false;
    }
}

void SessionQueryRemoteCDNowPlaying::onRun()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::onRun :queryId=%lu and object address : %p :mMediumId=%llu", mQueryId, this, mMediumId);

	FacadeHelper<MediaPresSettings> mSettings;

	MMPresTypes::eCallResult callResult;
	MMPresTypes::eQueryStatus queryStatus;
    
    subscribeBrowseEvent();

	if (validateQueryInput())
	{
        if (mSettings->getBrowseHistoryEnumeration())
		{
			setBrowseHistoryEnumeration();
		}
		
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

void SessionQueryRemoteCDNowPlaying::setBrowseHistoryEnumeration()
{
	DeviceBase* getDevicePtr = getDevice(); /* CodeSonar Warning. If getDevice() NULL, this pointer in getPlayPlanQueryId will ne NULL */

	if(getDevicePtr != nullptr)
	{
		if (getDevicePtr->getPlayPlanQueryId()>0)
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::setBrowseHistoryEnumeration-setting browse transition to last browse path");
				getDevicePtr->setBrowseHistoryEnumeration(MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_NOWPLAYINGDATA);
			}
			else
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::setBrowseHistoryEnumeration-setting browse transition to root");
				getDevicePtr->setBrowseHistoryEnumeration(MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_ROOT);
			}

	}
	else
	{
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::setBrowseHistoryEnumeration - Unable to get Device Pointer ");
	}

}

bool SessionQueryRemoteCDNowPlaying::validateQueryInput()
{
	bool paramsValid = true;

	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::validateQueryInput-session and device are available");

	if (mStartIndex == 0 && mNumItems == 0)
	{
		if (mQueryCallback != nullptr)
		{
			if (validateResultType())
			{
				if (!validateFilterInput())
				{
					paramsValid = false;
				}
			}
			else
			{
				paramsValid = false;
			}
		}
	}
	return paramsValid;
}

bool SessionQueryRemoteCDNowPlaying::validateResultType()
{
	return true;
}

bool SessionQueryRemoteCDNowPlaying::validateFilterInput()
{
	bool ret = true;

	return ret;
}


void SessionQueryRemoteCDNowPlaying::determineCount()
{
#if 0
	uint64_t count = 0;
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	Session* sess = getSession();
	DeviceBase* device = getDevice();
	if (sess!= nullptr)
	{
		if (device != nullptr)
		{
			count = mRemoteCDPlayBackEngine -> mTrackStateRemoteCD. getTotalTracks();
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::determineCount-count=%lu", count);
			if (count > 0)
			{
				Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
				if (sessionQueryData != nullptr)
				{
                sessionQueryData->mNumResults = count;
				}
				sess->setCurrentQueryId(mQueryId);
			}
		}
		else
		{
			result = MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED;
		}
	}
	else
	{
		result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
	}
	sendQueryCount(result, count);
	setCompleted();
	
#endif


    DLT_LOGS_UPDATED();
    Session::SessionQueryData* currData = nullptr;
    if(getSession() != nullptr)
    {
    	currData = getSession()->getSessionQueryData(mQueryId);
    	if (currData != nullptr)
    	{
    		// this->setItemRequestData();
    		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::determineQueryItemList-mContainerId=%llu,mStartIndex=%llu,mNumItems=%llu", mContainerId, mStartIndex, mNumItems);
    		//startTimer();
    		//if ( mRemoteCDPlayBackEngine->getPluginInstance() != nullptr) mRemoteCDPlayBackEngine->getPluginInstance()->getFolderItemCount( mRemoteCDPlayBackEngine->getActiveFolderNumber() );

    		//setIdle();
            

    uint64_t TotalCount =  ( uint64_t ) mRemoteCDPlayBackEngine -> getFolderItemCount ( (int ) mRemoteCDPlayBackEngine->getActiveFolderNumber() ) ;  

    
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d < TotalCount: %llu>", __FUNCTION__ , __LINE__, TotalCount );;
    MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
    Session* sess = mManager->getSession(mSessionId);
    cancelTimer();
    
if (sess != nullptr)
	{
		Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
        if (sessionQueryData != nullptr)
        {
            
            sessionQueryData->mContainerId = mRemoteCDPlayBackEngine->getActiveFolderNumber();
            if ( TotalCount > 0 )
            {
                sess->insertToQueryList(mQueryId);
                sess->setCurrentQueryId(mQueryId);
                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d Sending Query Count to HMI <TotalCount: %d>", __FUNCTION__ , __LINE__, TotalCount );
               sendQueryCount(result, TotalCount);
               setCompleted();
            }
        else
          {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::determineQueryItemList-mContainerId=%llu,mStartIndex=%llu,mNumItems=%llu", mContainerId, mStartIndex, mNumItems);
    		startTimer();
            if ( mRemoteCDPlayBackEngine->getPluginInstance() != nullptr) mRemoteCDPlayBackEngine->getPluginInstance()->getFolderItemCount( mRemoteCDPlayBackEngine->getActiveFolderNumber() );

    		setIdle();
        }
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::queryCountCallback :updated query parameter info to cache");
        }
	}
   


    
                

            
    	}
    }
}


bool SessionQueryRemoteCDNowPlaying::CheckIfWindowCachedCD ( MMPresTypes::MetadataItemList& itemList )
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


void SessionQueryRemoteCDNowPlaying::determineQueryItemList()
{
    std::vector<std::string> filter = { "*" };

    MMPresTypes::MetadataItemList itemList;
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineData:retrieve the item list for queryId=%lu,startIndex=%llu,numItems=%llu", mQueryId, mStartIndex, mNumItems);
	bool ret = CheckIfWindowCachedCD(itemList);
    
    LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d <ActiveFolderNumber : %d , TotalItem : %d>", __FUNCTION__, __LINE__, mRemoteCDPlayBackEngine->getActiveFolderNumber() , mRemoteCDPlayBackEngine->getActiveFolderTotalItem () );
    
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
         currData = getSession()->getSessionQueryData(mQueryId);
    
        if (currData != nullptr)
        {
        // this->setItemRequestData();
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::determineQueryItemList-mContainerId=%llu,mStartIndex=%llu,mNumItems=%llu", mRemoteCDPlayBackEngine->getActiveFolderNumber(), mStartIndex, mNumItems);

            mFolderRequest = make_shared <FolderRequestedData> ();
            startTimer();
            this->mContainerId = mRemoteCDPlayBackEngine->getActiveFolderNumber() ;
            this->setItemRequestData();
            DLT_LOGS_UPDATED();
            mRemoteCDPlayBackEngine->getPluginInstance()->getBrowseInfo(mFolderRequest);
            DLT_LOGS_UPDATED();
            setIdle();

        }
    }
}


FolderRequest SessionQueryRemoteCDNowPlaying::getItemRequestData()
{
    return mFolderRequest;
}

void SessionQueryRemoteCDNowPlaying::setItemRequestData()
{
    DLT_LOGS_UPDATED();
    
    mFolderRequest->setNumber( mRemoteCDPlayBackEngine->getActiveFolderNumber() ); 
    mFolderRequest->setItemNameLen(20); // maximum 20 characters are supported in CD player for each item name
    mFolderRequest->setCount(mNumItems);
    mFolderRequest->setStartIndex( mStartIndex - 1);
    mFolderRequest->setFolderContent(FolderRequestedData::eRequestedFolder);
    
   /* 
    mFolderRequest->setNumber( mRemoteCDPlayBackEngine->getActiveFolderNumber() ); // we have to call getRequestedFolderNumber() for this
    mFolderRequest->setItemNameLen(20); // maximum 20 characters are supported in CD player for each item name*/
    
     DLT_LOGS_UPDATED();
     /*
     if ( ( (unsigned int)( mRemoteCDPlayBackEngine->mFolderCount + mRemoteCDPlayBackEngine->mFileCount ) > ( mRemoteCDPlayBackEngine ->
         mTrackStateRemoteCD. getTotalTracks()  ) && ( mRemoteCDPlayBackEngine->mFolderCount + mRemoteCDPlayBackEngine->mFileCount ) != 0 )
    )
     {
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d < Total Items: %d, numOfItems %d > ", __FUNCTION__ , __LINE__ ,  ( mRemoteCDPlayBackEngine->mFolderCount + mRemoteCDPlayBackEngine->mFileCount ), mNumItems  );
    mFolderRequest->setCount( ( mRemoteCDPlayBackEngine->mFolderCount + mRemoteCDPlayBackEngine->mFileCount ) <  20 ? ( mRemoteCDPlayBackEngine->mFolderCount + mRemoteCDPlayBackEngine->mFileCount ) : mNumItems );
     }
    else 
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying:: < numOfItems : %d>:", mNumItems );
        mFolderRequest->setCount ( mRemoteCDPlayBackEngine -> mTrackStateRemoteCD. getTotalTracks() );
    }*/
     
     /*
    mFolderRequest->setStartIndex( mStartIndex -1 );
     
    mFolderRequest->setFolderContent(FolderRequestedData::eRequestedFolder);
    DLT_LOGS_UPDATED();*/
}

short SessionQueryRemoteCDNowPlaying::getRequestedFolderNumber()
{
    return (short)mContainerId;
}
void SessionQueryRemoteCDNowPlaying::queryDataCallback(Folder mFolderData, bool status)
{
    DLT_LOGS_UPDATED();
    cancelTimer();
    MMPresTypes::MetadataItemList metadataItemList;
    metadataItemList.clear();
    MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
    Session* sess = mManager->getSession(mSessionId);
    DeviceBase* device = mDetect->getDevice((uint64_t)mMediumId);
    uint64_t itemid = 0;

    if (device == nullptr)
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::queryDataCallback :device not available: medium id : %d", mMediumId);
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
                mRemoteCDPlayBackEngine -> setCurrentFolder ( FolderNum );
               /* if( ( totalItems > 0 ) && ( mStartIndex == 0 ) && ( mNumItems == 0 ) )
                {
                    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::queryDataCallback :Sending Query Count : %d", totalItems);
                    MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
                    sendQueryCount ( result , totalItems ) ; 
                    setCompleted();
                    return;
                }*/
                
                short parentFolderNum = mFolderData->getParent ();
                string FolderName = mFolderData->getName ();
                ItemList mItemList = mFolderData->getItemsList ();
                mRemoteCDPlayBackEngine -> insertFolderItemCount ( (int ) FolderNum, (int ) totalItems );
                 LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::queryDataCallback  FolderItemCount : %d ",mRemoteCDPlayBackEngine ->getFolderItemCount ( FolderNum )  );

                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::queryDataCallback :FolderNum: %d, totalItems: %d, parentFolderNum: %d, FolderName: %s", FolderNum, totalItems, parentFolderNum, FolderName.c_str() );

              //  if ( mFolderData->getNumber () == getRequestedFolderNumber() )
               // {

                    MMPresTypes::MetadataCountMap metadataCountMap;
                    MMPresTypes::MetadataStringMap metadataStringMap;
                    uint64_t childcount = 0;
                    for ( auto it = mItemList.begin(); it != mItemList.end(); it++ )
                    {
                       // if ( ( ( ( *it ) ->getIndex () >= mStartIndex ) ) &&  ( ( *it ) ->getIndex () <=  (mStartIndex + mNumItems -1  ) ) )
                        //{
                        childcount++;
                        TrackNumber mTrackNumber = (*it)->getNumber ();
                        MMPresTypes::tMetadataItem metadataItem;
                        metadataItem.setPlayable(false);
                        metadataItem.setBrowsable(false);
                        if ( mediaSettings -> is_valid_utf8 (  (*it)->getName().c_str () ) )
                        {
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
                        metadataStringMap[MMPresTypes::eMetadataType::METADATATYPE_FILE] = metadataItem.getValue () ;
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
                            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDFS::queryDataCallback: Track num :%d foldername : %s ", getTrackId(mTrackNumber), metadataItem.getValue ().c_str() );
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
                                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::queryDataCallback :updated query data to Media Cache");
                                //	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryCallback :updated query data info successfully to cache");
                            }
                            else
                            {
                                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying::queryDataCallback :updated query data info to cache failed");
                            }
                        }

                        
                        
                  //  } // end of if check of bound checking
                   // else
                    //{
                      //  LOG_INFO( LOG_CONTEXT_MEDIA, "<<< SessionQueryRemoteCDNowPlaying:: Index out of bound :%d", ( *it ) ->getIndex () );
                        //queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED ;
                    //}
                }// end of for loop
            }// end of device check
        } //
    } // device not null 
   
    
    sendQueryData(queryStatus, metadataItemList);
    setCompleted(); 

}

void SessionQueryRemoteCDNowPlaying::startTimer()
{
	/*
	 * This timer is introduced to handle the following case
	 * If a corrupted CD is inserted and user tries to browse in it, We are not getting the response
	 * from the player with in 5 and also sometimes onrun() of SessionQueryRemoteCDNowPlaying is not called as expected,
	 * Since we didn't get the response and a request is in process other requests following are cancelled and entire browse
	 * is not functional for other media as well.
	 *
	 * So to avoid the above case this timer has been introduced, If we didn't get the response for the browse request
	 * with 4 seconds we will sent a empty reply to HMI.
	 */
	timerId = mTimer.setTimer(CTimerInterface::TIMER_ONESHOT,6000);

	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SessionQueryRemoteCDNowPlaying::startTimer-timerId=%d", timerId);
	mRequestTimerExpired = false;
}
void SessionQueryRemoteCDNowPlaying::cancelTimer()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SessionQueryRemoteCDNowPlaying::cancelTimer");
	mTimer.cancelTimer(timerId);
	timerId = 0;

	mRequestTimerExpired = false;
}

void SessionQueryRemoteCDNowPlaying::timerEvent(int timerId)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SessionQueryRemoteCDNowPlaying::timerEvent-timerId=%d expired", timerId);
	mRequestTimerExpired = true;
	MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;
    MMPresTypes::MetadataItemList metadataItemList;
    metadataItemList.clear();
    sendQueryData(queryStatus, metadataItemList);
    setCompleted();

}

void SessionQueryRemoteCDNowPlaying::queryDataCountCallback (  short  TotalCount )
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

int SessionQueryRemoteCDNowPlaying::getTrackId(TrackNumber mTrackNumber)
{
    //return  ( mTrackNumber.second );
    
    LOG_INFO(LOG_CONTEXT_MEDIA, " %s %d <folderNumber: 1st value  : %d , 2nd value itemIndex : %d >", __FUNCTION__, __LINE__ ,  mTrackNumber.first, mTrackNumber.second )
    return (((mTrackNumber.first) << 16 ) | (mTrackNumber.second));
}
