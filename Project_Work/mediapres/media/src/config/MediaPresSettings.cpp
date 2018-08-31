#include<config/MediaPresSettings.h>
#include <playengine/MediaPlaybackEngine.h>

std::unordered_map<std::string,MMPresTypes::eSlotType> MediaPresSettings::mSlotMap = {
	{"usb",MMPresTypes::eSlotType::SLOTTYPE_USB}
   ,{"ipod",MMPresTypes::eSlotType::SLOTTYPE_IPOD}
	,{"bluetooth",MMPresTypes::eSlotType::SLOTTYPE_BLUETOOTH}
	,{"REMOTE_CD", MMPresTypes::eSlotType::SLOTTYPE_REMOTE_CD}
};

std::unordered_map<std::string, double> MediaPresSettings::mRateMap = {
	{ "2x",2.000000 }
	,{ "4x",4.000000 }
	,{ "8x",8.000000 }
	,{ "16x",16.000000 }
	,{ "32x",32.000000 }
	,{ "64x",64.000000 }
	,{ "128x",128.000000 }
};

std::unordered_map<std::string, std::string> MediaPresSettings::mVehicleSettingLangToMediaLang = {
   { "UK_English","en"  },
   { "NA_English","en"  },
   { "German","de"  },
   { "Italian","it" },
   { "EU_French","fr"},
   { "Cana_French","es" },
   { "EU_Spanish","es"},
   { "Mex_Spanish","es"},
   { "Turkish","tr"},
   { "Russian","ru"},
   { "Dutch","nl" },
   { "Flemish","nl"  },
   { "Polish","pl" },
   { "Czech","cs" },
   { "Greek","el" },
   { "Hungarian","hu" },
   { "Swedish","sv" },
   { "Danish","da" },
   { "Norwegian","no" },
   { "Finish","fi" },
   { "EU_Portuguese","pt" },
   { "Braz_Portuguese","pt"},
   { "Japanese","ja" },
   { "AU_English","en" },
   { "Korean","ko" },
   { "Mandarin_Chinese","zh" },
   { "Taiwanese","zh" },
   { "Arabic","ar" },
   { "Slovak","sk" },
   { "Thai","th" },
   { "Indian_English","en" }
};

MediaPresSettings::MediaPresSettings()
:mIndexerProxy("local", "inst0")
,mGenresCategoryName("genres")
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::MediaPresSettings");
    SUBSCRIBE(mIndexerProxy, getProxyStatusEvent(), &MediaPresSettings::availibility, std::placeholders::_1);
	initSlots();
	initSourceList();
	initAMInteraction();
	initSeekSpeedSettings();
	initSeekSpeedSettingsfor_video();
	initSeekSpeedSettingsForAud_Podcast();
	initBrowseHistoryEnumeration();
	initAppleAuthenticationChipFunctionalCheck();
	initGetAttributeUsageForCount();
	initGenresCategoryName();
	initRepeatSettings();
	initDequeAllUsage();
	initContainerNameMapping();
    initPersLibrary();
    initReadEolMediaIndexing();
    initReadEolUSB();
    initReadEolVideoPlayback();
    initReadEolDefaultLanguage();
    initReadEolEXTCD();
    initSingleCtageoryListForBrowse();
    initPauseVideoPlaybackDuringBrowse();
    initElevatorBarHelperLetterTimeout();

    initPluginEvents();
}

MediaPresSettings::~MediaPresSettings()
{
    deinitPersLibrary();
}

void MediaPresSettings::availibility(const CommonAPI::AvailabilityStatus& avail)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPresSettings::availibility-IndexerProxyAvail : %s", mIndexerProxy.getStringAvailibility(avail).c_str());
    mIndexerProxyStatus = avail;
    if(mIndexerProxy->isAvailable())
    {
        setAllEOLData();
    }
}

