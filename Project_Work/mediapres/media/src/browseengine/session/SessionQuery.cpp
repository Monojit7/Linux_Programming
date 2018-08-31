#include <browseengine/session/SessionQuery.h>
#include <device/DeviceBase.h>

SessionQuery::SessionQuery(const uint32_t& sessionID,const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply,MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const  uint64_t & mediumID, const MMPresTypes::FilterIDMap & filterIDs, const MMPresTypes::FilterMap & filters, const MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex,const uint64_t& numItems)
	:RequestBase(e_REQUEST_SESSION_QUERY_TASK,"SessionQuery")
	,mSessionId(sessionID)
	,mQueryId(0u)
	,mMediumId(mediumID)
	,mFilterIDMap(filterIDs)
	,mFilters(filters)
	,mResultType(resultType)
   ,mBrowseResultType(resultType)
	,mMetadataAttributes(metadataAttributes)
	,mMetadataCountTypes(metadataCountTypes)
	,mQueryType(type)
	,mResetPrevious(resetPrevious)
	,mAutoUpdateEnabled(AutoUpdateEnabled)
	,mContentMask(ContentMask)
	,mQueryCallback(std::move(reply))
	,mGotoPreviousCallback(std::move(gotoPreviousReply))
	,mJumpToQueryCallback(std::move(jumpToQueryReply))
	,mClient(_client)
	,mStartIndex(startIndex)
	,mNumItems(numItems)
	,mQueryCached(false)
	,mUpdateCount(false)
	,mContainerId(0u)
	,mLevelId(0u)
	,mBreadCrumbPath("")
{
    
     requestQueueTokenMap [REQ_CLEAR_QUEUE_CD] = PresEventAbstraction::getInstance()->subscribe<eventrequestQueueCD>(REQ_CLEAR_QUEUE_CD, std::bind( & SessionQuery::doPlayStatus , this,_1));
	mQueryId =  mManager->registerSessionQuery(sessionID, this);
	std::unordered_map<MMPresTypes::eMetadataType, uint64_t>::const_iterator itr;
	uint64_t folderId = 0;
	itr = mFilterIDMap.find(MMPresTypes::eMetadataType::METADATATYPE_FOLDER);
	if (itr != mFilterIDMap.end())
	{
		folderId = itr->second;
	}
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::SessionQuery-folderId=%llu mContentMask = %d", folderId , mContentMask);
}

SessionQuery::SessionQuery(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply,const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
	:RequestBase(e_REQUEST_SESSION_QUERY_TASK, "SessionQuery")
	, mSessionId(sessionID)
	, mQueryId(queryID)
	, mMediumId(queryCacheData.mMediumId)
	, mFilterIDMap(queryCacheData.mFilterIDMap)
	, mFilters(queryCacheData.mFilters)
	, mResultType(queryCacheData.mResultType)
   , mBrowseResultType(queryCacheData.mResultType)
	, mMetadataAttributes(queryCacheData.mMetadataAttributes)
	, mMetadataCountTypes(queryCacheData.mMetadataCountTypes)
	, mQueryType(queryCacheData.mQueryType)
	, mResetPrevious(queryCacheData.mResetPrevious)
	, mAutoUpdateEnabled(queryCacheData.mAutoUpdateEnabled)
	, mContentMask(queryCacheData.mContentMask)
	, mQueryCallback(std::move(reply))
	, mGotoPreviousCallback(std::move(gotoPreviousReply))
	, mJumpToQueryCallback(std::move(jumpToQueryReply))
	, mClient(_client)
	, mStartIndex(startIndex)
	, mNumItems(numItems)
	, mQueryCached(true)
	, mContainerId(queryCacheData.mContainerId)
	, mLevelId(queryCacheData.mLevelId)
	, mBreadCrumbPath(queryCacheData.mBreadCrumbPath)
{
    
     requestQueueTokenMap [REQ_CLEAR_QUEUE_CD] = PresEventAbstraction::getInstance()->subscribe<eventrequestQueueCD>(REQ_CLEAR_QUEUE_CD, std::bind( & SessionQuery::doPlayStatus , this,_1));

}

SessionQuery::~SessionQuery()
{
    PRESEVENT_UNSUBSCRIBE( REQ_CLEAR_QUEUE_CD, requestQueueTokenMap [REQ_CLEAR_QUEUE_CD] );

}

void SessionQuery::onCancelled()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::onCancelled-queryId=%lu",mQueryId);
    
	MMPresTypes::MetadataItemList list;
	list.clear();
	sendQueryCount(MMPresTypes::eCallResult::CALLRESULT_CANCELLED_USER_REQUEST, 0);
	sendQueryData(MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED, list);

	JobCompleted();
}



