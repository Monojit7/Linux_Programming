#include <browseengine/session/SessionQueryCategory.h>
#include <device/DeviceBase.h>

SessionQueryCategory::SessionQueryCategory(const uint32_t& sessionID,const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex,const uint64_t& numItems)
	:SessionQuery(sessionID,_client,std::move(reply),std::move(gotoPreviousReply),std::move(jumpToQueryReply),mediumID,filterIDs,filters,resultType,metadataAttributes,metadataCountTypes,type,resetPrevious,AutoUpdateEnabled,ContentMask,startIndex,numItems)
{
	Session* sess = mManager->getSession(mSessionId);
	if (sess != nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::SessionQueryCategory:initialize queryId=%lu in cache",mQueryId);
		Session::SessionQueryData* data = new Session::SessionQueryData(mQueryId, mMediumId, mFilterIDMap, mFilters, mResultType, mMetadataAttributes, mMetadataCountTypes, mQueryType, mAutoUpdateEnabled, mResetPrevious, mContentMask);
		sess->insertNewQueryToCache(mQueryId,data);
	}
	mCategoriesObtained = false;
	mCategoryIndex = 1;
}

SessionQueryCategory::SessionQueryCategory(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
	:SessionQuery(sessionID,queryID,_client,std::move(reply),std::move(gotoPreviousReply),std::move(jumpToQueryReply),queryCacheData,startIndex,numItems)
{

}

SessionQueryCategory::~SessionQueryCategory()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::~SessionQueryCategory");
}

void SessionQueryCategory::onRun()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::onRun-queryId=%lu", mQueryId);

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
		JobCompleted();
	}
}

bool SessionQueryCategory::validateQueryInput()
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
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::validateQueryInput-paramsValid %d", paramsValid);

   return paramsValid;
}

bool SessionQueryCategory::validateResultType()
{
	mResultType = MMPresTypes::eMetadataType::METADATATYPE_CATEGORY;
	return true;

#if 0
	bool ret = false;
	if (mResultType = MMPresTypes::eMetadataType::METADATATYPE_CATEGORY)
	{
		ret = true;
	}
	return ret;
#endif

}

bool SessionQueryCategory::validateFilterInput()
{
	bool ret = true;
	return ret;
}

void SessionQueryCategory::determineCount()
{
	FacadeHelper<MediaPresSettings> mSettings;

	if (mSettings->getAttributeUsageForCount())
	{
		if (!mCategoriesObtained)
		{
			determineCategories();
		}
		else
		{
			determineCountViaGetAttribute();
		}
	}
	else
	{
		determineCountViaListChildren();
	}
}

void SessionQueryCategory::determineCountViaListChildren()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::determineCountViaListChildren:we have to find the count of category %lu", mResultType);
	std::string browsePath = "";
	MMPresTypes::eTitleContentType titleContentType = MMPresTypes::eTitleContentType::TITLETYPE_UNKNOWN;
	if (getDevice() != nullptr)
	{
		browsePath = getDevice()->getBrowsePath();
		browsePath.append("/");
		switch(mContentMask)
		{
		case MMPresTypes::eMetadataType::METADATATYPE_IMAGE:
			browsePath.append(BrowseHelper::IMAGE_CATEGORY_NAME);
			titleContentType = MMPresTypes::eTitleContentType::TITLETYPE_IMAGE;
			break;
		case MMPresTypes::eMetadataType::METADATATYPE_VIDEO:
			browsePath.append(BrowseHelper::VIDEO_CATEGORY_NAME);
			titleContentType = MMPresTypes::eTitleContentType::TITLETYPE_VIDEO;
			break;
		default:
			browsePath.append(BrowseHelper::MUSIC_CATEGORY_NAME);
			titleContentType = MMPresTypes::eTitleContentType::TITLETYPE_AUDIO;
		}
	}
	std::vector<std::string> filter = { "*" };
	mBrowser->listChildren(mBrowser.mClientID, browsePath, 0, 0, filter, std::bind(&SessionQueryCategory::categoryCountCallbackViaListChild, this, titleContentType,_1, _2, _3));
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::determineCountViaListChildren:browsePath=%s,titleType=%d", browsePath.c_str(), titleContentType);
	setIdle();
}

