
#ifndef _MEDIAPRESSERVICE_H
#define _MEDIAPRESSERVICE_H

#include <core.h>
#include <playengine/MediaPlayerStubImpl.hpp>
#include <browseengine/MediaBrowseEngine.h>
#include <detectionengine/MediaDetectionEngine.h>
#include<audiomanagerinterface/audiomanagerinterface.h>
#include<playengine/MediaPlaybackEngine.h>
#include<config/ConfigParser.h>



class MediaPresCore
{
public:
	void setPlayerAvailable();
	void setBrowserAvailable();
	void setAMAvailable();
	void setDevManagerAvailable();
	void setBtServiceProxyAvailable();
    IRemoteCDPlugin* getRemoteCDPluginInstance ();
private:
	uint32_t registerClient()
	{
		return 0u;
	}
	void deregisterClient(uint32_t ClientID)
	{

	}
	MediaPresCore();
	~MediaPresCore();
	friend class FacadeHelper<MediaPresCore>;
	void startProxyTasks();
	bool mPlayerAvail;
	bool mBrowseAvail;
	bool mAMAvailable;
	bool mDevManagerAvail;
	bool mBtServiceProxyAvailable;
	std::vector<PluginLoader<>*> mLoader;
	FacadeHelper<MediaDetectionEngine> mDetect;
	FacadeHelper<MediaBrowseEngine> mBrowser;
	FacadeHelper<MediaPlaybackEngine> mPlayer;
	FacadeHelper<ConfigParser> mConfig;
    std::map<string, IPluginBase*> MediaPresPluginMap_t;
   
};

#endif
