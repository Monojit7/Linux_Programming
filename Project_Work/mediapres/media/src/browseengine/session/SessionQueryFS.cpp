#include <browseengine/session/SessionQueryFS.h>
#include <device/DeviceBase.h>

SessionQueryFS::SessionQueryFS(const uint32_t& sessionID,const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex,const uint64_t& numItems)
	:SessionQuery(sessionID,_client,std::move(reply),std::move(gotoPreviousReply),std::move(jumpToQueryReply),mediumID,filterIDs,filters,resultType,metadataAttributes,metadataCountTypes,type,resetPrevious,AutoUpdateEnabled,ContentMask,startIndex,numItems)
{
	Session* sess = mManager->getSession(mSessionId);
	if (sess != nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::SessionQueryFS:initialize queryId=%lu in cache", mQueryId);
		Session::SessionQueryData* data = new Session::SessionQueryData(mQueryId, mMediumId, mFilterIDMap, mFilters, mResultType, mMetadataAttributes, mMetadataCountTypes, mQueryType, mAutoUpdateEnabled, mResetPrevious, mContentMask);
		sess->insertNewQueryToCache(mQueryId, data);
	}
}

SessionQueryFS::SessionQueryFS(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
	:SessionQuery(sessionID,queryID,_client,std::move(reply),std::move(gotoPreviousReply),std::move(jumpToQueryReply),queryCacheData,startIndex,numItems)
{

}

SessionQueryFS::~SessionQueryFS()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::~SessionQueryFS");
}

void SessionQueryFS::onRun()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::onRun-queryId=%lu,mMediumId=%llu", mQueryId,mMediumId);

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

bool SessionQueryFS::validateQueryInput()
{
	bool paramsValid = true;
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::validateQueryInput-session and device are available");
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

bool SessionQueryFS::validateResultType()
{
	return BrowseHelper::checkIfFileSystemBrowseItemSupported(mResultType);
}

bool SessionQueryFS::validateFilterInput()
{
	bool ret = true;
	bool inputMatched = false;

	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::validateFilterInput-filterIDMap size=%d", (int)(mFilterIDMap.size()));
	std::unordered_map<MMPresTypes::eMetadataType, uint64_t>::const_iterator itr;

	if (!mFilterIDMap.empty())
	{
			itr = mFilterIDMap.begin();
			while (itr != mFilterIDMap.end())
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::validateFilterInput-filterIDMap key=%d,value=%d", (int)(itr->first), (int)(itr->second));
                if ((itr->first)==(MMPresTypes::eMetadataType::METADATATYPE_FOLDER))
				{
					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::validateFilterInput-filterID map contains a valid key");
					mContainerId = itr->second;
					inputMatched = true;
					break;
				}
				itr++;
			}
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::validateFilterInput-mContainerId=%llu", mContainerId);
			if (!inputMatched)
			{
				ret = false;
			}
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::validateFilterInput-mFilterIDMap is empty...so equivalent to obtaining root level data");
	}
	return ret;
}

void SessionQueryFS::determineCount()
{
	uint64_t totCount = 0;
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	FacadeHelper<MediaPresSettings> mSettings;
	DeviceBase* pDevice = mDetect->getDevice(mMediumId);
	DeviceBase* devicePointer = getDevice();
	if (mGotoPreviousCallback != nullptr || mJumpToQueryCallback != nullptr)
	{
		/* Empty A-Z list should be sent to HMI only once for explore device window even though user scroll down */
		if(devicePointer != nullptr)
		{
			devicePointer->setSmartSearchAlphaJumpCandidateRequiredToBeSend(true);
		}
		processAndSendCachedCount();
	}
	else
	{
		if (mSettings->getAttributeUsageForCount())
		{
			/* Empty A-Z list should be sent to HMI only once for explore device window even though user scroll down */
			if(devicePointer != nullptr)
			{
				devicePointer->setSmartSearchAlphaJumpCandidateRequiredToBeSend(true);
			}
			determineCountViaGetAttribute();
		}
		else
		{
			if (mContainerId==0)
			{
				getRootFolderCount();
			}
			else
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::determineCount-get cached count for sessionQuery");
				totCount = getQueryCountFromCachedItemList();
				Session* getSessionptr = getSession();
				if(getSessionptr != nullptr)
				{
					if (totCount > 0)
					{
						getSessionptr->insertToQueryList(mQueryId);
						getSessionptr->setCurrentQueryId(mQueryId);
					}
					else if((totCount == 0)&&(pDevice != nullptr))
					{
					  if(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH == pDevice->getDevType())
                  {
                     getSessionptr->insertToQueryList(mQueryId);
                     getSessionptr->setCurrentQueryId(mQueryId);
                  }
					}
					else
					{
						getSessionptr->removeQueryFromCache(mQueryId);
					}
				}
				else
				{
               LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::determineCount ERROR: Session Pointer is NULL");
				}
				determineBreadCrumbPath();
				sendQueryCount(result, totCount);
				setCompleted();
			}
		}
	}
}

