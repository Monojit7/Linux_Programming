#include "CCOIManager.hpp"

/***** EXTERN declarations *****/

extern CCOITimer timer ;                                                         // CCOI timer class global object 
extern shared_ptr < v0::com::harman::voice::CCOIProxyManager >     proxyFac ;    // proxy factory global object 
extern shared_ptr < v2::com::harman::voice::CCOIPresCtrlStubImpl > presCtrl ;    // CCOI provides shared instance

namespace v0 {
    namespace com {
        namespace harman {
            namespace voice {

// global instance of the logger - will be used by all class/ modules in the CCOI component.
CCOILog CcoiLog ;

// Has the state machine handlers based on the events & the state of the CCOI
void CCOIManager::initStateMachine ( )
{
    eventStateMap_ = 
    {
        /*    Event                             State                   HandlerFunction                                                                                           */
        {   { eventHMI,              	        CCOI_INVALID       },    bind ( &CCOIManager::preConditions,                this, placeholders::_1, placeholders::_2 )    },
        {   { eventTCU,              	        CCOI_INVALID       },    bind ( &CCOIManager::preConditions,                this, placeholders::_1, placeholders::_2 )    },
        {   { eventTCU,              	        CCOI_WAITING       },    bind ( &CCOIManager::preConditions,                this, placeholders::_1, placeholders::_2 )    },
        {   { eventTCU,              	        CCOI_SYNC_NEEDED   },    bind ( &CCOIManager::preConditions,                this, placeholders::_1, placeholders::_2 )    },
        {   { eventTcuState,                    ANY_STATE          },    bind ( &CCOIManager::tcuStateReceived,             this, placeholders::_1, placeholders::_2 )    },
        {   { eventSyncNeeded,                  ANY_STATE          },    bind ( &CCOIManager::syncNeeded,                   this, placeholders::_1, placeholders::_2 )    },
        {   { eventPTE,                  	    CCOI_SYNCHRONIZING },    bind ( &CCOIManager::PTEReceived,                  this, placeholders::_1, placeholders::_2 )    },
        {   { eventUFM,                  	    CCOI_SYNCHRONIZING },    bind ( &CCOIManager::UFMReceived,                  this, placeholders::_1, placeholders::_2 )    },
        {   { eventSyncSettings,                CCOI_SYNCHRONIZING },    bind ( &CCOIManager::syncSettingsReceived,         this, placeholders::_1, placeholders::_2 )    },
        {   { eventLocalUserPrompt,             CCOI_WAITING       },    bind ( &CCOIManager::userPromptReceived,           this, placeholders::_1, placeholders::_2 )    },
        {   { eventTcuUserPrompt,               CCOI_SYNCHRONIZED  },    bind ( &CCOIManager::userPromptReceived,           this, placeholders::_1, placeholders::_2 )    },
        {   { eventUserPromptResponse,          ANY_STATE          },    bind ( &CCOIManager::userPromptResponseReceived,   this, placeholders::_1, placeholders::_2 )    },
        {   { eventUserSettingsResponse,        ANY_STATE          },    bind ( &CCOIManager::userSettingsResponseReceived, this, placeholders::_1, placeholders::_2 )    }
    };
}

// CCOI state machine implementation
void CCOIManager::start ( void )
{
	while ( 1 )
	{
        {                                   // do not remove this local block else will give warning due to scoped mutex lock over a sleep call
            ScopedLock lock ( sync_ ) ;
            
            shared_ptr < eventStateQueueItem > item = getEvent () ;
            if ( item != nullptr )
            {
                auto eventState = make_pair ( item-> event_, item-> state_ ) ;

                auto handler = eventStateMap_. find ( eventState ) ;

                if ( handler != eventStateMap_. end () )    handler-> second ( item-> param1_, item-> param2_ ) ;
                
                // processed the event remove it from the queue
                removeEvent () ;
                CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "event %d state %d %shandled param1 %d param2 %s", item-> event_, item-> state_, 
                                                                              ( handler != eventStateMap_. end () )? "" : "not ", item-> param1_, item-> param2_. c_str () ) ;
            }
            else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_DEBUG, "%s:%d No new Event to process\n", __FUNCTION__, __LINE__ ) ;
        }
        usleep ( 10000 ) ;
	}
}

// constructor
CCOIManager::CCOIManager ( )
{
	//setup the logger
	CcoiLog.init ( ) ;

    // initialize the member variables	
    init () ;
    initStateMachine () ;

    // create the timer to know the tcu heart beat failure which comes every 1 second
    //tcuHeartBeatTimer_ = timer. create ( TCU_HEART_BEAT_START, 0, bind ( &CCOIManager::tcuHeartBeatTimeOut, this ) ) ;
    
    // start state machine thread 
    stateMachine_ = thread ( bind ( &CCOIManager::start, this ) ) ;

    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d", __FUNCTION__, __LINE__ ) ;
}

/*****   STATE MACHINE FUNCTIONS   *****/
bool CCOIManager::preConditions ( int param1, string param2 )
{
    // updates the preConditions flag, send the corresponding bit position as last param
    updateFlag ( "preConditions", param1, ( param2. empty () )? HMI_BIT_POS : TCU_BIT_POS ) ;

    // means TCU ESN is informed so set it to versionManager via proxyManager
    if ( ! param2. empty () )    updateTcuVersion ( param2 ) ;
    else
    { 
        if ( preConditions_ & ePreConditions::eHMI )
        {
            setCcoiState ( CCOI_WAITING ) ;
            
            //check if CCS enabled if it is we have to show our partial prompt screen else EuLa screen would be showed by SWDL
            if ( isCCSCapable () )
            {
                string data ;
                if ( initPrompt ( data ) )    addEvent ( make_shared < eventStateQueueItem > ( eventLocalUserPrompt, currentState_, 0, data ) ) ;
            }
        }
    }
    return true ;
}

bool CCOIManager::PTEReceived ( int value, string name )
{
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d value %d name %s", __FUNCTION__, __LINE__, value, name. c_str () ) ; 
    return fileReceived ( ePTE, value, name ) ;
}

bool CCOIManager::UFMReceived ( int value, string name )
{
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d value %d name %s", __FUNCTION__, __LINE__, name. c_str () ) ; 
	return fileReceived ( eUFM, value, name ) ;
}

bool CCOIManager::syncSettingsReceived ( int value, string name )
{
    bool success = false ;
    // update the tcuData flag  with eSyncSettings, 3rd param - bit pos of syncSettings
    updateFlag ( "tcuData", SET_ON, SYNC_BIT_POS ) ;

    if ( ( getCcoiData () == eFullData ) )
	{
        // full data received from TCU now process and populate it for our display
		CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d FullData received in CCOI", __FUNCTION__, __LINE__ ) ; 
        // summary report only if the entity persistency is sucessful
        CCOIPersist persist ;
        if ( persist. persistSettingsData ( name ) )  
        {
            // cancel the sync timeout timer 
            timer. destroy ( syncTimer_ ) ;
            if ( presCtrl )
            {
                presCtrl-> loadEntities () ;
                // get entries to be processed from PTE & UFM files based on the tag "menuPolicies" in the json, 
                // messageUtil component does this job for us and after populating will call createCCSMenuFile back to persist the contents into 
                CCOIMessagesUtil msgUtil ;
                msgUtil. populateEntries ( "menuPolicies", presCtrl-> getEntities () ) ;
                
                presCtrl-> loadHMIData () ;
                
                sendSyncSummaryResponse ( false ) ;
                setCcoiState ( CCOI_SYNCHRONIZED ) ;
                // refresh the tcuData data flags for the next cycle
                updateFlag ( "tcuData", RESET, 0 ) ;

                success = true ;
            }
            else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "%s:%d presCtrl pointer not available check the creation in main.cpp", __FUNCTION__, __LINE__ ) ;
        }
        else
        {
            //persistent storage failed, set the state & send to TCU to flag this.
            setCcoiState ( CCOI_DATA_STORAGE_ERROR ) ;
            updateFlag ( "tcuData", SET_OFF, SYNC_BIT_POS ) ;
        }
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "%s:%d Partial Data received, PTE or UFM with right hash values missing from TCU", __FUNCTION__, __LINE__ ) ;

