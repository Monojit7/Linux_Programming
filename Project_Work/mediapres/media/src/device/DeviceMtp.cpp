#include <device/DeviceMtp.h>

DeviceMtp::DeviceMtp(const v1::com::harman::media::BaseType::tMediumInfo& MediumInfo)
	:DeviceBase(MediumInfo)
	, mSeekTimer(*this)
	, mSeekTimerId(0u)
	, mRateInvoked(0)
	, mffwdRequested(false)
	, mfrwdRequested(false)
	, mRateTimeMap(mSettings->getRateTimeMap())
    , mRateTimeMapVideo(mSettings->getRateTimeMapForVideo())
	, mRateTimeMapForAud_Podcast(mSettings->getRateTimeMapForAud_Podcast())
{
	for (std::map<double, int>::iterator ctr = mRateTimeMap.begin(); ctr != mRateTimeMap.end(); ctr++)
	{
		if ((ctr->second) == 0)
		{
			mRateTimeMap.erase(ctr->first);
			ctr--;
		}
	}

	for (std::map<double, int>::iterator ctr = mRateTimeMapVideo.begin(); ctr != mRateTimeMapVideo.end(); ctr++)
		{
			if ((ctr->second) == 0)
			{
				mRateTimeMapVideo.erase(ctr->first);
				ctr--;
			}
		}

	for (std::map<double, int>::iterator ctr = mRateTimeMapForAud_Podcast.begin(); ctr != mRateTimeMapForAud_Podcast.end(); ctr++)
	{
		if ((ctr->second) == 0)
		{
			mRateTimeMapForAud_Podcast.erase(ctr->first);
			ctr--;
		}
	}
}

uint32_t DeviceMtp::getDeviceCaps()
{
	return (MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATABROWSE | MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATASEARCH);
}

