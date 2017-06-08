
#include "CCOIButton.hpp"

// namespaces definition
namespace v0 {
	namespace com {
		namespace harman {
			namespace voice {

//constructors
CCOIButton::CCOIButton () : id_ (0), name_ (""), value_ (""), visible_ (false), enabled_ (false)
{

	CcoiLog.write (CCOILog::eLogCCOIButton, CCOILog::LOG_INFO, "empty button created" ) ;
}

CCOIButton::CCOIButton (int id, string name, string text, bool popup, bool vis, bool ena) : id_ (id), name_ (name), value_ (text), isPopup_ (popup), visible_ (vis), enabled_ (ena)
{

	CcoiLog.write (CCOILog::eLogCCOIButton, CCOILog::LOG_INFO, "button: id %d name %s text %s popup %d vis %d ena %d created.", id_, name_.c_str(), value_.c_str (), 
                                                                                              isPopup_, visible_, enabled_) ;
}

bool CCOIButton::isPopupSupported () const
{
    bool retVal = false ;

    retVal = isPopup_ && !( getValue ().empty () ) ;

	CcoiLog.write (CCOILog::eLogCCOIButton, CCOILog::LOG_INFO, "button: id %d name %s value %s isPopup %s supported", id_, name_.c_str(), getValue ().c_str (), ((retVal)? "" : "not") ) ;

	return retVal ;
}

// CCOIMenuButton class

//constructors
CCOIMenuButton::CCOIMenuButton () : CCOIButton (), entityId_ (""), entityType_ ("")
{

	CcoiLog.write (CCOILog::eLogCCOIButton, CCOILog::LOG_INFO, "empty button created" ) ;
}

CCOIMenuButton::CCOIMenuButton (string entityId, string entityType, int id, string name, string on, string off, bool popup, bool OnOffVisible, bool OnOff, bool vis, bool ena, int seq ) : 
                CCOIButton (id, name, on, popup, vis, ena), entityId_ (entityId), entityType_ (entityType), OnValue_ (on), OffValue_ (off), OnOffVisible_ (OnOffVisible), OnOffStatus_ (OnOff), sequence_ (seq)
{

	CcoiLog.write (CCOILog::eLogCCOIButton, CCOILog::LOG_INFO, "Menu button: entityID %s entityType %s OnText %s offText %s created.", 
                                                                entityId_. c_str (), entityType_. c_str (), OnValue_. c_str (), OffValue_. c_str (), OnOffVisible_, OnOffStatus_, seq ) ;
}

			} // namespace voice
		} // namespace harman
	} // namespace com
} // namespace v0


