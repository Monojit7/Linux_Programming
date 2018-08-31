#pragma once

#include <vector>
#include <string>
#include "RemoteCDFolder.hpp"
// namespace declaratives
using namespace std ;

    // itemNumber - for track is a combination of FolderNumber, itemIndex of that track in the folder corresponding to FolderNumber, for folder it is just the folderNumber in the first one and second value will be 0
    typedef pair < short, short >       CDPlayItemNumber ;
    
	typedef enum eCDStatus
    {
        eInvalidStatus    =  0x0,
        eReading          =  0x1,
        eLoading          =  0x2,
        eLoaded           =  0x3,
        eInsert           =  0x4,
        ePrepareEject     =  0x5,
        eEjecting         =  0x6,
        eRemoveDisc       =  0x7
    }eCDStatus;

    typedef enum eEOLResult { 
               
     // EXT CD not configured
               
     EXT_CD_PRESENT_NO_CONFIGURED = 0x00,
               
     // Ext CD Disabled 
               
     EXD_CD_PRESENT_DISABLED = 0x01,
               
     // External CD present with no warning popup
               
     EXT_CD_PRESENT_NO_WARNING_POPUP = 0x02 ,
               
     // External CD present with warning popup
               
      EXT_CD_PRESENT_WARNING_POPUP = 0x03 
               
    } eEOLResult ;
    
    typedef enum eCDType
    {
        eUnknownCD        = 0x0,
        eNoCDTextCDDA     = 0x1,
        eMP3              = 0x2,
        eNonAudioCD       = 0x3,
        eDVD              = 0x4,
        eMP3DVD           = 0x5,
        eCDTextCDDA       = 0x6
    }eCDType;

	typedef enum eCDError
    {
        eInvalidError     =  0x0,
        eNoError          =  0x1,
        eNoDisc           =  0x2,
        eDiscReadError    =  0x3
    }eCDError;

	typedef enum eHardwareError
    {
        eInvalidHWError             =  0x0,
        eNoHWError                  =  0x1,
        eROMReadError               =  0x2,
        eKeyLockedHWError           =  0x3,
        eKeyLockedForeverHWError    =  0x4,
        eCommunicationHWError       =  0x5,
        eOverTemperatureHWError     =  0x6,
        eFocusHWError               =  0x7
    }eHardwareError;

    // errors about CD notified by plf it will be a combination of CDError & the hardwareErro
    typedef pair < eCDError, eHardwareError >   CDError ;

    // play actions client calls us which we have to inform to player via CAN
    typedef enum eCDPlayActions
    {
        eNoAction         = 0x0,
        eStop             = 0x1,
        ePlay             = 0x2,
        ePause            = 0x3,
        eForwardSpeed1    = 0x4,
        eForwardSpeed2    = 0x5,
        eReverseSpeed1    = 0x6,
        eReverseSpeed2    = 0x7
    }eCDPlayActions;
    
    typedef enum eCDPlayItemType
    {
        ePlayFolder     = 0x1,
        eTrack          = 0x2,
        ePlayPlayList   = 0x4
    }eCDPlayItemType;

    typedef enum eCDPlayDirection
    {
        ePrev       = 0x1,
        eCurrent    = 0x2,
        eNext       = 0x4
    }eCDPlayDirection;

    typedef enum eCDActionContext
    {
        eNoContext  = 0x0,
        eShuffle    = 0x1,
        eRepeat     = 0x2,
        eScan       = 0x4
    }eCDActionContext;

    typedef enum eCDActionContextValue
    {
        eInvalid    = 0x0,
        eOn         = 0x1,
        eOff        = 0x2,
        eOnFolder   = 0x3
    }eCDActionContextValue;

    typedef pair < eCDActionContext, eCDActionContextValue >    CDPlayContext ;

    struct CDPlayItem
    {
        eCDPlayDirection    direction_ ;
        eCDPlayItemType     type_ ;               
        CDPlayItemNumber    number_ ;

        // constructors
        CDPlayItem ( ) : direction_ ( eCurrent ), type_ ( eTrack ), number_ ( { 0, 0 } )    { }
        CDPlayItem ( eCDPlayDirection direction ) : direction_ ( direction ), type_ ( eTrack ), number_ ( { 0, 0 } )    { }
        CDPlayItem ( eCDPlayDirection direction, eCDPlayItemType type ) : direction_ ( direction ), type_ ( type ), number_ ( { 0, 0 } )    { }
        CDPlayItem ( eCDPlayDirection direction, eCDPlayItemType type, CDPlayItemNumber number ) : direction_ ( direction ), type_ ( type ), number_ ( number )    { }
    };

    struct CDActiveFolder
    {
        FolderSummary summary ;
        FolderPath    path ;
    };
   
    typedef enum eNowPlayingNameType
    {
        eAlbumName      = 0x1,
        eArtistName     = 0x2,
        eGenreName      = 0x4,
        eFileName       = 0x8,
        eTrackName      = 0x10,
        eFolderName     = 0x20,
        eAllNames       = 0x1F      // value indicates all above data presence except FolderName
    }eNowPlayingNameType;
    
    typedef pair < eNowPlayingNameType, string >     nowPlayingName ;
    struct CDNowPlaying
    {
        int       numberOfTracks_ ;
        int       trackNumber_ ;
        vector < nowPlayingName > names_ ;
        int       namesAvailable_ ;         // bitwise vector which indicates the names arrived so far
    };

	typedef pair < uint16_t, uint16_t >   TrackTime ;

