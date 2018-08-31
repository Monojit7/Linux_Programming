/*********************************************************************
* Project         Media Presentation Control
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/*
* @file            main.cpp
* @ingroup         mediapresctrl
* @author          sriram srivatsan (sriram.srivatsan@harman.com)
* @brief           This is the test client for tetsing media pres API..
*/


#include <v1/com/harman/mmpres/MediaPlayerProxy.hpp>
#include <v1/com/harman/mmpres/MediaManagerProxy.hpp>
#include <v3/com/harman/mmpres/MediaBrowserProxy.hpp>
#include<v1/org/genivi/am/routinginterface/RoutingControlProxy.hpp>
#include <CommonAPI/CommonAPI.hpp>
#include<thread>
#include<future>
#include<signal.h>
#include <stdint.h>

using namespace v1::com::harman::mmpres;
using namespace v3::com::harman::mmpres;
using namespace std;
using namespace std::placeholders;
using namespace v1::org::genivi::am::routinginterface;

#define EXIT_TESTCLIENT 0
static bool quit = false;

void sigHandler(int sig)
{
	//quit = true;
}

class MediaPresTestCLI
{
public:

	MediaPresTestCLI();
	~MediaPresTestCLI();
	void resume();
	void pause();
	void next();
	void previous();
	void setRepeat();
	void setShuffle();
	void fastForward();
	void fastReverse();
	void getNowplayingAlbumartPath();
	void getSlots();
	void seekToTime();
	static MediaPresTestCLI& getInstance();
	void initializeRepeatmap();
	void initializeShuffleMap();
	void initializePlaystateMap();
	void initializeSpeedMap();
	void initializeArtworkMap();
	void initializeFuncMap();
	int getFuncMapSize();
	void displayFuncMap();
	void callFunc(int input);

	void openSession()
	{
		CommonAPI::CallStatus internalCallStatus;
		uint32_t sessionID;
		MMPresTypes::eCallResult result;

		browser->openSession(internalCallStatus, sessionID, result);
		if (CommonAPI::CallStatus::SUCCESS == internalCallStatus)
		{
			cout << "Successfully Opened Session : " << sessionID << endl;
		}
		else
		{
			cout << "Failed to open Session-CallStatus:" << (int)internalCallStatus << ",callResult:" << (int)result << endl;
		}
	}

