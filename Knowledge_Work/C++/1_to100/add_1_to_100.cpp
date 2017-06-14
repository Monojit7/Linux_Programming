#include <iostream>
using namespace std;

template<int n>
struct fun
{

enum 
{

val = n + fun<n-1>::val

};

fun()
{

cout << "fun constructor : val: " << val << endl;

}

};

template<>
struct fun<1>
{

enum
{

val =1

};

fun()
{

cout << "special argument " << val << endl;

}
};

template<>
struct fun<0>
{

enum
{
val = 0
};

fun()
{
cout <<"special argument :" << val << endl; 
}
};

int main()
{

fun<100> a;
static int sum = 0;
cout << "the value is   " << a.val << endl;
for ( int i = 1; i <=100; i++ )
{
 
sum = sum + i;


}

cout << "sum: " << sum << endl;
return 0;
}
