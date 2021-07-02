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








int main ()
{


 struct Node* headref = NULL;

 push (&headref, getNewNode (23));

 push (&headref, getNewNode(45));

 push (&headref, getNewNode (14));

 push (&headref, getNewNode(19));

 push (&headref, getNewNode (25));

 push (&headref, getNewNode(13));
 push (&headref, getNewNode (56));

 push (&headref, getNewNode(34));

 printf("printing list before sorting \n");

printList (headref);

QuickSortLinkedList (&headref);

printf("printing list after sorting \n");
printList (headref);



}