void SessionQuery::sendQueryCount(const MMPresTypes::eCallResult& result,const uint64_t& count)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::sendQueryCount-queryId=%lu count %d", mQueryId, count);

   determineBrowseResultType();
   sendItemIndexTobeHighlighted();

	if (mQueryCallback != nullptr)
	{
		if ( mediaSettings -> is_valid_utf8 ( mBreadCrumbPath.c_str() ) )
		{
         std::string l_BreadCrumbPath = mBreadCrumbPath;
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< l_BreadCrumbPath1 %s  ", l_BreadCrumbPath.c_str());

         if (MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE != mQueryType)
         {
            l_BreadCrumbPath = BrowseHelper::removeStaticTextToken(l_BreadCrumbPath);
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< l_BreadCrumbPath2 %s  ", l_BreadCrumbPath.c_str());
         }
         l_BreadCrumbPath = BrowseHelper::updateBreadCrumbPath(l_BreadCrumbPath);
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d valid breadCrumbpath %s", __FUNCTION__, __LINE__, l_BreadCrumbPath.c_str() );
			mQueryCallback(result,count,mQueryId, l_BreadCrumbPath, mBrowseResultType);
		}
		else
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d not valid breadCrumbpath so root", __FUNCTION__, __LINE__ );
			mQueryCallback(result,count,mQueryId, "/" , mBrowseResultType);
		}
	}
	else if (mGotoPreviousCallback != nullptr)
	{
		if ( mediaSettings -> is_valid_utf8 ( mBreadCrumbPath.c_str() ) )
		{
         std::string l_BreadCrumbPath = mBreadCrumbPath;
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< l_BreadCrumbPath1 %s  ", l_BreadCrumbPath.c_str());

         if (MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE != mQueryType)
         {
            l_BreadCrumbPath = BrowseHelper::removeStaticTextToken(l_BreadCrumbPath);
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< l_BreadCrumbPath2 %s  ", l_BreadCrumbPath.c_str());
         }
         l_BreadCrumbPath = BrowseHelper::updateBreadCrumbPath(l_BreadCrumbPath);
         LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d valid breadCrumbpath %s  ", __FUNCTION__, __LINE__, l_BreadCrumbPath.c_str());
			mGotoPreviousCallback(result, count, mQueryId,MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT, l_BreadCrumbPath, mBrowseResultType);
		}
		else
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d not valid breadCrumbpath so root   ", __FUNCTION__, __LINE__);
			mGotoPreviousCallback(result, count, mQueryId,MMPresTypes::ePrevBrowseLevel::PREVBROWSELEVEL_DEFAULT,"/", mBrowseResultType);
		}
	}
	else if(mJumpToQueryCallback!=nullptr)
	{
		if ( mediaSettings -> is_valid_utf8 ( mBreadCrumbPath.c_str() ) )
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d valid breadCrumbpath %s  ", __FUNCTION__, __LINE__, mBreadCrumbPath.c_str() );
			mJumpToQueryCallback(result, count,mBreadCrumbPath);
		}
		else
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d not valid breadCrumbpath so root   ", __FUNCTION__, __LINE__);
			mJumpToQueryCallback(result, count,  "/" );
		}
	}
	else
	{
	}
}


void SessionQuery::doPlayStatus ( const bool& doPlayStatus )
{
    
    //cancelTimer ();
    
    if ( doPlayStatus == true )
    {
      MMPresTypes::MetadataItemList metadataItemList;
      MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
      metadataItemList.clear(); 
      sendQueryData(queryStatus, metadataItemList);
      JobCompleted ();
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d Job Completed for current object because of doPlay  ", __FUNCTION__, __LINE__);
    }
    
    
}

