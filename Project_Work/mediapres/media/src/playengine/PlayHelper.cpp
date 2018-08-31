#include<playengine/PlayHelper.h>

std::unordered_map<MMPresTypes::eMetadataType, std::string> PlayHelper::mNowPlayingItemTypeMapping = {
	{MMPresTypes::eMetadataType::METADATATYPE_TITLE,"song"}
	,{MMPresTypes::eMetadataType::METADTATATYPE_PODCAST,"podcast"}
	,{MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK,"audiobook"} 
	,{MMPresTypes::eMetadataType::METADATATYPE_COMPILATION,"iTunesRadio"}
	,{MMPresTypes::eMetadataType::METADATATYPE_VIDEO,"video"}
	,{ MMPresTypes::eMetadataType::METADATATYPE_IMAGE,"image"}
};

std::unordered_map<MMPresTypes::eMetadataType, BaseType::tAudioObjectType> PlayHelper::mMetadataMapping = {
	{ MMPresTypes::eMetadataType::METADTATATYPE_PODCAST,BaseType::tAudioObjectType::PODCAST}
	,{ MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK,BaseType ::tAudioObjectType::AUDIOBOOK}
};

std::unordered_map<MMPresTypes::ePlaybackSpeed, double> PlayHelper::mPlaybackSpeedMapping = {
	{ MMPresTypes::ePlaybackSpeed::NORMAL,1.00 }
	,{ MMPresTypes::ePlaybackSpeed::SLOWER,0.67}
	,{ MMPresTypes::ePlaybackSpeed::FASTER,1.25}
};

MMPresTypes::eMetadataType PlayHelper::getNowPlayingItemType(const std::string& itemType)
{
	std::unordered_map<MMPresTypes::eMetadataType,std::string>::const_iterator itr = mNowPlayingItemTypeMapping.begin();
	while (itr != mNowPlayingItemTypeMapping.end())
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< PlayHelper::getNowPlayingItemType: itemType=%s,string metadataType from map=%s int metadataType from map=%d", itemType.c_str(), (itr->second).c_str(), (int)(itr->first));
		if (((itr->second).compare(itemType)) == 0)
		{
			return itr->first;
		}
		itr++;
	}
	return MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;
}

bool PlayHelper::getMetadataType(const MMPresTypes::eMetadataType& type,BaseType::tAudioObjectType& audioType)
{
	std::unordered_map<MMPresTypes::eMetadataType, BaseType::tAudioObjectType>::const_iterator itr= mMetadataMapping.find(type);
	if (itr != mMetadataMapping.end())
	{
		audioType = itr->second;
		return true;
	}
	return false;
}

bool PlayHelper::isPlaybackSpeedSupported(const MMPresTypes::eMetadataType type)
{
	bool ret = false;
	switch (type)
	{
	case MMPresTypes::eMetadataType::METADTATATYPE_PODCAST:
		ret = true;
		break;
	case MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK:
		ret = true;
		break;
	default:
		break;
	}
	return ret;
}


double PlayHelper::getPlaybackSpeed(const MMPresTypes::ePlaybackSpeed speed)
{
	std::unordered_map<MMPresTypes::ePlaybackSpeed, double>::const_iterator itr = mPlaybackSpeedMapping.find(speed);
	double rate=1.00;
	if (itr != mPlaybackSpeedMapping.end())
	{
		rate = itr->second;
	}
	return rate;
}
