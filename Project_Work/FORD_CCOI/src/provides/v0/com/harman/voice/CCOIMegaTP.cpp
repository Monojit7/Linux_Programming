#include "CCOIMegaTP.hpp"

// extern instance of timer
extern CCOITimer timer ;

namespace v0 {
    namespace com {
        namespace harman {
            namespace voice {

// timer instance for measuring consecutive data timeouts
CCOITimer::timer_id gMegaTPTimerId ;

// class members instantiation
condition_variable    CCOIMegaTP::cv_ ;
mutex                 CCOIMegaTP::sync_ ;
queue < tpData >      CCOIMegaTP::packages_ ;

// constructor
CCOIMegaTP::CCOIMegaTP ( ) : stopped_ ( false ), dataTimedOut_ ( false ), runningCounter_ ( 0 ), currentPackageNum_ ( 0 ), totalBytes_ ( 0 )  
{
    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d", __FUNCTION__, __LINE__ ) ;
    // CCOI related data like PTE, UFM etc,. comes in canId - 0x246, and Tcu ESN number data comes in 0x4A0
    canIds_ = { 0x246, 0x4A0 } ;

    // start packagesWorker to consume the queued packages if any. 
    packageWorker_ = thread ( bind ( &CCOIMegaTP::packagesWorker, this ) ) ;
}

// CCOIMegaTp thread to process the packages recieved from ambTp
void CCOIMegaTP::packagesWorker ( void )
{
	while ( ! stopped_ )        // until megaTp is not stopped work on the queue of packages
	{
        tpData data = nullptr ;

        ScopedLock lock ( sync_ ) ;
        if ( getPackages (). empty () )    cv_. wait ( lock ) ;   // wait forever until there is data to consume from ambTp 
       
        // data available now go get it and process
        data = getPackage () ;
        if ( data != nullptr )
        {
            // if our interested package has arrived read and store it
            if ( interestedPackage ( data ) )    readPackage ( data ) ;
            // else ask for retransmission of the packet
            else                                 sendReTransmit () ;

            CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d processed package with length %d firstByte 0x%x", __FUNCTION__, __LINE__, data-> size (), (*data) [ 0 ] ) ;
            // processed the item remove it from the queue
            removePackage () ;

        }
        else    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_DEBUG, "%s:%d No new Event to process\n", __FUNCTION__, __LINE__ ) ;
	}
}

// Reads the package and writes it into a file, if all data received sends the file to the client for further process
void CCOIMegaTP::readPackage ( tpData data )
{
    unsigned int start = 0 ;
   
    // destroy the timer for the packet arrived in time and update the timeout flag as well
    timer. destroy ( gMegaTPTimerId ) ; 
    dataTimedOut_ = false ;

    // read megaTp header into respective members of this megaTp instance
    getSignalId   ( data ) ;
    currentPackageNum_ = getPackageNum ( data ) ;
    
    // get where (file name) to write and from which point (offset) to read in the package data
    getFileName ( getSkipBytes ( data, start ) ) ; 
    FILE *file = fopen ( fileName_. c_str (), "ab+" ) ;
    if ( file )
    {
        fwrite ( &( (*data) [ start ] ), sizeof ( char ), ( data-> size () - start ), file ) ;
        fclose ( file ) ;
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d file %s open error", __FUNCTION__, __LINE__, fileName_. c_str () ) ;

    // if we got the number of bytes we needed send the response to the client
    if ( CCOIPersist::fileSize ( fileName_ ) == getTotalBytes ( data ) )    sendResponseToClient ( fileName_ ) ;
    // else wait for the data from ambTp
    else
    {
        startDataTimeOutTimer () ;
        CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d Still waiting for remaining bytes", __FUNCTION__, __LINE__ ) ;
    }
}

/*****   EXTERN or Client data update calls   *****/

// callback that is registered to ambtpclient and will copy the received packet into the packages queue 
void CCOIMegaTP::onDataReceived ( tpRequest_s* data )
{
    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d dataLen %d", __FUNCTION__, __LINE__, data-> dataLen ) ; 

	if ( (data-> dataLen != 0) && (data-> dataLen <= PACKAGE_SIZE ) )
    {
        // make a local copy of the ambTp data received
        tpData payload = make_shared < vector < uint8_t > > ( data-> dataLen ) ;
        payload-> assign ( data-> reqData, data-> reqData + data-> dataLen ) ;

        // add the copy of data to the packages queue for further processing of the same
        ScopedLock lock ( sync_ ) ;

        addPackage ( payload ) ;
        cv_. notify_one () ;    // notify packagesWorker_ thread to consume this data
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_ERROR, "%s:%d invalid dataLen from ambTp recieved %d requiredMaxSize %d", __FUNCTION__, __LINE__, data-> dataLen, PACKAGE_SIZE ) ;
}

// client (CCOIManager) calls this function to send response data to ambTp
void CCOIMegaTP::sendData(int numBytes, char* TPdata, CCOI_RX_PKT_TYPES type)
{
    tpData_s* data = ( tpData_s* ) malloc ( sizeof ( tpData_s ) ) ;
    if ( data != NULL )
    { 
        int numPaddingBytes = ( (type == MEGATP_PKG_RETRANSMISSION_TYPE)? 2 : 4 ) ;

        data-> payLoad = (uint8_t*) malloc ( numBytes + numPaddingBytes ) ;
        if ( data-> payLoad != NULL )
        {
            memset ( data-> payLoad, 0, numBytes ) ;

            data-> payLoad [ 0 ] = (uint8_t) type ;
            data-> payLoad [ 1 ] = 0x32 ;
            if ( numPaddingBytes > 2 )    data-> payLoad [ numPaddingBytes - 1 ] = 0x80;    // fillup the last byte as per Ids in the transport protocol doc.. 

            memcpy ( &data-> payLoad [ numPaddingBytes ], TPdata, numBytes - numPaddingBytes ) ;
        }
        data-> canRxId = RDISP_OPTIN_WORD_RX ;
        data-> dataLen = numBytes + numPaddingBytes ;
        data-> handle  = (void*) runningCounter_ ;

        // send to ambtp process to pass onto to TCU via CAN protocols 
        sendTPResponse ( data ) ;
        release ( data ) ;
        CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d dataLen %d bytes sent to ambTp", __FUNCTION__, __LINE__, data->dataLen ) ; 
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d malloc failure to get the buffer", __FUNCTION__, __LINE__ ) ; 
}

// sends retransmit request to ambTp
void CCOIMegaTP::sendReTransmit ( )
{
                   //                        signalId,              Include packageNum,     All packets,                         packageNum
    char data [] = {  static_cast < char > ( currentSignalId_ ),    0x01,                   0x01,         static_cast < char > ( currentPackageNum_ + 1 ) } ;
    
    sendData ( SIZEOF ( data ), data, MEGATP_PKG_RETRANSMISSION_TYPE ) ;
}

// sends data received from ambTp to the client CCOIManager
void CCOIMegaTP::sendResponseToClient ( string &value )
{
    char dataType = '\0' ;

    dataTimedOut_ = false ; // reset for the next packages
    
    // payload has the first byte as the signalId retrieve it and send to client so as to identify the data received
    ifstream sFile ( value. c_str(), ios::binary ) ;
    if ( sFile. is_open () )
    {
        sFile. read ( &dataType, 1 ) ;
        sFile. close () ;
    }
    
    // send the data to client CCOIManager to process it
    if ( pccoi )    pccoi-> onDataReceived ( true, dataType, value ) ;

    else    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d pccoi NULL check it", __FUNCTION__, __LINE__ ) ;
}

/*****   HELPERS/ Utility   *****/
int CCOIMegaTP::init ( void )
{
    // initialize the ambTp library
    int ret = initTpLib () ;
    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d Initialized ambTP library ret %d", __FUNCTION__, __LINE__, ret ) ; 

    // register the CanIds we are interested in along with callbacks for data receiving from ambTp
    for ( auto canId : canIds_ )
    {
        ret = registerTPMessage ( canId, &CCOIMegaTP::onDataReceived, &CCOIMegaTP::statusCallBack ) ;
        CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d registered canId 0x%x ret %d", __FUNCTION__, __LINE__, canId, ret ) ; 
    }
    return ret ;
}

// returns true if we are interested in the current package we received from ambTp available in @param data
bool CCOIMegaTP::interestedPackage ( tpData data )
{
    // we are not interested in data timed out packets
    bool interested = ( dataTimedOut_ == false ) ;

    // we are interested in everything else except for an out of order consecutive package
    if ( (*data) [ 0 ] == CONSECUTIVE_PACKAGE_TAG_ID )    interested &= ( getPackageNum ( data ) == currentPackageNum_ + 1 ) ;

    CcoiLog.write ( CCOILog::eLogCCOIManager, CCOILog::LOG_INFO, "%s:%d firstByte 0x%x timedOut %d currentPackageNum_ %d interested %d", __FUNCTION__, __LINE__, (*data) [ 0 ], dataTimedOut_, currentPackageNum_, interested ) ;
    return interested ;
}

/** updates the number of bytes to skip these bytes has megaTp header information and hence should be considered for payload copy for the client
*   @param [IN]  - data      : package received form ambTp
*   @param [OUT] - skipBytes : the number of bytes to skip for this package
*   returns true if the data has to be written to a new file or the existing file to be appended in case of a consexutive package received
*/
bool CCOIMegaTP::getSkipBytes ( tpData data, unsigned int &skipBytes )
{
    bool append = false ;
    skipBytes = 0 ;     // for normalTp data there won't be any megaTp header so we should not skip anything
    
    if      ( (*data) [ 0 ] == FIRST_PACKAGE_TAG_ID )          skipBytes = 6 ;    // for first package first 6 contains, PackageId, TotalPackages, TotalLength of the data
    else if ( (*data) [ 0 ] == CONSECUTIVE_PACKAGE_TAG_ID )
    {
        skipBytes = 4 ; // for consecutive package first 4 bytes has megaTP header, PackageId, PackageNum, SignalId
        append = true ;
    }

    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d firstByte 0x%x skipBytes %d", __FUNCTION__, __LINE__, (*data) [ 0 ], skipBytes ) ;
    return append ;
}

// gets the package number of the package data
short CCOIMegaTP::getPackageNum ( tpData data )
{
    short retVal = 0 ; 

    // PackageNumber comes in byte 1 & 2 offsets
    if ( (*data) [ 0 ] == CONSECUTIVE_PACKAGE_TAG_ID )    retVal = ( ( (*data) [ 1 ] << 0x08 ) | (*data) [ 2 ] ) ;
    
    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d data [0] 0x%x retVal %d", __FUNCTION__, __LINE__, (*data) [ 0 ], retVal ) ;
    return retVal ;
}

// updates fileName_ member to new file if we have to append to add additional packages, else creates a new file name
void CCOIMegaTP::getFileName ( bool append )
{
    if ( ! append )
    {
        fileName_ = "/tmp/megaTP" ;
        fileName_. append ( to_string ( ++runningCounter_ ) ) ;
    }

    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d fileName %s", __FUNCTION__, __LINE__, fileName_. c_str () ) ;
}

// gives the totalBytes for megaTp package it comes in First Package, for normal packages it is same as recieved from ambTp 
unsigned int CCOIMegaTP::getTotalBytes ( tpData data )
{
    // megaTp packets the total payload bytes come in byte - 3, 4, 5 offset of the first package 
    if      ( (*data) [ 0 ] == FIRST_PACKAGE_TAG_ID )          totalBytes_ = ( ( (*data) [ 3 ] << 8 ) | ( (*data) [ 4 ] << 8 ) | (*data) [ 5 ] )  ;
    
    // send the totalbytes same as we copy so as the response is sent to client
    else if ( (*data) [ 0 ] != CONSECUTIVE_PACKAGE_TAG_ID )    totalBytes_ = data-> size () ;
    
    // else for consecutive packages do not update the totalBytes as it would be preserved when we recieve the first package

    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d firstByte 0x%x totalBytes_ %d ", __FUNCTION__, __LINE__, (*data) [ 0 ], totalBytes_ ) ;
    return totalBytes_ ;
}

void CCOIMegaTP::packageMissed ()
{
	dataTimedOut_ = true ;
	CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d dataTimedOut_ %d", __FUNCTION__, __LINE__, dataTimedOut_ ) ; 
}

void CCOIMegaTP::startDataTimeOutTimer ()
{
	CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d Waiting for Packet %d", __FUNCTION__, __LINE__, currentPackageNum_ + 1 ) ; 
	gMegaTPTimerId = timer. create ( PACKAGE_TIME_OUT, 0, std::bind ( &CCOIMegaTP::packageMissed, this ) ) ;
}

void CCOIMegaTP::release ( tpData_s* &megaTpData )
{
    if ( megaTpData-> payLoad != NULL )    free ( megaTpData-> payLoad ) ;
    free ( megaTpData ) ;
}

void CCOIMegaTP::statusCallBack ( tpTxStatus_s* tmp )   {   }

// destructor
CCOIMegaTP::~CCOIMegaTP()
{
    CcoiLog.write ( CCOILog::eLogCCOIMegaTP, CCOILog::LOG_INFO, "%s:%d", __FUNCTION__, __LINE__ ) ;
    stopped_ = true ;
    packageWorker_. join () ;
}

            } // namespace voice
        } // namespace harman
    } // namespace com
} // namespace v0

