#ifndef __CCOI_MESSAGES_UTIL_
#define __CCOI_MESSAGES_UTIL_

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <jsoncpp/json.h>

#include "CCOILang.hpp"
#include "CCOIUserPromptScreenData.hpp"
#include "CCOIPersist.hpp"
#include "EntityElement.hpp"
#include "v2/com/harman/voice/CCOIPresCtrlStubImpl.hpp"

using namespace v2::com::harman::voice ;

namespace v0 {
    namespace com {
        namespace harman {
            namespace voice {

// macro definitions
#define VARIABLE_TEXT_LENGTH    ( 128 ) // 128 bytes of data
#define APP_BRAND_MSG_CODE      ( "100" ) // app and brand name comes in message code 100 in the UFM

//forward declaration
class CCOIUserPromptScreenData ;

class CCOIMessagesUtil
{
     private:
       
        EntityListVector                    listVector_ ;
        unordered_map < string, string >    variableKeyNamesMap_ ;

    private:
   
        /*****   USER PROMPTS related PTE, UFM data processing  *****/ 

        string  processUserPromptMetaData ( string &userPromptMetadata, CCOIUserPromptScreenData &userPrompt ) ;
        bool    populateUserPromptButtons ( Json::Value &initialPrompt, CCOIUserPromptScreenData &userPrompt ) ;

        /*****   MENU SETTINGS related PTE, UFM data processing  *****/ 

        bool    populateMenu ( const Json::Value &featurePolicies, int index, int sequence ) ;
        bool    populateMenuPrompt ( const Json::Value &root, CCOIMenuButton &menu, CCOIButton &info ) ;
        bool    populateMenuConsentPrompt ( const Json::Value &root, CCOIMenuButton &menu ) ;
        bool    packageDownloadMsg ( CCOIMenuButton &menu ) ;
        // generic functions to get conditions match them etc,.
        int     getMatchIndex ( EntityList &entities ) ;
        int     findEntity ( Entity entity, EntityList &entities ) ;
        bool    getListVector ( Json::Value &root, string key, string flagKey ) ;
        bool    getList ( Json::Value &root, EntityList &entityList, string flagKey, string priority ) ;
        bool    getList ( Json::Value &root, EntityList &entityList, string key, string flagKey, string priority ) ;

        /*****   updates/ responses to clients   *****/

        bool    updateIconData (  Json::Value &root, int index ) ;
        bool    updatePromptPolicies (  Json::Value &root, int index ) ;
        bool    updateControllingEntities (  Json::Value &root, int index ) ;
        bool    updateCCSMenuEntries ( Json::Value &root, Json::Value &policies, int index ) ;
        
        /*****   Helpers/ utility functions   *****/

        string  getUpdatedMessage ( Json::Value &root, string key ) ;
        bool    getCurrentLangMessageFromUFM ( Json::Value &message, Json::Value &langMessage ) ;
        bool    getMessageFromUFM ( string &messageCode, Json::Value &message, bool Default = false ) ;
        string  getMessage ( string name, string messageCode, string key, Json::Value &root, bool Default = false ) ;

    public:

        // constructors
        CCOIMessagesUtil () ;
        
        /*****   EXTERNAL calls from CCOIManager, CCOIPresCtrl etc,.   *****/
        bool    populateEntries ( string entriesType, EntityList entities ) ;
        bool    populateUserPrompt ( string &userPromptMetadata, CCOIUserPromptScreenData &userPrompt, bool Default = false ) ;

        // stroes app & brand name, this might also be needed for others hence precomputing into a static variable of this util class
        static string appName_ ;
        static string brandName_ ;
        static string modemName_ ;

        // to get the app & brand names from UFM and update above class variables
        string  getName  ( string name, bool Default = false ) ;
        string  getValue ( string name, string msgCode, string key, bool Default = false ) ;
        string  getMessage ( string name, string messageCode, string key, bool Default = false ) ;

        // destructor
        virtual ~CCOIMessagesUtil () ;
};

            } // voice
        } // harman
    } // com
} // v0

#endif // __CCOI_MESSAGES_UTIL_
