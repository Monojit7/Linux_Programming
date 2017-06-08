#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
using namespace std;


class String
{

public:

char* str;
int size;
String(const char* str = '\0')
{
 
cout << "constructor : String" << endl;
size = strlen(str);
this->str = new char[size + 1];

strcpy(this->str, str);

}


void print()
{

cout << "The string is : " << this->str << endl;

}

void change (const char* str)
{

cout << "Changing the string from: " << this->str << " to" << str << endl;
delete [] (this->str);
size = strlen(str);

this->str = new char[size + 1];

strcpy(this->str, str);
}

String( const String& old_obj)
{

cout << "copying :" << old_obj.str << "object to the new object " << endl; 
size = strlen(old_obj.str);

this->str = new char[size +1 ];

strcpy( this->str, old_obj.str);

}
};


int main()
{

String S1 ("Monojit");
String S2 = S1;

S1.print();
S2.print();

S2.change("Mark Zuckerburg");
S1.print();// without copy constructor this will print "Mark Zuckerburg" ideally it should print "Monojit" Since S2.change() happened and it should 
          // not reflect S1.
S2.print();

return 0;
}
