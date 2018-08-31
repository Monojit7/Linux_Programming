#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <RemoteCDCore.hpp>
#include <IRemoteCDPlugin.hpp>
#include <RemoteCDPlayControls.hpp>
#include <RemoteCDBrowse.hpp>
#include <RemoteCDTP.hpp>
#include <RemoteCDLog.hpp>
#include <RemoteCDConfig.hpp>

// namespace declaratives
using namespace std ;
using namespace std::placeholders ;

// plugin hooks for the client to create or destroy us
extern "C" IRemoteCDPlugin* CreatePlugin () ;
extern "C" void DestroyPlugin ( IRemoteCDPlugin* base ) ;

// forward declaration
class RemoteCDBrowse ;

class RemoteCDManager : public IRemoteCDPlugin
{
    public:
        // gets the signleton of the Manager component 
        static RemoteCDManager* getInstance ()    {  if ( ! instance_ )    instance_ = new RemoteCDManager () ;    return instance_ ;  }

        // client calls these to init & deInit our things
        bool init () ;
        bool deInit () ;
        void initEXTEOL (); 
        
        // play controls related            
        virtual void setAction ( eCDPlayActions action ) ;
        virtual void setContext ( CDPlayContext context ) ;
        virtual void setPlayItem ( CDPlayItem item ) ;
        
        // browse related
        virtual void getBrowseInfo ( FolderRequest request ) ;
        virtual void getFolderItemCount ( short FolderNumber );
        virtual void getFolderName ( short folderNumber, int nameLength, FolderRequestedData::eCutNameType cutName ) ;
        
        virtual eEOLResult getFordEolData (); 

        // ignition of disc - load/ Eject
        virtual void load ( bool Load = false ) ;

        // platform callbacks to send the data to us from player
        void    ACUSignals5 ( mediaplayback_types::ACU_Send_Signals_5_struct signal ) ;
        void    CDSignals ( mediaplayback_types::CD_Send_Signals_struct signal ) ;
        
        // playerproxy available notification from commonAPI
        void    playerAvailable ( const CommonAPI::AvailabilityStatus& status ) ;

        // sends the data/ values to the player
        void    requestTOC () ;
        void    requestFolder ( short folderNumber, int startIndex, int numItems ) ;
        void    requestFolderName ( short folderNumber, short nameLength, FolderRequestedData::eCutNameType cutname = FolderRequestedData::eHead ) ;

        // plf async callbacks
        void    plfCallback1 ( const CommonAPI::CallStatus& status, uint8_t   plfValue ) ;
        void    plfCallback2 ( const CommonAPI::CallStatus& status, uint16_t  plfValue ) ;
        void    plfCallback3 ( const CommonAPI::CallStatus& status, uint32_t  plfValue ) ;
        void    plfCallback4 ( const CommonAPI::CallStatus& status, mediaplayback_types::Dsp_CutItem_Rq_enum  plfValue ) ;
        void    plfCallback5 ( const CommonAPI::CallStatus& status, mediaplayback_types::Dsp_CutItem2_Rq_enum  plfValue ) ;
        void    plfCallback6 ( const CommonAPI::CallStatus& status, mediaplayback_types::Dsp_FolderContent_Rq_enum  whichFolder ) ;
        
        void    keyInputAvailable ( const CommonAPI::AvailabilityStatus& status );
       // void    eep2ButtonPresEvent ( key_input_types::EFP2_Button_Press_struct mEFP2Button_ );
        void    processCDType ( int type ) ;
        void    processCDStatus ( int status ) ;
        void    processCDError ( int cdError, int HWError ) ;
        void    processCDEject ( uint8_t Button_A );
        
        void    subscribeSignals ( bool subscribe ) ;
        

        // destructor
        virtual ~RemoteCDManager () ;
        
                // cd player instance
        CommonAPIClientAbstraction < mediaplaybackProxy >   player_ ;
        
        // key input instance
        
        
        // CommonAPIClientAbstraction < key_inputProxy >   keyInput_ ;
        
                // sub components which contains the actual implementation of respective client calls 
        shared_ptr < RemoteCDPlayControls >     playControl_ ;
        shared_ptr < RemoteCDBrowse >           browser_ ;
        shared_ptr < RemoteCDTP >               tp_ ;
        shared_ptr < CRemoteCDConfig >          RemoteCDConfig_ ;
        
    private:
        // constructor
        RemoteCDManager () ;

        // singleton manager instance
        static RemoteCDManager*     instance_ ;

        // status variables book keep to publish events to client only upon a change in the values of the respective status/ values
        eCDType                     cdType_ ;
        eCDStatus                   cdStatus_ ;
        CDError                     cdError_ ;


        
        // to indicate the state of plf signals subscription
        bool subscribed_ ;


        
        // returns whether we have a validCD informed by the player, used to handle client request only when thi sreturns true
        bool    validCD () ;

        // subscribes/ unsubcribes the interested attributes from the player
        

        // processes the data received from the player sent as a parameter to these and publishes the data to client

        // init External CD EOL item
        
};
