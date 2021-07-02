#include <stdio.h>

#include <iostream>

#include <assert.h>

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

	while ( current != NULL )
	{
		printf( "The data is : %d \n", current->data );
		current = current->next;
	}
}

//mergeing two sorted linkedlist in ascending order

Node* MergeSorting ( Node* p , Node* q , Node* sorting )
{
	Node* result = NULL;
	if ( p == NULL ) return q;
	if ( q == NULL ) return p;

	if ( p && q )
	{
		if ( p-> data <= q->data )
		{
			sorting = p;
			p = p->next;
		}
		else{
			sorting = q;
			q = q->next;
		}
	}

	result = sorting;

	while ( p && q )
	{
		if ( p->data <= q->data )
		{
            sorting->next = p;
			sorting= p;
			p = sorting->next;
			

		}
		else
		{
            sorting->next = q;
			sorting = q;
			q = sorting->next;
		}
	}

	if ( p == NULL ) sorting ->next = q;
	if ( q == NULL ) sorting->next = p;

	return result;
}

int searchElement ( struct Node** headref, int key )
{
	struct Node* current = *headref;

   int pos = 0;
	while ( current != NULL )
	{
		printf ("searchElement the value is %d \n", current->data);
		if ( current->data == key )
		{
			printf("searchElement The position key %d and pos %d \n", key, pos);
			return pos;
		}

		pos++;

		current = current->next;
	}

	return -1;
}

// This function is used to get a new node with the mentioned data
struct Node* getNewNode ( int data )
{
	struct Node* newNode = ( struct Node* ) malloc ( sizeof ( struct Node ) );
	newNode -> data = data;
	newNode -> next = NULL;

	return newNode;
}

int findElementFromEnd ( struct Node** headref , int nthFromEnd )
{
	struct Node* current = *headref;

   int length = 0;
	while (current != NULL)
	{
		length++;
		current = current->next;
	}

    printf ("the length is %d \n ", length );
	if ( length < nthFromEnd ) return -1;

	int position = length - nthFromEnd;

	for (int index = 0; index <= position; index++ )
	{
		current = current->next;
	}

	return current->data;
	
}

int main ()
{


	struct Node* head_first = NULL;
    struct Node* head_second = NULL;
    struct Node* result_sorted = NULL;


	for ( int i = 16; i > 0; i = i - 2 )
	{
		push ( &head_first, getNewNode ( i )  );
	}
   
    printList( head_first );

    	for ( int i = 15; i > 0; i = i - 2 )
	{
		push ( &head_second, getNewNode ( i )  );
	}

    printList( head_second );

  Node *result = MergeSorting (head_first , head_second , result_sorted );

  printList( result );

  int key = 7;
  int pos = searchElement (  &head_second, key);

  printf ("The position of key %d is %d \n", key, pos );

}