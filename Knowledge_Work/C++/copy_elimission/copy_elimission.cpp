#include <iostream>
using namespace std;
 
class B
{
 
private:
 
const char* str;
public:
 
B( const char* str = 0 ): str(str)
{
 
cout << " B :: Constructor " << endl;
 
 
}
 
B( const B& ob)
{
 
cout << "Copy constructor is getting called " << endl;
 
}
 
string getStr() const
{
 
return this->str;
 
}
 
};
 
 
int main()
{
 
B ob = "Copy Me";
 
cout << "String Name : " << ob.getStr() << endl;
 
return 0;
}
