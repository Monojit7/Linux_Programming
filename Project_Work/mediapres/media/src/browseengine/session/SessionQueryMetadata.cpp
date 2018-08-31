#include <browseengine/session/SessionQueryMetadata.h>
#include <device/DeviceBase.h>

SessionQueryMetadata::SessionQueryMetadata(const uint32_t& sessionID,const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex,const uint64_t& numItems)
	:SessionQuery(sessionID,_client,std::move(reply),std::move(gotoPreviousReply),std::move(jumpToQueryReply),mediumID,filterIDs,filters,resultType,metadataAttributes,metadataCountTypes,type,resetPrevious,AutoUpdateEnabled,ContentMask,startIndex,numItems)
{
	Session* sess = mManager->getSession(mSessionId);
	if (sess != nullptr)
	{
		mIsSmartSearch = false;

		Session::SessionQueryData* data = new Session::SessionQueryData(mQueryId, mMediumId, mFilterIDMap, mFilters, mResultType, mMetadataAttributes, mMetadataCountTypes, mQueryType, mAutoUpdateEnabled, mResetPrevious, mContentMask);
		sess->insertNewQueryToCache(mQueryId,data);

		/* Below code identify whether current query is for smart search or not.
		 * For smart search, below are various conditions
		 * 1. After selection of smart search button, HMI call setSessionQuery() with filterIDmap & filterMap as empty. For all leveles of smart search
		 * 2. Once available characters are displayed on keyboard, user presses any valid character, HMI calls setSessionQuery() with filterIDMap as empty, but
		 * filterMap will not be empty.
		 * 3. This is applicable only for Metadata_Browse and hence verify previous data also whether user is entering Smart search from which category.
		 */
		Session::SessionQueryData* prevData = nullptr;
		Session* tmpSesson = nullptr;
		tmpSesson = getSession();

		if(tmpSesson != nullptr)
		{
			uint32_t currQueryId = tmpSesson->getCurrentQueryId();
			prevData = tmpSesson->getSessionQueryData(currQueryId);
		}

		if(prevData != nullptr)
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::SessionQueryMetadata:initialize queryId=%lu in cache, prevData->mQueryType = %d ",mQueryId, prevData->mQueryType);

			/* Below boolean variable is used further to identify smart search or normal browsing.
			 * This is required for getting correct browse path for getting correct smart search results.
			 */
			if((mFilterIDMap.empty()) && ((prevData->mQueryType != MMPresTypes::eSessionQueryTypes::SESSION_QUERY_CATEGORY_BROWSE) || !mFilters.empty()))
				mIsSmartSearch = true;
		}

		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::SessionQueryMetadata:initialize queryId=%lu in cache, mIsSmartSearch = %d ",mQueryId, mIsSmartSearch);
	}
}

SessionQueryMetadata::SessionQueryMetadata(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
	:SessionQuery(sessionID,queryID,_client,std::move(reply),std::move(gotoPreviousReply),std::move(jumpToQueryReply),queryCacheData,startIndex,numItems)
{

}

SessionQueryMetadata::~SessionQueryMetadata()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::~SessionQueryMetadata : %d ", this );
}

void SessionQueryMetadata::onRun()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::onRun-queryId=%lu", mQueryId);

	MMPresTypes::eCallResult callResult;
	MMPresTypes::eQueryStatus queryStatus;

	if(validateQueryInput())
	{
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
		 JobCompleted () ;
	}
}

bool SessionQueryMetadata::validateQueryInput()
{
	bool paramsValid = true;

		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::validateQueryInput-session and device are available");

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

bool SessionQueryMetadata::validateResultType()
{
	return BrowseHelper::checkIfMetadataBrowseItemSupported(mResultType);
}

bool SessionQueryMetadata::validateFilterInput()
{
	bool ret = true;
	bool inputMatched = false;
	Session::SessionQueryData* currData = nullptr;
	Session::SessionQueryData* prevData = nullptr;
	Session* tmpSession = nullptr;
	uint32_t currQueryId;

	tmpSession = getSession();

	if(tmpSession != nullptr)
	{
		currData    = tmpSession->getSessionQueryData(mQueryId);
		currQueryId = tmpSession->getCurrentQueryId();
		prevData    = tmpSession->getSessionQueryData(currQueryId);
	}
	MMPresTypes::eMetadataType currType;
	MMPresTypes::eMetadataType prevType;

	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::validateFilterInput-filterIDMap size=%d", (int)(mFilterIDMap.size()));
	std::unordered_map<MMPresTypes::eMetadataType, uint64_t>::const_iterator itr;

	if (!mFilterIDMap.empty())
	{
		if ((prevData != nullptr) && (currData != nullptr))
		{
			itr = mFilterIDMap.begin();

			while (itr != mFilterIDMap.end())
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::validateFilterInput-filterIDMap key=%d,value=%d, prevData->mResultType=%d", (int)(itr->first), (int)(itr->second), (int)(prevData->mResultType));

				if ((prevData->mResultType) == (itr->first))
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::validateFilterInput-filterID map contains an input metadata type same as the previous query metadata type");
					mContainerId = itr->second;
					inputMatched = true;
					break;
				}
				itr++;
			}

			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::validateFilterInput-mContainerId=%llu, inputMatched=%d", mContainerId, (int)inputMatched);

			if (inputMatched)
			{
				ret = BrowseHelper::checkIfBrowseItemTypeValidForTheBrowseLevel(prevData->mResultType, currData->mResultType);
			}
			else
			{
				ret = false;
			}
		}
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::validateFilterInput-mFilterIDMap is empty...so equivalent to obtaining root level data");
	}

	return ret;
}

