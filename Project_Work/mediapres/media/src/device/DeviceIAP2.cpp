#include <device/DeviceIAP2.h>

DeviceIAP2::DeviceIAP2(const v1::com::harman::media::BaseType::tMediumInfo& mediumInfo)
	:DeviceBase(mediumInfo)
{

}

uint32_t DeviceIAP2::getDeviceCaps()
{

	return (MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATABROWSE | MMPresTypes::eDeviceCapabilities::DEVICECAP_METADATASEARCH);
}
/*
void DeviceIAP2::setRepeat(MMPresTypes::eRepeatMode mode)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceIAP2::setRepeat - repeat input is %d", (int)mode);
	PlayerTypes::RepeatStatus repeat;
	bool repeatSupported = true;
	switch (mode)
	{
	case MMPresTypes::eRepeatMode::REPEAT_ALL:
		repeat = PlayerTypes::RepeatStatus::REPEAT_CHANGE;
		break;
	case MMPresTypes::eRepeatMode::REPEAT_SONG:
		repeat = PlayerTypes::RepeatStatus::REPEAT_CHANGE;
		break;
	case MMPresTypes::eRepeatMode::REPEAT_OFF:
		repeat = PlayerTypes::RepeatStatus::REPEAT_CHANGE;
		break;
	case MMPresTypes::eRepeatMode::REPEAT_ALBUM:
		repeat = PlayerTypes::RepeatStatus::REPEAT_CHANGE;
		break;
	case MMPresTypes::eRepeatMode::REPEAT_FOLDER:
		repeat = PlayerTypes::RepeatStatus::REPEAT_CHANGE;
		break;
	case MMPresTypes::eRepeatMode::REPEAT_SUBFOLDER:
		repeat = PlayerTypes::RepeatStatus::REPEAT_CHANGE;
		break;
	default:
		repeatSupported = false;
		break;
	}

	if (repeatSupported)
	{
        LOG_INFO(LOG_CONTEXT_MEDIA, "DeviceIAP2::setRepeat - repeat requested to mediaone is %d", (int)repeat);
		mPlayEngine->setRepeat(mPlayEngine.mClientID, repeat, std::bind(&DeviceIAP2::setRepeatCallback, this, _1, _2));
	}
}

void DeviceIAP2::setRepeatCallback(const CommonAPI::CallStatus& status, const PlayerTypes::RepeatStatus& mode)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< DeviceIAP2::setRepeatCallback :CallStatus : %d RepeatStatus %d", (int)status, (int)mode);
}
*/
