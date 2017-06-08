
#include "CCOIProxyManager.hpp"

//#define TESTING

namespace v0    {
    namespace com    {
        namespace harman    {
            namespace voice    {


CCOIProxyManager::CCOIProxyManager ( )
{
	// get runtime of the middleware
    runtime_ = CommonAPI::Runtime::get () ;

    // initialize the member variables	
    init () ;
}

void CCOIProxyManager::init ()
{
    // init the display language id
    displayLangId_        = VehicleType::eDisplayLang::Unknown ;
    
    // driving restrictions
    drivingRestrictions_  = false ;

    // pccoi init
    pccoi                 = nullptr ;    
    // proxies grounded
    masterResetProxy_     = nullptr ;
    versionManagerProxy_  = nullptr ;
    notifAlertProxy_      = nullptr ;
    bodyCtrlDataProxy_    = nullptr ;
    vehicleSettingsProxy_ = nullptr ;
    softwareUpdateProxy_  = nullptr ;
    hmiProxy_             = nullptr ;
}

void CCOIProxyManager::getAllProxies ()
{
	if ( runtime_ )
    {
        // order of proxies is important as other might cause timing issues, so put the ones we depend on the data first, ex: lang is important for hmi to display so first vehicleSettings and then HMI here in the list etc,.
        vehicleSettingsProxy_ = runtime_-> buildProxy < VehicleSettingsProxy >             ( "local",        "PresCtrlVehicleSettings.vehSettings",                 "ccoiVehicleSetProxy"     ) ;
        checkAndSubscribeProxy ( vehicleSettingsProxy_, bind ( &CCOIProxyManager::cbVehicleSetProxyAvbl,     this, placeholders::_1 ) ) ;

        notifAlertProxy_      = runtime_-> buildProxy < notifalertProxy >                  ( "local",        "AutomotiveMessageBrokerProvider.ford_NotifAlert",     "ccoiAutoMsgBrokerProxy"  ) ;
        checkAndSubscribeProxy ( notifAlertProxy_,      bind ( &CCOIProxyManager::cbNotifyAlertProxyAvbl,    this, placeholders::_1 ) ) ;
                                                         
        softwareUpdateProxy_  = runtime_-> buildProxy < SoftwareUpdateServiceProxy >       ( "local",        "SwupdtPresCtrl.Instance",                             "ccoiSoftwareUpdateProxy" ) ;
        checkAndSubscribeProxy ( softwareUpdateProxy_,  bind ( &CCOIProxyManager::cbSoftwareUpdateProxyAvbl, this, placeholders::_1 ) ) ;

        masterResetProxy_     = runtime_-> buildProxy < MasterResetProxy >                 ( "local",        "PresCtrlVehicleSettings.masterReset",                 "ccoiMasterResetProxy"    ) ;
        checkAndSubscribeProxy ( masterResetProxy_,     bind ( &CCOIProxyManager::cbMasterResetProxyAvbl,    this, placeholders::_1 ) ) ;

        bodyCtrlDataProxy_    = runtime_-> buildProxy < bodycontroldataProxy >             ( "local",        "AutomotiveMessageBrokerProvider.ford_BodyCtrlData",   "ccoiAmbBodyCtrlProxy"    ) ;
        checkAndSubscribeProxy ( bodyCtrlDataProxy_,    bind ( &CCOIProxyManager::cbAMBBodyCtrlProxyAvbl,    this, placeholders::_1 ) ) ;

        versionManagerProxy_  = runtime_-> buildProxy < VersionManagerProxy >              ( "local",        "VersionManager.inst0",						        "ccoiVersionManagerProxy" ) ;
        checkAndSubscribeProxy ( versionManagerProxy_,  bind ( &CCOIProxyManager::cbVersionManagerProxyAvbl, this, placeholders::_1 ) ) ;

        hmiProxy_             = runtime_-> buildProxy < hmiDialogueServiceInterfaceProxy > ( "local",        "hmiDialogueService.hmiDialogueServiceInterface",      "ccoiHmiProxy"            ) ;
        checkAndSubscribeProxy  ( hmiProxy_,            bind ( &CCOIProxyManager::cbHMIProxyAvbl,            this, placeholders::_1 ) ) ;
    }

    else    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_ERROR, "ERROR: Could not load commonAPI runtime" ) ; 
}

bool CCOIProxyManager::checkAndSubscribeProxy ( shared_ptr < CommonAPI::Proxy > proxyHandle, const proxyAvblCallback &avblCallback )
{
    if ( proxyHandle )
    {
        for ( int wait = 10 ; ( proxyHandle-> isAvailable () == false ) && ( wait ); sleep (1), wait-- ) ;
        
        // register the availability callback of the client to the proxy created
        proxyHandle-> getProxyStatusEvent(). subscribe ( avblCallback ) ;
    }

    else    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_ERROR, "%s:%d Proxy Handle NULL", __FUNCTION__, __LINE__ ) ;

