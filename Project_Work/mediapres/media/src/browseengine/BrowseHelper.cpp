#include <browseengine/BrowseHelper.h>
#include <config/MediaPresSettings.h>

using namespace std;

const char* BrowseHelper::CHILD_COUNT = "ChildCount";
const char* BrowseHelper::CONTAINER_CHILD_COUNT = "Children count";
const char* BrowseHelper::PATH = "Path";
const char* BrowseHelper::FOLDER_CATEGORY_NAME = "folder";
const char* BrowseHelper::MUSIC_CATEGORY_NAME = "music";
const char* BrowseHelper::VIDEO_CATEGORY_NAME = "video";
const char* BrowseHelper::IMAGE_CATEGORY_NAME = "image";
const char* BrowseHelper::URI = "URI";
const char* BrowseHelper::TYPE = "Type";
const char* BrowseHelper::TypeEx = "TypeEx";
const char* BrowseHelper::NAME = "DisplayName";
const char* BrowseHelper::FOLDER_CONTAINER_TYPE = "container.storageFolder";
const char* BrowseHelper::AUDIO_FILE_ITEM_TYPE= "music";
const char* BrowseHelper::VIDEO_FILE_ITEM_TYPE = "video";
const char* BrowseHelper::IMAGE_FILE_ITEM_TYPE = "image";
const char* BrowseHelper::PLAYLIST_CONTAINER_TYPE = "container.playlistContainer";
const char* BrowseHelper::ALL_ITEM_NAME = "000000!0*all*";
const char* BrowseHelper::FILESYS_PATH="SysPath";
const char* BrowseHelper::SMART_SEARCH_CANDIDATES = "Candidates";
const char* BrowseHelper::ITEMID = "ItemId";
const char* BrowseHelper::STATIC_TEXT_TOKEN = "^^$$^^";


std::unordered_map<MMPresTypes::eMetadataType, std::string> BrowseHelper::mAudioContainerNameMapping = {
	{MMPresTypes::eMetadataType::METADATATYPE_TITLE,"all"}
	,{MMPresTypes::eMetadataType::METADATATYPE_ARTIST,"artists"}
	,{MMPresTypes::eMetadataType::METADATATYPE_ALBUM, "albums"}
	,{MMPresTypes::eMetadataType::METADATATYPE_GENRE,"genres"}
	,{MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST,"playlists"}
	,{MMPresTypes::eMetadataType::METADATATYPE_COMPOSER, "composers"}
	,{MMPresTypes::eMetadataType::METADATATYPE_FOLDER,"folder"}
	,{MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK,"audiobook"}
	,{MMPresTypes::eMetadataType::METADTATATYPE_PODCAST,"podcast"}
	,{MMPresTypes::eMetadataType::METADATATYPE_COMPILATION,"itunesradio" }
};

std::unordered_map<MMPresTypes::eMetadataType, std::string> BrowseHelper::mVideoContainerNameMapping = {
	{MMPresTypes::eMetadataType::METADATATYPE_VIDEO,"all"}
	,{ MMPresTypes::eMetadataType::METADATATYPE_ARTIST,"artists" }
	,{ MMPresTypes::eMetadataType::METADATATYPE_ALBUM, "albums" }
	,{ MMPresTypes::eMetadataType::METADATATYPE_GENRE,"genres" }
	,{ MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST,"playlists" }
	,{ MMPresTypes::eMetadataType::METADATATYPE_COMPOSER, "composers" }
	,{ MMPresTypes::eMetadataType::METADATATYPE_FOLDER,"folder" }
	,{ MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK,"audiobook" }
	,{ MMPresTypes::eMetadataType::METADTATATYPE_PODCAST,"podcast" }
};

std::unordered_map<MMPresTypes::eMetadataType, std::string> BrowseHelper::mImageContainerNameMapping = {
	{MMPresTypes::eMetadataType::METADATATYPE_IMAGE,"all"}
	,{MMPresTypes::eMetadataType::METADATATYPE_FOLDER,"folder"}
};

std::unordered_map<MMPresTypes::eMetadataType,std::string> BrowseHelper::mMetadataBrowseItemTypeMapping = {
	{ MMPresTypes::eMetadataType::METADATATYPE_TITLE,"music" }
	,{ MMPresTypes::eMetadataType::METADATATYPE_ARTIST,"container.person.musicArtist"}
	,{ MMPresTypes::eMetadataType::METADATATYPE_ALBUM,"container.album.musicAlbum"}
	,{ MMPresTypes::eMetadataType::METADATATYPE_GENRE,"container.genre.musicGenre"}
	,{ MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST,"container.playlistContainer"}
	,{ MMPresTypes::eMetadataType::METADATATYPE_COMPOSER,"container.person.musicComposer"}
	,{ MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK,"container.album.musicAlbum"}
	,{ MMPresTypes::eMetadataType::METADTATATYPE_CHAPTER,"music.audioBook"}
	,{ MMPresTypes::eMetadataType::METADTATATYPE_PODCAST,"container.album.musicAlbum"}
	,{ MMPresTypes::eMetadataType::METADTATATYPE_EPISODE,"music.audioPodcast"}
	,{ MMPresTypes::eMetadataType::METADATATYPE_COMPILATION,"music.itunesradio"}
	,{ MMPresTypes::eMetadataType::METADATATYPE_VIDEO,"video"}
	,{ MMPresTypes::eMetadataType::METADATATYPE_IMAGE,"image"}
};

