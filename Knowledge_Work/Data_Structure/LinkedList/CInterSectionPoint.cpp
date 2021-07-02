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




int getCount ( struct Node* head1 )
{
	struct Node* current = head1;
    int count = 0;
	while ( current != NULL )
	{
       count++;

	   current = current->next;
	}

	return count ;
}

int _getInterSectionPoint ( int d, struct Node* head1, struct Node* head2 )
{
	struct Node* current1 = head1;

	struct Node* current2 = head2;

	for ( int i = 0; i < d; i++ )
	{
		if (current1 == NULL )
            return -1;

		current1= current1->next;
	}

	while ( current1 != NULL && current2 != NULL )
	{
		if ( current1 == current2)
		   return current2->data;

		   current2=current2->next;
		   current1 = current1->next;
	}

}

int  getInterSectionPoint ( struct Node* head1 , struct Node* head2 )
{
   int c1 = getCount (head1);

   int c2 = getCount ( head2 );

   int d = 0;

   if ( c1 > c2 )
   {
	   d = c1-c2;

	   return _getInterSectionPoint ( d, head1, head2 );
   }

   else
   {
	   d = c2-c1;

	   return _getInterSectionPoint ( d, head2, head1 );
   }
}







int main ()
{


struct Node* head1 = NULL;

struct Node* head2 = NULL;

head1 = getNewNode (1);

head2 = getNewNode (4);

head2->next = getNewNode (12);

head2->next->next = getNewNode (15);

head1->next = getNewNode (3);
head1->next->next = getNewNode (7);

 
struct Node* newNode = getNewNode (9);

head2->next->next->next = newNode;

head2->next->next->next->next = getNewNode (14);

head2->next->next->next->next->next = getNewNode (19);

head1->next->next->next = getNewNode (11);;

head1->next->next->next->next = newNode; 

head1->next->next->next->next->next = getNewNode (15);
head1->next->next->next->next->next->next= getNewNode (17);
head1->next->next->next->next->next->next->next= getNewNode (19);

//struct Node* newNode = getNewNode (17);

printf("printing head1\n");
printList (head1);

printf("printing head2\n");
printList (head2);




int intersectionPoint = getInterSectionPoint ( head1 ,head2 );

printf("intersection point is %d \n", intersectionPoint);





}