    return ( proxyHandle == nullptr ) ;
}

/***** CALL BACKS from proxies   *****/

/* availability callbacks */

// hmi proxy
void CCOIProxyManager::cbHMIProxyAvbl ( CommonAPI::AvailabilityStatus Status )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "HMI Proxy %s available", ((CommonAPI::AvailabilityStatus::AVAILABLE == Status)? "" : "not") ) ;

    if ( CommonAPI::AvailabilityStatus::AVAILABLE == Status )
    {
        // HMI interface handle available, go ahead with all the events that needs to be registered with call backs.
        hmiProxy_-> getEvent_alertButtonInteractionEvent (). subscribe ( bind ( &CCOIProxyManager::onButtonClicked, this, std::placeholders::_1 ) ) ;
        hmiProxy_-> getEvent_alertSpecialButtonInteractionEvent (). subscribe ( bind ( &CCOIProxyManager::onSpecialButtonClicked, this, std::placeholders::_1 ) ) ;
    }

    // inform clients about HMI status 
    if ( pccoi != nullptr ) pccoi -> setHMIStatus ( ( CommonAPI::AvailabilityStatus::AVAILABLE == Status ) ) ;

    else    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "%s:%d pccoi pointer NULL", __FUNCTION__, __LINE__ ) ;

    CCOIUserPromptScreenData userPrompt ;
    userPrompt. sendScreen ( ( CommonAPI::AvailabilityStatus::AVAILABLE == Status ) ) ;
}

// vehicleSettings proxy
void CCOIProxyManager::cbVehicleSetProxyAvbl ( CommonAPI::AvailabilityStatus Status )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "vehicleSettings Proxy %s available", ((CommonAPI::AvailabilityStatus::AVAILABLE == Status)? "" : "not") ) ;

    if (CommonAPI::AvailabilityStatus::AVAILABLE == Status)
    {
	    vehicleSettingsProxy_-> getDispLangStatusAttribute (). getChangedEvent (). subscribe ( bind ( &CCOIProxyManager::cbDisplayLangIdUpdate, this, std::placeholders::_1 ) ) ;
	    vehicleSettingsProxy_-> getCountryRegionAttribute (). getChangedEvent (). subscribe ( bind ( &CCOIProxyManager::cbRegionUpdate, this, std::placeholders::_1 ) ) ;
	    vehicleSettingsProxy_-> getDriverRestrictionsGeneralAttribute (). getChangedEvent(). subscribe ( bind ( &CCOIProxyManager::cbDrivingRestrictionsUpdate, this, std::placeholders::_1 ) ) ;
    }
}

// AMBBdyCtrl proxy
void CCOIProxyManager::cbAMBBodyCtrlProxyAvbl ( CommonAPI::AvailabilityStatus Status )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "AMBBodyCtrl Proxy %s available", ((CommonAPI::AvailabilityStatus::AVAILABLE == Status)? "" : "not") ) ;

	if (CommonAPI::AvailabilityStatus::AVAILABLE == Status)
	{
	    // request the TCU part numbers, need it to feed to the VersionManager proxy
		CommonAPI::CallStatus status;
        
        bodyCtrlDataProxy_-> TCU_ESN_D_Rq_method ( bodycontroldata_types::TCU_ESN_D_Rq_enum::En_TCU_ESN_Request, status ) ;
		
		CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "Request sent to AMBBodyCtrl Proxy for TCU_ESN" ) ;
	}
}