    return success ;
}

bool CCOIManager::userPromptReceived ( int value, string name )
{
    string data ;
    CCOIUserPromptScreenData userPrompt ( bind ( &CCOIManager::onUserPromptResponseReceived, this, placeholders::_1, placeholders::_2 ) ) ;

    CCOIMessagesUtil msgUtil ;
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d value %d", __FUNCTION__, __LINE__, value ) ;
    
    // coming from tcu as a file so read the contents and push it for msgUtil component for parsing and display trigger
    if ( value == 1 )
    {
        ifstream stream ( name. c_str () ) ;
        if ( stream. is_open () )
        {
            char ch = '\0' ;
            while ( stream. get ( ch ) )   data += ch ;
            stream. close () ;
        }
        else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d file %s open error", __FUNCTION__, __LINE__, name. c_str () ) ;
    }
    else    data = name ;

    if ( msgUtil. populateUserPrompt ( data, userPrompt, true ) )
    {
        userPrompt. addScreen () ;
        //send the user prompt info to HMI
        userPrompt. sendDataToHMI () ;
    }

    return true ;
}

bool CCOIManager::userSettingsResponseReceived ( int status, string entityList )
{
    // first indicate that user update is pending, but user has not yet updated will come later
    if ( status == SELECTED )   setCcoiState ( CCOI_ONBOARD_UPDATE_PENDING ) ;
    
    // user has cancelled his choice, so go back to synchronized state and inform TCU also about the state
    else if ( status == CANCELLED )    setCcoiState ( CCOI_SYNCHRONIZED ) ;

    // user has confirmed his change in the prompt, or this entity did not require any consent prompt upon user selection on the menu entity toggle choice
    else
    {
        sendUserSettingsResponse ( status, entityList ) ;
    }

    return true ;
}

bool CCOIManager::userPromptResponseReceived ( int choice, string requestToken )
{
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d choice %d requestToken %s", __FUNCTION__, __LINE__, choice, requestToken. c_str () ) ;
    if ( choice != ASK_ME_LATER )
    {
        // user had made a decision already, set automatic sofwtare updates to ON, if user has opted-in for the updates else false if the user is opted out
        if ( proxyFac )    proxyFac-> setAutomaticUpdates ( ( choice == YES_BUTTON )? true : false ) ;

        // customer has chosen YES in the userPrompt/ EULA screens, so enable the features based on prompt_policies in the PTE message.
        if ( ( choice == YES_BUTTON ) || ( choice == YES_PLUS_CHECKED ) )
        {
            CCOIMessagesUtil msgUtil ;
            msgUtil. populateEntries ( "menuPolicies", presCtrl-> getEntities () ) ;
        }
    }

    return updateUserPromptResponse ( choice, requestToken ) ;
}

bool CCOIManager::tcuStateReceived ( int tcuState, string value )
{
    bool followTcu = true ;

    if ( ! isErrorState ( tcuState ) )
    { 
        if ( tcuState == CCOI_SYNCHRONIZING )
        {
            // sync process started start the timer for keeping track of time taken for synchronization
            if ( getCcoiState () == CCOI_SYNC_NEEDED )    syncTimer_ = timer. create ( SYNCHRONIZATION_TIMEOUT, 0, bind ( &CCOIManager::CcoiSynchronizationTimeOut, this ) ) ;
            
            else
            {
                CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d Tcu sent CCOI_SYNCHRONIZING state but CCOI module not in SYNC_NEEDED state", __FUNCTION__, __LINE__ ) ;
                setCcoiState ( CCOI_ONBOARD_DISTRIBUTED_STATE_MACHINE_INCONSISTENT ) ;
                followTcu = false ;
            }
        }
    } 
    // tcu reports error states, currently we just log it, no requirement on the action based on this
    else    logErrorConditions ( tcuState ) ;

    if ( followTcu )    setCcoiState ( static_cast < CCOI_STATE_MACHINE_STATES > ( tcuState ) ) ;
    
    if ( tcuState == CCOI_SYNC_NEEDED )    addEvent ( make_shared < eventStateQueueItem > ( eventSyncNeeded, ANY_STATE, 1, "" ) ) ;

    return true ;
}

bool CCOIManager::syncNeeded ( int param, string value )
{
    // first set to an onboardUpdate pending state & inform TCU, param = 1 indicates the syncNeeded is because of CCOI_OFFBOARD_UPDATE_PENDING from tcu so no need to set the state to below.
    if ( param != 1 )    setCcoiState ( CCOI_ONBOARD_UPDATE_PENDING ) ;

    // then say sync needed for TCU to send the data
    setCcoiState ( CCOI_SYNC_NEEDED ) ;
    
    ifstream hdr ( PTEHDR_BIN ) ;
    // send the data we have so as tcu will check and validate our data and send the updated ones, second param is to determine to take from default file or if persist files exists
    sendSyncSummaryResponse ( true, ! hdr. is_open () ) ;
    
    if ( hdr. is_open () )    hdr. close () ;

    return true ;
}

/*****   EXTERNAL calls from CCOIUserPrompt, proxyManager, CCOIPresCtrl   *****/

// ProxyManager calls this function to intimate the HMI proxy availability
void CCOIManager::setHMIStatus ( bool avbl )
{
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d avbl %d", __FUNCTION__, __LINE__, avbl ) ;

    ScopedLock lock ( sync_ ) ;

    addEvent ( make_shared < eventStateQueueItem > ( eventHMI, currentState_, avbl, "" ) ) ;
}

// MegaTP calls this function to update the tcu data received
void CCOIManager::onDataReceived ( bool fileType, int dataType, string &value )
{
    if ( !value. empty () )
    {
        auto messageId = tcuMap_. find ( dataType ) ;
        if ( messageId != tcuMap_. end () )
        {
            ScopedLock lock ( sync_ );
            // userprompt data can come in any state
            addEvent ( make_shared < eventStateQueueItem > ( messageId-> second, ( dataType == eUserPrompt )? ANY_STATE : currentState_, fileType, value ) ) ;
        }

        else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "Unknown MessageId %d sent from megaTP ", dataType ) ;
    }