MMPresTypes::eCallResult DeviceMtp::fastForward()
{
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	cancelSeekTimer();
	std::map<double, int>::const_iterator ctr = mRateTimeMap.begin();
	std::map<double, int>::const_iterator ctr1 = mRateTimeMapVideo.begin();
	std::map<double, int>::const_iterator ctr2 = mRateTimeMapForAud_Podcast.begin();

	MMPresTypes::eMetadataType  type  = mNowPlayingInfo.getNowPlayingItemType();

	if((mRateTimeMapVideo.size() > 0)&&(type == MMPresTypes::eMetadataType::METADATATYPE_VIDEO))
	{
		if (ctr1 != mRateTimeMapVideo.end())
		{
			mRateInvoked = ctr1->first;
			mffwdRequested = true;
			mfrwdRequested = false;
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::fastForward:mRateTimeMapVideo-mRateInvoked : %f", mRateInvoked);
			mPlayEngine->fastForward(mPlayEngine.mClientID, mRateInvoked, std::bind(&DeviceMtp::fastForwardCallback, this, _1, _2));
			setSeekTimer();
		}
		else
		{
			result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
		}
	}
	else if((mRateTimeMapForAud_Podcast.size()>0) && ((type == MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK) ||( type == MMPresTypes::eMetadataType::METADTATATYPE_PODCAST)))
	{
	    if (ctr2 != mRateTimeMapForAud_Podcast.end())
	     {
		    mRateInvoked = ctr2->first;
		    mffwdRequested = true;
		    mfrwdRequested = false;
		    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::fastForward:mRateTimeMapForAud_Podcast-mRateInvoked : %f", mRateInvoked);
		    mPlayEngine->fastForward(mPlayEngine.mClientID, mRateInvoked, std::bind(&DeviceMtp::fastForwardCallback, this, _1, _2));
		    setSeekTimer();
	    }
	    else
	    {
		    result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
	    }
	}
	else if((mRateTimeMap.size() > 0) && (type == MMPresTypes::eMetadataType::METADATATYPE_TITLE))
	{
		if (ctr != mRateTimeMap.end())
		{
			mRateInvoked = ctr->first;
			mffwdRequested = true;
			mfrwdRequested = false;
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::fastForward:mRateTimeMap-mRateInvoked : %f", mRateInvoked);
			mPlayEngine->fastForward(mPlayEngine.mClientID, mRateInvoked, std::bind(&DeviceMtp::fastForwardCallback, this, _1, _2));
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

MMPresTypes::eCallResult DeviceMtp::fastReverse()
{
	MMPresTypes::eCallResult result = MMPresTypes::eCallResult::CALLRESULT_OK;
	cancelSeekTimer();
	std::map<double, int>::const_iterator ctr = mRateTimeMap.begin();
	std::map<double, int>::const_iterator ctr1 = mRateTimeMapVideo.begin();
	std::map<double, int>::const_iterator ctr2 = mRateTimeMapForAud_Podcast.begin();
	MMPresTypes::eMetadataType type  = mNowPlayingInfo.getNowPlayingItemType();

	if((mRateTimeMapVideo.size() > 0)&&(type == MMPresTypes::eMetadataType::METADATATYPE_VIDEO))
	{
		if (ctr1 != mRateTimeMapVideo.end())
		{
			mRateInvoked = ctr1->first;
			mffwdRequested = false;
			mfrwdRequested = true;
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::fastReverse:mRateTimeMapVideo-mRateInvoked : %f", mRateInvoked);
			mPlayEngine->fastReverse(mPlayEngine.mClientID, (MINUS_1)*mRateInvoked, std::bind(&DeviceMtp::fastReverseCallback, this, _1, _2));
			setSeekTimer();
		}
		else
		{
			result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
		}
	}
	else if((mRateTimeMapForAud_Podcast.size() >0) &&
				((type == MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK) ||( type == MMPresTypes::eMetadataType::METADTATATYPE_PODCAST)))
	{
		if (ctr2 != mRateTimeMapForAud_Podcast.end())
		 {
			mRateInvoked = ctr2->first;
			mffwdRequested = false;
			mfrwdRequested = true;
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::fastReverse:mRateTimeMapForAud_Podcast-mRateInvoked : %f", mRateInvoked);
			mPlayEngine->fastReverse(mPlayEngine.mClientID, (MINUS_1)*mRateInvoked, std::bind(&DeviceMtp::fastReverseCallback, this, _1, _2));
			setSeekTimer();
		}
		else
		{
			result = MMPresTypes::eCallResult::CALLRESULT_FAILED_INTERNALERROR;
		}
	}

	else if (mRateTimeMap.size() > 0)
	{
		if (ctr != mRateTimeMap.end())
		{
			mRateInvoked = ctr->first;
			mffwdRequested = false;
			mfrwdRequested = true;
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::fastReverse:mRateTimeMap-mRateInvoked : %f", mRateInvoked);
			mPlayEngine->fastReverse(mPlayEngine.mClientID, (MINUS_1)*mRateInvoked, std::bind(&DeviceMtp::fastReverseCallback, this, _1, _2));
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

void DeviceMtp::fastForwardCallback(const CommonAPI::CallStatus& status, const double& rate)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::fastForwardCallback :CallStatus : %d rate: %f", (int)status, rate);
}

void DeviceMtp::fastReverseCallback(const CommonAPI::CallStatus& status, const double& rate)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::fastReverseCallback :CallStatus : %d rate: %f", (int)status, rate);
}

void DeviceMtp::timerEvent(int timerId)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::timerEvent:timerId : %d,mSeekTimerId:%d,mTrackSeekSpeed:%d,playState:%d,mRateInvoked:%f", timerId, mSeekTimerId, (int)mTrackSeekSpeed, (int)mPlayState, mRateInvoked);
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
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::timerEvent:mRateInvoked:%f,currentRate:%f", mRateInvoked, mCurrentRate);
		if (mRateInvoked > 0)
		{
			if (mffwdRequested)
			{
				mPlayEngine->fastForward(mPlayEngine.mClientID, mRateInvoked, std::bind(&DeviceMtp::fastForwardCallback, this, _1, _2));
				setSeekTimer();
			}
			else if (mfrwdRequested)
			{
				mPlayEngine->fastReverse(mPlayEngine.mClientID, (MINUS_1)*mRateInvoked, std::bind(&DeviceMtp::fastReverseCallback, this, _1, _2));
				setSeekTimer();
			}
			else
			{

			}
		}
	}
}

int DeviceMtp::determineTimerValue()
{
	int value = -1;
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::determineTimerValue-mRateInvoked : %f", mRateInvoked);
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
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::determineTimerValue :value : %d ms", value);
	return value;
}

void DeviceMtp::cancelSeekTimer()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::cancelSeekTimer-mSeekTimerId : %d", mSeekTimerId);
	mSeekTimer.cancelTimer(mSeekTimerId);
	mSeekTimerId = 0;
	mRateInvoked = 0;
	mffwdRequested = false;
	mfrwdRequested = false;
}

void DeviceMtp::setSeekTimer()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::setSeekTimer");
	int value = determineTimerValue();
	if (value > 0)
	{
		mSeekTimerId = mSeekTimer.setTimer(CTimerInterface::TIMER_ONESHOT, value);
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceMtp::setSeekTimer-mSeekTimerId : %d", mSeekTimerId);
	}
}