// notifAlert proxy
void CCOIProxyManager::cbNotifyAlertProxyAvbl ( CommonAPI::AvailabilityStatus Status )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "NotifyAlert Proxy %s available", ((CommonAPI::AvailabilityStatus::AVAILABLE == Status)? "" : "not") ) ;

    if (CommonAPI::AvailabilityStatus::AVAILABLE == Status)
	{
        // proxy available so subscribe to the interested notifcations & events on this proxy
        
        // subscribe for the tcu state
		notifAlertProxy_-> getGWM_Send_Signals_8_HS3_sig_4Attribute().getChangedEvent().subscribe ( bind ( &CCOIProxyManager::cbTcuStateChange, this, std::placeholders::_1 ) ) ;
	}
}

// versionManager proxy
void CCOIProxyManager::cbVersionManagerProxyAvbl ( CommonAPI::AvailabilityStatus Status )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "Version Manager Proxy %s available", ((CommonAPI::AvailabilityStatus::AVAILABLE == Status)? "" : "not") ) ;
}

// softwareUpdate proxy
void CCOIProxyManager::cbSoftwareUpdateProxyAvbl ( CommonAPI::AvailabilityStatus Status )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "SoftwareUpdate Proxy %s available", ((CommonAPI::AvailabilityStatus::AVAILABLE == Status)? "" : "not") ) ;

    if (CommonAPI::AvailabilityStatus::AVAILABLE == Status)
	{
        // proxy available so subscribe to the interested notifcations & events on this proxy
		softwareUpdateProxy_-> getSoftwareUpdateStatusEvent().subscribe ( bind ( &CCOIProxyManager::cbSoftwareUpdateProxy, this, std::placeholders::_1, std::placeholders::_2 ) ) ;
	}
}

// masterReset proxy
void CCOIProxyManager::cbMasterResetProxyAvbl ( CommonAPI::AvailabilityStatus Status )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "masterReset Proxy %s available", ((CommonAPI::AvailabilityStatus::AVAILABLE == Status)? "" : "not") ) ;

    if (CommonAPI::AvailabilityStatus::AVAILABLE == Status)
	{
        // proxy available so subscribe to the interested notifcations & events on this proxy
		masterResetProxy_-> getFactoryResetNotificationEvent().subscribe ( bind ( &CCOIProxyManager::cbMasterReset, this, std::placeholders::_1 ) ) ;
    }
}

/* data update callbacks the notifications/ events which we had subscribed with the proxies */

// HMI calls this callback: Button interaction event from the hmi upon the user selecting the normal button choice prompt screen.
void CCOIProxyManager::onButtonClicked ( hmiDialogueServiceTypes::ButtonInteraction hmiresp )
{
    CCOIUserPromptScreenData userPrompt ; 

    userPrompt. buttonClicked ( hmiresp.getAlertcontext(), static_cast < int > ( hmiresp.getButtonID() ), static_cast < int > ( hmiresp.getAction() ) ) ;
}

// HMI calls this callback: Button interaction event from the hmi upon the user selecting the special button choice prompt screen.
void CCOIProxyManager::onSpecialButtonClicked ( hmiDialogueServiceTypes::SpecialButtonInteraction hmiresp )
{
    int action   = static_cast < int > ( hmiresp.getBasicButtonInteractionInfo().getAction () ) ;
    int buttonId = static_cast < int > ( hmiresp.getBasicButtonInteractionInfo().getButtonID () ) ;

    CCOIUserPromptScreenData userPrompt ; 

    userPrompt. buttonClicked ( hmiresp.getBasicButtonInteractionInfo().getAlertcontext(), buttonId, action, hmiresp.getSpecialButtonValue () ) ;
}

// vehicleSettings proxy
void CCOIProxyManager::cbDisplayLangIdUpdate ( VehicleType::eDisplayLang dispLangId )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "DisplayLanguageChanged callback from VehicleSettings with langId %d prevLangId in CCOI %d", dispLangId, displayLangId_ ) ;

	displayLangId_ = dispLangId;
}

void CCOIProxyManager::cbDrivingRestrictionsUpdate ( bool status )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "%s:%d callback from VehicleSettings with status %d prevStatus in CCOI %d", __FUNCTION__, __LINE__, status, drivingRestrictions_ ) ;
    
    drivingRestrictions_ = status ;
    if ( ! drivingRestrictions_ )
    {
        // drivingRestrictions is relaxed now, see if there were any prompts we had queued up due to this and trigger sending it to HMI
        CCOIUserPromptScreenData userPrompt ;
        userPrompt. sendScreen ( true ) ;
    }
}