std::unordered_map<MMPresTypes::eMetadataType, std::string> BrowseHelper::mBreadCrumbCategoryNameMapping = {
	{ MMPresTypes::eMetadataType::METADATATYPE_TITLE,"^^$$^^Songs^^$$^^"}
	,{ MMPresTypes::eMetadataType::METADATATYPE_ARTIST,"^^$$^^Artists^^$$^^"}
	,{ MMPresTypes::eMetadataType::METADATATYPE_ALBUM,"^^$$^^Albums^^$$^^"}
	,{ MMPresTypes::eMetadataType::METADATATYPE_GENRE,"^^$$^^Genres^^$$^^" }
	,{ MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST,"^^$$^^Playlists^^$$^^" }
	,{ MMPresTypes::eMetadataType::METADATATYPE_COMPOSER,"^^$$^^Composers^^$$^^" }
	,{ MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK,"^^$$^^Audiobooks^^$$^^" }
	,{ MMPresTypes::eMetadataType::METADTATATYPE_PODCAST,"^^$$^^Podcasts^^$$^^" }
	,{ MMPresTypes::eMetadataType::METADATATYPE_COMPILATION,"^^$$^^iTunesRadio^^$$^^" }
	,{ MMPresTypes::eMetadataType::METADATATYPE_IMAGE,"^^$$^^Images^^$$^^" }
	,{ MMPresTypes::eMetadataType::METADATATYPE_VIDEO,"^^$$^^Videos^^$$^^" }
};

std::unordered_map<MMPresTypes::eMetadataType, std::string> BrowseHelper::mFileSystemBrowseItemTypeMapping = {
	{ MMPresTypes::eMetadataType::METADATATYPE_FOLDER,"container.storageFolder"}
	,{ MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST,"container.playlistContainer"}
    ,{ MMPresTypes::eMetadataType::METADATATYPE_FILE,"music"}
};

std::unordered_map<MMPresTypes::eMetadataType, MMPresTypes::eMetadataType> BrowseHelper::mBrowseItemTypeOrdering = {
	{ MMPresTypes::eMetadataType::METADATATYPE_GENRE,MMPresTypes::eMetadataType::METADATATYPE_ARTIST}
	,{ MMPresTypes::eMetadataType::METADATATYPE_ARTIST,MMPresTypes::eMetadataType::METADATATYPE_ALBUM}
	,{ MMPresTypes::eMetadataType::METADATATYPE_ALBUM,MMPresTypes::eMetadataType::METADATATYPE_TITLE}
	,{ MMPresTypes::eMetadataType::METADATATYPE_COMPOSER,MMPresTypes::eMetadataType::METADATATYPE_ALBUM}
	,{ MMPresTypes::eMetadataType::METADTATATYPE_PLAYLIST,MMPresTypes::eMetadataType::METADATATYPE_TITLE}
	,{ MMPresTypes::eMetadataType::METADTATATYPE_AUDIOBOOK,MMPresTypes::eMetadataType::METADTATATYPE_CHAPTER}
	,{ MMPresTypes::eMetadataType::METADTATATYPE_PODCAST,MMPresTypes::eMetadataType::METADTATATYPE_EPISODE}
	,{ MMPresTypes::eMetadataType::METADATATYPE_COMPILATION,MMPresTypes::eMetadataType::METADATATYPE_TITLE}
};

std::unordered_map<MMPresTypes::eMetadataType, std::string> BrowseHelper::mAllItemNameMapping = {
	{MMPresTypes::eMetadataType::METADATATYPE_GENRE,"All Artists"}
	,{MMPresTypes::eMetadataType::METADATATYPE_ARTIST,"All Albums"}
	,{MMPresTypes::eMetadataType::METADATATYPE_ALBUM,"All Songs" }
	,{ MMPresTypes::eMetadataType::METADATATYPE_COMPOSER,"All Albums"}
};

