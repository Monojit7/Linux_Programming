#ifndef _DEVICE_BASE_H_
#define _DEVICE_BASE_H_

#include <core.h>
#include<detectionengine/MediaDetectionEngine.h>
#include<detectionengine/DeviceManagerHelper.h>
#include<browseengine/MediaBrowseEngine.h>
#include<playengine/MediaPlaybackEngine.h>
#include<playengine/PlayHelper.h>
#include<browseengine/BrowseHelper.h>
#include<audiomanagerinterface/audiomanagerinterface.h>
#include<statemachine/sourceswitch.h>
#include<config/MediaPresSettings.h>
#include<browseengine/session/Session.h>
#include<playengine/RemoteCDplaybackengine.h>

using namespace v1::com::harman::media;
using namespace v3::com::harman::mmpres;

#define PLAY_POSITION_DIVISOR 1000000
#define MINUS_1 int(-1)
#define FFWD_RATE_THRESHOLD 1
#define PLAYING_SPEED 1
#define PAUSED_SPEED 0
#define FRWD_RATE_THRESHOLD -1
#define SEC_MILLISEC 1000
#define FFWD_RATE_THRESHOLD_AP 3
#define TIMELIMIT_FOR_PREVIOUSUSAGE 3
#define STEPCOUNT_ONE 1
#define REMOTECD_DEVICE_ID 65537

class MediaDetectionEngine;
class MediaPlaybackEngine;
class AudioManagerInterface;
class SourceSwitch;
class RemoteCDPlaybackEngine;

class DeviceBase : public RequestBase
{
public:
	virtual ~DeviceBase();

	virtual void mediumInfoUpdate(const v1::com::harman::media::BaseType::tMediumInfo& MediumInfo);

	virtual void resume();
	virtual void pause();
	virtual void stop();
	virtual void next(uint64_t step);
	virtual void previous(uint64_t step);
   virtual void seek(const int64_t pos);
	virtual void seekToTime(uint64_t pos);
	virtual void setRepeat(MMPresTypes::eRepeatMode);
	virtual void setShuffle(MMPresTypes::eShuffleMode);
	virtual MMPresTypes::eCallResult fastForward();
	virtual MMPresTypes::eCallResult fastReverse();
	virtual std::string getNowPlayingAlbumArtPath(uint8_t);
   virtual void playDevice(const MMPresTypes::eMetadataType objType ,std::string="");
	virtual void dequeAll();

	virtual void subscribeToPlayerEvents();
	virtual void unSubscribeToPlayerEvents();

