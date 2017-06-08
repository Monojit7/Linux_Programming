#ifndef CCOIUSERPROMPTSCREENDATA_HPP
#define CCOIUSERPROMPTSCREENDATA_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <cstdlib>
#include "CCOIManager.hpp"
#include "CCOIException.hpp"
#include "CCOIButton.hpp"
#include "CCOIProxyManager.hpp"
#include "CCOILog.hpp"

#include "HmiInterfaces.hpp"

// MACROS
#define INFO_TEXT_TIMEOUT             ( 5000 )
#define GO_BACK                       ( BACK_BUTTON )
// TODO: To check if this needs to be done based on language in that case can we base it upon '<' & '>' tokens to parse and replace it.
#define VARTEXT_KEY_1	              ( "%elementName%" )
#define VARTEXT_KEY_2	              ( "%appName%" )
#define IS_HMI_SPECIALBUTTON(btn)     ( ( btn == CHECKBOX_BUTTON ) || ( btn == INFO_BUTTON ) )
#define IS_VISIBLE(btn)               ( ( btn != nullptr ) && ( btn->isVisible () == true ) )

//using declaratives for used namespaces.
using namespace std ;
using namespace v1::com::harman::hmiDialogueService ;
using namespace com::harman::hmiDialogueServiceTypes ;

namespace v0 {
	namespace com {
		namespace harman {
			namespace voice {

// TYPEDEFS for pointers, structures etc,.
typedef shared_ptr < CCOIButton >   CCOIButtonPtr ;
typedef function < void ( int, const string& ) > ClientHandlerType ;
typedef shared_ptr < hmiDialogueServiceInterfaceProxy < > > hmiProxyPtr ;

class CCOIUserPromptScreenData {

	// unique token the TCU sends in the User Prompt payload packets, required to be sent back in the response to TCU.
	string requestToken_ ;

	// the body text of the screen to be displayed on the screen, contains the sub header information as well 
	// (separated with \r\n 's with the rest of the text..)
	string text_ ;

    vector < string > varTexts_ ;
    vector < string > varTextKeys_ ;

    // timeout in millisecs for this screen.
    int timeout_ ;

	// container to hold the buttons the prompt screen supports.
	vector < CCOIButtonPtr > buttons_ ;
	
    // client handler to cllback with the user choice
    ClientHandlerType clientHandler_ ;
    
	// private function to populate HMI alert data, convert the data we populated into the one HMI would understand.
    bool populateHMIData ( hmiDialogueServiceTypes::RequestDialogueAlert &hmiRequest ) ;

	// private function to populate HMI special alert data, convert the data we populated into the one HMI would understand.
    bool populateHMIData ( hmiDialogueServiceTypes::RequestDialogueAlert &hmiRequest, hmiDialogueServiceTypes::RequestDialogueSpecialAlert &hmiSpecialRequest ) ;

	// private function to populate the buttons of the button popup screen.
    bool populateButtons ( CCOIButtonPtr &button  ) ;
	
	// forms the screenText replacing the variable keys in it with right text sent form the client)
	bool replaceVarsInScreenText ( ) ;
	
    // populates the button popup screen
    void populateButtonPopupScreen ( string &requestToken, CCOIButtonPtr button, const ClientHandlerType &clientHandler ) ;

    CCOIButtonPtr getButton ( int buttonID ) ;

    void printHMIData ( hmiDialogueServiceTypes::RequestDialogueAlert &hmiRequest, hmiDialogueServiceTypes::RequestDialogueSpecialAlert &hmiSpecialRequest, bool special ) ;
    
    int  result_ ;
    
    // helper function to get the type of special button - HMI enums, & also fills our data objects used to populate HMI understandable data
    hmiDialogueServiceTypes::SpecialButtonType getHMISpecialButtonType ( CCOIButtonPtr &checkboxbtn, CCOIButtonPtr &infobtn ) ;

    bool isInfoButtonScreen ( const string &token ) ;

	public:

		// constructors
		CCOIUserPromptScreenData ( ) ;
		CCOIUserPromptScreenData ( const ClientHandlerType &clientHandler ) ;
        CCOIUserPromptScreenData ( string &requestToken, CCOIButtonPtr button, const ClientHandlerType &clientHandler ) ;

		// destructor
		virtual ~CCOIUserPromptScreenData () ;
		
		// getters
		inline string  getRequestToken ()       const               { return requestToken_ ;          }
		inline string  getScreenText ()         const               { return text_ ;                  }	
        inline bool    getTimeOut ()            const               { return timeout_ ;               }
		
        inline vector < CCOIButtonPtr > & getButtons ()             { return buttons_ ;               }
        inline vector < string >          getVariableTexts ()       { return varTexts_ ;              }
	
		// setters
		void        setRequestToken  ( string &reqToken ) ;
		void        setScreenText    ( string &scrntext ) ;
        void        addVariableTexts ( string &text ) ;

        inline void setTimeOut       ( int val )                    { timeout_ = val ;                }

		// adds a button into the contents of the screen data
		inline void addButton ( CCOIButtonPtr b)                    { buttons_.push_back ( b ) ;      }

		// removes a button into the contents of the screen 
	    // Implement if this is required in future at present we do not have any requirement to remove the buttons in the screen dynamically
		inline void removeButton ( CCOIButtonPtr b)                 { }
		
		// sends the populated data to HMI, called by the clients
		bool sendDataToHMI ( ) ;
        
        // Button interaction event from the user on the user prompt screen.
        void onButtonClicked ( hmiDialogueServiceTypes::ButtonInteraction hmiresp ) ;

        // Button interaction event on special buttons such as (CheckBox, Icon etc,. ) from the user on the user prompt screen.
        void onSpecialButtonClicked ( hmiDialogueServiceTypes::SpecialButtonInteraction hmiresp ) ;
        
        // sends the user response to the client using clienthandler callback
        void sendResponseToClient ( ) ;

        // triggers form outside to show a screen if anything is queued up earlier due to HMI not available or drivingRestrictions etc,.
        void sendScreen ( bool available ) ;
        
        // adds the current object to the gloabl list so as to communicate to HMI later on
        void addScreen ( ) ;

        // callback for locally triggered screen
        void cbUserResponse ( int choice, const string &token ) ;
        
        // called by proxy to inform the user choice handles button click response on both special & normal alerts
        void buttonClicked ( const string& alertcontext, int hmiButtonID, int action, bool value = false ) ;
    
        // utility which replaces all the occurences of key in the string with replacing text
        static bool replaceAll ( string &src, string key, int startPos, string replacing ) ;
};

			} // namespace voice
		} // namespace harman
	} // namespace com
} // namespace v0

#endif // CCOIUSERPROMPTSCREENDATA_HPP