void MediaPresSettings::initSeekSpeedSettings()
{
	Json::Value val = config->GetConfigFileKey("IncrementalSpeedSettingForTrack");
   LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettings-size of json data is %d type of json data is %d", val.size(), val.type());
	std::vector<std::string> keyList;
	keyList.clear();
	int value = 0;

	for (unsigned int i = 0; i < val.size(); i++)
	{
		Json::Value val1 = val[i];

		std::vector<std::string> memberList;
		memberList = val1.getMemberNames();
		for (unsigned int i = 0; i < memberList.size(); i++)
		{
			Json::Value val2 = val1.get(memberList[i].c_str(), Json::nullValue);
			if (val2 != Json::nullValue)
			{
				value = val2.asInt();
            LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettings-member name is %s,value is %d", memberList[i].c_str(), value);
				std::unordered_map < std::string, double > ::const_iterator ctr = mRateMap.find(memberList[i]);
				if (ctr != mRateMap.end())
					mRateTimeMap.insert(std::make_pair(ctr->second, value));
			}
		}
		if (value < 0)
		{
			break;
		}
	}

    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettings-size of RateTimeMap is %d", mRateTimeMap.size());

	for (std::map<double, int>::const_iterator itr = mRateTimeMap.begin(); itr != mRateTimeMap.end(); itr++)
	{
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettings-key:%f,value:%d", itr->first, itr->second);
	}
}

void MediaPresSettings::initSeekSpeedSettingsfor_video()
{
    Json::Value val = config->GetConfigFileKey("IncrementalSpeedSettingForVideoTrack");
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettingsfor_video-size of json data is %d type of json data is %d", val.size(), val.type());
	std::vector<std::string> keyList;
	keyList.clear();
	int value = 0;

	for (unsigned int i = 0; i < val.size(); i++)
	{
		Json::Value val1 = val[i];
		std::vector<std::string> memberList;
		memberList = val1.getMemberNames();
		for (unsigned int i = 0; i < memberList.size(); i++)
		{
			Json::Value val2 = val1.get(memberList[i].c_str(), Json::nullValue);
			if (val2 != Json::nullValue)
			{
				value = val2.asInt();
            LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettingsfor_video-member name is %s,value is %d", memberList[i].c_str(), value);
				std::unordered_map < std::string, double > ::const_iterator ctr = mRateMap.find(memberList[i]);
				if (ctr != mRateMap.end())
					mRateTimeMapVideo.insert(std::make_pair(ctr->second, value));
			}
		}

		if (value < 0)
		{
			break;
		}
	}

    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettingsfor_video-size of RateTimeMap is %d", mRateTimeMap.size());

	for (std::map<double, int>::const_iterator itr = mRateTimeMapVideo.begin(); itr != mRateTimeMapVideo.end(); itr++)
	{
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettingsfor_video-key:%f,value:%d", itr->first, itr->second);
	}
}

void MediaPresSettings::initSeekSpeedSettingsForAud_Podcast()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettingsForAud_Podcast");
	Json::Value val = config->GetConfigFileKey("IncrementalSpeedSettingForAudioBook_Podcast");
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettingsForAud_Podcast-size of json data is %d", val.size());
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettingsForAud_Podcast-type of json data is %d", val.type());

	std::vector<std::string> keyList;
	keyList.clear();
	int value = 0;

	for (unsigned int i = 0; i < val.size(); i++)
	{
		Json::Value val1 = val[i];

		std::vector<std::string> memberList;
		memberList = val1.getMemberNames();
		for (unsigned int i = 0; i < memberList.size(); i++)
		{
			Json::Value val2 = val1.get(memberList[i].c_str(), Json::nullValue);
			if (val2 != Json::nullValue)
			{
				value = val2.asInt();
                LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettingsForAud_Podcast-member name is %s,value is %d", memberList[i].c_str(), value);
				std::unordered_map < std::string, double > ::const_iterator ctr = mRateMap.find(memberList[i]);
				if (ctr != mRateMap.end())
					mRateTimeMapForAud_Podcast.insert(std::make_pair(ctr->second, value));
			}
		}

		if (value < 0)
		{
			break;
		}
	}

    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettingsForAud_Podcast-size of RateTimeMap is %d", mRateTimeMapForAud_Podcast.size());

	for (std::map<double, int>::const_iterator itr = mRateTimeMapForAud_Podcast.begin(); itr != mRateTimeMapForAud_Podcast.end(); itr++)
	{
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSeekSpeedSettingsForAud_Podcast-key:%f,value:%d", itr->first, itr->second);
	}
}


