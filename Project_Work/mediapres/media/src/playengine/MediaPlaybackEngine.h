/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            MediaPlaybackEngine.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           The heart of the MediaPlaybackEngine.
*/

#ifndef _MEDIA_PLAYBACK_ENGINE_H
#define _MEDIA_PLAYBACK_ENGINE_H
#include <core.h>
#include<config/MediaPresSettings.h>
#include<statemachine/sourceswitch.h>
#include<detectionengine/MediaDetectionEngine.h>
#include<audiomanagerinterface/audiomanagerinterface.h>

//#include<me>
//#include "spot/harman_//KPIMARKER.h"
using namespace v1::org::genivi::mediamanager;
using namespace v3::com::harman::mmpres;
using namespace v0::org::harman::ford;



#define ARG_LIST_PLAYBACK_RESPONSE <CommonAPI::CallStatus ,PlayerTypes::PlayerError >
#define ARG_LIST_REPEAT_RESPONSE <CommonAPI::CallStatus ,PlayerTypes::RepeatStatus >
#define ARG_LIST_SHUFFLE_RESPONSE <CommonAPI::CallStatus ,PlayerTypes::ShuffleStatus >
#define ARG_LIST_SPEED_RESPONSE <CommonAPI::CallStatus ,double >
#define ARG_LIST_VOLUME_RESPONSE <CommonAPI::CallStatus ,double >
#define ARG_LIST_SETPLAYBACKSPEED_RESPONSE <CommonAPI::CallStatus,BaseType::tBaseError>
#define ARG_LIST_GETPLAYBACKSPEED_RESPONSE <CommonAPI::CallStatus,double,BaseType::tBaseError>
#define ARG_LIST_SUBTITLE_RESPONSE <CommonAPI::CallStatus ,PlayerTypes::SubtitleStatus >
#define ARG_LIST_UPSCALE_RESPONSE <CommonAPI::CallStatus ,PlayerTypes::AspectModeStatus >
#define ARG_LIST_AUDIOCHANNEL_RESPONSE <CommonAPI::CallStatus ,PlayerTypes::AudioChannelStatus >
#define ARG_LIST_PLAYSTATUS_CALLBACK <PlayerTypes::PlaybackStatus>
#define ARG_LIST_CURRENTTRACK_CALLBACK <uint64_t>
#define ARG_LIST_PLAYPOSITION_CALLBACK <uint64_t>
#define ARG_LIST_PLAYDURATION_CALLBACK <uint64_t>
#define ARG_LIST_METADATA_CALLBACK <MediaTypes::ResultMap>
#define ARG_LIST_REPEATSTATUS_CALLBACK <PlayerTypes::RepeatStatus>
#define ARG_LIST_SHUFFLESTATUS_CALLBACK <PlayerTypes::ShuffleStatus>
#define ARG_LIST_RATE_CALLBACK <double>
#define ARG_LIST_IAP2KEY_CALLBACK <CommonAPI::CallStatus>
#define ARG_LIST_GETCURRENTPLAYQUEUE_CALLBACK <CommonAPI::CallStatus,MediaTypes::ResultMapList,PlayerTypes::PlayerError>
#define ARG_LIST_SETDISPLAYWINDOW_CALLBACK <CommonAPI::CallStatus, PlayerTypes::PlayerError>
#define ARG_LIST_RESTRICTEDMODE_RESPONSE <CommonAPI::CallStatus ,PlayerTypes::RestrictedModeStatus >
#define ARG_LIST_LOCALENAME_RESPONSE <CommonAPI::CallStatus ,std::string >

class MediaDetectionEngine;
class MediaPlaybackEngine
{
public : 

