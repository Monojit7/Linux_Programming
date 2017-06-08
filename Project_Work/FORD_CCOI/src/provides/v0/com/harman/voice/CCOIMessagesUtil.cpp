#include "CCOIMessagesUtil.hpp"
#include <iostream>

extern shared_ptr < v0::com::harman::voice::CCOIProxyManager >     proxyFac ;    // proxy factory global object 
extern shared_ptr < v2::com::harman::voice::CCOIPresCtrlStubImpl > presCtrl ;    // CCOI provides shared instance

namespace v0 {
    namespace com {
        namespace harman {
            namespace voice {

string CCOIMessagesUtil::appName_   = "" ;
string CCOIMessagesUtil::brandName_ = "" ;
string CCOIMessagesUtil::modemName_ = "" ;

// constructor
CCOIMessagesUtil::CCOIMessagesUtil ()
{
    // nothing to setup here, just print for reference
    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_INFO, "%s:%d", __FUNCTION__, __LINE__ ) ;

    // list of variable keys and the names in which the actual text is available in UFM - messageCode - "100".
    variableKeyNamesMap_ = { { "%appName%", "ford_app" }, { "%appname%", "ford_app" }, { "%vehicleMake%", "ford_brand" }, { "%modemName%", "ford_modemName" } } ;
}

/*****   USER PROMPTS INFORMATION PROCESSING  *****/

/** This function is called by client to construct the CCOIUserPromptScreen object's data
 *  @param (IN)  : userPromptMetadata - the meta data for the userprompt sent by TCU or internal CCOI one ( for EULA screen and partial activation prompt )
 *                                      it contains data like requestToken, variableTexts, promptType to look for in UFM
 *  @param (OUT) : CCOIUserPromptScreenData object populated with the contents, buttons in the screen added to it etc,.
*/
bool CCOIMessagesUtil::populateUserPrompt ( string &userPromptMetadata, CCOIUserPromptScreenData &userPrompt, bool Default )
{
    bool success = false ;
    
    // parse the meta data and populate the contents ointo the userPrompt object
    string messageCode = processUserPromptMetaData ( userPromptMetadata, userPrompt ) ;

    if ( ! messageCode. empty () )
    {
        // get the message from UFM message correspondign to messageCode and populate the contents ointo the userPrompt object
	    Json::Value root ( Json::arrayValue ) ;
        string text = getMessage ( "textBody", messageCode, "initialPrompt", root, Default ) ;
        
        userPrompt. setScreenText ( text ) ;
        success = ( ! text. empty () ) ;
                
        //Now, the left over for the userPrompt is the buttons on the screen, so go ahead and populate the same done in below helper function
        success &= populateUserPromptButtons ( root, userPrompt ) ;
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d partial userPrompt metadata received : but promptType??", __FUNCTION__, __LINE__ ) ;   
    
    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_INFO, "%s:%d success %d", __FUNCTION__, __LINE__, success ) ;
    return success ;
}

// processes metadata & populates userPrompt object, and returns the messageCode present in the metadata to look up further
string CCOIMessagesUtil::processUserPromptMetaData ( string &userPromptMetadata, CCOIUserPromptScreenData &userPrompt )
{
    string requestToken ;
    string messageCode ;

    // skip the first 4 bytes of the metadata it has signalId, Utilization, commandExecStatus, Character coding
    int metadataPos = 4;

    // request Token is 32 bits so copy 4 bytes from 4th position onwards
    requestToken.assign ( userPromptMetadata, metadataPos, 4 ) ;
    if ( ! requestToken. empty () )
    {
	    userPrompt. setRequestToken ( requestToken ) ; 
        
        metadataPos += requestToken. length () ;

        // messageCode of UFM or promptType in userprompt metadata is 1 byte so copy it
        messageCode. assign ( userPromptMetadata, metadataPos, 1 ) ;
        
        metadataPos += messageCode. length () ;

        // now pick the variable texts from the metadata
        string text ;

        text. assign ( userPromptMetadata, metadataPos, VARIABLE_TEXT_LENGTH ) ;
        metadataPos += ( text. length () + 1 ) ;    // also has null terminating which don't need to copy for strings
        
	    userPrompt. addVariableTexts ( text ) ;
        
        text. assign ( userPromptMetadata, metadataPos, VARIABLE_TEXT_LENGTH ) ;
        metadataPos += ( text. length () + 1 ) ;    // also has null terminating which don't need to copy for strings

	    userPrompt. addVariableTexts ( text ) ;

        // thats all we have in metadata now return messageCode to look for further information about buttons their texts etc,.
    }
    
    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d partial userPrompt metadata received : requestToken??", __FUNCTION__, __LINE__ ) ;   
   
    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d messageCode %s", __FUNCTION__, __LINE__, messageCode. c_str () ) ; 
    return messageCode ;
}

bool CCOIMessagesUtil::populateUserPromptButtons ( Json::Value &initialPrompt, CCOIUserPromptScreenData &userPrompt )
{
	Json::Value emptyDefault ( Json::nullValue ) ;

	// yes button
	string value = getUpdatedMessage ( initialPrompt, "button1ConfirmationText" ) ;
    if ( ! value. empty () )    userPrompt. addButton ( make_shared < CCOIButton > ( YES_BUTTON, "Yes", value ) ) ;
    
	// no button
	value = getUpdatedMessage ( initialPrompt, "button2ConfirmationText" ) ;
    if ( ! value. empty () )    userPrompt. addButton ( make_shared < CCOIButton > ( NO_BUTTON, "No", value ) ) ;

    // ASK ME LATER Button, add this only if above buttons are present
	if ( ! value. empty () )    userPrompt. addButton ( make_shared < CCOIButton > ( ASK_ME_LATER, "Ask Me Later", "", false ) ) ;
	
    // Info Button
	bool visible = initialPrompt. get ( "infoIcon_vis",   emptyDefault ). asBool () ;
	value = getUpdatedMessage ( initialPrompt, "infoText" ) ;

	userPrompt. addButton ( make_shared < CCOIButton > ( INFO_BUTTON, "i", value, true, visible ) ) ;
	
	// CHECKBOX BUTTON
	value        = getUpdatedMessage ( initialPrompt, "line1" ) ;
	bool checked = initialPrompt. get ( "line1_checkbox_default_checked", emptyDefault ). asBool () ;
	visible      = initialPrompt. get( "line1_checkbox_vis", emptyDefault ).asBool() ; 
	
	userPrompt. addButton ( make_shared < CCOIButton > ( CHECKBOX_BUTTON, value, ( (checked)? "T" : "F" ), false, visible, true ) ) ;

	return true;
}
	

/*****   MENU SETTINGS ENTRIES PROCESSING   *****/

// constructs Menu elements object's data.
bool CCOIMessagesUtil::populateMenu ( const Json::Value &featurePolicies, int index, int sequence )
{
	Json::Value root ( Json::arrayValue ) ;
	Json::Value emptyDefault ( Json::nullValue ) ;
	root = featurePolicies [ index ] ;
    if ( root != Json::nullValue )
    {
        string entityID          =  root. get ( "entityID",            emptyDefault ). asString () ;
        string entityType        =  root. get ( "entityType",          emptyDefault ). asString () ;
        bool   infoVisible       =  root. get ( "infoBookIcon",        emptyDefault ). asBool () ;
        bool   toggleVisible     =  root. get ( "optInSwitch",         emptyDefault ). asBool () ;
        bool   userConsentReqd   =  root. get ( "userConsentRequired", emptyDefault ). asBool () ;
        string name              =  root. get ( "commentEntityName",   emptyDefault ). asString () ;

        // create buttons - with metadata and then populate in the respective conditions the other fields of the buttons.
        CCOIMenuButton menu ( entityID, entityType, MENU_BUTTON, name, "", "", userConsentReqd, toggleVisible ) ;
        CCOIButton     info ( INFO_BUTTON, "i", "", true, infoVisible ) ;
        
        menu. setSequence ( sequence ) ;    // display order for the menu item 
        // pick the message content from UFM file based on the message code from PTE
        bool bSAllowOnOff        = false ;
        bool isUserNotAuthorized = presCtrl-> getEntityValue ( "UserAuthorizationPending", bSAllowOnOff ) ;
        string messageCode       = root. get ( ( isUserNotAuthorized )? "partialActivationMessageCode" : "fullAuthMessageCode", emptyDefault ). asString () ;

        Json::Value message      ( Json::arrayValue ) ;
        if ( getMessageFromUFM ( messageCode, message ) )
        {
            Json::Value langMessage ( Json::arrayValue ) ;
            if ( getCurrentLangMessageFromUFM ( message, langMessage ) )
            {
                populateMenuPrompt ( langMessage, menu, info ) ;
                
                if ( userConsentReqd )    populateMenuConsentPrompt ( langMessage, menu ) ;
                else
                {
                    // user consent not required for this as per PTE message, so set popup of the button to false.
                    menu.setPopup ( false ) ;
                    CcoiLog.write (CCOILog::eLogCCOIPresCtrlStubImpl, CCOILog::LOG_INFO, "userConsentReqd %d for entity <%d, %d > so no consent prompt for this menu item\n", userConsentReqd, entityID, entityType ) ;
                }
            }
        }
        presCtrl-> addButton ( menu, info ) ;
    }

	return true;
}

bool CCOIMessagesUtil::populateMenuPrompt ( const Json::Value &root, CCOIMenuButton &menu, CCOIButton &info )
{
	Json::Value emptyDefault ( Json::nullValue ) ;
    
    // Menu would come in "menu" Prompt Type.
    Json::Value menuPrompt ( Json::arrayValue ) ;
    menuPrompt = root [ "promptType" ] [ "menu" ] ;

    string name = menuPrompt.get( "settingsMenuText", emptyDefault ).asString() ;
    menu.setName ( name ) ;

    Entity item ;
    bool subscribed = true ;
    if ( presCtrl-> getEntity ( menu. getEntityId (), menu. getEntityType (), item ) )
    {
        for ( auto &value : item-> values_ )
        {
            if ( value. find ( "bSAllow"  ) != string::npos )   subscribed = ( value == "bSAllowOnOff_True" ) ;
        }
    }
    else    CcoiLog.write (CCOILog::eLogCCOIPresCtrlStubImpl, CCOILog::LOG_ERROR, "%s:%d No Entity with this pair <%s, %s>", __FUNCTION__, __LINE__, menu. getEntityId (), menu. getEntityType () ) ;

    info. setValue ( getUpdatedMessage ( menuPrompt, ( subscribed )? "infoText" : "infoTextNotSubscribed" ) ) ;

    return true ;
}

bool CCOIMessagesUtil::populateMenuConsentPrompt ( const Json::Value &root, CCOIMenuButton &menu )
{
	Json::Value emptyDefault ( Json::nullValue ) ;

    // MenuConsent texts would come in "menuConsentPrompt" Prompt Type.
    Json::Value menuConsentPrompt ( Json::arrayValue ) ;
    menuConsentPrompt = root [ "promptType" ] [ "menuConsentPrompt" ] ;

    menu.setOnValue ( getUpdatedMessage ( menuConsentPrompt, "textBody" ) ) ;

    // if packageDownload Message take it from IVSU field from the CFM.
    menu.setOffValue ( getUpdatedMessage ( menuConsentPrompt, ( packageDownloadMsg ( menu )? "confirmation_off_plusIVSU" : "confirmation_off" ) ) ) ;
    
    return true ;
}

// returns true, if we need to pick the softwareDownload message text for the repective menuButton, false otherwise.
bool CCOIMessagesUtil::packageDownloadMsg ( CCOIMenuButton &menu )
{
    bool retVal = false ;
    
    string key = "VehicleConnectivity" ;
    auto entityIdTypePair = CCOIPresCtrlStubImpl::entityNamesMap_. find ( key ) ;

    if ( entityIdTypePair != CCOIPresCtrlStubImpl::entityNamesMap_. end () )
    {
        if ( ( menu.getEntityType () == entityIdTypePair-> second. first ) && ( menu.getEntityId () == entityIdTypePair-> second. second ) )
        {
            retVal = proxyFac-> packageDownloading () ; 
        }
    }
    return retVal ;
}

// helper functions to populate various entries of the entities in various policy arrays

// Extracts the list identified by key in the input and populates the output param EntityListVector
bool CCOIMessagesUtil::getListVector ( Json::Value &root, string key, string flagKey )
{
    string priority ;

    listVector_. resize ( root. size () ) ;
    for ( unsigned int i = 0; i < root. size (); i++ )
    {
        auto &list = listVector_ [ i ] ;

        getList ( root [ i ], list, key, flagKey, priority ) ;
    }

    return ( ( listVector_. size () > 0 ) && ( listVector_ [ 0 ]. size () > 0 ) ) ;
}

// Extracts the list from the json based for the key
bool CCOIMessagesUtil::getList ( Json::Value &root, EntityList &list, string key, string flagKey, string priority )
{
    if ( root != Json::nullValue )
    {
        Json::Value emptyDefault ( Json::nullValue ) ;
        priority = root. get ( "priority",   emptyDefault ). asString () ;
        
        list. resize ( root [ key ]. size () ) ;
        for ( unsigned int j = 0; j < root [ key ]. size (); j++ )
        {
            getList ( root [ key ], list, flagKey, priority ) ;
        }
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d json root sent NULL for <%s,%s>", __FUNCTION__, __LINE__, key, flagKey ) ;

    return ( list. size () > 0 ) ;
}

// Extracts the list from the json based on the entityID & entityType & flagKey
bool CCOIMessagesUtil::getList ( Json::Value &root, EntityList &list, string flagKey, string priority )
{
    if ( root != Json::nullValue )
    {
        for ( unsigned int j = 0; j < root. size (); j++ )
        {
            Json::Value emptyDefault ( Json::nullValue ) ;

            string entityID     = root [ j ]. get ( "entityID",   emptyDefault ). asString () ;
            string entityType   = root [ j ]. get ( "entityType", emptyDefault ). asString () ;
           
            vector < string > flags ;
            if ( ! flagKey. empty () ) 
            {
                string flag     = root [ j ]. get (  flagKey,     emptyDefault ). asString () ;
                flags. push_back ( flag ) ;
            }
            list [ j ] = make_shared < EntityElement > ( entityID, entityType, flags, ( priority. empty ()? -1 : atoi ( priority. c_str () ) ) ) ;
        }
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d json root sent NULL for %s", __FUNCTION__, __LINE__, flagKey. c_str () ) ;

    return ( list. size () > 0 ) ;
}

/** checks the conditions list aganist the entities received in syncSettings, and returns the matched condition index 
*  from which the showInMenu is identitfed and picked the menu items for display
*  return -1 when no condition matched the values sent in the syncSettings response from TCU, else would return the index from which showInMenu contents needs to be picked in PTE.
*/
int CCOIMessagesUtil::getMatchIndex ( EntityList &entities )
{
    int index = -1, minPriority = INT_MAX ;
    bool found = ( listVector_. size () > 0 ) && ( ( listVector_ [ 0 ]. size () > 0 ) ) ;

    for ( unsigned int i = 0; i < listVector_. size (); i++ )
    {
        for ( auto &list : listVector_ [ i ] )
        {
            if ( ( list-> priority_ != -1 ) && ( list-> priority_ <= minPriority ) )
            {
                found &= ( findEntity ( list, entities ) != -1 ) ;
                if ( found )    minPriority = list-> priority_ ;
            }
            else    found = false ;
            
            if ( !found )   break ;
        }
        
        if ( found )    index = i ;

        else            found = true ;  // reset flag for next iteration
    }

    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d found %d index %d minPriority %d", __FUNCTION__, __LINE__ , found, index, minPriority ) ;

    return index ;
}

/** Finds the entity based on the tuple (entityID, entityType) and if flags are there they shall be matched as well
 *  returns the index in the list of entities, -1 if nothing found
 */
int CCOIMessagesUtil::findEntity ( Entity entity, EntityList &entities )
{
    int index = -1, foundIndex = -1 ;
    for ( auto item : entities )
    {
		index ++ ;
		// first just find the entity
		if ( ( entity-> entityID_ == item-> entityID_ ) && ( entity-> entityType_ == item-> entityType_ ) )
        {
			foundIndex = index ;
			// if there are any flags associated with the entity then that also should match
            for ( auto flag : entity-> values_ )
            {
				foundIndex = ( ( find ( item-> values_. begin (), item-> values_. end (), flag ) != item-> values_. end () ) ? foundIndex : -1 ) ;
			}
			break ;
        }		
    }

    return foundIndex ;
}

/** This function is called by clients CCOIPresCtrlStubImpl, CCOIManager to help process the PTE & UFM messages and call their APIs to update the entities
 *  @ param (IN) : entriesType - the entry to look into PTE, like menuPolicies, controllingEntities, iconRules etc,.
 *  @ param (IN) : entities - the current entities values in the CCOI system based on their status & values the conditions matching to be done and necessary updates to clients are made.
 *  @ returns, nothing - just callback the corresponding client APIs.
*/
bool CCOIMessagesUtil::populateEntries ( string entriesType, EntityList entities )
{
    bool success = ( ! entriesType. empty () && ( entities. size () > 0 ) ) ;
    Json::Value  root ;   // placeholder to contain the parsed value of the json.
    Json::Reader reader ;
    
    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_INFO, "%s:%d entriesType %s", __FUNCTION__, __LINE__, entriesType. c_str () ) ;
    if ( success )
    { 
        ifstream PTEFile ( PTE_FILE ) ;
        if ( PTEFile. is_open () )
        {
            if ( reader.parse ( PTEFile, root, false ) )
            {
                Json::Value policies ( Json::arrayValue ) ;
                policies = root [ "ccsFeaturePolicies" ] [ entriesType ] ;
                if ( policies != Json::nullValue )
                {
                    // clear the old entries
                    listVector_. clear () ;

                    if ( getListVector ( policies, "conditions", "entityBitSetting" ) ) 
                    {
                        int index = getMatchIndex ( entities ) ;
                        if ( index >= 0 )
                        {
                            if      ( entriesType == "menuPolicies" )           success = updateCCSMenuEntries ( root [ "ccsFeaturePolicies" ], policies, index ) ;
                            else if ( entriesType == "controllingEntities" )    success = updateControllingEntities ( policies, index ) ;
                            else if ( entriesType == "promptPolicies" )         success = updatePromptPolicies ( policies, index ) ;
                            else                                                success = updateIconData ( policies, index ) ;
                        }
                    }
                    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d Not able to get the list of the conditions for %s", __FUNCTION__, __LINE__, entriesType. c_str () ) ;
                }
                else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d No ccsFeaturePolicies or %s in the json check file %s", __FUNCTION__, __LINE__, entriesType, PTE_FILE ) ;
            }
            else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d %s file json parse error", __FUNCTION__, __LINE__, PTE_FILE ) ;
        }
        else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d file %s open error", __FUNCTION__, __LINE__, PTE_FILE ) ;
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d Empty entity %s received entriesType %s", __FUNCTION__, __LINE__, ( entriesType. empty () )? "Type" : "List", entriesType. c_str () ) ; 

    return success ;
}

/*****   Functions which makes calls to client APIs to update their attribute values  *****/

/** This function calls the CCOIPersist client to create & persist the entries for the menu settings
 *  @param (IN) root - json array of ccsFeaturePolicies of PTE, index - the matching index of which the showInMenu items need to be taken and create menu entries
 *  output: create the persist file which contains the menu entries
*/
bool CCOIMessagesUtil::updateCCSMenuEntries ( Json::Value &root, Json::Value &policies, int index )
{
    // first clear the old data of the menu
    presCtrl-> getButtons (). clear () ;

	Json::Value menuItems ( Json::arrayValue ) ;
	Json::Value featurePolicies ( Json::arrayValue ) ;
	
	featurePolicies = root [ "featurePolicies" ] ;
        
    EntityList entities ;
    entities. resize ( featurePolicies. size () ) ; 

    if ( entities. size () > 0 )    getList ( featurePolicies, entities, "", "" ) ;

    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d featurePolicies empty index %d", __FUNCTION__, __LINE__, index ) ;

	menuItems = policies [ index ] [ "showInMenu" ] ;
    for ( unsigned int policyIndex = 0; policyIndex < menuItems. size (); policyIndex ++ )
    {
        Json::Value emptyDefault ( Json::nullValue ) ;

        string entityID     = menuItems [ policyIndex ] . get ( "entityID",   emptyDefault ). asString () ;
        string entityType   = menuItems [ policyIndex ] . get ( "entityType", emptyDefault ). asString () ;
        string sequence     = menuItems [ policyIndex ] . get ( "sequence",   emptyDefault ). asString () ;

        Entity entity       = make_shared < EntityElement > ( entityID, entityType ) ;

		index = findEntity ( entity, entities ) ;
        if ( index != -1 )    populateMenu ( featurePolicies, index, ( ( sequence. empty () )? 0 : atoi ( sequence. c_str () ) ) ) ;
        
        else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "entityId %s entityType %s in showMenu but not present in featurePolicies in PTE", entityID. c_str (), entityType. c_str () ) ;
    }

    return ( index != -1 ) ;
}

/** This function calls the CCOIPresCtrlStubImpl client to update the entries in the menu settings
 *  @param (IN) root - json array of ccsFeaturePolicies of PTE, index - the matching index of which the effects nees to be taken and update menu entries
 *  output: some buttons are greyed out/ disabled based on the controlling metadata in the PTE and their conditions match
*/
bool CCOIMessagesUtil::updateControllingEntities (  Json::Value &root, int index )
{
    EntityList entities ; 
    entities. resize ( root [ index ] [ "effects" ]. size () ) ; 

    if ( entities. size () > 0 )    getList ( root [ index ], entities, "effects", "action", "" ) ;

    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d effects empty index %d", __FUNCTION__, __LINE__, index ) ;
    
    for ( auto &entity : entities )
    {
        if ( entity-> values_ . size () > 0 )
        {
            // call the client to update the menu button
            // controllingEntities should impact only the enable/ disable of the menu button, visible & toggle status attributes has no update here
            presCtrl-> updateMenuButton ( entity-> entityID_, entity-> entityType_, NO_UPDATE, !( entity-> values_ [ 0 ] == "unselectable_switch" ), NO_UPDATE ) ;
        }
    }

    return ( ( index != -1 ) && ( entities. size () > 0 ) ) ;
}

/** This function calls the CCOIPresCtrlStubImpl client to update the entries in the menu settings
 *  @param (IN) root - json array of ccsFeaturePolicies of PTE, index - the matching index of which the effects nees to be taken and update menu entries
 *  output: some buttons toggle values are updated based on the prompt policies metadata in the PTE and their conditions match
*/
bool CCOIMessagesUtil::updatePromptPolicies (  Json::Value &root, int index )
{
    EntityList entities ; 
    entities. resize ( root [ "userActions" ] [ index ]. size () ) ; 

    if ( entities. size () > 0 )   getList ( root [ "userActions" ] [ index ], entities, "effects", "action", "" ) ; 
    
    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d userActions empty index %d", __FUNCTION__, __LINE__, index ) ;
    
    for ( auto &entity : entities )
    {
        if ( entity-> values_ . size () > 0 )
        {
            // call the client to update the menu button
            // promptPolicies should impact only the toggle status of the menu button, visible & enable status attributes has no update here
            presCtrl -> updateMenuButton ( entity-> entityID_, entity-> entityType_, NO_UPDATE, NO_UPDATE, ( entity-> values_ [ 0 ] == "bUAllowOnOff_True" ) ) ;
        }
    }

    return ( ( index != -1 ) && ( entities. size () > 0 ) ) ;
}

/** This function calls the CCOIPresCtrlStubImpl client to update the dataLocShare icon
 *  @param (IN) root - json array of ccsFeaturePolicies of PTE, index - the matching index of the iconRules array (of PTE) from which the messageCode needs to be taken to get icon details from UFM
 *  output: dataLocShare icon is updated in client and also the client triggers the popup for th eicon if the value is changed.
*/
bool CCOIMessagesUtil::updateIconData (  Json::Value &root, int index )
{
    bool success = false ;
	Json::Value iconRules ( Json::arrayValue ) ;
    
    iconRules = root [ index ] ;
    if ( iconRules != Json::nullValue )
    {
        Json::Value emptyDefault ( Json::nullValue ) ;
        string messageCode = iconRules. get ( "messageCode", emptyDefault ). asString () ;

        // call the client to update the icon 
        presCtrl-> updateLocDataShareIcon ( getMessage ( "icon", messageCode, "menu" ), getMessage ( "data_sharing_popup", messageCode, "menu" ) ) ;

        success = true ;
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d iconRules not available for index %d in PTE, check PTE", __FUNCTION__, __LINE__, index ) ;

    return success ;
}

/*****   HELPERS   *****/

/** Helper function to get the message value corresponding to the messageCode in the UFM
 *  @param (IN) : messageCode - the messagecode number which needs to be looked upon in the UFM
 *  @param (OUT) : message - contains the json array value of the message belonging to messageCode, returns true if a message if found with given code else false.
*/
bool CCOIMessagesUtil::getMessageFromUFM ( string &messageCode, Json::Value &message, bool Default )
{
    bool success = false ;

    Json::Value  root ;
    Json::Reader reader ;
    
    ifstream ufmStream ( (Default)? DEFAULT_UFM_FILE : UFM_FILE ) ;
    if ( ufmStream. is_open () )
    {
        success = reader. parse ( ufmStream, root, false ) ;
        if ( success ) 
        {
            Json::Value ufm ( Json::arrayValue ) ;
	        Json::Value  emptyDefault ( Json::nullValue ) ;
            
            ufm = root. get ( "consumer_friendly_messages", emptyDefault ) ;
            if ( ufm != Json::nullValue )
            {
                message = ufm ["messageCode"] [ messageCode ] ;

                success = ( message != Json::nullValue ) ;
                if ( ! success )    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d messageCode %s not present in UFM, check UFM", __FUNCTION__, __LINE__, messageCode. c_str () ) ;
            }
            else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "incorrect UFM contents - consumer_friendly_messages not there" ) ;
        }
        else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d %s json parse error", __FUNCTION__, __LINE__, (Default)? DEFAULT_UFM_FILE : UFM_FILE ) ;
        
        ufmStream. close () ;
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d file %s open error", __FUNCTION__, __LINE__, (Default)? DEFAULT_UFM_FILE : UFM_FILE ) ;

    return success ;
}

/** Helper function to get the message corresponding to the current display language in the UFM
 *  @param (IN)  : message - the json array value of the message contents
 *  @param (OUT) : langMessage - contains the json array value of the message belonging to current language returns true if a message found with matching language in UFM else false
*/
bool CCOIMessagesUtil::getCurrentLangMessageFromUFM ( Json::Value &message, Json::Value &langMessage )
{
    bool success = false ;

    // get the current user language and retrieve the corresponding language texts from the parsed json.
    string currLang = CCOILang::getLangName ( proxyFac-> getCurrentDispLanguage () ) ;
    if ( ! currLang. empty () )
    {
        langMessage = message [ "displaySize" ] [ "largeDisplay" ] [ "languages" ] [ currLang ] ;

        success = ( langMessage != Json::nullValue ) ;

        if ( ! success )    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d LangId %d Language %s not present in UFM", __FUNCTION__, __LINE__, proxyFac-> getCurrentDispLanguage (), currLang. c_str () ) ; 
    }

    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d Language %d not supported by TCU", __FUNCTION__, __LINE__, proxyFac-> getCurrentDispLanguage () ) ;

    return success ;
}

string CCOIMessagesUtil::getName ( string name, bool Default )
{
    string retVal ;

    if      ( name == "ford_app" )         retVal = appName_ ;
    else if ( name == "ford_brand" )       retVal = brandName_ ;
    else if ( name == "ford_modemName" )   retVal = modemName_ ;
    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d name %s not valid", __FUNCTION__, __LINE__, name. c_str () ) ;
   
    if ( retVal. empty () )
    {
        Json:: Value root ( Json::arrayValue ) ;
        // get the value freshly and update the appName or brandName variables 
        string messageCode ( APP_BRAND_MSG_CODE ) ;
        
        retVal = getMessage ( "textBody", messageCode, name, Default ) ;
         
        if      ( name == "ford_app" )         appName_   = retVal ;
        else if ( name == "ford_brand" )       brandName_ = retVal ;
        else                                   modemName_ = retVal ;
    }
    
    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_INFO, "%s:%d retVal %s", __FUNCTION__, __LINE__, retVal. c_str () ) ;
    return retVal ;
}

// gets the Value of key under msgCode from PTE
string CCOIMessagesUtil::getValue ( string name, string msgCode, string key, bool Default )
{
    string retVal ;
    bool valid = ! name. empty () && ! msgCode. empty () && ! key. empty () ;
    if ( valid )
    {
        Json::Value  root ;   // placeholder to contain the parsed value of the json.
        Json::Reader reader ;

        ifstream stream ( ( Default )? DEFAULT_PTE_FILE : PTE_FILE ) ;
        if ( stream. is_open () )
        {
            if ( reader.parse ( stream, root, false ) )
            {
                Json::Value policies ( Json::arrayValue ) ;
                policies = root [ "ccsFeaturePolicies" ] [ name ] ;
                if ( policies != Json::nullValue )
                {
                    for ( unsigned int i = 0; i < policies. size (); i++ )
                    {
	                    Json::Value emptyDefault ( Json::nullValue ) ;

                        string code =  policies [ i ]. get( "messageCode", emptyDefault ). asString () ;
                        if ( ! code. empty () && code == msgCode )
                        {
                            retVal =  policies [ i ]. get( key, emptyDefault ). asString () ;
                            if ( retVal. empty () )    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_INFO, "%s:%d key %s does not exist in PTE", __FUNCTION__, __LINE__, key. c_str () ) ;
                        }
                    }
                }
                else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_INFO, "%s:%d Name %s does not exist in PTE", __FUNCTION__, __LINE__, name. c_str () ) ;
            }
            else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d file %s json parse error", __FUNCTION__, __LINE__, PTE_FILE ) ;
        }
        else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_INFO, "%s:%d file %s open error", __FUNCTION__, __LINE__, PTE_FILE ) ;
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_INFO, "%s:%d Invalid params name %s key %s", __FUNCTION__, __LINE__, name. c_str (), key. c_str () ) ;