void MediaPresSettings::initSourceList()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSourceList");
	Json::Value val = config->GetConfigFileKey("SupportedSources");
	std::string sourceName = "";
	for (unsigned int i = 0; i < val.size(); i++)
	{
		sourceName = val[i].asString();
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSourceList-sourceName=%s",sourceName.c_str());
		mSourceList.push_back(sourceName);
	}
}

std::list<std::string> MediaPresSettings::getSupportedSources()
{
	return mSourceList;
}

void MediaPresSettings::initSlots()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSlots");
	setDummyDevice();
	Json::Value val = config->GetConfigFileKey("SlotConfiguration");
	std::string type = "";
	std::unordered_map<std::string, MMPresTypes::eSlotType>::iterator itr;
	uint8_t slotNumber=0;
	MMPresTypes::eSlotType slotType;
	for (unsigned int i = 0; i < val.size(); i++)
	{
		type = (val[i]["SlotType"]).asString();
		itr = mSlotMap.find(type);
		if (itr != mSlotMap.end())
		{
			slotType=itr->second;
            LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSlots,slotType=%s available",type.c_str());
		}
		else
		{
            LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSlots,slotType=%s not available", type.c_str());
		}
		slotNumber = static_cast<uint8_t>(atoi(val[i]["SlotNumber"].toStyledString().c_str()));
        LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initSlots,slotNumber=%d,slotType=%s,slotType=%ld",slotNumber,type.c_str(),(int32_t)slotType);
		mSlotList.push_back(MMPresTypes::tSlot(slotNumber,slotType,MMPresTypes::eSlotState::SLOTSTATE_EJECTED,getDummyDevice()));
	}
}



void MediaPresSettings::setDummyDevice()
{
dummyDevice= MMPresTypes::tDevice(0u, MMPresTypes::eDeviceType::DEVICETYPE_UKNOWN, 0u, 0u, 0u,"",MMPresTypes::ContentInfoMap(),MMPresTypes::tBTStatus());
}

MMPresTypes::tDevice MediaPresSettings::getDummyDevice()
{
	return dummyDevice;
}

MMPresTypes::tSlotList MediaPresSettings::getSlots()
{
	return mSlotList;
}

void MediaPresSettings::setAMInteraction(bool status)
{
	mAMInteraction = status;
}
bool MediaPresSettings::getAMInteraction()
{
	return mAMInteraction;
}

void MediaPresSettings::initAMInteraction()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initAMInteraction");
	Json::Value val = config->GetConfigFileKey("AMInteraction");
	bool status = val.asBool();
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initAMInteraction-status=%d",status);
	setAMInteraction(status);
}

void MediaPresSettings::initBrowseHistoryEnumeration()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initBrowseHistoryEnumeration");
	Json::Value val = config->GetConfigFileKey("BrowseHistoryEnumerationForGotoPrevious");
	bool value = val.asBool();
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initBrowseHistoryEnumeration-value=%d", value);
	mBrowseHistoryEnumeration = value;
}

void MediaPresSettings::initAppleAuthenticationChipFunctionalCheck()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initAppleAuthenticationChipFunctionalCheck");
	Json::Value val = config->GetConfigFileKey("AppleAuthenticationChipFunctionalCheck");
	bool value = val.asBool();
	LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initAppleAuthenticationChipFunctionalCheck-value=%d", value);
	mAppleAuthenticationChipFunctionalCheck = value;
}

