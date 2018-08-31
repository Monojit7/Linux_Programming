#include <browseengine/session/SessionQueryFS_BTSA.h>
#include <device/DeviceBase.h>

SessionQueryFS_BTSA::SessionQueryFS_BTSA(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t & mediumID, const ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap & filterIDs, const ::v3::com::harman::mmpres::MMPresTypes::FilterMap & filters, const ::v3::com::harman::mmpres::MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const  ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask, const uint64_t& startIndex, const uint64_t& numItems)
	 :SessionQueryFS(sessionID,_client,std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), mediumID, filterIDs, filters, resultType, metadataAttributes, metadataCountTypes, type, resetPrevious, AutoUpdateEnabled, ContentMask, startIndex, numItems)
{
	Session* sess = mManager->getSession(mSessionId);
	if (sess != nullptr)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS_BTSA::SessionQueryFS_BTSA:initialize queryId=%lu in cache", mQueryId);
		Session::SessionQueryData* data = new Session::SessionQueryData(mQueryId, mMediumId, mFilterIDMap, mFilters, mResultType, mMetadataAttributes, mMetadataCountTypes, mQueryType, mAutoUpdateEnabled, mResetPrevious, mContentMask);
		sess->insertNewQueryToCache(mQueryId, data);
	}
}

SessionQueryFS_BTSA::SessionQueryFS_BTSA(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex, const uint64_t& numItems)
	:SessionQueryFS(sessionID, queryID, _client, std::move(reply), std::move(gotoPreviousReply), std::move(jumpToQueryReply), queryCacheData, startIndex, numItems)
{

}

SessionQueryFS_BTSA::~SessionQueryFS_BTSA()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< SessionQueryFS_BTSA::~SessionQueryFS_BTSA");
}