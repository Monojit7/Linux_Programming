#include <iostream>

using namespace std;

template<typename T>
class shared_refptr
{

public:

int refCount;
T* Pointee;

explicit shared_refptr(T* pointee = NULL): refCount(0),Pointee(NULL)
{

this->refCount = 1;

cout << "shared_refptr:constructor:: refcount" << this->refCount << endl;

this->Pointee = pointee;
 
}

void release()
{
this->refCount--;
cout << "shared_refptr:release:: refcount" << this->refCount << endl;
if(this->refCount == 0)
{

delete this->Pointee;

cout << "shared_refptr deleting memory" << endl;
}

}

shared_refptr(const shared_refptr<T>& mshared_ptr)
{

this->refCount = mshared_ptr.refCount;
this->Pointee = mshared_ptr.Pointee;

this->refCount++;

cout << "shared_refptr:copy constructor:: refcount" << this->refCount << endl;
}

shared_refptr<T>& operator= (const shared_refptr<T>& mshared_ptr)
{
this->release();
this->refCount = mshared_ptr.refCount;
this->Pointee = mshared_ptr.Pointee;

this->refCount++;
cout << "shared_refptr:assignment operator:: refcount" << this->refCount << endl;
//return *this;
}

T* operator->()
{

 return this->Pointee;
}


T& operator* ()
{

return *(this->Pointee);
}

~shared_refptr()
{

cout << "shared_refptr:release:: destructor"  << endl;
this->release();

}
};



class Device
{

private:
int data;

public:

void show(int data)
{
this->data = data;
cout << "data: Device " << this->data << endl;
}

Device()
{
cout << " Device: constructor " << endl;

}
~Device()
{

cout << "Device: destructor" << endl;

}
};



class Engine
{

public:
Engine()
{
cout << "Engine Constructor" << endl;
}

~Engine()
{
cout << "Engine Destructor" << endl;
}

};

int main()
{

shared_refptr<Device> mDevice ( new Device());
shared_refptr<Device> mDevice1 = mDevice;
shared_refptr<Device> mDevice2 (new Device());
shared_refptr<Engine> mEngine ( new Engine());
shared_refptr<Engine> mEngine2 = mEngine;
//shared_refptr<Engine> mEngine3;
//mEngine3 = mEngine2;

mDevice1->show(40);
mDevice2->show(50);

return 0;
}
