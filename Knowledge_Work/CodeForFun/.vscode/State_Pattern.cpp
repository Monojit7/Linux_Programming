/*

In State pattern a class behavior changes based on its state. This type of design 
pattern comes under behavior pattern.

In State pattern, we create objects which represent various states and a context 
object whose behavior varies as its state object changes

*/


#include <iostream>

using namespace std;

class Context;

// interface class
class State 
{
    public:
    virtual void doAction (  ) = 0;
    virtual void ReportState () = 0;
};

// context class

class Context 
{
    public:
    State* mState = nullptr;

    void setState ( State* mState )
    {
        this->mState = mState;
    }

    State* getState ()
    {
        return this->mState;
    }


};


// concrete class

class StartState : public State
{
    public:

    void doAction ( )
    {
        cout << "Start state Action" << endl;
       
    }

    void ReportState ()
    {
        cout << "State is Start" << endl;
    }
};



class StopState : public State
{
    public:

    void doAction ( )
    {
        cout << "Stop state Action" << endl;

    }

    void ReportState ()
    {
        cout << "State is Stop" << endl;
    }
};



int main()
{
    State* vState = new StartState();

    Context* vContext = new Context();

    vContext->setState ( vState );

    vContext->getState()->doAction();

     vContext->getState()->ReportState();

       vState = new StopState();

    vContext->setState ( vState );


    vContext->getState()->doAction();

    vContext->getState()->ReportState();

}