	void resume(uint32_t ClientID, PlayerProxyBase::PlayAsyncCallback callback);
	void pause(uint32_t ClientID, PlayerProxyBase::PauseAsyncCallback callback);
	void stop(uint32_t ClientID, PlayerProxyBase::StopAsyncCallback callback);
	void next(uint32_t ClientID, PlayerProxyBase::NextAsyncCallback callback);
	void previous(uint32_t ClientID, PlayerProxyBase::PreviousAsyncCallback callback);
	void nextChapter(uint32_t ClientID, PlayerProxyBase::NextChapterAsyncCallback callback);
	void previousChapter(uint32_t ClientID, PlayerProxyBase::PreviousChapterAsyncCallback callback);
	void seekToTime(uint32_t ClientID,uint64_t pos, PlayerProxyBase::SetPositionAsyncCallback callback);
   void seek(uint32_t ClientID,const int64_t pos, PlayerProxyBase::SeekAsyncCallback callback);
	void setRepeat(uint32_t ClientID, PlayerTypes::RepeatStatus,std::function<void(const CommonAPI::CallStatus&, const PlayerTypes::RepeatStatus&)>);
	void setShuffle(uint32_t ClientID, PlayerTypes::ShuffleStatus, std::function<void(const CommonAPI::CallStatus&, const PlayerTypes::ShuffleStatus&)>);
	void fastForward(uint32_t ClientID, double, std::function<void(const CommonAPI::CallStatus& status, const double& rate)>);
	void fastReverse(uint32_t ClientID, double, std::function<void(const CommonAPI::CallStatus& status, const double& rate)>);
	void playDevice(uint32_t ClientID, std::string, PlayerProxyBase::OpenUriAsyncCallback callback);
	void enqueueUri(uint32_t ClientID, std::string, PlayerProxyBase::EnqueueUriAsyncCallback callback);
	void dequeAll(uint32_t ClientID, PlayerProxyBase::DequeueAllAsyncCallback callback);
	void dequeIndex(uint32_t ClientID,uint64_t, PlayerProxyBase::DequeueIndexAsyncCallback callback);
	void getCurrentPlayQueue(uint32_t ClientID, PlayerProxyBase::GetCurrentPlayQueueAsyncCallback callback);
	void getCurrentPlayQueueEx(uint32_t ClientID,uint64_t,uint64_t, PlayerProxyBase::GetCurrentPlayQueueExAsyncCallback callback);
	void setPlaybackSpeed(uint32_t ClientID, ::v1::com::harman::media::BaseType::tAudioObjectType _type, double _speed, MediaSettingProxyBase::SetPlaybackSpeedAsyncCallback callback);
	void getPlaybackSpeed(uint32_t ClientID, const ::v1::com::harman::media::BaseType::tAudioObjectType _type, MediaSettingProxyBase::GetPlaybackSpeedAsyncCallback callback);
	void setSubtitle(uint32_t ClientID, PlayerTypes::SubtitleStatus, std::function<void(const CommonAPI::CallStatus&, const PlayerTypes::SubtitleStatus&)>);
	void setAudioChannel(uint32_t ClientID, PlayerTypes::AudioChannelStatus, std::function<void(const CommonAPI::CallStatus&, const PlayerTypes::AudioChannelStatus&)>);
	void setDisplayWindow(uint32_t ClientID, bool _fullscreen, MMPresTypes::tScreen screeenDimension, std::function<void(const CommonAPI::CallStatus&, const PlayerTypes::AspectModeStatus&)> callback);
   void setVolume(uint32_t ClientID, double, std::function<void(const CommonAPI::CallStatus& status, const double& vol)>);
   void setRate(uint32_t ClientID, double, std::function<void(const CommonAPI::CallStatus& status, const double& rate)>);
   void sendIAP2KeyEvent(uint32_t ClientID, ::v1::org::genivi::mediamanager::PlayerTypes::IAP2ButtonType buttonType, bool pressed, PlayerProxyBase::SendIAP2KeyEventAsyncCallback callback);
   void promote(uint32_t ClientID, std::string &_uri, PlayerProxyBase::PromoteAsyncCallback callback);
   void demote(uint32_t ClientID, std::string &_uri, PlayerProxyBase::DemoteAsyncCallback callback);
   void addToWishList(uint32_t ClientID, std::string &_uri, PlayerProxyBase::AddToWishListAsyncCallback callback);


