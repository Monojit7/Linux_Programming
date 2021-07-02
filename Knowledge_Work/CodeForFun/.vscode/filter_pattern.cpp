/*

Filter pattern comes under structural pattern.

Filter pattern or criteria pattern enables developer to filter set of objects 
based on different criteria. 

This pattern is a structural pattern because this pattern combines multiple 
criteria to obtain single criteria.

We are going to use criteria pattern to filter set of person objects based 
on criteria.

Criteria is a interface class , the concrete classes of this class implements
it interface to list down set of person objects based on criteria
*/

#include <iostream>
#include <list>
using namespace std;

class Person
{

    string name;

    string gendre;

    string marrital_status;

public:
    Person(string name, string gendre, string marrital_status)
    {
        this->name = name;
        this->gendre = gendre;
        this->marrital_status = marrital_status;
    }

    void setName(string name)
    {
        this->name = name;
    }

    void setGendre(string gendre)
    {
        this->gendre = gendre;
    }

    void setMarritalStatus(string marrital_status)
    {
        this->marrital_status = marrital_status;
    }

    string getName()
    {
        return this->name;
    }

    string getMarritalStatus()
    {
        return this->marrital_status;
    }

    string getGendre()

    {
        return gendre;
    }
};

class Criteria
{

public:
    list<Person *> mCriteriaList;

    virtual list<Person *> meetCriteria(list<Person *> persons) = 0;

    void addPerson(Person *person)
    {
        getmCriteriaList().push_back(person);
    }

    list<Person *> &getmCriteriaList()
    {
        return this->mCriteriaList;
    }
};

class MaleCriteria : public Criteria
{
public:
    list<Person *> meetCriteria(list<Person *> persons)
    {
        for (Person *person : persons)
        {
            if (person->getGendre() == "MALE")
            {
                addPerson(person);
            }
        }
        return getmCriteriaList();
    }
};

class FemaleCriteria : public Criteria
{
public:
    list<Person *> meetCriteria(list<Person *> persons)
    {
        for (Person *person : persons)
        {
            if (person->getGendre() == "FEMALE")
            {
                addPerson(person);
            }
        }

        return getmCriteriaList();
    }
};

class MarriedCriteria : public Criteria
{
public:
    list<Person *> meetCriteria(list<Person *> persons)
    {
        for (Person *person : persons)
        {
            if (person->getMarritalStatus() == "MARRIED")
            {
                addPerson(person);
            }
        }

        return getmCriteriaList();
    }
};

class SingleCriteria : public Criteria
{
public:

    list<Person *> meetCriteria(list<Person *> persons)
    {
        for (Person *person : persons)
        {
            if (person->getMarritalStatus() == "SINGLE")
            {
                addPerson(person);
            }
        }

        return getmCriteriaList();
    }
};



class AndCriteria : public Criteria
{
    Criteria *mCriteria;
    Criteria *otherCriteria;

public:

    AndCriteria(Criteria *mCriteria, Criteria *otherCriteria)
    {
        this->mCriteria = mCriteria;
        this->otherCriteria = otherCriteria;
    }

    list<Person *> meetCriteria(list<Person *> persons)
    {
        list<Person *> firstCriteriaPersons = mCriteria->meetCriteria(persons);
        return otherCriteria->meetCriteria( firstCriteriaPersons );
    }
};
/*
class OrCriteria : public Criteria
{
    Criteria *mCriteria;
    Criteria *otherCriteria;

    OrCriteria(Criteria *mCriteria, Criteria *otherCriteria)
    {
        this->mCriteria = mCriteria;
        this->otherCriteria = otherCriteria;
    }

    list<Person *> &meetCriteria(list<Person *> persons)
    {
        list<Person *> &persons = mCriteria->meetCriteria(persons);
        list<Person *> &otherPersons = otherCriteria->meetCriteria(persons);

        for (Person *person : otherPersons)
        {
            if (find(persons.begin(), persons.end(), person) != persons.end())
            {
                persons.push_back(person);
            }
        }

        return persons;
    }
};*/

static void printDetails ( list<Person*>& persons )
{
    for ( Person* person : persons )
    {
        cout << " name " << person->getName() << " gendre " << person->getGendre() << " marrital status " << person->getMarritalStatus() << endl;
    }
}

int main()
{
    list<Person *> persons {};

    persons.push_back(new Person("Monojit", "MALE", "MARRIED"));
    persons.push_back(new Person("Radha", "FEMALE", "MARRIED"));
    persons.push_back(new Person("Jadu", "FEMALE", "SINGLE"));
    persons.push_back(new Person("Madhu", "MALE", "SINGLE"));
    persons.push_back(new Person("Sujata", "FEMALE", "SINGLE"));
    persons.push_back(new Person("Raju", "MALE", "MARRIED"));
    persons.push_back(new Person("KRISHNA", "FEMALE", "SINGLE"));
    persons.push_back(new Person("Roja", "FEMALE", "SINGLE"));
    persons.push_back(new Person("Raka", "MALE", "SINGLE"));
    persons.push_back(new Person("Raksha", "FEMALE", "SINGLE"));
    persons.push_back(new Person("Nansi", "FEMALE", "MARRIED"));
    persons.push_back(new Person("Prana", "MALE", "SINGLE"));
    persons.push_back(new Person("Rajat", "MALE", "MARRIED"));

    Criteria* maleCriteria = new MaleCriteria;
    Criteria* femaleCriteria = new FemaleCriteria ;
    Criteria* singleCriteria = new SingleCriteria ;
    Criteria* marriedCriteria = new MarriedCriteria;
   Criteria* andCriteria = new AndCriteria ( femaleCriteria, singleCriteria );
    //Criteria* OrCriteria = new OrCriteria ( singleCriteria, femaleCriteria);

    list<Person*> maleList = maleCriteria->meetCriteria ( persons );

    cout << "below male list :" << endl;

    printDetails ( maleList );

    list <Person*> femalelist = femaleCriteria->meetCriteria ( persons );

    cout << "below female list :" << endl;
    printDetails ( femalelist );

    list <Person*> singlelist = singleCriteria->meetCriteria ( persons );
    cout << "below single list :" << endl;
    printDetails ( singlelist );

    list <Person*> marriedlist = marriedCriteria->meetCriteria ( persons );
    cout << "below married list :" << endl;
    printDetails ( marriedlist );

    list <Person*> singleFemalelist = andCriteria->meetCriteria ( persons );

    cout << "below single Female  list :" << endl;
    printDetails ( singleFemalelist );





}