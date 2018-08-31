#pragma once

#include <string>
#include <vector>
#include <functional>
#include <RemoteCDCore.hpp>
#include <RemoteCDTypes.hpp>
#include <RemoteCDEvents.hpp>
#include <RemoteCDLog.hpp>

#define     T_FAST              ( 5 *1000 ) // 5 seconds for fast forward/ reverse as per SPSS 
#define     PLAYTIME_OFFSET     ( - 100 ) // As per FORD dbc file initial offset value for playtime and totalTime is -100 

// namespace declaratives
using namespace std ;
using namespace std::placeholders ;
using namespace v0::org::harman::ford ;

    class RemoteCDPlayControls : public ITimerEventListener
    {
        public:
            // constructor
            RemoteCDPlayControls ( CommonAPIClientAbstraction < mediaplaybackProxy >   &player_ ) ;

            // clients play control APIs related            
            virtual void setAction ( eCDPlayActions action ) ;
            virtual void setContext ( CDPlayContext context ) ;
            virtual void setPlayItem ( CDPlayItem item ) ;
            
            // playerproxy available notificatoin from RemoteCDManager
            void    playerAvailable ( bool available ) ;

            // player callbacks for the attributes changed
            void ACUSignals5 ( mediaplayback_types::ACU_Send_Signals_5_struct signal ) ;
            void ACUSignals6 ( mediaplayback_types::ACU_Send_Signals_6_struct signal ) ;
            void ACUSignals9 ( mediaplayback_types::ACU_SendSignals_9_struct  signal ) ;

            // implement timer callback to be notified about the timer event we set
            void timerEvent ( int timerId ) ;

            // plf async callbacks
            void    plfCallback1 ( const CommonAPI::CallStatus& status, uint8_t   plfValue ) ;
            void    plfCallback2 ( const CommonAPI::CallStatus& status, uint16_t  plfValue ) ;
            void    plfCallback3 ( const CommonAPI::CallStatus& status, uint32_t  plfValue ) ;
            void    plfCallback4 ( const CommonAPI::CallStatus& status, mediaplayback_types::NAV_SetOperationMode_Rq_enum  plfValue ) ;

            // destructor
            virtual ~RemoteCDPlayControls () ;
        
        private:

            // timer related
            CTimerInterface             timer_ ;
            int                         timerId_ ;

        private:
            
            // cd player instance RemoteCDManager would send, we need it communicate with player for player controls functionality
            CommonAPIClientAbstraction < mediaplaybackProxy >   &player_ ;
        
            // to indicate the state of plf signals subscription
            bool subscribed_ ;

            // status variables book keep to publish events to client only upon a change in the values of the respective status/ values
            eCDPlayActions              playAction_ ;
            vector < CDPlayContext >    playContexts_ ;
            TrackNumber                 activeTrack_ ;
            TrackTime                   trackPlayTime_ ;
            uint64_t                    numberOfTracks_ ;
            
            // Helpers

            // to initialize members
            void    init () ;
            
            // subscribes/ unsubcribes the interested attributes from the player
            void    subscribeSignals ( bool subscribe ) ;

            // processes the data received from the player sent as a parameter to these and publishes the data to client
            void    processPlayContext ( CDPlayContext response ) ;
            void    processPlayAction ( int action ) ;
            void    processPlayTime ( uint16_t playTime, uint16_t totalTime ) ;
            void    processNumberOfTracks ( uint16_t numTracks ) ;

            // sends the data/ values to the player
            void    setContextToPlayer ( CDPlayContext context ) ;
            void    setPlayFolderToPlayer ( short folderNumber ) ;
            void    setTrackToPlayer ( eCDPlayDirection direction, CDPlayItemNumber trackNumber ) ;
            void    setTrackToPlayer ( uint32_t trackNumber ) ;
            void    skipTrackToPlayer ( eCDPlayDirection direction ) ;
    };
