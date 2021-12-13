// C++ program to shuffle
// the given array
// using shuffle() method

#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <chrono>


using namespace std;

#define ENUM_TO_STR(ENUM) std::string(#ENUM)

#define QUESTION_COUNT 5

enum  eLIST
{
   VISITOR_PATTERN,
   FACTORY_PATTERN,
   OBSERVER_PATTERN,
   PROXY_PATTERN,
   STATE_PATTERN,
   STREATEGY_PATTERN,
   FILTER_PATTERN,
   FACADE_PATTERN,
   ADAPTER_PATTERN,
   BUILDER_PATTERN,
   BUSINESS_DELEGATE_PATTERN,
   CHAIN_OF_RESPONSIBILITY_PATTERN,
   COMPOSITE_PATTERN,
   DAO_PATTERN,
   BRIDGE_PATTERN,
   DECORATOR_PATTERN,
   INTERPRETOR_PATTERN,
   MOMENTO_PATTERN,
   NULLOBJECT_PATTERN,
   TEMPLATE_PATTERN,
   END_PATTERN

} ;

static const char* enum_string [] = 
{
   "VISITOR_PATTERN",
   "FACTORY_PATTERN",
   "OBSERVER_PATTERN",
   "PROXY_PATTERN",
   "STATE_PATTERN",
   "STREATEGY_PATTERN",
   "FILTER_PATTERN",
   "FACADE_PATTERN",
   "ADAPTER_PATTERN",
   "BUILDER_PATTERN",
   "BUSINESS_DELEGATE_PATTERN",
   "CHAIN_OF_RESPONSIBILITY_PATTERN",
   "COMPOSITE_PATTERN",
   "DAO_PATTERN",
   "BRIDGE_PATTERN",
   "DECORATOR_PATTERN",
   "INTERPRETOR_PATTERN",
   "MOMENTO_PATTERN",
   "NULLOBJECT_PATTERN",
   "TEMPLATE_PATTERN",
   "END_PATTERN"

};

string getStringEnum ( int val )
{
    string tmp ( enum_string [val]);
    return tmp;
}

// Shuffle array
void shuffle_array(int arr[], int total_Element , int total_sum_of_result )
{

     std::srand ( std::time (nullptr));
	// To obtain a time-based seed
	unsigned seed =  std::rand () % ( total_Element - 1 ) ;

	// Shuffling our array
	shuffle(arr, arr + total_Element,
			default_random_engine(seed));

	// Printing our array
	for (int i = 0; i < total_sum_of_result; ++i)
    {
        eLIST v = static_cast <eLIST > ( arr[i])  ;
		cout  << getStringEnum ( v ) << endl;
    }
	cout << endl;
}


void init_Array ( int a[], int max_value )
{
    for ( int i = 0 ; i < max_value; i++ )
    {
       a[i] = i;
    }
}

// Driver code
int main()
{

	
    
    int a [ END_PATTERN ] = {};
    init_Array ( a, END_PATTERN );

	//int n = sizeof(a) / sizeof(a[0]);

	shuffle_array(a, END_PATTERN , QUESTION_COUNT );

	return 0;
}
