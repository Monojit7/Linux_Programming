#include <iostream>

#include <typeinfo>

using namespace std;

class T
{

  public:
         T() = default;
         virtual void show () { cout << "method show T" << endl; } 

};

class B : public T
{

  public:
        B() = default;

        void show () { cout << "method show B" << endl; } 
};


int main()
{

 T* obj = new B();
 T* obj1 = new B();
 cout << " [ obj: ] " << typeid ( &obj ) . name () << " [ obj1 : ]" << typeid ( &obj ) . name()  << endl; // result will be PP1T meaning pointer to pointer of T

 obj -> show ( ) ;

 return 0;
}