void MediaPresSettings::initGetAttributeUsageForCount()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initGetAttributeUsageForCount");
	Json::Value val = config->GetConfigFileKey("GetAttributeUsageForCount");
	bool value = val.asBool();
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initGetAttributeUsageForCount-value=%d", value);
	mGetAttributeUsageForCount = value;
}

std::map<double, int> MediaPresSettings::getRateTimeMap()
{
	return mRateTimeMap;
}

std::map<double, int> MediaPresSettings::getRateTimeMapForVideo()
{
	return mRateTimeMapVideo;
}

std::map<double, int> MediaPresSettings::getRateTimeMapForAud_Podcast()
{
	return mRateTimeMapForAud_Podcast;
}

bool MediaPresSettings::getBrowseHistoryEnumeration()
{
	return mBrowseHistoryEnumeration;
}

bool MediaPresSettings::getAppleAuthenticationChipFunctionalCheck()
{
	return mAppleAuthenticationChipFunctionalCheck;
}

bool MediaPresSettings::getAttributeUsageForCount()
{
	return mGetAttributeUsageForCount;
}

void MediaPresSettings::initGenresCategoryName()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initGenresCategoryName");
	Json::Value val = config->GetConfigFileKey("GenresCategoryName");
	if (val != Json::nullValue)
	{
		std::string value = val.asString();
		mGenresCategoryName = value;
	}
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initGenresCategoryName-mGenresCategoryName=%s", mGenresCategoryName.c_str());
}

std::string MediaPresSettings::getGenresCategoryName()
{
	return mGenresCategoryName;
}

void MediaPresSettings::initRepeatSettings()
{
	mRepeatOffSameAsRepeatAllForUSB = false;
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initRepeatSettings");

	Json::Value val = config->GetConfigFileKey("RepeatOffSameAsRepeatAllForUSB");
	if (val != Json::nullValue)
	{
		bool value = val.asBool();
		mRepeatOffSameAsRepeatAllForUSB = value;
	}
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initRepeatSettings-mRepeatOffSameAsRepeatAllForUSB=%d", mRepeatOffSameAsRepeatAllForUSB);
}

bool MediaPresSettings::isRepeatOffSameAsRepeatAllForUSB()
{
	return mRepeatOffSameAsRepeatAllForUSB;
}

void MediaPresSettings::initDequeAllUsage()
{
	mIsDequeAllReqdForSourceSwitch = false;
	Json::Value val = config->GetConfigFileKey("DequeueAllReqdForSourceSwitch");
	if (val != Json::nullValue)
	{
		bool value = val.asBool();
		mIsDequeAllReqdForSourceSwitch = value;
	}
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initDequeAllUsage-mIsDequeAllReqdForSourceSwitch=%d", mIsDequeAllReqdForSourceSwitch);
}

bool MediaPresSettings::isDequeAllReqdForSourceSwitch()
{
	return mIsDequeAllReqdForSourceSwitch;
}

void MediaPresSettings::initContainerNameMapping()
{
  mContainerNameMapping = {
		{ MMPresTypes::eMetadataType::METADATATYPE_TITLE,"songs" }
		,{ MMPresTypes::eMetadataType::METADATATYPE_ARTIST,"artists" }
		,{ MMPresTypes::eMetadataType::METADATATYPE_ALBUM, "albums" }
		,{ MMPresTypes::eMetadataType::METADATATYPE_GENRE,"genres" }
		,{ MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST,"playlists" }
		,{ MMPresTypes::eMetadataType::METADATATYPE_COMPOSER, "composers" }
		,{ MMPresTypes::eMetadataType::METADATATYPE_FOLDER,"folder" }
		,{ MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK,"audiobooks" }
		,{ MMPresTypes::eMetadataType::METADTATATYPE_PODCAST,"podcasts" }
	    ,{ MMPresTypes::eMetadataType::METADATATYPE_COMPILATION,"iTunesRadio" }
		,{ MMPresTypes::eMetadataType::METADATATYPE_VIDEO,"videos"}
		,{ MMPresTypes::eMetadataType::METADATATYPE_IMAGE,"images"}
	};
}