std::unordered_map<MMPresTypes::eTitleContentType, std::string> BrowseHelper::mTitleContentTypeMapping = {
	{ MMPresTypes::eTitleContentType::TITLETYPE_AUDIO,"music"}
   ,{ MMPresTypes::eTitleContentType::TITLETYPE_VIDEO,"video"}
   ,{ MMPresTypes::eTitleContentType::TITLETYPE_IMAGE,"image"}
   ,{ MMPresTypes::eTitleContentType::TITLETYPE_UNKNOWN,"unknown"}
};

MMPresTypes::eMetadataType BrowseHelper::getContainerId(const std::string& name, std::string mediaType)
{
  if (!mediaType.compare(BrowseHelper::MUSIC_CATEGORY_NAME))
  {
	  auto  it = mAudioContainerNameMapping.begin();
	  FacadeHelper<MediaPresSettings> mSettings;
	  if (!name.compare(mSettings->getGenresCategoryName().c_str()))
	  {
		  return MMPresTypes::eMetadataType::METADATATYPE_GENRE;
	  }
	  while (mAudioContainerNameMapping.end() != it)
	  {
		  string str = it->second;
		  if (!str.compare(name.c_str()))
		  {
			  return it->first;
		  }
		  it++;
	  }
  }
  else if(!mediaType.compare(BrowseHelper::VIDEO_CATEGORY_NAME))
	{
		auto it = mVideoContainerNameMapping.begin();
		while (mVideoContainerNameMapping.end() != it)
		{
			string str = it->second;
			if (!str.compare(name.c_str()))
			{
				return it->first;
			}
			it++;
		}
	}
	else if(!mediaType.compare(BrowseHelper::IMAGE_CATEGORY_NAME))
	{
		auto it = mImageContainerNameMapping.begin();
		while (mImageContainerNameMapping.end() != it)
		{
			string str = it->second;
			if (!str.compare(name.c_str()))
			{
				return it->first;
			}
			it++;
		}
	}
	else
	{
	}
	return MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;
}

const std::string  BrowseHelper::getContainerName(MMPresTypes::eMetadataType id)
{
	auto  it = mAudioContainerNameMapping.begin();

	FacadeHelper<MediaPresSettings> mSettings;
	if (id == MMPresTypes::eMetadataType::METADATATYPE_GENRE)
	{
		return mSettings->getGenresCategoryName();
	}

	while (mAudioContainerNameMapping.end() != it)
	{
		if (it->first == id)
		{
			return it->second; 
		}

		it++;
	}

	it = mVideoContainerNameMapping.begin();
	while (mVideoContainerNameMapping.end() != it)
	{
		if (it->first == id)
		{
			return it->second;
		}

		it++;
	}

	it = mImageContainerNameMapping.begin();
	while (mImageContainerNameMapping.end() != it)
	{
		if (it->first == id)
		{
			return it->second;
		}

		it++;
	}
	return "";
}

std::string BrowseHelper::getBreadCrumbCategoryName(MMPresTypes::eMetadataType id)
{
	auto  it = mBreadCrumbCategoryNameMapping.begin();
	while (mBreadCrumbCategoryNameMapping.end() != it)
	{
		if (it->first == id)
		{
			return it->second;
		}

		it++;
	}
	return "";
}

const std::string BrowseHelper::updateBreadCrumbPath(std::string const& breadCrumb)
{
   std::string l_BreadCrumbPath = breadCrumb;
   const std::string l_AllAlbums = "All Albums";
   const std::string l_AllAlbumsReplace = "^^$$^^All Albums^^$$^^";
   const std::string l_AllSongs = "All Songs";
   const std::string l_AllSongsReplace = "^^$$^^All Songs^^$$^^";
   size_t pos = 0;

   while ((pos = l_BreadCrumbPath.find(l_AllAlbums, pos)) != std::string::npos)
   {
      l_BreadCrumbPath.replace(pos, l_AllAlbums.length(), l_AllAlbumsReplace);
      pos += l_AllAlbumsReplace.length();
   }
   pos = 0;
   while ((pos = l_BreadCrumbPath.find(l_AllSongs, pos)) != std::string::npos)
   {
      l_BreadCrumbPath.replace(pos, l_AllSongs.length(), l_AllSongsReplace);
      pos += l_AllSongsReplace.length();
   }
   return l_BreadCrumbPath;
}

const std::string BrowseHelper::removeStaticTextToken(std::string const& breadCrumb)
{
   std::string l_BreadCrumbPath = breadCrumb;
   size_t pos = 0;
   while ((pos = l_BreadCrumbPath.find(BrowseHelper::STATIC_TEXT_TOKEN, pos)) != std::string::npos)
   {
      l_BreadCrumbPath.erase(pos, strlen(BrowseHelper::STATIC_TEXT_TOKEN));
   }

   return l_BreadCrumbPath;
}