void SessionQuery::determineBrowseResultType()
{
   //If the selection is made from Root Folder 
   switch (mQueryType)
   {
      case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE:
      {
         DeviceBase* pDevice = mDetect->getDevice(mMediumId);
         if (nullptr != pDevice)
         {
            if (true == pDevice->getVideoPlayStatus())
            {
               // For Video -
               mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_RATING;
            }
            else
            {
               //For Audio -
               mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_NAME;
            }
         }
         break;
      }
      case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE:
      {
         mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_FOLDER;
         break;
      }
      case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_CATEGORY_BROWSE:
      {
         mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_CATEGORY;
         break;
      }
      case MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE:
      {
         switch (mResultType)
         {
         case MMPresTypes::eMetadataType::METADATATYPE_ARTIST:
         {
            if (!mBreadCrumbPath.compare("/artists"))
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_ARTIST;
            }
            else
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_ALBUM;
            }
            break;
         }
         case MMPresTypes::eMetadataType::METADATATYPE_ALBUM:
         {
            if (!mBreadCrumbPath.compare("/albums"))
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_ALBUM;
            }
            else
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_TITLE;
            }
            break;
         }
         case MMPresTypes::eMetadataType::METADATATYPE_GENRE:
         {
            if (!mBreadCrumbPath.compare("/genres"))
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_GENRE;
            }
            else
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_ALBUM;
            }
            break;
         }
         case MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST:
         {
            if (!mBreadCrumbPath.compare("/playlists"))
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST;
            }
            break;
         }
         case MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK:
         {
            if (!mBreadCrumbPath.compare("/audiobooks"))
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK;
            }
            else
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADTATATYPE_CHAPTER;
            }
            break;
         }
         case MMPresTypes::eMetadataType::METADTATATYPE_PODCAST:
         {
            if (!mBreadCrumbPath.compare("/podcasts"))
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADTATATYPE_PODCAST;
            }
            else
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADTATATYPE_EPISODE;
            }
            break;
         }
         case MMPresTypes::eMetadataType::METADATATYPE_COMPOSER:
         {
            if (!mBreadCrumbPath.compare("/composers"))
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_COMPOSER;
            }
            else
            {
               mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_ALBUM;
            }
            break;
         }

         case MMPresTypes::eMetadataType::METADATATYPE_TITLE:
         {
        	 /* below case is added only for video playlist
        	  * For video playlist selection/browsing, HMI expect browseResultType as "METADATATYPE_VIDEO" to show video thumbnails for files.
        	  * When any playlist is browsed, HMI calls setSessionQuery with resultType as Title & QueryType as MetadataBrowse.
        	  * But FilterIDMap contains playList.
        	  */

          if(mContentMask == MMPresTypes::eTitleContentType::TITLETYPE_VIDEO)
          {
             if(!mFilterIDMap.empty())
             {
                std::unordered_map<MMPresTypes::eMetadataType, uint64_t>::const_iterator itr;
                itr = mFilterIDMap.begin();
                while (itr != mFilterIDMap.end())
                {
                   if(itr->first == MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST)
                   {
                      mBrowseResultType = MMPresTypes::eMetadataType::METADATATYPE_VIDEO;
                   }
                   itr++;
                }
             } //if(!mFilterIDMap.empty())
          } //mContentMask == MMPresTypes::eTitleContentType::TITLETYPE_VIDEO

          break;
         }

         default:
         {
            //Do Nothing
         }
         break;
         }
      }
      default:
      {
         //Do Nothing
      }
   }
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineResultType-mBrowseResultType=%d mResultType=%d", mBrowseResultType , mResultType);
}

void SessionQuery::sendQueryData(const MMPresTypes::eQueryStatus& status, const MMPresTypes::MetadataItemList& list,const bool& windowContainsCurrentlyPlayingTrack,const uint64_t& itemIndexForHighlight)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::sendQueryData-queryId=%lu",mQueryId);
	if (mQueryCallback == nullptr && mGotoPreviousCallback == nullptr)
	{
		std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::eQueryStatus &_status, const uint64_t &_startIndex, const uint64_t &_numItems, const ::v3::com::harman::mmpres::MMPresTypes::MetadataItemList &_itemList, const bool &_windowContainsCurrentlyPlayingTrack, const uint64_t &_itemIndexTobeHighlighted)>& func = mBrowser->getWindowUpdateHandler();

      if (func != nullptr)
      {
         func(mSessionId, mQueryId, status, mStartIndex, mNumItems, list, windowContainsCurrentlyPlayingTrack, itemIndexForHighlight);
      }
	}
}

void SessionQuery::sendQueryDataforThumbnail(const MMPresTypes::eQueryStatus& status, const std::vector<std::string> & thumbnails)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::sendQueryDataforThumbnail-queryId=%lu",mQueryId);
    if (mQueryCallback == nullptr && mGotoPreviousCallback == nullptr)
    {
        std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const uint64_t &_startIndex, const uint64_t &_numItems, const std::vector<std::string> &_thumbnaiList)>& func = mBrowser->getThumbnailListHandler();

        if(func != nullptr)
        {
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::sendQueryDataforThumbnail- sending thumbnail data to HMI");
        	func(mSessionId, mQueryId, mStartIndex,mNumItems, thumbnails );
        }
    }

}

