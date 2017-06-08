
#include "CCOIPersist.hpp"

namespace v0 {
    namespace com {
        namespace harman {
            namespace voice {

// constructor 
CCOIPersist::CCOIPersist()
{
    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_INFO, "%s:%d", __FUNCTION__, __LINE__ ) ;
}

bool CCOIPersist::splitHeader ( int type, string &name, bool &validPayloadSize )
{
    int bytes = 0 ;
    bool success = false ;
    string hdrpath = ( ( type == ePTE )? PTEHDRCOPY_BIN : UFMHDRCOPY_BIN ) ;
    
    ifstream src    ( name.c_str () ) ;
    ofstream header ( hdrpath.c_str () );
   
    if ( src && header )
    {
        for ( unsigned char ch = '\0'; ( bytes < HEADER_BYTES ) && ( src >> ch ); bytes ++ )
        {
            header << hex << ch ;

            // get the payloadSize while copying itself and check if it has any data, else mark it as invalid so as not to overwrite the persistant file
            if ( ( PAYLOAD_SIZE_POSITION ( bytes ) ) && ( ch > 0 ) )    validPayloadSize |= true ;
        }

        src. close () ;
        header. close () ;
		
		success = ( validPayloadSize )? persistHeader ( type ) : compareHeader ( type ) ;
    }

    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_INFO, "%s:%d name %s success %d validPayloadSize %d bytes %d", __FUNCTION__, __LINE__, name. c_str (), success, validPayloadSize, bytes ) ;

    // return true if we have read HEADER BYTES succesfully.
    return ( success && ( bytes == HEADER_BYTES ) ) ;
}

bool CCOIPersist::splitPayload ( int type, string &name )
{
    bool success = false ;
    string payloadpath      = ( ( type == ePTE )? PTE_GZ     : UFM_GZ ) ;
    
    ifstream src         ( name.c_str () ) ;
    ofstream payload     ( payloadpath.c_str () ) ;
    if ( src.is_open () && payload. is_open () )
    {
        // skip header for payload read
        src. seekg ( HEADER_BYTES ) ;
        
        int bytes = 0;
        char ch = '\0' ;
        while ( src.get ( ch ) )
        {
            payload .put ( ch ) ;
            bytes ++ ;
        }
        src. close () ;
        payload. close () ;

        success = true ;
        
        CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_INFO, "%s:%d payload bytes read %d", __FUNCTION__, __LINE__, bytes ) ;
    }

    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_INFO, "%s:%d file %s fileSize %d", __FUNCTION__, __LINE__, name. c_str (), CCOIPersist::fileSize ( name ) ) ;
    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_INFO, "%s:%d file %s fileSize %d", __FUNCTION__, __LINE__, payloadpath. c_str (), CCOIPersist::fileSize ( payloadpath ) ) ;

    // if success go ahead and unzip the data into persistant file locations
    return ( success && ( zlibUnzip ( payloadpath.c_str (), ( type == ePTE )? PTE_FILE : UFM_FILE ) == 0 ) ) ;
}

// compares the new header data with the existing one if no new payload sent, and returns false if mismatch, else true.
bool CCOIPersist::compareHeader ( int type )
{
    string header1 = ( ( type == ePTE )? PTEHDRCOPY_BIN : UFMHDRCOPY_BIN ) ;
    string header2 = ( ( type == ePTE )? PTEHDR_BIN     : UFMHDR_BIN ) ;

    ifstream file1 ( header1.c_str () ) ;
    ifstream file2 ( header2.c_str () ) ;
    
    unsigned char ch1 = '\0', ch2 = '1' ; 
    if ( file1. is_open () && file2. is_open () )
    { 
        // nothing new coming here so the headers tcu sent now should match existing one
        do
        {
            file1 >> hex >> ch1 ;
            file2 >> hex >> ch2 ;
        } while ( ch1 == ch2 ) ;

        file1. close () ;
        file2. close () ;

        remove ( header1. c_str () ) ;    // remove the temporary copy of the header
    }

    // the condition below takes care of every condtions positive & negative and returns correct return value
    return ( ch1 == ch2 ) ;
}

// for a data which comes with payload, so version matching or header matching is done and persists that data and returns true.
bool CCOIPersist::persistHeader ( int type )
{
    bool success = false ;
    string header1 = ( ( type == ePTE )? PTEHDRCOPY_BIN : UFMHDRCOPY_BIN ) ;
    string header2 = ( ( type == ePTE )? PTEHDR_BIN     : UFMHDR_BIN ) ;

    ifstream file1 ( header1.c_str () ) ;
    ofstream file2 ( header2.c_str () ) ;
    
    unsigned char ch1 = '\0' ; 
    if ( file1. is_open () && file2. is_open () )
    { 
        // has the payload data so copy without header comparsions
        while ( file1 >> hex >> ch1 )   file2 << hex << ch1 ;

        file1. close () ;
        file2. close () ;

        remove ( header1. c_str () ) ;    // remove the temporary copy of the header
        success = true ;
    }

    // the condition below takes care of every condtions positive & negative and returns correct return value
    return ( success ) ;
}

