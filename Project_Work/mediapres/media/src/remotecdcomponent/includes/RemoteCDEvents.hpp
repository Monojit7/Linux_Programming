#pragma once

#include <vector>
#include <string>
#include <functional>
#include "RemoteCDTypes.hpp"

// namespace declaratives
using namespace std ;

// events that are published to clients from the plugin
enum RemoteCDEvents
{
    // external events - client would subscribe to
    EVENT_CD_TYPE                               = 0x7,  // mpres events ends at 6, so start the remotecd from there on
    EVENT_CD_ERROR                              = 0x9,
    EVENT_PLAYCONTROL_ACTION_STATUS             = 0xA,
    EVENT_PLAYCONTROL_CONTEXT_STATUS            = 0xB,
    EVENT_PLAYCONTROL_TRACKTIME_STATUS          = 0xC,
    EVENT_PLAYCONTROL_NUMBER_TRACKS_STATUS      = 0xD,
    EVENT_PLAYCONTROL_NOWPLAYING_STATUS         = 0xE,
    EVENT_PLAYCONTROL_ACTIVEFOLDER_STATUS       = 0xF,
    EVENT_PLAYCONTROL_ACTIVETRACK_STATUS        = 0x10,
    EVENT_BROWSECONTROL_GETFOLDERNAME_STATUS    = 0x11,
    EVENT_BROWSECONTROL_BROWSEINFO_STATUS       = 0x12,
    EVENT_BROWSECONTROL_TOTAL_FOLDER_COUNT      = 0x22,

    // internal events 
    EVENT_TOC_DATA_UPDATE					    = 0x13,
    EVENT_FOLDER_DATA_UPDATE					= 0x14,
    EVENT_NOWPLAYING_DATA_UPDATE				= 0x15,
    EVENT_PLAYER_PROXY_AVAILABLE                = 0x16,
    EVENT_CACHE_TOC_UPDATE					    = 0x17,
    EVENT_CACHE_FOLDER_UPDATE					= 0x18,
    EVENT_CACHE_NOWPLAYING_UPDATE				= 0x19,
    EVENT_EJECT_CHAIMS_WARNING					= 0x23,
    EVENT_CD_STATUS                             = 0x1000
};

// signature of the events that are published to clients from the plugin
typedef function < void ( eCDType & ) >                 cdType ;
typedef function < void ( eCDStatus ) >                 cdStatus ;
typedef function < void ( CDError   ) >                 cdError ;
typedef function < void ( eCDPlayActions & ) >          playActionStatus ;
typedef function < void ( CDPlayContext ) >             playContextStatus ;
typedef function < void ( CDNowPlaying ) >              cdNowPlayingInfo ;
typedef function < void ( CDActiveFolder ) >            activeFolder ;
typedef function < void ( short number, string name ) > getFolderNameStatus ;
typedef function < void ( Folder, bool ) >              browseStatus ;
typedef function < void ( TrackTime ) >                 playtrackTime ;
typedef function < void ( uint64_t	) >                 activeTrack ;
typedef function < void ( uint64_t ) >                  numberOfTracksStatus ;
typedef function < void ( Folder, bool ) >              cacheUpdate ;
typedef function < void ( Folder, bool ) >              folderDataUpdate ;
typedef function < void ( CDTableofContents& ) >        tocDataUpdate ;
typedef function < void ( bool ) >                      playerAvailability ;
typedef function < void ( bool ) >                      ejectChimesWarning ;
typedef function < void ( short ) >                     browseTotalCount ;

