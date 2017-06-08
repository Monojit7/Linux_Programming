
#include "CCOIUserPromptScreenData.hpp"

extern shared_ptr < v0::com::harman::voice::CCOIProxyManager >     proxyFac ;    // proxy factory global object 

namespace v0 {
	namespace com {
		namespace harman {
			namespace voice {

// TYPEDEFS for pointers, structures etc,.
typedef shared_ptr < CCOIUserPromptScreenData >   CCOIUserPromptScreenDataPtr ;

// the stack of the user prompt data, these screens are active in HMI with the latest screen (seen by user) being on the top of the stack.
deque < CCOIUserPromptScreenDataPtr > gActiveUserPrompts ;

// default constructor
CCOIUserPromptScreenData::CCOIUserPromptScreenData ( ) : requestToken_ (""), text_ (""), timeout_ (0), result_ (0)
{
    varTextKeys_ = { "%appName%", "%elementName%" } ;
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_INFO, "%s:%d", __FUNCTION__, __LINE__ ) ;
}

// constructor with client handle param
CCOIUserPromptScreenData::CCOIUserPromptScreenData ( const ClientHandlerType &clientHandler ) : requestToken_ (""), text_ (""), timeout_ (0), result_ (0)
{
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_INFO, "%s:%d", __FUNCTION__, __LINE__ ) ;

    varTextKeys_ = { "%appName%", "%elementName%" } ;
    clientHandler_ = clientHandler ;
}

/** constructor to populate and display the screen corresponding to a button uses populateButtonPopupScreen helper function.
 *  @param (IN) : button - button for which the pop-up screen is to be populated for display purpose.
 *  @param (IN) : requestToken - unique set that needs to identify the button pop-up screen.
 */ 
CCOIUserPromptScreenData::CCOIUserPromptScreenData ( string &requestToken, CCOIButtonPtr button, const ClientHandlerType &clientHandler ) : timeout_ (0), result_(0)
{
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_INFO, "%s:%d requestToken %s", __FUNCTION__, __LINE__, requestToken. c_str () ) ;
    varTextKeys_ = { "%appName%", "%elementName%" } ;

    // currently the button pop-up screens come from only settings menu & other than local ones here.
    populateButtonPopupScreen ( requestToken, button, clientHandler ) ; 
}
    
// destructor
CCOIUserPromptScreenData::~CCOIUserPromptScreenData ()
{
	cout << "CCOIUserPromptScreenData::~CCOIUserPromptScreenData" << endl;

	// clear the buttons container of the screen
	buttons_.clear();
}

// setters
void CCOIUserPromptScreenData::setRequestToken ( string &reqToken )
{
	if ( !reqToken.empty() )
		requestToken_ = reqToken ;
}

void CCOIUserPromptScreenData::setScreenText ( string &scrnText )
{
	if ( !scrnText.empty() )
		text_ = scrnText ;
}

void CCOIUserPromptScreenData::addVariableTexts ( string &text )
{
    string copyText ;

    for ( int i = 0; text [ i ] != 0 ; i++ )
        copyText. push_back ( text [ i ] ) ;

    varTexts_. push_back ( copyText ) ;
}
/** This function retrieves the variable text from the json & replaces it with the substitue text key in the user prompt body text
  * @param (IN) : root - constains the parsed json data holding the data from the root.
  * @Returns    : true on SUCCESS, false on failure.
*/

bool CCOIUserPromptScreenData::replaceVarsInScreenText( )
{
    size_t startPos = 0 ;
    unsigned int i = 0 ;

    for ( ; i < varTextKeys_. size (); i++ )
    {
        if ( i < varTexts_. size () )    replaceAll ( text_, varTextKeys_ [ i ], startPos, varTexts_ [ i ] ) ;

        else    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_ERROR, "%s:%d Variable texts sent by TCU is less than the keys to be replaced index %d", __FUNCTION__, __LINE__, i ) ;
    }
    
	return ( i == varTexts_ . size () ) ;
}

