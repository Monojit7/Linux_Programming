// This is circular queue implementation concept taken from

// https://www.programiz.com/dsa/circular-queue

#include <iostream>

using namespace std;

#define MAX_SIZE 6

struct sQueue
{
	int size = 0;

	int front = -1;

	int rear  = -1 ;

	int items [ MAX_SIZE ] = {};
};

bool isFull ( sQueue* sq )
{
	return (( ( sq->rear == MAX_SIZE - 1 ) && sq->front == 0 ) || ( sq->front == sq->rear + 1 )) ;
}

bool isEmpty ( sQueue* sq )
{
	return ( sq->front == -1 );
}

void enQueue ( sQueue* sq , int element )
{
	if ( isFull ( sq ))
	{
		cout << " Queue is Full so can not add element "  << element <<  endl;
	}
	else
	{
		if ( sq->front == -1 )
		{
			sq->front = 0;
		}

		sq->rear = ( sq->rear + 1 ) % MAX_SIZE;

		sq->items [ sq->rear ] = element;

		cout << "element added at " << sq->rear << " element -> " << element << endl;
	}
}


void deQueue ( sQueue* sq )
{
	if ( isEmpty ( sq ))
	{
		cout << "Queue is empty hence can not be dequeued " << endl;  
	}

	else
	{
		cout << "element deleted at "  << sq->front << " element -> " << sq->items [sq->front] << endl;

       // when there is only one element then front and rear is equal
		if ( sq->front == sq->rear )
		{
			sq->front = -1 ;
			sq->rear  = -1;
		}
		else
		{
			// to make the queue circular 
			sq->front =  ( sq->front + 1 ) % MAX_SIZE ;
		}

		
	}

}

	void printQueue ( sQueue* sq )
	{
		cout << "Below are the contents where [ front ->  " << sq->front << " ] and  [ rear -> " << sq->rear << " ]"  << endl;
		for ( int c = sq->front ; c != sq->rear ; c = ( c + 1 ) % MAX_SIZE )
		{
			cout << " position-> " << c << " element -> " << sq->items [c] << endl;
		}

        // since the above condition prints till sq-rear , hence need to print the value at sq->rear
		cout << "position -> " << sq->rear << " element -> " << sq->items [sq->rear] << endl;
	}

	void EmptyQueue ( sQueue* sq )
	{
		sq->front = -1;
		sq->rear  = -1 ;
	}


int main ()
{
	sQueue* sq = ( sQueue*) malloc ( sizeof (sQueue));

	EmptyQueue ( sq );

	enQueue ( sq, 10 );
	enQueue ( sq, 20 );
	enQueue ( sq, 30 );
	enQueue ( sq, 140 );
	enQueue ( sq, 50 );
	enQueue ( sq, 60 );
	enQueue ( sq, 70 );
	enQueue ( sq, 80 );
	enQueue ( sq, 90 );
	enQueue ( sq, 130 );

	deQueue ( sq );

	printQueue ( sq);

	enQueue ( sq, 56 );
	deQueue ( sq);
	printQueue ( sq);
	enQueue ( sq, 45 );
	enQueue  ( sq, 67 );
    printQueue ( sq);
	deQueue ( sq );
	deQueue ( sq );
	printQueue ( sq);



}