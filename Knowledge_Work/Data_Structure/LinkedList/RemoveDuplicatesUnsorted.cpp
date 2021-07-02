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

void removeDuplicatesUnsorted ( struct Node** headref )
{
	struct Node* ptr1 = *headref;

	struct Node* ptr2 = NULL;

	struct Node* dup = NULL;

	if (*headref == NULL ) return;

	while ( ptr1 != NULL && ptr1->next != NULL )
	{
		ptr2 = ptr1 ;

		while ( ptr2->next != NULL )
		{
			if ( ptr1->data == ptr2->next->data )
			{
                dup = ptr2->next->next;
				free ( ptr2->next);
				ptr2->next = dup;
			}
			else
			ptr2 = ptr2->next;
		}

		ptr1 = ptr1->next;
	}
	
}

//mergeing two sorted linkedlist in ascending order


// remove duplicates unsorted list

void removeDuplicatesUnsortedMethod2 ( struct Node** headref )
{
	struct Node* current = *headref;
	struct Node* prev = NULL;

	unordered_set <int> seen;

	while ( current != NULL && current->next != NULL)
	{
		if ( seen.find ( current->data ) != seen.end())
		{
			prev->next = current->next;
			free (current);
		}
		else
		{
			seen.insert (current->data);
			prev = current;
		}

		current= current->next;
	}
	
}

void swapNodes ( struct Node** headref , int x , int y )
{
	struct Node* CurrX = *headref;
	struct Node* CurrY = *headref;

	struct Node* PrevX = NULL;
	struct Node* PrevY = NULL;

    if ( x == y ) return ;
    
	while ( CurrX != NULL && CurrX->data != x )
	{
		PrevX = CurrX;
		CurrX = CurrX->next;
	}

	while (CurrY != NULL && CurrY->data != y )
	{
		PrevY = CurrY;
		CurrY = CurrY->next;
	}
	
	if ( CurrX == NULL || CurrY == NULL )
	return ;

	if ( PrevX != NULL )
	{
		PrevX->next = CurrY;

	}
	else
	{
	   *headref = CurrY;
	}

	if ( PrevY != NULL )
	{
		PrevY->next = CurrX;
	}
	else 
	{
		*headref = CurrX;
	}

   struct Node* temp = CurrX->next;
   CurrX->next = CurrY->next;
   CurrY->next = temp;
	


}


// This function is used to get a new node with the mentioned data
struct Node* getNewNode ( int data )
{
	struct Node* newNode = ( struct Node* ) malloc ( sizeof ( struct Node ) );
	newNode -> data = data;
	newNode -> next = NULL;

	return newNode;
}




// remove duplicates of sorted list 
void removeDuplicates ( struct Node** headref )
{
	struct Node* current = *headref;

	struct Node* next_next = NULL;

	if ( current == NULL ) return ;

	while ( current->next != NULL )
	{
         if ( current->data == current->next->data )
		 {
			 next_next = current->next->next;
			 free ( current->next);
			 current->next = next_next;
		 }
		 else
		 current = current->next;
	}
}





int main ()
{


    struct Node* headref = NULL;

   push (&headref, getNewNode (4));
   push (&headref, getNewNode (13));
   push (&headref, getNewNode (23));
   push (&headref, getNewNode (122));
   push (&headref, getNewNode (2));
   push (&headref, getNewNode (5));

  
    printf ("before swapping the nodes \n");
	printList(headref);

   printf ("after swapping the nodes \n");
	swapNodes (&headref, 13 , 2 );
   // printf ( "linklist is %s \n",  IsPalinDrome(&headref).c_str()); 

   printList(headref);

	removeDuplicatesUnsortedMethod2 ( &headref );

	printf ("after removing the duplicates \n");
	printList (headref);


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