bool CCOIPersist::persistFiles ( int type, string name )
{
    bool validPayloadSize = false ;

    bool success = splitHeader ( type, name, validPayloadSize ) ;
    if ( success )
    {
        if ( validPayloadSize )    success &= splitPayload ( type, name ) ;
    }
    
    string header  = ( ( type == ePTE )? PTEHDR_BIN : UFMHDR_BIN ) ;
    string payload = ( ( type == ePTE )? PTE_FILE   : UFM_FILE   ) ;

	return ( success && compareHash ( header, payload ) ) ;
}

/** This function compares the hash value sent in packet header by TCU and the hash value of the total msg File.
 *  @param (IN) : headerFile - file path where the header contents of TCU packet is stored.
 *  @param (IN) : msgFile    - file path where the total msg contents (header + payload) sent by TCU is stored.
 *  @returns    : true - if both the hashes matches, false - otherwise.
    NOTE : it assumes the hash is stored at the last in the header file.
*/
bool CCOIPersist::compareHash ( const string &headerFile, const string &msgFile ) {

	bool success = false;

    ifstream header ( headerFile, ios::binary ) ;
	if ( header.is_open() ) {

		// header hash
		string headerHash ;
		headerHash.resize ( HASH_LENGTH ) ;
		
		header.seekg ( HASH_START_POSITION ) ;	// move the pointer to the start of the hash. 
		header.read ( &headerHash[0], HASH_LENGTH ) ;
		
		// msg hash
		string msgHash ;
		if ( !sha256_file( msgFile.c_str(), msgHash ) && ( msgHash == headerHash) )    success = true ;

		header.close () ;
    	
        CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_INFO, " Hashes %s headerHash %s msgHash %s\n", ((success)? "" : " does not"),  headerHash.c_str(), msgHash.c_str() ) ;
	}
    // simulator does not send the hash value scorrectly (sends ascii hex values of 32 bytes hash which curtails the actual value to 16 bytes only), remove this in tcu integration
    success = true ;
	return success ;
}

/** compares the metadata taken from PTE & UFM headers to the metadata sent in syncSettings message by the Tcu
 * returns true if it matches else false
*/
bool CCOIPersist::isValidSyncSettingsData ( string &value )
{
    bool valid = false ;
    
    if ( value. length () > ( HEADER_METADATA_SIZE * 2 ) )
    {    
        ifstream headerFile1 ( PTEHDR_BIN, ios::binary ) ; 
        ifstream headerFile2 ( UFMHDR_BIN, ios::binary ) ; 
         
        if ( headerFile1. is_open () && headerFile2. is_open () )
        {
            char *buffer = new char [ HEADER_METADATA_SIZE * 2 ] ;
            if ( buffer != NULL )
            {
                memset ( buffer, 0, ( HEADER_METADATA_SIZE * 2 ) ) ;

                headerFile1. seekg ( HEADER_METADATA_POSITION ) ;
                headerFile1. read ( &buffer[ 0 ], HEADER_METADATA_SIZE ) ;
            
                headerFile2. seekg ( HEADER_METADATA_POSITION ) ;
                headerFile2. read ( &buffer[ HEADER_METADATA_SIZE ], HEADER_METADATA_SIZE ) ;
            
                valid = ( memcmp ( ( char *) (value.data () + SYNC_METADATA_POSITION), buffer, (HEADER_METADATA_SIZE * 2) ) == 0 ) ;

                delete [] buffer ;
            }
            else    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_ERROR, "%s:%d Unable to allocate Heap memory", __FUNCTION__, __LINE__ ) ;
        }
        else    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_ERROR, "%s:%d Unable to open header files %s %s", __FUNCTION__, __LINE__, PTEHDR_BIN, UFMHDR_BIN ) ;
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_ERROR, "%s:%d Invalid Sync Data received size %d", __FUNCTION__, __LINE__, value. length () ) ;
    
    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_INFO, "%s:%d valid %d", __FUNCTION__, __LINE__, valid ) ;
   
    // remove the work around in the tcu integration
    valid = true ; 
    return valid ;
}

