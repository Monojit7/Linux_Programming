#pragma once

#include <vector>
#include <string>
#include <map>
#include <RemoteCDCore.hpp>
#include <RemoteCDItem.hpp>
#include <RemoteCDFolder.hpp>
#include <RemoteCDCacheBuilder.hpp>
#include <RemoteCDLog.hpp>

// namespace declaratives
using namespace std ;
using namespace std::placeholders ;

#define MAX_FOLDERS_IN_CACHE    ( 500 ) // change this value based on the performance on memory side & execution flow cost

typedef enum eCacheType
{
    eCDTableData = 0x1,
    eMP3Data 	 = 0x2		
}eCacheType;

// forward declaration
class RemoteCDCacheBuilder ;

class RemoteCDCache
{
    public:
        // constructors
        RemoteCDCache () ;
        RemoteCDCache ( bool cacheBuilderEnable ) ;
        
        // cd status notified by RemoteCDManager
        void    onCDStatus ( eCDStatus status ) ;
        void    onCDError ( CDError   error  ) ;
        void    onCDType ( eCDType cdType ) ;

        // notificatons from TP component upon parsing the data asks it to cache
        void    onTOCDataUpdate ( CDTableofContents &contents ) ;
        void    onFolderDataUpdate ( Folder folder , bool valid ) ;
        
        // clients calls
        
        // to check if the record is available in cache or not
        bool    isAvailable ( short folderNumber ) ;
        bool    isAvailable ( short folderNumber, int start, int numItems ) ;
        
        // to get the data corresponding to the param client is interested in, isAvailable to be called first as otherwise unnecessary element would be added in the cache
        Folder  getData ( short folderNumber )          {  return folders_ [ folderNumber ] ;  }
        bool    getData ( CDTableofContents& contents ) ;
        
        // clears the cache contents clients can use this maybe due to CD disc eject
        void    clear ( eCacheType type ) ;

        // destructor
        virtual ~RemoteCDCache () ;

    private:

        // cache is made configurable so as depending on the performance can be switched on/ off easily
        bool builderEnabled_ ;
        
        // cache builder instance
        shared_ptr < RemoteCDCacheBuilder >     cacheBuilder_ ; 

        // data structure where we maintain the cache for faster access of the data received from player
        map < short, Folder >       folders_ ;
        
        // TOC data cached
        CDTableofContents           CDTable_ ;
        
        eCDType                     cdType_ ;

        // helpers
        void    init () ;
        // updates the folder present in the cache with new contents passed as parameter
        bool    updateFolder ( Folder folder ) ;
        void    populateCDDACache () ;
};
