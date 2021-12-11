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

    virtual void subscribe ( ) = 0;

    virtual void unSubscribe ( ) = 0;
 

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

    void unAttach ( Observer* mObserver  )
    {
        if ( !Observers.empty())
        {
            Observers.remove ( mObserver );
        }
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

    }

    void subscribe ( )
    {
        this->mSubject->attach ( this );
    }

    void unSubscribe ( )
    {
        this->mSubject->unAttach ( this );
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

    }

    void subscribe ( )
    {
        this->mSubject->attach ( this );
    }

    void unSubscribe ( )
    {
        this->mSubject->unAttach ( this );
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
    }

    void subscribe ( )
    {
        this->mSubject->attach ( this );
    }

    void unSubscribe ( )
    {
        this->mSubject->unAttach ( this );
    }

    void update ()
    {
        cout << "State in Octal is "  << oct << this->mSubject->getState() << endl;
    }

  virtual ~OctState ()
    {
        cout << "OctObserver destructor got called " << endl;

    }
};

int main()
{
    Subject* vSubject = new Subject;

    Observer* mBinObserve = new BinaryState ( vSubject );
    Observer* mOctObserve = new OctState ( vSubject );
    Observer* mHexObserve = new HexState ( vSubject );

    cout << "subscribed only for Binary state update, oct, hex " << endl;

    mBinObserve->subscribe();
    mOctObserve->subscribe();
    mHexObserve->subscribe();

    cout << "first state change to 15 " << endl;

    vSubject->setState ( 15 );

    cout << "unsubscribed for HexState " << endl;

    mHexObserve->unSubscribe();

        cout << "second state change to 13 " << endl;
    
    vSubject->setState ( 13 );

   mHexObserve->subscribe ();
        cout << "third state change to 10 " << endl;
    
    vSubject->setState ( 10 );

    delete vSubject;

    delete mBinObserve;
    delete mOctObserve;
    delete mHexObserve;

    return 0;
}

