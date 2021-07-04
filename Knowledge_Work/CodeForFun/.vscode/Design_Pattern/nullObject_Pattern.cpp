/*

Null object pattern , null object has been used to replace check for null object instance.
Instead of putting if check for a null value , Null object reflects do nothing relationship.
Such Null object can also be used to provide default behaviour in case of data is not available.

*/

#include <iostream>
#include <string>
#include <vector>
#include <list>
using namespace std;

class AbstractCustomer
{
public:
    virtual string getName() = 0;

    virtual bool isNull() = 0;
};

class RealCustomer : public AbstractCustomer
{

    string name;

public:
    RealCustomer(string name)
    {
        this->name = name;
    }

    string getName()
    {
        return name;
    }

    bool isNull()
    {
        return true;
    }
};

class NullCustomer : public AbstractCustomer
{

    string name = " Not a valid name since Object is valid";

public:
    NullCustomer()
    {
    }

    string getName()
    {
        return name;
    }

    bool isNull()
    {
        return false;
    }
};

class CustomerFactory
{

    vector<string> nameList = {"Rob", "Roshni", "Sujata", "Puja"};

public:
    AbstractCustomer *getCustomer(string name)
    {
        if (find(nameList.begin(), nameList.end(), name) != nameList.end())
        {
            return new RealCustomer(name);
        }
        else
        {
            return new NullCustomer;
        }
    }
};

int main()
{
    CustomerFactory *mCustomerFactory = new CustomerFactory;

    AbstractCustomer *mAbstractCustomer1 = mCustomerFactory->getCustomer("Rob");
    AbstractCustomer *mAbstractCustomer2 = mCustomerFactory->getCustomer("Roshni");
    AbstractCustomer *mAbstractCustomer3 = mCustomerFactory->getCustomer("Krishna");
    AbstractCustomer *mAbstractCustomer4 = mCustomerFactory->getCustomer("Sujata");
    AbstractCustomer *mAbstractCustomer5 = mCustomerFactory->getCustomer("Karim");
    AbstractCustomer *mAbstractCustomer6 = mCustomerFactory->getCustomer("Puja");
    AbstractCustomer *mAbstractCustomer7 = mCustomerFactory->getCustomer("Jagan");

    cout << "customer 1" << mAbstractCustomer1->getName() << endl;
    cout << "customer 2" << mAbstractCustomer2->getName() << endl;
    cout << "customer 3" << mAbstractCustomer3->getName() << endl;
    cout << "customer 4" << mAbstractCustomer4->getName() << endl;
    cout << "customer 5" << mAbstractCustomer5->getName() << endl;
    cout << "customer 6" << mAbstractCustomer6->getName() << endl;
    cout << "customer 7" << mAbstractCustomer7->getName() << endl;

    return 0;
}