	std::function<void(const MMPresTypes::tTrackListSupport&)>& getTracklistSupportStubCallBack();
   std::function<void(const bool&)>& getPromoteStatusStubCallBack();
   std::function<void(const bool&)>& getDemoteStatusStubCallBack();
   std::function<void(const bool&)>& getAddToWishListStatusStubCallBack();
   std::function<void(const MMPresTypes::ePlayState&)>& getPlayStateStubCallback();
	std::function<void(const uint64_t&)>& getCurrentTimeStubCallback();
	std::function<void(const uint64_t&)>& getTotalDurationStubCallback();
   std::function<void(const uint32_t&)>& getRemotePlayerChangedStubCallback();
	std::function<void(const MMPresTypes::tTrackState&)>& getTrackStateStubCallback();
	std::function<void(const MMPresTypes::tNowPlayingInfo&)>& getNowPlayingDataStubCallback();
	std::function<void(const MMPresTypes::eRepeatMode&)>& getRepeatModeStubCallback();
	std::function<void(const MMPresTypes::eShuffleMode&)>& getShuffleModeStubCallback();
	std::function<void(const MMPresTypes::eTrackSeekSpeed&)>& getTrackSeekSpeedStubCallback();
	std::function<void(const MMPresTypes::eArtWorkAvailabilityStatus&)>& getArtworkStatusCallback();
	std::function<void(const MMPresTypes::tRemotePlayerCapability&)>& getRemotePlayerCapabilityStubCallback();
	std::function<void(const MMPresTypes::SyncMap&)>& getSyncStatusStubCallback();
	std::function<void(const MMPresTypes::MediaErrorMap&)>& getMediaErrorStubCallback();
	std::function<void(const bool&)>& getVideoPlayStatusCallback();
	std::function<void(const MMPresTypes::eSubtitleStatus&)>& getSubtitleStatusStubCallback();
	std::function<void(const bool&)>& getUpScaleStatusStubCallback();
   //std::function<void(const bool&)>& getActiveDeviceBTLinkLossStubCallBack();
	std::function<void(const MMPresTypes::eAudioChannelStatus&)>& getAudioChannelStatusStubCallback();
   std::function<void(const MMPresTypes::tPlaybackSpeed&)>& getSpeedInfoStubCallback();
	void setTracklistSupportStubCallBack(std::function<void(const MMPresTypes::tTrackListSupport&)>);
   void setPromoteStatusStubCallBack(std::function<void(const bool&)>);
   void setDemoteStatusStubCallBack(std::function<void(const bool&)>);
   void setAddToWishListStatusStubCallBack(std::function<void(const bool&)>);
	void setPlaystateStubCallBack(std::function<void(const MMPresTypes::ePlayState&)>);
	void setTrackStateStubCallBack(std::function<void(const MMPresTypes::tTrackState&)>);
	void setNowPlayingDataStubCallBack(std::function<void(const MMPresTypes::tNowPlayingInfo&)>);
	void setCurrentPlayTimeStubCallBack(std::function<void(const uint64_t&)>);
	void setTotalPlayDurationStubCallBack(std::function<void(const uint64_t&)>);
   void setRemotePlayerChangedStubCallBack(std::function<void(const uint32_t&)>);
	void setRepeatModeStubCallBack(std::function<void(const MMPresTypes::eRepeatMode&)>);
	void setShuffleModeStubCallBack(std::function<void(const MMPresTypes::eShuffleMode&)>);
	void setTrackSeekSpeedStubCallBack(std::function<void(const MMPresTypes::eTrackSeekSpeed&)>);
	void setArtworkStatusStubCallback(std::function<void(const MMPresTypes::eArtWorkAvailabilityStatus&)>);
	void setRemotePlayerCapabilityStubCallback(std::function<void(const MMPresTypes::tRemotePlayerCapability&)>);
	void setSyncStatusStubCallback(std::function<void(const MMPresTypes::SyncMap&)>);
	void setMediaErrorStubCallback(std::function<void(const MMPresTypes::MediaErrorMap&)>);
	void setVideoPlayStatusCallBack(std::function<void(const bool&)>);
	void setSubtitleStatusStubCallBack(std::function<void(const MMPresTypes::eSubtitleStatus&)>);
	void setUpScaleStatusStubCallBack (std::function<void(const bool&)>);
	void setAudioChannelStatusStubCallBack(std::function<void(const MMPresTypes::eAudioChannelStatus&)>);
   void setActiveDeviceBTLinkLossCallBack(std::function<void(const bool&)>);
	void deviceErrorEvent(const tdeviceError& deviceError);
	void masterResetEvent(const bool& factoryResetStatus);
	void factoryResetEvent(const bool& factoryResetStatus);
	void resetToFactory(MediaSettingProxyBase::ResetToFactoryAsyncCallback callback);
	void resetToFactoryCallback(const CommonAPI::CallStatus&, const BaseType::tBaseError&);
	void resetToMasterFactoryCallback(const CommonAPI::CallStatus& status, const BaseType::tBaseError& error);
    void ActiveTrackNumCB(const CommonAPI::CallStatus &status);
    void TotalTracksNumCB(const CommonAPI::CallStatus &status);
    void setActiveTrackNum(const uint32_t);
    void setTotalTracks(const uint32_t);
    void driverRestrictionsVideoStatusEvent(const bool& driverRestrictionsVideoStatus);
    void setRestrictedMode(uint32_t ClientID, std::function<void(const CommonAPI::CallStatus& status, const PlayerTypes::RestrictedModeStatus& value)>);
    void setSpeedInfoStubCallback(std::function<void(const MMPresTypes::tPlaybackSpeed&)>);
	void updateSlotStateMap(uint8_t slotId, MMPresTypes::eSyncState state);
	MMPresTypes::ePlayState getPlayState();