    else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "%s:%d value empty from megaTP fileType %d dataType %d", __FUNCTION__, __LINE__, fileType, dataType ) ;
}

// ProxyManager calls this function to intimate TCU state, this shall come periodically
void CCOIManager::onTcuStateReceived ( int tcuState )
{
    // uncomment the below code lines at the time of tcu intergation, at present disabled as without tcu the below would make customerOptIn not to work 
    // tcu State received so refresh the tcu heartbeat timer 
//    timer. destroy ( tcuHeartBeatTimer_ ) ;
//    tcuHeartBeatTimer_ = timer. create ( TCU_HEART_BEAT_START, 0, bind ( &CCOIManager::tcuHeartBeatTimeOut, this ) ) ;

    switch ( tcuState )
    {
        case CCOI_INVALID :
        case CCOI_WAITING :
        case CCOI_ONBOARD_UPDATE_PENDING :
        case CCOI_OFFBOARD_UPDATE_PENDING :
        case CCOI_SYNC_NEEDED :
        case CCOI_SYNCHRONIZING :
        case CCOI_SYNCHRONIZED :
        case CCOI_SYNC_FAILED :
        case CCOI_UNRECOVERABLE_SYNC_ERROR :
        case CCOI_ONBOARD_DISTRIBUTED_STATE_MACHINE_INCONSISTENT :
        case CCOI_DATA_STORAGE_ERROR :

            addEvent ( make_shared < eventStateQueueItem > ( eventTcuState, ANY_STATE, tcuState, "" ) ) ;
            break ;
        default :
            CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "%s:%d Incorrect state %d received from tcu", __FUNCTION__, __LINE__, tcuState ) ;
            break ;
    }
}

