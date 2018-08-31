#pragma once

#include <vector>
#include <string>
#include <memory>

// namespace declaratives
using namespace std ;

    // track Number - is a combination of FolderNumber, itemIndex of that track in the folder corresponding to FolderNumber
    typedef pair < short, short >       TrackNumber ;

  enum eItemType
    {
        eFolder     = 0x0,
        eFile       = 0x1,
        ePlayList   = 0x4,
        eVideoFile  = 0x8,
        eImageFile  = 0x10,
        eNone       = 0xFF
    };

    struct ItemData
    {
        short           index_ ;     // index of the item in its parent folder
        string          name_ ;      // name of the item
        eItemType       type_ ;      // type of the item
        TrackNumber     trackNum_ ;  // trackNum is the pair of <FolderNumber, position of the item in its parent - siff between this and index is for folder this would be zero>
        
        // constructor   
        ItemData () : index_ ( -1 ), name_ ( "" ), type_ ( eNone ), trackNum_ ( { -1, -1 } )    { }
        
        // getters
        inline short          getIndex ()  const   {    return index_ ;     }
        inline string         getName ()   const   {    return name_;       }
        inline eItemType      getType ()   const   {    return type_;       }
        inline TrackNumber    getNumber () const   {    return trackNum_;   }

        // setters
        inline void           setIndex ( const short index )         {  index_ = index ;   }
        inline void           setName ( const string name )          {  name_ = name ;     }
        inline void           setType ( const eItemType type )       {  type_ = type ;     }
        inline void           setNumber ( const TrackNumber num )    {  trackNum_ = num ;  }

        // destructor   
        virtual ~ItemData ()    { }
    };

    typedef shared_ptr < ItemData >   Item ;
    typedef vector < Item >           ItemList ;