void SessionQueryMetadata::determineCount()
{
	uint64_t totCount = 0;
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	FacadeHelper<MediaPresSettings> mSettings;
	DeviceBase* pDevice = getDevice();
	if (mGotoPreviousCallback != nullptr || mJumpToQueryCallback!=nullptr)
	{
		/* for each smart search i.e. character search, set this flag so smart search & alpha jump characters will be provided.
		 * This is only for setSessionQuery
		 */
		if(pDevice != nullptr)
		{
			pDevice->setSmartSearchAlphaJumpCandidateRequiredToBeSend(true);
		}
		processAndSendCachedCount();
	}
	else
	{
		if (!mFilters.empty())
		{
			/* for each smart search i.e. character search, set this flag so smart search & alpha jump characters will be provided.
			 * This is only for setSessionQuery
			 */
			if(pDevice != nullptr)
			{
				pDevice->setSmartSearchAlphaJumpCandidateRequiredToBeSend(true);
			}

			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCount -get search count for sessionQuery");
			determineCountForSearchQuery();
		}
		else if (mSettings->getAttributeUsageForCount())
		{
			/* This is for folder browsing level. i.e Artist, albums etc. We have to provide smart search and alpha jump information.
			 * This is only for setSessionQuery
			 */
			if(pDevice != nullptr)
			{
				pDevice->setSmartSearchAlphaJumpCandidateRequiredToBeSend(true);
			}
			determineCountViaGetAttribute();
		}
		else
		{
			if (mContainerId==0)
			{
				determineRootLevelCategoryCount();
			}
			else
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCount-get cached count for sessionQuery");
				totCount = getQueryCountFromCachedItemList();
				 Session* getSessionPtr = getSession();
				 if(getSessionPtr != nullptr)
				 {
					 if (totCount > 0)
					 {
						 /* this is only for normal browsing and it is not for smart search. Do not remember previous query for smart search back button.*/
						 if(mFilters.empty())
						        getSessionPtr->insertToQueryList(mQueryId);

						 getSessionPtr->setCurrentQueryId(mQueryId);
					 }
					 else
					 {
						 getSessionPtr->removeQueryFromCache(mQueryId);
					 }
				 }
				 else
				 {
					 LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCount- ERROR : Session Pointer is NULL");
				 }
				determineBreadCrumbPath();
				sendQueryCount(result, totCount);
				 JobCompleted () ;
			}
		}
	}
}

void SessionQueryMetadata::determineRootLevelCategoryCount()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineRootLevelCategoryCount:we have to find the count of category %lu mContentMask %d", mResultType, mContentMask);
	std::string browsePath = "";
	std::string categoryName = "";
	std::string mediaType = "";
	if (getDevice() != nullptr)
	{
		browsePath = getDevice()->getBrowsePath();
		browsePath.append("/");
		switch (mContentMask)
		{
		case MMPresTypes::eTitleContentType::TITLETYPE_VIDEO:
			browsePath.append(BrowseHelper::VIDEO_CATEGORY_NAME);
			break;
		case MMPresTypes::eTitleContentType::TITLETYPE_IMAGE:
			browsePath.append(BrowseHelper::IMAGE_CATEGORY_NAME);
			break;
		case MMPresTypes::eTitleContentType::TITLETYPE_AUDIO:
		default:
			browsePath.append(BrowseHelper::MUSIC_CATEGORY_NAME);
			break;
		}
		categoryName = BrowseHelper::getContainerName(mResultType);
	}
	std::vector<std::string> filter = { "*" };
	mBrowser->listChildren(mBrowser.mClientID, browsePath, 0, 0, filter, std::bind(&SessionQueryMetadata::categoryCountCallback, this, browsePath, categoryName, mediaType,_1, _2, _3));
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineRootLevelCategoryCount:browsePath=%s,categoryName=%s", browsePath.c_str(), categoryName.c_str());

	setIdle();
}

