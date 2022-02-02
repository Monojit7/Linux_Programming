// This is priority queue  implementation concept taken from

// https://www.programiz.com/dsa/priority-queue

// this implementation is based on heap data structure and based on max heap

#include <iostream>
#include <vector>

using namespace std;

void swap ( int* a , int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
	
}

void heapify ( vector <int>& ht , int index )
{

	int size = ht.size();

    int largest = index ;

	int leftIndex = 2*index + 1 ;

	int rightIndex = 2*index + 2 ;

	if ( leftIndex < size && ht [leftIndex] > ht [largest ])
	{
		largest = leftIndex;
	}

	if ( rightIndex < size && ht [rightIndex] > ht [largest ])
	{
		largest = rightIndex;
	}

	if ( largest  != index )
	{
		swap ( &ht[largest], &ht[index]);

		heapify ( ht, largest );

	}

	



}


void enque ( vector <int>& ht , int element )
{
	int size = ht.size();

   if ( size == 0 )
   {
	ht.push_back ( element );
   }
	else
	{
     ht.push_back ( element );
	for ( int index = (size/2 -1 ); index >= 0 ; index-- )
	{
		heapify ( ht, index );
	}
	}
}

void deque ( vector <int>& ht , int element  )
{
	int size = ht.size();

	int pos = 0;

	for ( ; pos < size ; pos++)
	{
		if ( ht[pos] == element )
		break;

	}

	swap (&ht[ pos ], &ht[ size-1 ]);

    ht.pop_back();

	for ( int index = size/2 - 1 ; index >=0; index-- )
	{
		heapify ( ht, index );
	}

}

void printElements ( vector <int>& ht )
{
	for ( int it = 0 ; it < ht.size(); it++ )
	{
		cout << "    " << ht [it];
	}
	cout << endl;
}

int main ()
{
	vector <int> ht;// = { 1, 23, 45, 67, 89, 87, 90 };

    enque ( ht, 10 );

	enque ( ht, 17 );

	enque ( ht, 500 );

	enque ( ht, 9 );

	enque ( ht, 12 );

	enque ( ht, 15 );

	printElements ( ht );

	deque ( ht, 17 );

	deque ( ht, 12);

	printElements ( ht );



}