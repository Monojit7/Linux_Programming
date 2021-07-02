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

void deleteNodeByPosition ( struct Node** headref , int pos )
{
	struct Node* current = *headref;
	struct Node* prev = NULL;
     
	int offset = 0;
	if ( current != NULL && pos == offset )
	{
		*headref= current->next;
		free ( current );
		return ;
	}

    offset++;
	while ( current != NULL && offset <= pos )
	{
		prev = current;
		current = current->next;
		offset++;
		/* code */
	}

	if ( current == NULL ) return;

	prev->next = current->next;

	free(current );
	
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



// This function is used to get a new node with the mentioned data
struct Node* getNewNode ( int data )
{
	struct Node* newNode = ( struct Node* ) malloc ( sizeof ( struct Node ) );
	newNode -> data = data;
	newNode -> next = NULL;

	return newNode;
}

// delete a node of a linkedlist

void deleteNode ( struct Node** headref , int key )
{
	struct Node* current = *headref;
	struct Node* prev = *headref;

   // check if headNode has the key then change the head
	if ( current != NULL && current->data == key )
	{
		*headref = current->next;
		free (current);
		return;
	}

	// traverse till the key found and save the prev pointer
	while ( current != NULL && current->data != key )
	{
		prev = current;
		current = current->next;
	}

	if ( current == NULL )
	return;

   // make link with prev next with current next
	prev->next = current->next;

	free(current);
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

   // deleteNode (&head_first, 2);

	deleteNodeByPosition ( &head_first, 0 );

	printf("printing after delete Node \n");

  printList( head_first);

}