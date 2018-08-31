#include <browseengine/session/SessionQueryNowPlaying.h>
#include <device/DeviceBase.h>

SessionQueryNowPlaying::SessionQueryNowPlaying(const uint32_t& sessionID,const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex,const uint64_t& numItems)
	:SessionQuery(sessionID,_client,std::move(reply),std::move(gotoPreviousReply),std::move(jumpToQueryReply),mediumID,filterIDs,filters,resultType,metadataAttributes,metadataCountTypes,type,resetPrevious,AutoUpdateEnabled,ContentMask,startIndex,numItems)
{
	Session* sess = mManager->getSession(mSessionId);
	if (sess != nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::SessionQueryNowPlaying:initialize queryId=%lu in cache",mQueryId);
		Session::SessionQueryData* data = new Session::SessionQueryData(mQueryId, mMediumId, mFilterIDMap, mFilters, mResultType, mMetadataAttributes, mMetadataCountTypes, mQueryType, mAutoUpdateEnabled, mResetPrevious, mContentMask);
		sess->insertNewQueryToCache(mQueryId,data);
	}
}

SessionQueryNowPlaying::SessionQueryNowPlaying(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
	:SessionQuery(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems)
{

}

SessionQueryNowPlaying::~SessionQueryNowPlaying()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::~SessionQueryNowPlaying");
}

void SessionQueryNowPlaying::onRun()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::onRun :queryId=%lu mMediumId=%llu", mQueryId, mMediumId);

	if (validateQueryInput())
	{
      FacadeHelper<MediaPresSettings> mSettings;
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
			determineData();
		}
	}
	else
	{
      MMPresTypes::eCallResult callResult = MMPresTypes::eCallResult::CALLRESULT_BAD_PARAMETER;
      MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;

		sendQueryCount(callResult, 0);
		MMPresTypes::MetadataItemList list;
		list.clear();
     	Session* sess = getSession();
		if (sess != nullptr)
		{
			sess->removeQueryFromCache(mQueryId);
		}
      sendQueryData(queryStatus, list);
		setCompleted();
	}
}

void SessionQueryNowPlaying::setBrowseHistoryEnumeration()
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

bool SessionQueryNowPlaying::validateQueryInput()
{
	bool paramsValid = true;

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
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::validateQueryInput-session and device are available paramsValid %d", paramsValid);
   return paramsValid;
}

bool SessionQueryNowPlaying::validateResultType()
{
	return true;
}

bool SessionQueryNowPlaying::validateFilterInput()
{
	const bool ret = true;
	return ret;
}