	virtual void SessionQueryCreateNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap & filterIDs, const MMPresTypes::FilterMap & filters, const MMPresTypes::eMetadataType & resultType, const uint32_t & metadataAttributes, const  uint32_t & metadataCountTypes, const MMPresTypes::eSessionQueryTypes & type, const  bool & resetPrevious, const bool & AutoUpdateEnabled, const uint32_t & ContentMask, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
	virtual void SessionQueryCreateExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t && reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t&& gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t&& jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryNowPlayingNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryNowPlayingExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);

	virtual MMPresTypes::tTrackState getTrackState();
	uint64_t getTotalDuration();

	uint64_t getDeviceId();
	uint64_t getCurrentTime();
	MMPresTypes::eTrackSeekSpeed getTrackSeekSpeed();
	virtual MMPresTypes::ePlayState getPlayState();
	MMPresTypes::eRepeatMode getRepeatMode();
	MMPresTypes::tNowPlayingInfo getNowPlayingInfo();
	double getCurrentRate();

	std::string getBrowsePath();

	bool getCanSeek();
	bool getCanNext();
	bool getCanPrevious();
	bool getCanPause();
	bool getCanPlay();
	bool getResetMediaIndex();
	void setResetMediaIndex(bool);
	void setMasterMediaReset(bool);
   virtual const void onRemotePlayerChanged();
	void cancelTimers();
	void setPlayState(const MMPresTypes::ePlayState&);
	virtual int32_t getBrowseHistoryEnumeration();
	virtual void setBrowseHistoryEnumeration(int32_t value);

	MMPresTypes::eDeviceType getDevType();
	std::string getDeviceName();
	bool getReadyToPlay();
	BaseType::tMediumInfo getMediumInfo();
	MMPresTypes::tAdditionalDeviceInfo getAdditionalDeviceInfo();
	void triggerMediaErrorToDevice(uint8_t slotId, MMPresTypes::eMediaError error);

	void getPlayQueue(uint32_t sessionID,uint64_t start,uint64_t num,uint32_t queryId, MediaBrowserStub::setSessionQueryReply_t&& reply);
	void getCategoryCountMap();
	void getContentInfoMap();

	void setPlayPlanQueryId(uint32_t id);
	uint32_t getPlayPlanQueryId();
	void contructPlayPlan(std::list<uint32_t> queryList);
	std::list<uint32_t> getPlayPlanQueryList();
   uint32_t getPreviousPlayPlanQueryId(const uint32_t id);
	void categoryChildCountCallback(const MMPresTypes::eMetadataType& type, const CommonAPI::CallStatus& status,const MediaTypes::ResultMap& map,const BrowserTypes::BrowserError& browseError);
	void categoryChildCountCallbackForVideo(const MMPresTypes::eMetadataType& type, const CommonAPI::CallStatus& status, const MediaTypes::ResultMap& map, const BrowserTypes::BrowserError& browseError);
	void categoryChildCountCallbackForImage(const MMPresTypes::eMetadataType& type, const CommonAPI::CallStatus& status, const MediaTypes::ResultMap& map, const BrowserTypes::BrowserError& browseError);
	void clearPlayPlan();

	virtual void onRepeatChanged(const PlayerTypes::RepeatStatus&);
	virtual void subscribeToRepeatStatusUpdate(bool subscribe);
	virtual bool getVideoPlayStatus();
	std::string getCurrentPlayingUri();
	std::string getRootlevelPlaybackUri(const MMPresTypes::eTitleContentType& contentType,const MMPresTypes::eMetadataType& metadataType);
   void setBreadCrumbForSetSession(std::string);
   std::string getBreadCrumbForSetSession();
   void setVolumeAttribute(const double&);
   void clearMetadata();
   void setRestrictedMode();
   void restrictedModeCallback(const CommonAPI::CallStatus& status, const PlayerTypes::RestrictedModeStatus& value);
   void getNowPlayingType(std::string & type);
   std::string mAlbumArt;
   MMPresTypes::eDeviceType getDeviceType();
   bool getNowPlayingListSupported();
   bool getTrackListSupported();
   void setTrackListSupported(bool);
   void setNowPlayingTotalTrack(uint64_t);
   uint64_t getNowPlayingTotalTrack();
   MMPresTypes::tPlaybackSpeed getSpeedInfoObj();
   void setSlotForAudiobookPodcast(int8_t);
   int8_t getSlotForAudiobookPodcast();
   void setMetadatType(MMPresTypes::eMetadataType);
   MMPresTypes::eMetadataType getMetadataType();
   //MMPresTypes::eMetadataType getContainerType(std::string str);
   virtual void gotoTrack(const uint64_t trackPosition);
   bool getAudiobookPodcast();
   void setAudiobookPodcast(bool);
   void getSpeedPodCastCallback(const CommonAPI::CallStatus& status, const double& speed, const ::v1::com::harman::media::BaseType::tBaseError& error);
   void getSpeedAudioBookCallback(const CommonAPI::CallStatus& status, const double& speed, const ::v1::com::harman::media::BaseType::tBaseError& error);

   void setSmartSearchAlphaJumpCandidateRequiredToBeSend(bool value);
   bool getSmartSearchAlphaJumpCandidateRequiredToBeSend();

   void setIsDeviceEmpty(bool value);
   bool getIsDeviceEmpty();
   virtual void getRemotePlayerCapability();
   virtual void sendIAP2KeyEvent(::v3::com::harman::mmpres::MMPresTypes::IAP2ButtonType buttonType, bool pressed);
   //iTunes related functions`
   virtual void promote(uint64_t fileid);
   virtual void demote(uint64_t fileid);
   virtual void addToWishList(uint64_t fileid);
   virtual bool getPromoteStatus();
   virtual void setPromoteStatus(bool value);
   virtual bool getDemoteStatus();
   virtual void setDemoteStatus(bool value);
   virtual bool getAddToWishListStatus();
   virtual void setAddToWishListStatus(bool value);

   bool mSessionEnd = false;

