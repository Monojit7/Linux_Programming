#include <device/DeviceUmass.h>

DeviceUmass::DeviceUmass(const v1::com::harman::media::BaseType::tMediumInfo& MediumInfo)
	:DeviceBase(MediumInfo)
	, mSeekTimer(*this)
	, mSeekTimerId(0u)
	, mRateInvoked(0)
	, mffwdRequested(false)
	, mfrwdRequested(false)
	, mRateTimeMap(mSettings->getRateTimeMap())
    , mRateTimeMapVideo(mSettings->getRateTimeMapForVideo())
	, mRateTimeMapForAud_Podcast(mSettings->getRateTimeMapForAud_Podcast())
	, mMediaOneRepeatStatus(PlayerTypes::RepeatStatus::REPEAT)
	, mExposeRepeatOffToHMI(false)
	, mRepeatToken(0u)
{
	for (std::map<double, int>::iterator ctr = mRateTimeMap.begin(); ctr != mRateTimeMap.end();)
	{
		if ((ctr->second) == 0)
		{
			mRateTimeMap.erase(ctr->first);
			ctr = mRateTimeMap.begin();
		}
		else
		{
			++ctr;
		}
	}

	for (std::map<double, int>::iterator ctr = mRateTimeMapVideo.begin(); ctr != mRateTimeMapVideo.end();)
		{
			if ((ctr->second) == 0)
			{
				mRateTimeMapVideo.erase(ctr->first);
				ctr = mRateTimeMapVideo.begin();
			}
			else
			{
				++ctr;
			}
		}

	for (std::map<double, int>::iterator ctr = mRateTimeMapForAud_Podcast.begin(); ctr != mRateTimeMapForAud_Podcast.end();)
	{
		if ((ctr->second) == 0)
		{
			mRateTimeMapForAud_Podcast.erase(ctr->first);
			ctr = mRateTimeMapForAud_Podcast.begin();
		}
		else
		{
			++ctr;
		}
	}
}

DeviceUmass::~DeviceUmass()
{
	if (mSettings->isRepeatOffSameAsRepeatAllForUSB())
	{
		mExposeRepeatOffToHMI = false;
	}
}

uint32_t DeviceUmass::getDeviceCaps()
{

	return (MMPresTypes::eDeviceCapabilities::DEVICECAP_FSBROWSE | MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATABROWSE | MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATASEARCH);
}

MMPresTypes::eCallResult DeviceUmass::fastForward()
{
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	std::map<double, int> timeMap;
	timeMap.clear();
	switch (mNowPlayingInfo.getNowPlayingItemType())
	{
	case MMPresTypes::eMetadataType::METADATATYPE_TITLE:
		timeMap = mRateTimeMap;
		break;
	case MMPresTypes::eMetadataType::METADATATYPE_VIDEO:
		timeMap = mRateTimeMapVideo;
	    break;
	case MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_CHAPTER:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_PODCAST:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_EPISODE:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	default:
		timeMap = mRateTimeMap;
		break;
	}

	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceUmass::fastForward-timeMap size:%d", timeMap.size());

	if (timeMap.size()>0)
	{
		cancelSeekTimer();
		std::map<double, int>::const_iterator ctr = timeMap.begin();
		if (ctr != timeMap.end())
		{
			mRateInvoked = ctr->first;
			mffwdRequested = true;
			mfrwdRequested = false;
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceUmass::fastForward-mRateInvoked : %f", mRateInvoked);
			mPlayEngine->fastForward(mPlayEngine.mClientID, mRateInvoked, std::bind(&DeviceUmass::fastForwardCallback, this, _1, _2));
			setSeekTimer();
		}
		else
		{
			result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
		}
	}
	else
	{
		result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
	}
	return result;
}

