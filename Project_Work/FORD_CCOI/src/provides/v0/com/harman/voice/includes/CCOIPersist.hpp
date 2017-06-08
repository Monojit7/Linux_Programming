#ifndef CCOIPERSIST_HPP
#define CCOIPERSIST_HPP

#include <unistd.h> 
#include <queue>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "EntityElement.hpp"
#include "CCOIManager.hpp"
#include "Persistpath.hpp"
#include "zlibWrapper.h"
#include "Sha256.h"
#include "CCOILog.hpp"

// namespace declaratives
using namespace std ;

// macro defintions
#define HASH_START_POSITION	            ( 16 )  // Hash value start from Byte 17 onwards.
#define HASH_LENGTH		                ( 32 )  // 32 bytes of hash value - SHA256.
#define HEADER_BYTES                    ( 52 )  // 52 bytes.
#define HEADER_METADATA_POSITION        (  2 )  // the data we are interested in starts from 3rd byte onwards, while first byte - signal identifier, 2nd byte - character coding type.
#define HEADER_METADATA_SIZE            ( 14 )  // includes data like platform versions & timestamp of the file the data was updated, contains no actual data or hash value etc,.
#define SYNC_METADATA_POSITION          (  4 )  // the synchronizationSettings data received from Tcu has the meta data position starting from 5th byte onwards.
#define SYNC_DATA_POSITION              ( 32 )  // the synchronizationSettings data received from Tcu has the data starting after 32 bytes onwards.
#define PAYLOAD_SIZE_POSITION(pos)      ( (pos == 48) || (pos == 49) || (pos == 50) )  // the payload size position in any header is at 48th byte, 49th byte & 50th byte together.

namespace v0 {
    namespace com {
        namespace harman {
            namespace voice {
    
class CCOIPersist
{
    private:
        
        EntityList entities_ ;

        bool    compareHeader ( int type ) ;
        bool    persistHeader ( int type ) ;
        bool    splitPayload ( int type, string &name ) ;
        bool    persistEntities ( ) ;
        bool    isValidSyncSettingsData ( string &value ) ;
        bool    splitHeader ( int type, string &name, bool &validPayloadSize ) ;
        bool    compareHash ( const string &headerFile, const string &msgFile ) ;
        void    processSyncEntityBitVector ( unsigned short entityBitVector ) ;

    public:

        // constructors
        CCOIPersist ( ) ;
        
        bool    persistSettingsData ( string &value ) ;
        bool    persistFiles ( int type, string name ) ;
        
        // used by other to print the entities
        static void printEntities ( EntityList &entities ) ;
        static size_t fileSize ( string &name ) ;
        // destructor
        virtual ~CCOIPersist ( ) ;
};
            } // voice
        } // harman
    } // com
} // v0

#endif // CCOIPERSIST_HPP
