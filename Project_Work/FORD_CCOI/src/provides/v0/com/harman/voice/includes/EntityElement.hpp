
#ifndef ENTITY_ELEMENT_HPP
#define ENTITY_ELEMENT_HPP

#include <vector>
#include <string>
#include <memory>

// namespace declaratives
using namespace std ;

namespace v0 {
    namespace com {
        namespace harman {
            namespace voice {
    
struct EntityElement
{
    string entityID_ ;
    string entityType_ ;

    vector < string > values_ ;

    int priority_ ;

    // constructors
    EntityElement ()    { }
    EntityElement ( string id, string type, int priority = -1 ) : entityID_ ( id ), entityType_ ( type ), priority_ ( priority ) { }
    EntityElement ( string id, string type, vector < string > flags, int priority = -1 ) : entityID_ ( id ), entityType_ ( type ), values_ ( flags ), priority_ ( priority ) { }
    
    // setters
    inline void setPriority ( int priority )               {    priority_ = priority ;   } 
    inline void setValues ( vector < string > &flags )     {    values_. assign ( flags. begin (), flags. end () ) ;   } 
    
};

typedef shared_ptr < EntityElement > Entity ;
typedef vector < shared_ptr < EntityElement > > EntityList ;
typedef vector < vector < shared_ptr < EntityElement > > > EntityListVector ;

            } // voice
        } // harman
    } // com
} // v0

#endif // ENTITY_ELEMENT_HPP

