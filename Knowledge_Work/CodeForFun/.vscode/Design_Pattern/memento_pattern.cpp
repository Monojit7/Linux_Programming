/*

memento pattern comes under behavioral pattern. It is used when the objects
previous state need to be restored

*/

#include <iostream>
#include <string>
#include <vector>
#include <list>
using namespace std;


class Memento 
{

  private:
  string state;

  public:
  Memento ( string state )
  {
      this->state = state;
  }

  string getState ()
  {
      return state;
  }

};

class Originator 
{
    private:
    string state ;

    public:

    Memento* saveStateToMemento ( string state )
    {
        return new Memento ( state );
    }

    void getStateFromMemento ( Memento* memento )
    {
        if ( memento != nullptr )
        this->state = memento->getState();
    }

    string getState ()
    {
        return this->state;
    }
 
};

class CareTaker 
{
    list <Memento*>  mementoList ;

    public:

    void add ( Memento* vMemento )
    {
        this->mementoList.push_front ( vMemento);
    }

    Memento* get ( int index )
    {
        if ( index < mementoList.size())
        {
            auto it = next ( mementoList.begin() , index );
            return *it;
        }
        else
        {
            cout << "invalid index" << endl;
            
            return nullptr;
        }
    }
};

int main ()
{
    Originator* mOriginator = new Originator;
    CareTaker* mCaretaker = new CareTaker;

    mCaretaker->add ( mOriginator->saveStateToMemento ( "state #1 "));


    mCaretaker->add ( mOriginator->saveStateToMemento ( "state #2 "));
    mCaretaker->add ( mOriginator->saveStateToMemento ( "state #3 "));
    mCaretaker->add ( mOriginator->saveStateToMemento ( "state #4 "));

    mOriginator->getStateFromMemento ( mCaretaker->get (0));

    cout << " current state is " << mOriginator->getState() << endl;

    mOriginator->getStateFromMemento ( mCaretaker->get (1));
    cout << "Second state is " <<  mOriginator->getState() << endl;

        mOriginator->getStateFromMemento ( mCaretaker->get (2));
    cout << "Second state is " <<  mOriginator->getState() << endl;

        mOriginator->getStateFromMemento ( mCaretaker->get (3));
    cout << "Second state is " <<  mOriginator->getState() << endl;

        mOriginator->getStateFromMemento ( mCaretaker->get (4));
    cout << "Second state is " <<  mOriginator->getState() << endl;




     
}