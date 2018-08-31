#include <browseengine/session/SessionQueryHighestRated.h>
#include <device/DeviceBase.h>

SessionQueryHighestRated::SessionQueryHighestRated(const uint32_t& sessionID,const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex,const uint64_t& numItems)
	:SessionQuery(sessionID,_client,std::move(reply),std::move(gotoPreviousReply),std::move(jumpToQueryReply),mediumID,filterIDs,filters,resultType,metadataAttributes,metadataCountTypes,type,resetPrevious,AutoUpdateEnabled,ContentMask,startIndex,numItems)
{
	Session* sess = mManager->getSession(mSessionId);
	if (sess != nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::SessionQueryHighestRated:initialize queryId=%lu in cache", mQueryId);
		Session::SessionQueryData* data = new Session::SessionQueryData(mQueryId, mMediumId, mFilterIDMap, mFilters, mResultType, mMetadataAttributes, mMetadataCountTypes, mQueryType, mAutoUpdateEnabled, mResetPrevious, mContentMask);
		sess->insertNewQueryToCache(mQueryId, data);
	}
}

SessionQueryHighestRated::SessionQueryHighestRated(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
	:SessionQuery(sessionID,queryID,_client,std::move(reply),std::move(gotoPreviousReply),std::move(jumpToQueryReply),queryCacheData,startIndex,numItems)
{

}

SessionQueryHighestRated::~SessionQueryHighestRated()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::~SessionQueryHighestRated");
}

void SessionQueryHighestRated::onRun()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::onRun-queryId=%lu,mMediumId=%llu", mQueryId,mMediumId);

	MMPresTypes::eCallResult callResult;
	MMPresTypes::eQueryStatus queryStatus;

	if (validateQueryInput())
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
		setCompleted();
	}
}

