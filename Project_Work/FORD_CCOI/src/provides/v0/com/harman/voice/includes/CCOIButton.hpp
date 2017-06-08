#ifndef _CCOIBUTTON_HPP__
#define _CCOIBUTTON_HPP__

#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <memory>
#include "CCOILog.hpp"

//using declaratives for used namespaces.
using namespace std;

// namespaces definition
namespace v0 {
	namespace com {
		namespace harman {
			namespace voice {

// Id/ Types values is mapped ot the status enums TCU expects so as to avoid unnecessary mapping again while in TCU reply from HMI
enum ButtonTypes {

	NO_BUTTON      = 0x2,   // SELECT_NO for TCU
	ASK_ME_LATER,           // SELECT_ASK_ME_LATER for TCU
	YES_BUTTON,             // SELECT_YES for TCU
	CHECKBOX_BUTTON,
	INFO_BUTTON,
    OK_BUTTON,
    BACK_BUTTON,
    CONTINUE_BUTTON,
    CANCEL_BUTTON,
    MENU_BUTTON,
    ICON_BUTTON,
    CLOSE_BUTTON,
    YES_PLUS_CHECKED
};

class CCOIButton {

	protected:
		// unique id for the type of button, TODO: to check if this is really needed.
		int id_ ;

		// name of the button as displayed on the screen ex: YES, NO, ASK ME LATER, OK, BACK etc,.
		string name_ ;

		// is optional: would contain a value (text) to the button which supports a pop-up that needs to be displayed by HMI when chosen.
		string value_ ;

		// flag to support: when the button is clicked, some buttons support popup message & some do not.
		bool isPopup_ ;

		// true - button would be displayed, false - button would not be displayed.
		bool visible_ ;

		// true - button is actionable for click, false - button is disabled i.e., greyed out.		
		bool enabled_ ;

	public:
		//constructors
		CCOIButton () ;
		
		CCOIButton (int id, string name, string text, bool popup = true, bool vis = true, bool ena = true) ;
	
		// destructor
		virtual ~CCOIButton () { }
		
		// getters
		inline string  getName ()      const  { return name_ ;    }
		inline int 	   getID ()        const  { return id_ ;      }
		inline string  getValue ()     const  { return value_ ;   }
		inline bool    isVisible ()    const  { return visible_ ; }
		inline bool    getEnabled ()   const  { return enabled_ ; }
        
        // setters
        inline void    setName    ( string &name )     { name_    = name ;  }
        inline void    setValue   ( string value)      { value_   = value ; }
        inline void    setPopup   ( bool   value )     { isPopup_ = value ; }
		inline void    setEnabled ( bool   value )     { enabled_ = value ; }
		inline void    setVisible ( bool   value )     { visible_ = value ; }
		
	    // returns true if the button supports popup, clients would populate the popup screen with the text of the screen taken from value_ of the button.	
		bool isPopupSupported() const ;
		
};

typedef shared_ptr < CCOIButton >   CCOIButtonPtr ;

// CCOIMenuButton class
class CCOIMenuButton : public CCOIButton
{

		// entityId sent by TCU to uniquely identify a menu button
		string entityId_ ;

		// entityType sent by TCU to uniquely identify a menu button
        string entityType_ ;        

        // text that would be displayed in prompt when the toggle of this menu is turned ON.
        string OnValue_ ;

        // text that would be displayed in prompt when the toggle of this menu is turned OFF.
        string OffValue_ ;

        // toggle switch visible or not
        bool OnOffVisible_ ;

		// toggle button status of the menu item true = TOGGLE_ON, false - TOGGLE_OFF
		bool OnOffStatus_;
    
        // icon button of the menu button
        CCOIButtonPtr icon_ ;

        int sequence_ ;

	public:
		//constructors
		CCOIMenuButton () ;
		
		CCOIMenuButton (string entityId, string entityType, int id, string name, string on, string off, bool popup = true, bool OnOffVisible = true, bool OnOff = true, bool vis = true, bool ena = true, int seq = 0 ) ;
	
		// destructor
		virtual ~CCOIMenuButton () { }
	    
        // getters 
        string  getEntityId ()                              {   return entityId_ ;      }
        string  getEntityType ()                            {   return entityType_ ;    }
        bool    getOnOffStatus()                            {   return OnOffStatus_ ;   }
        bool    getOnOffVisible()                           {   return OnOffVisible_ ;  }
        string  getOnValue ()                               {   return OnValue_ ;       }
        string  getOffValue ()                              {   return OffValue_ ;      }
        int     getSequence ()                              {   return sequence_ ;      }

                // setters
        void    setOnOffVisible ( bool   value )           {   OnOffVisible_ = value ;  }
        void    setOnOffStatus  ( bool   value )           {   OnOffStatus_  = value ;  }
        void    setOnValue      ( string value )           {   OnValue_      = value ;  }
        void    setOffValue     ( string value )           {   OffValue_     = value ;  }
        void    setSequence     ( int    value )           {   sequence_     = value ;  }

};

			} // namespace voice
		} // namespace harman
	} // namespace com
} // namespace v0

#endif	// _CCOIBUTTON_HPP__