void SessionQuery::sendQueryDataforAlphajump(const MMPresTypes::eQueryStatus& status, const MMPresTypes::arAlphaIndex& ajList)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::sendQueryDataforAlphajump-queryId=%lu",mQueryId);
	if (mQueryCallback == nullptr && mGotoPreviousCallback == nullptr)
	{
		std::function<void(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::arAlphaIndex &_ajList)>& func = mBrowser->getAlphajumpListHandler();
		if (func != nullptr)
		{
			func(mSessionId, mQueryId, ajList );
		}	
	}
}

 DeviceBase* SessionQuery::getDevice()
 {
	 return mDetect->getDevice(mMediumId);
 }

 Session* SessionQuery::getSession()
 {
	return mManager->getSession(mSessionId);
 }

 uint64_t SessionQuery::getQueryCount(uint32_t queryId)
 {
	 uint64_t count = 0;
	 if (getSession() != nullptr)
	 {
		 Session::SessionQueryData* data = nullptr;
		 data = getSession()->getSessionQueryData(queryId);
		 if (data != nullptr)
		 {
			 return data->mNumResults;
		 }
	 }
	 return count;
 }

 void SessionQuery::setCurrentQuery(uint32_t queryId)
 {
	 Session* getSessionptr = getSession();
	 if (getSessionptr != nullptr)
	 {
		 getSessionptr->setCurrentQueryId(mQueryId);
	 }
 }

 uint64_t SessionQuery::getQueryCountFromCachedItemList()
 {
	 Session::SessionQueryData* prevData = nullptr, *currData = nullptr;
	 Session* tmpSession = nullptr;
	 uint32_t currQueryId;

	 tmpSession = getSession();

	 if(tmpSession != nullptr)
	 {
		 currQueryId = tmpSession->getCurrentQueryId();
		 prevData    = tmpSession->getSessionQueryData(currQueryId);
		 currData    = tmpSession->getSessionQueryData(mQueryId);
	 }

	 LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getQueryCountFromCachedItemList- determining the new query count, iterate through itemlist of previous cached query and get the count");
	 uint64_t count = 0;
	 if (prevData != nullptr)
	 {
		 std::unordered_map<uint64_t, Session::ItemData>::const_iterator itr;
		 for (itr = prevData->itemList.begin(); itr != prevData->itemList.end(); itr++)
		 {
			 if (mContainerId == itr->second.item.getID())
			 {
				 LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getQueryCountFromCachedItemList- found an container item with containerId=%llu,browse path=%s from the previous item list", mContainerId, itr->second.mItemPath.c_str());
				 count = itr->second.mChildcount;
				 // update cache...this is mandatory
				 if (currData != nullptr)
				 {
					 currData->mPlayURI = itr->second.mItemPlayUri;
					 currData->mBrowsePath = itr->second.mItemPath;
					 currData->mNumResults = itr->second.mChildcount;
					 currData->mContainerId = mContainerId;
				 }
				 break;
			 }
		 }
	 }// end of prevData check
	 return count;
 }

 std::string SessionQuery::getItemPathFromCachedItemList()
 {
	 std::string path = "";
	 Session::SessionQueryData* prevData = nullptr, *currData = nullptr;
	 Session* tmpSession = nullptr;
	 tmpSession = getSession();
	 uint32_t currQueryId = 0;

	 if(tmpSession != nullptr)
	 {
		 currQueryId= tmpSession->getCurrentQueryId();
		 prevData = tmpSession->getSessionQueryData(currQueryId);
		 currData = tmpSession->getSessionQueryData(mQueryId);
		 LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getItemPathFromCachedItemList- currQueryId=%lu", currQueryId);
	 }

	 LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getItemPathFromCachedItemList- determining the new query count, iterate through itemlist of previous cached query and get the count");
	 if (prevData != nullptr)
	 {
		 std::unordered_map<uint64_t, Session::ItemData>::const_iterator itr;
		 for (itr = prevData->itemList.begin(); itr != prevData->itemList.end(); itr++)
		 {
			 if (mContainerId == itr->second.item.getID())
			 {
				 LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getItemPathFromCachedItemList- found an container item with containerId=%llu,browse path=%s from the previous item list", mContainerId, itr->second.mItemPath.c_str());
				 path = itr->second.mItemPath;
				 if (currData != nullptr)
				 {
					 currData->mPlayURI = itr->second.mItemPlayUri;
					 currData->mBrowsePath = itr->second.mItemPath;
					 currData->mContainerId = mContainerId;
				 }
				 break;
			 }
		 }
	 }// end of prevData check
	 return path;
 }

 void SessionQuery::determineBreadCrumbPath()
 {
     LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d NNNNNNNNNNNNNNNNNNNNNNN", __FUNCTION__, __LINE__);
    std::string name = "";
    std::string breadCrumbPath;
    std::string path = "/";
    Session::SessionQueryData* prevData = nullptr, *currData = nullptr;
    uint32_t prevId = getSession()->getPreviousQueryId(mQueryId);
    prevData = getSession()->getSessionQueryData(prevId);
    currData = getSession()->getSessionQueryData(mQueryId);
    DeviceBase* pDevice = mDetect->getDevice(mMediumId);
    // breadcrumb path for root level
    if (mContainerId == 0)
    {
       if (mQueryType == MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE)
       {
          LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineBreadCrumbPath- for root");
          if(pDevice != nullptr)
          {
             LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineBreadCrumbPath- pDevice->getDeviceName():%s",pDevice->getDeviceName().c_str());
            //path.append(BrowseHelper::FOLDER_CATEGORY_NAME);
             path.append(pDevice->getDeviceName());
          }
       }
       else if (mQueryType == MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE)
       {
          LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineBreadCrumbPath- for Metadata Browse");
          path.append(BrowseHelper::getBreadCrumbCategoryName(mResultType));
       }
       else if (mQueryType == MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE)
       {
          LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineBreadCrumbPath- for NowPlayingList");
          if(pDevice != nullptr)
          {
            path ="";
            path.append(pDevice->getBreadCrumbForSetSession());
          }
       }
		 else
		 {
		 }
		 mBreadCrumbPath = path;
	 }
	 else
	 {
		// compute breadcrumb by appending to the previos breadcrumbpath
		 if (prevData != nullptr)
		 {
			 std::unordered_map<uint64_t, Session::ItemData>::const_iterator itr;
          const std::string search = "/";
          const std::string replace = "|";
          size_t pos = 0;
			 for (itr = prevData->itemList.begin(); itr != prevData->itemList.end(); itr++, pos=0)
			 {
				 if (mContainerId == itr->second.item.getID())
				 {
                LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineBreadCrumbPath-found an container item with containerId=%llu,name=%s from the previous item list", mContainerId, itr->second.item.getValue().c_str());
                name = itr->second.item.getValue();
                //pos = 0;
                while ((pos = name.find(search, pos)) != std::string::npos)
                {
                  name.replace(pos, search.length(),replace);
                  pos += replace.length();
                }
					 break;
				 }
			 }
         breadCrumbPath.append(prevData->mBreadCrumbPath.c_str());
         path.append(name.c_str());
         breadCrumbPath.append(path.c_str());
         mBreadCrumbPath = breadCrumbPath;
			 LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineBreadCrumbPath- prev query breadCrumbPath=%s mBreadCrumbPath=%s", prevData->mBreadCrumbPath.c_str(), mBreadCrumbPath.c_str());
		 }
	 }
	 if (currData != nullptr)
	 {
		 LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineBreadCrumbPath- updating breadCrumbPath=%s to cache", mBreadCrumbPath.c_str());
		 currData->mBreadCrumbPath = mBreadCrumbPath;
	 }
 }

