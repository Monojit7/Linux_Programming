/*

In strategy pattern  , class behaviour or underlying algorithm can be changed at runtime .
This is behaviour pattern.

In strategy pattern, there are strategy object and context objects. if the strategy objects
changes its algorithm context object also behaves accordingly.

strategy object changes the behaviour of the context object

*/

/*

In the below example strategy class has one interface doOperation which takes two number as arguments.

startegy class has three subclasses operationAdd, operationSubstract, operationMultiply which implements

doperation method.

Context class has memeber of strategy class pointer which is assigned to proper concrete class ( operationAdd, operationSubstract, operationMultiply)
in the constructor

It has method executeStartegy which in turn invoke strategy class dooperation and corresponding strategy has been executed

*/


#include <iostream>
#include <list>

using namespace std;

class Strategy
{
    public:
    virtual void doOperation ( int n1, int n2 ) = 0;
};

class operationAdd : public Strategy
{
    public:
    void doOperation( int n1, int n2 )
    {
        cout << "the result is " << ( n1 + n2 ) << endl;
    }
};

class operationMultiPly : public Strategy
{
    public:
    void doOperation( int n1, int n2 )
    {
        cout << "the result is " << ( n1 * n2 ) << endl;
    }
};

class operationSubstract : public Strategy
{
    public:
    void doOperation( int n1, int n2 )
    {
        cout << "the result is " << ( n1 - n2 ) << endl;
    }
};

class Context
{
    private: 
    Strategy* mStrategy = nullptr;

    public:
    Context ( )
    {
       
    }

    void setStrategy ( Strategy* mStrategy )
    {
        this->mStrategy = mStrategy;
    }

    void executeStrategy ( int n1 , int n2 )
    {
        this->mStrategy->doOperation ( n1 , n2 );
    }

};


int main ()
{
    int n1 = 11;
    int n2 = 6;
    Context* mContext = new Context ;
    Strategy* mStrategy = new operationAdd;
    mContext->setStrategy ( mStrategy );
    mContext->executeStrategy ( n1, n2 );
    mStrategy = new operationMultiPly;
    mContext->setStrategy ( mStrategy );
    mContext->executeStrategy ( n1, n2 );
    mStrategy = new operationSubstract;
    mContext->setStrategy ( mStrategy );
    mContext->executeStrategy ( n1, n2 );
}