void SessionQueryCategory::categoryCountCallbackViaListChild(const MMPresTypes::eTitleContentType& titleContentType, const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
    if ( stat == CommonAPI::CallStatus::SUCCESS )
    {
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::categoryCountCallbackViaListChild :CallStatus : %d BrowserError %d count:%d titleContentType:%d", (int)stat, (int)err, map.size(), titleContentType);
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	DeviceBase* device = getDevice();
	uint16_t count = map.size();
	std::string itemPath = "";
	std::string itemName = "";
	uint64_t childCount = 0;
	std::string uri = "";
	std::string strTitleType = "";
	uint32_t itemIndex = 0;
   uint64_t itemId = 0;

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
							 strTitleType = BrowseHelper::getTitleContentType(titleContentType);
							 type = BrowseHelper::getContainerId(itemName, strTitleType);

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
                           LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::categoryCountCallbackViaListChild:itemName=%s,childcount : %llu", itemName.c_str(), childCount);
                           }// child count check
                           MMPresTypes::tMetadataItem item;
                           item.setID((i+1));
                           item.setBrowsable(true);
                           item.setPlayable(false);
                           item.setContentType(titleContentType);
                           item.setItemType(type);
                           item.setMetadataCountMap({ { type,childCount } });
                           FacadeHelper<MediaPresSettings> mSettings;
                           item.setValue(mSettings->getContainerName(type));
                           queryData->itemList.insert(std::make_pair((i+1), Session::ItemData(itemPath, "", childCount, itemId, item)));
                           itemIndex++;
							} // item metadata type check
						}// itr check
					} // for loop
					queryData->mBrowsePath = itemPath;
					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::categoryCountCallbackViaListChild:itemPath=%s", itemPath.c_str());
					queryData->mNumResults = itemIndex;
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
	if(sess != nullptr)
	{
		if (itemIndex > 0)
		{
			sess->setCurrentQueryId(mQueryId);
		}
		else
		{
			sess->removeQueryFromCache(mQueryId);
		}
	}
	determineBreadCrumbPath();
	sendQueryCount(result,itemIndex);
	JobCompleted();
    }
    else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::CommonAPI:: callstatus =%d", stat);
        JobCompleted();
    }
}

void SessionQueryCategory::determineCountViaGetAttribute()
{
	std::string browsePath = "";
	std::vector<std::string> filter = { BrowseHelper::CHILD_COUNT };

	if (getDevice() != nullptr)
	{
		browsePath = getDevice()->getBrowsePath();
		browsePath.append("/");

        switch (mContentMask)
		{
		case MMPresTypes::eTitleContentType::TITLETYPE_IMAGE:
			browsePath.append(BrowseHelper::IMAGE_CATEGORY_NAME);
			break;
		case MMPresTypes::eTitleContentType::TITLETYPE_VIDEO:
			browsePath.append(BrowseHelper::VIDEO_CATEGORY_NAME);
			break;
		default:
			browsePath.append(BrowseHelper::MUSIC_CATEGORY_NAME);
		}
	}

   if (nullptr != getSession())
   {
      Session::SessionQueryData* data = getSession()->getSessionQueryData(mQueryId);
      if (nullptr != data)
      {
         std::unordered_map<uint64_t, Session::ItemData>::const_iterator ctr = data->itemList.find(mCategoryIndex);
         MMPresTypes::eMetadataType metadataType = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;
         if (ctr != (data->itemList.end()))
         {
            metadataType = ctr->second.item.getItemType();
            browsePath.append("/");
            browsePath.append(BrowseHelper::getContainerName(metadataType));
            mBrowser->getAttributes(mBrowser.mClientID, browsePath, filter, std::bind(&SessionQueryCategory::categoryChildCountCallbackViagetAttribute, this, metadataType, _1, _2, _3));
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::determineCountViaGetAttribute:browsePath=%s,metadataType=%d object %p", browsePath.c_str(), (int)metadataType, this);
            setIdle();
         }
         else
         {
            //Do Nothing
         }
      }
   }
}