bool SessionQuery::checkIfWindowContainsCurrentTrack(uint64_t& index)
{
	bool ret = false;
	index = 0;
	DeviceBase* device = getDevice();
	if (device != nullptr)
	{
		uint64_t currTrack = (device->getTrackState()).getCurrentTrack();

		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::checkIfWindowContainsCurrentTrack-currTrack=%llu, mStartIndex=%llu, mNumItems=%llu", currTrack, mStartIndex, mNumItems);

		if (currTrack > 0 && currTrack>=mStartIndex && currTrack<=mNumItems)
		{
		  ret = true;
		  index = currTrack - mStartIndex + 1;
		}
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::checkIfWindowContainsCurrentTrack-currTrack=%llu,index=%llu", currTrack,index);
	}
	return ret;
}

bool SessionQuery::checkIfWindowDataCached(MMPresTypes::MetadataItemList& itemList)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< %s %d OOOOOOOOOOOOOOOOOOOOOOOOOOO", __FUNCTION__, __LINE__);
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

void SessionQuery::determineData()
{
	MMPresTypes::MetadataItemList itemList;
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineData:retrieve the item list for queryId=%lu,startIndex=%llu,numItems=%llu", mQueryId, mStartIndex, mNumItems);
	bool ret = checkIfWindowDataCached(itemList);
	if (ret)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<<SessionQuery::determineData:send data from cache");
		MMPresTypes::eQueryStatus stat = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;
		sendQueryData(stat, itemList);
		//determineAlphajumpData();

		/* For metadata browse, data is cached.
		 * For video we have to send thumbnail data on the fly and then only setCompleted()
		 */
		if((mQueryType == MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE) && (mContentMask == MMPresTypes::eTitleContentType::TITLETYPE_VIDEO))
		{
			std::string query = "";

			Session::SessionQueryData* currData = nullptr;
			currData = getSession()->getSessionQueryData(mQueryId);

			if(currData != nullptr)
			{
				mBrowser->listThumbnails(mBrowser.mClientID,currData->mBrowsePath,mStartIndex-1,mNumItems,query,std::bind(&SessionQuery::listThumbnailsDataCallback,this,_1,_2,_3));
            LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineData: calling listThumbnails when data is cached");
			    setIdle();
			}
			else
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineData: Error as SessionQueryData is not found");
				JobCompleted () ;
			}
		}
		else
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<<SessionQuery::determineData: Job completed");
			JobCompleted();
		}
	} //if (ret)
	else
	{
		determineQueryItemList();
	}
}