protected:
	DeviceBase(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo);
    DeviceBase();

	void initRateToSeekSpeedConversionMap();
	void initAlbumArt();
	void setAlbumArt(uint8_t, std::string);

	virtual void onRun();
	virtual void onCancelled();
	virtual void processInsertion();
	virtual void processReadyToPlay();
	virtual void processFirstPass();
	virtual void processSecondPass();
	virtual void processActive();
	virtual void getDeviceFirmWareInfo();
	virtual void getDeviceHardWareInfo();
	virtual void getDeviceItemsInfo();
	//virtual void getRemotePlayerCapability();
	virtual void determineAudio();
	virtual void determineVideo();
	virtual void determineImage();
	virtual void getDeviceFirmWareInfoCallback(const CommonAPI::CallStatus&, const ::v1::com::harman::media::BaseType::tFirmwareInfo&, const ::v1::com::harman::media::BaseType::tBaseError&);
	virtual void getDeviceHardWareInfoCallback(const CommonAPI::CallStatus&, const ::v1::com::harman::media::BaseType::tHardwareInfo&, const ::v1::com::harman::media::BaseType::tBaseError&);
	virtual void getDeviceItemsInfoCallback(const CommonAPI::CallStatus&, const ::v1::com::harman::media::BaseType::tItemsInfo&, const ::v1::com::harman::media::BaseType::tBaseError&);
	virtual void getRemotePlayerCapabilityCallback(const CommonAPI::CallStatus&, const ::v1::com::harman::media::BaseType::tRemotePlayerCapability&, const ::v1::com::harman::media::BaseType::tBaseError&);

	//virtual void onPlaybackStatusChanged(const PlayerTypes::PlaybackStatus&);
	virtual void onCurrentTrackChanged(const uint64_t&);
	virtual void onTotalTracksChanged(const uint64_t&);
	virtual void onPlayPositionChanged(const uint64_t&);
	virtual void onPlayDurationChanged(const uint64_t&);
	virtual void onRateChanged(const double&);
	virtual void onMetadataChanged(const MediaTypes::ResultMap& metadataMap);
	virtual void onShuffleChanged(const PlayerTypes::ShuffleStatus&);
	virtual void onCanGoNextChanged(const bool&);
	virtual void onCanGoPreviousChanged(const bool&);
	virtual void onCanPauseChanged(const bool&);
	virtual void onCanPlayChanged(const bool&);
	virtual void onCanSeekChanged(const bool&);
	virtual void onSessionEndChanged(const bool&);
	virtual void onPlayQueueChanged();
	virtual void onPlayerTypeChanged(const PlayerTypes::PlayerType&);
	virtual void onCanGetPlayQueueAttributeChanged(const bool&);

	virtual void resumeCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
	virtual void pauseCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
	virtual void nextCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
	virtual void nextChapterCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
	virtual void previousCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
	virtual void previousChapterCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
	virtual void seekToTimeCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
   virtual void seekCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
	virtual void setRepeatCallback(const CommonAPI::CallStatus& status, const PlayerTypes::RepeatStatus& mode);
	virtual void setShuffleCallback(const CommonAPI::CallStatus& status, const PlayerTypes::ShuffleStatus& mode);
	virtual void fastForwardCallback(const CommonAPI::CallStatus& status, const double& rate);
	virtual void fastReverseCallback(const CommonAPI::CallStatus& status, const double& rate);
	virtual void playDeviceCallback(const CommonAPI::CallStatus&, const PlayerTypes::PlayerError&);
	virtual void dequeAllCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
	virtual void stopCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
    virtual void openuriPlayAllCallback(const CommonAPI::CallStatus& status, const PlayerTypes::PlayerError& error);
	virtual void cancelSeekTimer();
	virtual void setSeekTimer();
    virtual void updateAvailabilityToAudioManager(bool available);
    virtual void volumeCallback(const CommonAPI::CallStatus& status,const double& vol); 

	void deviceContainerUpdate(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err);
	void deviceContainerUpdateForVideo(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err);
	void deviceContainerUpdateForImage(const CommonAPI::CallStatus & stat, const MediaTypes::ResultMapList& map, const BrowserTypes::BrowserError& err);
	virtual uint32_t getDeviceCaps() = 0;

	virtual void sendItemIndexTobeHighlighted(const uint64_t&);

	virtual RequestBase* SessionQueryFSNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryFSExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryMetadataNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryMetadataExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryHighestRatedNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryHighestRatedExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryRecentlyPlayedNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryRecentlyPlayedExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryCategoryNew(const uint32_t& sessionID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const  uint64_t& mediumID, const MMPresTypes::FilterIDMap& filterIDs, const MMPresTypes::FilterMap& filters, const MMPresTypes::eMetadataType& resultType, const uint32_t&  metadataAttributes, const  uint32_t& metadataCountTypes, const  MMPresTypes::eSessionQueryTypes& type, const  bool& resetPrevious, const bool& AutoUpdateEnabled, const uint32_t& ContentMask, const uint64_t & startIndex = 0, const uint64_t& numItems = 0);
	virtual RequestBase* SessionQueryCategoryExisting(const uint32_t& sessionID, const uint32_t& queryID, const std::shared_ptr<CommonAPI::ClientId> _client, MediaBrowserStub::setSessionQueryReply_t reply, MediaBrowserStub::gotoPreviousSessionQueryReply_t gotoPreviousReply, MediaBrowserStub::jumpToSessionQueryReply_t jumpToQueryReply, const Session::SessionQueryData& queryCacheData, const uint64_t& startIndex = 0, const uint64_t& numItems = 0);

	BaseType::tMediumInfo mMediumInfo;
	BaseType::tFirmwareInfo mFirmWareInfo;
	BaseType::tHardwareInfo mHardWareInfo;
	BaseType::tItemsInfo mItemsInfo;
   MMPresTypes::tPlaybackSpeed mSpeedInfo;

	MMPresTypes::ePlayState mPlayState;
	uint64_t mCurrentTime;
	uint64_t mTotalDuration;
	double mCurrentRate;
	MMPresTypes::tTrackState mTrackState;
	MMPresTypes::tNowPlayingInfo mNowPlayingInfo;
	MMPresTypes::eRepeatMode mRepeatMode;
	MMPresTypes::eShuffleMode mShuffleMode = MMPresTypes::eShuffleMode::SHUFFLE_OFF;
	MMPresTypes::eTrackSeekSpeed mTrackSeekSpeed;
	MMPresTypes::eArtWorkAvailabilityStatus mArtworkStatus;
	MMPresTypes::tAlbumArtInformation mAlbumArtInformation;
	MMPresTypes::tAdditionalDeviceInfo mAdditionalDeviceInfo;

	uint32_t  mBrowseCaps;
	MMPresTypes::MetadataCountMap mAudioCategoriesCount,mVideoCategoriesCount,mImageCategoriesCount;;
	MMPresTypes::tBTStatus mBTStatus;
	MMPresTypes::ContentInfoMap mContentInfoMap;

	bool mCanSeek;
	bool mCanPlay;
	bool mCanPause;
	bool mCanGoNext;
	bool mCanGoPrevious;

	bool mReadyToPlay;
	bool mVideoPlayStatus;
	bool mMultipleChapterAvailable;
   bool mbShuffle = false;

   uint32_t mPlayDurationToken = 0u;
   uint32_t mPlayPosToken = 0u;
   uint32_t mTrackToken = 0u;
   uint32_t mTotalTrackToken = 0u;

   BaseType::tRemotePlayerCapability mRemotePlayerCapability;
   FacadeHelper<AudioManagerInterface> mAudioInterface;
   FacadeHelper<MediaPlaybackEngine> mPlayEngine;
   FacadeHelper<MediaDetectionEngine> mDetect;
   FacadeHelper<SourceSwitch> mSourceSwitch;
   FacadeHelper<MediaPresSettings> mSettings;
   FacadeHelper<MediaBrowseEngine> mBrowse;