void SessionQueryNowPlaying::determineCount()
{
   uint64_t count = 0;
   MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
   Session* sess = getSession();
   DeviceBase* device = getDevice();
   if (sess!= nullptr)
   {
      if (device != nullptr)
      {
          if((!device->getAudiobookPodcast())||(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH != device->getDeviceType()))
           {
              if(device->mSessionEnd)
              {
                 count = device->getNowPlayingTotalTrack();
              }
              else
              {
                 count = (device->getTrackState()).getTotalTracks();
              }
           }
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::determineCount-count=%llu", count);
         if (count > 0)
         {
            Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
            if (sessionQueryData != nullptr)
            {
               sessionQueryData->mNumResults = count;
            }
            sess->setCurrentQueryId(mQueryId);

            /* Empty A-Z list should be sent to HMI only once for nowPlaying window even though user scroll down */
            device->setSmartSearchAlphaJumpCandidateRequiredToBeSend(true);
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
   determineBreadCrumbPath();
   sendQueryCount(result, count);
   setCompleted();
}

void SessionQueryNowPlaying::determineAlphajumpData()
{
   DeviceBase* getDevicePtr = getDevice(); /* CodeSonar Warning. If getDevice() NULL, this pointer in getPlayPlanQueryId will be NULL */

   if (getDevicePtr != nullptr)
   {
      std::string browsePath = getDevicePtr->getBrowsePath() + "/nowplaying";
      ::v1::org::genivi::mediamanager::BrowserTypes::SortKey sortKey;
      sortKey.setOrder(::v1::org::genivi::mediamanager::BrowserTypes::SortOrder::Literal::ASCENDING); sortKey.setKeyName("");
      mBrowser->getAlphaJumpInfo(mBrowser.mClientID,browsePath,sortKey,std::bind(&SessionQuery::getAlphaJumpInfoDataCallback,this,_1,_2,_3));
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::determineCount - calling getAlphaJumpInfo - mBrowsePath = %s", browsePath.c_str());
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::DevicePtr is NULL!!");
      JobCompleted ();
   }

}

void SessionQueryNowPlaying::QueryDataCallback(const uint64_t& offset, const uint64_t& numItems, const CommonAPI::CallStatus& status,const ::v1::org::genivi::mediamanager::MediaTypes::ResultMapList& resultList, const ::v1::org::genivi::mediamanager::PlayerTypes::PlayerError& error)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback: CallStatus=%d,PlayerError= %d,offset=%llu,numItems=%llu,resultmap size:%llu", (int)status, (int)error,offset,numItems,(uint64_t)(resultList.size()));

	uint64_t num = 0;
	Session* sess = getSession();
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	MMPresTypes::eQueryStatus queryStatus;
	MMPresTypes::MetadataItemList metadataItemList;
	metadataItemList.clear();
   DeviceBase* getDevicePtr = getDevice(); /* CodeSonar Warning. If getDevice() NULL, this pointer in getPlayPlanQueryId will ne NULL */


	FacadeHelper<MediaPresSettings> mSettings;
	if (sess != nullptr && error == PlayerTypes::PlayerError::NO_ERROR && resultList.size()>0)
	{
		num = resultList.size();
      std::string path = "";
		std::string uri = "";
		std::string name = "";
		std::string type = "";
		std::string typeex = "";
		uint64_t childcount = 0;
      uint64_t itemid = 0;
      queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;
		for (uint64_t i = offset,j=0; j<num; i++,j++)
		{
			const MediaTypes::ResultMap& result = resultList[j];
			uint64_t id = (i + 1);
			auto it = result.find(BrowseHelper::NAME);
			if (it != result.end())
			{
				name = it->second.get<std::string>();
			}

			it = result.find(BrowseHelper::TYPE);
			if (it != result.end())
			{
				type = it->second.get<std::string>();
			}

			it = result.find(BrowseHelper::TypeEx);
			if (it != result.end())
			{
				typeex = it->second.get<std::string>();
			}

			it = result.find(BrowseHelper::URI);
			if (it != result.end())
			{
				uri = it->second.get<std::string>();
			}
         if ((getDevicePtr != nullptr) && (MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH == getDevicePtr->getDeviceType()))
         {
            itemid = i + 1;
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback:-itemid was updated for BT");
         }
         else
         {
            it = result.find(BrowseHelper::ITEMID);
            if (it != result.end())
            {
               itemid = (uint64_t)(it->second.get<std::uint64_t>());
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback:-itemid id=%llu,", itemid);
            }
         }

			it = result.find(BrowseHelper::PATH);
			if (it != result.end())
			{
				path = it->second.get<std::string>();
			}

			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback - item id=%llu,name:%s,type:%s,typeex:%s,uri:%s,path:%s", itemid, name.c_str(), type.c_str(), typeex.c_str(), uri.c_str(), path.c_str());

			MMPresTypes::tMetadataItem metadataItem;
			metadataItem.setID(id);
			metadataItem.setValue(name);
			metadataItem.setPlayable(true);
			metadataItem.setBrowsable(false);
			metadataItem.setFolderType(MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_UNKNOWN);
			metadataItem.setContentType(MMPresTypes::eTitleContentType::TITLETYPE_UNKNOWN);
			metadataItem.setItemType(BrowseHelper::getItemMetadataType(type));
			metadataItemList.push_back(metadataItem);

            //setCurrentBrowsePath ( path ); 
			Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
			if (sessionQueryData != nullptr)
			{
				std::pair<std::unordered_map<uint64_t, Session::ItemData>::iterator, bool> res = sessionQueryData->itemList.insert(std::make_pair((i + 1), Session::ItemData(path, uri, (uint64_t)childcount, itemid, metadataItem)));
				if (res.second)
				{
					//	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryCallback :updated query data info successfully to cache");
				}
				else
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback :updated query data info to cache failed");
				}
			}
		} // end of for loop
	}
    else if (resultList.size() == 0)
    {
        std::string browsePath = "";
        std::vector<std::string> filter = { "*" };
        Session::SessionQueryData* prevSessionQueryData = nullptr;

        if (getDevicePtr != nullptr)
        {
           browsePath = getDevicePtr->getBrowsePath();
           prevSessionQueryData = sess->getSessionQueryData(getDevicePtr->getPlayPlanQueryId());//mQueryId-1);
        }
	
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback - browsePath1 %s ",browsePath.c_str());
        if(prevSessionQueryData != nullptr)
        {
        	if (!prevSessionQueryData->mBrowsePath.empty())
        	{
        		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback - browsePath2 %s ",prevSessionQueryData->mBrowsePath.c_str());
        		browsePath = prevSessionQueryData->mBrowsePath;
        	}
        }
        else
        {
        	browsePath = browsePath + "/" + BrowseHelper::AUDIO_FILE_ITEM_TYPE + "/" + "all";
        	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback - browsePath3 %s ",browsePath.c_str());
        }
      mBrowser->listChildren(mBrowser.mClientID, browsePath.c_str(), mStartIndex - 1, mNumItems, filter, std::bind(&SessionQueryNowPlaying::queryDataCallback, this,0, _1, _2, _3));
      setIdle();
    }
	else
	{
		queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
		result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
	}

	uint64_t index = 0;
	bool stat = checkIfWindowContainsCurrentTrack(index);
    sendQueryData(queryStatus,metadataItemList,stat,index);
    
    /* for nowPlaying list, A-Z/Smart search is not applicable as per requirements.
     * Similarly as per HMI requirement, for a screen where A-Z/Smart search is not applicable, elevator bar helper letter is not applicable
     * Hence removing below code for A-Z which was added for elevator bar helper letter.
     * mpres to send empty A-Z list to HMI for nowPlaying list and accordingly HMI will not display smart search/A-Z/Elevator bar helper letters
     * Directly thumbnail specific call should be made to CoC media
     */
#if 0
    if(MMPresTypes::eCallResult::CALLRESULT_OK == result)
	{
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback - sending minemineAlphajumpData call");
		determineAlphajumpData();
	}
	else
	{
		setCompleted();
	}
#endif

    std::string query = "";
    if(getDevicePtr != nullptr)
    {
      std::string browsepath = getDevicePtr->getBrowsePath() + "/nowplaying";
      if(mContentMask == MMPresTypes::eTitleContentType::TITLETYPE_VIDEO)
      {
          mBrowser->listThumbnails(mBrowser.mClientID,browsepath,mStartIndex-1,mNumItems,query,std::bind(&SessionQuery::listThumbnailsDataCallback,this,_1,_2,_3));
          LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback - calling listThumbnails");
      }
    	else
    	{
    		/* For audio nowPlaying window, send empty A-Z list to HMI so HMI will not show A-Z/Smart search buttons */
    		if(getDevicePtr->getSmartSearchAlphaJumpCandidateRequiredToBeSend())
    		{
    			MMPresTypes::arAlphaIndex aJListCopy;

    			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback - sending empty A-Z list to HMI");
    			sendQueryDataforAlphajump(queryStatus, aJListCopy);

    			getDevicePtr->setSmartSearchAlphaJumpCandidateRequiredToBeSend(false);
    		}

            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback - setCompleted for Audio");
    		JobCompleted();
    	}
    }
    else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::QueryDataCallback - Error: Device is NULL");
        JobCompleted();
    }
}

