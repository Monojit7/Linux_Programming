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

     vector<string>  nameList= {"sujata", "puja", "krishna", "raksha"};
    int index = 0 ;

    NameIterator ()
    {
       
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

        return nullptr;
    }
};

class NameRepository :public Container
{

   public:

  

  Iterator* getIterator ()
  {
      return new NameIterator;
  }
 
};

int main ()
{
   NameRepository* mNameRepository = new NameRepository;

   for ( Iterator* it = mNameRepository->getIterator () ; it->hasNext() == true ;  )
   {
       cout << "name is "<< it->Next () << endl;
   }

}