MMPresTypes::eCallResult DeviceUmass::fastReverse()
{
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	std::map<double, int> timeMap;
	timeMap.clear();
	switch (mNowPlayingInfo.getNowPlayingItemType())
	{
	case MMPresTypes::eMetadataType::METADATATYPE_TITLE:
		timeMap = mRateTimeMap;
		break;
	case MMPresTypes::eMetadataType::METADATATYPE_VIDEO:
		timeMap = mRateTimeMapVideo;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_CHAPTER:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_PODCAST:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_EPISODE:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	default:
		timeMap = mRateTimeMap;
		break;
	}
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceUmass::fastReverse-timeMap size:%d", timeMap.size());
	if (timeMap.size()>0)
	{
		cancelSeekTimer();
		std::map<double, int>::const_iterator ctr = timeMap.begin();
		if (ctr != timeMap.end())
		{
			mRateInvoked = ctr->first;
			mffwdRequested = false;
			mfrwdRequested = true;
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceUmass::fastReverse-mRateInvoked : %f", mRateInvoked);
			mPlayEngine->fastReverse(mPlayEngine.mClientID, (MINUS_1)*mRateInvoked, std::bind(&DeviceUmass::fastReverseCallback, this, _1, _2));
			setSeekTimer();
		}
		else
		{
			result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
		}
	}
	else
	{
		result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
	}
	return result;
}

void DeviceUmass::timerEvent(int timerId)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceUmass::timerEvent:timerId : %d,mSeekTimerId:%d,mTrackSeekSpeed:%d,playState:%d,mRateInvoked:%f", timerId, mSeekTimerId, (int)mTrackSeekSpeed, (int)mPlayState, mRateInvoked);
	double rate = 0;
	std::map<double, int> timeMap;
	switch (mNowPlayingInfo.getNowPlayingItemType())
	{
	case MMPresTypes::eMetadataType::METADATATYPE_TITLE:
		timeMap = mRateTimeMap;
		break;
	case MMPresTypes::eMetadataType::METADATATYPE_VIDEO:
		timeMap = mRateTimeMapVideo;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_CHAPTER:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_PODCAST:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_EPISODE:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	default:
		timeMap = mRateTimeMap;
		break;
	}

	if (timerId > 0 && mSeekTimerId == timerId)
	{
		for (std::map<double, int>::const_iterator ctr = timeMap.begin(); ctr != timeMap.end(); ctr++)
		{
			if (mRateInvoked == ctr->first)
			{
				ctr++;
				if (ctr != timeMap.end())
				{
					rate = ctr->first;
				}
			}
		}
		mRateInvoked = rate;
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceUmass::timerEvent:mRateInvoked:%f,currentRate:%f", mRateInvoked, mCurrentRate);
		if (mRateInvoked > 0)
		{
			if (mffwdRequested)
			{
				mPlayEngine->fastForward(mPlayEngine.mClientID, mRateInvoked, std::bind(&DeviceUmass::fastForwardCallback, this, _1, _2));
				setSeekTimer();
			}
			else if (mfrwdRequested)
			{
				mPlayEngine->fastReverse(mPlayEngine.mClientID, (MINUS_1)*mRateInvoked, std::bind(&DeviceUmass::fastReverseCallback, this, _1, _2));
				setSeekTimer();
			}
			else
			{

			}
		}
	}
}

int DeviceUmass::determineTimerValue()
{
	int value = -1;
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceUmass::determineTimerValue-mRateInvoked : %f", mRateInvoked);
	std::map<double, int> timeMap;
	switch (mNowPlayingInfo.getNowPlayingItemType())
	{
	case MMPresTypes::eMetadataType::METADATATYPE_TITLE:
		timeMap = mRateTimeMap;
		break;
	case MMPresTypes::eMetadataType::METADATATYPE_VIDEO:
		timeMap = mRateTimeMapVideo;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_CHAPTER:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_PODCAST:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_EPISODE:
		timeMap = mRateTimeMapForAud_Podcast;
		break;
	default:
		timeMap = mRateTimeMap;
		break;
	}
	for (std::map<double, int>::const_iterator ctr = timeMap.begin(); ctr != timeMap.end(); ctr++)
	{
		if (mRateInvoked == ctr->first)
		{
			value = (ctr->second)*SEC_MILLISEC;
			break;
		}
	}
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceUmass::determineTimerValue :value : %d ms", value);
	return value;
}

