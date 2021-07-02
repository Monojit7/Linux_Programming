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
    int pos = 0;
	while ( current != NULL )
	{
		printf( "The data is [%d ->] : %d \n",pos,  current->data );
		current = current->next;
		pos++;
	}
}

//mergeing two sorted linkedlist in ascending order



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

	int position = length - nthFromEnd + 1;

    current = *headref;

	for (int index = 1; index < position; index++ )
	{
		current = current->next;
	}

	return current->data;
	
}


int ValueOfPosition ( struct  Node** headref , int position )
{
	 struct Node* current = *headref;

    int index = 0;
	 while ( current != NULL )
	 {
	    if ( index == position )
		{
			return current->data;
		}	

			 index++;

	         current = current->next; 
	 }


  return -1;

	 
}

int main ()
{


	struct Node* head_first = NULL;
    struct Node* head_second = NULL;
    struct Node* result_sorted = NULL;



   
    printList( head_first );

    	for ( int i = 15; i > 0; i = i - 1 )
	{
		push ( &head_second, getNewNode ( i )  );
	}

    printList( head_second );

	printf ("The value of position %d \n", ValueOfPosition(&head_second , 7));

    int value = findElementFromEnd ( &head_second , 2 );

	printf ("The value is %d \n", value );
 


/*
  0 1 2 3 4 5 

  len = 6

  2nd Node from last = 3

  3rd Node from begin = 3

  len = 6

  nodeFrom Last = 2

  6-2-1
*/

}