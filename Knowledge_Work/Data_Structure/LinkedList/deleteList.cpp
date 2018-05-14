#include <stdio.h>

#include <iostream>

struct Node
{

	int data;
	struct Node* next;
};


// Adding a node in the front 

void  push ( struct Node** headref, struct Node* newNode )
{
	printf ("Adding node with data : %d \n", newNode -> data );


	// to add a node Headpointer needs to be changes 
	// First old head pointer needs to be pointed of the 
	// next of newNode
	// Then newNode becomes the head pointer
	newNode -> next  = *headref;
	*headref = newNode;

	
}

// delete a list 

void deleteList ( struct Node** headref )
{

	printf ("Deleting the node \n" );

	struct Node* current = *headref;

	// To delete a node head pointer needs to change to next node during 
	// current node traversal and then delete current node 
	// Eventually all the nodes will be deleted

	while ( current  != NULL )
	{
	     *headref = current->next;
	     printf ( "deleting : %d \n", current ->data );
	    free ( current ) ;
	    current = current->next;
	}

   if ( *headref == NULL )
   {
	   printf ("head is NULL \n");
   }

}


void insertNth ( struct Node** headref, int position, int data )
{
	printf ("Adding data:  %d  in position : %d ", data, position );
	struct Node* current = *headref;

	// Traversing the currrent pointer to the positions previos node 
	

	for ( int i = 0; i < position - 1 ; i++ )
	{
		current = current->next;
	}

	struct Node* newNode = ( struct Node* ) malloc ( sizeof ( struct Node ) );
	newNode ->data = data;
	// Change the newNode next to the current node next
	newNode ->next = current -> next;
	// change the current Node to newNode
	current->next = newNode;
}

void printList ( struct Node** headref )
{
	struct Node* current = *headref;

	while ( current != NULL )
	{
		printf( "The data is : %d \n", current->data );
		current = current->next;
	}
}

// This function is used to insert the Node in a proper sorting order of a sorted list

void SortedMerge ( struct Node** headref, struct Node* newNode  )
{

	printf ("%s %d \n", __FUNCTION__ , __LINE__ );
	struct Node dummyNode;
	// result will hold dummyNode address , dummyNode -> next or result ->next 
	// will hold head pointer 
	struct Node* result  = &dummyNode;

	dummyNode.next = *headref;

	// Traversing through the loop and check if data of a newNode is more 
	// of current Node. The below loop changing will change the sorting 
	// order.
	// If the result ->next -> data . In the first iteration it will be head
	// data is less than newNode then traverse through the loop till it finds
	// node which is greater than newNode
	while ( result->next != NULL && result->next->data < newNode->data )
	{
		result = result->next;
	}

	// Currently result->next is pointing to node which is greater than Newnode
	// We have to insert newNode in the position of current result  

	newNode->next = result->next;
	result->next = newNode;

	// newList is created in result ( dummyNode.next ) . Now change the headref with
	// result 

	*headref = dummyNode.next;
       
	printf ("%s %d Added Node : %d \n", __FUNCTION__ , __LINE__, newNode ->data );

}

void MoveNode ( struct Node** destRef, struct Node** sourceRef )

{

	printf ("%s %d \n", __FUNCTION__ , __LINE__ );

	struct Node* newNode = *sourceRef;
	*sourceRef = newNode ->next;
	newNode ->next = *destRef;
	*destRef = newNode;

	printf ("%s %d Adding node : %d to the destination node  \n", __FUNCTION__ , __LINE__ , newNode->data );

}

// This function is used to sort a given linked list 
// This works by appending the result parameter 
struct Node*    SortInsert ( struct Node** headref  )
{
	printf ("%s %d \n", __FUNCTION__ , __LINE__ );


	struct Node* current = *headref;
	struct Node* result = NULL;
	// The below next pointer is imortant to hold the current pointer address
	// since it is going to be changed inside SortedMerge
	struct Node* next;
	// Traversing through the loop and updating the result pointer
	while (  current != NULL  )
	{
		// next pointer is necessary to hold current -> next address since current ->next will be changed with latest value 
		// inside sorteMerge
		next = current ->next;
		// result will be empty first and it will be added with values according to the order
		SortedMerge ( &result , current );
		current = next;
	}

	*headref = result;

	printf ("%s %d  sorted the list  \n", __FUNCTION__ , __LINE__ );
	return result;


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


	struct Node* head = NULL;


	for ( int i = 16; i > 1; i = i - 2 )
	{
		push ( &head, getNewNode ( i )  );
	}

//	printList ( &head );


	push ( &head, getNewNode ( 25 ) );
	push ( &head, getNewNode ( 3 )  );
	push ( &head, getNewNode ( 12 )  );
	push ( &head, getNewNode ( 5 )  );
	push ( &head, getNewNode ( 134 )  );

	//insertNth ( &head, 3, 4 );
	printList ( &head );


	struct Node*  newHead = SortInsert ( &head );
	printList ( &newHead );

	deleteList ( &head );

	return 0;
}