	void setSessionQuery()
	{
		uint32_t sessionID = 0u;
		int slotId= 0;
		uint64_t FolderID = 0u;
		int key = -1;
		uint64_t filterIDMap_value = 0;
		string slot;
		uint32_t contentMask=0,titleType=0;

		int type = 0;
		int metadataType = 0;
		cout << "Enter Session ID : " << endl;
		cin >> sessionID;
		cout << "Enter slot ID : " << endl;
		cin >> slot;
		slotId = atoi(slot.c_str());
		cout << " Enter session query type,type 1 for file system browse and 2 for metadata browse,3 for NowPlayingList browse,4 for category browse,5 for highest rated,6 for recently played" << endl;
		cin >> type;
		// uint32_t QueryID = 0u;
		// uint64_t NumResults = 0u;
		// CommonAPI::CallStatus internalCallStatus;

		MMPresTypes::FilterIDMap map;
		MMPresTypes::FilterMap filters;
		MMPresTypes::eCallResult result;
		MMPresTypes::eSessionQueryTypes queryType;
		MMPresTypes::eMetadataType resultType, filterIDMap_key;
		if (type == 1)
		{
			cout << " Enter Folder ID (0 For Root)" << endl;
			cin >> FolderID;
			
		    map[MMPresTypes::eMetadataType::METADATATYPE_FOLDER] = FolderID;
			queryType = MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE;
			resultType = MMPresTypes::eMetadataType::METADATATYPE_FILE;
		}
		else if (type == 2)
		{
			queryType = MMPresTypes::eSessionQueryTypes::SESSION_QUERY_METADATA_BROWSE;
			cout << "enter the filterIDMap (metadata type for key and folderid/containerid as value) (press 0 for key to quit)" << endl;
			for (;;)
			{
				cout << "enter the key (press 1 for title, 2 for album, 3 for artist, 4 for genre, 5 for composer, 6 for playlist, 7 for audiobook, 8 for chapter, 9 for podcast and 10 for episode,11 for image,12 for video)" << endl;
				cin >> key;
				if (key == 1)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADATATYPE_TITLE;
				}
				else if (key == 2)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADATATYPE_ALBUM;
				}
				else if (key == 3)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADATATYPE_ARTIST;
				}
				else if (key == 4)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADATATYPE_GENRE;
				}
				else if (key == 5)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADATATYPE_COMPOSER;
				}
				else if (key == 6)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST;
				}
				else if (key == 7)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK;
				}
				else if (key == 8)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADTATATYPE_CHAPTER;
				}
				else if (key == 9)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADTATATYPE_PODCAST;
				}
				else if (key == 10)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADTATATYPE_EPISODE;
				}
				else if (key == 11)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADATATYPE_IMAGE;
				}
				else if (key == 12)
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADATATYPE_VIDEO;
				}
				else
				{
					filterIDMap_key = MMPresTypes::eMetadataType::METADATATYPE_TITLE;
				}

				if (key == 0)
				{
					break;
				}
				
				cout << "enter value" << endl;
				cin >> filterIDMap_value;

			/*	if (filterIDMap_value == 0)
				{
					break;
				}*/
				map[filterIDMap_key] = filterIDMap_value;
			}

			cout << " Enter session query result type: press 1 for title,2 for album,3 for artist,4 for genre,5 for composer,6 for playlist,7 for audiobook,8 for chapter,9 for podcast and 10 for episode,11 for image,12 for video" << endl;
			cin >> metadataType;

			if (metadataType == 1)
			{
				resultType = MMPresTypes::eMetadataType::METADATATYPE_TITLE;
			}
			else if (metadataType == 2)
			{
				resultType = MMPresTypes::eMetadataType::METADATATYPE_ALBUM;
			}
			else if (metadataType == 3)
			{
				resultType = MMPresTypes::eMetadataType::METADATATYPE_ARTIST;
			}
			else if (metadataType == 4)
			{
				resultType = MMPresTypes::eMetadataType::METADATATYPE_GENRE;
			}
			else if (metadataType == 5)
			{
				resultType = MMPresTypes::eMetadataType::METADATATYPE_COMPOSER;
			}
			else if (metadataType == 6)
			{
				resultType = MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST;
			}
			else if (metadataType == 7)
			{
				resultType = MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK;
			}
			else if (metadataType == 8)
			{
				resultType = MMPresTypes::eMetadataType::METADTATATYPE_CHAPTER;
			}
			else if (metadataType == 9)
			{
				resultType = MMPresTypes::eMetadataType::METADTATATYPE_PODCAST;
			}
			else if (metadataType == 10)
			{
				resultType = MMPresTypes::eMetadataType::METADTATATYPE_EPISODE;
			}
			else if (metadataType == 11)
			{
				resultType = MMPresTypes::eMetadataType::METADATATYPE_IMAGE;
			}
			else if (metadataType == 12)
			{
				resultType = MMPresTypes::eMetadataType::METADATATYPE_VIDEO;
			}
			else
			{
				resultType = MMPresTypes::eMetadataType::METADATATYPE_TITLE;
			}

		}
		else if (type == 3)
		{
			queryType = MMPresTypes::eSessionQueryTypes::SESSION_QUERY_NOWPLAYINGLIST_BROWSE;
		}
		else if(type==4)
		{
			queryType = MMPresTypes::eSessionQueryTypes::SESSION_QUERY_CATEGORY_BROWSE;
		}
		else if (type == 5)
		{
			queryType = MMPresTypes::eSessionQueryTypes::SESSION_QUERY_HIGHESTRATED;
		}
		else if (type == 6)
		{
			queryType = MMPresTypes::eSessionQueryTypes::SESSION_QUERY_RECENTLYPLAYED;
		}
		else
		{

		}

		cout << " Enter titleType (1 for audio,2 for video,3 for image" << endl;
		cin >> titleType;

		if (titleType == 1)
		{
			contentMask = MMPresTypes::eTitleContentType::TITLETYPE_AUDIO;
		}
		else if (titleType == 2)
		{
			contentMask = MMPresTypes::eTitleContentType::TITLETYPE_VIDEO;
		}
		else if (titleType == 3)
		{
			contentMask = MMPresTypes::eTitleContentType::TITLETYPE_IMAGE;
		}
		else
		{
			contentMask = MMPresTypes::eTitleContentType::TITLETYPE_AUDIO;
		}
		

		// uint32_t SessQueryID;
		//	browser->setSessionQuery(sessionID, mediumID, map, filters, MMPresTypes::eMetadataType::METADATATYPE_FILE, 0u, 0u, MMPresTypes::eSessionQueryTypes::SESSION_QUERY_FS_BROWSE, false, true, 0u, internalCallStatus, result, NumResults, SessQueryID);
		//	for (int i = 0; i <= 5; i++)
		{
			//	browser->setSessionQuery(sessionID, mediumID, map, filters, resultType, 0u, 0u, queryType, false, true, 0u, internalCallStatus, result, NumResults, SessQueryID);
			browser->setSessionQueryAsync(sessionID,(uint8_t)slotId, map, filters, resultType, 0u, 0u, queryType, false,false, contentMask, std::bind(&MediaPresTestCLI::setSessionQueryCallback, this, _1, _2, _3, _4,_5));
			//	MMPresTypes::eCallResult result;
			//	cout << "setSessionQuery-Query ID : " << SessQueryID << " , NumResults : " << NumResults <<" callResult:"<<(int32_t)result<< endl;
		}
	}

	void setSessionQueryCallback(const CommonAPI::CallStatus& status, const MMPresTypes::eCallResult& result, const uint64_t& numResults, const uint32_t& SessQueryID,const string& breadCrumbPath)
	{
		cout << "setSessionQueryCallback-CallStatus:" << (int)status << "Query ID : " << SessQueryID << " , NumResults : " << numResults << " callResult:" << (int32_t)result << endl;
		cout << "setSessionQueryCallback-breadcrumbpath:" << breadCrumbPath << endl;
	}

	void jumpToSessionQuery()
	{
		uint32_t sessionID = 0u;
		uint32_t sessionQueryID = 0u;

		cout << "Enter Session ID : " << endl;
		cin >> sessionID;

		cout << "Enter SessionQueryID : " << endl;
		cin >> sessionQueryID;
		
		browser->jumpToSessionQueryAsync(sessionID, sessionQueryID, std::bind(&MediaPresTestCLI::jumpToSessionQueryCallback, this,_1,_2,_3,_4));
	}

	void jumpToSessionQueryCallback(const CommonAPI::CallStatus& status, const MMPresTypes::eCallResult& result, const uint64_t& numResults,const string& breadCrumbPath)
	{
		cout << "jumpToSessionQueryCallback-CallStatus:" << (int)status <<" , NumResults : " << numResults << " callResult:" << (int32_t)result << endl;
		cout << "jumpToSessionQueryCallback-breadCrumbPath:" << breadCrumbPath << endl;
	}

	void setSessionQueryWindow()
	{
		uint32_t sessID, SessQID;
		uint64_t startIndex, numItems;
		MMPresTypes::eCallResult result;
		// CommonAPI::CallStatus internalCallStatus;

		cout << "Enter Session ID and Session Query ID ";
		cin >> sessID;
		cin >> SessQID;

		cout << endl << "Enter start index and number of items ";
		cin >> startIndex;
		cin >> numItems;

		//for (int i = 0; i <= 15; i++)
		{

			//browser->setSessionQueryWindow(sessID, SessQID, startIndex, numItems, internalCallStatus, result);
			browser->setSessionQueryWindowAsync(sessID, SessQID, startIndex, numItems, std::bind(&MediaPresTestCLI::setSessionQueryWindowCallback, this, _1, _2));
			//	cout << "setSessionQueryWindow-CallStatus : " << (int)internalCallStatus << " , callResult : " << (int)result << endl;
		}
	}

	void setSessionQueryWindowCallback(const CommonAPI::CallStatus& status, const MMPresTypes::eCallResult& result)
	{
		cout << "setSessionQueryWindowCallback-CallStatus: " << (int)status << " callResult:" << (int32_t)result << endl;
	}

	void gotoPreviousSessionQuery()
	{
		uint32_t sessionID = 0u;
		cout << "Enter Session ID : " << endl;
		cin >> sessionID;

		// uint32_t QueryID = 0u;
		// uint64_t NumResults = 0u;
		// CommonAPI::CallStatus internalCallStatus;

		MMPresTypes::eCallResult result;
		// uint32_t SessQueryID;
	//	browser->gotoPreviousSessionQuery(sessionID, internalCallStatus, result, NumResults, SessQueryID);
		browser->gotoPreviousSessionQueryAsync(sessionID, std::bind(&MediaPresTestCLI::gotoPreviousSessionQueryCallback,this,_1,_2,_3,_4,_5,_6));
		//cout << "gotoPreviousSessionQuery-Query ID : " << SessQueryID << " , NumResults : " << NumResults << endl;
	}

	void gotoPreviousSessionQueryCallback(const CommonAPI::CallStatus& status, const MMPresTypes::eCallResult& result, const uint64_t& numResults, const uint32_t& SessQueryID,const  MMPresTypes::ePrevBrowseLevel prevBrowseLevel,const string& breadCrumbPath)
	{
		cout << "gotoPreviousSessionQueryCallback-CallStatus:" << (int)status << "Query ID : " << SessQueryID << " , NumResults : " << numResults << " callResult:" << (int32_t)result << endl;
		cout << "gotoPreviousSessionQueryCallback-breadcrumbpath:" << breadCrumbPath << " prev browsee level:" << (int)(prevBrowseLevel) << endl;
	}

	void playSession()
	{
		uint32_t sessionID = 0u;
		uint64_t fileID = 0u;
		cout << "Enter Session ID : " << endl;
		cin >> sessionID;
		cout << " Enter file ID" << endl;
		cin >> fileID;

		MMPresTypes::FilterIDMap map;
		map[MMPresTypes::eMetadataType::METADATATYPE_FILE] = fileID;

		CommonAPI::CallStatus internalCallStatus;
		MMPresTypes::eCallResult result;

		browser->playSession(sessionID, map, internalCallStatus, result);

		cout << "callStatus : " << (int)internalCallStatus << " , result: " << (int)result << endl;
	}

	void sessionQueryUpdate(const uint32_t & sessID, const uint32_t & sessQId, const uint64_t & numResults)
	{
		cout << "Session Query Update :" << sessID << " Session Query ID : " << sessQId << " Num Results : " << numResults << endl;
	}

	void sessionQueryWindowUpdate(const uint32_t & sessID, const uint32_t & sessionQID, const MMPresTypes::eQueryStatus & status, const uint64_t & startIndex, const uint64_t & numItems, const MMPresTypes::MetadataItemList & itemList, const bool & currentPlayingTrackIndex, const uint64_t & indexHighlight)
	{
		cout << "Session Query Window Update :" << sessID << " Session Query ID : " << sessionQID << " eQueryStatus:" << (int)status << " Num Results : " << numItems << endl;

		for (uint64_t i = 0; i < itemList.size(); i++)
		{
			cout << "Item " << (i + 1) << " details" << endl;
			cout << "id:" << itemList[i].getID() << endl;
			cout << "value:" << itemList[i].getValue() << endl;
			cout << "playable:" << itemList[i].getPlayable() << endl;
			cout << "browsable:" << itemList[i].getBrowsable() << endl;
			cout << "file system type:" << (int)(itemList[i].getFolderType()) << endl;
			cout << "content type:" << (int)(itemList[i].getContentType()) << endl;
			cout << "item type:" << (int)(itemList[i].getItemType()) << endl;
			
			cout << "metadata count map" << endl;
			MMPresTypes::MetadataCountMap countMap = itemList[i].getMetadataCountMap();
			auto it = countMap.begin();
			while (it != countMap.end())
			{
				cout << it->first << ":" << it->second << endl;
				it++;
			}

			cout << "metadata string map" << endl;
			MMPresTypes::MetadataStringMap map = itemList[i].getMetadataMap();
			auto it1 = map.begin();
			while (it1 != map.end())
			{
				cout << it1->first << ":" << it1->second << endl;
				it1++;
			}
		}

		cout << "WindowContainsCurrentlyPlayingTrack:" << currentPlayingTrackIndex << endl;
		cout << "ItemIndexToHighlight:" << indexHighlight << endl;
	}

	void asyncSetSourceState()
	{
		int id = 0, state = 0;
		static int reqHandle = 0;
		::v3::org::genivi::am_t::am_Handle_s sourceHandle;
		::v3::org::genivi::am_t::am_sourceID_t sourceId;
		::v3::org::genivi::am_t::am_SourceState_e sourceState;
		std::unordered_map<int, ::v3::org::genivi::am_t::am_SourceState_e>::const_iterator ctr;
		std::unordered_map<int, ::v3::org::genivi::am_t::am_sourceID_e>::const_iterator ctr1;
		cout << "Enter source Id (enter 55 for USB1,56 for USB2 & 65 for BTSA)" << endl;
		cin >> id;
		cout << "Enter source state that needs to be set (1 for SS_ON,2 for SS_OFF and 3 for SS_PAUSED)" << endl;
		cin >> state;

		ctr = sourceStateMap.find(state);
		ctr1 = sourceIDMap.find(id);

		if (ctr != sourceStateMap.end() && ctr1 != sourceIDMap.end())
		{
			sourceHandle.setHandleType(::v3::org::genivi::am_t::am_Handle_e::H_SETSOURCENOTIFICATION);
			sourceHandle.setHandle(++reqHandle);

			sourceId = ctr1->second;
			sourceState = ctr->second;

			routing->asyncSetSourceStateAsync(sourceHandle, sourceId, sourceState, std::bind(&MediaPresTestCLI::asyncSetSourceStateCallback, this));
		}
		else
		{
			cout << "wrong input parameters " << endl;
		}
	}

	void asyncSetSourceStateCallback()
	{
		cout << "asyncSetSourceStateCallback -" << endl;
	}

	void ackSetSourceStateCallback(const ::v3::org::genivi::am_t::am_Handle_s& reqHandle, const ::v3::org::genivi::am_t::am_Error_e& error)
	{
		cout << "ackSetSourceStateCallback - req handle is " << reqHandle.getHandle() << " and error is " << (int)error << endl;
	}

	void initSourceStateMap()
	{
		sourceStateMap.insert(std::make_pair(1, ::v3::org::genivi::am_t::am_SourceState_e::SS_ON));
		sourceStateMap.insert(std::make_pair(2, ::v3::org::genivi::am_t::am_SourceState_e::SS_OFF));
		sourceStateMap.insert(std::make_pair(3, ::v3::org::genivi::am_t::am_SourceState_e::SS_PAUSED));
	}

	void initSourceIDMap()
	{
		sourceIDMap.insert(std::make_pair(55, ::v3::org::genivi::am_t::am_sourceID_e::eSRC_USB1));
		sourceIDMap.insert(std::make_pair(56, ::v3::org::genivi::am_t::am_sourceID_e::eSRC_USB2));
		sourceIDMap.insert(std::make_pair(65, ::v3::org::genivi::am_t::am_sourceID_e::eSRC_BT_A2DP));
	}

	void closeSession()
	{
		CommonAPI::CallStatus internalCallStatus;
		uint32_t sessionID;
		MMPresTypes::eCallResult result;
		cout << "Enter sessionID " << endl;
		cin >> sessionID;
		browser->closeSession(sessionID, internalCallStatus, result);
		if (CommonAPI::CallStatus::SUCCESS == internalCallStatus && result == MMPresTypes::eCallResult::CALLRESULT_OK)
		{
			cout << "Successfully closed Session : " << sessionID << endl;
		}
		else
		{
			cout << "Failed to close SessionId:" << sessionID << "CallStatus:" << (int)internalCallStatus << " callResult:" << (int)result << endl;
		}
	}

	void cancelSession()
	{
		CommonAPI::CallStatus internalCallStatus;
		uint32_t sessionID;
		MMPresTypes::eCallResult result;
		cout << "Enter sessionID " << endl;
		cin >> sessionID;
		browser->cancelSession(sessionID, internalCallStatus, result);
		if (CommonAPI::CallStatus::SUCCESS == internalCallStatus && result == MMPresTypes::eCallResult::CALLRESULT_OK)
		{
			cout << "Successfully cancelled Session : " << sessionID << endl;
		}
		else
		{
			cout << "Failed to cancel SessionId:" << sessionID << "CallStatus:" << (int)internalCallStatus << " callResult:" << (int)result << endl;
		}
	}

	void slotUpdate(const ::v3::com::harman::mmpres::MMPresTypes::tSlot &slot)
	{
		cout << "Slot Update event" << endl;

		cout << "DeviceManager Slot information Start" << endl;
		cout << "Slot ID : " << (int)slot.getSlotId() << endl;
		cout << "Slot Type : " << slot.getSlotType() << endl;
		cout << "Slot State : " << slot.getSlotState() << endl;
		cout << "Device ID : " << slot.getDevice().getDeviceId() << endl;
		cout << "Device Name : " << slot.getDevice().getDeviceName() << endl;
		cout << "Device Type : " << slot.getDevice().getDeviceType() << endl;
		
		const MMPresTypes::ContentInfoMap& contentInfoMap = slot.getDevice().getContentInfo();
		auto it = contentInfoMap.begin();
		while (contentInfoMap.end() != it)
		{
			cout << "Title content type: " << it->first << " Count :  " << it->second << endl;
			it++;
		}
		cout << "DeviceManager Slot information end" << endl;

	}

	void setSpeed()
	{
		uint8_t slotId = 0;
		int mtype;
		MMPresTypes::eMetadataType type = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;
		MMPresTypes::ePlaybackSpeed playSpeed;
		int speed = 1;

		cout << "Enter slotId (0 if setting is for all devices)" << endl;
		cin >> slotId;
		cout << "Enter metadata type (0 for audiobook,1 for podcast, 2 for others)" << endl;
		cin >> mtype;

		if (mtype == 0)
		{
			type = MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK;
		}
		else if (mtype == 1)
		{
			type = MMPresTypes::eMetadataType::METADTATATYPE_PODCAST;
		}
		else
		{
			/* TODO: Yet to implement*/
		}


		cout << "Enter playspeed (0 for slower,1 for normal, 2 for faster)" << endl;
		cin >> speed;
		if (speed == 0)
		{
			playSpeed = MMPresTypes::ePlaybackSpeed::SLOWER;
		}
		else if (speed == 1)
		{
			playSpeed = MMPresTypes::ePlaybackSpeed::NORMAL;
		}
		else if(speed==2)
		{
			playSpeed = MMPresTypes::ePlaybackSpeed::FASTER;
		}
		else
		{
			speed = 1;
		}
		player->setSpeedAsync(slotId,type,playSpeed,std::bind(&MediaPresTestCLI::setSpeedCallback,this,_1,_2));
	}

	void setSpeedCallback(const CommonAPI::CallStatus& status,const MMPresTypes::eCallResult& result)
	{
		cout << "MediaPresTestCLI::setSpeedCallback-callstatus:"<<(int)status<<" callResult:"<<(int)result<<endl;
	}

	void getSpeed()
	{
		uint8_t slotId = 0;
		int mtype;
		MMPresTypes::eMetadataType type = MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;
		
		cout << "Enter slotId (0 if setting is for all devices)" << endl;
		cin >> slotId;
		cout << "Enter metadata type (0 for audiobook,1 for podcast, 2 for others)" << endl;
		cin >> mtype;

		if (mtype == 0)
		{
			type = MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK;
		}
		else if (mtype == 1)
		{
			type = MMPresTypes::eMetadataType::METADTATATYPE_PODCAST;
		}

		player->getSpeedAsync(slotId, type, std::bind(&MediaPresTestCLI::getSpeedCallback, this, _1, _2));

	}

	void getSpeedCallback(const CommonAPI::CallStatus & status,const MMPresTypes::ePlaybackSpeed& playSpeed)
	{
		cout << "MediaPresTestCLI::getSpeedCallback-callstatus:"<<(int)status<<" playSpeed:" << (int)playSpeed <<endl;
	}

	void getAdditionalDeviceInfo()
	{
		int slotId = 0;
		string slot;
		cout << "Enter slotId" << endl;
		cin >> slot;
		slotId = atoi(slot.c_str());
		deviceManager->getAdditionalDeviceInfoAsync((uint8_t)slotId, std::bind(&MediaPresTestCLI::getAdditionalDeviceInfoCallback, this, _1,_2));
	}

	void getAdditionalDeviceInfoCallback(const CommonAPI::CallStatus& status, const MMPresTypes::tAdditionalDeviceInfo& additonalDevInfo)
	{
		cout << "MediaPresTestCLI::getAdditionalDeviceInfoCallback-callstatus:" << (int)status << endl 
			<<" firmware:" << additonalDevInfo.getFirmware() << endl
			<< " protocol:" << additonalDevInfo .getProtocol() << endl
			<< " modelNumber:" << additonalDevInfo.getModelNumber() << endl
			<< " productName:" << additonalDevInfo.getProductName() << endl
			<< " serialNumber:" << additonalDevInfo .getSerialNumber() << endl
			<< " versionNumber:" << additonalDevInfo.getVersionNumber() << endl
			<< " vendorId:" << additonalDevInfo.getVendorId() << endl
			<< " productId:" << additonalDevInfo .getProductId() << endl
			<< " manufacturer:" << additonalDevInfo.getManufacturer() << endl
			<< " MAC:" << additonalDevInfo.getMAC() << endl	;
	}

