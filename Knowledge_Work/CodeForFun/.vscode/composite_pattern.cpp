/*

Composite pattern is used when we need to create a single object which holds a group 
of similar objects.

Composite pattern composes objects in tree to represent part as well whole hierarchy

This type of design pattern comes under structural pattern.

This pattern creates a class that contains group of its own object.

*/

#include <iostream>
#include <list>


using namespace std;

class Employee 
{
    public:
   string name ;
   string dept ;
   int salary ;
   list <Employee* > subOrdinates;
    Employee ( string name , string dept, int salary )
    {
        this->name = name ;
        this->dept = dept;
        this->salary = salary;
    }

    void add ( Employee* e )
    {
        subOrdinates.push_back ( e );
    }

    void remove ( Employee* e )
    {
        subOrdinates.remove ( e );
    }

    list<Employee*> getSubordinates ()
    {
        return this->subOrdinates;
    }


};

int main ()
{
    Employee* CEO = new Employee ("Ram", "CEO", 100000);
    Employee* HeadSales = new Employee ("Binita", "HeadSales", 60000 );
    Employee* HeadMarketing = new Employee ("Raghu", "HeadMarketing", 40000);

    Employee* clerk1 = new Employee ("Raksha", "Sales", 30000);
    Employee* clerk2 = new Employee ("Laxman", "Sales", 30000);

    Employee* salesExe1 = new Employee ("Roshni", "Marketing", 30000);
    Employee* salesExe2 = new Employee ("Jyotshna", "Marketing", 30000);

    CEO->add (HeadSales);
    CEO->add (HeadMarketing);

    HeadSales->add ( clerk1 );
    HeadSales->add ( clerk2 );

    HeadMarketing->add ( salesExe1 ) ;
    HeadMarketing->add ( salesExe2 ) ;


   cout << " name : " << CEO->name << " dept : " << CEO->dept << " salary " << CEO->salary << endl;
    for ( Employee* HeadEmployee : CEO->getSubordinates() )
    {
       cout << " name : " << HeadEmployee->name << " dept : " << HeadEmployee->dept << " salary " << HeadEmployee->salary << endl;
       for ( Employee* emp : HeadEmployee->getSubordinates() )
       {
           cout << " name : " << emp->name << " dept : " << emp->dept << " salary " << emp->salary << endl;
       }        
    }
}