// utility function to replace all occurences of a keyword key in the src string with replacing string
bool CCOIUserPromptScreenData::replaceAll ( string &src, string key, int startPos, string replacing )
{
    size_t pos = startPos ;
    
    while ( (pos = src. find ( key, pos ) ) != string::npos )
    {
        src. replace ( pos, key.length(), replacing ) ;
        pos += key. length () ;
    }

    return true ;
}

void CCOIUserPromptScreenData::addScreen ( )
{ 
    // push it into the global stack of active user prompts, needed for hand shake calls with TCU later upon response from HMI.
    gActiveUserPrompts.push_back ( make_shared < CCOIUserPromptScreenData > (*this) ) ;
}

/** This function populates the buttons for the UserPrompt LOCAL screen (the screen context would not come from TCU directly),
  * @param (IN) : button type for whcih this pop-up screen is populated with required screen text & the sub-buttons in that popup screen.
  * @Returns    : true - upon successfull population,
                  false - otherwise.
*/
bool CCOIUserPromptScreenData::populateButtons ( CCOIButtonPtr &button  )
{
    switch ( button->getID() )
    {
        case YES_BUTTON  :  // YES Button has a popup screen with only "OK" button option, so add it. 
        case ICON_BUTTON :  // Icon notificaiton has a popup screen with only "OK" button option, so add it. 
        case INFO_BUTTON :  // for info button popup requirement does not say anything about this popup if it should have any buttons, go ahead with OK as for now
            
            addButton ( make_shared < CCOIButton > ( OK_BUTTON,       "OK",       "", false ) ) ;
            break ;
        case NO_BUTTON :
            // NO Button has a popup screen with "OK" & "Go BACK" button options, so add these.
            addButton ( make_shared < CCOIButton > ( OK_BUTTON,       "OK",       "", false ) ) ;
            addButton ( make_shared < CCOIButton > ( BACK_BUTTON,     "GO BACK",  "", false ) ) ;
            break ;
        case MENU_BUTTON :
            // MENU Buttons has a popup/ userconsent screen with "Continue" & "Cancel" button options, so add these.
            addButton ( make_shared < CCOIButton > ( CANCEL_BUTTON,   "Cancel",   "", false ) ) ;
            addButton ( make_shared < CCOIButton > ( CONTINUE_BUTTON, "Continue", "", false ) ) ;
            break ;
        default :

            CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_ERROR, "Unknown Button %d sent to populate the popup screen for the button", button->getID() ) ;
            break ;
    }
    return true ;
}

void CCOIUserPromptScreenData::populateButtonPopupScreen ( string &requestToken, CCOIButtonPtr button, const ClientHandlerType &clientHandler )
{
    CCOIUserPromptScreenDataPtr buttonScreen = make_shared < CCOIUserPromptScreenData > ( clientHandler ) ;

    buttonScreen-> setRequestToken ( requestToken ) ;
    	
    // text that would be displayed upon click of this button
    string value = button-> getValue () ;
    buttonScreen-> setScreenText ( value ) ;
    
    // Now add the required buttons for the screen.
    buttonScreen-> populateButtons ( button ) ;
    
    CCOIMessagesUtil msgUtil ;

    string varText1 = msgUtil. getName ( "ford_app" ) ; // varText1
    buttonScreen-> addVariableTexts ( varText1 ) ;
    
    string varText2 ( msgUtil. getName ( "ford_brand", true ) ) ; // varText2
    buttonScreen-> addVariableTexts ( varText2 ) ;
    
    // add it to the active screens stack data before sending the data to HMI for display
    gActiveUserPrompts.push_back ( buttonScreen ) ;
}

/** Function populates the HMI data for the current user prompt screen request
 * @param (IN)      : &hmiRequest - contains data & context for the user prompt screen - has only normal buttons.
 * @Returns         : Nothing, as the every screen would atleast have one button or a valid data text to be displayed.
 */