private:
	std::shared_ptr<MediaPlayerProxy<>> player;
	std::shared_ptr<MediaManagerProxy<>> deviceManager;
	std::shared_ptr<MediaBrowserProxy<>> browser;
	std::shared_ptr<RoutingControlProxy<>>routing;
	std::shared_ptr<CommonAPI::Runtime> pRuntime;
	void resumeResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result);
	void pauseResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result);
	void nextResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result);
	void previousResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result);
	void setRepeatResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result);
	void setShuffleResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result);
	void fastForwardResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result);
	void fastReverseResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result);
	void seekToTimeResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result);
	void getNowplayingAlbumartPathResponse(const CommonAPI::CallStatus& status, const std::string& path, const uint8_t&);
	void getSlotsResponse(const CommonAPI::CallStatus& status, const MMPresTypes::tSlotList& slotList);
	void playStatusAttributeCallback(const MMPresTypes::ePlayState& playState);
	void trackStateAttributeCallback(const MMPresTypes::tTrackState& trackState);
	void currentPlayTimeAttributeCallback(const uint64_t& currTime);
	void totalPlayTimeAttributeCallback(const uint64_t& duration);
	void nowPlayingDataCallback(const MMPresTypes::tNowPlayingInfo& nowPlayingInfo);
	void repeatChangeCallback(const MMPresTypes::eRepeatMode& mode);
	void shuffleChangeCallback(const MMPresTypes::eShuffleMode& mode);
	void trackSeekSpeedCallback(const MMPresTypes::eTrackSeekSpeed& speed);
	void playerProxyAvailability(const CommonAPI::AvailabilityStatus& avail);
	void deviceManagerProxyAvailability(const CommonAPI::AvailabilityStatus& avail);
	void browserProxyAvailability(const CommonAPI::AvailabilityStatus& avail);
	void routingProxyAvailability(const CommonAPI::AvailabilityStatus& avail);
	void artWorkAvailabilityStatusCallback(const MMPresTypes::eArtWorkAvailabilityStatus& status);
	static MediaPresTestCLI* mTestObj;
	std::unordered_map<MMPresTypes::eRepeatMode, std::string> repeatMap;
	std::unordered_map<std::string, MMPresTypes::eRepeatMode> stringToRepeatMap;
	std::unordered_map<MMPresTypes::eShuffleMode, std::string> shuffleMap;
	std::unordered_map<std::string, MMPresTypes::eShuffleMode> stringToShuffleMap;
	std::unordered_map<MMPresTypes::ePlayState, std::string> playstateMap;
	std::unordered_map<MMPresTypes::eTrackSeekSpeed, std::string> speedMap;
	std::unordered_map<std::string, MMPresTypes::eTrackSeekSpeed> stringToSpeedMap;
	std::unordered_map<MMPresTypes::eArtWorkAvailabilityStatus, std::string> artworkMap;
	std::unordered_map<int, ::v3::org::genivi::am_t::am_SourceState_e> sourceStateMap;
	std::unordered_map<int, ::v3::org::genivi::am_t::am_sourceID_e> sourceIDMap;
	std::map<int, std::pair<std::string, std::function<void()>>> funcMap;
};