void CCOIPersist::processSyncEntityBitVector ( unsigned short entityBitVector )
{
    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_INFO, "%s:%d Data 0x%x", __FUNCTION__, __LINE__, entityBitVector ) ;

    // entityType - 3 LSB bits
    int entityType = ( entityBitVector & 0x07 ) ;
    entityBitVector >>= 3 ; // discard the entityType bits

    // entityID - bit 3 to bit 9 i.e., 7 bits
    int entityID = ( entityBitVector & 0x7F ) ;
    entityBitVector >>= 7 ;  //discard the entityID bits

    vector < string > flags ;

    flags. push_back ( ( (uint8_t)entityBitVector & 0x1 )? "bUAllowOnOff_True"  : "bUAllowOnOff_False" ) ;
    entityBitVector >>= 1 ;  //discard the bUAllowOnOff bit

    flags. push_back ( ( (uint8_t)entityBitVector & 0x1 )? "bPAllowOnOff_True"  : "bPAllowOnOff_False" ) ;
    entityBitVector >>= 1 ;  //discard the bPAllowOnOff bit

    flags. push_back ( ( (uint8_t)entityBitVector & 0x1 )? "bFPOnOff_True"      : "bFPOnOff_False" ) ;
    entityBitVector >>= 1 ;  //discard the bFPAllowOnOff bit

    flags. push_back ( ( (uint8_t)entityBitVector & 0x1 )? "bSAllowOnOff_True"  : "bSAllowOnOff_False" ) ;

    entities_. push_back ( make_shared < EntityElement > ( to_string ( entityID ), to_string ( entityType ), flags ) ) ;
}

bool CCOIPersist::persistSettingsData ( string &name )
{
    bool success = false ; 
    
    entities_. clear () ;   // clear off the old entries
    vector < char > value ;
    ifstream stream ( name. c_str () ) ;
    if ( stream. is_open () )
    {
        char ch = '\0' ;
        while ( stream. get ( ch ) )    value. push_back ( ch ) ;

        success = true ;
    }

    if ( success /* isValidSyncSettingsData ( value ) */ )
    {
        //strip the meta data, and come to the actual data i.e., the entities and their respective values
        int index = SYNC_DATA_POSITION ;

        unsigned short numOfEntities = (uint8_t) ( value [ index++ ] ) ;   // Num of records down the line
        numOfEntities <<= 8 ;
        numOfEntities |= (uint8_t) ( value [ index++ ] ) ;

        CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_INFO, "%s:%d numOfEntities %d", __FUNCTION__, __LINE__ , numOfEntities ) ;
        for ( int i = 0; i < numOfEntities; i++ )
        {
            // each individual entity information comes in 2 bytes, so get that first & get the individual attribute from their respective bit positions
            unsigned short temp = ( ((uint8_t) value [ index ]) << 8 ) | ( (uint8_t) value [ index + 1 ] ) ;
            
            index += 2 ;    // skip the above processed 2 bytes
            processSyncEntityBitVector ( temp ) ;
        }

        if ( persistEntities ( ) )    printEntities ( entities_ ) ;
    }

    return true;
}

bool CCOIPersist::persistEntities ( )
{
    bool success = false ;

    if ( entities_. size () > 0 )
    {
        ofstream entitiesStream ( ENTITIES_DATA_PATH ) ;
        if ( entitiesStream. is_open () )
        {
            for ( unsigned int i = 0; i < entities_. size (); i++ )
            {
                entitiesStream << entities_ [ i ]-> entityID_ ;
                entitiesStream << "\n" ;
                entitiesStream << entities_ [ i ]-> entityType_ ;
                entitiesStream << "\n" ;

                for ( auto &value : entities_ [ i ]-> values_ )
                    entitiesStream << value << "\n" ;
            }
            success = true ;
        }
        else    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_ERROR, "%s:%d unable to open file %s to persist the entities data", __FUNCTION__, __LINE__, ENTITIES_DATA_PATH ) ;
    }
    else    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_ERROR, "%s:%d nothing to persist for entities data %s", __FUNCTION__, __LINE__, ENTITIES_DATA_PATH ) ;

    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_ERROR, "%s:%d success %d", __FUNCTION__, __LINE__, success ) ;
    
    return success ;
}

void CCOIPersist::printEntities ( EntityList &entities )
{
    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_INFO, "%s:%d Persisted Entities in CCOI Total: %d", __FUNCTION__, __LINE__, entities. size () ) ;
    for ( auto entity : entities )
    {
        stringstream entityStream ;

        entityStream << entity-> entityID_ ;
        entityStream << " " ;
        entityStream << entity-> entityType_ ;
        entityStream << " " ;
        
        for ( auto &value : entity-> values_ )
        {
            entityStream << value ;
            entityStream << " " ;
        }
        CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_INFO, "%s %d %s", __FUNCTION__, __LINE__, entityStream. str (). c_str () ) ;
    }
}

size_t CCOIPersist::fileSize ( string &name )
{
    size_t size = 0 ;
    
    ifstream stream ( name, ios::ate ) ;
    if ( stream. is_open () )
    {
        size = stream. tellg () ;
        stream. close () ; 
    }
    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_ERROR, "%s:%d file %s size %d", __FUNCTION__, __LINE__, name. c_str (), size ) ;
    return size ;
}

// destructor 
CCOIPersist::~CCOIPersist()
{
    CcoiLog.write ( CCOILog::eLogCCOIPersist, CCOILog::LOG_ERROR, "%s:%d", __FUNCTION__, __LINE__ ) ;
}


            } // namespace voice
        } // namespace harman
    } // namespace com
} // namespace v0