bool CCOIUserPromptScreenData::populateHMIData ( hmiDialogueServiceTypes::RequestDialogueAlert &hmiRequest )
{
    bool success = true ;
    if ( !gActiveUserPrompts.empty () )
    {
        shared_ptr < CCOIUserPromptScreenData > currentRequest = gActiveUserPrompts.back ();

        // varTextKeys these keys will be replaced with the vartexts sent by TCU in userPrompt request
	    currentRequest-> replaceVarsInScreenText ( ) ;

        hmiRequest. setAlertContext ( currentRequest-> getRequestToken() );
        hmiRequest. setPriority(hmiDialogueServiceTypes::AlertDisplayPriority::NORMAL);

        hmiDialogueServiceTypes::AlertStringList stringlist;
        
        string screenText = currentRequest->getScreenText() ; 
        // strip all \r's as HMI does not understand it and they are shown as bullet points in the display on target.
        string key ( "\r") ;
        replaceAll ( screenText, key, 0, "" ) ;
        
        stringlist. push_back ( screenText ) ;
        hmiRequest. setText ( stringlist ) ;
       
        hmiRequest. setAlertTimeoutDuration ( currentRequest->getTimeOut () ) ;
        hmiRequest. setHonorDrivingRestrictions ( true ) ;
        hmiDialogueServiceTypes::AlertSoftButtonList buttonList;
        for ( auto const &btn : currentRequest->getButtons() )
        {
            // only add visible buttons to HMI for display
            if ( IS_VISIBLE (btn) )
            {
                // only normal HMI buttons added in this function.
                if ( !IS_HMI_SPECIALBUTTON ( btn->getID () ) )
                {
                    hmiDialogueServiceTypes::AlertSoftButtons button ;

                    button. setButtonText          ( btn->getName() );
                    button. setButtonID            ( btn->getID () ) ;
                    button. setButtonType          ( hmiDialogueServiceTypes::SoftButtonType::TEXT ) ;
                    button. setButtonAction        ( hmiDialogueServiceTypes::HMIActionOnSoftButtonPress::DEFAULT_ACTION ) ;
                    button. setIsButtonDisabled    ( ! btn->getEnabled() );
                    
                    buttonList.push_back ( button ) ;   // add to the hmi button list for display.
                }
            }
        }
        hmiRequest. setSoftButtons ( buttonList ) ;
        hmiRequest. setShowAlert ( true ) ;
    }
    else
    {
        CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_ERROR, "No data in CCOIUserPromptScreenData to populate for HMI") ;
        success = false ;
    }
    return success ;
}

/** Function populates the HMI data for the current user prompt screen request
 * @param (IN)      : &hmiRequest - contains data & context for the user prompt screen - has only normal buttons.
 * @param (IN/ OUT) : &hmiSpecialRequest - contains data for the special buttons of HMI.
 * @Returns         : true, // if the current request we are processing has special buttons in it.
 *                    false, // if there are no special buttons in the screen.
 */
bool CCOIUserPromptScreenData::populateHMIData ( hmiDialogueServiceTypes::RequestDialogueAlert &hmiRequest, hmiDialogueServiceTypes::RequestDialogueSpecialAlert &hmiSpecialRequest )
{
    CCOIButtonPtr infobtn     ;
    CCOIButtonPtr checkboxbtn ;
   
    hmiDialogueServiceTypes::SpecialButtonType buttonType = getHMISpecialButtonType ( checkboxbtn, infobtn ) ; 

    // check if there really any special button to add.
    if ( buttonType != hmiDialogueServiceTypes::SpecialButtonType::INVALID_ENUM )
    {
        hmiSpecialRequest.setBasicAlertProperties ( hmiRequest ) ;  
        // now add the special buttons into the screen.
        hmiDialogueServiceTypes::AlertSpecialButtonList specialbuttonlist ;
        hmiDialogueServiceTypes::AlertSpecialButton button ;

        button.setButtonType                ( buttonType ) ;
        button.setButtonAction              ( hmiDialogueServiceTypes::HMIActionOnSoftButtonPress::KEEP_ALIVE ) ;
        
        button.setSpecialButtonText         ( checkboxbtn-> getName () );
        button.setSpecialButtonID           ( ( buttonType != hmiDialogueServiceTypes::SpecialButtonType::SPECIAL_BUTTON_1 ) ? checkboxbtn->getID () : infobtn->getID () );
        button.setInfoButtonID              ( infobtn->getID () );
        button.setSpecialButtonValue        ( (checkboxbtn-> getValue () == "T")? true : false ) ;
        button.setTakeActionOnSpecialButton ( true ) ;
        //add to the list for hmi 
        specialbuttonlist.push_back         ( button ) ;
        hmiSpecialRequest.setAlertSpecialButton  ( specialbuttonlist ) ;
    }

    return ( buttonType == hmiDialogueServiceTypes::SpecialButtonType::INVALID_ENUM )? false : true ;
}