void SessionQuery::determineAlphajumpData()
{
	Session::SessionQueryData* currData = nullptr;
   if (nullptr != getSession())
   {
      currData = getSession()->getSessionQueryData(mQueryId);
   }
   else
   {
       JobCompleted();
   }
   if (nullptr != currData)
   {
      ::v1::org::genivi::mediamanager::BrowserTypes::SortKey sortKey;
      sortKey.setOrder(::v1::org::genivi::mediamanager::BrowserTypes::SortOrder::Literal::ASCENDING); sortKey.setKeyName("");
      mBrowser->getAlphaJumpInfo(mBrowser.mClientID, currData->mBrowsePath, sortKey, std::bind(&SessionQuery::getAlphaJumpInfoDataCallback, this, _1, _2, _3));
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::determineAlphajumpData-calling getAlphaJumpInfo - mBrowsePath = %s", currData->mBrowsePath.c_str());
   }
   else
   {
      LOG_INFO(LOG_CONTEXT_MEDIA, "<<< CurrData ptr is NULL!!");
      JobCompleted();
   }
}

void SessionQuery::determineQueryItemList()
{

}

void SessionQuery::processAndSendCachedCount()
{
	uint64_t totCount = 0;
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	Session* getSessionPtr = nullptr;

	if (mGotoPreviousCallback != nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::processAndSendCachedCount-get cached count for gotoPrevious");
	}
	else if (mJumpToQueryCallback != nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::processAndSendCachedCount-get cached count for jumptoSessionquery");
	}
	else
	{

	}

	totCount = getQueryCount(mQueryId);
	getSessionPtr = getSession();

	if(getSessionPtr != nullptr)
	{
		if (totCount > 0)
		{
			getSessionPtr->setCurrentQueryId(mQueryId);
			// Note:This is mandatory else medioane breaks.
			Session::SessionQueryData* data = getSessionPtr->getSessionQueryData(mQueryId);
			if (data != nullptr)
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::processAndSendCachedCount-clearing item list");
				data->itemList.clear();
			}
		}
		else
		{
			getSessionPtr->removeQueryFromCache(mQueryId);
		}
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, " SessionQuery::processAndSendCachedCount ERROR : Session Pointer is NULL");
	}
	sendQueryCount(result, totCount);
    JobCompleted();
	//setCompleted();
}