private:
   void subscribeToMetadataUpdate(bool subscribe);
//   void subscribeToPlaybackStatusUpdate(bool subscribe);
	virtual void subscribeToCurrentTrackUpdate(bool subscribe);
	virtual void subscribeToTotalTrackUpdate(bool subscribe);
	virtual void subscribeToPlayPositionUpdate(bool subscribe);
	virtual void subscribeToPlayDurationUpdate(bool subscribe);
	void subscribeToShuffleStatusUpdate(bool subscribe);
	void subscribeToRateUpdate(bool subscribe);
	void subscribeToCanGoNext(bool subscribe);
	void subscribeToCanGoPrevious(bool subscribe);
	void subscribeToCanPause(bool subscribe);
	void subscribeToCanPlay(bool subscribe);
	void subscribeToCanSeek(bool subscribe);
	void subscribeToSessionEnd(bool subscribe);
	void checkUnknownAndReplace(std::string &str1, std::string &str2);
	//void susbcribeToPlayQueueChanged(bool subscribe);
	
	void updateSourceInfoToAudioPres();
	void updateSyncState(MMPresTypes::eSyncState state);
   
	static std::unordered_map<v1::com::harman::media::BaseType::tMediumType, MMPresTypes::eDeviceType> mDevTypeMapping;
	//uint32_t mPlaybackStatusToken;
	uint32_t mMetadataToken;
	uint32_t mRepeatToken;
	uint32_t mShuffleToken;
	uint32_t mRateToken;
	uint32_t mGoNextToken;
	uint32_t mGoPreviousToken;
	uint32_t mCanPauseToken;
	uint32_t mCanPlayToken;
	uint32_t mCanSeekToken;
	uint32_t mSessionEndToken;
	uint32_t mPlayQueueChangedToken;
    uint64_t mNowPlayingTotalTrack;
    int8_t mSlotIdForPodcastAudiobook;
    MMPresTypes::eMetadataType mTypeForPodcastAudiobook;

	std::map<std::pair<double, double>, MMPresTypes::eTrackSeekSpeed> rateToSeekSpeedConversionMap;
	std::map<MMPresTypes::eTrackSeekSpeed, double> seekSpeedToRateConversionMap;

	// map which contains a mapping b/n image number and corresponding path for the album art of the current nowplaying track.
	std::map<uint8_t, std::string> mAlbumArtPathMap;

	bool mSubscribed;
	int32_t mBrowseHistoryEnumValue;
	bool mInserted;
	bool mResetMediaIndex;

	//std::unordered_map<uint32_t, uint32_t> mNowPlayingSessionMap;
	uint32_t mPlayPlanQueryId;
	std::list<uint32_t> mPlayPlanQueryList;
	std::string mCurrentPlayingUri;
    std::string musicUri;
    std::string videoUri;
    std::string mBreadCrumbForNowPlaying;
   std::string mNowPlayingType;
	std::unordered_map<MMPresTypes::eTitleContentType,std::unordered_map<MMPresTypes::eMetadataType,std::string>> mRootlevelPlaybackUriMap;
	void subscribeToPlayerType(bool subscribe);
	uint32_t mPlayerTypeToken;
	void subscribeToCanGetPlayQueue(bool subscribe);
	uint32_t mCanGetPlayQueueToken;
	bool mNowPlayingListSupported;
	bool mSmartSearchAlphaJumpCandidateRequiredToBeSend;
	bool mIsDeviceEmpty;
	bool mIsTrackListSupported;
	bool bAudioPodcastVal;
};
#endif
