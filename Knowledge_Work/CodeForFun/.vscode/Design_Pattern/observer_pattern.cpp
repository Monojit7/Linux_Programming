/*

Observer pattern comes under behavioural pattern.
Observer pattern is used when there is one to many relationship 
between objects such as if one object is modified its dependent
objects are notified automatically

*/


#include <iostream>
#include <list>

using namespace std;

class Observer ;

class Subject;

class BinaryState;

class HexState;

class OctState;



class Observer 
{
    public:

    virtual ~Observer ()
    {
        cout << "Observer destructor got called " << endl;

    };

    Subject* mSubject;

 

    virtual void update() = 0;

};

class Subject

{
    public:

    list <Observer*> Observers;

    int state  = 0;

    void attach ( Observer* mObserver )
    {
        Observers.push_back ( mObserver );

       // NotifyAllUsers ();
    }  

    void detach (Observer* mObserver )
    {
        Observers.remove (mObserver);
      //  delete mObserver;
    }

    void setState ( int state )
    {

        this->state = state;

        NotifyAllUsers ();

    }

    int getState ()
    {
        return state;
    }

    void NotifyAllUsers ()
    {
        for ( Observer* ob : Observers )
        {
            ob->update();
        }
    }

};






class BinaryState : public Observer
{
    public:

    BinaryState ( Subject* vSubject )
    {
        this->mSubject = vSubject;

        this->mSubject->attach(this);
    }

    virtual ~BinaryState ()
    {
        cout << "Binary Observer destructor got called " << endl;

    };





    void update ()
    {
        cout << "state in Binary is "  << bitset <8> ( this->mSubject->getState() ) << endl;
    }

} ;


class HexState : public Observer
{

    public:
    HexState ( Subject* vSubject )
    {
        this->mSubject = vSubject;
        this->mSubject->attach ( this );
    }

    void update ()
    {
        cout << "State in hexadecimal is " << hex << this->mSubject->getState() << endl;
    }



     ~HexState ()
    {
        cout << "Hex Observer destructor got called " << endl;

    }
};

class OctState : public Observer
{
   public :

    OctState ( Subject* vSubject )
    {
        this->mSubject = vSubject;
        this->mSubject->attach ( this );
    }

    void update ()
    {
        cout << "State in Octa is "  << oct << this->mSubject->getState() << endl;
    }



  virtual ~OctState ()
    {
        cout << "OctObserver destructor got called " << endl;

    }
};

int main()
{
    Subject* vSubject = new Subject;

    Observer* mOb1 = new BinaryState ( vSubject );
    Observer* mOb2 = new OctState ( vSubject );
    Observer* mOb3 = new HexState ( vSubject );

    cout << "first state change to 15 " << endl;

    vSubject->setState ( 15 );


        cout << "unsubscribing oct " << endl;

        vSubject->detach ( mOb2 );

         cout << "second state change to 13 " << endl;
    
    vSubject->setState ( 13 );

        cout << "third state change to 10 " << endl;
    
    vSubject->setState ( 10 );

    delete vSubject;

    delete mOb1;
    delete mOb2;
    delete mOb3;

    return 0;
}

