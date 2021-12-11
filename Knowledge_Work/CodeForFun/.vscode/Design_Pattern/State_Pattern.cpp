/*

In State pattern a class behavior changes based on its state. This type of design 
pattern comes under behavior pattern.

In State pattern, we create objects which represent various states and a context 
object whose behavior varies as its state object changes



problem : when multiple states has complex behavioural impact then
          making lean state machine would be a trouble to maintain

solution : each state will be based on a interface super class and will 
           be separate class and the state class will maintain all 
        the state related behaviour in it.

        Context class will store the state object and changes all the operation
        as the state object changes

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

    void doAction ()
    {
        this->mState->doAction ();
    }

    void ReportState()
    {
        this->mState->ReportState();
    }




};


// concrete class

class StartState : public State
{
    public:

    void doAction (  )
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

    void doAction (  )
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
    State* vStartState = new StartState;

    State* vStopState = new StopState;

    Context* vContext = new Context();

   // state change happend
    vContext->setState ( vStartState );

   // behaviour changes of doAction
    vContext->doAction();
    vContext->ReportState();

    // state changes of doAction
    vContext->setState ( vStopState );

   // Behaviour changes
    vContext->doAction();
    vContext->ReportState();

    

}