void SessionQueryNowPlaying::queryDataCallback(const uint64_t& totalItemCount, const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::determineData %llu , map size = %d , error = %d",totalItemCount,map.size(),(int32_t)err);
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback:totalItemCount=%llu,callStatus=%d,Browser error:%d, ResultMap size=%d", totalItemCount, (int)stat, (int)err, (int)map.size());
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback :mStartIndex=%llu,mNumItems=%llu", mStartIndex, mNumItems);
    MMPresTypes::MetadataItemList metadataItemList;
    metadataItemList.clear();
    MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
    Session* sess = getSession();
    DeviceBase* device = getDevice();
    if (device == nullptr)
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback :device not available");
        if (sess != nullptr)
        {
            sess->removeQueryFromCache(mQueryId);
        }
    }
    else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback: device available");
        if (err == BrowserTypes::BrowserError::NO_ERROR)
        {
            if (sess != nullptr)
            {
                queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;
                uint16_t num = map.size();
                int start = mStartIndex - 1;
                if (start <= 0)
                    start = 0;

                bool levelObtained = false;
                int32_t levelId = -1;

                std::string path = "";
                std::string uri = "";
                std::string name = "";
                std::string type = "";
                std::string typeex = "";
                std::string sysPath = "";
                uint64_t childcount = 0;
                uint64_t id = 0;
                uint64_t itemid = 0;


                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback: start=%d", start);
                for (uint64_t i = start, j = 0; j < num; i++, j++)
                {
                    const MediaTypes::ResultMap& result = map[j];
                    auto it = result.find(BrowseHelper::PATH);
                    if (it != result.end())
                    {
                        path = it->second.get<std::string>();
                        if (!levelObtained)
                        {
                            levelId = BrowseHelper::getLevelIdFromPath(path);
                            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback: levelId=%d", levelId);
                            levelObtained = true;

                            if(levelId < 0)
                            {
                            	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback: Invalid Level Id %d", levelId);
                            }
                            else
                            	sess->setCurrentLevelId(levelId);
                        }
                    }

                    it = result.find(BrowseHelper::URI);
                    if (it != result.end())
                    {
                        uri = it->second.get<std::string>();
                    }

                    it = result.find(BrowseHelper::FILESYS_PATH);
                    if (it != result.end())
                    {
                        sysPath = it->second.get<std::string>();
                    }

                    it = result.find(BrowseHelper::NAME);
                    if (it != result.end())
                    {
                        name = it->second.get<std::string>();
                        if (!name.compare(BrowseHelper::ALL_ITEM_NAME))
                        {
                            name = BrowseHelper::getAllItemName(mResultType);
                        }
                    }

                    it = result.find(BrowseHelper::TYPE);
                    if (it != result.end())
                    {
                        type = it->second.get<std::string>();
                    }

                    it = result.find(BrowseHelper::TypeEx);
                    if (it != result.end())
                    {
                        typeex = it->second.get<std::string>();
                    }

                    it = result.find(BrowseHelper::ITEMID);
                    if (it != result.end())
                    {
                       itemid = (uint64_t)(it->second.get<std::uint64_t>());
                    }


                    it = result.find(BrowseHelper::CHILD_COUNT);
                    if (it != result.end())
                    {
                        if (it->second.isType<int32_t>())
                        {
                            childcount = (uint64_t)(it->second.get<std::int32_t>());
                        }
                        else if (it->second.isType<uint32_t>())
                        {
                            childcount = (uint64_t)(it->second.get<std::uint32_t>());
                        }
                        else if (it->second.isType<int64_t>())
                        {
                            childcount = (uint64_t)(it->second.get<std::int64_t>());
                        }
                        else if (it->second.isType<uint64_t>())
                        {
                            childcount = (uint64_t)(it->second.get<std::uint64_t>());
                        }
                        else
                        {

                        }
                    }

                    std::string folder_name = BrowseHelper::getFolderNameFromPath(path);
                    id = (std::stoull(folder_name) + 1);
                    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback: path:%s, folder_name id=%s, item id=%llu", path.c_str(), folder_name.c_str(), id);
                    bool playable = false, browsable = false;
                    MMPresTypes::eFileSystemContentType fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_UNKNOWN;
                    MMPresTypes::eTitleContentType titleType = MMPresTypes::eTitleContentType::TITLETYPE_UNKNOWN;
                    MMPresTypes::MetadataCountMap metadataCountMap;
                    MMPresTypes::MetadataStringMap metadataStringMap;

                    if (!type.compare(BrowseHelper::AUDIO_FILE_ITEM_TYPE))
                    {
                        playable = true;
                        browsable = false;
                        fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_UNKNOWN;
                        titleType = MMPresTypes::eTitleContentType::TITLETYPE_AUDIO;
                    }
                    else if (!type.compare(BrowseHelper::VIDEO_FILE_ITEM_TYPE))
                    {
                        playable = true;
                        browsable = false;
                        fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_UNKNOWN;
                        titleType = MMPresTypes::eTitleContentType::TITLETYPE_VIDEO;
                    }
                    else if (!type.compare(BrowseHelper::IMAGE_FILE_ITEM_TYPE))
                    {
                        playable = false;
                        browsable = false;
                        fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_UNKNOWN;
                        titleType = MMPresTypes::eTitleContentType::TITLETYPE_IMAGE;
                        metadataStringMap[MMPresTypes::eMetadataType::METADATATYPE_IMAGE] = sysPath;
                    }
                    else
                    {
                        playable = false;
                        browsable = true;
                        fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_UNKNOWN;
                        titleType = MMPresTypes::eTitleContentType::TITLETYPE_UNKNOWN;
                    }

                    MMPresTypes::tMetadataItem metadataItem;
                    metadataItem.setID(id);
                    metadataItem.setValue(name);
                    metadataItem.setPlayable(playable);
                    metadataItem.setBrowsable(browsable);
                    metadataItem.setFolderType(fileSystemContentType);
                    metadataItem.setContentType(titleType);
                    metadataItem.setItemType(BrowseHelper::getItemMetadataType(typeex));
                    metadataItem.setMetadataMap(metadataStringMap);

                    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback: query result type=%llu,actual item type=%llu", mResultType, metadataItem.getItemType());

                    /* this is a junk condition which will be hit for audiobooks and podcasts because CoC gives same browse item Type for albums ,audiobooks and podcasts.??
                    if (!(metadataItem.getItemType() == mResultType))
                    {
                        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: resetting the item type to the expected result type");
                        metadataItem.setItemType(mResultType);
                    }*/

                    metadataItemList.push_back(metadataItem);

                    Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
                    if (sessionQueryData != nullptr)
                    {
                    	if(levelId < 0)
                    	{
                    		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback: Invalid Level Id %d", levelId);
                    	}
                    	else
                    		sessionQueryData->mLevelId = levelId;

                        std::pair<std::unordered_map<uint64_t, Session::ItemData>::iterator, bool> res = sessionQueryData->itemList.insert(std::make_pair((i + 1), Session::ItemData(path, uri, childcount, itemid, metadataItem)));
                        if (res.second)
                        {
                            //  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback :updated query data info successfully to cache");
                        }
                        else
                        {
                            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback :updated query data info to cache failed");
                        }
                    }
                } // end of for loop
            } // end of sess==nullptr check
        } // end of err=browseerror check
    }// end of device check

    //sendQueryData(queryStatus, metadataItemList);

    uint64_t index = 0;
    bool stat1 = checkIfWindowContainsCurrentTrack(index);
    sendQueryData(queryStatus,metadataItemList,stat1,index);

