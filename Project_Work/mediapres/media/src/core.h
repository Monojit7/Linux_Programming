/*********************************************************************
* Project         MediaPresCtrl
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            core.h
* @ingroup         Media Pres Control
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           Pre compiled header for Media Pres Control. Core provides the file as a reference, projects have to modify it. 
*				   This file must include all the Common API related headers, proxies / stubs , core headers etc. 
*				   This drastically reduces compilation time for the solution. 
*/

#pragma once
//#include <unordered_map>
#include <CommonAPI/CommonAPI.hpp>
#include <core/PresCtrlTypes.h>
#include <core/RequestBase.h>
#include <core/CommonAPIClientAbstraction.h>
#include <core/CommonAPIServiceAbstraction.h>
#include <core/logging.h>
#include <core/PluginLoader.h>
#include <core/PresCtrl.h>
#include <core/FacadeHelper.h>
#include <MediaPresControlConstants.h>
#include <core/CThreadInterface.h>
#include <core/IDataHandler.h>
#include <core/ITimerEventListener.h>
#include <core/CTimerInterface.h>
#include <MediaPresControlTypes.hpp>
#include <core/PresEventAbstraction.h>
#include <core/IPluginBase.h>
#include <IRemoteCDPlugin.hpp>
#include <RemoteCDManager.hpp>


#ifdef NO_ERROR
#undef NO_ERROR
#endif
#ifdef S_OK
#undef S_OK
#endif

//Proxies
#include <v1/com/harman/media/DeviceManagerProxy.hpp>
#include<v1/com/harman/media/ImageProcessProxy.hpp>
#include<v1/com/harman/media/PlayerExtProxy.hpp>
#include<v1/com/harman/media/MediaSettingProxy.hpp>
#include<v1/com/harman/media/BaseType.hpp>

#include <v1/org/genivi/mediamanager/IndexerProxy.hpp>
#include <v1/org/genivi/mediamanager/BrowserProxy.hpp>
#include<v1/org/genivi/mediamanager/PlayerProxy.hpp>
#include<v1/org/genivi/mediamanager/MediaTypes.hpp>
#include<v1/org/genivi/mediamanager/PlayerTypes.hpp>
#include<v1/org/genivi/mediamanager/BrowserTypes.hpp>

#include <v3/org/genivi/am_t.hpp>
#include <v3/org/genivi/am/routinginterface/RoutingControlObserverProxy.hpp>
#include <v3/org/genivi/am/commandinterface/CommandControlProxy.hpp>

#include<v4/com/harman/connectivity/BtService.hpp>
#include<v4/com/harman/connectivity/BtServiceProxy.hpp>
#include<v4/com/harman/connectivity/BtServiceProxyBase.hpp>
#include<com/harman/connectivity/btTypedefs.hpp>

#include<v4/com/harman/btpres/BTPresTypes.hpp>
#include<v2/com/harman/btpres/ConnectionManager.hpp>
#include<v2/com/harman/btpres/ConnectionManagerProxy.hpp>
#include<v2/com/harman/btpres/ConnectionManagerProxyBase.hpp>

//Stubs
#include <v1/com/harman/mmpres/MediaPlayerStubDefault.hpp>
#include <v3/com/harman/mmpres/MediaBrowserStubDefault.hpp>
#include <v1/com/harman/mmpres/MediaManagerStubDefault.hpp>
#include <v3/com/harman/mmpres/MMPresTypes.hpp>
#include <v3/com/harman/mmpres/MediaBrowserStub.hpp>
#include <v1/com/harman/mmpres/MediaSettingsStubDefault.hpp>

#include <v1/org/genivi/am/routinginterface/RoutingControlStubDefault.hpp>

#include<v3/com/harman/audio/audioPresCtrl/audioPresCtrlTypes.hpp>
#include<v1/com/harman/audio/audioPresCtrl/audioSourceInfoProxy.hpp>
#include <v0/org/harman/ford/mediaplaybackProxy.hpp>

#include <stdint.h>
#define LOG_CONTEXT_MEDIA 1
#define LOG_CONTEXT_MEDIADETECTION 2
#define LOG_CONTEXT_MEDIAPLAYBACK 3
#define LOG_CONTEXT_MEDIABROWSE   4
#define LOG_CONTEXT_MPRESPLUGIN 5
#define LOG_CONTEXT_RCD            6
