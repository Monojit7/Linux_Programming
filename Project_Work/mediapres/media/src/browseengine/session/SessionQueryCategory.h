
/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            SessionQueryCategory.h
* @ingroup         mediapresctrl
* @author          sriram srivatsan (sriram.srivatsan@harman.com)
* @brief           This contains the implementation of a metadata based based session query
*/

#ifndef _SESSION_QUERY_CATEGORY_
#define _SESSION_QUERY_CATEGORY_
#include <browseengine/session/SessionQuery.h>

class SessionQueryCategory :public SessionQuery
{
public:
   SessionQueryCategory(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask,const uint64_t& startIndex=0,const uint64_t& numItems=0);
   SessionQueryCategory(const uint32_t& sessionID,const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
   virtual ~SessionQueryCategory();

protected : 
   virtual void onRun();
   virtual bool validateQueryInput();
   virtual bool validateFilterInput();
   virtual bool validateResultType();
   virtual void determineCount();
   virtual void determineCountViaListChildren();
   virtual void categoryCountCallbackViaListChild(const MMPresTypes::eTitleContentType& titleContentType,const CommonAPI::CallStatus & stat, const    MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err);
   virtual void determineQueryItemList();
   virtual void determineCountViaGetAttribute();
   virtual void determineCategories();
   virtual void categoryCallback(const std::string& browsePath,const MMPresTypes::eTitleContentType& titleContentType, const CommonAPI::CallStatus & stat,  const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err);
   void categoryChildCountCallbackViagetAttribute(const MMPresTypes::eMetadataType& type, const CommonAPI::CallStatus& status, const MediaTypes::ResultMap& map, const BrowserTypes::BrowserError& browseError);
   void hmiIndexingDisabled(MMPresTypes::eMetadataType type, uint64_t childCount);
   void hmiIndexingEnabled(MMPresTypes::eMetadataType type , uint64_t childCount);
private:
   bool mCategoriesObtained;
   uint32_t mCategoryIndex;
   std::vector<uint32_t> zeroChildCountlist;
};

#endif