#if 0 //original code is commented
    if((!metadataItemList.empty()) && (metadataItemList.size() > 0))
    {
        //determineAlphajumpData();
    }
    else
    {
        setCompleted();
    }
#endif

    std::string query = "";
    if(device != nullptr)
    {
    	std::string browsepath = device->getBrowsePath() + "/nowplaying";
    	if(mContentMask == MMPresTypes::eTitleContentType::TITLETYPE_VIDEO)
    	{
    		mBrowser->listThumbnails(mBrowser.mClientID,browsepath,mStartIndex-1,mNumItems,query,std::bind(&SessionQuery::listThumbnailsDataCallback,this,_1,_2,_3));
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback - calling listThumbnails");
    	}
    	else
    	{
         /* For audio nowPlaying window, send empty A-Z list to HMI so HMI will not show A-Z/Smart search buttons */
         if(device->getSmartSearchAlphaJumpCandidateRequiredToBeSend())
         {
            MMPresTypes::arAlphaIndex aJListCopy;

            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback - sending empty A-Z list to HMI");
            sendQueryDataforAlphajump(queryStatus, aJListCopy);

            device->setSmartSearchAlphaJumpCandidateRequiredToBeSend(false);
         }

         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback - setCompleted for Audio");
         JobCompleted();
      }
    }
    else
    {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::queryDataCallback - Error: Device is NULL");
      JobCompleted();
    }
}