void CCOIProxyManager::cbRegionUpdate ( VehicleType::eCountryRegion region )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "%s:%d callback from VehicleSettings with region %d prevRegion in CCOI %d", __FUNCTION__, __LINE__, region, region_ ) ;
    
    region_ = region ;
}

// notifAlert proxy
void CCOIProxyManager::cbTcuStateChange ( notifalert_types::GWM_Send_Signals_8_HS3_sig_4_struct tGWM_Send_Signals )
{
    int tcuState = static_cast < int > ( tGWM_Send_Signals.getCCOI_PolicyServer_St_enum() ) ;
    
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "On getCCOI_PolicyServer_St_enum change %d", tcuState) ;

    pccoi-> onTcuStateReceived ( tcuState ) ;
}

#ifdef TESTING
void CCOIProxyManager::cbMockTcuStateChange ( int tcuState )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "On getCCOI_PolicyServer_St_enum change %d", tcuState) ;

    pccoi-> onTcuStateReceived ( tcuState ) ;
}
#endif

// notifAlertProxy callback for the ccoi state we set
std::function < void (const CommonAPI::CallStatus&, uint64_t testValue ) > fcb_clock =

    [](const CommonAPI::CallStatus&, uint64_t testValue) {
    
	};

// softwareUpdate proxy
void CCOIProxyManager::cbSoftwareUpdateProxy ( SoftwareUpdateTypes::tStatus status, SoftwareUpdateTypes::eUpdateSource eSource )
{
	SoftwareUpdateTypes::eSwdlState eswdlstate = status.getCurrentState();
	CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "Software Update Status %d, Source %d\n", eswdlstate, eSource) ;

    if ( eSource == SoftwareUpdateTypes::eUpdateSource::UPDATE_SOURCE_OTA )
    {
        softwareDownload_ = eswdlstate ;
    }
}

// master reset proxy
void CCOIProxyManager::cbMasterReset ( ResetType::eFactoryResetParams eResetParams )
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_ERROR, "%s:%d eResetParams %d", __FUNCTION__, __LINE__, eResetParams ) ;
    if ( ( eResetParams != ResetType::eFactoryResetParams::RESET_UNKNOWN ) && ( eResetParams != ResetType::eFactoryResetParams::RESET_ERROR ) )
    {
        bool success = pccoi-> onMasterReset () ;
   
        // inform the masterReset process about the resetStatus so as they can book keep it and continue with tracking other presCtrls resets.
        CommonAPI::CallStatus           status ;
        ResetType::eFactoryResetStatus  result = ( success )? ResetType::eFactoryResetStatus::SUCCESS : ResetType::eFactoryResetStatus::FAILURE ;

        masterResetProxy_-> FactoryResetStatus ( ResetType::eModuleID::MID_CUSTOME2_APP, result, status ) ;
    }
}

/*****   getters of the data intimated from proxies, used by other components in ccoi   *****/
VehicleType::eDisplayLang CCOIProxyManager::getCurrentDispLanguage ()
{
    if ( ( displayLangId_ == VehicleType::eDisplayLang::Invalid ) || ( displayLangId_ == VehicleType::eDisplayLang::Unknown ) )
        CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_ERROR, " Display Language is Invalid or Unknown anything wrong?? check with vehicleSettings" ) ;

    return displayLangId_ ;
}

VehicleType::eCountryRegion CCOIProxyManager::getRegion ()
{
    if ( region_ == VehicleType::eCountryRegion::REGION_INVALID )
        CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_ERROR, "%s:%d region invalid anything wrong?? check with vehicleSettings", __FUNCTION__, __LINE__ ) ;

    return region_ ;
}

bool CCOIProxyManager::getDrivingRestrictions ()
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_ERROR, "%s:%d retval %d", __FUNCTION__, __LINE__, drivingRestrictions_ ) ;
    
    // uncomment this after demo, seems drivingRestrictions is wrongly updated from vehicleSettings probably
    return false ;
}

