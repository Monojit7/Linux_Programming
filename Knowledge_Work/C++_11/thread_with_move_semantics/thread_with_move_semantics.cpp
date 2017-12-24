/*

 move semantics does transfer of RValue for "auto t" to the vector "workers".
 [ lambda function with RValue ] : Rvalue must be passed in operator [] of the functor lambda
 [ lambda functions with LValue ] : Lvalue must be passed in operator () of the functor lamda 
 [ for_each  ] : can be used when iterating through whole container . It takes functor as 
                 third parameter 

 [ assert ] :  ( header file <cassert> ) : assert function is used when the programmer needs to
                terminate the program when certain situation happend , mostly for debugging purpose
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <cassert>

using namespace std;



int main ()
{
   
    vector < thread > workers;
    
    for ( int i = 0; i < 5 ; i++ )
        {
            
            auto t = thread ( [ i ] 
                                  {
                                    
                                      cout << "thread function " << i << endl;
                                    
                                  } );

        workers .push_back ( move ( t ) );        
          
        }   


   for_each ( workers.begin () , workers.end () , [] ( thread & t )
            {
                
             cout << "[ thread id ] :" << t.get_id () << " [ total thread supported ] : " << thread::hardware_concurrency( ) << endl;
 
         
                assert ( t.joinable ()  &&  "assert failed : thread not joinable " );
                       t.join () ;
            });

}