MediaPresTestCLI* MediaPresTestCLI::mTestObj = nullptr;

MediaPresTestCLI& MediaPresTestCLI::getInstance()
{
	if (mTestObj == nullptr)
	{
		mTestObj = new MediaPresTestCLI();
	}

	return *mTestObj;
}

void MediaPresTestCLI::initializeFuncMap()
{
	int start = 1;
	funcMap.insert(std::make_pair(start++, std::make_pair("getSlots", std::bind(&MediaPresTestCLI::getSlots, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("next", std::bind(&MediaPresTestCLI::next, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("previous", std::bind(&MediaPresTestCLI::previous, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("pause", std::bind(&MediaPresTestCLI::pause, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("resume", std::bind(&MediaPresTestCLI::resume, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("setRepeat", std::bind(&MediaPresTestCLI::setRepeat, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("setShuffle", std::bind(&MediaPresTestCLI::setShuffle, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("fastForward", std::bind(&MediaPresTestCLI::fastForward, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("fastReverse", std::bind(&MediaPresTestCLI::fastReverse, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("getNowplayingAlbumartPath", std::bind(&MediaPresTestCLI::getNowplayingAlbumartPath, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("seekToTime", std::bind(&MediaPresTestCLI::seekToTime, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("openSession", std::bind(&MediaPresTestCLI::openSession, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("setSessionQuery", std::bind(&MediaPresTestCLI::setSessionQuery, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("setSessionQueryWindow", std::bind(&MediaPresTestCLI::setSessionQueryWindow, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("gotoPreviousSessionQuery", std::bind(&MediaPresTestCLI::gotoPreviousSessionQuery, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("playSession", std::bind(&MediaPresTestCLI::playSession, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("ayncSetSourceState", std::bind(&MediaPresTestCLI::asyncSetSourceState, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("closeSession", std::bind(&MediaPresTestCLI::closeSession, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("cancelSession", std::bind(&MediaPresTestCLI::cancelSession, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("jumpToSessionQuery", std::bind(&MediaPresTestCLI::jumpToSessionQuery, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("setSpeed", std::bind(&MediaPresTestCLI::setSpeed, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("getSpeed", std::bind(&MediaPresTestCLI::getSpeed, this))));
	funcMap.insert(std::make_pair(start++, std::make_pair("getAdditionalDeviceInfo", std::bind(&MediaPresTestCLI::getAdditionalDeviceInfo, this))));
}

int MediaPresTestCLI::getFuncMapSize()
{
	return funcMap.size();
}

void MediaPresTestCLI::displayFuncMap()
{
	std::map <int, std::pair<std::string, std::function<void()>>>::const_iterator itr;
	for (itr = funcMap.begin(); itr != funcMap.end() && funcMap.size() > 0; itr++)
	{
		cout << itr->first << ":" << itr->second.first.c_str() << endl;
	}
}

void MediaPresTestCLI::callFunc(int input)
{
	std::map <int, std::pair<std::string, std::function<void()>>>::const_iterator itr;
	itr = funcMap.find(input);
	if (itr != funcMap.end())
	{
		itr->second.second();
	}
	else
	{
		cout << "Function not defined in MediaPres" << endl;
	}

}

void MediaPresTestCLI::initializeRepeatmap()
{
	repeatMap[MMPresTypes::eRepeatMode::REPEAT_ALL] = "REPEAT_ALL";
	repeatMap[MMPresTypes::eRepeatMode::REPEAT_OFF] = "REPEAT_OFF";
	repeatMap[MMPresTypes::eRepeatMode::REPEAT_SONG] = "REPEAT_SONG";
	repeatMap[MMPresTypes::eRepeatMode::REPEAT_FOLDER] = "REPEAT_FOLDER";
	repeatMap[MMPresTypes::eRepeatMode::REPEAT_SUBFOLDER] = "REPEAT_SUBFOLDER";
	repeatMap[MMPresTypes::eRepeatMode::REPEAT_ALBUM] = "REPEAT_ALBUM";
	repeatMap[MMPresTypes::eRepeatMode::REPEAT_ERROR] = "REPEAT_ERROR";
	repeatMap[MMPresTypes::eRepeatMode::REPEAT_UNKNOWN] = "REPEAT_UNKNOWN";
	repeatMap[MMPresTypes::eRepeatMode::REPEAT_TOGGLE] = "REPEAT_TOGGLE";
	repeatMap[MMPresTypes::eRepeatMode::REPEAT_NOTSUP] = "REPEAT_NOTSUP";

	stringToRepeatMap["all"] = MMPresTypes::eRepeatMode::REPEAT_ALL;
	stringToRepeatMap["off"] = MMPresTypes::eRepeatMode::REPEAT_OFF;
	stringToRepeatMap["song"] = MMPresTypes::eRepeatMode::REPEAT_SONG;
	stringToRepeatMap["folder"] = MMPresTypes::eRepeatMode::REPEAT_FOLDER;
	stringToRepeatMap["subfolder"] = MMPresTypes::eRepeatMode::REPEAT_SUBFOLDER;
	stringToRepeatMap["album"] = MMPresTypes::eRepeatMode::REPEAT_ALBUM;
	stringToRepeatMap["toggle"] = MMPresTypes::eRepeatMode::REPEAT_TOGGLE;
}

void MediaPresTestCLI::initializeShuffleMap()
{
	shuffleMap[MMPresTypes::eShuffleMode::SHUFFLE_ALL] = "SHUFFLE_ALL";
	shuffleMap[MMPresTypes::eShuffleMode::SHUFFLE_OFF] = "SHUFFLE_OFF";
	shuffleMap[MMPresTypes::eShuffleMode::SHUFFLE_FOLDER] = "SHUFFLE_FOLDER";
	shuffleMap[MMPresTypes::eShuffleMode::SHUFFLE_SUBFOLDER] = "SHUFFLE_SUBFOLDER";
	shuffleMap[MMPresTypes::eShuffleMode::SHUFFLE_ALBUM] = "SHUFFLE_ALBUM";
	shuffleMap[MMPresTypes::eShuffleMode::SHUFFLE_ERROR] = "SHUFFLE_ERROR";
	shuffleMap[MMPresTypes::eShuffleMode::SHUFFLE_UNKNOWN] = "SHUFFLE_UNKNOWN";
	shuffleMap[MMPresTypes::eShuffleMode::SHUFFLE_TOGGLE] = "SHUFFLE_TOGGLE";
	shuffleMap[MMPresTypes::eShuffleMode::SHUFFLE_NOTSUP] = "SHUFFLE_NOTSUP";

	stringToShuffleMap["all"] = MMPresTypes::eShuffleMode::SHUFFLE_ALL;
	stringToShuffleMap["off"] = MMPresTypes::eShuffleMode::SHUFFLE_OFF;
	stringToShuffleMap["folder"] = MMPresTypes::eShuffleMode::SHUFFLE_FOLDER;
	stringToShuffleMap["subfolder"] = MMPresTypes::eShuffleMode::SHUFFLE_SUBFOLDER;
	stringToShuffleMap["toggle"] = MMPresTypes::eShuffleMode::SHUFFLE_TOGGLE;
}

void MediaPresTestCLI::initializePlaystateMap()
{
	playstateMap[MMPresTypes::ePlayState::PLAYSTATE_PLAYING] = "PLAYSTATE_PLAYING";
	playstateMap[MMPresTypes::ePlayState::PLAYSTATE_PAUSED] = "PLAYSTATE_PAUSED";
	playstateMap[MMPresTypes::ePlayState::PLAYSTATE_STOPPED] = "PLAYSTATE_STOPPED";
	playstateMap[MMPresTypes::ePlayState::PLAYSTATE_FASTFORWARD] = "PLAYSTATE_FASTFORWARD";
	playstateMap[MMPresTypes::ePlayState::PLAYSTATE_FASTREVERSE] = "PLAYSTATE_FASTREVERSE";
	playstateMap[MMPresTypes::ePlayState::PLAYSTATE_ERROR] = "PLAYSTATE_ERROR";
	playstateMap[MMPresTypes::ePlayState::PLAYSTATE_UNKNOWN] = "PLAYSTATE_UNKNOWN";
}

void MediaPresTestCLI::initializeSpeedMap()
{
	speedMap[MMPresTypes::eTrackSeekSpeed::SPEED_ZERO] = "SPEED_ZERO";
	speedMap[MMPresTypes::eTrackSeekSpeed::SPEED_1X] = "SPEED_1X";
	speedMap[MMPresTypes::eTrackSeekSpeed::SPEED_2X] = "SPEED_2X";
	speedMap[MMPresTypes::eTrackSeekSpeed::SPEED_4X] = "SPEED_4X";
	speedMap[MMPresTypes::eTrackSeekSpeed::SPEED_8X] = "SPEED_8X";
	speedMap[MMPresTypes::eTrackSeekSpeed::SPEED_16X] = "SPEED_16X";
	speedMap[MMPresTypes::eTrackSeekSpeed::SPEED_32X] = "SPEED_32X";
	speedMap[MMPresTypes::eTrackSeekSpeed::SPEED_64X] = "SPEED_64X";
	speedMap[MMPresTypes::eTrackSeekSpeed::SPEED_128X] = "SPEED_128X";
	speedMap[MMPresTypes::eTrackSeekSpeed::SPEED_DEVICE_DEPENDENT] = "SPEED_DEVICE_DEPENDENT";

	stringToSpeedMap["0x"] = MMPresTypes::eTrackSeekSpeed::SPEED_ZERO;
	stringToSpeedMap["1x"] = MMPresTypes::eTrackSeekSpeed::SPEED_1X;
	stringToSpeedMap["2x"] = MMPresTypes::eTrackSeekSpeed::SPEED_2X;
	stringToSpeedMap["4x"] = MMPresTypes::eTrackSeekSpeed::SPEED_4X;
	stringToSpeedMap["8x"] = MMPresTypes::eTrackSeekSpeed::SPEED_8X;
	stringToSpeedMap["16x"] = MMPresTypes::eTrackSeekSpeed::SPEED_16X;
	stringToSpeedMap["32x"] = MMPresTypes::eTrackSeekSpeed::SPEED_32X;
	stringToSpeedMap["64x"] = MMPresTypes::eTrackSeekSpeed::SPEED_64X;
	stringToSpeedMap["128x"] = MMPresTypes::eTrackSeekSpeed::SPEED_128X;
}

void MediaPresTestCLI::initializeArtworkMap()
{
	artworkMap[MMPresTypes::eArtWorkAvailabilityStatus::ARTWORK_STATUS_AVAILABLE] = "ARTWORK_STATUS_AVAILABLE";
	artworkMap[MMPresTypes::eArtWorkAvailabilityStatus::ARTWORK_STATUS_UNAVAILABLE] = "ARTWORK_STATUS_UNAVAILABLE";
	artworkMap[MMPresTypes::eArtWorkAvailabilityStatus::ARTWORK_NOT_SUPPORTED] = "ARTWORK_NOT_SUPPORTED";
	artworkMap[MMPresTypes::eArtWorkAvailabilityStatus::ARTWORK_STATUS_UNKNOWN] = "ARTWORK_STATUS_UNKNOWN";
}


MediaPresTestCLI::MediaPresTestCLI()
{
	player = nullptr;
	pRuntime = nullptr;
	deviceManager = nullptr;
	browser = nullptr;
	routing = nullptr;
	pRuntime = CommonAPI::Runtime::get();
	if (pRuntime != nullptr)
	{
		cout << "run time is obtained successfully " << endl;
		player = pRuntime->buildProxy<MediaPlayerProxy>("local", "mmpres.playengine");
		deviceManager = pRuntime->buildProxy<MediaManagerProxy>("local", "mmpres.mediamanager");
		browser = pRuntime->buildProxy<MediaBrowserProxy>("local", "mmpres.browser");
		routing = pRuntime->buildProxy<RoutingControlProxy>("local", "mmpres.routingControl");

		if (player != nullptr)
		{
			cout << "player proxy is obtained successfully " << endl;
			player->getProxyStatusEvent().subscribe(std::bind(&MediaPresTestCLI::playerProxyAvailability, this, std::placeholders::_1));
		}
		else
		{
			cout << " no player proxy" << endl;
		}

		if (deviceManager != nullptr)
		{
			cout << "devicemanager proxy is obtained successfully " << endl;
			deviceManager->getProxyStatusEvent().subscribe(std::bind(&MediaPresTestCLI::deviceManagerProxyAvailability, this, std::placeholders::_1));
		}
		else
		{
			cout << " no deviceManager proxy" << endl;
		}

		if (browser != nullptr)
		{
			cout << "browser proxy is obtained successfully " << endl;
			browser->getProxyStatusEvent().subscribe(std::bind(&MediaPresTestCLI::browserProxyAvailability, this, std::placeholders::_1));
		}
		else
		{

		}

		if (routing != nullptr)
		{
			cout << "routing proxy is obtained successfully " << endl;
			routing->getProxyStatusEvent().subscribe(std::bind(&MediaPresTestCLI::routingProxyAvailability, this, std::placeholders::_1));
		}
		else
		{

		}
	}
	else
	{
		cout << " no run time" << endl;
	}

	initializeFuncMap();
	initializeRepeatmap();
	initializeShuffleMap();
	initializePlaystateMap();
	initializeSpeedMap();
	initializeArtworkMap();
	initSourceIDMap();
	initSourceStateMap();
}

MediaPresTestCLI::~MediaPresTestCLI()
{
	delete mTestObj;
}

void MediaPresTestCLI::resume()
{
	std::cout << "Calling Resume" << std::endl;
	player->resumeAsync(std::bind(&MediaPresTestCLI::resumeResponse, this, std::placeholders::_1, std::placeholders::_2));
	std::cout << "Called Resume" << std::endl;
}

void MediaPresTestCLI::pause()
{
	std::cout << "Calling pause" << std::endl;
	player->pauseAsync(std::bind(&MediaPresTestCLI::pauseResponse, this, std::placeholders::_1, std::placeholders::_2));
	std::cout << "Called pause" << std::endl;
}

void MediaPresTestCLI::next()
{
	std::cout << "Calling next" << std::endl;
	player->nextAsync(0, std::bind(&MediaPresTestCLI::nextResponse, this, std::placeholders::_1, std::placeholders::_2));
	std::cout << "Called next" << std::endl;
}

void MediaPresTestCLI::previous()
{
	std::cout << "Calling previous" << std::endl;
	player->previousAsync(0, std::bind(&MediaPresTestCLI::previousResponse, this, std::placeholders::_1, std::placeholders::_2));
	std::cout << "Called previous" << std::endl;
}

void MediaPresTestCLI::setRepeat()
{
	MMPresTypes::eRepeatMode mode;
	string repeat;
	cout << "Enter one of the following for the repeat values to be set " << endl;
	cout << "[ \"all\", \"off\", \"song\", \"folder\", \"subfolder\", \"album\", \"toggle\" ]" << endl;
	cin >> repeat;
	std::unordered_map<std::string, MMPresTypes::eRepeatMode>::const_iterator itr;
	itr = stringToRepeatMap.find(repeat);
	if (itr != stringToRepeatMap.end())
	{
		mode = itr->second;
		std::cout << "Calling setRpeat" << std::endl;
		player->setRepeatAsync(mode, std::bind(&MediaPresTestCLI::setRepeatResponse, this, std::placeholders::_1, std::placeholders::_2));
		std::cout << "Called setRpeat" << std::endl;
	}
	else
	{
		cout << "repeat " << repeat << " is not defined in media pres " << endl;
	}
}

void MediaPresTestCLI::setShuffle()
{
	MMPresTypes::eShuffleMode mode;
	string shuffle;
	cout << "Enter one of the following for the shuffle values to be set " << endl;
	cout << "[ \"all\", \"off\",  \"folder\", \"subfolder\", \"album\", \"toggle\" ]" << endl;
	cin >> shuffle;
	std::unordered_map<std::string, MMPresTypes::eShuffleMode>::const_iterator itr;
	itr = stringToShuffleMap.find(shuffle);
	if (itr != stringToShuffleMap.end())
	{
		mode = itr->second;
		std::cout << "Calling setShuffle" << std::endl;
		player->setShuffleAsync(mode, std::bind(&MediaPresTestCLI::setShuffleResponse, this, std::placeholders::_1, std::placeholders::_2));
		std::cout << "Called setShuffle" << std::endl;
	}
	else
	{
		cout << "shuffle " << shuffle << " is not defined in media pres " << endl;
	}
}

void MediaPresTestCLI::fastForward()
{
	std::cout << "Calling fastForward" << std::endl;
	player->fastForwardAsync(std::bind(&MediaPresTestCLI::fastForwardResponse, this, std::placeholders::_1, std::placeholders::_2));
	std::cout << "Called fastForward" << std::endl;
}

void MediaPresTestCLI::fastReverse()
{
	std::cout << "Calling fastReverse" << std::endl;
	player->fastReverseAsync(std::bind(&MediaPresTestCLI::fastReverseResponse, this, std::placeholders::_1, std::placeholders::_2));
	std::cout << "Called fastReverse" << std::endl;
}

void MediaPresTestCLI::seekToTime()
{
	uint64_t input;
	cout << "Enter the time position in seconds" << endl;
	cin >> input;
	player->seekToTimeAsync(input, std::bind(&MediaPresTestCLI::seekToTimeResponse, this, std::placeholders::_1, std::placeholders::_2));
}

void MediaPresTestCLI::getNowplayingAlbumartPath()
{
	std::string input;
	cout << "Enter image number" << endl;
	cin >> input;
	int num = atoi(input.c_str());
	player->getNowPlayingAlbumArtPathAsync(num, std::bind(&MediaPresTestCLI::getNowplayingAlbumartPathResponse, this, std::placeholders::_1, std::placeholders::_2, num));
}

void MediaPresTestCLI::getSlots()
{
	deviceManager->getSlotsAsync(std::bind(&MediaPresTestCLI::getSlotsResponse, this, std::placeholders::_1, std::placeholders::_2));
}

void MediaPresTestCLI::resumeResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result)
{
	std::cout << "Resume Response" << std::endl;
	std::cout << "CallStatus is " << (int)status << " and callresult is " << (int)result << endl;
}

void MediaPresTestCLI::pauseResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result)
{
	std::cout << "Pause Response" << std::endl;
	std::cout << "CallStatus is " << (int)status << " and callresult is " << (int)result << endl;
}

void MediaPresTestCLI::nextResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result)
{
	std::cout << "next Response" << std::endl;
	std::cout << "CallStatus is " << (int)status << " and callresult is " << (int)result << endl;
}

void MediaPresTestCLI::previousResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result)
{
	std::cout << "previous Response" << std::endl;
	std::cout << "CallStatus is " << (int)status << " and callresult is " << (int)result << endl;
}

void MediaPresTestCLI::setRepeatResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result)
{
	std::cout << "setRepeat Response- CallStatus is " << (int)status << " and callresult is " << (int)result << endl;
}

void MediaPresTestCLI::setShuffleResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result)
{
	std::cout << "setShuffle Response- CallStatus is " << (int)status << " and callresult is " << (int)result << endl;
}

void MediaPresTestCLI::fastForwardResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result)
{
	std::cout << "fastForwardResponse- CallStatus is " << (int)status << " and callresult is " << (int)result << endl;
}

void MediaPresTestCLI::fastReverseResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result)
{
	std::cout << "fastReverseResponse- CallStatus is " << (int)status << " and callresult is " << (int)result << endl;
}

void MediaPresTestCLI::seekToTimeResponse(const CommonAPI::CallStatus& status, const  MMPresTypes::eCallResult& result)
{
	std::cout << "seekToTimeResponse- CallStatus is " << (int)status << " and callresult is " << (int)result << endl;
}

void MediaPresTestCLI::getNowplayingAlbumartPathResponse(const CommonAPI::CallStatus& status, const std::string& path, const uint8_t& num)
{
	std::cout << "getNowplayingAlbumartPathResponse- image number is " << static_cast<unsigned int>(num) << " and path is" << path << endl;
}

void MediaPresTestCLI::getSlotsResponse(const CommonAPI::CallStatus& status, const MMPresTypes::tSlotList& list)
{
	cout << "list size is " << list.size() << endl;
	MMPresTypes::tSlotList::const_iterator itr;
	MMPresTypes::tSlot slot;

	for (itr = list.begin(); itr != list.end(); itr++)
	{
		slot = (*itr);

		cout << "DeviceManager Slot information Start" << endl;
		cout << "Slot ID : " << (int)slot.getSlotId() << endl;
		cout << "Slot Type : " << (int)slot.getSlotType() << endl;
		cout << "Slot State : " << (int)slot.getSlotState() << endl;
		cout << "Device ID : " << slot.getDevice().getDeviceId() << endl;
		cout << "Device Name : " << slot.getDevice().getDeviceName() << endl;
		cout << "Device Type : " << (int)slot.getDevice().getDeviceType() << endl;
		const MMPresTypes::ContentInfoMap& contentInfoMap = slot.getDevice().getContentInfo();
		auto it = contentInfoMap.begin();
		while (contentInfoMap.end() != it)
		{
			cout << "Title content type: " << it->first << " Count :  " << it->second << endl;
			it++;
		}
		cout << "DeviceManager Slot information end" << endl;
	}
}


void MediaPresTestCLI::playStatusAttributeCallback(const MMPresTypes::ePlayState& playState)
{
	cout << "Playtstate is " << playstateMap[playState] << endl;
}

void MediaPresTestCLI::trackStateAttributeCallback(const MMPresTypes::tTrackState& trackState)
{
	uint64_t currTrack = trackState.getCurrentTrack();
	uint64_t totalTrack = trackState.getTotalTracks();

	cout << "trackState: - currTrack:" << currTrack << " totalTrack:" << totalTrack << endl;
}

void MediaPresTestCLI::currentPlayTimeAttributeCallback(const uint64_t& currTime)
{
	cout << "currentPlayTime:" << currTime << endl;
}

void MediaPresTestCLI::totalPlayTimeAttributeCallback(const uint64_t& duration)
{
	cout << "totalPlayTime:" << duration << endl;
}

void MediaPresTestCLI::nowPlayingDataCallback(const MMPresTypes::tNowPlayingInfo& nowPlayingInfo)
{
	cout << "nowPlayingData:mediumId-" << nowPlayingInfo.getMediumId() << endl;
	cout << "nowPlayingData:title-" << nowPlayingInfo.getTitle() << endl;
	cout << "nowPlayingData:filename-" << nowPlayingInfo.getFilename() << endl;
	cout << "nowPlayingData:artist-" << nowPlayingInfo.getArtist() << endl;
	cout << "nowPlayingData:album-" << nowPlayingInfo.getAlbum() << endl;
	cout << "nowPlayingData:genre-" << nowPlayingInfo.getGenre() << endl;
	cout << "nowPlayingData:isFav-" << nowPlayingInfo.getIsFav() << endl;
	cout << "nowPlayingData:itemType-" << nowPlayingInfo.getNowPlayingItemType() << endl;
}

void MediaPresTestCLI::repeatChangeCallback(const MMPresTypes::eRepeatMode& mode)
{
	cout << "MediaPresTestCLI::repeatChangeCallback-Repeat status is " << repeatMap[mode] << endl;
}

void MediaPresTestCLI::shuffleChangeCallback(const MMPresTypes::eShuffleMode& mode)
{
	cout << "MediaPresTestCLI::shuffleChangeCallback-Shuffle status is " << shuffleMap[mode] << endl;
}

void MediaPresTestCLI::trackSeekSpeedCallback(const MMPresTypes::eTrackSeekSpeed& seekSpeed)
{
	std::string speed;
	std::unordered_map<MMPresTypes::eTrackSeekSpeed, std::string>::const_iterator itr;
	itr = speedMap.find(seekSpeed);
	if (itr != speedMap.end())
	{
		speed = itr->second;
		cout << " MediaPresTestCLI::trackSeekSpeedCallback - speed is " << speed << endl;
	}
}

void MediaPresTestCLI::artWorkAvailabilityStatusCallback(const MMPresTypes::eArtWorkAvailabilityStatus& status)
{
	cout << "MediaPresTestCLI::artWorkAvailabilityStatusCallback-artwork status is " << artworkMap[status] << endl;
}

void MediaPresTestCLI::playerProxyAvailability(const CommonAPI::AvailabilityStatus& avail)
{
	cout << " MediaPlaybackEngine::availibility - playerProxy is " << (int)avail << endl;;

	if (player != nullptr && avail == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		player->getPlayStateAttribute().getChangedEvent().subscribe(std::bind(&MediaPresTestCLI::playStatusAttributeCallback, this, std::placeholders::_1));
		player->getCurrentPlayTimeAttribute().getChangedEvent().subscribe(std::bind(&MediaPresTestCLI::currentPlayTimeAttributeCallback, this, std::placeholders::_1));
		player->getTotalPlayDurationAttribute().getChangedEvent().subscribe(std::bind(&MediaPresTestCLI::totalPlayTimeAttributeCallback, this, std::placeholders::_1));
		player->getTrackStateAttribute().getChangedEvent().subscribe(std::bind(&MediaPresTestCLI::trackStateAttributeCallback, this, std::placeholders::_1));
		player->getNowPlayingDataAttribute().getChangedEvent().subscribe(std::bind(&MediaPresTestCLI::nowPlayingDataCallback, this, std::placeholders::_1));
		player->getRepeatModeAttribute().getChangedEvent().subscribe(std::bind(&MediaPresTestCLI::repeatChangeCallback, this, std::placeholders::_1));
		player->getShuffleModeAttribute().getChangedEvent().subscribe(std::bind(&MediaPresTestCLI::shuffleChangeCallback, this, std::placeholders::_1));
		player->getSpeedAttribute().getChangedEvent().subscribe(std::bind(&MediaPresTestCLI::trackSeekSpeedCallback, this, std::placeholders::_1));
		player->getNowPlayingArtworkStatusAttribute().getChangedEvent().subscribe(std::bind(&MediaPresTestCLI::artWorkAvailabilityStatusCallback, this, std::placeholders::_1));
	}

}

void MediaPresTestCLI::deviceManagerProxyAvailability(const CommonAPI::AvailabilityStatus& avail)
{
	cout << " MediaPresTestCLI::deviceManagerProxyAvailability - available status is " << (int)avail << endl;
	if (deviceManager != nullptr && avail == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		getSlots();
		deviceManager->getSlotUpdateEvent().subscribe(std::bind(&MediaPresTestCLI::slotUpdate, this, _1));
	}
}

void MediaPresTestCLI::browserProxyAvailability(const CommonAPI::AvailabilityStatus& avail)
{
	cout << " MediaPresTestCLI::browserProxyAvailability - available status is " << (int)avail << endl;
	if (browser != nullptr && avail == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		browser->getSessionQueryUpdateSelectiveEvent().subscribe(std::bind(&MediaPresTestCLI::sessionQueryUpdate, this, _1, _2, _3));
		browser->getSessionQueryWindowUpdateSelectiveEvent().subscribe(std::bind(&MediaPresTestCLI::sessionQueryWindowUpdate, this, _1, _2, _3, _4, _5, _6, _7, _8));
	}
}

void MediaPresTestCLI::routingProxyAvailability(const CommonAPI::AvailabilityStatus& avail)
{
	cout << " MediaPresTestCLI::routingProxyAvailability - available status is " << (int)avail << endl;
	if (routing != nullptr && avail == CommonAPI::AvailabilityStatus::AVAILABLE)
	{
		routing->getAckSetSourceStateSelectiveEvent().subscribe(std::bind(&MediaPresTestCLI::ackSetSourceStateCallback, this, _1, _2));
	}
}


int main()
{
	CommonAPI::Runtime::setProperty("LogContext", "MPRS");
	CommonAPI::Runtime::setProperty("LibraryBase", "mmprestest");

	MediaPresTestCLI::getInstance();

	signal(SIGTERM, sigHandler);
	signal(SIGINT, sigHandler);

	//	MediaManagerTestClient mManager; 

	int input;
	cin.clear();
	std::fflush(stdout);
	while (!quit)
	{
		cout << " The list of functions are " << endl;
		MediaPresTestCLI::getInstance().displayFuncMap();
		cout << "Enter the index for the function. For example,press 1 for getslots, 2 for next etc. Enter 0 to stop the test client " << endl;
		cin >> input;
		if (input == EXIT_TESTCLIENT || input>MediaPresTestCLI::getInstance().getFuncMapSize())
		{
			quit = true;
		}
		else
		{
			MediaPresTestCLI::getInstance().callFunc(input);
		}
		cin.clear();
		std::fflush(stdout);
	}

	return 0;
}
