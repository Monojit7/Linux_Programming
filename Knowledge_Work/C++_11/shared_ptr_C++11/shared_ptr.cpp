/* 

shared pointer: Advantage:

1> It takes care of the memory management when the pointer is shared between many owners.
2> make_shared<class T> is variadic template function which creates contigious memory in heap 
   for the class T and it is referrence counter. 
   It inovokes constructor with the argument of class T.
   It ommits the usage of new , delete etc and takes care of heap memory management.
   It instantiate only once , shared between others based on referrence counter.
   It returns shared_ptr.

3> It always better to have shared_ptr as return type of function instead of naked_pointer.
   Reason: If the library is shared with other supplier supplier will not be sure either
           to delete the pointer or customize it etc.

4> shared_ptr can be used inside STL so programmer need not to do memory management.
5> shared_ptr is always exception safe. 


*/


#include <iostream>
#include <memory>
using namespace std;

class X
{
private:
int data;
public:

X(int data)
{

cout << "X: constructor" << endl;
this->data = data;
} 

~X()
{

 cout << "X: destructor" << this->data << endl;

}

int getvalue()
{

return this->data;
}
};



int main()
{

shared_ptr <X> a = make_shared<X> (30);
shared_ptr <X> b = a;
shared_ptr <X> c = make_shared<X> (50);
shared_ptr <X> d = make_shared<X> (60);
shared_ptr <X> e = make_shared<X> (70);

c = b;// ref count of c will be decremented, destructor will be called

cout << "a->getvalue" << a->getvalue() <<  endl;
cout << "b->getvalue" << b->getvalue() <<  endl;
cout << "c->getvalue" << c->getvalue() <<  endl;
cout << "d->getvalue" << d->getvalue() <<  endl;
cout << "e->getvalue" << e->getvalue() <<  endl;

}