hmiDialogueServiceTypes::SpecialButtonType CCOIUserPromptScreenData::getHMISpecialButtonType ( CCOIButtonPtr &checkboxbtn, CCOIButtonPtr &infobtn )
{
    hmiDialogueServiceTypes::SpecialButtonType buttonType = hmiDialogueServiceTypes::SpecialButtonType::INVALID_ENUM ;

    shared_ptr <CCOIUserPromptScreenData> currentRequest = gActiveUserPrompts.back () ;
    
    infobtn     = currentRequest-> getButton ( INFO_BUTTON ) ;
    checkboxbtn = currentRequest-> getButton ( CHECKBOX_BUTTON ) ;
    
    // only add visible buttons to HMI for display 
    if ( IS_VISIBLE ( checkboxbtn ) && IS_VISIBLE ( infobtn ))
    {
        buttonType = hmiDialogueServiceTypes::SpecialButtonType::SPECIAL_BUTTON_2 ;
    }
    else if ( IS_VISIBLE ( checkboxbtn ) && !IS_VISIBLE ( infobtn ) )
    {
        buttonType = hmiDialogueServiceTypes::SpecialButtonType::SPECIAL_BUTTON_3 ;
    }
    else if ( !IS_VISIBLE ( checkboxbtn ) && IS_VISIBLE ( infobtn ) )
    {
        buttonType = hmiDialogueServiceTypes::SpecialButtonType::SPECIAL_BUTTON_1 ;
    }
    else
    {
        CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_INFO, "%s %d No HMI special buttons to send", __FUNCTION__, __LINE__) ;
    }
    
    return buttonType ;
}

/** This function converts the populate UserPrompt content into HMI data structure content and sends the hmi data structure content to HMI for display purpose.
 *  @Returns : true on SUCCESS, else false on FAILURE.
*/
bool CCOIUserPromptScreenData::sendDataToHMI ()
{
    bool success = false ;
    if ( ! proxyFac-> getDrivingRestrictions () )
    {
        // create HMI object & populate data into it.    
        hmiDialogueServiceTypes::RequestDialogueAlert hmiRequest ;

        success = populateHMIData ( hmiRequest ) ;
        if ( success )
        {
            // create HMI special object & populate data into it if it is there in our current request.    
            hmiDialogueServiceTypes::RequestDialogueSpecialAlert hmiSpecialRequest ;
            
            bool special = populateHMIData ( hmiRequest, hmiSpecialRequest ) ;
            if ( ! proxyFac-> sendDataToHMI ( special, hmiRequest, hmiSpecialRequest ) ) 
            {
                CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_INFO, "HMI not available yet so keep the user prompt pending and show it when HMI becomes available" ) ;
            }
            else    printHMIData ( hmiRequest, hmiSpecialRequest, special ) ;
        }
    }
    else    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_ERROR, "UserPrompt trigger queued up due to driving restrictions will be retriggered later once restrictions are relaxed" ) ;

	return success ;
}

