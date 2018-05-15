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

	printf ("Calling %s and line : %d  \n", __FUNCTION__, __LINE__  );
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

// This function is a utility function which removes one node from sourceRef and 
// adding the same node to the destination.
// Example : sourceRef = {1, 2, 3, 4}
//           destRef = {3, 4, 5 }
//           MoveNode will make :
//           destRef = { 1, 3, 4, 5 }
//           sourceRf = { 2, 3, 4  }

void MoveNode ( struct Node** destRef, struct Node** sourceRef )

{

	printf ("%s %d \n", __FUNCTION__ , __LINE__ );

	struct Node* newNode = *sourceRef;
	assert ( newNode != NULL );
	*sourceRef = newNode ->next;
	newNode ->next = *destRef;
	*destRef = newNode;

	printf ("%s %d Adding node : %d to the destination node  \n", __FUNCTION__ , __LINE__ , newNode->data );

}

// This function is used to sort a given linked list 
// This works by appending the result parameter 
struct Node*    SortInsert ( struct Node** headref  )
{
	printf ("Calling %s and line : %d  \n", __FUNCTION__, __LINE__  );


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

	printf ("Calling %s and line : %d  \n", __FUNCTION__, __LINE__  );
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




// This function will split a linkedlist into two halves , frontRef will be having frontRef and
// backRef will hold the second halve of the list 
void SplitNode ( struct Node* source , struct Node** frontRef, struct Node** backRef )
{


	

	struct Node* current = source ;
	int count = 0;
	// Retriving the count of the linkedlist
	while ( current != NULL )
	{
		count++;
		current = current ->next;
	}


	// If the count is even then first halve will be count / 2 
	// If the count is odd then first halve will be ( count + 1 ) / 2
	
	if ( count % 2 != 0 )
	{
		count = count + 1;
	}

	struct Node* front = source;

	*frontRef = front ;
	// Moving through the end of first halve
	for ( int i = 0; i < (count /2 ); i++ )
	{

		front = front ->next;

	}

	// second halve start from next of end of first halve

	*backRef = front ->next;

	front->next = NULL;


}


// This function is used to add Node at the end of the linkedlist

void append ( struct Node** headRef, struct Node* newNode )
{
	if ( *headRef == NULL )
	{
		newNode ->next = *headRef;
		*headRef = newNode;
	}

	else
	{
		struct Node* current = *headRef;
  		while ( current -> next != NULL )
		{
			current = current -> next;
		}

		current ->next = newNode ;
	}
}


// This function is used to remove duplicate values if any of a sorted linked list

void RemoveDuplicates ( struct Node** headRef )
{

	printf ("Calling %s and line : %d  \n", __FUNCTION__, __LINE__  );
	struct Node* current = *headRef;
	struct Node* next_next = NULL ;
	while ( current -> next != NULL )
	{
		// In case of sorted linked list duplicate value present only to the next node
		if ( current -> data == current -> next ->data )
		{
			next_next = current -> next -> next;
			free ( current -> next );
			current -> next = next_next;

		}
		else
		{
			current = current -> next;
		}
	}

}


// This function is used to move one Node from source Node 
// and add the Same Node to the end of destination Node 
// Ex: sourceNode = { 1, 2, 3, 4 }
//     destinationNode = {2, 4, 5, 6 }
//     After MoveNode AtEnd
//     sourceNode = { 2, 3, 4  }
//     destinationNode = { 2, 4, 5, 6, 1} 

void MoveNodeAtEnd ( struct Node** destRef , struct Node** sourceRef )
{
	// If the destination does not any Node then adding 
	// new Node at the begining 
	if ( *destRef == NULL )
	{
		MoveNode ( destRef , sourceRef );
	}
	else
	{
		// If the destination node has nodes then
		// Moving to the end of the linkedlist 
		// and adding a node at the end
		struct Node* currentDest = *destRef;
		while ( currentDest -> next != NULL )
		{
			currentDest = currentDest -> next;
		}

		struct Node* newNode = *sourceRef;
		*sourceRef = newNode -> next;
		currentDest -> next = newNode;
		newNode ->next = NULL;
	}
}	

// This function is used to split the linked list in alternative order
// Example : source = { 2 , 4, 5, 6, 7 , 8 }
//  After AlternativeSplit 
//           aRef = { 2, 5, 7 }
//           bRef = { 4, 6, 8 }

void AlternativeSplit ( struct Node* source , struct Node** aRef, struct Node** bRef )
{

	printf ("Calling %s and line : %d  \n", __FUNCTION__, __LINE__  );
	struct Node* current = source ;
	while ( current != NULL )
	{
		// Move the node from beginning of current to end of aRef
		
		MoveNodeAtEnd ( aRef, &current );
		if ( current != NULL )
		{
			// Move the node from begining of current to end of bRef
			MoveNodeAtEnd ( bRef, &current ); 
		}
	}


}


// This function is used to merge two sorted linked list and result list also will be in sorted order
struct Node*       SortedMerge ( struct Node** aRef, struct Node** bRef   )
{

	struct Node* result = NULL;
	struct Node** listptrRef = &result ;

	// Below program works in a way by moving node one after another to result linkedlist
	// moving of node will be based on the value. Whichever value is lesser will be moved 
	// to the result list.
	//
	// In the process of moving either *aRef will be out of Node meaning will become NULL
	// or *bRef .
	//
	// When one of the linkedlist will be NULL , while ( 1 ) will be terminated
	
	while ( 1 )
	{
		if ( *aRef == NULL )
		{
			*listptrRef = *bRef;
			break;
		}
		else if ( *bRef == NULL )
		{
			*listptrRef  = *aRef;
			break;
		}

		else
		{
			if ( (*aRef ) -> data <= ( *bRef ) -> data )
			{
				// Moving value one after another from aRef to listptrRef
				MoveNode ( listptrRef , aRef );
			}
			else
			{

				// Moving value one after another from bRef to listptrRef
				MoveNode ( listptrRef , bRef );
			}

			// once one node is moved to listptrRef. listptrRef must go to the next node
			// to maintain the sorting order

			listptrRef = &(( *listptrRef ) -> next ); 
			
		}



	}

	
	return result;

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

	struct Node* frontNode = NULL;
	struct Node* backNode = NULL;
	SplitNode ( newHead, &frontNode, &backNode  );
	printf (" printing frontList  details \n"); 
	printList ( &frontNode );
	printf (" printing backList  details \n"); 
	printList ( &backNode );
	struct Node* firstSplit = NULL;
        struct Node* secondSplit = NULL;
	AlternativeSplit ( frontNode, &firstSplit, &secondSplit );

	printf (" printing firstSplit  details \n"); 
	printList ( &firstSplit );
	printf (" printing secondSplit  details \n"); 
	printList ( &secondSplit );
	struct Node* mergelist = SortedMerge ( &firstSplit, &secondSplit );
	printf (" printing mergelist  details \n"); 
	printList ( &mergelist );
	deleteList ( &head );

	deleteList ( &backNode );
	return 0;
}
