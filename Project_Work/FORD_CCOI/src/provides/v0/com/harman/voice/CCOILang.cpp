#include "CCOILang.hpp"

namespace v0 {
    namespace com {
        namespace harman {
            namespace voice {

//TODO: update this list with correct language labels once Ford client responds with the list of language strings they support
landIdsToNamesMap CCOILang::langdIdsMap_ =
{
    {   VehicleType::eDisplayLang::Invalid,             "invalid"   },
    {   VehicleType::eDisplayLang::Unknown,             "unknown"   },
    {   VehicleType::eDisplayLang::UK_English,          "en-gb"     },
    {   VehicleType::eDisplayLang::NA_English,          "en-us"     },
    {   VehicleType::eDisplayLang::German,              "de-de"     },
    {   VehicleType::eDisplayLang::Italian,             "it-it"     },
    {   VehicleType::eDisplayLang::EU_French,           "fr-fr"     },
    {   VehicleType::eDisplayLang::Cana_French,         "fr-ca"     },
    {   VehicleType::eDisplayLang::EU_Spanish,          "es-es"     },
    {   VehicleType::eDisplayLang::Mex_Spanish,         "es-mx"     },
    {   VehicleType::eDisplayLang::Turkish,             "tr-tr"     },
    {   VehicleType::eDisplayLang::Russian,             "ru-ru"     },
    {   VehicleType::eDisplayLang::Dutch,               "nl-nl"     },
    {   VehicleType::eDisplayLang::Polish,              "pl-pl"     },
    {   VehicleType::eDisplayLang::Czech,               "cs-cz"     },
    {   VehicleType::eDisplayLang::Swedish,             "sv-se"     },
    {   VehicleType::eDisplayLang::Danish,              "da-dk"     },
    {   VehicleType::eDisplayLang::Norwegian,           "no-no"     },
    {   VehicleType::eDisplayLang::Finish,              "fi-fi"     },
    {   VehicleType::eDisplayLang::EU_Portuguese,       "pt-pt"     },
    {   VehicleType::eDisplayLang::Braz_Portuguese,     "pt-br"     },
    {   VehicleType::eDisplayLang::AU_English,          "en-au"     },
    {   VehicleType::eDisplayLang::Korean,              "ko-kr"     },
    {   VehicleType::eDisplayLang::Mandarin_Chinese,    "zh-cn"     },
    {   VehicleType::eDisplayLang::Arabic,              "ar-sa"     },
    {   VehicleType::eDisplayLang::Thai,                "th-th"     }
} ;

string CCOILang::getLangName ( VehicleType::eDisplayLang id )
{
    string emptyStr ;

    auto value = langdIdsMap_.find ( id ) ; 

    if ( value != langdIdsMap_.end () )
        return value->second ;

    return emptyStr ;
}

            } // voice 
        } // harman
    } // com
} // v0