void SessionQueryCategory::categoryChildCountCallbackViagetAttribute(const MMPresTypes::eMetadataType& type, const CommonAPI::CallStatus& status, const MediaTypes::ResultMap& map, const BrowserTypes::BrowserError& browseError)
{
   if ( status == CommonAPI::CallStatus::SUCCESS )
   {
	   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::categoryChildCountCallbackViagetAttribute-metadataType:%d,CallStatus:%d BrowserError:%d count:%d Object %p",(int)type,(int)status, (int)browseError, map.size(), this);
	   MMPresTypes::eCallResult callResult = MMPresTypes::eCallResult::CALLRESULT_OK;
	   uint16_t num = map.size();
	   uint64_t childCount = 0;
	   Session* sess = getSession();
	   if (sess != nullptr)
	   {
		   if (getDevice() != nullptr)
		   {
			   if (browseError == BrowserTypes::BrowserError::NO_ERROR)
			   {
				   for (uint16_t i = 0; i < num; i++)
				   {
					   auto it = map.find(BrowseHelper::CHILD_COUNT);
					   if (it != map.end())
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
						   break;
					   }
				   }
                    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::categoryChildCountCallbackViagetAttribute-childCount=%llu type = %d", childCount, type);
                    FacadeHelper<MediaPresSettings> mSettings;
                    if(mSettings->getHMIindex() ==  Indexer::IndexOption::DISABLE_HIERARCHY)
                    {
                       hmiIndexingDisabled(type ,childCount);
                    }
                    else
                    {
                       hmiIndexingEnabled(type ,childCount);
                    }

                    sess->setCurrentQueryId(mQueryId);
                    Session::SessionQueryData* data = sess->getSessionQueryData(mQueryId);
                    if (data != nullptr)
                    {
                    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::categoryChildCountCallbackViagetAttribute-mCategoryIndex=%d , data->mNumResults = %d ", mCategoryIndex,data->mNumResults);
                    std::unordered_map<uint64_t, Session::ItemData>::iterator ctr = data->itemList.find(mCategoryIndex);
                    if (ctr != (data->itemList.end()))
                    {
                        MMPresTypes::eMetadataType metadataType = ctr->second.item.getItemType();
                        ctr->second.item.setMetadataCountMap({ { metadataType,childCount } });
                        if (mCategoryIndex >= data->itemList.size())
                        {
                           for(unsigned int i =0 ;i <zeroChildCountlist.size();i++)
                           {
                              data->itemList.erase(zeroChildCountlist.at(i));
                              data->mNumResults--;
                           }
                           sess->setCurrentQueryId(mQueryId);
                           zeroChildCountlist.clear();
                           mCategoryIndex++;
                           sendQueryCount(callResult, data->mNumResults);
                           JobCompleted();
                        }
                        else
                        {
                        mCategoryIndex++;
                        setReady();
                        }
                    }
                    else
                    {
                        callResult = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
                        JobCompleted();
                    }
				   }
				   else
				   {
					   callResult = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
					   JobCompleted();
				   }
			   }
			   else
			   {
				   callResult = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
				   JobCompleted();
			   }
		   }
		   else
		   {
			   callResult = MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED;
			   JobCompleted();
		   }
	   }
	   else
	   {
		   callResult = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
		   JobCompleted();
	   }
    }
    else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::CommonAPI:: callstatus =%d", stat);
        JobCompleted();
    }
}

void SessionQueryCategory::determineQueryItemList()
{
   Session::SessionQueryData* currData = nullptr;
   if (getSession() != nullptr)
   {
      currData = getSession()->getSessionQueryData(mQueryId);
   }
   MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;
   MMPresTypes::MetadataItemList metadataItemList;
   if (currData != nullptr)
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::determineQueryItemList-mContainerId=%llu,browsePath=%s,mStartIndex=%llu,mNumItems=%llu", mContainerId, currData->mBrowsePath.c_str(), mStartIndex, mNumItems);
      std::unordered_map<uint64_t, Session::ItemData>::const_iterator ctr = currData->itemList.begin();
      while (ctr != currData->itemList.end())
      {
         MMPresTypes::tMetadataItem item;
         item = ctr->second.item;
         metadataItemList.push_back(item);
         ctr++;
      }
   }
   sendQueryData(queryStatus, metadataItemList);
   JobCompleted();
}


void SessionQueryCategory::determineCategories()
{
	std::vector<std::string> filter = { "*" };
	std::string browsePath = "";
   if (getDevice() != nullptr)
   {
      browsePath = getDevice()->getBrowsePath();
   }
	browsePath.append("/");
	MMPresTypes::eTitleContentType titleContentType = MMPresTypes::eTitleContentType::TITLETYPE_UNKNOWN;
	switch(mContentMask)
	{
	   case MMPresTypes::eTitleContentType::TITLETYPE_VIDEO:
		   browsePath.append(BrowseHelper::VIDEO_CATEGORY_NAME);
		   titleContentType = MMPresTypes::eTitleContentType::TITLETYPE_VIDEO;
		   break;
	   case MMPresTypes::eTitleContentType::TITLETYPE_IMAGE:
		   browsePath.append(BrowseHelper::IMAGE_CATEGORY_NAME);
		   titleContentType = MMPresTypes::eTitleContentType::TITLETYPE_IMAGE;
		   break;
	   case MMPresTypes::eTitleContentType::TITLETYPE_AUDIO:
	   default:
		   browsePath.append(BrowseHelper::MUSIC_CATEGORY_NAME);
		   titleContentType = MMPresTypes::eTitleContentType::TITLETYPE_AUDIO;
		   break;
	}
	mBrowser->listChildren(mBrowser.mClientID, browsePath, 0, 0, filter, std::bind(&SessionQueryCategory::categoryCallback,this,browsePath,titleContentType,_1,_2,_3));
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::determineCategories-mContentMask %d", mContentMask);

	setIdle();
}

