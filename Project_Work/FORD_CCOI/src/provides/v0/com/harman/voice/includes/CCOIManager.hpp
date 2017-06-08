#ifndef __CCOI_MANAGER_HPP_
#define __CCOI_MANAGER_HPP_

#include <unistd.h> 
#include <queue>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <mutex>
#include <memory>
#include <fstream>
#include <map>
#include <CommonAPI/CommonAPI.hpp>

#include "EntityElement.hpp"
#include "CCOIPersist.hpp"
#include "CCOIButton.hpp"
#include "CCOIMegaTP.hpp"
#include "CCOITimer.hpp"
#include "CCOIProxyManager.hpp"
#include "v2/com/harman/voice/CCOIPresCtrlStubImpl.hpp"
#include "CCOILog.hpp"

// #define TESTING      // uncomment this when ccoi module is in simulation mode and not using CAN or Tcu
#ifdef TESTING
#include "CCOIMockTcu.hpp"

// mock tcu instance
extern shared_ptr < v0::com::harman::voice::CCOIMockTcu > mockTcu ;
#endif

// namespace declaratives
using namespace std ;
using namespace v2::com::harman::voice ;

namespace v0 {
    namespace com {
        namespace harman {
            namespace voice {

/*****   MACRO definitions   *****/
#define RESET                                   ( -1 )
#define SET_ON                                  (  1 )
#define SET_OFF                                 (  0 )
                
#define HMI_BIT_POS                             (  0 )
#define TCU_BIT_POS                             (  1 )

#define PTE_BIT_POS                             (  0 )
#define UFM_BIT_POS                             (  1 )
#define SYNC_BIT_POS                            (  2 )

#define SYNC                                    (  1 )
#define SETTINGS                                (  1 )
#define TCU_HEART_BEAT_START                    ( 5 * 1000 )
#define TCU_HEART_BEAT_PERIOD                   ( 2 * 1000 )
#define SYNCHRONIZATION_TIMEOUT                 ( 1200 * 1000 )    // 120 seconds change after TCU is available as per the requirement to 5 secs
#define SUMMARY_PER_FILE_BYTES( type )          ( ( type == SYNC )? 46 : 14 )    // for sync summary includes 32 bytes of hash along with 14 bytes of header, for settingsUpdateSummary there is no hash required, so 14 bytes
#define USERPROMPT_DATA_LENGTH                  ( 9 + ( VARIABLE_TEXT_LENGTH + 1 ) * 2 )
/***** ENUMS & STRUCT DEFINITIONS   *****/

// OnBoardHMISystemPolicyClient_St or OnBoardHMISystemPolicyServer_St enums
typedef enum _CCOI_STATE_MACHINE_STATES
{
    CCOI_INVALID                                        = 0x00,
    CCOI_WAITING                                        = 0x01,
    CCOI_ONBOARD_UPDATE_PENDING                         = 0x02,
    CCOI_OFFBOARD_UPDATE_PENDING                        = 0x03,
    CCOI_SYNC_NEEDED                                    = 0x04,
    CCOI_SYNCHRONIZING                                  = 0x05,
    CCOI_SYNCHRONIZED                                   = 0x06,
    CCOI_SYNC_FAILED                                    = 0x08,
    CCOI_UNRECOVERABLE_SYNC_ERROR                       = 0x09,
    CCOI_ONBOARD_DISTRIBUTED_STATE_MACHINE_INCONSISTENT = 0x0A,
    CCOI_DATA_STORAGE_ERROR                             = 0x0B,
	CCOI_RESERVED1                                      = 0x07,
	CCOI_RESERVED2                                      = 0x0C,
	CCOI_RESERVED3                                      = 0x0D,
	CCOI_RESERVED4                                      = 0x0E,
	CCOI_RESERVED5                                      = 0x0F,
    ANY_STATE                                           = 0x1000
} CCOI_STATE_MACHINE_STATES ;

// tcu data indicators
typedef enum _eTCUData
{
    eNoData         = 0x0,  // no bit
    ePTE            = 0x1,  // bit  0
    eUFM            = 0x2,  // bit  1
    eSyncSettings   = 0x4,  // bit  2
    eFullData       = 0x7,  // we conside only PTE, UFM & SyncSettings recived as the full data, as userPrompt is optional
    eUserPrompt     = 0x8
} eTCUData ;

// STATE MACHINE events
typedef enum _eCcoiEvents
{
    eventHMI                        =    0x1,
    eventTCU                        =    0x2,
    eventTcuState                   =    0x4,
    eventSyncNeeded                 =    0x8,
    eventPTE                        =   0x10,
    eventUFM                        =   0x20,
    eventSyncSettings               =   0x40,
    eventLocalUserPrompt            =   0x80,
    eventTcuUserPrompt              =  0x100,
    eventUserPromptResponse         =  0x200,
    eventUserSettingsResponse       =  0x400,
    eventSendUserPromptResponse     =  0x800,
    eventSendUserSettingsResponse   = 0x1000
} eCcoiEvents ;

// preconditions indicators
typedef enum _ePreConditions
{
    eNone        = 0x0,   // no bit
    eHMI         = 0x1,   // bit 0
    eTCU         = 0x2,   // bit 1
    eIgnitionOn  = 0x4,   // bit 2
    eNormalMode  = 0x8,   // bit 3
    eCCSEnabled  = 0x10,
    eAll         = 0xF    // TODO: Change this to include CCSEnabled after the DID is available in 1.5 spec
} ePreConditions ;

// indicators about user nature of choice in settings menu
typedef enum entityUserChoice
{
    SELECTED  = 0x0,
    CANCELLED = 0x1,
    CONFIRMED = 0x2
} entityUserChoice;

// definition of a state machine event has the metadata  - event, state and the params to the handler
struct eventStateQueueItem
{
   int                          event_  ;
   CCOI_STATE_MACHINE_STATES    state_  ;
   int                          param1_ ;
   string                       param2_ ;