void SessionQuery::getAttributeCallback(const std::string browsePath,const CommonAPI::CallStatus & stat, const MediaTypes::ResultMap& map, const BrowserTypes::BrowserError& error)
{
    if ( ( stat == CommonAPI::CallStatus::SUCCESS ) && ( error == ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError::NO_ERROR  ) )
    {
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAttributeCallback-callStatus=%d,browsePath=%s,Browser error:%d, ResultMap size=%d", (int)stat, browsePath.c_str(), (int)error, (int)map.size());
     

	MMPresTypes::eCallResult callResult = MMPresTypes::eCallResult::CALLRESULT_OK;
	uint16_t num = map.size();
	uint64_t childCount = 0;
	DeviceBase* pDevice = mDetect->getDevice(mMediumId);
	Session* sess = getSession();
	if (sess != nullptr)
	{
		if (getDevice() != nullptr)
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
		}
		else
		{
			callResult = MMPresTypes::eCallResult::CALLRESULT_FAILED_MEDIUM_REMOVED;
		}
	}
	else
	{
		callResult = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
	}

	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAttributeCallback-childcount=%llu", childCount);
	if (childCount > 0)
	{
		if (sess != nullptr)
		{
			/* Below code is added for smart search.
			 * For any sort of smart search (entering into smart search windows), do not add that queryID into queryList.
			 * This is because, HMI will take care of back button for smart search specific windows in terms of calling setSessionQuery() again and agin
			 */
			Session::SessionQueryData* prevData = nullptr;
			uint32_t currQueryId = sess->getCurrentQueryId();
			prevData = sess->getSessionQueryData(currQueryId);
			if((prevData != nullptr) && (prevData->mResultType == mResultType) && (mQueryType == MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE))
			{
				/* Above check is to identify that smart search and multilevel smart search. So do not insert current/newly created queryID into QueryList
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
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAttributeCallback-not inserting mQueryId=%lu", mQueryId);
			}
			else
				sess->insertToQueryList(mQueryId);

			sess->setCurrentQueryId(mQueryId);
			Session::SessionQueryData* data = sess->getSessionQueryData(mQueryId);
			if (data != nullptr)
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAttributeCallback-updating childcount=%llu for the current query",childCount);
				data->mNumResults = childCount;
				data->mBrowsePath = browsePath;
				data->mContainerId = mContainerId;
			}
		}
	}
	else if((pDevice !=nullptr )&&(childCount == 0))
	{
		if((MMPresTypes::eDeviceType::DEVICETYPE_BLUETOOTH == pDevice->getDevType())||(mResultType == MMPresTypes::eMetadataType::METADATATYPE_COMPILATION))
		{
		  sess->insertToQueryList(mQueryId);
          sess->setCurrentQueryId(mQueryId);
		}
	}
	else
	{
      if (sess!=nullptr)
         sess->removeQueryFromCache(mQueryId);
	}
   LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAttribute-mQueryId=%lu", mQueryId);
	determineBreadCrumbPath();
	sendQueryCount(callResult, childCount);
    JobCompleted();
    }
    else
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::CommonAPI:: callstatus =%d", stat);
        JobCompleted();
    }
	//setCompleted();
}


void SessionQuery::listThumbnailsDataCallback(const CommonAPI::CallStatus& status,  const std::vector<std::string> &_thumbnails, const ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError& error)
{
	if ( ( status == CommonAPI::CallStatus::SUCCESS ) && ( error == ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError::NO_ERROR  ) )
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::listThumbnailsDataCallback: status=%d,error=%d,ListOfThumbnails=%d", (int)status, (int)error,(int)_thumbnails.size());
		LOG_INFO(LOG_CONTEXT_MEDIA, "REQUEST : listThumbnailsDataCallback RESPONSE %d", this );
		MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_ABORTED;
		Session* sess = getSession();
		DeviceBase* device = getDevice();
		for(unsigned int i =0 ;i <_thumbnails.size() ;i++)
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::listThumbnailsData %s",_thumbnails.at(i).c_str());
		if (device == nullptr)
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::listThumbnailsDataCallback :device not available");
			if (sess != nullptr)
			{
				sess->removeQueryFromCache(mQueryId);
			}
		}
		else
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::listThumbnailsDataCallback :device available");

			if (error == BrowserTypes::BrowserError::NO_ERROR)
			{
				if (sess != nullptr)
				{
					sendQueryDataforThumbnail(queryStatus, _thumbnails);
				}
			}
		}
		// setCompleted();
		JobCompleted();
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::listThumbnailsDataCallback:: callstatus =%d, error =%d", (int)status, (int)error);
		JobCompleted();
	}
}

void SessionQuery::getAttributesCallbackForSmartSearch(const CommonAPI::CallStatus& status, const ::v1::org::genivi::mediamanager::MediaTypes::ResultMap& map, const ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError& error)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAttributesCallbackForSmartSearch- CallStatus:%d BrowserError:%d count:%d",(int)status, (int)error, map.size());

    LOG_INFO(LOG_CONTEXT_MEDIA, "REQUEST : getAttributesCallbackForSmartSearch RESPONSE %d", this );
	Session* sess = getSession();
	uint16_t num = map.size();

	//here assumption is for smart-search, always "map" containing candidates is of size 1 only.
	for (uint16_t i = 0; i < num; i++)
	{
        auto it = map.find(BrowseHelper::SMART_SEARCH_CANDIDATES);
        if(it != map.end())
        {
        	  //get candidates list from map
              std::vector<std::string> candidateList = it->second.get<std::vector<std::string>>();
              LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAttributesCallbackForSmartSearch- candidateList size = %d", candidateList.size());

#if 0 //below code is for debugging purpose
              for(auto candidate : candidateList)
              {
                  LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAttributesCallbackForSmartSearch- candidate = %s", candidate.c_str());
              }
#endif
              //directly send candidates to HMI through attributes of mpres.
              if (mQueryCallback == nullptr && mGotoPreviousCallback == nullptr)
              {
            	  std::function<void(const std::vector<std::string>&)> callback = mBrowser->getSearchCandidateStubCallback();
            	  if(callback != nullptr)
            		  callback(candidateList);
              }
        } //if(it != map.end())

	} //for (uint16_t i = 0; i < num; i++)

	if(sess != nullptr)
	{
		determineAlphajumpData();
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAttributesCallbackForSmartSearch- Error as Session is not found");
		//setCompleted();
        JobCompleted();
	}
}


void SessionQuery::getAlphaJumpInfoDataCallback(const CommonAPI::CallStatus& status, const ::v1::org::genivi::mediamanager::MediaTypes::arAlphaIndex& _ajList, const ::v1::org::genivi::mediamanager::BrowserTypes::BrowserError& error)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAlphaJumpInfoDataCallback: status=%d,error=%d,ListOfAlphabets=%d", (int)status, (int)error,(int)_ajList.size());

	LOG_INFO(LOG_CONTEXT_MEDIA, "REQUEST : getAlphaJumpInfoDataCallback RESPONSE %d", this );
	MMPresTypes::eQueryStatus queryStatus = MMPresTypes::eQueryStatus::QUERY_STATE_FINISHED;
	Session* sess = getSession();
	DeviceBase* device = getDevice();

	if (device == nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAlphaJumpInfoDataCallback :device not available");
		if (sess != nullptr)
		{
			sess->removeQueryFromCache(mQueryId);
		}
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAlphaJumpInfoDataCallback :device available");

		/* Specifically error check is removed as for unsorted cases, mediaone returns error as BAD_PATH with empty alpha jump list
		 * mpres sends this empty alpha-jump list to HMI.
		 * HMI will decide to show A-Z/Smart search button depending upon whether A-Z list is empty or not for unsorted screens.
		 * For e.g. tracks inside albums etc.
		 */
		//if (error == BrowserTypes::BrowserError::NO_ERROR)
		{
			if (sess != nullptr)
			{
				MMPresTypes::arAlphaIndex aJListCopy;
				MMPresTypes::tAlphaIndex aIndex;

				for(unsigned int i = 0 ;i < _ajList.size(); i++)
				{
					aIndex.setAlpha(_ajList.at(i).getAlpha());
					aIndex.setIndex(_ajList.at(i).getIndex());

					LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::getAlphaJumpInfoDataCallback : index = %d, character = %s", _ajList.at(i).getIndex(), _ajList.at(i).getAlpha().c_str());

					aJListCopy.push_back(aIndex);
				}
				sendQueryDataforAlphajump(queryStatus, aJListCopy);

				/* smart search and alpha jump candidates are sent. So make flag as false */
				device->setSmartSearchAlphaJumpCandidateRequiredToBeSend(false);

			} //if (sess != nullptr)
		}
	} //else of if (device == nullptr)

	JobCompleted();
}

void SessionQuery::JobCompleted () 
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "SessionQuery JobCompleted :%d", this );
    setCompleted();
}