void DeviceUmass::cancelSeekTimer()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceUmass::cancelSeekTimer-mSeekTimerId : %d", mSeekTimerId);
	mSeekTimer.cancelTimer(mSeekTimerId);
	mSeekTimerId = 0;
	mRateInvoked = 0;
	mffwdRequested = false;
	mfrwdRequested = false;
}

void DeviceUmass::setSeekTimer()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceUmass::setSeekTimer");
	int value = determineTimerValue();
	if (value > 0)
	{
		mSeekTimerId = mSeekTimer.setTimer(CTimerInterface::TIMER_ONESHOT, value);
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceUmass::setSeekTimer-mSeekTimerId : %d", mSeekTimerId);
	}
}

void DeviceUmass::setRepeat(MMPresTypes::eRepeatMode mode)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::setRepeat -mediumId:%lu , repeat input is %d", mMediumInfo.getMMediumId(), (int)mode);
	PlayerTypes::RepeatStatus repeat;
	FacadeHelper<MediaPresSettings> mSettings;
	bool repeatSupported = true;
	switch (mode)
	{
	case MMPresTypes::eRepeatMode::REPEAT_ALL:
		repeat = PlayerTypes::RepeatStatus::REPEAT;
		if (mSettings->isRepeatOffSameAsRepeatAllForUSB())
		{
			if (repeat == mMediaOneRepeatStatus)
			{
				mRepeatMode = mode;
                LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::setRepeat-send the internal mediaone repeat all to HMI");
				std::function<void(const MMPresTypes::eRepeatMode&)> callback = mPlayEngine->getRepeatModeStubCallback();
				if (callback != nullptr)
				{
					callback(mode);
				}
			}
			else
			{
				mPlayEngine->setRepeat(mPlayEngine.mClientID, repeat, std::bind(&DeviceUmass::setRepeatCallback, this, _1, _2));
			}
			mExposeRepeatOffToHMI = false;
		}
		else
		{
		  mPlayEngine->setRepeat(mPlayEngine.mClientID, repeat, std::bind(&DeviceUmass::setRepeatCallback, this, _1, _2));
		}
		break;
	case MMPresTypes::eRepeatMode::REPEAT_SONG:
		repeat = PlayerTypes::RepeatStatus::REPEAT_SINGLE;
		if (mSettings->isRepeatOffSameAsRepeatAllForUSB())
		{
			mExposeRepeatOffToHMI = false;
		}
		mPlayEngine->setRepeat(mPlayEngine.mClientID, repeat, std::bind(&DeviceUmass::setRepeatCallback, this, _1, _2));
		break;
	case MMPresTypes::eRepeatMode::REPEAT_OFF:
		if (mSettings->isRepeatOffSameAsRepeatAllForUSB())
		{
			if (mMediaOneRepeatStatus == PlayerTypes::RepeatStatus::REPEAT)
			{
                LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::setRepeat- set repeat as all to mediaone even though request is repeat off");
				repeat = PlayerTypes::RepeatStatus::REPEAT;
				mExposeRepeatOffToHMI = true;

				mRepeatMode = mode;
                LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::setRepeat-send the internal mediapres repeat off to HMI");
				std::function<void(const MMPresTypes::eRepeatMode&)> callback = mPlayEngine->getRepeatModeStubCallback();
				if (callback != nullptr)
				{
					callback(mode);
				}
			}
			else
			{
            LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::setRepeat- set repeat as all to mediaone even though request is repeat off");
			repeat = PlayerTypes::RepeatStatus::REPEAT;
			mExposeRepeatOffToHMI = true;
				mPlayEngine->setRepeat(mPlayEngine.mClientID, repeat, std::bind(&DeviceUmass::setRepeatCallback, this, _1, _2));
			}
		}
		else
		{
			repeat = PlayerTypes::RepeatStatus::NO_REPEAT;
			mExposeRepeatOffToHMI = false;
			mPlayEngine->setRepeat(mPlayEngine.mClientID, repeat, std::bind(&DeviceUmass::setRepeatCallback, this, _1, _2));
		}
		break;
	case MMPresTypes::eRepeatMode::REPEAT_ALBUM:
		repeat = PlayerTypes::RepeatStatus::REPEAT;
		if (mSettings->isRepeatOffSameAsRepeatAllForUSB())
		{
			mExposeRepeatOffToHMI = false;
		}
		mPlayEngine->setRepeat(mPlayEngine.mClientID, repeat, std::bind(&DeviceUmass::setRepeatCallback, this, _1, _2));
		break;
	case MMPresTypes::eRepeatMode::REPEAT_FOLDER:
		repeat = PlayerTypes::RepeatStatus::REPEAT;
		if (mSettings->isRepeatOffSameAsRepeatAllForUSB())
		{
			mExposeRepeatOffToHMI = false;
		}
		mPlayEngine->setRepeat(mPlayEngine.mClientID, repeat, std::bind(&DeviceUmass::setRepeatCallback, this, _1, _2));
		break;
	case MMPresTypes::eRepeatMode::REPEAT_SUBFOLDER:
		repeat = PlayerTypes::RepeatStatus::REPEAT;
		if (mSettings->isRepeatOffSameAsRepeatAllForUSB())
		{
			mExposeRepeatOffToHMI = false;
		}
		mPlayEngine->setRepeat(mPlayEngine.mClientID, repeat, std::bind(&DeviceUmass::setRepeatCallback, this, _1, _2));
		break;
	default:
		repeatSupported = false;
		if (mSettings->isRepeatOffSameAsRepeatAllForUSB())
		{
			mExposeRepeatOffToHMI = false;
		}
		break;
	}

	if (repeatSupported)
	{
        LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::setRepeat -current playstate is %d", (int)mPlayState);
		if (mode == MMPresTypes::eRepeatMode::REPEAT_ALL && mPlayState == MMPresTypes::ePlayState::PLAYSTATE_STOPPED)
		{
			mPlayEngine->resume(mPlayEngine.mClientID, std::bind(&DeviceUmass::resumeCallback, this, _1, _2));
		}
	}
}

