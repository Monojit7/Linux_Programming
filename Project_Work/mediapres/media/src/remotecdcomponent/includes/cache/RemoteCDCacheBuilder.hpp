#pragma once

#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>
#include <RemoteCDCore.hpp>
#include <RemoteCDItem.hpp>
#include <RemoteCDFolder.hpp>
#include <RemoteCDManager.hpp>
#include <RemoteCDEvents.hpp>
#include <RemoteCDLog.hpp>

// namespace declaratives
using namespace std ;

#define  ROOT_FOLDER_NUMBER             ( 0 )   // root folder number will be 0 as per SPSS
#define  MAX_FOLDERS_IN_CACHE           ( 500 ) // change this value based on the performance on memory side & execution flow cost
#define  MAX_ITEM_COUNT_PER_REQUEST     ( 20 )  // change this based on SPSS and how much the player is supporting in real for per request
#define	 MAX_ITEM_NAME_LENGTH			( 20 )  // change this based on SPSS and how much the player is supporting in real for per request

class RemoteCDCacheBuilder
{
    public:
        // constructors
        RemoteCDCacheBuilder () ;
        RemoteCDCacheBuilder ( int maxItemsPerRequest, int maxItemNameLength ) ;
        
        // thread handler where the builder data requests are made to player to buildup the cache
        void    start ( void ) ;

        // cd status notified by RemoteCDManager
        void    onCDType   ( eCDType   type ) ;
        void    onCDStatus ( eCDStatus status ) ;

        // cache notifications to inform the result whether the record was cached or not
        void    onCacheUpdate ( Folder data, bool success ) ;
        
        // destructor
        virtual ~RemoteCDCacheBuilder () ;

    private:

        // max items that can be sent in one request for the player
        int maxItems_ ;
        // max length of the name a track or a folder can contain made configurable
        int maxItemNameLength_ ;
        // helpers
        void    init () ;
        void    processData ( Folder folder ) ;
    
    private:    // builder thread related

        // flag to stop the builder thread when data acquistion is complete or cacheBuilder is destructed
        bool done_ ;
        
        eCDType                     cdType_ ;

        // builder thread run on separate thread to perform data acquistion
        thread      builder_ ;

        // Holds the messages sent from ambTp in the queue and processes one by one in messagesWorker
        static queue < Folder >   events_ ;

        // getters
        queue < Folder >&  getEvents ()          {   return events_ ; }
        Folder             getEvent  ()          {   if ( ! events_. empty() )    return events_. front ( ) ; else    return nullptr ; }
        
        // setters
        static void      addEvent ( Folder data )        {   events_. push  ( data ) ;   }
        void             removeEvent ()                  {   events_. pop ( ) ;          }
    
        // mutex & conditional varibale for sync & race conditions handling on the events_ struct data
        static mutex                  sync_ ;
        static condition_variable     cv_ ;
        typedef unique_lock < mutex > ScopedLock ;
};
