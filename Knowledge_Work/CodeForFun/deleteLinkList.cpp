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



void deleteLinkedList ( struct Node** headref )
{
	struct Node* current = *headref;

	while (current != NULL )
	{
		current->next = current->next->next;
		free (current);
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

int main ()
{


	struct Node* head_first = NULL;
    struct Node* head_second = NULL;
    struct Node* result_sorted = NULL;


	for ( int i = 16; i > 0; i = i - 2 )
	{
		push ( &head_first, getNewNode ( i )  );
	}
   

    deleteLinkedList(&head_first);
    printList( head_first );

  

}