    return retVal ;
}

// gets the message of key under msgCode from UFM
string CCOIMessagesUtil::getMessage ( string name, string messageCode, string key, Json::Value &root, bool Default )
{
    string retVal ;
    if ( ! messageCode. empty () && ! name. empty () && ! key. empty () )
    {
        // get the message from UFM message correspondign to messageCode
	    Json::Value message ( Json::arrayValue ) ;
        if ( getMessageFromUFM ( messageCode, message, Default ) )
        {
            Json::Value langMessage ( Json::arrayValue ) ;
            if ( getCurrentLangMessageFromUFM ( message, langMessage ) )
            {
                root = langMessage [ "promptType" ] [ key ] ;
                if ( root != Json::nullValue )    retVal = getUpdatedMessage ( root, name ) ;
                else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "%s:%d key %s is NULL in UFM", __FUNCTION__, __LINE__, key. c_str () ) ;
            }
        }
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_ERROR, "messageCode %s or name %s or key %s empty while trying to retrieve message", messageCode. c_str (), name. c_str (), key. c_str () ) ;

    return retVal ;
}

string CCOIMessagesUtil::getMessage ( string name, string messageCode, string key, bool Default )
{
    Json::Value root ( Json::arrayValue ) ;    
    
    return getMessage ( name, messageCode, key, root, Default ) ;
}

// reads the value of key and updates it with replacing variable keys with the actual texts available in messageCode "100" in UFM
string CCOIMessagesUtil::getUpdatedMessage ( Json::Value &root, string key )
{
	Json::Value emptyDefault ( Json::nullValue ) ;

	string retVal = root. get ( key, emptyDefault ). asString () ;
    
    if ( ! retVal. empty () )
    {
        for ( auto &keyName : variableKeyNamesMap_ )
        {
            CCOIUserPromptScreenData::replaceAll ( retVal, keyName. first, 0, getName ( keyName. second ) ) ;
        }
    }

    return retVal ;
}

// destructor
CCOIMessagesUtil::~CCOIMessagesUtil ()
{
    // nothing to clean up here, just print for reference
    CcoiLog.write ( CCOILog::eLogCCOIMsgUtil, CCOILog::LOG_INFO, "%s:%d", __FUNCTION__, __LINE__ ) ;
}

            } // voice
        } // harman
    } // com
} // v0