bool SessionQueryHighestRated::validateQueryInput()
{
	bool paramsValid = true;

	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::validateQueryInput-session and device are available");

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

bool SessionQueryHighestRated::validateResultType()
{
	return true;
}

bool SessionQueryHighestRated::validateFilterInput()
{
	bool ret = true;
	return ret;
}

void SessionQueryHighestRated::determineCount()
{
	Session* sess = getSession();
	DeviceBase* device = getDevice();
	std::vector<uint64_t> deviceList = { mMediumId };
	if (sess != nullptr)
	{
		if (device != nullptr)
		{
		 mBrowser->getMostPlayedCount(mBrowser.mClientID,deviceList,std::bind(&SessionQueryHighestRated::queryCountCallback, this, _1, _2, _3));
		 setIdle();
		}
	}

}

void SessionQueryHighestRated::queryCountCallback(const CommonAPI::CallStatus & stat, const uint64_t& count, const ::v1::com::harman::media::BaseType::tBaseError& err)
{
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	Session* sess = mManager->getSession(mSessionId);

	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryCountCallback :CallStatus : %d BaseError %d count:%d", (int)stat, (int)err, count);

	DeviceBase* device = mDetect->getDevice(mMediumId);
	if (device == nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryCountCallback :device not available");
		result = MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED;
		if (sess != nullptr)
		{
			sess->removeQueryFromCache(mQueryId);
		}
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryCountCallback :device available");
		if (err == BrowserTypes::BrowserError::NO_ERROR)
		{
			if (sess != nullptr)
			{
				Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
				if (sessionQueryData != nullptr)
				{
					sessionQueryData->mNumResults = count;
					sessionQueryData->mContainerId = mContainerId;
					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryCountCallback :updated query parameter info to cache");
				}
			}
		}
		else // if there is browse error
		{
			result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
		}
	} // null device check

	if (count > 0)
	{
		if(sess != nullptr)
			sess->setCurrentQueryId(mQueryId);
	}
	else
	{
      if (sess != nullptr)
      {
         sess->removeQueryFromCache(mQueryId);
      }
	}
	sendQueryCount(result, count);
	setCompleted();
}

void SessionQueryHighestRated::determineQueryItemList()
{    
	const std::vector<uint64_t> deviceList = {mMediumId};
	mBrowser->getMostPlayedList(mBrowser.mClientID,deviceList,(mStartIndex-1), mNumItems, std::bind(&SessionQueryHighestRated::queryDataCallback, this, _1, _2, _3));
	setIdle();
}

void SessionQueryHighestRated::queryDataCallback(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const ::v1::com::harman::media::BaseType::tBaseError& err)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryDataCallback :CallStatus:%d,BaseError %d,resultmap size:%llu",(int)stat, (int)err, (uint64_t)map.size());
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryDataCallback :mStartIndex=%llu,mNumItems=%llu", mStartIndex, mNumItems);
	
	MMPresTypes::MetadataItemList metadataItemList;
	metadataItemList.clear();
	MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
	Session* sess = mManager->getSession(mSessionId);
	DeviceBase* device = mDetect->getDevice(mMediumId);
   uint64_t itemid = 0;

	if (device == nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryDataCallback :device not available");
		if (sess != nullptr)
		{
			sess->removeQueryFromCache(mQueryId);
		}
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryDataCallback :device available");

		if (err == BrowserTypes::BrowserError::NO_ERROR)
		{
			if (sess != nullptr)
			{
				queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;
				uint64_t num = map.size();
				uint64_t start = mStartIndex - 1;
				if (start <= 0)
					start = 0;

				bool levelObtained = false;
				int32_t levelId = -1;

				std::string path = "";
				std::string uri = "";
				std::string name = "";
				std::string type = "";
				std::string typeex = "";
				uint64_t childcount = 0;
				std::string sysPath = "";
				uint64_t id = 0;

				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryDataCallback: start=%llu", start);
				for (uint16_t i = start, j = 0; j < num; i++, j++)
				{
					const MediaTypes::ResultMap& result = map[j];
					auto it = result.find(BrowseHelper::PATH);
					if (it != result.end())
					{
						path = it->second.get<std::string>();
						if (!levelObtained)
						{
							levelId = BrowseHelper::getLevelIdFromPath(path);
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryDataCallback: levelId=%d", levelId);
							levelObtained = true;
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

               it = result.find(BrowseHelper::ITEMID);
               if (it != result.end())
               {
                  itemid = (uint64_t)(it->second.get<std::uint64_t>());
               }

					it = result.find(BrowseHelper::NAME);
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
						/*
						if (childcount < 0)
						{
							childcount = 0;
						}
						*/
					}

					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryDataCallback :path:%s, uri:%s,name:%s,type:%s,typeex:%s,childcount:%llu", path.c_str(), uri.c_str(), name.c_str(), type.c_str(), typeex.c_str(), childcount);

				    id = (i + 1);
					bool playable = false, browsable = false;
					MMPresTypes::eFileSystemContentType fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_UNKNOWN;
					MMPresTypes::eTitleContentType titleType = MMPresTypes::eTitleContentType::TITLETYPE_UNKNOWN;
					MMPresTypes::MetadataCountMap metadataCountMap;
					MMPresTypes::MetadataStringMap metadataStringMap;
					MMPresTypes::eMetadataType itemType = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;

					if (!typeex.compare(BrowseHelper::FOLDER_CONTAINER_TYPE))
					{
						playable = false;
						browsable = true;
						fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_FOLDER;
						titleType = MMPresTypes::eTitleContentType::TITLETYPE_UNKNOWN;
						itemType = MMPresTypes::eMetadataType::METADATATYPE_FOLDER;
					}
					else if (!typeex.compare(BrowseHelper::AUDIO_FILE_ITEM_TYPE) || !type.compare(BrowseHelper::AUDIO_FILE_ITEM_TYPE))
					{
						playable = true;
						browsable = false;
						fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_FILE;
						titleType = MMPresTypes::eTitleContentType::TITLETYPE_AUDIO;
						itemType = MMPresTypes::eMetadataType::METADATATYPE_FILE;
					}
					else if (!typeex.compare(BrowseHelper::VIDEO_FILE_ITEM_TYPE) || !type.compare(BrowseHelper::VIDEO_FILE_ITEM_TYPE))
					{
						playable = true;
						browsable = false;
						fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_FILE;
						titleType = MMPresTypes::eTitleContentType::TITLETYPE_VIDEO;
						itemType = MMPresTypes::eMetadataType::METADATATYPE_FILE;
					}
					else if (!typeex.compare(BrowseHelper::IMAGE_FILE_ITEM_TYPE) || !type.compare(BrowseHelper::IMAGE_FILE_ITEM_TYPE))
					{
						playable = false;
						browsable = false;
						fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_FILE;
						titleType = MMPresTypes::eTitleContentType::TITLETYPE_IMAGE;
						itemType = MMPresTypes::eMetadataType::METADATATYPE_FILE;
						metadataStringMap[MMPresTypes::eMetadataType::METADATATYPE_IMAGE] = sysPath;
					}
					else if (!typeex.compare(BrowseHelper::PLAYLIST_CONTAINER_TYPE))
					{
						playable = false;
						browsable = true;
						fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_PLAYLIST;
						titleType = MMPresTypes::eTitleContentType::TITLETYPE_UNKNOWN;
						itemType = MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST;
					}
					else
					{

					}
					{

					}

					MMPresTypes::tMetadataItem metadataItem;
					metadataItem.setID(id);
					metadataItem.setValue(name);
					metadataItem.setPlayable(playable);
					metadataItem.setBrowsable(browsable);
					metadataItem.setFolderType(fileSystemContentType);
					metadataItem.setContentType(titleType);
					metadataItem.setItemType(itemType);
					metadataItem.setMetadataMap(metadataStringMap);
					metadataItemList.push_back(metadataItem);

					Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
					if (sessionQueryData != nullptr)
					{
						sessionQueryData->mLevelId = levelId;
						std::pair<std::unordered_map<uint64_t, Session::ItemData>::iterator, bool> res = sessionQueryData->itemList.insert(std::make_pair((i + 1), Session::ItemData(path, uri, (uint64_t)childcount, itemid, metadataItem)));
						if (res.second)
						{
							//	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryCallback :updated query data info successfully to cache");
						}
						else
						{
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryHighestRated::queryDataCallback :updated query data info to cache failed");
						}
					}
				} // end of for loop
			} // end of sess==nullptr check
		} // end of err=browseerror check
	}// end of device check

	sendQueryData(queryStatus, metadataItemList);
	setCompleted();
}
