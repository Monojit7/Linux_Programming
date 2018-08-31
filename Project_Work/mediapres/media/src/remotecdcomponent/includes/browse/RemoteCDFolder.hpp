#pragma once

#include <vector>
#include <string>
#include <memory>
#include <deque>
#include <RemoteCDItem.hpp>

// namespace declaratives
using namespace std ;

#define MAX_ITEM_NAME_LENGTH    ( 20 ) // currently supported max 19 characters + 1 end of string as per SPSS

    // structure to hold the path of the folder - it is a vector of folderNumber, itemIndex pair 
    // containing the information all the way from root to the parent of active folder & active folder being the first record in this vector
    typedef vector < pair < short, short > >            FolderPath ;
    
    // structure to hold the the summary contents a folder  this has type of content - file, playlist, folder along with their respective items count, for file - item count is zero
    typedef vector < pair < char, short > >             FolderSummary ;

    typedef vector < pair < unsigned char, short > >    CDTableofContents ;

    // structure to hold the the full contents of a folder
    struct FolderData
    {
        private:
            short           number_ ;       // folderNumber - enumerated by CD player slave 
            string          name_ ;         // full name of this folder
            unsigned short  totalItems_ ;   // totalItems in the folder
            short           parentNumber_ ; // parent of this folder
            ItemList        items_ ;        // actuals items data present in the folder
        
        public:
            // constructor   
            FolderData ( ) : number_ ( 0 ), name_ ( "" ), totalItems_ ( 0 ), parentNumber_ ( -1 )   { }
            FolderData ( short num, short parent, string name ) : number_ ( num ), name_ ( name ), totalItems_ ( 0 ), parentNumber_ ( parent )    { }

            // getters
            inline short           getNumber ()                   const   {  return number_ ;        }
            inline unsigned short  getTotalItems ()               const   {  return totalItems_ ;    }
            inline short           getParent ()                   const   {  return parentNumber_ ;  }
            inline string          getName ()                             {  return name_ ;          }
            inline ItemList&       getItemsList ()                        {  return items_ ;         }
            inline Item            getItem ( unsigned int index ) const   {  if ( index < items_. size () )    return items_ [ index ] ;    else    return nullptr ; }

            // setters
            inline void    setNumber ( short number )                    {  number_       = number ;      }
            inline void    setTotalItems ( unsigned short total )        {  totalItems_   = total ;       }
            inline void    setParent ( short parent )                    {  parentNumber_ = parent ;      }
            inline void    setName ( string &name )                      {  name_         = name ;        }
            inline void    addItem ( Item item )                         {  items_. push_back ( item ) ;  }
            bool updateItem ( short folderNumber, Item item ) ;   // find and updates the item

            // destructor   
            virtual ~FolderData ()    { items_. clear () ; }
    };
    typedef shared_ptr < FolderData >    Folder ;
    typedef vector < Folder >            FolderList ;

    // structure to hold the client's request to get the folder contents while browse of CD data
    struct FolderRequestedData
    {
        enum eFolderContent
        {
            eRequestedFolder,
            eParentFolder
        };

        enum eCutNameType
        {
            eNone,
            eHead,
            eTail,
            eThreeFourthHead,       // 3/4th of the name for the begining
            eOneFourthTail          // 1/4th of the name form the end
        };

        private:
            short             number_ ;
            short             startIndex_ ;
            eCutNameType      cutType_ ;
            char              count_ ;
            char              itemNameLength_ ;
            eFolderContent    content_ ;

        public:

            // constructors
            FolderRequestedData () : number_ ( 0 ), startIndex_ ( -1 ), cutType_ ( eHead ), count_ ( 0 ), itemNameLength_ ( MAX_ITEM_NAME_LENGTH ), content_ ( eRequestedFolder )    { }
            FolderRequestedData ( short num, short start, char count, eCutNameType cut, char nameLen, eFolderContent content )
                                 : number_ ( num ), startIndex_ ( start ), cutType_ ( cut ), count_ ( count ), itemNameLength_ ( nameLen ), content_ ( content )   { }

            // getters
            inline short        getCount ()            const   {  return count_ ;          }
            inline short        getNumber ()           const   {  return number_ ;         }
            inline eCutNameType getCutType ()          const   {  return cutType_ ;        }
            inline short        getStartIndex ()       const   {  return startIndex_ ;     }
            inline short        getItemNameLen ()      const   {  return itemNameLength_ ; }
            inline short        getFolderContent ()    const   {  return content_ ;        }

            // setters
            inline void    setNumber  ( short number )           {  number_         = number ;   }
            inline void    setCount   ( short count )            {  count_          = count;     }
            inline void    setCutType ( eCutNameType type )      {  cutType_        = type ;     }
            inline void    setStartIndex ( char start )          {  startIndex_     = start ;    }
            inline void    setItemNameLen ( char namelen )       {  itemNameLength_ = namelen ;  }
            inline void    setFolderContent ( eFolderContent content )    {  content_ = content ;        }

            // destructor   
            virtual ~FolderRequestedData ()    { }
    };
    typedef shared_ptr < FolderRequestedData >    FolderRequest ;
    typedef deque < FolderRequest >               FolderRequestList ;
    