void DeviceUmass::onRepeatChanged(const PlayerTypes::RepeatStatus& status)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceUmass::onRepeatChanged -mediumId:%lu , repeat status:%d ", mMediumInfo.getMMediumId(), (int)status);
	FacadeHelper<MediaPresSettings> mSettings;
	MMPresTypes::eRepeatMode mode;
	bool updateMode = true;
	mMediaOneRepeatStatus = status;
	switch (status)
	{
	case PlayerTypes::RepeatStatus::REPEAT:
		if (mSettings->isRepeatOffSameAsRepeatAllForUSB())
		{
			if (mExposeRepeatOffToHMI)
			{
				LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceUmass::onRepeatChanged-expose repeat off to HMI though repeat is all");
				mode = MMPresTypes::eRepeatMode::REPEAT_OFF;
			}
			else
			{
				mode = MMPresTypes::eRepeatMode::REPEAT_ALL;
			}
		}
		else
		{
			mode = MMPresTypes::eRepeatMode::REPEAT_ALL;
		}
		break;
	case PlayerTypes::RepeatStatus::REPEAT_SINGLE:
		mode = MMPresTypes::eRepeatMode::REPEAT_SONG;
		break;
	case PlayerTypes::RepeatStatus::NO_REPEAT:
		if (mSettings->isRepeatOffSameAsRepeatAllForUSB())
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, ">>> DeviceUmass::onRepeatChanged-internally set repeat off to repeat all");
			setRepeat(MMPresTypes::eRepeatMode::REPEAT_ALL);
			updateMode = false;
		}
		else
		{
			mode = MMPresTypes::eRepeatMode::REPEAT_OFF;
		}
		break;
	case PlayerTypes::RepeatStatus::REPEAT_CHANGE:
		mode = MMPresTypes::eRepeatMode::REPEAT_TOGGLE;
		break;
	default:
		mode = MMPresTypes::eRepeatMode::REPEAT_UNKNOWN;
		break;
	}

	mRepeatMode = mode;

	std::function<void(const MMPresTypes::eRepeatMode&)> callback = mPlayEngine->getRepeatModeStubCallback();
	if (callback != nullptr && updateMode)
	{
		callback(mode);
	}
}