void SessionQueryMetadata::categoryCountCallback(const std::string& parentPath, const std::string& categoryName, const std::string& mediaType, const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
    if ( ( stat == CommonAPI::CallStatus::SUCCESS ) && ( err == ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError::NO_ERROR  ) )
    {
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::categoryCountCallback :CallStatus : %d BrowserError %d count:%d mediaType:%s", (int)stat, (int)err, map.size(),mediaType.c_str());
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	DeviceBase* device = getDevice();
	uint16_t count = map.size();
	std::string itemPath = "";
	std::string itemName = "";
	uint64_t childCount = 0;
	std::string uri = "";
	Session* sess = getSession();
	if (device != nullptr)
	{
		Session::SessionQueryData* queryData = nullptr;
		if (sess != nullptr)
		{
			queryData = sess->getSessionQueryData(mQueryId);

			if (queryData != nullptr)
			{
				if (err == BrowserTypes::BrowserError::NO_ERROR)
				{
					for (uint16_t i = 0; i < count; i++)
					{
						const MediaTypes::ResultMap& result = map[i];
						auto it = result.find(BrowseHelper::PATH); // get the path
						if (it != result.end())
						{
							itemPath = it->second.get<std::string>();
							itemName = BrowseHelper::getFolderNameFromPath(itemPath);

							MMPresTypes::eMetadataType type = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;

							if (!itemName.compare(categoryName.c_str()))
							{
								type = BrowseHelper::getContainerId(itemName,mediaType);

								if (MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN != type) // check for containerID
								{
									it = result.find(BrowseHelper::CHILD_COUNT); // get child count
									if (it != result.end())
									{
										if (it->second.isType<int32_t>())
										{
											childCount = (uint64_t)(it->second.get<std::int32_t>());
										}
										else if (it->second.isType<uint32_t>())
										{
											childCount = (uint64_t)(it->second.get<std::uint32_t>());
										}
										else if (it->second.isType<int64_t>())
										{
											childCount = (uint64_t)(it->second.get<std::int64_t>());
										}
										else if (it->second.isType<uint64_t>())
										{
											childCount = (uint64_t)(it->second.get<std::uint64_t>());
										}
										else
										{

										}
										LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::categoryCountCallback:itemName=%s,childcount : %llu", itemName.c_str(), childCount);
									}// child count check
								} // item metadata type check
								break;
							}// category name check
						}// itr check
					} // for loop
					queryData->mBrowsePath = itemPath;
					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::categoryCountCallback:itemPath=%s", itemPath.c_str());
					queryData->mNumResults = childCount;
					queryData->mContainerId = mContainerId;
				} // browse error check
				else
				{
					result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
				}
			} // session query data existence check
			else
			{
				result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
			}
		} // session existence check
		else
		{
			result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
		}
	}// device existence check
	else
	{
		result = MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED;
		/*if (sess != nullptr)
		{
			sess->removeQueryFromCache(mQueryId);
		}*/
	}
	if (childCount > 0)
	{
		/* this is only for normal browsing and it is not for smart search. Do not remember previous query for smart search back button.*/
      if (sess != nullptr)
      {
         if (mFilters.empty())
            sess->insertToQueryList(mQueryId);

         sess->setCurrentQueryId(mQueryId);
      }
      else
      {
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::categoryCountCallback-GetSession ptr is NULL!!!");
      }
	}
	else
	{
      if (sess != nullptr)
   		sess->removeQueryFromCache(mQueryId);
	}
	determineBreadCrumbPath();
	sendQueryCount(result, childCount);
	 JobCompleted () ;
    }
    else
    {
         JobCompleted ();
    }
}

