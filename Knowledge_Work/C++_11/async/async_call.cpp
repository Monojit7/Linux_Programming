/*

async call is a method invokation that will be executed in different thread.
It is a way to unblock the caller thread and execute the method in different 
thread.

Advantage:

i> Caller thread or function need not to wait for the result to continue other
   operation

*/






#include <iostream>
#include <string>
#include <algorithm>
#include <future>
#include <unistd.h>
using namespace std;

string reverse_string(const string& ss)
{

cout << " reverse string " <<endl;

sleep(5);

string s(ss);
reverse(s.begin(), s.end());

return s;

}

int main()
{

future<string> f = async(launch::async,reverse_string , "potpal");
auto g = async(launch::async, reverse_string , "elibom"); // returns std::future object which holds the result of the object
auto e = async(launch::async, reverse_string , "igadnij");

cout << "print1" << endl; 
cout << "method1: " << f.get() << endl; // f.get() will be blocked till the result received
cout << "method2: " << g.get() << endl;
cout << "method2: " << e.get() << endl;

cout << "print2" << endl; 

}
