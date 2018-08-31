/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            MediaPresSettings.h
* @ingroup         mediapresctrl
* @author          sriram srivatsan (sriram.srivatsan@harman.com)
* @brief           This obtains the project specific settings data from the mediapres.cfg file.
*/

#ifndef _MEDIA_PRES_SETINGS_H
#define _MEDIA_PRES_SETINGS_H

#include<core.h>
#include<config/ConfigParser.h>
#include "persistence_client_library_file.h"
#include "persistence_client_library_key.h"
#include "persistence_client_library.h"
#include <SysetIds.h>
#include <iostream>

using namespace v1::org::genivi::mediamanager;
using namespace v3::com::harman::mmpres;

#define TIMEOUT_LIMIT 10.0
#define TIMEOUT_DEFAULT 2.0

class ConfigParser;

class MediaPresSettings
{
public:
	MMPresTypes::tSlotList getSlots();
    void availibility(const CommonAPI::AvailabilityStatus& avail);
	void initSourceList();
	std::list<std::string> getSupportedSources();
	bool getAMInteraction();
	std::map<double, int> getRateTimeMap();
	std::map<double, int> getRateTimeMapForVideo();
	std::map<double, int> getRateTimeMapForAud_Podcast();
	bool getBrowseHistoryEnumeration();
	bool getAppleAuthenticationChipFunctionalCheck();
	bool getAttributeUsageForCount();
	std::string getGenresCategoryName();
	bool isRepeatOffSameAsRepeatAllForUSB();
	bool isDequeAllReqdForSourceSwitch();
	std::string getContainerName(const MMPresTypes::eMetadataType&);
    void initPersLibrary();
    void deinitPersLibrary();
	bool isSingleCategoryListForBrowse();
	void initSingleCtageoryListForBrowse();
	bool getPauseVideoPlaybackDuringBrowse();
	bool getIsVideoSupported();
    v1::org::genivi::mediamanager::Indexer::IndexOption getHMIindex();
    void reIndexing(uint32_t _id, IndexerProxyBase::ReIndexingAsyncCallback callback);
    void removeDatabse(uint32_t _id);
    int getVideoPlaybackSupport();
    float getElevatorBarHelperLetterTimeout();
    bool is_valid_utf8(const char * string);

    void displayLanguageStatusEvent(const std::string& displayLanguage);
    void MediaLocaleNameCallback(const CommonAPI::CallStatus& status, const std::string& value);
    string getInvalidString () {        return              invalidString;      }

private:
	friend class FacadeHelper<MediaPresSettings>;
    CommonAPI::AvailabilityStatus mIndexerProxyStatus;
    CommonAPIClientAbstraction<IndexerProxy> mIndexerProxy;
    string invalidString = "[][][][][][][]";
	uint32_t registerClient()
	{
		return 0u;
	}
	void deregisterClient(uint32_t ClientID)
	{

	}
	MediaPresSettings();
	~MediaPresSettings();
	FacadeHelper<ConfigParser> config;
	static std::unordered_map<std::string, MMPresTypes::eSlotType> mSlotMap;
	MMPresTypes::tSlotList mSlotList;
	void initSlots();
	void setDummyDevice();
	MMPresTypes::tDevice getDummyDevice();
	MMPresTypes::tDevice dummyDevice;
	std::list<std::string> mSourceList;
	bool mAMInteraction;
	void setAMInteraction(bool status);
	void initAMInteraction();
	void initSeekSpeedSettings();
	void initSeekSpeedSettingsfor_video();
	void initSeekSpeedSettingsForAud_Podcast();
	void initBrowseHistoryEnumeration();
	void initAppleAuthenticationChipFunctionalCheck();
	void initGetAttributeUsageForCount();
	void initGenresCategoryName();
	void initRepeatSettings();
	void initDequeAllUsage();
	void initContainerNameMapping();
    void initReadEolMediaIndexing();
    void initReadEolUSB();
    void initReadEolVideoPlayback();
    void initReadEolDefaultLanguage();
    void initReadEolEXTCD();
    void setHMIindex(std::string);
    void setHWConfigUSB(std::string);
    void setVideoPlayBack(std::string );
    void setDefaultLanguage(std::string);
    void setExtCD(std::string );
    int ReadEOLData(unsigned long sysSetID, unsigned long sysIDSize);
	void initPauseVideoPlaybackDuringBrowse();
	void initCategoryListForBrowse();
    void setAllEOLData();
    void initElevatorBarHelperLetterTimeout();

	static std::unordered_map<std::string, double> mRateMap;
	std::map<double, int> mRateTimeMap;
	std::map<double, int> mRateTimeMapVideo;
	std::map<double, int> mRateTimeMapForAud_Podcast;
	bool mBrowseHistoryEnumeration;
	bool mAppleAuthenticationChipFunctionalCheck;
	bool mGetAttributeUsageForCount;
	bool mPauseVideoPlaybackDuringBrowse;
	std::string mGenresCategoryName;
	bool mRepeatOffSameAsRepeatAllForUSB;
	bool mIsDequeAllReqdForSourceSwitch;
	std::unordered_map<MMPresTypes::eMetadataType, std::string> mContainerNameMapping;

    std::string mHMIindex;
    std::string mHWUSBConfig;
	bool mIsVideoSupported;
    int mVideoPlaybackSupport;
    std::string mDefaultLanguage;
    std::string mExtCD;
	bool mIsSingleCategoryListForBrowse;
	float mElevatorBarHelperLetterTimeout;
	void initPluginEvents();
	static std::unordered_map<std::string, std::string> mVehicleSettingLangToMediaLang;

protected:
    void removeDatabaseCallback(const CommonAPI::CallStatus& stat, const Indexer::IndexerError& err);
    void IndexOptionCallback(const CommonAPI::CallStatus& stat, const Indexer::IndexerError& err);
    void VideoPlayCallback(const CommonAPI::CallStatus& stat, const Indexer::IndexerError& err);


};
#endif