void SessionQueryMetadata::determineQueryItemList()
{
	std::vector<std::string> filter = { "*" };
	std::string query = "";
	Session::SessionQueryData* currData = nullptr;
   if (getSession() != nullptr)
   {
      currData = getSession()->getSessionQueryData(mQueryId);
   }
	if (currData != nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineQueryItemList-mContainerId=%llu,browsePath=%s,mStartIndex=%llu,mNumItems=%llu", mContainerId,currData->mBrowsePath.c_str(), mStartIndex, mNumItems);

#if 1
		/* For Job-1 release, as per confirmation from Ford, smart search is not required.
		 * Only alpha-jump is required in all regions.
		 * So instead of searchObjects(), use earlier code i.e. listChildren() to get data from mediaone.
		 *
		 * Smart search will be required for Job-2 release as per confirmation of Ford.
		 * So once smart search will be required, just use #else code of this.
		 */
		if (!mFilters.empty())
		{
			auto it = mFilters.begin();
         if (it->first == MMPresTypes::eFilterType::BEGINS_WITH)
         {
            query = "startswith ";
         }
         else if (it->first == MMPresTypes::eFilterType::CONTAINS)
         {
            query = "contains ";
         }
         else
         {
            //Do Nothing
         }

			query.append(it->second);
			mBrowser->searchObjects(mBrowser.mClientID,currData->mBrowsePath,query,mStartIndex - 1,mNumItems,filter,std::bind(&SessionQueryMetadata::searchQueryDataCallback,this,_1,_2,_3));
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineQueryItemList-query=%s", query.c_str());

		}
		else
		{
			mBrowser->listChildren(mBrowser.mClientID, currData->mBrowsePath, mStartIndex - 1, mNumItems, filter, std::bind(&SessionQueryMetadata::queryDataCallback, this, currData->mNumResults, _1, _2, _3));
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineQueryItemList- Calling listChildren to get data");
		}
#else

		/* Below functionality is added for smart search implementation.
		 * Earlier "listChildren()" is used to get folder level details and smart search was handled through "searchObjects()"
		 * Now as per new modifications from CoC media, searchObjects() will do both jobs depending upon "query" being sent to CoC media.
		 */
		if (!mFilters.empty())
		{
			/* this is for smart search */
			auto it = mFilters.begin();

			if(it->first == MMPresTypes::eFilterType::BEGINS_WITH)
				query = "containPhonetic ";
			else if(it->first == MMPresTypes::eFilterType::CONTAINS)
				query = "contains ";

			query.append(it->second);
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineQueryItemList-query=%s",query.c_str());
		}
		else
		{
			/* this for normal browsing */
			query = "containPhonetic ";
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineQueryItemList-query=%s",query.c_str());
		}

        mBrowser->searchObjects(mBrowser.mClientID,currData->mBrowsePath,query,mStartIndex - 1,mNumItems,filter,std::bind(&SessionQueryMetadata::searchQueryDataCallback,this,_1,_2,_3));

#endif //#if 1

		setIdle();

	} //if (currData != nullptr)
}

void SessionQueryMetadata::queryDataCallback(const uint64_t& totalItemCount, const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
  if ( ( stat == CommonAPI::CallStatus::SUCCESS ) && ( err == ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError::NO_ERROR  ) )
    {
        
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback:totalItemCount=%llu,callStatus=%d,Browser error:%d, ResultMap size=%d", totalItemCount, (int)stat, (int)err, (int)map.size());
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback :mStartIndex=%llu,mNumItems=%llu", mStartIndex, mNumItems);
	MMPresTypes::MetadataItemList metadataItemList;
	metadataItemList.clear();
	Session::SessionQueryData* currData = nullptr;
	MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
	Session* sess = getSession();
	DeviceBase* device = getDevice();
	if (device == nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback :device not available");
		if (sess != nullptr)
		{
			sess->removeQueryFromCache(mQueryId);
		}
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: device available");

		if (err == BrowserTypes::BrowserError::NO_ERROR)
		{
			if (sess != nullptr)
			{
				queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;
				uint16_t num = map.size();
				uint64_t start = mStartIndex - 1;
				if (start <= 0)
					start = 0;

				bool levelObtained = false;
				int levelId = -1;

				std::string path = "";
				std::string uri = "";
				std::string name = "";
				std::string type = "";
				std::string typeex = "";
				std::string sysPath = "";
				uint64_t childcount = 0;
				uint64_t id = 0;
            uint64_t itemid = 0;


				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: start=%d", start);
				for (uint64_t i = start, j = 0; j < num; i++, j++)//mStartIndex value is ultimately used while iteration and mStartIndex is of type uint64_t. to avoid data loss as mentioned type of i changed
				{
					const MediaTypes::ResultMap& result = map[j];
					auto it = result.find(BrowseHelper::PATH);
					if (it != result.end())
					{
                        path = it->second.get<std::string>();
						if (!levelObtained)
						{
							levelId = BrowseHelper::getLevelIdFromPath(path);
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: levelId=%d", levelId);
							levelObtained = true;
							if(levelId >= 0)
							sess->setCurrentLevelId((uint32_t)levelId);
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
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback:-itemid id=%llu,", itemid);
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

							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback :path:%s, uri:%s,name:%s,type:%s,typeex:%s,childcount:%llu", path.c_str(), uri.c_str(), name.c_str(), type.c_str(), typeex.c_str(), childcount);

               std::string folder_name = BrowseHelper::getFolderNameFromPath(path);
					id = (std::stoull(folder_name) + 1);
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: item id computed from item path=%llu", id);
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

					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: query result type=%llu,actual item type=%llu", mResultType, metadataItem.getItemType());

					// this is a junk condition which will be hit for audiobooks and podcasts because CoC gives same browse item Type for albums ,audiobooks and podcasts.??
					if (!(metadataItem.getItemType() == mResultType))
					{
						LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: resetting the item type to the expected result type");
						metadataItem.setItemType(mResultType);
					}

					metadataItemList.push_back(metadataItem);

					Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
					if (sessionQueryData != nullptr)
					{
						sessionQueryData->mLevelId = levelId;
						std::pair<std::unordered_map<uint64_t, Session::ItemData>::iterator, bool> res = sessionQueryData->itemList.insert(std::make_pair((i + 1), Session::ItemData(path, uri, childcount, itemid, metadataItem)));
						if (res.second)
						{
							//  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback :updated query data info successfully to cache");
						}
						else
						{
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback :updated query data info to cache failed");
						}
					}
				} // end of for loop
			} // end of sess==nullptr check
		} // end of err=browseerror check
	}// end of device check

	sendQueryData(queryStatus, metadataItemList);

	if(sess != nullptr)
	{
		//get alpha-jump candidates from CoC media
		currData = getSession()->getSessionQueryData(mQueryId);
		if(currData != nullptr)
		{
			DeviceBase* device = getDevice();
			if(device != nullptr)
			{
				/* check added for performance whether to send alpha jump candidates.
				 * For each page scrolling, we should not send it. This should be send only once.
				 * This flag will be reseted after we send alpha jump data to HMI.
				 */
				if(device->getSmartSearchAlphaJumpCandidateRequiredToBeSend() && (mContentMask != MMPresTypes::eTitleContentType::TITLETYPE_VIDEO))
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: Calling determineAlphaJump ");
					determineAlphajumpData();
				}
				else
				{
					if(mContentMask == MMPresTypes::eTitleContentType::TITLETYPE_VIDEO)
					{
						std::string query = "";

						Session::SessionQueryData* currData = nullptr;
						currData = getSession()->getSessionQueryData(mQueryId);

						if(currData != nullptr)
						{
							mBrowser->listThumbnails(mBrowser.mClientID,currData->mBrowsePath,mStartIndex-1,mNumItems,query,std::bind(&SessionQuery::listThumbnailsDataCallback,this,_1,_2,_3));
                     LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: calling listThumbnails as smart search data is not required");
						}
						else
						{
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: Error as SessionQueryData is not found");
							JobCompleted () ;
						}
					} //if(mContentMask == MMPresTypes::eTitleContentType::TITLETYPE_VIDEO)
					else
					{
						LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: audio content and alpha-jump candidates not required to be sent");
						JobCompleted () ;
					}
				}
			}
			else
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback: Error as device is not found");
				JobCompleted () ;
			}

		} //if(currData != nullptr)
		else
		{
			//Error case where SessionQueryData is not found
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: Error as SessionQueryData is not found");
			JobCompleted () ;
		}

	} //if(sess != nullptr)
	else
	{
		//Error case where Session is not found
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: Error as Session is not found");
		JobCompleted () ;
	}
        

    } //  if ( ( stat == CommonAPI::CallStatus::SUCCESS ) && ( err == ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError::NO_ERROR  ) )
    else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::callstatus : %d", stat );
        JobCompleted () ;
    }
}


