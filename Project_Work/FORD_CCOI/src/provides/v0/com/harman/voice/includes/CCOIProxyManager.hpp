
#ifndef _CCOI_PROXY_FACTORY_
#define _CCOI_PROXY_FACTORY_

#include <string> 
#include <memory>
#include <map>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/com/harman/infotainment/systeminfrastructure/versionmanager/versionmanager/VersionManagerProxy.hpp>
#include <core/CommonAPIClientAbstraction.h>

#include "CCOIManager.hpp"
#include "CCOIUserPromptScreenData.hpp"

#include "v0/org/harman/ford/bodycontroldata.hpp"
#include "v0/org/harman/ford/bodycontroldataProxy.hpp"
#include "v0/org/harman/ford/bodycontroldata_types.hpp"

#include "v1/com/harman/vehicle/VehicleSettings.hpp"
#include "v1/com/harman/vehicle/VehicleSettingsProxy.hpp"
#include "v1/com/harman/vehicle/VehicleType.hpp"

#include "v0/com/harman/vehicle/MasterReset.hpp"
#include "v0/com/harman/vehicle/MasterResetProxy.hpp"
#include "v0/com/harman/vehicle/MasterResetProxyBase.hpp"

#include "v1/com/harman/swdl/SoftwareUpdateTypes.hpp"
#include "v0/com/harman/swdl/SoftwareUpdateService.hpp"
#include "v0/com/harman/swdl/SoftwareUpdateServiceProxy.hpp"
#include <v0/com/harman/swdl/SoftwareUpdateServiceProxyBase.hpp>

#include "v0/org/harman/ford/notifalert_types.hpp"
#include "v0/org/harman/ford/notifalert.hpp"
#include "v0/org/harman/ford/notifalertProxy.hpp"

#include "HmiInterfaces.hpp"

// Macro definitions
#define MAX_TCU_ESN_LENGTH      ( 12 )

// namespace declaratives
using namespace std ;
using namespace v0::com::harman::swdl ;
using namespace v1::com::harman::swdl ;
using namespace v0::org::harman::ford ;
using namespace v0::com::harman::vehicle ;
using namespace v1::com::harman::vehicle ;
using namespace v1::com::harman::hmiDialogueService ;
using namespace com::harman::hmiDialogueServiceTypes ;
using namespace v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanager ;
using namespace v1::com::harman::infotainment::systeminfrastructure::versionmanager::versionmanagertypes ;

namespace v0    {
    namespace com    {
        namespace harman    {
            namespace voice    {

// forward declaration
class CCOIManager ;

typedef function < void ( CommonAPI::AvailabilityStatus ) > proxyAvblCallback ;
class CCOIProxyManager
{
    private:

        // holds runtime of the middleware
        shared_ptr < CommonAPI::Runtime > runtime_ ;

        // stores the current display language intimated by vehicle settings proxy
        VehicleType::eDisplayLang displayLangId_ ;
        // stores the software download state intimated by SWDL proxy
        SoftwareUpdateTypes::eSwdlState softwareDownload_ ;
        // stores the current region the country belongs to, intimated by vehicle settings proxy
        VehicleType::eCountryRegion region_ ;
        // stroes the driving restrictions status of the vehicle, intimated by vehicle settings proxy
        bool drivingRestrictions_ ;

        void init ( void ) ;
        bool    checkAndSubscribeProxy ( shared_ptr < CommonAPI::Proxy > proxyHandle, const proxyAvblCallback &avblCallback ) ;
        
        shared_ptr < MasterResetProxy <> >                   masterResetProxy_     ;
        shared_ptr < VersionManagerProxy <> >                versionManagerProxy_  ;
        shared_ptr < notifalertProxy <> >                    notifAlertProxy_      ;
        shared_ptr < bodycontroldataProxy <> >               bodyCtrlDataProxy_    ;
        shared_ptr < VehicleSettingsProxy <> >               vehicleSettingsProxy_ ;
        shared_ptr < SoftwareUpdateServiceProxy <> >         softwareUpdateProxy_  ;
        shared_ptr < hmiDialogueServiceInterfaceProxy <> >   hmiProxy_             ;
        
    public:

        // constructors
        CCOIProxyManager () ;

        // client instances to send the proxies responses back.
        CCOIManager *pccoi ;

        void getAllProxies ( void ) ;
    public:

        /*****   Availability callbacks of proxies   *****/
        void    cbHMIProxyAvbl             ( CommonAPI::AvailabilityStatus Status ) ;
        void    cbVehicleSetProxyAvbl      ( CommonAPI::AvailabilityStatus Status ) ;
        void    cbMasterResetProxyAvbl     ( CommonAPI::AvailabilityStatus Status ) ;
        void    cbAMBBodyCtrlProxyAvbl     ( CommonAPI::AvailabilityStatus Status ) ;
        void    cbNotifyAlertProxyAvbl     ( CommonAPI::AvailabilityStatus Status ) ;
        void    cbSoftwareUpdateProxyAvbl  ( CommonAPI::AvailabilityStatus Status ) ;
        void    cbVersionManagerProxyAvbl  ( CommonAPI::AvailabilityStatus Status ) ;

        /*****   Data update callbacks from proxies   *****/
        void    cbDisplayLangIdUpdate       ( VehicleType::eDisplayLang dispLangId ) ;
        void    cbMasterReset               ( ResetType::eFactoryResetParams eResetParams ) ;
        void    cbTcuStateChange            ( notifalert_types::GWM_Send_Signals_8_HS3_sig_4_struct tGWM_Send_Signals ) ;
        void    cbSoftwareUpdateProxy       ( SoftwareUpdateTypes::tStatus status, SoftwareUpdateTypes::eUpdateSource eSource ) ;
        void    onButtonClicked             ( hmiDialogueServiceTypes::ButtonInteraction hmiresp ) ;
        void    onSpecialButtonClicked      ( hmiDialogueServiceTypes::SpecialButtonInteraction hmiresp ) ;
        void    cbRegionUpdate              ( VehicleType::eCountryRegion region ) ;
        void    cbDrivingRestrictionsUpdate ( bool status ) ;

        /*****  Data update calls to proxies from other CCOI components   *****/       
        bool    sendNotifAlert ( int value ) ;
        void    setTcuVersion ( string &version ) ;
        void    setAutomaticUpdates ( bool value ) ;
        bool    sendDataToHMI ( bool special, hmiDialogueServiceTypes::RequestDialogueAlert data, hmiDialogueServiceTypes::RequestDialogueSpecialAlert specialData ) ;

        /*****  Clients calls to retrieve proxies informed data   *****/
        VehicleType::eCountryRegion getRegion () ;
        bool                        packageDownloading () ;
        VehicleType::eDisplayLang   getCurrentDispLanguage () ;
        bool                        getDrivingRestrictions () ;
        
        // destructor
        virtual ~CCOIProxyManager () ;
};

            }   // voice
        }   // harman
    }   // com
}   // v0

#endif
