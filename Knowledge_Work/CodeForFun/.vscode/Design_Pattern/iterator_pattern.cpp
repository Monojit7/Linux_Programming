/*

Iterator pattern comes under behavioural pattern .

This pattern provides a way to get access of elements from collection of objects 
without knowing its underlying implementation

*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Iterator
{

 public:

     virtual bool hasNext() = 0;

     virtual string Next () = 0;

};

class Container 
{
    public:

    virtual Iterator* getIterator () = 0;
};


class NameIterator : public Iterator
{
    public:

     vector<string>  nameList ;//s= {"sujata", "puja", "krishna", "raksha"};
    int index = 0 ;

    NameIterator ( vector <string > nameList )
    {
       this->nameList = nameList;
    }
    bool hasNext ()
    {
        if ( index < nameList.size() )
        {
            return true;
        }

        return false;

    }

    string Next()
    {
        if ( hasNext ())
        {
            return nameList [index++];
        }

        return "invalid";
    }
};

class NameRepository :public Container
{

   vector <string > nameList ;
   public:

  NameRepository ( vector <string > nameList )
  {
      this->nameList = nameList;
  }

  Iterator* getIterator ()
  {
      return new NameIterator ( this->nameList );
  }
 
};

int main ()
{
    vector <string > nameList = {"ahona", "rita", "puja", "rekha", "saranya"};   
    NameRepository* mNameRepository = new NameRepository ( nameList );

   for ( Iterator* it = mNameRepository->getIterator () ; it->hasNext() == true ;  )
   {
       cout << "name is "<< it->Next () << endl;
   }

}