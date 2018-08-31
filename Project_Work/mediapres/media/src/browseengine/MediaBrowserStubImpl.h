#ifndef V1_COM_HARMAN_MMPRES_MEDIABROWSERSTUBIMPL_HPP
#define V1_COM_HARMAN_MMPRES_MEDIABROWSERSTUBIMPL_HPP

#include <CommonAPI/CommonAPI.hpp>
#include <browseengine/session/SessionManager.h>
#include<device/DeviceBase.h>
#include<config/MediaPresSettings.h>
#include <queue>
#include <playengine/RemoteCDplaybackengine.h>

#define FOLDERID 0

namespace v3 {
namespace com {
namespace harman {
namespace mmpres {

class MediaBrowserStubImpl : public v3::com::harman::mmpres::MediaBrowserStubDefault
{
private :
    FacadeHelper<MediaDetectionEngine> mDetect;
    FacadeHelper<SessionManager> mManager;
    FacadeHelper<MediaBrowseEngine> mBrowser;
    FacadeHelper<MediaPlaybackEngine> mPlayer;
    FacadeHelper<AudioManagerInterface> mAudioInterface;
    FacadeHelper<RemoteCDPlaybackEngine> mRemoteCDPlayBackEngine;
    FacadeHelper<SourceSwitch> mSourceSwitch;
    FacadeHelper<MediaPresSettings> mSettings;
    std::vector<std::string> mSearchCandidatesAttributeValue;
    MMPresTypes::HighlightItemIndexMap mItemIndexTobeHighlightedAttribute;

public:
   MediaBrowserStubImpl();
   virtual ~MediaBrowserStubImpl();
   virtual void openSession(const std::shared_ptr<CommonAPI::ClientId> _client, openSessionReply_t);
   virtual void closeSession(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, closeSessionReply_t);
   virtual void cancelSession(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, cancelSessionReply_t);
   virtual void setSessionQuery(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, uint8_t slotId, ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap filterIDs, ::v3::com::harman::mmpres::MMPresTypes::FilterMap filters, ::v3::com::harman::mmpres::MMPresTypes::eMetadataType resultType, uint32_t metadataAttributes, uint32_t metadataCountTypes, ::v3::com::harman::mmpres::MMPresTypes::eSessionQueryTypes type, bool resetPrevious, bool AutoUpdateEnabled, uint32_t ContentMask, setSessionQueryReply_t);
   virtual void setSessionQueryWindow(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, uint32_t sessionQueryID, uint64_t startIndex, uint64_t numItems, setSessionQueryWindowReply_t);
   virtual void gotoPreviousSessionQuery(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, gotoPreviousSessionQueryReply_t);
   virtual void jumpToSessionQuery(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, uint32_t sessionQueryID, jumpToSessionQueryReply_t);
   virtual void playSession(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap filterIDs, playSessionReply_t);
   virtual void gotoTrack(const std::shared_ptr<CommonAPI::ClientId> _client, uint64_t _trackPosition, gotoTrackReply_t reply);
   virtual void gotoNowplayingLevel(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, uint8_t slotId, gotoNowplayingLevelReply_t);
   virtual void fireSessionQueryWindowUpdateSelective(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::eQueryStatus &_status, const uint64_t &_startIndex, const uint64_t &_numItems, const ::v3::com::harman::mmpres::MMPresTypes::MetadataItemList &_itemList, const bool &_windowContainsCurrentlyPlayingTrack, const uint64_t &_itemIndexTobeHighlighted);
   virtual void fireThumbnailListUpdateSelective(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const uint64_t &_startIndex, const uint64_t &_numItems, const std::vector<std::string> &_thumbnaiList);
   virtual void fireSessionQueryAlphaJumpUpdateSelective(const uint32_t &_sessionID, const uint32_t &_sessionQueryID, const ::v3::com::harman::mmpres::MMPresTypes::arAlphaIndex &_ajList);	
   virtual void openuriCallback(const uint64_t mediumID, const uint32_t sessionID,const uint32_t queryId,const CommonAPI::CallStatus&, const ::v1::org::genivi::mediamanager::PlayerTypes::PlayerError&);
   virtual void enqueueUriCallback( const CommonAPI::CallStatus&,const ::v1::org::genivi::mediamanager::PlayerTypes::PlayerError&);
   virtual void setSearchCandidatesAttribute(std::vector<std::string> _value);
   virtual const std::vector<std::string>& getSearchCandidatesAttribute();
   virtual void getElevatorBarHelperLetterTimeout(const std::shared_ptr<CommonAPI::ClientId> _client, getElevatorBarHelperLetterTimeoutReply_t);
   virtual void setItemIndexTobeHighlightedAttribute(MMPresTypes::HighlightItemIndexMap _value);
   virtual const MMPresTypes::HighlightItemIndexMap& getItemIndexTobeHighlightedAttribute();

protected:
   void checkforclearingSessionCache(uint32_t sessionID, uint64_t mediumID, ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap map, MMPresTypes::eSessionQueryTypes type, ::v3::com::harman::mmpres::MMPresTypes::FilterMap filterMap);
   void checkforclearingSessionCacheWithBrowseEnumeration(uint32_t sessionID, uint64_t mediumID, ::v3::com::harman::mmpres::MMPresTypes::FilterIDMap map, MMPresTypes::eSessionQueryTypes type, ::v3::com::harman::mmpres::MMPresTypes::FilterMap filterMap);
   bool checkForPersistedPlayPlan(uint32_t sessionID);
   void gotoPreviousSessionQueryWithBrowseEnumeration(const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t sessionID, gotoPreviousSessionQueryReply_t&&);
   bool getPlayAllInfoForFS(const uint32_t sessionID, playSessionReply_t reply, bool& isReplySent);
 };

} // namespace mmpres
} // namespace harman
} // namespace com
} // namespace v3

#endif //V1_COM_HARMAN_MMPRES_MEDIABROWSERSTUBIMPL_HPP