void DeviceUmass::subscribeToRepeatStatusUpdate(bool subscribe)
{
	if (subscribe)
	{
		mRepeatToken = SUBSCRIBE(mPlayEngine->getPlayerProxy(), getRepeatAttribute().getChangedEvent(), &DeviceUmass::onRepeatChanged, _1)
        LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::subscribeToRepeatStatusUpdate - subscription token: %d", mRepeatToken);
	}
	else
	{
        LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::un-subscribeToRepeatStatusUpdate - subscription token: %d", mRepeatToken);
		UNSUBSCRIBE(mPlayEngine->getPlayerProxy(), getRepeatAttribute().getChangedEvent(), mRepeatToken)
	}
}

void DeviceUmass::next()
{
	cancelSeekTimer();
	if (mRepeatMode == MMPresTypes::eRepeatMode::REPEAT_OFF && mPlayState == MMPresTypes::ePlayState::PLAYSTATE_STOPPED)
	{
		uint8_t temp_slotid = mDetect->findMatchingSlot(mMediumInfo.getMMediumId());
		triggerMediaErrorToDevice(temp_slotid, MMPresTypes::eMediaError::MEDIAERROR_ENDOFPLAYLIST);
	}

	LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::mMultipleChapterAvailable:%d",mMultipleChapterAvailable);
	MMPresTypes::eMetadataType type = mNowPlayingInfo.getNowPlayingItemType();
	if((mMultipleChapterAvailable) && (type != MMPresTypes::eMetadataType::METADATATYPE_TITLE))
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::nextChapter");
		mPlayEngine->nextChapter(mPlayEngine.mClientID, std::bind(&DeviceUmass::nextChapterCallback, this, _1, _2));
	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::next");
		mPlayEngine->next(mPlayEngine.mClientID, std::bind(&DeviceUmass::nextCallback, this, _1, _2));
	}
}

void DeviceUmass::previous()
{
	cancelSeekTimer();
	if (mRepeatMode == MMPresTypes::eRepeatMode::REPEAT_OFF && mPlayState == MMPresTypes::ePlayState::PLAYSTATE_STOPPED)
	{
		uint8_t temp_slotid = mDetect->findMatchingSlot(mMediumInfo.getMMediumId());
		triggerMediaErrorToDevice(temp_slotid, MMPresTypes::eMediaError::MEDIAERROR_ENDOFPLAYLIST);
	}

	MMPresTypes::eMetadataType type = mNowPlayingInfo.getNowPlayingItemType();
	if((mMultipleChapterAvailable) && (type != MMPresTypes::eMetadataType::METADATATYPE_TITLE))
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::previousChapter");
		mPlayEngine->previousChapter(mPlayEngine.mClientID, std::bind(&DeviceUmass::previousChapterCallback, this, _1, _2));

	}
	else
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceUmass::previous");
		mPlayEngine->previous(mPlayEngine.mClientID, std::bind(&DeviceUmass::previousCallback, this, _1, _2));
	}
}
