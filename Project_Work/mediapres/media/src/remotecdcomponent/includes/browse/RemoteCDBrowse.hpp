#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <RemoteCDCore.hpp>
#include <RemoteCDManager.hpp>
#include <RemoteCDCache.hpp>
#include <RemoteCDEvents.hpp>
#include <RemoteCDTypes.hpp>
#include <RemoteCDLog.hpp>

// namespace declaratives
using namespace std ;

    // forward declaration
    class RemoteCDCache ;

    class RemoteCDBrowse
    {
        public:
            // constructor
            RemoteCDBrowse () ;
            
            // clients calls for browse
            virtual void    getBrowseInfo ( FolderRequest request ) ;
            virtual void    getFolderName ( short folderNumber, int nameLength, FolderRequestedData::eCutNameType cutName ) ;
            
            // cache notificatoin upon cache update
            void    onCacheUpdate ( Folder folder, bool success ) ;

            // destructor
            virtual ~RemoteCDBrowse () ;

        private:
            // requests from client
            FolderRequestList   requestList_ ;

            shared_ptr < RemoteCDCache >            cache_ ;

            // helpers to form the data client requires from the cache data we received form cache component upon our request
            bool    processRequest ( FolderRequest request ) ;
            Folder  applyRequestedRules ( FolderRequest request, Folder folder ) ;
            string  getInterestedName ( string actualName, FolderRequestedData::eCutNameType cutType, short requestedLength ) ;

    };
