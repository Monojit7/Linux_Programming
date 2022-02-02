#include <iostream>

using namespace std;

#define MAX 10

int size_s = 0;

struct stack
{
	int top = -1 ;

	int items [ MAX ] = {};
};



bool isFull ( stack* st )
{
	if ( st->top == ( MAX - 1 ) )
	return true;
	else
	return false;
}

bool isEmpty ( stack* st )
{
	if ( st->top == -1 )
	return true;
	else
	return false;
}

bool push ( stack* st, int newElement )
{
	bool status = false;
	if ( isFull ( st ))
	{
	   cout << "Stack is Full hence " << newElement << " can not be added " << endl;
	   status = false;
	}
	else
	{
		st->top++;
		st->items [ st->top ] = newElement;

		
		size_s++;
		cout << " new item has been added [ item : " <<  newElement << " ] top : -> " << st->top  << endl;
		status = true;
	}

	return status;
}

bool pop ( stack* st )
{
	bool status = false;

	if ( isEmpty (st))
	{
		status = false;
		cout << " Stack is Empty hence no items can be popped " << endl;
	}
	else
	{
		cout << " item : [ " << st->items [ st->top ] << " ] has been removed " << endl; 
		st->items [ st->top ] = 0;
		st->top--;
		size_s--;
		status = true;
	}

	return status;
}

void printStack ( stack* st )
{
	cout << "Below are the elements of the stack " << endl;
	for ( int i = 0 ; i <= size_s; i++ )
	{
		cout << " [ " << i << " ] --> " << st->items [ i ] << endl;
	}
}

// driver code

int main ()
{
	stack* st = ( stack*) malloc ( sizeof ( stack ));

	push ( st, 10 );

	push ( st, 20 );

	push ( st, 30 );

	push ( st, 40 );

	push ( st, 50 );

	push ( st, 60 );

	push ( st, 70 );

	push ( st, 80 );

	push ( st, 90 );

	push ( st, 98 );

	push ( st, 97 );


	printStack ( st );

	pop ( st );

	pop ( st );

	pop ( st );

	pop ( st );

	pop ( st );




   printStack ( st );

   pop ( st );

   pop ( st );

   pop ( st );

   pop ( st );

   pop ( st );

   pop ( st );

   pop ( st );

   pop ( st );

   pop ( st );

   pop ( st );


   printStack ( st );

   return 0;
	
}