// gets called whenever HMI proxy availability gets changed
void CCOIUserPromptScreenData::sendScreen ( bool available )
{
    // check if any pending data to be displayed and send it to display
    sendDataToHMI () ;
}

void CCOIUserPromptScreenData::buttonClicked ( const string& alertContext, int hmiButtonID, int action, bool value ) 
{
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_INFO, "%s%d hmiButtonID %d action %d value %d alertContext %s", __FUNCTION__, __LINE__, hmiButtonID, action, value, alertContext. c_str () ) ;
    if ( action == hmiDialogueServiceTypes::ButtonAction::RELEASED )
    {
        if ( !gActiveUserPrompts.empty() )
        {
            CCOIUserPromptScreenDataPtr currScreen = gActiveUserPrompts.back () ;   // get current active screen data, i.e., top element of the stack requests.
            // check if the button that is clicked by user is ours the alertContext should match our current active screen data's requestToken
            if ( currScreen-> getRequestToken () == alertContext )
            {
                if ( hmiButtonID != CHECKBOX_BUTTON )
                {
                    // cache the result, need to send to the clients based on what user has chosen in the 2nd level popup.
                    currScreen-> result_ = ( currScreen-> result_ == 1 && hmiButtonID == YES_BUTTON )? YES_PLUS_CHECKED : hmiButtonID ;

                    CCOIButtonPtr button = currScreen-> getButton ( hmiButtonID ) ;
                    if ( button )
                    {
                        // check if this button supports any pop up or further level screens, if so populate the screen and send the data to HMI for display
                        if ( button-> isPopupSupported () )
                        {
                            // to make sure token is unique for prompts of various buttons
                            string token = currScreen-> getRequestToken () ;
                            token += ":" ;
                            token += to_string ( hmiButtonID ) ;

                            // screen triggered locally, and the repsonse to the client would be sent based on what is selected in the local popup/ screen.
                            populateButtonPopupScreen ( token, button, bind ( &CCOIUserPromptScreenData::cbUserResponse, this, std::placeholders::_1, std::placeholders::_2 ) ) ;
                            sendDataToHMI () ;
                        }
                        // this button does not support any further level screens/ pop-up, so send the user response to the client
                        else    sendResponseToClient () ; 
                    }
                    else    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_ERROR, "%s:%d No Button with ID %d", __FUNCTION__, __LINE__, hmiButtonID ) ;
                }
                else    currScreen-> result_ = value ;  // checkbox updated no action apart from cachig the value ticked in the checkbox i.e., true of false
            }
        }
    }
}

// Gives the instance of the button from the list of buttons (of userPromptScreenCCOIData) corresponding to the ButtonID
CCOIButtonPtr CCOIUserPromptScreenData::getButton ( int hmiButtonID )
{
    CCOIButtonPtr button = nullptr;

    // build the popup dialogue if this button supports popup upon click & send to HMI for display.
    for ( auto const &btn : this->getButtons() )
    {
        if ( hmiButtonID == btn->getID() )
        {
            button = btn ;
            break ;
        }
    }

    return button ;
}

// callback for locally triggered screen
void CCOIUserPromptScreenData::cbUserResponse ( int choice, const string &token )
{
    // if it is go back button then display previous screen again
    if ( choice == BACK_BUTTON )                  sendDataToHMI () ; // this function always sends the content of the top frame in the active lists stack  
    
    // user has confirmed his selection in the local level popup, so send the response to client.
    else if ( ! isInfoButtonScreen ( token ) )    sendResponseToClient () ;
}

void CCOIUserPromptScreenData::sendResponseToClient ( )
{
    // clear the requests stack and send the response back to client.
    if ( !gActiveUserPrompts.empty() )
    {
        CCOIUserPromptScreenDataPtr currScreen = gActiveUserPrompts. back () ;   // get current active screen data, i.e., top element of the stack requests.
        
        // pop the request from the global active screens stack.
        gActiveUserPrompts. pop_back () ;

        CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_INFO, "%s:%d selected button %d sent to Client", __FUNCTION__, __LINE__, currScreen-> result_ ) ;
        currScreen-> clientHandler_ ( currScreen-> result_, currScreen-> getRequestToken () ) ;
    }
}