// ProxyManager calls this function to intimate a factory reset has been triggered by the user
bool CCOIManager::onMasterReset ( )
{
    // just change the state machine, as HMI will not show any details until we go to synchoronized and entities would as usual have been updated correctly during sync.
    // TODO: check if we have something as masterReset call to inform TCU., so as to ask TCU to send the files/ data afresh.
    
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d", __FUNCTION__, __LINE__ ) ;

    addEvent ( make_shared < eventStateQueueItem > ( eventSyncNeeded, ANY_STATE, 0, "" ) ) ;

    return true ;
}

void CCOIManager::onUserSettingsResponseReceived ( int status, vector < entityPtr > &entityList )
{
    string response ;
    
    // we need list only when user confirmed a change else go ahead and raise the event so as to update the state machine
    if ( ( prepareSettingsUpdateList ( entityList, response ) ) || ( status != CONFIRMED ) )
    {
        addEvent ( make_shared < eventStateQueueItem > ( eventUserSettingsResponse, ANY_STATE, status, response ) ) ;
    }
}

void CCOIManager::onUserPromptResponseReceived ( int status, const string &requestToken )
{
    addEvent ( make_shared < eventStateQueueItem > ( eventUserPromptResponse, ANY_STATE, status, requestToken ) ) ;
}

/*****   update or response calls to the clients i.e., TCU ( or HMI )   *****/

