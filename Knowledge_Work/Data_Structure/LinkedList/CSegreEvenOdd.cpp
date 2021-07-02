#include <stdio.h>

#include <iostream>

#include <assert.h>

#include <string.h>

#include <bits/stdc++.h>

using namespace std;


string toString ( bool status );
using namespace std;
struct Node
{

	int data;
	struct Node* next;
};


// Adding a node in the front 

void  push ( struct Node** headref, struct Node* newNode )
{
	printf ("Adding node with data : %d new node %p headref %p \n", newNode -> data, newNode, *headref );


	// to add a node Headpointer needs to be changes 
	// First old head pointer needs to be pointed of the 
	// next of newNode
	// Then newNode becomes the head pointer
	newNode -> next  = *headref;
	*headref = newNode;

	printf ("Added node with data : %d newNode->next %p headref %p \n", newNode -> data, newNode ->next, *headref );

	
}

void printList ( struct Node* headref )
{
	struct Node* current = headref;
    int pos = 0;
	while ( current != NULL )
	{
		printf( "The data is [%d ->] : %d \n",pos,  current->data );
		current = current->next;
		pos++;
	}
}



// This function is used to get a new node with the mentioned data
struct Node* getNewNode ( int data )
{
	struct Node* newNode = ( struct Node* ) malloc ( sizeof ( struct Node ) );
	newNode -> data = data;
	newNode -> next = NULL;

	return newNode;
}




//mergeing two sorted linkedlist in ascending order


// remove duplicates unsorted list



// quick sort

void QuickSortLinkedList ( struct Node** headref )
{
	struct Node* current = *headref;

	struct Node* next = NULL;

	while ( current != NULL )
	{
       next = current->next;
      while ( next != NULL )
	  {
			if ( current->data > next->data )
			{
			printf ("current->data %d , next->data %d \n", current->data, next->data);
			int temp = current->data;
			current->data = next->data;
			next->data = temp;
			}
			next = next->next;
	  }

	  current = current->next;
	  

	}
}


void SegregateEvenOdd ( struct Node** headref )
{
	struct Node* current = *headref;

	struct Node* OddHead = NULL;

	struct Node* lastOdd = NULL;

	struct Node* EvenHead = NULL;

	struct Node* lastEven = NULL;

	while ( current != NULL )
	{
		if ( current->data % 2 != 0)
		{
            if ( OddHead == NULL )
			{
				OddHead = current;
				lastOdd = current;
			}
			else
			{
				lastOdd->next = current;
				lastOdd = current;
			}
		}
		else
		{
			if ( EvenHead == NULL )
			{
				EvenHead = current;
				lastEven = current;
			}
			else
			{
				lastEven->next = current;
				lastEven = current;
			}

		}

		current = current->next;
	}

	if ( EvenHead != NULL )
	{
		*headref = EvenHead;
	}

	if ( lastEven != NULL )
	{
		lastEven->next = OddHead;
	}

	if ( lastOdd != NULL )
	{
		lastOdd->next = NULL;
	}


}


void printReverse ( struct Node* headref )
{

	struct Node* current = headref;
   if ( current == NULL )	return;

   printReverse ( current->next );

    printf (" -> %d \n", current->data );

}








int main ()
{


 struct Node* headref = NULL;

 push (&headref, getNewNode (23));

 push (&headref, getNewNode(45));

 push (&headref, getNewNode (9));

 push (&headref, getNewNode(20));

 push (&headref, getNewNode (25));

 push (&headref, getNewNode(13));
 push (&headref, getNewNode (11));

 push (&headref, getNewNode(7));

 printf("printing list before segreatig Even Odd  \n");

printList (headref);

//QuickSortLinkedList (&headref);

SegregateEvenOdd (&headref);

printf("printing list after segreatig Even Odd  \n");
printList (headref);

printf("printing list in reverse order \n");
printReverse (headref);

printf("end \n");

}