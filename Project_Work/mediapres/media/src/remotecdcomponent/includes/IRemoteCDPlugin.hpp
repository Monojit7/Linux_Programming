#pragma once

#include <string>
#include <core/IPluginBase.h>
#include "RemoteCDTypes.hpp"
#include "RemoteCDEvents.hpp"

// namespace declaratives
using namespace std ;

// This class is a wrapper of IPluginBase interface of prescore which provides the hooks to init/ deInit our plugin
    class IRemoteCDPlugin : public IPluginBase
    {
        public:
            // constructor
            IRemoteCDPlugin ( string name ) : IPluginBase ( move ( name ) )    { }
            
            // play controls related            
            virtual void setAction ( eCDPlayActions action ) = 0 ;
            virtual void setContext ( CDPlayContext context ) = 0 ;
            virtual void setPlayItem ( CDPlayItem item ) = 0 ;
            
            // browse related
            virtual void getBrowseInfo ( FolderRequest request ) = 0 ;
            virtual void getFolderName ( short folderNumber, int nameLength, FolderRequestedData::eCutNameType cutName ) = 0 ;
            virtual void getFolderItemCount ( short FolderNumber ) = 0;
            
            virtual eEOLResult getFordEolData () = 0;
            
            // ignition of disc - load/ Eject
            virtual void load ( bool Load = false )  = 0 ;

            // destructor
            virtual ~IRemoteCDPlugin ( )    { }
    };
