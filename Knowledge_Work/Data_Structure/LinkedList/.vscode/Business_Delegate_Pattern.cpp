/*
Business delegate pattern is used to decouple presentation tier with Business tier.
This is used to reduce direct communication between presentation tier with Business tier.
By using LookupService object this has been achieved.

Client : UI Service Object
Business Delegate class : This is a single entry class to client entities which provides access
                           to business service object
LookUp Service : This object provide relative object to Business delegate object to get the object 
                  which has the requested implementation
Business Service Interface : This is a interface class which describes the business methods. Concreate 
                             classes of this class implements the business logic of it.
*/
#include <iostream>

using namespace std;

class BusinessSerice 
{
    public :
            virtual void doAction () = 0 ;
};


class SoftwareService : public BusinessSerice
{
    public:
           void doAction ()
           {
               cout << "Executing Software Service operation " << endl;
           }
};

class CarpentorService : public BusinessSerice
{
    public:
           void doAction ()
           {
               cout << "Executing Carpentor Service operation " << endl;
           }
};

class BusinessLookupService
{
   public :



       BusinessSerice* getServiceObject ( string type )
       {
           if ( type == "Software")
           {
               cout << "returning software sevice object " << endl;
               return new SoftwareService;
           }
           else if ( type == "Carpentor" )
           {
               cout << "returning carpentor sevice object " << endl;
               return new CarpentorService;
           }
           else
           {
               return nullptr;
           }
       }


};

class BusinessDelegate 
{
   public:
   BusinessSerice* mBusinessService = nullptr;
   BusinessLookupService* mBusinessLookupService = nullptr;
   string serviceType = "";

   BusinessDelegate ()
   {
       mBusinessLookupService = new BusinessLookupService;
   }

   void setServiceType ( string type )
   {
       this->serviceType = type;
   }

   void doAction ()
   {
       if ( mBusinessLookupService  != nullptr )
       {

       }
       mBusinessService = mBusinessLookupService->getServiceObject ( serviceType );
       mBusinessService->doAction ();
   }



};

class Client 
{
    public :

    BusinessDelegate* mBusinessDelegate = nullptr;

    Client ( BusinessDelegate* mBusinessDelegate )
    {
        this->mBusinessDelegate = mBusinessDelegate;
    }

    void doAction ()
    {
        this->mBusinessDelegate->doAction ();
    }
};

int main ()
{
    BusinessDelegate* mBusinessDelegate = new BusinessDelegate;
    mBusinessDelegate->setServiceType ("Software");

    Client* mClient = new Client ( mBusinessDelegate );
    mClient->doAction ();

    //BusinessDelegate* mBusinessDelegate1 = new BusinessDelegate;
    mBusinessDelegate->setServiceType ( "Carpentor");
     
     Client* newClient = new Client (mBusinessDelegate );
     newClient->doAction ();



    }
