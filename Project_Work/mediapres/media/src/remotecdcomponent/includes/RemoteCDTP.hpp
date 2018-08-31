#pragma once    // to include the below piece of code only once during pre-compilation

#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <memory>
#include <mutex>
#include <AmbTPClientLib.h>     // ambTp header has Tp related functions & data structures used to receive/ transfer data to the CD player via amb

#include <RemoteCDCore.hpp>
#include <RemoteCDEvents.hpp>
#include <RemoteCDTypes.hpp>
#include <RemoteCDLog.hpp>

#define CD_UTILIZATION               ( 0x11 )  // TP responses are sent with 0x11 value for CD specifics see transport protocol document
#define CD_RDISP_WORD_RX             ( 0x246 ) // CanId on which our data sent to amb will go - just for reference we do not use this directly neither sendData to amb as no requirements as such now
#define CD_RDISP_WORD_TX             ( 0x2A6 ) // CanId on which amb will send the CD specific Tp data
#define MESSAGE_SIZE                 ( 4095 )  // ambTp sends 4095 bytes per package as per the megaTp standards
#define NUM_ITEM_TYPES_SUPPORTED     ( 3 )     // currently we have only 3 types of contents - file, playlist, folder
#define HEADER_BYTES                 ( 4 )     // signalId, utilization, command status & coding type are ignored for data read
#define TOC_RECORD_SIZE              ( 4 )     // each TOC record is of 4 bytes, 1st byte for index of the item, rest 3 bytes indicates the item
#define FOLDER_PATH_RECORD_SIZE      ( 4 )     // each record for folder path is of 4 bytes, first 2 bytes for folder number, second 2 bytes for itemIndex
#define FOLDER_SUMMARY_RECORD_SIZE   ( 3 )     // each record for folder summary is of 3 bytes, first 1 byte for type of content - file, playlist, folder, second 2 bytes for numItems present in that type
#define ACTIVE_FOLDER_PAYLOAD_OFFSET ( 3 )     // skip signalId, utilization, command status fields, this message does not have coding table type so payload starts from 4th byte itself
#define FOLDER_SUMMARY_SIZE          ( NUM_ITEM_TYPES_SUPPORTED * FOLDER_SUMMARY_RECORD_SIZE )

    // data structure defined to hold the payload raw bytes that is received from platform
    
struct TPData
{
   uint8_t* tpdata;
   uint16_t size;
   
};
    
typedef    TPData*    tpData ;
    
    // handler function
    typedef function < bool ( tpData ) > handlerType ;
    class RemoteCDTP
    {
        public:
            // constructor
            RemoteCDTP () ;

            // destructor
            virtual ~RemoteCDTP () ;

        private:
            // bool variable to exit the messageThread handler
            bool stopped_ ; 
            // messages worker runs on separate thread to process/ read messages and send to client
            thread    messageWorker_ ;

            // Holds the messages sent from ambTp in the queue and processes one by one in messagesWorker
            static queue < tpData >   messages_ ;

            // getters
            queue < tpData >&  getMessages ()          {   return messages_ ; }
            tpData             getMessage  ()          {   if ( ! messages_. empty() )    return messages_. front ( ) ; else    return nullptr ; }
            
            // setters
            static void      addMessage ( tpData data )        {   messages_. push  ( data ) ;   }
            void             removeMessage ()                  {   messages_. pop ( ) ;          }
        
            // mutex & conditional varibale for sync & race conditions handling on the messages_ struct data
            static mutex                  sync_ ;
            static condition_variable     cv_ ;
            typedef unique_lock < mutex > ScopedLock ;

            // CanIds we register to ambTp to receive the data transmitted by CD player with these ids
            vector < unsigned short >    canIds_ ;
            
            // map which holds the messageId and its corresponding handler function
            map < int, handlerType >    messageIdHandlerMap_ ;
            map < int, eNowPlayingNameType >    namesMessageIdsMap_ ;

        public: 
        
            // runs on separate thread to consume the data written to messages_ queue upon ambTp data callback
            void    messagesWorker ( void ) ;

            // callbacks registered with ambTp to receive the data and status respectively
            static void    onDataReceived ( tpRequest_s* data ) ;
            static void    statusCallBack ( tpTxStatus_s* tmp ) ;

        private:
            // helpers
            int     init ( void ) ;
            // inits the map of messageId with their corresponding function which has the implementation of data handling
            void    initDataHandlers () ;
            // to check if we are interested in the message received from plf
            bool    interestedMessage ( tpData data ) ;

            // data handlers
            bool    nameReceived ( tpData data ) ;
            bool    MPInfoReceived ( tpData data ) ;
            bool    folderMetaDataReceived ( tpData data ) ;
            bool    activeFolderInfoReceived ( tpData data ) ;
            bool    CDTableOfContentsReceived ( tpData data ) ;
            
            // helpers functions breakdown to parse the complex data and populate our data structures and publish to the clients who are interested in our data
            string  getName ( int &offset, char codingType, tpData data ) ;
            bool    getFolderPath ( unsigned int offset, tpData data, FolderPath &path ) ;
            bool    getFolderSummary ( unsigned int &offset, tpData data, FolderSummary &summary ) ;
            bool    getFolderItemsData ( int &offset, char codingType, tpData data, Folder contents, int itemsCount ) ;
    };