// This function sends synchronization smmary report to TCU via megaTP
bool CCOIManager::sendSyncSummaryResponse ( bool request, bool Default )
{
    string response ;
    response.reserve ( SUMMARY_PER_FILE_BYTES ( SYNC ) * 2 + 2 ) ;  // reserve the memory to hold to headers 1. - PTE, 2. - UFM

    bool success = prepareSummaryResponse ( ePTE, SYNC, response, Default ) && prepareSummaryResponse ( eUFM, SYNC, response, Default ) ;
    if ( success )
    {
        // now send the report to tcu via megaTP
        CCOIMegaTP tcu ;

        tcu. sendData ( response.length (), const_cast < char * > ( response. c_str () ), ( request )? CCOI_SYNCHRONIZATION_SESSION_RQ_TYPE : CCOI_SYNCHRONIZATION_SUMMARY_REPORT_TYPE ) ;
        CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d length %d response %s", __FUNCTION__, __LINE__, response.length (), response.c_str () ) ;
    }

    return success ;
}

// This function sends user prompt response to TCU via megaTP
void CCOIManager::sendUserPromptResponse ( int choice, string &requestToken ) 
{
    string response = requestToken ;
    
    response. push_back ( static_cast < char > ( choice ) ) ;

    // now send the report to tcu via megaTP
    CCOIMegaTP tcu ;

    tcu.sendData ( response.length (), const_cast < char * > ( response. c_str () ), CCOI_USERPROMPT_RESPONSE_TYPE ) ;
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d length %d response %s", __FUNCTION__, __LINE__, response.length (), response.c_str () ) ;
}

// This function sends user prompt response to TCU via megaTP
void CCOIManager::sendUserSettingsResponse ( int param1, string &entityList )
{
    string response ;
    response.reserve ( SUMMARY_PER_FILE_BYTES ( SETTINGS ) * 2 + 10 ) ;  // reserve the memory to hold to headers 1. - PTE, 2. - UFM

    bool success = prepareSummaryResponse ( ePTE, SETTINGS, response ) && prepareSummaryResponse ( eUFM, SETTINGS, response ) ;
    
    if ( success )    
    {
        response. append ( entityList ) ;
        // now send the report to tcu via megaTP
        CCOIMegaTP tcu ;

        tcu.sendData ( response.length (), const_cast < char * > ( response. c_str () ), CCOI_SETTINGS_UPDATE_RQ_TYPE ) ;
        CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d length %d", __FUNCTION__, __LINE__, response.length () ) ;
        
        // updates sent to TCU, so now sync needed with TCU so start it
        addEvent ( make_shared < eventStateQueueItem > ( eventSyncNeeded, ANY_STATE, 0, "" ) ) ;
    }
    
    else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "%s:%d settingsUpdate not sent to TCU as header retrieve failed", __FUNCTION__, __LINE__ ) ; 
}

// This function sets the state locally, also sends the state to TCU & HMI (via CCOIPresCtrl), also logs error states for diagnostic purposes
void CCOIManager::setCcoiState( CCOI_STATE_MACHINE_STATES state )
{
	CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "Ccoi State change prev %d curr %d new %d\n", prevState_, currentState_, state) ; 

    prevState_ = currentState_ ;
    currentState_  = state;

    // Notify other sub components dependent on this.
    if ( presCtrl )    presCtrl-> setSyncStatus ( currentState_ ) ;

    // notify TCU about the CCOI state change, so send ccoi state to TCU via notif alert proxy
    if ( proxyFac )    proxyFac-> sendNotifAlert ( static_cast < int > ( currentState_ ) ) ;

    // log CCOI error states
    if ( isErrorState ( static_cast < int > ( state ) ) )    logErrorConditions ( currentState_, true /* ccoiError = true */ ) ;

}

/****   HELPERS/ UTILITY functions   *****/