/* This api is also used to get query count for smart search. But not for actual smart search i.e. searching data containing various characters entered by user.
 * Once user enters into smart search screen through smart search button, HMI calls setSessionQuery() with filterIDMap & filterMap as empty
 * For mpres, this condition is also smart search and hence below api has conditions for smart-search
 */
void SessionQueryMetadata::determineCountViaGetAttribute()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountViaGetAttribute:we have to find the count of category %lu,content type %lu", mResultType, mContentMask);
	std::string browsePath = "";
	
	std::vector<std::string> filter = {BrowseHelper::CHILD_COUNT};

	/* Below stuff is added for smart search.
	 * Till now current QueryId for setSessionQuery() is not added to QueryList.
	 * So using below api, get previous QueryId to get previous queryData
	 */
	Session* tmpSession = nullptr;
	Session::SessionQueryData* prevData = nullptr;
	tmpSession = getSession();

	if(tmpSession != nullptr)
	{
		uint32_t currQueryId = tmpSession->getCurrentQueryId();
		prevData = tmpSession->getSessionQueryData(currQueryId);
	}

	if(prevData != nullptr)
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountViaGetAttribute-previous path= %s",prevData->mBrowsePath.c_str());

	if (mContainerId==0 && !mIsSmartSearch)
	{
		/* This condition is for normal browsing and not smart search */
		if (getDevice() != nullptr)
		{
			browsePath = getDevice()->getBrowsePath();
			browsePath.append("/");
	
			switch (mContentMask)
			{
			case MMPresTypes::eTitleContentType::TITLETYPE_VIDEO:
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountViaGetAttribute-content mask is videos");
				browsePath.append(BrowseHelper::VIDEO_CATEGORY_NAME);
				break;
			case MMPresTypes::eTitleContentType::TITLETYPE_IMAGE:
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountViaGetAttribute-content mask is images");
				browsePath.append(BrowseHelper::IMAGE_CATEGORY_NAME);
				break;
			case MMPresTypes::eTitleContentType::TITLETYPE_AUDIO:
			default:
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountViaGetAttribute-content mask is audios");
				browsePath.append(BrowseHelper::MUSIC_CATEGORY_NAME);
				break;
			}
			browsePath.append("/");
			browsePath.append(BrowseHelper::getContainerName(mResultType));
		}
	}
	else
	{
		if(mIsSmartSearch)
		{
			/* If smart search, always get browse path from previousData as for multilevel smart search, HMI does not provide filterIDMap.
			 * For e.g. Artists---> Albums of particular artist ---> Songs of that albums of that particular artist
			 * When user got results of 1st level of artist by entering some characters, it can be further browsed.
			 * Let us say, artist "Akon" is displayed as result screen for 1st level smart search and this artist has various albums for it.
			 * If user selects smart search here, HMI calls setSessionQuery() with filterIDMap & filterMap as empty.
			 * So to provide correct results of albums of artist of Akon, previous queryID is used to get previousData to get browsePath.
			 */
			if(prevData != nullptr)
					   browsePath = prevData->mBrowsePath;
		}
		else
		  browsePath = getItemPathFromCachedItemList();
	}

	mBrowser->getAttributes(mBrowser.mClientID, browsePath, filter, std::bind(&SessionQuery::getAttributeCallback, this, browsePath, _1, _2, _3));
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountViaGetAttribute:browsePath=%s", browsePath.c_str());
	setIdle();
}

