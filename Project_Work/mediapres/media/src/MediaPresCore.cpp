#include "MediaPresCore.h"
#include<core/PresCtrl.h>

#define REMOTE_CD_PLUGIN_PATH "/usr/lib/libremotecdfordplugin.so"

MediaPresCore::MediaPresCore() :
	mPlayerAvail(false)
	,mBrowseAvail(false)
	,mAMAvailable(false)
	,mDevManagerAvail(false)
	,mBtServiceProxyAvailable(true)
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresCore::MediaPresCore");

	//Load Media-Pres plugins here through mpres configuration file
	Json::Value plugins(Json::arrayValue);
	plugins = mConfig->GetConfigFileKey("Plugins");
	LOG_INFO(LOG_CONTEXT_MEDIA, "Plugins size %lu", plugins.size());
    for(uint32_t index=0; index < plugins.size(); ++index)
    {
        PluginLoader<>* loader = new PluginLoader<>();
        //PluginLoader<MediaPresCore*> loader;
		LOG_INFO(LOG_CONTEXT_MEDIA, "Plugins %s", plugins[index].asString().c_str());

    	//load plugin
        if(loader!=NULL)
        {
            LOG_INFO(LOG_CONTEXT_MEDIA, "Loading %s", plugins[index].asString().c_str());
            MediaPresPluginMap_t [ plugins [ index ] . asString () ] = loader -> LoadPlugin ( std::move ( plugins [ index ].asString ().c_str() ));
        }

    	//save these plugin in vector
    	mLoader.push_back(loader);
        LOG_INFO(LOG_CONTEXT_MEDIA, "mLoader size %lu", mLoader.size());
    }
}

MediaPresCore::~MediaPresCore()
{
	LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresCore::~MediaPresCore");

	//remove plugins from vector
	for (const auto& loader : mLoader)
	{
		delete loader;
	}
}


IRemoteCDPlugin* MediaPresCore::getRemoteCDPluginInstance ()
{
    LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d", __FUNCTION__ , __LINE__ );
    
    IRemoteCDPlugin* mRemoteCDPlugin_ = nullptr;
    
    if ( MediaPresPluginMap_t . find ( REMOTE_CD_PLUGIN_PATH  ) != MediaPresPluginMap_t.end() )
    {
        LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d, library path  < %s>", __FUNCTION__ , __LINE__ , REMOTE_CD_PLUGIN_PATH );
        mRemoteCDPlugin_ =    ( dynamic_cast < IRemoteCDPlugin*  > ( MediaPresPluginMap_t [ REMOTE_CD_PLUGIN_PATH ] ) ) ;
    }
  
  LOG_INFO(LOG_CONTEXT_MEDIA, "%s %d", __FUNCTION__ , __LINE__ );
  
  return mRemoteCDPlugin_;
    
}

void MediaPresCore::startProxyTasks()
{
	static bool mDevProxyStarted = false;
	if (mPlayerAvail && mDevManagerAvail)
	{
		LOG_INFO(LOG_CONTEXT_MEDIA, "MediaPresCore::startProxyTasks");
		if (!mDevProxyStarted)
		{
			mDetect->startProxyTasks();
			mPlayer->startProxyTasks();
			mDevProxyStarted=true;
		}
		if (mBrowseAvail)
		{
			mBrowser->startProxyTasks();
		}
	}
}

void MediaPresCore::setPlayerAvailable()
{
	mPlayerAvail = true;
	startProxyTasks();
}
void MediaPresCore::setBrowserAvailable()
{
	mBrowseAvail = true;
	startProxyTasks();
}
void MediaPresCore::setAMAvailable()
{
	mAMAvailable = true;
	startProxyTasks();
}
void MediaPresCore::setDevManagerAvailable()
{
	mDevManagerAvail = true;
	startProxyTasks();
}

void MediaPresCore::setBtServiceProxyAvailable()
{
	mBtServiceProxyAvailable = true;
}