void SessionQuery::sendItemIndexTobeHighlighted()
{
	/* below code is to send current playing track to HMI for only nowPlaying list browsing part
	 * So HMI can highlight that current playing track in nowPlaying list browse window */

	MMPresTypes::HighlightItemIndexMap highlightItemIndexMap;
	highlightItemIndexMap.clear();

	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::sendItemIndexTobeHighlighted- mQueryType = %d, mSessionId = %lu", mQueryType, mSessionId);

	if(mQueryType == MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE)
	{
		DeviceBase* device = getDevice();
		if(device != nullptr)
		{
			uint64_t currTrack = (device->getTrackState()).getCurrentTrack();

			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQuery::sendItemIndexTobeHighlighted- For nowPlaying Browse: currTrack=%llu", currTrack);

			highlightItemIndexMap[mSessionId] = currTrack;
		}

	} //   if(mQueryType == MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE)

	for(auto& iterator: highlightItemIndexMap)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SessionQuery::sendItemIndexTobeHighlighted - trackNumber = %llu for sessionId = %lu from highlightItemIndexMap", iterator.second, iterator.first);
	}

	if(!highlightItemIndexMap.empty())
	{
		std::function<void(const MMPresTypes::HighlightItemIndexMap&)> itemIndexTobeHighlightedStubCallBack = mBrowser->getItemIndexTobeHighlightedStubCallBack();

		if(itemIndexTobeHighlightedStubCallBack != nullptr)
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, ">>> SessionQuery::sendItemIndexTobeHighlighted - sending highlightItemIndexMap to HMI");
			itemIndexTobeHighlightedStubCallBack(highlightItemIndexMap);
		}

	} //if(!highlightItemIndexMap.empty())

}