std::string MediaPresSettings::getContainerName(const MMPresTypes::eMetadataType& type)
{
	auto  it = mContainerNameMapping.begin();

	while (mContainerNameMapping.end() != it)
	{
		if (it->first == type)
		{
			return it->second;
		}
		it++;
	}
	return "";
}



/**
 * @brief MediaPresSettings::InitPersLibrary
 */
void MediaPresSettings::initPersLibrary()
{
    //Initializing Persistency
    LOG_INFO(LOG_CONTEXT_MEDIA,"MediaPresSettings::initPersLibrary...");
    pclInitLibrary("Media", PCL_SHUTDOWN_TYPE_NONE);
    LOG_INFO(LOG_CONTEXT_MEDIA,"MediaPresSettings::initPersLibrary.Done");
}

/**
 * @brief MediaPresSettings::DeinitPersLibrary
 */
void MediaPresSettings::deinitPersLibrary()
{
    //Initializing Persistency
    LOG_INFO(LOG_CONTEXT_MEDIA,"MediaPresSettings::deinitPersLibrary...");
    pclDeinitLibrary();
    LOG_INFO(LOG_CONTEXT_MEDIA,"MediaPresSettings::deinitPersLibrary.Done.");
}

void MediaPresSettings::initReadEolMediaIndexing()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "%s+++", __FUNCTION__);
    int result = 0;
    result =  ReadEOLData(ADJ_E2P_DIAG_EOL_DE07_HMI_MEDIA_INDEXING_ID, ADJ_E2P_DIAG_EOL_DE07_HMI_MEDIA_INDEXING_SIZE);
    setHMIindex(std::to_string(result).c_str());
    LOG_INFO(LOG_CONTEXT_MEDIA, "EOLHMIindex : %s +++", (std::to_string(result).c_str()));

}

void MediaPresSettings::initReadEolDefaultLanguage()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "%s+++", __FUNCTION__);
    int result = 0;
    result =  ReadEOLData(ADJ_E2P_DIAG_EOL_DE07_HMI_DEFAULT_LANGUAGE_ID, ADJ_E2P_DIAG_EOL_DE07_HMI_DEFAULT_LANGUAGE_SIZE);
    setDefaultLanguage(std::to_string(result).c_str());
    LOG_INFO(LOG_CONTEXT_MEDIA, "EolDefaultLanguage : %s +++", (std::to_string(result).c_str()));
}

void MediaPresSettings::initReadEolEXTCD()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "%s+++", __FUNCTION__);
    int result = 0;
    result =  ReadEOLData(ADJ_E2P_DIAG_EOL_DE00_AUDIO_EXT_CD_ID, ADJ_E2P_DIAG_EOL_DE00_AUDIO_EXT_CD_SIZE);
    setExtCD(std::to_string(result).c_str());
    LOG_INFO(LOG_CONTEXT_MEDIA, "EolEXTCD : %s +++", (std::to_string(result).c_str()));
}


void MediaPresSettings::setHMIindex(std::string hmiIndex)
{
    mHMIindex = hmiIndex;
}

v1::org::genivi::mediamanager::Indexer::IndexOption MediaPresSettings::getHMIindex()
{
    v1::org::genivi::mediamanager::Indexer::IndexOption index ;
    if((stoi(mHMIindex.c_str())== 0 ) || (stoi(mHMIindex.c_str())== 1))
        index = v1::org::genivi::mediamanager::Indexer::IndexOption::NONE;
    else if(stoi(mHMIindex.c_str())==2)
        index = v1::org::genivi::mediamanager::Indexer::IndexOption::DISABLE_HIERARCHY;
    LOG_INFO(LOG_CONTEXT_MEDIA, "%s+++ index = %d", __FUNCTION__,index);
    return index;
}

