/*

In visitor pattern there will be a visitor class ,as and when it changes it can
change the execution algorthm of its element class 

This is part of behavioural pattern

What problems can the Visitor design pattern solve?
It should be possible to define a new operation for
 (some) classes of an object structure without changing the classes.
When new operations are needed frequently and the object structure 
consists of many unrelated classes, it's inflexible to add new subclasses
each time a new operation is required because "[..] distributing all these
operations across the various node classes leads to a system that's hard 
to understand, maintain, and change."[1]

What solution does the Visitor design pattern describe?
Define a separate (visitor) object that implements an operation to be performed 
on elements of an object structure.
Clients traverse the object structure and call a dispatching operation accept (visitor) on an element
 — that "dispatches" (delegates) the request to the "accepted visitor object". The visitor object then
  performs the operation on the element ("visits the element").
This makes it possible to create new operations independently from the classes of an object structure by
 adding new visitor objects.


 Visitor pattern uses "double displatch" mechanism

*/


#include <iostream>
#include <list>
using namespace std;

class ComputerPart;

class KeyBoard;

class Mouse;

class Monitor;

class Computer;

class ComputerPartVisitor;

class ComputerPart
{

  public:

  virtual void accept ( ComputerPartVisitor* mComputerPartVisitor ) = 0;

};

class ComputerPartVisitor
{
    public:

    virtual void visit ( KeyBoard* mKeyBoard )=0;
    virtual void visit ( Mouse* mMouse )=0;
    virtual void visit ( Monitor* mMonitor )=0;
    virtual void visit ( Computer* mComputer )=0;
};

class KeyBoard : public ComputerPart
{
    public:

    void accept ( ComputerPartVisitor* mComputerPartVisitor ) 
    {
        mComputerPartVisitor->visit (this);
    }

};

class Mouse : public ComputerPart
{
    public:

    void accept ( ComputerPartVisitor* mComputerPartVisitor ) 
    {
        mComputerPartVisitor->visit (this);
    }

};



class Monitor : public ComputerPart
{
    public:

    void accept ( ComputerPartVisitor* mComputerPartVisitor ) 
    {
        mComputerPartVisitor->visit (this);
    }

};

class Computer : public ComputerPart
{

   list<ComputerPart*> computers = {};

   public:

   Computer ()
   {
     // computers.push_front ( new KeyBoard);
      computers.push_front ( new Mouse    );
      computers.push_front ( new Monitor  ) ;
      computers.push_front ( new KeyBoard );

   }

   void accept ( ComputerPartVisitor* mComputerPartVisitor  )
   {
       for ( ComputerPart* computerPart : this->computers )
       {
           computerPart->accept ( mComputerPartVisitor  );
       }

       mComputerPartVisitor->visit (this);
   }


};



class ComputerDisplayVisitor : public ComputerPartVisitor
{
    void visit ( KeyBoard* mKeyBoard )
    {
        cout << "Displaying KeyBoard " << endl;
    }

        void visit ( Mouse* mKeyBoard )
    {
        cout << "Displaying Mouse " << endl;
    }

        void visit ( Monitor* mKeyBoard )
    {
        cout << "Displaying Monitor " << endl;
    }

        void visit ( Computer* mKeyBoard )
    {
        cout << "Displaying Computer " << endl;
    }

};

int main()
{
    Computer* mComputer = new Computer;

    mComputer->accept ( new ComputerDisplayVisitor );

    return 0;
}