/* Below api is used to get actual query count for SmartSearch query i.e. user presses any available characters on keyboard.
 * For any level of smart search when user presses available character, HMI calls setSessionQuery() with filterIdMap as empty
 * and filterMap as "BEGINS_WITH" with "character/characters" pressed by user.
 */
void SessionQueryMetadata::determineCountForSearchQuery()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountForSearchQuery: to find the count for search %lu", mResultType);
	std::string path = "";
	searchPath = "";
	std::string searchKey = "";
	std::string categoryName = "";

#if 0 //original code is kept for reference for some days as it used to support only 1st level of smart search
	if (getDevice() != nullptr)
	{
		path = getDevice()->getBrowsePath();
		path.append("/");
		switch (mContentMask)
		{
		case MMPresTypes::eTitleContentType::TITLETYPE_VIDEO:
			path.append(BrowseHelper::VIDEO_CATEGORY_NAME);
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountForSearchQuery:path:%s",path.c_str());
			break;
		case MMPresTypes::eTitleContentType::TITLETYPE_IMAGE:
			path.append(BrowseHelper::IMAGE_CATEGORY_NAME);
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountForSearchQuery:path:%s",path.c_str());
			break;
		case MMPresTypes::eTitleContentType::TITLETYPE_AUDIO:
		default:
			path.append(BrowseHelper::MUSIC_CATEGORY_NAME);
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountForSearchQuery:path:%s",path.c_str());
			break;
		}
		categoryName = BrowseHelper::getContainerName(mResultType);

	}
	std::unordered_map<MMPresTypes::eFilterType, std::string>::const_iterator it;
	path.append("/");
	if(!categoryName.empty()){
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountForSearchQuery:path : %s ,categoryName: %s",path.c_str(),categoryName.c_str());
		path.append(categoryName);
		path.append("/");
	}
	
#endif

	/* Below stuff is added for smart search.
	 * Till now current QueryId for setSessionQuery() for actual smart search is not added to QueryList.
	 * So using below api, get previous QueryId to get previous queryData
	 */
	Session::SessionQueryData* prevData = nullptr;
	Session* tmpSession = nullptr;
	tmpSession = getSession();

	if(tmpSession != nullptr)
	{
		uint32_t currQueryId = tmpSession->getCurrentQueryId();
		prevData = tmpSession->getSessionQueryData(currQueryId);
	}

	if(prevData != nullptr)
	{
		/* If smart search, always get browse path from previousData as for multilevel smart search, HMI does not provide filterIDMap.
		 * For e.g. Artists---> Albums of particular artist ---> Songs of that albums of that particular artist
		 * When user got results of 1st level of artist by entering some characters, it can be further browsed.
		 * Let us say, artist "Akon" is displayed as result screen for 1st level smart search and this artist has various albums for it.
		 * If user selects smart search here, HMI calls setSessionQuery() with filterIDMap & filterMap as empty.
		 * So to provide correct results of albums of artist of Akon, previous queryID is used to get previousData to get browsePath.
		 */
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountForSearchQuery-previous path= %s",prevData->mBrowsePath.c_str());
		path = prevData->mBrowsePath;
		//path.append("/");
	}

	if (!mFilters.empty())
	{
		std::unordered_map<MMPresTypes::eFilterType, std::string>::const_iterator it;
		it = mFilters.begin();
		if (it->first == MMPresTypes::eFilterType::BEGINS_WITH)
		{
			/* For smart search, getting count for search objects, searchKey should be "containPhonetic" and not "startswith". */
			//searchKey = "startswith ";
			searchKey = "containPhonetic ";
		}
		else if (it->first == MMPresTypes::eFilterType::CONTAINS)
			searchKey = "contains ";

		searchKey.append(it->second);
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::determineCountForSearchQuery:Search keyword,path: %s,%s", searchKey.c_str(), path.c_str());
		searchPath = path;

		/* Below api from CoC media is used to get smartSearch specific queryCount and sent to HMI along with next level available characters */
		mBrowser->getSearchObjectsCount(mBrowser.mClientID, path, searchKey, std::bind(&SessionQueryMetadata::searchQueryCountcallback, this, _1, _2, _3));
	}
}