void MediaPresSettings::setHWConfigUSB(std::string usbConfig)
{
    mHWUSBConfig = usbConfig;
}

void MediaPresSettings::setVideoPlayBack(std::string playBack)
{
    //mVideoPlayBack = playBack;
}

void MediaPresSettings::setDefaultLanguage(std::string language)
{
    mDefaultLanguage = language;
}

void MediaPresSettings::setExtCD(std::string extCD)
{
    mExtCD = extCD;
}

void MediaPresSettings::initReadEolUSB()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "%s+++", __FUNCTION__);
    int result = 0;
    result =  ReadEOLData(ADJ_E2P_DIAG_EOL_DE08_HARDWARE_USB_ID, ADJ_E2P_DIAG_EOL_DE08_HARDWARE_USB_SIZE);
    setHWConfigUSB(std::to_string(result).c_str());
    LOG_INFO(LOG_CONTEXT_MEDIA, "EOLusb : %s +++", (std::to_string(result).c_str()));
}

void MediaPresSettings::initReadEolVideoPlayback()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "%s+++", __FUNCTION__);
	mIsVideoSupported = true;
    int result = 0;
    result =  ReadEOLData(ADJ_E2P_DIAG_EOL_DE07_HMI_VIDEO_PLAYBACK_ID, ADJ_E2P_DIAG_EOL_DE07_HMI_VIDEO_PLAYBACK_SIZE);
	LOG_INFO(LOG_CONTEXT_MEDIA, "EOLVideoPlayBack :: 0x%02x ", result);
    mVideoPlaybackSupport = result;
	int var = result & 0x0F;

    /*if (var == 0x00)
    {
        mIndexerProxy->setIndexOptionAsync(getHMIindex(),"DISABLE_VIDEO", std::bind(&MediaPresSettings::VideoPlayCallback, this, _1, _2));
    }*/
	if (var == 0x01)
	{
		mIsVideoSupported = false;
	}
	LOG_INFO(LOG_CONTEXT_MEDIA, "mIsVideoSupported %d var 0x%02x", mIsVideoSupported,  var);

}
int MediaPresSettings::getVideoPlaybackSupport()
{
    return mVideoPlaybackSupport;
}
bool MediaPresSettings::getIsVideoSupported()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "getIsVideoSupported %d ", mIsVideoSupported);
	return mIsVideoSupported;
}


int MediaPresSettings::ReadEOLData(unsigned long sysSetID, unsigned long sysIDSize)
{
    LOG_INFO(LOG_CONTEXT_MEDIA,"%s",(__FUNCTION__));

    char id[4];
    char buf_byte[1]="";
    int result = 0;

    id[0] = (sysSetID >> 24) & 0xFF;
    id[1] = (sysSetID >> 16) & 0xFF;
    id[2] = (sysSetID >> 8) & 0xFF;
    id[3] = sysSetID & 0xFF;

    pclKeyReadData(PCL_LDBID_EOL, (const char *)id, PCL_STD_USER_NO, PCL_STD_SEAT_NO, (unsigned char*)buf_byte, sysIDSize);

    result = buf_byte[0];

    LOG_INFO(LOG_CONTEXT_MEDIA,"EOL read is %s", (std::to_string(result).c_str()));

    return result;
}

void MediaPresSettings::initPauseVideoPlaybackDuringBrowse()
{
	Json::Value val = config->GetConfigFileKey("PauseVideoPlaybackDuringBrowse");
	bool value = val.asBool();

	LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initPauseVideoPlaybackDuringBrowse - value = %d", value);

	mPauseVideoPlaybackDuringBrowse = value;
}

bool MediaPresSettings::getPauseVideoPlaybackDuringBrowse()
{
    return mPauseVideoPlaybackDuringBrowse;
}

