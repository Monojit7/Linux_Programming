#include <iostream>
#include <map>
#include <memory>
#include <functional>
#include <string>
#include <stdlib.h>
#include <vector>
using namespace std;
using namespace placeholders;
typedef shared_ptr < vector < string > >   tpdata ;
    
    // handler function
typedef function < bool ( tpdata ) > handlerType ;

map < int, handlerType >    messageIdHandlerMap_ ;

class DataHandler
{

public:

DataHandler()
{

  this->initDataHandler();
}

void initDataHandler();
bool nameReceived      ( tpdata data );
bool albumNameReceived ( tpdata data );
void func(vector<string> namelist, int value);

};

void DataHandler::func(vector <string> namelist, int value)
{

  cout << "   " << __FUNCTION__ << "   " << __LINE__ << endl;
  auto data = make_shared < vector < string > > ();
  *data = namelist; 
  messageIdHandlerMap_[value] (data);

}

bool DataHandler::nameReceived(tpdata data)
{

cout << "   " << __FUNCTION__ << "   " << __LINE__ << endl;

for ( auto i : (*data))
cout << "data value is:  " << i << endl;


}



bool DataHandler::albumNameReceived      ( tpdata data )
{

cout << "   " << __FUNCTION__ << "   " << __LINE__ << endl;

for ( auto i : (*data))
cout << "data value is:  " << i << endl;



}


void DataHandler::initDataHandler()
{

cout << "   " << __FUNCTION__ << "   " << __LINE__ << endl;

    messageIdHandlerMap_ = 
    {
        /* trackNameSt */       {   0x46,   bind ( &DataHandler::nameReceived, this,   _1 )   },
        /* AlbumName */         {   0x56,   bind ( &DataHandler::albumNameReceived, this,   _1 )   } 
    };

}

int main()
{

  DataHandler obj;
  vector <string> namelist = { "Monojit" , "Chatterjee" , "GarhJoypur" , "Purulia" };
  obj.func(namelist, 0x46 );
 
  vector <string> albumList = { "Rabindranath" ,  "Tagore" , "Ekla Chalo re" ,  "by Amitabh Bachan" };
  obj. func ( albumList, 0x56 );
  return 0;

}
