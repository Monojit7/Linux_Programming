/*

Data access pattern is used to segregate low level API implementation with high level
business service.
When DataBase or any xml has been accessed ( any other storage container) to segregate the 
database accessing implementation with high level business service this has been used.

Data Access Object Interface : This interface class has all the methods which are used by
                               business service
Data Access Object Concrete class : This class has all the implementation logic which are mentioned
                                    in the DAO interface class .All the Database related API's are
                                    implemented here
Model Object or Value Object : This class is simple C++ class which has get , set method and stores
                               values accessed from DATA access object.
                

*/

#include <iostream>
#include <list>
using namespace std;

class Student 
{
    public:
    string name =  {};
    int roll = 0;

    Student ( string name , int roll)
    {
        this->name = name;
        this->roll = roll;
    }

    int getRoll ()
    {
        return this->roll;
    }

    void setRoll ( int roll )
    {
        this->roll = roll;
    }


    string getName ()
    {
        return this->name;
    }

    void setName ( string name )
    {
        this->name = name;
    }


};

class DAOInterface
{

public:
    virtual list<Student*>& getAllStudents() = 0 ;
    virtual Student* getStudent ( int roll ) = 0;
    virtual void upDateStudent ( Student* student , string name ) = 0;
    virtual void deleteStudent ( Student* student ) = 0;


};

class DAOImpl : public DAOInterface
{
  public :

  list <Student*> students ;

  DAOImpl ()
  {
      
      Student* student1 = new Student ("Raj", 11 );
      Student* student2 = new Student ("Rohit", 14 );

      addStudent ( student1);
      addStudent ( student2 );

  }

  list<Student*>& getAllStudents ()
  {
      return this->students;
  }

  Student* getStudent ( int roll )
  {
      for ( Student* st : getAllStudents() )
      {
          if ( st->getRoll() == roll )
          {
              return st;
          }

      }
  }

  void upDateStudent ( Student* student , string name )
  {
      cout << " update student list for student name " << student->name << " with " << name << endl;
      auto it = find ( getAllStudents().begin(), getAllStudents().end(), student );
      (*it)->setName (name);
      cout << "updating  done "   << endl;
  }

  void deleteStudent ( Student* student )
  {
      cout << " deleting student " << " student name : " << student->name << endl;
      this->getAllStudents().remove ( student);
      delete student;
      
  } 

  void addStudent ( Student* student )
  {
      cout << "  add student " << " student name : " << student->name << endl;
      this->getAllStudents().push_back ( student );
  }

  void printDetails ( )
  {
      cout << " printDetails " << endl;
      for ( Student* st : students )
      {
          cout << " name --> " << st->getName() << " roll --> " << st->getRoll() << endl;
      }
  }

  
} ;

int main ()
{
    DAOImpl* dao = new DAOImpl;
    Student* student = dao->getStudent ( 11 );
    cout << " name : " << student->getName () << "  roll : " << student->getRoll() << endl;
    dao->addStudent ( new Student ( "Rakesh", 15 ));
    dao->printDetails();
    dao->deleteStudent ( student );
    dao->printDetails();
    dao->upDateStudent ( dao->getStudent (14), "puja");
    dao->printDetails ();
    return 0;
}