void SessionQueryCategory::categoryCallback(const std::string& browsePath, const MMPresTypes::eTitleContentType& titleContentType, const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err)
{
    if ( stat == CommonAPI::CallStatus::SUCCESS )
    {
	  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::categoryCallback :CallStatus : %d BrowserError %d count:%d titleContentType %d object %p", (int)stat, (int)err, map.size(),(int)titleContentType, this);

	uint16_t count = map.size();
	MMPresTypes::eCallResult callResult = MMPresTypes::eCallResult::CALLRESULT_OK;
	DeviceBase* device = mDetect->getDevice(mMediumId);
   uint64_t itemid = 0;

	mCategoriesObtained = true;
	if (device != nullptr)
	{
		std::string itemPath;
		std::string itemName;
		uint64_t itemIndex = 0;
		std::string uri="";
		std::string strTitleType = "";
		Session::SessionQueryData* queryData = nullptr;
		Session* sess = getSession();
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
							strTitleType = BrowseHelper::getTitleContentType(titleContentType);
							type = BrowseHelper::getContainerId(itemName, strTitleType);

							if (MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN != type) // check for containerID
							{
								  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryCategory::categoryCallback: itemName=%s", itemName.c_str());

								  it = result.find(BrowseHelper::URI);
								  if (it != result.end())
								  {
									  uri = it->second.get<std::string>();
								  }
								  
								  MMPresTypes::tMetadataItem item;
								  ++itemIndex;
								  item.setID((i + 1));
								  item.setBrowsable(true);
								  item.setPlayable(false);
								  item.setContentType(titleContentType);
								  item.setItemType(type);
								  FacadeHelper<MediaPresSettings> mSettings;
								  item.setValue(mSettings->getContainerName(type));
								  queryData->itemList.insert(std::make_pair(itemIndex, Session::ItemData(itemPath,"",count, itemid,item)));
							} // item metadata type check
						}// itr check
					} // for loop
					queryData->mBrowsePath = browsePath;
					queryData->mNumResults = itemIndex;
					queryData->mContainerId = mContainerId;
					setReady();
				} // browse error check
				else
				{
					callResult = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
					sendQueryCount(callResult, count);
					JobCompleted();
				}
			} // session query data existence check
			else
			{
				callResult = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
				sendQueryCount(callResult, count);
				JobCompleted();
			}
		} // session existence check
		else
		{
			callResult = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
			sendQueryCount(callResult, count);
			JobCompleted();
		}
	}// device existence check
	else
	{
		callResult = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
		sendQueryCount(callResult, count);
		JobCompleted();
	}
    }
	else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::CommonAPI:: callstatus =%d", stat);
        JobCompleted();
    }
}

void SessionQueryCategory::hmiIndexingDisabled(MMPresTypes::eMetadataType type , uint64_t childCount)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SessionQueryCategory::%s type %d childCount %llu" ,__FUNCTION__ ,(int32_t)type ,childCount);
   if (childCount == 0)
   {
      switch (type)
      {
         case MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK:
         case MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST:
         case MMPresTypes::eMetadataType::METADTATATYPE_PODCAST:
         case MMPresTypes::eMetadataType::METADATATYPE_TITLE:
         case MMPresTypes::eMetadataType::METADATATYPE_ARTIST:
         case MMPresTypes::eMetadataType::METADATATYPE_ALBUM:
         case MMPresTypes::eMetadataType::METADATATYPE_GENRE:
         case MMPresTypes::eMetadataType::METADATATYPE_COMPOSER:
         case MMPresTypes::eMetadataType::METADATATYPE_VIDEO:
         {
            zeroChildCountlist.push_back(mCategoryIndex);
            break;
         }
         default:
         {
            //Do Nothing
            break;
         }
      }
   }
   else if ((-1 == childCount)&& (MMPresTypes::eMetadataType::METADATATYPE_COMPILATION == type))// For devices which doesn't support iTunes radio
   {
      zeroChildCountlist.push_back(mCategoryIndex);
   }
   else
   {
      //Do Nothing
   }
}

void SessionQueryCategory::hmiIndexingEnabled(MMPresTypes::eMetadataType type , uint64_t childCount)
{
   LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SessionQueryCategory::%s type %d childCount %llu", __FUNCTION__, (int32_t)type, childCount);
   if (childCount == 0)
   {
      switch (type)
      {
         case MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK:
         case MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST:
         case MMPresTypes::eMetadataType::METADTATATYPE_PODCAST:
         {
            zeroChildCountlist.push_back(mCategoryIndex);
            break;
         }
         default:
         {
            //Do Nothing
            break;
         }
      }
   }
   else if ((-1 == childCount) && (MMPresTypes::eMetadataType::METADATATYPE_COMPILATION == type))// For devices which doesn't support iTunes radio
   {
      zeroChildCountlist.push_back(mCategoryIndex);
   }
   else
   {
      //Do Nothing
   }
}