void SessionQueryFS::getRootFolderCount()
{
	std::string path = "";
	if (getDevice() != nullptr)
	{
		path = getDevice()->getBrowsePath();
		path.append("/");
		switch (mContentMask)
		{
		case MMPresTypes::eTitleContentType::TITLETYPE_VIDEO:
			path.append(BrowseHelper::VIDEO_CATEGORY_NAME);
			break;
		case MMPresTypes::eTitleContentType::TITLETYPE_IMAGE:
			path.append(BrowseHelper::IMAGE_CATEGORY_NAME);
			break;
		case MMPresTypes::eTitleContentType::TITLETYPE_AUDIO: 
		default:
			path.append(BrowseHelper::MUSIC_CATEGORY_NAME);
			break;
		}
	}
	
	std::vector<std::string> filter = { "*" };
	uint64_t folderId = 0;
	mBrowser->listChildren(mBrowser.mClientID, path, mStartIndex, mNumItems, filter, std::bind(&SessionQueryFS::queryCountCallback, this, path, folderId, _1, _2, _3));
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::getRootFolderCount-path:%s", path.c_str());
	setIdle();
}

void SessionQueryFS::determineCountViaGetAttribute()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::determineCountViaGetAttribute:we have to find the count of category %lu", mResultType);
	std::string browsePath = "";

	std::vector<std::string> filter = { BrowseHelper::CHILD_COUNT };
	if (mContainerId==0)
	{
		if (getDevice() != nullptr)
		{
			browsePath = getDevice()->getBrowsePath();
			browsePath.append("/");
			switch (mContentMask)
			{
			case MMPresTypes::eTitleContentType::TITLETYPE_VIDEO:
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::determineCountViaGetAttribute-folder browse for videos");
				browsePath.append(BrowseHelper::VIDEO_CATEGORY_NAME);
				break;
			case MMPresTypes::eTitleContentType::TITLETYPE_IMAGE:
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::determineCountViaGetAttribute-folder browse for images");
				browsePath.append(BrowseHelper::IMAGE_CATEGORY_NAME);
				break;
			case MMPresTypes::eTitleContentType::TITLETYPE_AUDIO:
			default:
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::determineCountViaGetAttribute-folder browse for audios");
				browsePath.append(BrowseHelper::MUSIC_CATEGORY_NAME);
				break;
			}
			browsePath.append("/");
			browsePath.append(BrowseHelper::FOLDER_CATEGORY_NAME);
		}
	}
	else
	{
		browsePath = getItemPathFromCachedItemList();
	}
	mBrowser->getAttributes(mBrowser.mClientID, browsePath, filter, std::bind(&SessionQuery::getAttributeCallback, this, browsePath, _1, _2, _3));
	setIdle();
}