void MediaPresSettings::initSingleCtageoryListForBrowse()
{
	mIsSingleCategoryListForBrowse = false;
	Json::Value val = config->GetConfigFileKey("IsSingleCategoryListForBrowse");
	if (val != Json::nullValue)
	{
		bool value = val.asBool();
		mIsSingleCategoryListForBrowse = value;
	}
    LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initIsGstreamer-mIsSingleCategoryListForBrowse=%d", mIsSingleCategoryListForBrowse);
}

bool MediaPresSettings::isSingleCategoryListForBrowse()
{
	return mIsSingleCategoryListForBrowse;
}

void MediaPresSettings::reIndexing(uint32_t _id, IndexerProxyBase::ReIndexingAsyncCallback callback)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaPresSettings::reIndexing- _id = %d",_id);
    /* call to reIndexingAsync of CoC media */
    mIndexerProxy->reIndexingAsync(_id, callback);
}

void MediaPresSettings::removeDatabse(uint32_t _id)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "<<< MediaBrowseEngine::removeDatabse- _id = %d",_id);
    /* call to removeDatabaseAsync of CoC media */
    mIndexerProxy->removeDatabaseAsync(_id, std::bind(&MediaPresSettings::removeDatabaseCallback, this, _1, _2));
}

void MediaPresSettings::setAllEOLData()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPresSettings::%s",__FUNCTION__);

    if(mIndexerProxyStatus == CommonAPI::AvailabilityStatus::AVAILABLE)
    {
        /*Based on EOL enable and Disableling the HMI indexing*/
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPresSettings::HMIindex = %d ",getHMIindex());
#if 0
        //mIndexerProxy->setIndexOptionAsync(Indexer::IndexOption::NONE,"AUDIO_SKIP_METADATA", std::bind(&MediaPresSettings::IndexOptionCallback, this, _1, _2));
        if(getHMIindex() == Indexer::IndexOption::DISABLE_HIERARCHY)
        {
           mIndexerProxy->setIndexOptionAsync(Indexer::IndexOption::NONE,"AUDIO_SKIP_METADATA", std::bind(&MediaPresSettings::IndexOptionCallback, this, _1, _2));
        }

        /*Based on EOL enable and Disableling the Video indexing*/
        //int var = getVideoPlaybackSupport() & 0x0F;
        //if ((getVideoPlaybackSupport() & 0x0F) == 0x01)
        {
            mIndexerProxy->setIndexOptionAsync(Indexer::IndexOption::NONE,"DISABLE_VIDEO", std::bind(&MediaPresSettings::VideoPlayCallback, this, _1, _2));
        }
        //LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPresSettings::VideoPlay = %x ",var);
#endif
    }
}

void MediaPresSettings::removeDatabaseCallback(const CommonAPI::CallStatus& stat, const Indexer::IndexerError& err)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPresSettings::removeDatabaseCallback: status=%d, error=%d", (int)(stat), (int)err);
}

void MediaPresSettings::IndexOptionCallback(const CommonAPI::CallStatus& stat, const Indexer::IndexerError& err)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPresSettings::IndexOptionCallback: status=%d, error=%d", (int)(stat), (int)err);
}

void MediaPresSettings::VideoPlayCallback(const CommonAPI::CallStatus& stat, const Indexer::IndexerError& err)
{
    LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPresSettings::VideoPlayCallback: status=%d, error=%d", (int)(stat), (int)err);
}

void MediaPresSettings::initElevatorBarHelperLetterTimeout()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initElevatorBarHelperLetterTimeout");

	Json::Value val = config->GetConfigFileKey("ElevatorBarHelperLetterTimeout");

	/* timeout value from config file is in seconds.
	 * Default value is 2 second and range is 10 sec
	 * It can be configured by 0.5 sec
	 * If value provided from config file is more than 10 sec then use default value as 2 sec */
    float timeout = val.asFloat();
    if(timeout > TIMEOUT_LIMIT)
    {
    	LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initElevatorBarHelperLetterTimeout - value from config = %f", timeout);
    	timeout = TIMEOUT_DEFAULT;
    }

	LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresSettings::initElevatorBarHelperLetterTimeout - value = %f", timeout);

	mElevatorBarHelperLetterTimeout = timeout;
}

