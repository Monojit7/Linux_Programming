/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            session.h
* @ingroup         mediapresctrl
* @author          sriram srivatsan (sriram.srivatsan@harman.com)
* @brief           This contains the information about a session(mainly used for browsing).Info about the session queries for particular session
                   is maintained here.Also session cache info is maintained here.This also contains interfaces to add a new query to a session,
				   clearing a session etc. The session methods are triggered from different clients via the session manager.
*/

#ifndef _SESSION_
#define _SESSION_

#include <unordered_map>
#include <stdint.h>
#include <string>

class SessionQuery;

using namespace v3::com::harman::mmpres;

class Session
{
public : 

	struct ItemData
	{
		std::string mItemPath;
		std::string mItemPlayUri;
		uint64_t mChildcount;
      uint64_t mItemId;
		MMPresTypes::tMetadataItem item;
		ItemData()
		{

		}
		ItemData(const std::string& itmPath, const std::string& itmPlayUri,const uint64_t& childcount, const uint64_t& itmId, MMPresTypes::tMetadataItem& itm)
		:mItemPath(itmPath)
		,mItemPlayUri(itmPlayUri)
		,mChildcount(childcount)
      ,mItemId(itmId)
		,item(itm)
		{

		}
	};

	// this struct reperesents the session query data that is cached.
	struct SessionQueryData
	{
		uint32_t mQueryID;
		uint64_t mContainerId;
		int32_t mLevelId;
		uint64_t mMediumId;
		std::string mPlayURI;
		std::string mBrowsePath;
		MMPresTypes::FilterIDMap mFilterIDMap;
		MMPresTypes::FilterMap mFilters;
		MMPresTypes::eMetadataType mResultType;
      MMPresTypes::eMetadataType mBrowseResultType;
		uint32_t mMetadataAttributes;
		uint32_t mMetadataCountTypes;
		MMPresTypes::eSessionQueryTypes mQueryType;
		bool mAutoUpdateEnabled;
		bool mResetPrevious;
		uint32_t mContentMask;
		uint64_t mNumResults;
		std::string mBreadCrumbPath;
		std::unordered_map<uint64_t, Session::ItemData> itemList;
		SessionQueryData()
		{

		}
		SessionQueryData(const uint32_t& queryId,const uint64_t& mediumId,const  MMPresTypes::FilterIDMap& filterIDMap,const MMPresTypes::FilterMap& filterMap,const MMPresTypes::eMetadataType& resultType,const uint32_t& metadataAttributes,const uint32_t& metadataCountType, const MMPresTypes::eSessionQueryTypes& queryType,const bool & autoUpdate,const bool& resetPrevious,const uint32_t& contentMask)
		:mQueryID(queryId)
		, mLevelId(0u)
		, mMediumId(mediumId)
		, mPlayURI("")
		, mBrowsePath("")
		, mFilterIDMap(filterIDMap)
		, mFilters(filterMap)
		, mResultType(resultType)
      , mBrowseResultType(resultType)
		, mMetadataAttributes(metadataAttributes)
		, mMetadataCountTypes(metadataCountType)
		, mQueryType(queryType)
		, mAutoUpdateEnabled(autoUpdate)
		, mResetPrevious(resetPrevious)
		, mContentMask(contentMask)
		, mNumResults(0u)
		{
			itemList.clear();
		}

		~SessionQueryData()
		{
			itemList.clear();
		}
	};

	Session(uint32_t sessionID); 

	uint32_t registerQuery(SessionQuery* query); 
	SessionQueryData* getSessionQueryData(uint32_t queryId);
	uint32_t getCurrentQueryId();
	void setCurrentQueryId(uint32_t);
	uint32_t getPreviousQueryId(uint32_t);
	void setCurrentLevelId(int32_t);
	int32_t getCurrentLevelId();
	void clearSessionCache();
	void removeQueryFromCache(uint32_t);
	void insertNewQueryToCache(uint32_t,Session::SessionQueryData*);
	void insertToQueryList(uint32_t id);
	bool isJumpValid(uint32_t queryIdToJump);
	void switchToPlayPlanQuery(std::list<uint32_t> playPlanQueryList);
	void insertDummyQueryId();
	void printQuerylist();
	bool isJumpValidBasedOnFilter(MMPresTypes::FilterIDMap filterID,uint32_t& id);
	std::list<uint32_t> getQueryList();
	void clearQueryList();
	void insertToPlayPlanDeviceList(uint64_t deviceId);
	void removeFromPlayPlanDeviceList(uint64_t deviceId);
	std::list<uint64_t>& getPlayPlanDeviceList();
private : 

	uint32_t mCurrentQueryID;
	uint32_t mAssignQueryId;
	std::list<uint32_t> mQueryList;
	uint32_t mSessionID;
	int32_t mCurrentLevelId;
    std::map<uint32_t, Session::SessionQueryData*> mQueryDataCache;
	std::list<uint64_t> mPlayPlanDeviceList;
};

#endif