void SessionQueryNowPlaying::determineData()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::determineData");

	std::unordered_map<uint64_t, Session::ItemData>::iterator itr;
	MMPresTypes::MetadataItemList itemList;
	itemList.clear();
	MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_PROCESSING;
	Session* sess = getSession();
	if (sess != nullptr)
	{
		Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
		if (sessionQueryData != nullptr)
		{
			queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;

			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::determineQueryData-startIndex=%llu,total query count=%llu", mStartIndex, (sessionQueryData->mNumResults));

		    if ((sessionQueryData->mNumResults) > 0)
			{
				if (mNumItems > (sessionQueryData->mNumResults))
				{
					mNumItems = sessionQueryData->mNumResults;
				}

				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::determineQueryData-startIndex=%llu,mNumItems=%llu", mStartIndex,mNumItems);

			    mPlayer->getCurrentPlayQueueEx(mPlayer.mClientID,(mStartIndex-1),mNumItems,std::bind(&SessionQueryNowPlaying::QueryDataCallback, this,(mStartIndex-1),mNumItems,_1,_2,_3));
				setIdle();
			}// end of sessionQueryData->mNumResults check
		} // end of sessionQueryData
		else
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::determineQueryData-sessionquerydata is invalid");
			queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
		}
	}// end of session
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryNowPlaying::determineQueryData-session is invalid");
		queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
	}
}