bool CCOIProxyManager::packageDownloading ()
{
    bool retVal = ( ( softwareDownload_ == SoftwareUpdateTypes::eSwdlState::DOWNLOADING_UPDATE ) || ( softwareDownload_ == SoftwareUpdateTypes::eSwdlState::INSTALLATION_IN_PROGRESS) ) ;

    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "%s:%d softwareDownload_ %d retVal %d", __FUNCTION__, __LINE__, softwareDownload_, retVal ) ;

    return retVal ;
}

bool CCOIProxyManager::sendNotifAlert ( int value )
{

#ifdef TESTING
    mockTcu-> onClientStateReceived ( value ) ;

#else
    if ( notifAlertProxy_ && notifAlertProxy_-> isAvailable() )
    {
        notifalert_types::CCOI_PolicyClient_St_enum state = static_cast < notifalert_types::CCOI_PolicyClient_St_enum::Literal > ( value ) ;

        notifAlertProxy_ -> getCCOI_PolicyClient_St_enumAttribute().setValueAsync ( state, fcb_clock ) ;
        CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "%s:%d ccoiState %d sent to notifAlert", __FUNCTION__, __LINE__, value ) ;
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_ERROR, "NotifAlert proxy %s not available", ( ( notifAlertProxy_ == nullptr )? "pointer" : "handle" ) ) ;

#endif
    return true ;
}

// sets the Automatic download option to SWDL component based on user choice selection in userPromptScreen
void CCOIProxyManager::setAutomaticUpdates ( bool value )
{
    if ( softwareUpdateProxy_ && softwareUpdateProxy_ -> isAvailable() )
    {
        CommonAPI::CallStatus status ;
        SoftwareUpdateTypes::eMethodResult error ;

        // if YES send autoUpdates value as true, else false - for user clicking NO in EULA screen.
        softwareUpdateProxy_-> SetAutomaticDownload( value, status, error ) ;

        CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, " SetAutomaticDownload called to swdl enableAutoDownload %d returnStatus %d methodError %d\n", value, status, error ) ;
    }
    else
    {
        CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_ERROR, "softwareUpdateProxy %p SWDL proxy %s not available", softwareUpdateProxy_, ( ( softwareUpdateProxy_ == nullptr )? "pointer" : "handle" ) ) ;
    }
}

// sets the Automatic download option to SWDL component based on user choice selection in userPromptScreen
void CCOIProxyManager::setTcuVersion ( string &version )
{
    if ( versionManagerProxy_ && versionManagerProxy_-> isAvailable() )
    {
        string temp ;
        // spec says the Max length to be 13 for ESN TCU so shrink the string before u[pdating to version Manager
        if ( version. length () > MAX_TCU_ESN_LENGTH )    temp. assign ( version, 0, MAX_TCU_ESN_LENGTH ) ;
        else    temp = version ;

        versionmanagertypes::VersionInfo tcuInfo ;
        tcuInfo. setVersionName   ( "ESN_TCU" ) ;
        tcuInfo. setVersionData   ( temp ) ;
        tcuInfo. setVersionStatus ( versionmanagertypes::VersionStatus::VERSION_STATUS_INFO_AVAILABLE ) ;
        
        versionmanagertypes::VersionInfoList versions;
        versions.push_back ( tcuInfo ) ;
        
        CommonAPI::CallStatus status ;
        versionManagerProxy_-> setVersion ( versions, status ) ;
        
        CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, " version %s length %d set to VersionManager", version. c_str (), version. length () ) ;
    }

    else    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_ERROR, "VersionManager proxy %s not available", ( ( versionManagerProxy_ == nullptr )? "pointer" : "handle" ) ) ;
}

bool CCOIProxyManager::sendDataToHMI ( bool special, hmiDialogueServiceTypes::RequestDialogueAlert data, hmiDialogueServiceTypes::RequestDialogueSpecialAlert specialData )
{
    bool success = false ;
    if ( hmiProxy_ && hmiProxy_-> isAvailable () )
    {
        if ( special )  hmiProxy_-> request_special_alertAsync ( specialData ) ;

        else    hmiProxy_-> request_alertAsync ( data ) ;

        success = true ;
    }
    
    return success ;
}

// destructor
CCOIProxyManager::~CCOIProxyManager ()
{
    CcoiLog.write ( CCOILog::eLogCCOIProxyMgr, CCOILog::LOG_INFO, "%s:%d", __FUNCTION__, __LINE__ ) ;
    init () ;
}
            }   // voice
        }   // harman
    }   // com
}   // v0
