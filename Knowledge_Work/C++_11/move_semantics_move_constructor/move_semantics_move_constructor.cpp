/*

  [ move semantics with move constructor syntax ]  : A ( A&& obj ) {} : move constructor takes Rvalue by default
  [ move semantic if move constructor present in base class ] : calling move will in turn call base class move constructor
  [ move semantic if destructor present , move constructor in base class ] : calling move will call copy constructor of base class 
 
*/


#include <iostream>
#include <functional>
#include <algorithm>
#include <string>

using namespace std;

class A 
{

       private :
						string s;
	public  :
            A () : s (" A default string")   { cout << "default constructor" << endl ;  }
            A ( const A& obj ) : s ( obj. s ) { cout << "copy constructor of A" << endl; }
            A ( A&& obj ) :      s ( move ( obj. s )  ) { cout << "move constructor of A" << endl; }
	    string            printString () {  return    s ;                               }
};


A temp ( A a )
{

  return a ;
}

class B : public A 
{
  
};

class C : public B 
{
     public :
          ~C() {};
};

class D : public C 
{
 
     public :
         ~D()  {} ;
          D()  = default ; 
          D ( D && obj ) = default ;
};

int main ()
{
		
      A a1 = temp ( A ( )  ); // temporary object will be created which will universal Rvalue 
      cout << " [ before move string value a1 ] : " << a1.printString () << endl;
      A a2 = std::move ( a1 ) ; // move constructor will be used 
      cout << " [ After move string value a1  ] : " << a1.printString () << endl;
      cout << " [ After move string value a2  ] : " << a2.printString () << endl;
      B b1;
      B b2 = move ( b1 ) ; // Base class move constructor will be called since it does not have move constructor 
      C c1 ;
      C c2 = move ( c1 );  // copy constructor of the base class will be called
      D d1;
      D d2 = move ( d1 ) ;  // ideally move constructor of the base class should be called but compiler specific

}