// this work around is required as for user prompts we maintain the info popup ideally should be done by HMI, but it requires FIDL change
bool CCOIUserPromptScreenData::isInfoButtonScreen ( const string &token )
{
    bool retVal = false ;

    string key = ":" ;
    key. append ( to_string ( INFO_BUTTON ) ) ; 
    
    retVal = ( token. find ( key ) != string::npos ) ;
    
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_INFO, "%s %d retVal %d", __FUNCTION__, __LINE__, retVal ) ;
    return retVal ;
}

// remove this after intial HMI CCOI testing and maturity of the code.
void CCOIUserPromptScreenData::printHMIData ( hmiDialogueServiceTypes::RequestDialogueAlert &hmiRequest, hmiDialogueServiceTypes::RequestDialogueSpecialAlert &hmiSpecialRequest, bool special )
{
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "%s %d HMI Data start:", __FUNCTION__, __LINE__) ;
     
    if (special)
    {
        hmiRequest = const_cast < hmiDialogueServiceTypes::RequestDialogueAlert & > ( hmiSpecialRequest.getBasicAlertProperties () ) ;  
    }
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "AlertContext %s", hmiRequest.getAlertContext ().c_str() ) ;
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "Priority %d", static_cast < int > (hmiRequest.getPriority ()) ) ;
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "AlertTitleContext %s", hmiRequest.getAlertTitleText ().c_str() ) ;
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "getAlertTimeoutDuration %d", hmiRequest.getAlertTimeoutDuration () ) ;
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "ShowAlert %d", hmiRequest. getShowAlert () ) ;

    hmiDialogueServiceTypes::AlertStringList stringlist;
    stringlist = hmiRequest.getText () ;
   
    for ( unsigned int i = 0; i < stringlist.size(); i++)
        CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "Text %s", stringlist [i].c_str() ) ;

    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "SoftButtons start:") ;
    hmiDialogueServiceTypes::AlertSoftButtonList buttonList;
    for ( auto const &btn : hmiRequest.getSoftButtons() )
    {
        CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "ButtonID %d", btn. getButtonID () ) ;
        CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "ButtonText %s", btn. getButtonText().c_str() ) ;
        CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "ButtonType %d", static_cast < int > ( btn. getButtonType () ) ) ;
        CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "ButtonDisabled %d", btn. getIsButtonDisabled () ) ;
        CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "ButtonAction %d", static_cast < int > ( btn. getButtonAction () ) ) ;
    }
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "SoftButtons end") ;
    if (special)
    {
        CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "SpecialButtons start:") ;
        hmiDialogueServiceTypes::AlertSpecialButtonList specialbuttonlist ;
        for ( auto const &btn : hmiSpecialRequest.getAlertSpecialButton () )
        {
            CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "ButtonID %d", btn. getSpecialButtonID () ) ;
            CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "ButtonText %s", btn. getSpecialButtonText().c_str() ) ;
            CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "ButtonType %d", static_cast < int > ( btn. getButtonType () ) ) ;
            CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "InfoButtonID %d", btn. getInfoButtonID () ) ;
            CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "SpecialButtonValue %d", btn. getSpecialButtonValue () ) ;
            CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "TakeActionOnSpecialButton %d", btn. getTakeActionOnSpecialButton () ) ;
            CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "ButtonDisabled %d", btn. getIsButtonDisabled () ) ;
            CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "ButtonAction %d", static_cast < int > ( btn. getButtonAction () ) ) ;
        }
        CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "SpecialButtons end") ;
    }
    CcoiLog.write (CCOILog::eLogCCOIUserPromptScreenData, CCOILog::LOG_DEBUG, "%s %d HMI Data end", __FUNCTION__, __LINE__) ;
}
			} // namespace voice
		} // namespace harman
	} // namespace com
} // namespace v0