void SessionQueryMetadata::searchQueryCountcallback(const CommonAPI::CallStatus& status, const uint64_t& count, const ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError& error)
{
    if ( ( status == CommonAPI::CallStatus::SUCCESS ) && ( error == ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError::NO_ERROR  ) )
    {
        
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryCountcallback: status=%d,count=%llu,error=%d", (int)status, count, (int)error);
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	DeviceBase* device = getDevice();
	Session* sess = getSession();
	if (device != nullptr)
	{
		Session::SessionQueryData* queryData = nullptr;
		if (sess != nullptr)
		{
			queryData = sess->getSessionQueryData(mQueryId);

			if (queryData != nullptr)
			{
				queryData->mNumResults = count;
				queryData->mContainerId = mContainerId;
				queryData->mBrowsePath = searchPath;
			} // session query data existence check
			else
			{
				result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
			}
		} // session existence check
		else
		{
			result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
		}// device existence check
	}
	else
	{
		result = MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED;
	}
	if (count > 0)
	{
		/* For smart search and multilevel smart search, do not insert current/newly created queryID into QueryList.
		 * For actual smart search, "mFilter" will not be empty.
		 * Below are some special cases.
		 * Case 1: For multilevel smart search, result screen of artist selected through smart search shows albums for that artist.
		 * For this, setSessionQuery() already done with filterIDMap as not empty and resultType as albums.
		 * Once smart search is initiated through button from this, HMI calls setSessionQuery() with filterIDMap as empty and resultType as album.
		 * So for mediapres, smart search started and hence do not add that query to queryList.
		 *
		 * Case 2: User already into smart search screen from albums of artist, i.e. 2nd level smart search.
		 * Without pressing any character, user presses back from screen and expectation is to go back to albums of that artist screen
		 * HMI calls setSessionQuery() with empty FilterIDMap, filterMap but resultType is albums.
		 * Hence, used previous data again to get correct results and do not insert mQueryID into queryList.
		 *
		 * Case 3: For multilevel smart search, once results of smart search is available in screen, option of "show all" is available to user.
		 * If user presses this, HMI calls setSessionQuery() with filterIDMap empty and we treat this also as smart search only.
		 * But expectations is to go back to screen where smart search started in multilevel.
		 * For e.g. Artist [Level 1] --> smart search done --> Artist result screen --> artist selected
		 *      -   -> album screen [Level 2] --> smart search done --> result screen for albums --> press Show All
		 *                                   --> go to album screen from where smart search started
		 */
      if (sess != nullptr)
      {
         if (mFilters.empty())
            sess->insertToQueryList(mQueryId);

         sess->setCurrentQueryId(mQueryId);
      }
	}
	else
	{
      if (sess != nullptr)
      {
         sess->removeQueryFromCache(mQueryId);
      }
	}

	/* below code is required for smart search for all levels to get correct breadcrumb path and to send to HMI.
	 * Breadcrumbpath during smart search will be sent through reply of setSessionQuery() in sessionQuery.cpp through sendQueryCount() api.
	 */
	determineBreadCrumbPath();

	sendQueryCount(result, count);
	 JobCompleted () ;
    }
       
    else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::CommonAPI:: callstatus =%d", stat);
        JobCompleted();
    }
}

