/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            BrowseHelper.h
* @ingroup         presctrlcore
* @author          Sriram Srivatsan (sriram.srivatsan@harman.com)
* @brief           Implements some helper methods for browse functionality. Basically does the mapping between data from coc media to media pres..
*/

#ifndef _BROWSE_HELPER_H_
#define _BROWSE_HELPER_H_

#include <core.h>
#include <unordered_map>

using namespace v3::com::harman::mmpres; 

class BrowseHelper
{
public : 

	static void init(); 

	static const char*  CHILD_COUNT;
	static const char* CONTAINER_CHILD_COUNT;
	static const char*  PATH; 
	static const char*  FOLDER_CATEGORY_NAME;
	static const char*  MUSIC_CATEGORY_NAME;
	static const char* VIDEO_CATEGORY_NAME;
	static const char* IMAGE_CATEGORY_NAME;
	static const char*  URI;
	static const char* TYPE;
	static const char* TypeEx;
	static const char* NAME;
	static const char* FOLDER_CONTAINER_TYPE;
	static const char* AUDIO_FILE_ITEM_TYPE;
	static const char* VIDEO_FILE_ITEM_TYPE;
	static const char* IMAGE_FILE_ITEM_TYPE;
	static const char* PLAYLIST_CONTAINER_TYPE;
	static const char* ALL_ITEM_NAME;
	static const char* FILESYS_PATH;
	static const char* SMART_SEARCH_CANDIDATES;
   static const char* ITEMID;
   static const char* STATIC_TEXT_TOKEN;

	static const std::string buildPath(uint16_t mediaInternalSessionID,uint8_t Level,uint16_t FolderID);
	
	static MMPresTypes::eMetadataType getContainerId(const std::string& name, std::string mediaType="music");
	static const std::string getContainerName(MMPresTypes::eMetadataType);

	static const std::string getFolderNameFromPath(const std::string & path);
	static const int32_t getLevelIdFromPath(const std::string& path);
	static MMPresTypes::eMetadataType getItemMetadataType(const std::string& itemType);
	static bool checkIfMetadataBrowseItemSupported(MMPresTypes::eMetadataType browseItemType);
	static bool checkIfFileSystemBrowseItemSupported(MMPresTypes::eMetadataType browseItemType);
	static bool checkIfBrowseItemTypeValidForTheBrowseLevel(MMPresTypes::eMetadataType prevBrowseItemType, MMPresTypes::eMetadataType currBrowseItemType);

	static const std::string getAllItemName(MMPresTypes::eMetadataType itemType);

	// auto keyword for lambda declration is supported only in c++14
	/*template <typename map>
	static bool map_compare(map const &lhs, map const &rhs) 
	{
        auto pred = [](auto a, auto b) { return ((a.first == b.first) && (a.second == b.second));};
		return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin(), pred);
	}*/

	static bool map_compare(MMPresTypes::FilterIDMap const &lhs, MMPresTypes::FilterIDMap const &rhs);
	static std::string getBreadCrumbCategoryName(MMPresTypes::eMetadataType id);
   static const std::string updateBreadCrumbPath(std::string const& breadCrumb);
   static const std::string removeStaticTextToken(std::string const& breadCrumb);
	static const std::string getTitleContentType(MMPresTypes::eTitleContentType type);


private : 
	static std::unordered_map<MMPresTypes::eMetadataType,std::string> mAudioContainerNameMapping;
	static std::unordered_map<MMPresTypes::eMetadataType, std::string> mVideoContainerNameMapping;
	static std::unordered_map<MMPresTypes::eMetadataType, std::string> mImageContainerNameMapping;
	static std::unordered_map<MMPresTypes::eMetadataType, std::string> mMetadataBrowseItemTypeMapping;
	static std::unordered_map<MMPresTypes::eMetadataType, std::string> mFileSystemBrowseItemTypeMapping;
	static std::unordered_map<MMPresTypes::eMetadataType, MMPresTypes::eMetadataType> mBrowseItemTypeOrdering;
	static std::unordered_map<MMPresTypes::eMetadataType, std::string> mAllItemNameMapping;
	static std::unordered_map<MMPresTypes::eMetadataType, std::string> mBreadCrumbCategoryNameMapping;
	static std::unordered_map<MMPresTypes::eTitleContentType, std::string> mTitleContentTypeMapping;
};

#endif
