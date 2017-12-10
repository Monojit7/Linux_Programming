#include <iostream>

using namespace std;

template<typename T>
class shared_refptr
{

public:

int refCount_;
T* Pointee_;

explicit shared_refptr( T* pointee = nullptr ): refCount_( 0 ),Pointee_( nullptr )
{

this->refCount_ = 1;

cout << "shared_refptr:constructor:: refcount" << this->refCount_ << endl;

this->Pointee_ = pointee;
 
}

void release()
{
this->refCount_--;
cout << "shared_refptr:release:: refcount" << this->refCount_ << endl;
if(this->refCount_ == 0)
{

delete this->Pointee_;

cout << "shared_refptr deleting memory" << endl;
}

}

shared_refptr(const shared_refptr<T>& mshared_ptr)
{

this->refCount_ = mshared_ptr.refCount_;
this->Pointee_ = mshared_ptr.Pointee_;

this->refCount_++;

cout << "shared_refptr:copy constructor:: refcount" << this->refCount_ << endl;
}

shared_refptr<T>& operator= (const shared_refptr<T>& mshared_ptr)
{
this->release();
this->refCount_ = mshared_ptr.refCount_;
this->Pointee_ = mshared_ptr.Pointee_;

this->refCount_++;
cout << "shared_refptr:assignment operator:: refcount" << this->refCount_ << endl;
//return *this;
}

T* operator->()
{

 return this->Pointee_;
}


T& operator* ()
{

return *(this->Pointee_);
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