void SessionQueryFS::queryCountCallback(const std::string&parentPath, const uint64_t&folderId, const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
	std::string path = "";
	std::string folderName = "";
	uint64_t childCount = 0;
	std::string uri = "";
	std::string type = "", typeex = "";
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;

	Session* sess = mManager->getSession(mSessionId);

	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryCountCallback :CallStatus : %d BrowserError %d count:%d", (int)stat, (int)err, map.size());

	DeviceBase* device = mDetect->getDevice(mMediumId);
	if (device == nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryCountCallback :device not available");
		result = MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED;
		if (sess != nullptr)
		{
			sess->removeQueryFromCache(mQueryId);
		}
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryCountCallback :device available");
		uint16_t count = map.size();
		if (err == BrowserTypes::BrowserError::NO_ERROR)
		{
			if (folderId == 0) // if root folder
			{
				for (uint16_t i = 0; i < count; i++)
				{
					const MediaTypes::ResultMap& result = map[i];

					auto it = result.find(BrowseHelper::TYPE);
					if (it != result.end())
						type = it->second.get<std::string>();

					it = result.find(BrowseHelper::TypeEx);
					if (it != result.end())
						typeex = it->second.get<std::string>();

					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryCountCallback - type:%s,typeex:%s", type.c_str(), typeex.c_str());

					it = result.find(BrowseHelper::PATH); // get the path
					if (it != result.end())
					{
						path = it->second.get<std::string>();
						LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryCountCallback :path : %s", path.c_str());

						folderName = BrowseHelper::getFolderNameFromPath(path);

						if (!folderName.compare(BrowseHelper::FOLDER_CATEGORY_NAME)) // check for "folder"
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
								LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryCountCallback :childcount : %llu", childCount);
								//	sendQueryCount(MMPresTypes::eCallResult::CALLRESULT_OK, childCount);
								it = result.find(BrowseHelper::URI);
								if (it != result.end())
								{
									uri = it->second.get<std::string>();
									LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryCountCallback :uri : %s", uri.c_str());
								}
							}
							break;
						} // folder name check
					}// itr check
				} // for loop
			}
			else // if not root folder
			{
				// this will be implemented if we don't follow the caching strategy for determining the count of items in subfolders.
				// the implementation will be similar to what we have done for the root folder.
			}

			if (sess != nullptr)
			{
				Session::SessionQueryData* sessionQueryData = sess->getSessionQueryData(mQueryId);
				if (sessionQueryData != nullptr)
				{
					if(sessionQueryData->mPlayURI.capacity() >= uri.capacity())
					sessionQueryData->mPlayURI = uri;

					if(sessionQueryData->mBrowsePath.capacity() >= path.capacity())
					sessionQueryData->mBrowsePath = path;

					sessionQueryData->mNumResults = childCount;
					sessionQueryData->mContainerId = mContainerId;
					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryCountCallback :updated query parameter info to cache");
				}
			}
		}
		else // if there is browse error
		{
			result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
		}
	} // null device check

	if (childCount > 0)
	{
		if(sess != nullptr)
		{
			sess->insertToQueryList(mQueryId);
			sess->setCurrentQueryId(mQueryId);
		}

	}
	else if((childCount == 0)&&(device != nullptr))
	{
		if(MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH == device->getDevType())
		{
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
	determineBreadCrumbPath();
	sendQueryCount(result, childCount);
	setCompleted();
}

void SessionQueryFS::determineQueryItemList()
{
	std::vector<std::string> filter = { "*" };
	Session::SessionQueryData* currData = nullptr;
	currData = getSession()->getSessionQueryData(mQueryId);
	if (currData != nullptr)
	{
		mBrowser->listChildren(mBrowser.mClientID, currData->mBrowsePath, mStartIndex - 1, mNumItems, filter, std::bind(&SessionQueryFS::queryDataCallback, this, currData->mNumResults, _1, _2, _3));
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::determineQueryItemList-mContainerId=%llu,browsePath=%s,mStartIndex=%llu,mNumItems=%llu", mContainerId, currData->mBrowsePath.c_str(), mStartIndex, mNumItems);
		setIdle();
	}
}

void SessionQueryFS::queryDataCallback(const uint64_t& totalItemCount, const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback :totalItemCount:%llu,CallStatus : %d,BrowserError %d,resultmap size:%llu mStartIndex=%llu,mNumItems=%llu", totalItemCount, (int)stat, (int)err, (uint64_t)map.size(), mStartIndex, mNumItems);
	MMPresTypes::MetadataItemList metadataItemList;
	metadataItemList.clear();
	MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
	Session* sess = mManager->getSession(mSessionId);
	DeviceBase* device = mDetect->getDevice(mMediumId);
	if (device == nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback :device not available");
		if (sess != nullptr)
		{
			sess->removeQueryFromCache(mQueryId);
		}
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback :device available");

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
            uint64_t itemid = 0;

				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback: start=%llu", start);
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
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback: levelId=%d", levelId);
							levelObtained = true;
							sess->setCurrentLevelId(levelId);
						}
					}

					it = result.find(BrowseHelper::URI);
					if (it != result.end())
					{
						uri = it->second.get<std::string>();
					}

               it = result.find(BrowseHelper::ITEMID);
               if (it != result.end())
               {
                  itemid = (uint64_t)(it->second.get<std::uint64_t>());
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback:-itemid id=%llu,", itemid);
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
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback: childcount not updated:childcount:%llu",childcount);
						}
						/*
						if (childcount < 0)
						{
							childcount = 0;
						}
						*/
					}

					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback :path:%s, uri:%s,name:%s,type:%s,typeex:%s,childcount:%llu", path.c_str(), uri.c_str(), name.c_str(), type.c_str(), typeex.c_str(), childcount);
               std::string folder_name = BrowseHelper::getFolderNameFromPath(path);
					id = (std::stoull(folder_name) + 1);
					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback: item id computed from item path=%llu", id);
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
                  playable = true;
                  browsable = false;
						fileSystemContentType = MMPresTypes::eFileSystemContentType::FS_CONTENT_TYPE_PLAYLIST;
						titleType = MMPresTypes::eTitleContentType::TITLETYPE_UNKNOWN;
						itemType = MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST;
					}
					else
					{
                  //Do Nothing
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
							LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback :updated query data info to cache failed");
						}
					}
				} // end of for loop
			} // end of sess==nullptr check
        } // end of err=browseerror chelistChildrenck
	}// end of device check

	sendQueryData(queryStatus, metadataItemList);
	
	/* alphajump is not applicable to FS browsing and not Elevator bar helper letter.
	 * So removing alpha jump code and retaining old video thumbnail code below
	 */