// initializes the instance variables
void CCOIManager::init ( void )
{
    // ccoi states
    currentState_ = prevState_ = CCOI_INVALID ;

    // flag for clients handles we as a middleware has only 2 clients, 1 - UI ( i.e., HMI ), 2 - PLATFORM ( i.e., TCU ), so wait for them to come up and update
    updateFlag ( "preConditions", RESET , 0 ) ;

    // reset the tcuData flag, data - 0, dataBitPos - 0
    updateFlag ( "tcuData", RESET, 0 ) ;

    ifstream userPromptResponse ( USER_PROMPT_RESPONSE_FILE ) ;
    if ( userPromptResponse. is_open () )
    {
        char ch = '\0' ;
        string retries ;
        while ( userPromptResponse. get ( ch ) )    retries += ch ;
        
        numRetries_ = atoi ( retries. c_str () ) ;
        userPromptResponse. close () ;
    }
    else    
    {   // take numberOfRetries from PTE file for corresponding messageCode for partial prompt it is "0"  
        CCOIMessagesUtil msgUtil ;
        numRetries_ = atoi ( msgUtil. getValue ( "promptPolicies", "0", "numberOfRetries", true ). c_str () ) ;
    }
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d userPrompt retries left %d", __FUNCTION__, __LINE__, numRetries_ ) ;
}

/** Function to update the bitwise flags (currently used for preconditions & tcuData flags )
 *  @ param (IN): name - name used to determine which flag to updateFlag (preConditions_ or ccoiData_ )
 *  @ param (IN): avbl - if 1, the data corresponding to the data position has to set to ON (1), if 0 - the bit is set to OFF (0), if -1 - the flag is reset to 0.
 *  @ param (IN): dataBitPos - the data positoin in the flag which needs to set or reset.
 *  output: respective flag is updated.
 */
bool CCOIManager::updateFlag ( string name, int data, int dataBitPos )
{
    if ( name == "preConditions" )
    {
        // resets the flag of preconditions
        if ( data == -1 )         preConditions_ &= ePreConditions::eNone ;
        // sets the data bit to ON corresponding to the dataBit position
        else if ( data ==  1 )    preConditions_ |= 1u << dataBitPos ;
        // clears the data bit corresponding to the dataBit position
        else                      preConditions_ &= ~( 1u << dataBitPos ) ;
    }
    else if ( name == "tcuData" )
    {
        // resets the flag of tcu data
        if ( data == -1 )         ccoiData_ &= eTCUData::eNoData ;
        // sets the data bit to ON corresponding to the dataBit position
        else if ( data ==  1 )    ccoiData_ |= ( 1u << dataBitPos ) ;
        // clears the data bit corresponding to the dataBit position
        else                      ccoiData_ &= ~( 1u << dataBitPos ) ;
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "%s:%d unknown data or client update, name: %s", __FUNCTION__, __LINE__, name ) ;

    return true ;
}

// processes the file recived ( either PTE or UFM )
int	CCOIManager::fileReceived ( int fileType, int value, string &name )
{
    // if it is good file with valid data, update the tcuData flag PTE/ UFM bit position is set to ON
    CCOIPersist persist ;
    if ( persist. persistFiles ( fileType, name ) )   updateFlag ( "tcuData", SET_ON, ( fileType == ePTE) ? PTE_BIT_POS : UFM_BIT_POS ) ;
    else
    {
        CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "%s:%d Not able to persist file due to hash mismatch or no persistent data etc,. name %s", __FUNCTION__, __LINE__, name. c_str () ) ;
        // hash value mismatch failed, enter in to sync failed state and inform TCU about it.
        setCcoiState ( CCOI_SYNC_FAILED ) ;
        // update the tcuData flag, PTE bit position is set to OFF
        updateFlag ( "tcuData", SET_OFF, ( fileType == ePTE) ? PTE_BIT_POS : UFM_BIT_POS ) ;
    }
    
    // this means the name is a file, as we have processed the file data now, remove the file ( temp file created megaTp ) 
    if ( value )    remove ( name. c_str () ) ;

    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d ccoiData %d", __FUNCTION__, __LINE__, ccoiData_ ) ; 
    return ( ccoiData_ & fileType ) ;
}

// prepares the summary response for sync session based on the headers of the persisted data in CCOI
bool CCOIManager::prepareSummaryResponse ( int fileType, int summaryType, string &response, bool Default )
{
    string hdrpath ;
    
    if ( Default )  hdrpath  =  ( fileType == ePTE )? DEFAULT_PTEHDR_BIN : DEFAULT_UFMHDR_BIN ;
    else            hdrpath  =  ( fileType == ePTE )? PTEHDR_BIN : UFMHDR_BIN ;
    
    ifstream hdr ( hdrpath. c_str () ) ;

    int bytes = 0 ;
    if ( hdr. is_open () )
    {
        // the first 2 bytes in header are signalIdentifier & character coding, we do not require that for summaryResponse
        hdr. seekg ( 2 ) ;
        char ch = '\0' ;
        for ( ; ( bytes < SUMMARY_PER_FILE_BYTES ( summaryType ) ); bytes++ )
        {
            hdr >> hex >> ch ;
            response += ch ;
        }
        hdr. close () ;
    }
    
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d hdrpath %s bytes %d", __FUNCTION__, __LINE__, hdrpath. c_str (), bytes ) ; 
    return ( bytes == SUMMARY_PER_FILE_BYTES ( summaryType ) ) ;
}