	CommonAPI::AvailabilityStatus getPlayerProxyStatus();
	CommonAPIClientAbstraction<PlayerProxy>& getPlayerProxy()
	{
		return mPlayer;
	}

	void subscribeToPlayQueueChanged(bool);
	void startProxyTasks();
	void stopProxyTasks();
	void triggerMediaError(uint8_t slotId, MMPresTypes::eMediaError error);
	void intimatePlayEngineForCleanup(uint32_t mediumId);
	uint32_t getCurrentMediumId();
	PlayerTypes::tRectangle _rect;
	bool getDriverRestrictionsVideoStatus();
	void setDriverRestrictionsVideoStatus(bool );
	bool getDriverRestrictionsVideoStatusInformedToCoC();
	void setDriverRestrictionsVideoStatusInformedToCoC(bool );

	void setVideoPlaybackStatusFromCoC(bool );
	bool getVideoPlaybackStatusFromCoC();

    void setMediaLocalName(uint32_t ClientID, std::string, std::function<void(const CommonAPI::CallStatus& status, const std::string& value)>);
    const MMPresTypes::eSyncState getSyncStatus(const uint8_t slotId);
protected:

	void availibility(const CommonAPI::AvailabilityStatus&);
	void playQueueChangedCallback();//CommonAPI::WakeupCallback callback,std::function<void(CommonAPI::CallStatus)>);

private : 
   bool mSubscribed;

	uint32_t registerClient()
	{
		mPlayer.addClient(++mNumClients);
		return mNumClients;
	}
	void deregisterClient(uint32_t ClientID)
	{
		mPlayer.removeClient(ClientID);
	}
	uint32_t mNumClients;
    FacadeHelper<AudioManagerInterface> mAudioInterface;
	MediaPlaybackEngine();
	~MediaPlaybackEngine();
    std::shared_ptr<CommonAPI::Runtime> mRuntime ;
	void initPluginEvents();
    void initAMBInterface();

	void subscribeToPlaybackStatus(bool subscribe);
	void onPlaybackStatusChanged(const PlayerTypes::PlaybackStatus& status);

	void subscribeToUpScaling(bool subscribe);
	void subscribeToSubtitle(bool subscribe);
	void onSubtitleStatusChanged(const PlayerTypes::SubtitleStatus& status);
	void onAspectModeAttributeChanged(const PlayerTypes::AspectModeStatus& status );

	void subscribeToAudioChannel(bool subscribe);
	void onAudioChannelStatusChanged(const PlayerTypes::AudioChannelStatus& status);

	void subscribeToPlayError(bool subscribe);
	void playErrorCallback(const uint32_t& mediumId, const PlayerTypes::PlayerError& error);

	void subscribeToCurrentMediumId(bool subscribe);
	void onCurrentMediumIdChanged(const uint32_t&);

