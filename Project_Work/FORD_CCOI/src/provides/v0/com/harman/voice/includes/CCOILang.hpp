#ifndef _CCOI_LANG_HPP_
#define _CCOI_LANG_HPP_

#include <string>
#include <unordered_map>

// for language Ids
#include "v1/com/harman/vehicle/VehicleSettings.hpp"
#include "v1/com/harman/vehicle/VehicleSettingsProxy.hpp"
#include "v1/com/harman/vehicle/VehicleType.hpp"

// namespace declaratives
using namespace std ;
using namespace v1::com::harman::vehicle ;

namespace v0 {
    namespace com {
        namespace harman {
            namespace voice {

typedef unordered_map < VehicleType::eDisplayLang, string > landIdsToNamesMap ;

class CCOILang
{
    public:
        // Maps the languageIds to the language names that TCU or other CCOI components understands.
        static landIdsToNamesMap langdIdsMap_ ;

        static string getLangName ( VehicleType::eDisplayLang id ) ;
};

            } // voice 
        } // harman
    } // com
} // v0
#endif // _CCOI_LANG_HPP_