bool CCOIManager::prepareSettingsUpdateList ( vector < entityPtr > &entityList, string &response )
{
    // prepare the entityList along with userchoice and trigger sending it to tcu
    if ( entityList. size () > 0 )
    {
        for ( auto &entity : entityList )
        {
            short int result = entity-> getOnOffStatus () ;
            result <<= 10 ; // bUAllowOnOff is 1 bit and make place for entityId & entityType to be followed in last 10 bits

            char entityId    = static_cast < char > ( atoi ( entity-> getEntityId ().c_str () ) ) ;
            result |= ( ( entityId & 0x7F ) << 3 ) ; // entityId is 7 bits, and make place for entityType in last 3 bits

            char entityType  = static_cast < char > ( atoi ( entity-> getEntityType ().c_str () ) ) ;
            result |= ( entityType & 0x7 ) ;   // last 3 bits is entityType
            
            // push the packed bits to the result list 
            response. push_back ( static_cast < char > ( result & 0xFF ) ) ;
            result >>= 8 ;
            response. push_back ( static_cast < char > ( result & 0xFF ) ) ;
        }
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "%s:%d entityList empty", __FUNCTION__, __LINE__ ) ;

    return ( entityList. size () > 0 ) ;
}

// This function has the list of error states, it returns true if the input state is in the list of error states else false
bool CCOIManager::isErrorState ( int state )
{
    bool retVal = false ;

    switch ( state )
    {
        case CCOI_INVALID :
        case CCOI_SYNC_FAILED :
        case CCOI_UNRECOVERABLE_SYNC_ERROR :
        case CCOI_DATA_STORAGE_ERROR :
        case CCOI_ONBOARD_DISTRIBUTED_STATE_MACHINE_INCONSISTENT :
            
            retVal = true ;
            break ;
    }

    return retVal ;
}

// Logs the error conditions for diagnostics
void CCOIManager::logErrorConditions ( int status, bool ccoiError )
{
	CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s\n", __FUNCTION__ ) ;
    
    fstream diagFile ( DIAGNOSTIC_FILE, fstream::app ) ;
    if ( diagFile.is_open () )
    {
        diagFile << "CCOI time: " << time ( 0 ) << " Error - Origin: " ;

        if ( ccoiError )    diagFile << "CCOI," << " CCOI Curr State: " << currentState_ << ". CCOI Prev State: " << prevState_ << endl ;

        else                diagFile << "TCU," << " OnBoardPolicyServer State: " << status << endl ;

        diagFile.close () ;
    }

    else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "Unable to open file %s for diagnostics logging\n", DIAGNOSTIC_FILE ) ;
}

void CCOIManager::tcuHeartBeatTimeOut ()
{
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "%s:%d", __FUNCTION__, __LINE__ ) ;

    // heart beat missing from tcu so set the state to INVALID
    setCcoiState ( CCOI_INVALID ) ;
}