   eventStateQueueItem () : event_ ( 0 ), state_ ( CCOI_INVALID ), param1_ ( 0 ), param2_ ("")  { }
   eventStateQueueItem ( int event, CCOI_STATE_MACHINE_STATES state, int param1, string param2 ) : event_ ( event ), state_ ( state ), param1_ ( param1 ), param2_ ( param2 )    { }

} ;

// function pointer used as handler of the event in the state machine 
typedef function < bool ( int, string value ) > ccoiFunction ;
// Menu buttons are treated as entities in this component to be in sync with the usage of names in TCU etc,.
typedef shared_ptr < CCOIMenuButton > entityPtr ;
typedef unique_lock < mutex > ScopedLock ;

// CCOI heart class starts
class CCOIManager 
{
    private:

        // precondition flags
        int preConditions_ ;
    
	    // CCOIManager state
        CCOI_STATE_MACHINE_STATES prevState_ ;
        CCOI_STATE_MACHINE_STATES currentState_ ;
    
        //setters
        void    setCcoiState ( CCOI_STATE_MACHINE_STATES state ) ;

        // getters
        inline CCOI_STATE_MACHINE_STATES getCcoiState ( )       {   return currentState_ ;    }

	    // TCU data related
        static unordered_map < int, eCcoiEvents >   tcuMap_ ;
        int ccoiData_ ;
        
        inline int  getCcoiData ( )                             {   return ccoiData_ ;          }

        // state machine run on separate thread
        thread stateMachine_ ;
        // synchronization mechanism between CCOI state machine thread and outside
        mutex sync_ ;
        
        // retries can be any number so safe to put it in int 
        int numRetries_ ;

    public:
        // constructor
        CCOIManager ( ) ;
        // constructor
        ~CCOIManager ( ) ;

        // CCOI state machine implementation
        void start ( void ) ;

    private:
	    /*****   STATE MACHINE related   *****/
        queue < shared_ptr < eventStateQueueItem > >                      events_   ;
        map   < pair < int, CCOI_STATE_MACHINE_STATES >, ccoiFunction >   eventStateMap_ ;
    
        void    addEvent ( shared_ptr < eventStateQueueItem > item )        { events_. push ( item ) ;   }

        inline  shared_ptr < eventStateQueueItem > getEvent ( )             { if ( !events_. empty () )    return events_. front () ; else    return nullptr ; }
        inline  void removeEvent ( )                                        { events_. pop () ;          }

        /*****   STATE MACHINE FUNCTIONS   *****/
        bool    PTEReceived ( int value, string name ) ;
        bool    UFMReceived ( int value, string name ) ;
        bool    syncNeeded ( int param, string value ) ;
        bool    preConditions ( int param1, string param2 ) ;
        bool    userPromptReceived ( int value, string name ) ;
        bool    syncSettingsReceived ( int value, string name ) ;
        bool    tcuStateReceived ( int tcuState, string value ) ;
        bool    userPromptResponseReceived ( int choice, string requestToken ) ;
        bool    userSettingsResponseReceived ( int status, string entityList ) ;

    public:
        /*****   EXTERNAL calls from CCOIUserPrompt, proxyManager, CCOIPresCtrl   *****/

        bool    onMasterReset ( ) ;
        void    setTCUStatus ( bool avbl ) ;
        void    setHMIStatus ( bool avbl ) ;
        void    onTcuStateReceived ( int tcuState ) ;
        void    onDataReceived ( bool fileType, int dataType, string &value ) ;
        void    onUserPromptResponseReceived ( int status, const string &requestToken ) ;
        void    onUserSettingsResponseReceived ( int status, vector < entityPtr > &entityList ) ;

    private:
        /*****  functions to send responses to the clients i.e., TCU ( or HMI )   *****/
        void    sendUserPromptResponse ( int choice, string &requestToken ) ;
        void    sendUserSettingsResponse ( int param1, string &entityList ) ;
        bool    sendSyncSummaryResponse ( bool request, bool Default = false ) ;
        
        /****   UTILITY functions   *****/
        void    init ( ) ;
        bool    isCCSCapable () ;
        void    initStateMachine () ;
        string  toHex ( char val ) ;
        bool    isErrorState ( int state ) ;
        bool    initPrompt ( string &data ) ;
        void    updateTcuVersion ( string &name ) ;
        bool    updateFlag ( string name, int data, int dataBitPos ) ;
        int     fileReceived ( int fileType, int value, string &name ) ;
        void    logErrorConditions ( int status, bool ccoiError = false ) ;
        bool    updateUserPromptResponse ( int choice, string &requestToken ) ;
        bool    prepareSettingsUpdateList ( vector < entityPtr > &entityList, string &response ) ;
        bool    prepareSummaryResponse ( int fileType, int summaryType, string &response, bool Default = false ) ;

    private:
        // holds timer handle for pre conditions timeout
        CCOITimer::timer_id tcuHeartBeatTimer_ ;
        // holds timer handle for synchronization timeout mechanism
        CCOITimer::timer_id syncTimer_ ;

    public:
        void    tcuHeartBeatTimeOut () ;
        void    CcoiSynchronizationTimeOut () ;
};
// ccoi heart class ends

            } // namespace voice
        } // namespace harman
    } // namespace com
} // namespace v0

#endif // __CCOI_MANAGER_HPP_
