/*

unique pointer is a smart pointer which takes care of the memory management of an class object.
It automatically invokes the destructor when the object reached end of lifetime.

unique_ptr supports single ownership of a pointer . It deletes the copy constructor and assignment 
operator instead it supports move copy constructor and move assignment operator which transfers the
pointer from one unique_ptr to another.

*/

#include <iostream>
#include <memory>

using namespace std;

class X
{

public:

X() {cout << "X: constructor" << endl;};
~X() {cout << "X: destructor" << endl;};

void sayHello()
{

cout << "Hello X" << endl;

}

X(const X& obj)
{

cout << "copy constructor : X" << endl;

}

X& operator = (const X& obj)
{

cout << "assignment operator : X" << endl;
return *this;

}

};

int main()
{

unique_ptr<X> a( new X());

unique_ptr<X> b = move(a);

cout << "a.get():" << a.get() << endl;

b->sayHello();
a->sayHello();
return 0;
}