float MediaPresSettings::getElevatorBarHelperLetterTimeout()
{
   return mElevatorBarHelperLetterTimeout;
}

bool MediaPresSettings::is_valid_utf8(const char * string)
{
     LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d  string name : %s:", __FUNCTION__, __LINE__, string  );
     
    if (!string)
        return true;

    const unsigned char * bytes = (const unsigned char *)string;
    unsigned int cp;
    int num;

    while (*bytes != 0x00)
    {
        if ((*bytes & 0x80) == 0x00)
        {
            // U+0000 to U+007F 
            cp = (*bytes & 0x7F);
            num = 1;
        }
        else if ((*bytes & 0xE0) == 0xC0)
        {
            // U+0080 to U+07FF 
            cp = (*bytes & 0x1F);
            num = 2;
        }
        else if ((*bytes & 0xF0) == 0xE0)
        {
            // U+0800 to U+FFFF 
            cp = (*bytes & 0x0F);
            num = 3;
        }
        else if ((*bytes & 0xF8) == 0xF0)
        {
            // U+10000 to U+10FFFF 
            cp = (*bytes & 0x07);
            num = 4;
        }
        else
            return false;

        bytes += 1;
        for (int i = 1; i < num; ++i)
        {
            if ((*bytes & 0xC0) != 0x80)
                return false;
            cp = (cp << 6) | (*bytes & 0x3F);
            bytes += 1;
        }

        if ((cp > 0x10FFFF) ||
            ((cp >= 0xD800) && (cp <= 0xDFFF)) ||
            ((cp <= 0x007F) && (num != 1)) ||
            ((cp >= 0x0080) && (cp <= 0x07FF) && (num != 2)) ||
            ((cp >= 0x0800) && (cp <= 0xFFFF) && (num != 3)) ||
            ((cp >= 0x10000) && (cp <= 0x1FFFFF) && (num != 4)))
            return false;
    }

    return true;
}

void MediaPresSettings::initPluginEvents()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPresSettings::initPluginEvents ");

	/* subscribe to displayLanguage status event from media pres plugin */
	PresEventAbstraction::getInstance()->subscribe<eventDisplayLanguageStatus>(REQ_DISPLAYLANGUAGE, std::bind(&MediaPresSettings::displayLanguageStatusEvent, this, std::placeholders::_1));
}

void MediaPresSettings::displayLanguageStatusEvent(const std::string& displayLanguage)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPresSettings::displayLanguageStatusEvent, displayLanguage = %s", displayLanguage.c_str());

    std::string mediaLanguage = "";
	FacadeHelper<MediaPlaybackEngine> mPlayEngine;

    auto it = mVehicleSettingLangToMediaLang.find(displayLanguage);
    if(it != mVehicleSettingLangToMediaLang.end())
    {
        mediaLanguage = it->second;
        LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPresSettings::displayLanguageStatusEvent, found mediaLanguage = %s", (it->second).c_str());
    }
    else
    {
    	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPresSettings::displayLanguageStatusEvent, Error: mediaLanguage is not found");
    }

    mPlayEngine->setMediaLocalName(mPlayEngine.mClientID, mediaLanguage, std::bind(&MediaPresSettings::MediaLocaleNameCallback, this, _1, _2));
}

void MediaPresSettings::MediaLocaleNameCallback(const CommonAPI::CallStatus& status, const std::string& value)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, ">>> MediaPresSettings::MediaLocaleNameCallback, callStatus = %d, value = %s",status, value.c_str());

}