MMPresTypes::eMetadataType BrowseHelper::getItemMetadataType(const std::string& itemType)
{
	std::unordered_map<MMPresTypes::eMetadataType, std::string>::const_iterator itr = mMetadataBrowseItemTypeMapping.begin();
	while(itr != mMetadataBrowseItemTypeMapping.end())
	{
		if (((itr->second).compare(itemType))==0)
		{
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< BrowseHelper::getItemMetadataType: itemType=%s,string metadataType from map=%s int metadataType from map=%d", itemType.c_str(), (itr->second).c_str(), (int)(itr->first));
			return itr->first;
		}
		itr++;
	}
	return MMPresTypes::eMetadataType::METADATATYPE_UNKNOWN;
}


const std::string BrowseHelper::getFolderNameFromPath(const std::string & path)
{
	uint32_t size = path.size(); 
	uint32_t pos = path.find_last_of('/');
	std::string ret(""); 
	if ( (pos < (size-1) ))
	{
		ret = path.substr(pos + 1);
	}
	
	return ret; 
}

const int32_t BrowseHelper::getLevelIdFromPath(const std::string& path)
{
	int32_t level = -1;
	uint32_t size = path.size();
	uint32_t pos = path.find_last_of('/');
	LOG_INFO(LOG_CONTEXT_MEDIA, "<<< BrowseHelper::getLevelIdFromPath: path=%s size=%d pos=%d", path.c_str(),size,pos);

	std::string ret("");
	if ((pos!=std::string ::npos) && pos < (size - 1))
	{
		ret = path.substr(0,pos);
		size = ret.size();
		pos = ret.find_last_of('/');
		LOG_INFO(LOG_CONTEXT_MEDIA, "<<< BrowseHelper::getLevelIdFromPath: ret=%s size=%d pos=%d", ret.c_str(),size,pos);
		if ((pos != std::string::npos) &&  pos < (size - 1))
		{
			ret = ret.substr(pos+1);
			level = atoi(ret.c_str());
			++level;
			LOG_INFO(LOG_CONTEXT_MEDIA, "<<< BrowseHelper::getLevelIdFromPath :level=%d", level);
		}
	}

	return level;
}

bool BrowseHelper::checkIfMetadataBrowseItemSupported(MMPresTypes::eMetadataType browseItemType)
{
	return(mMetadataBrowseItemTypeMapping.find(browseItemType) != mMetadataBrowseItemTypeMapping.end());
}

bool BrowseHelper::checkIfFileSystemBrowseItemSupported(MMPresTypes::eMetadataType browseItemType)
{
	return(mFileSystemBrowseItemTypeMapping.find(browseItemType) != mFileSystemBrowseItemTypeMapping.end());
}

bool BrowseHelper::checkIfBrowseItemTypeValidForTheBrowseLevel(MMPresTypes::eMetadataType prevBrowseItemType, MMPresTypes::eMetadataType currBrowseItemType)
{
	bool valid = false;
	std::unordered_map<MMPresTypes::eMetadataType, MMPresTypes::eMetadataType>::const_iterator itr;
	for (itr = mBrowseItemTypeOrdering.begin(); itr != mBrowseItemTypeOrdering.end(); itr++)
	{
		if (prevBrowseItemType == itr->first && currBrowseItemType == itr->second)
		{
			valid = true;
			break;
		}
	}
	return valid;
}

const std::string BrowseHelper::getAllItemName(MMPresTypes::eMetadataType itemType)
{
	std::unordered_map<MMPresTypes::eMetadataType, std::string>::const_iterator itr;
	itr = mAllItemNameMapping.find(itemType);
	if (itr != mAllItemNameMapping.end())
	{
		return itr->second;
	}
	return "";

}

bool BrowseHelper::map_compare(MMPresTypes::FilterIDMap const &lhs, MMPresTypes::FilterIDMap const &rhs)
{
	bool ret = false;
	if (lhs.size() > 0 && rhs.size() > 0 && lhs.size() == rhs.size())
	{
		MMPresTypes::FilterIDMap::const_iterator itr1, itr2;
		itr1 = lhs.begin();
		itr2 = rhs.begin();
		for (;itr1!=lhs.end() && itr1->first == itr2->first && itr1->second == itr2->second; itr1++,itr2++);
		if (itr1 == lhs.end())
		{
			ret = true;
		}
	}
	return ret;
}

const std::string BrowseHelper::getTitleContentType(MMPresTypes::eTitleContentType type)
{
	std::string strtype = "";
	std::unordered_map<MMPresTypes::eTitleContentType, std::string>::const_iterator ctr = mTitleContentTypeMapping.begin();
	while (ctr != mTitleContentTypeMapping.end())
	{
		if (type == (ctr->first))
		{
			strtype = ctr->second;
			return strtype;
		}
		ctr++;
	}
	return strtype;
}