#if 0
	if((!metadataItemList.empty()) && (metadataItemList.size() > 0))
	{
		determineAlphajumpData();
	}
	else
	{
		setCompleted();
	}
#endif
	Session* pSession = getSession();
	if(pSession != nullptr)
	{
		std::string query ="";
		Session::SessionQueryData* currData = nullptr;
		currData = pSession->getSessionQueryData(mQueryId);
		if(currData != nullptr)
		{
			if((mContentMask == MMPresTypes::eTitleContentType::TITLETYPE_VIDEO) || (mContentMask == MMPresTypes::eTitleContentType::TITLETYPE_IMAGE))
			{
				mBrowser->listThumbnails(mBrowser.mClientID,currData->mBrowsePath,mStartIndex-1,mNumItems,query,std::bind(&SessionQuery::listThumbnailsDataCallback,this,_1,_2,_3));
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback : Calling listThumbnails");
			}
			else
			{
				DeviceBase* pDevice = getDevice();
				/* For audio explore device window, send empty A-Z list to HMI so HMI will not show A-Z/Smart search buttons */
				if(pDevice != nullptr)
				{
					if(pDevice->getSmartSearchAlphaJumpCandidateRequiredToBeSend())
					{
						MMPresTypes::arAlphaIndex aJListCopy;

						LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback - sending empty A-Z list to HMI");
						sendQueryDataforAlphajump(queryStatus, aJListCopy);

						pDevice->setSmartSearchAlphaJumpCandidateRequiredToBeSend(false);
					}
				}
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback : JobCompleted for Audio");
				JobCompleted();
			}
		}
		else
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback : Error: currData is NULL");
			JobCompleted();
		}

	} //if(getSession() != nullptr)
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS::queryDataCallback :Error: Session is NULL");
		JobCompleted();
	}
}