bool CCOIManager::initPrompt ( string &data )
{
    bool /* persistAvailable = true, */ bSAllowOnOff = false, isPartialPromptRequired = true ;

    ifstream entitiesStream ( ENTITIES_DATA_PATH ) ;
    if ( entitiesStream. is_open () )    entitiesStream. close () ;
    
    // else    persistAvailable = false ;
    
    // Enable this with tcu integration, commented to make tetsiung with simulator easy
    isPartialPromptRequired = ( 0 /* presCtrl && persistAvailable */ )? presCtrl-> getEntityValue ( "UserAuthorizationPending", bSAllowOnOff ) : true ;
    if ( isPartialPromptRequired )
    {
        CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d user partially authorized show our screen numRetries_ %d left for this", __FUNCTION__, __LINE__, numRetries_ ) ;
        if ( numRetries_ > 0 )
        {
            int dataPos = 0 ;
            CCOIMessagesUtil msgUtil ;
            string varText1 ( msgUtil. getName ( "ford_app",  true ) ) ; // varText1
            string varText2 ( msgUtil. getName ( "ford_brand", true ) ) ; // varText2

            data. resize ( USERPROMPT_DATA_LENGTH, '\0' ) ;

            data. insert ( dataPos, "TEMP" ) ;    // header 4 bytes skipped so kept as temp
            dataPos += 4 ;
            data. insert ( dataPos, "CCOI" ) ;    // requestTokenId
            dataPos += 4 ;
            data. insert ( dataPos, "2" ) ;   // promptType is "2" in latest UFM for partial prompt
            dataPos += 1 ;
            data. insert ( dataPos, varText1, 0, varText1. length () ) ;
            dataPos += VARIABLE_TEXT_LENGTH + 1 ;
            data. insert ( dataPos, varText2, 0, varText2. length () ) ;
        }
        else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d partial UserPrompt not shown as the numRetries is exceeded or user had already confirmed the decision", __FUNCTION__, __LINE__ ) ;
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d partial UserPrompt not shown as entityID <2, 0> UserAuthorizationPending is false", __FUNCTION__, __LINE__ ) ;

    return ( ! data. empty () ) ;
}

bool CCOIManager::updateUserPromptResponse ( int choice, string &requestToken )
{
    // check if prompt is initiatied by tcu, then send the response back to tcu
    bool tcu = ( requestToken. find ( "CCOI" ) == string::npos ) ;
    if ( !tcu ) 
    {
        if ( choice == ASK_ME_LATER )    numRetries_ -- ;
        else    numRetries_ = 0 ;   // reset the retries as user has confirmed his decision and hence should not show this popup

        ofstream userPromptResponse ( USER_PROMPT_RESPONSE_FILE ) ;
        if ( userPromptResponse. is_open () )
        {
            userPromptResponse << to_string ( numRetries_ ). c_str () ;
            userPromptResponse. close () ;
        }
    }
    else    sendUserPromptResponse ( choice, requestToken ) ;

    return true ;
}

void CCOIManager::updateTcuVersion ( string &name )
{
    ifstream version ( name. c_str () ) ;
    if ( version. is_open () )
    {
        string data ;
        char ch = '\0' ;
        
        version.seekg ( 1 ) ;
        while ( version. get ( ch ) )   data += ch ;

        proxyFac-> setTcuVersion ( data ) ;

        version. close () ;
    }
}

// checks if the system has the CCS DID enabled, returns true if so, else false 
bool CCOIManager::isCCSCapable ()
{
    // TODO: check after DID things are clarified here and return true or false based on that, at present hardcoded to true to show our screen
    bool CCS = true ;

    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d This car has CCS %s", __FUNCTION__, __LINE__, ( CCS )? "enabled" : "disabled" ) ; 
    return CCS ;
}

// This function gets called upon time out of 5 secs starting from the time point synchronizing state received from TCU
void CCOIManager::CcoiSynchronizationTimeOut ()
{
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d currentState_ %d", __FUNCTION__, __LINE__, currentState_ ) ;
    if ( currentState_ != CCOI_SYNCHRONIZED )
    {
        CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "%s:%d timeout happened and yet not in synchronized state", __FUNCTION__, __LINE__ ) ;
        
        setCcoiState( CCOI_SYNC_FAILED ) ;
    } 
}

string CCOIManager::toHex ( char val )
{
    stringstream ss;

    ss << hex << setw ( 2 ) << setfill ( '0' ) << (int) val ;
    return ss.str();
}


// map which holds the TCU signalIDs onto the CCOI ids used for identificaitojn of message types to be processed
unordered_map < int, eCcoiEvents > CCOIManager::tcuMap_ = 
{
    /* CCOIPolicyTableExtension_Rsp */      {   0x02,   eventPTE            },
    /* CCOIUserFriendlyMessages_Rsp */      {   0x03,   eventUFM            },
    /* CCOISynchronizationSettings_Rsp */   {   0x9D,   eventSyncSettings   },
    /* CCOIUserPrompt_Rq */                 {   0xA5,   eventTcuUserPrompt  },
    /* TCU_ESN number */                    {   0x87,   eventTCU            }
};

// destructor 
CCOIManager::~CCOIManager()
{
    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_ERROR, "%s:%d", __FUNCTION__, __LINE__ ) ;

    stateMachine_. join () ;
}

            } // voice
        } // harman
    } // com
} // v0