void SessionQueryMetadata::searchQueryDataCallback(const CommonAPI::CallStatus& status, const ::v1::org::genivi::mediamanager::MediaTypes::ResultMapList& map, const ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError& error)
{
    if ( ( status == CommonAPI::CallStatus::SUCCESS ) && ( error == ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError::NO_ERROR  ) )
    {
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: status=%d,error=%d", (int)status, (int)error);
			MMPresTypes::MetadataItemList metadataItemList;
			metadataItemList.clear();
			MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
			Session::SessionQueryData* currData = nullptr;
			std::vector<std::string> filter = { BrowseHelper::SMART_SEARCH_CANDIDATES };
			Session* sess = getSession();
			DeviceBase* device = getDevice();
			if (device == nullptr)
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback :device not available");
				if (sess != nullptr)
				{
					sess->removeQueryFromCache(mQueryId);
				}
			}
			else
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback :device available");

				if (error == BrowserTypes::BrowserError::NO_ERROR)
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
                  uint64_t itemid = 0;

						uint64_t id = 0;

						LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: start=%llu", start);
						for (uint16_t i = start, j = 0; j < num; i++, j++)
						{
							const MediaTypes::ResultMap& result = map[j];
							auto it = result.find(BrowseHelper::PATH);
							if (it != result.end())
							{
								path = it->second.get<std::string>();
								if (!levelObtained)
								{
									levelId =  BrowseHelper::getLevelIdFromPath(path);
									LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: levelId=%d", levelId);
									levelObtained = true;

									if(levelId < 0)
									{
										LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: Invalid Level Id %d", levelId);
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
                        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback:-itemid id=%llu,", itemid);
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

							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback :path:%s, uri:%s,name:%s,type:%s,typeex:%s,childcount:%llu", path.c_str(), uri.c_str(), name.c_str(), type.c_str(), typeex.c_str(), childcount);

                            std::string folder_name = BrowseHelper::getFolderNameFromPath(path);
							id = (std::stoull(folder_name) + 1);
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: item id computed from item path=%llu", id);
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

					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: query result type=%llu,actual item type=%llu", mResultType, metadataItem.getItemType());

					// this is a junk condition which will be hit for audiobooks and podcasts because CoC gives same browse item Type for albums ,audiobooks and podcasts.??
					if (!(metadataItem.getItemType() == mResultType))
					{
						LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: resetting the item type to the expected result type");
						metadataItem.setItemType(mResultType);
					}

					metadataItemList.push_back(metadataItem);

					Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
					if (sessionQueryData != nullptr)
					{
						sessionQueryData->mLevelId = levelId;
						std::pair<std::unordered_map<uint64_t, Session::ItemData>::iterator, bool> res = sessionQueryData->itemList.insert(std::make_pair((i + 1), Session::ItemData(path, uri, childcount, itemid, metadataItem)));
						if (res.second)
						{
							//  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::queryDataCallback :updated query data info successfully to cache");
						}
						else
						{
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback :updated query data info to cache failed");
						}
					}
				} // end of for loop
			} // end of sess==nullptr check
		} // end of err=browseerror check
	}// end of device check

	sendQueryData(queryStatus, metadataItemList);

	if(sess != nullptr)
	{
		//get smart-search specific candidates from CoC media
		currData = getSession()->getSessionQueryData(mQueryId);
		if(currData != nullptr)
		{
			DeviceBase* device = getDevice();
			if(device != nullptr)
			{
				/* check added for performance whether to send smart search and alpha jump candidates.
				 * For each page scrolling, we should not send it.
				 * This flag will be reseted after we send smart search and alpha jump data to HMI.
				 */
				if(device->getSmartSearchAlphaJumpCandidateRequiredToBeSend() && (mContentMask != MMPresTypes::eTitleContentType::TITLETYPE_VIDEO))
				{
					mBrowser->getAttributes(mBrowser.mClientID, currData->mBrowsePath, filter, std::bind(&SessionQuery::getAttributesCallbackForSmartSearch, this, _1, _2, _3));
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: Calling getAttributes for Smart Search with mBrowsePath =%s ", currData->mBrowsePath.c_str());
				}
				else
				{
					if(mContentMask == MMPresTypes::eTitleContentType::TITLETYPE_VIDEO)
					{
						std::string query = "";

						Session::SessionQueryData* currData = nullptr;
						currData = getSession()->getSessionQueryData(mQueryId);

						if(currData != nullptr)
						{
							mBrowser->listThumbnails(mBrowser.mClientID,currData->mBrowsePath,mStartIndex-1,mNumItems,query,std::bind(&SessionQuery::listThumbnailsDataCallback,this,_1,_2,_3));
                     LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: calling listThumbnails as smart search data is not required");
						}
						else
						{
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: Error as SessionQueryData is not found");
							JobCompleted () ;
						}
					} //if(mContentMask == MMPresTypes::eTitleContentType::TITLETYPE_VIDEO)
					else
					{
						LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: audio content and smart search data is not required to be sent");
						 JobCompleted () ;
					}
				}
			}
			else
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: Error as device is not found");
				 JobCompleted () ;
			}

		} //if(currData != nullptr)
		else
		{
			//Error case where SessionQueryData is not found
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: Error as SessionQueryData is not found");
			 JobCompleted () ;
		}

	} //if(sess != nullptr)
	else
	{
		//Error case where Session is not found
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryMetadata::searchQueryDataCallback: Error as Session is not found");
        JobCompleted () ;
	}
    }
    else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::CommonAPI:: callstatus =%d", stat);
        JobCompleted();
    }
    //setCompleted();
}





