#include <iostream>
#include <thread>


using namespace std;

int main ()
{
   
  std::string s = "lambda expression" ;
   std:: thread t ( [] (  )
               {
                    cout << " thread function "  <<  endl;
               } ) ;

   t.join (); 
   
}
