#ifndef CCOI_MEGA_TP_H
#define CCOI_MEGA_TP_H

#include <vector>
#include <fstream>
#include <queue>
#include <AmbTPClientLib.h>	// has the calls to send the TP data or receive data from TCU/ platform
#include "CCOIManager.hpp"
#include "CCOILog.hpp"

#define PACKAGE_SIZE                    ( 4095 )    // ambTp sends 4095 bytes per package as per the megaTp standards
#define PACKAGE_TIME_OUT                ( 1000 )    // 1 sec timeout to receive the data from ambTp
#define FIRST_PACKAGE_TAG_ID            ( 0xFF )
#define CONSECUTIVE_PACKAGE_TAG_ID      ( 0xCF )
#define RDISP_OPTIN_WORD_RX             ( 0x246 )
#define SIZEOF( a )                     ( sizeof (a) / sizeof ( a [ 0 ] ) )

namespace v0 {
    namespace com {
        namespace harman {
            namespace voice {

// signalIds for various data sent to ambTp, Tcu uses this to different the responses from CCOI
typedef enum _CCOI_RX_PKT_TYPES_
{
    CCOI_SYNCHRONIZATION_SESSION_RQ_TYPE     = 0x9C,
    CCOI_SYNCHRONIZATION_SUMMARY_REPORT_TYPE = 0x9F,
    CCOI_SETTINGS_UPDATE_RQ_TYPE             = 0xA0,
    CCOI_USERPROMPT_RESPONSE_TYPE            = 0xA6,
    MEGATP_PKG_RETRANSMISSION_TYPE           = 0xAB
}CCOI_RX_PKT_TYPES;

// forward declaration
class CCOIManager ;

// data structure defined to hold the payload raw bytes that is received from platform
typedef shared_ptr < vector < uint8_t > >   tpData ;

class CCOIMegaTP
{
    public:
        
        // constructor
        CCOIMegaTP ( ) ;

        // initializes the ambTpLibrary
        int     init ( ) ;

        // client (CCOIManager) calls this to transmit data to ambTp
        void    sendData ( int NumberOfBytes, char* TPdata, CCOI_RX_PKT_TYPES type ) ;
        
        // holds client instance to send the data (sent by Tcu) received from ambTp
        CCOIManager *pccoi ;

        // destructor
        ~CCOIMegaTP ( ) ;

    private:
        
        // packages worker runs on separate thread to process/ read packages and send to client
        thread    packageWorker_ ;

        // Holds the packages sent from ambTp in the queue and processes one by one in packagesWorker
        static queue < tpData >   packages_ ;

        // getters
        queue < tpData >&  getPackages ()          {   return packages_ ; }
        tpData             getPackage  ()          {   if ( ! packages_. empty() )    return packages_. front ( ) ; else    return nullptr ; }
        
        // setters
        static void      addPackage ( tpData data )        {   packages_. push  ( data ) ;   }
        void             removePackage ()                  {   packages_. pop ( ) ;          }
        
        // mutex & conditional varibale for sync & race conditions handling on the packages_ struct data
        static mutex     sync_ ;
        static condition_variable            cv_ ;
        typedef unique_lock < mutex > ScopedLock ;

        // exit flag to indicate the megatp running status
        bool            stopped_ ;
        // flag to indicate the data timeout in receiving the consecutive package form ambTp
        bool            dataTimedOut_ ;
        // file in which the ambTp data is written into
        string          fileName_ ;
        // maintains the unique file name for the new data package
        unsigned int    runningCounter_ ;
        // contains the signalId this is needed for retransmit request to ambTp
        short int       currentSignalId_ ;
        // current package received this is needed for retransmit request to ambTp
        short int       currentPackageNum_ ;
        // holds the total Bytes of a single entity - signalId
        unsigned int    totalBytes_ ;

        // gets the signalId from the first package
        inline void getSignalId ( tpData data )           { if ( (*data) [ 0 ] == FIRST_PACKAGE_TAG_ID )    currentSignalId_ = (*data) [ 6 ] ; } // signalId is transmitted in the 6th byte offset of the first package 

        // CanIds we register to ambTp to receive the data transmitted with these ids
        vector < unsigned short >    canIds_ ;

        // utility see comments in implementation to know what they do respectively
        void            packageMissed ( ) ;
        void            sendReTransmit ( ) ;
        void            readPackage ( tpData data ) ;
        void            getFileName ( bool append ) ;
        unsigned int    getTotalBytes ( tpData data ) ;
        short           getPackageNum ( tpData data ) ;
        bool            interestedPackage ( tpData data ) ;
        void            sendResponseToClient ( string &value ) ;
        bool            getSkipBytes ( tpData data, unsigned int &skipBytes ) ;
        
        // timeout handler this function gets called if the packages does not arrive in time
        void    startDataTimeOutTimer ( ) ;

        // sends data written in fileName_ to the client (CCOIManager)
        void    sendResponseToClient ( ) ;

        // releases the memory held in data used to send data to ambTp
        void release ( tpData_s* &data ) ;
    
    public: 
        
        // runs on separate thread to consume the data written to packages_ queue upon ambTp data callback
        void    packagesWorker ( void ) ;

        // callbacks registered with ambTp to receive the data and status respectively
        static void    onDataReceived ( tpRequest_s* data ) ;
        static void    statusCallBack ( tpTxStatus_s* tmp ) ;
};

            } // namespace voice
        } // namespace harman
    } // namespace com
} // namespace v0

#endif // CCOI_MEGA_TP_H