    void subscribeTodevicePlayStatusChange(bool subscribe);
    void devicePlayStatusCallback(const uint32_t& mediumId, PlayerTypes::PlaybackStatus status,std::string uri);

    void subscribeToplaybackSpeedChange(bool subscribe);
    void playbackSpeedChangeCallback(const BaseType::tAudioObjectType audioObjectType,const double& speed);

	friend class FacadeHelper<MediaPlaybackEngine>;
	CommonAPIClientAbstraction<PlayerProxy> mPlayer;
	CommonAPIClientAbstraction<MediaSettingProxy> mMediaSetting;
	CommonAPI::AvailabilityStatus mPlayerProxyStatus;

	std::function<void(const MMPresTypes::tTrackListSupport&)> mtrackListSupportStubCallback;
   std::function<void(const bool&)> mPromoteStatusStubCallBack;
   std::function<void(const bool&)> mDemoteStatusStubCallBack;
   std::function<void(const bool&)> mAddToWishListStatusStubCallBack;
	std::function<void(const MMPresTypes::ePlayState&)> mPlayStateStubCallback;
	std::function<void(const uint64_t&)> mCurrentTimeStubCallback;
	std::function<void(const uint64_t&)> mTotalDurationStubCallback;
   std::function<void(const uint32_t&)> mRemotePlayerChangedStubCallback;
	std::function<void(const MMPresTypes::tTrackState&)> mTrackStateStubCallback;
	std::function<void(const MMPresTypes::tNowPlayingInfo&)> mNowPlayingDataStubCallback;
	std::function<void(const MMPresTypes::eRepeatMode&)> mRepeatModeStubCallback;
	std::function<void(const MMPresTypes::eShuffleMode&)> mShuffleModeStubCallback;
	std::function<void(const MMPresTypes::eTrackSeekSpeed&)> mTrackSeekSpeedStubCallback;
	std::function<void(const MMPresTypes::eArtWorkAvailabilityStatus&)> mArtworkStatusStubCallback;
	std::function<void(const MMPresTypes::tRemotePlayerCapability&)> mRemotePlayerCapabilityStubCallback;
	std::function<void(const MMPresTypes::SyncMap&)> mSyncStatusStubCallback;
	std::function<void(const MMPresTypes::MediaErrorMap&)> mMediaErrorStubCallback;
	std::function<void(const bool&)> mVideoPlayStatusCallback;
	std::function<void(const MMPresTypes::eSubtitleStatus&)> mSubtitleStatusStubCallback;
	std::function<void(const bool&)> mUpScaleStatusStubCallback;
	std::function<void(const MMPresTypes::eAudioChannelStatus&)> mAudioChannelStatusStubCallback;
   std::function<void(const MMPresTypes::tPlaybackSpeed&)> mSpeedInfoStubCallback;
    void mediaPlayBackProxyAvailabiltyCB(const CommonAPI::AvailabilityStatus &AvailState);


    std::shared_ptr<mediaplaybackProxy<>> mMediaPlayBack;
	FacadeHelper<MediaDetectionEngine> mDetect;
	uint32_t mPlaybackStatusToken;
	uint32_t mSubtitleToken;
	uint32_t mUpScaleToken;
	uint32_t mAudioChannelToken;
	uint32_t mCurrentMediumIdToken;
	uint32_t mCurrentMediumId;
	MMPresTypes::ePlayState mPlayState;
	MMPresTypes::SyncMap mSyncMap;
	MMPresTypes::MediaErrorMap mMediaErrorMap;
	static std::unordered_map<PlayerTypes::PlayerError, MMPresTypes::eMediaError> mPlayerToMediaError;
	bool mDriverRestrictionsVideoStatus;
	void subscribeToRestrictedMode(bool subscribe);
	uint32_t mRestrictedModeToken;
	void onRestrictedModeChanged(const PlayerTypes::RestrictedModeStatus& status);
	bool mDriverRestrictionsVideoStatusInformedToCoC;

	void subscribeToVideoPlayStatus(bool subscribe);
	void onVideoPlayStatusChanged(const bool&);
	bool mVideoPlaybackStatusFromCoC;

};

#endif
