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

enum eOrderType 
{
    ASCENDING,
    DESCENDING,
    INVALID
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

// write a program to merge two sorted linkedlist in reverse order

struct Node* SortedMerge ( struct Node** headref1, struct Node** headref2   )
{
	struct Node* list1 = *headref1;
	struct Node* list2 = *headref2;

	struct Node* result = NULL;

	while ( list1 != NULL && list2 != NULL )
	{
		if (list1->data <= list2->data )
		{
			push (&result, getNewNode (list1->data));
			list1 = list1->next;
		}
		else
		{
			push (&result, getNewNode (list2->data));
			list2 = list2->next;
		}
	}

	while ( list1 != NULL )
	{
		push ( &result, getNewNode (list1->data));
		list1 = list1->next;
	}

	while ( list2 != NULL )
	{
		push ( &result, getNewNode (list2->data));
		list2 = list2->next;
	}

	return result;
}

struct Node* reverseK ( struct Node*& current , int k )
{
	//struct Node* current = headref;
    int count = 0;

	struct Node* prev = NULL;

	struct Node* next = NULL;
	while ( current != NULL && count++ < k)
	{
		//printf ("count %d current node value is %d\n",count,  current->data);
        next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}

	return prev;
}


struct Node* reverseInGroups(Node* head, int k)
{
    // base case
    if (!head)
        return NULL;
    Node* current = head;
    Node* next = NULL;
    Node* prev = NULL;
    int count = 0;

	printf ("current data %d \n", current->data);
 
    /*reverse first k nodes of the linked list */
    while (current != NULL && count < k) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
        count++;
    }
 

    /* next is now a pointer to (k+1)th node
    Recursively call for the list starting from current.
    And make rest of the list as next of first node */
    if (next != NULL)
	{
		printf ("next data %d \n", next->data);
	    printf ("prev data %d \n", prev->data);
		printf ("head->data %d \n", head->data);
        head->next = reverseInGroups(next, k);
	}
 
 

    if (head->next != NULL )
	printf ("head->next data after recursion %d \n", head->next->data);

    if ( prev != NULL)
	printf ("prev data after recursion %d \n", prev->data);

    return prev;
}

Node* reverse ( Node*& current  , int k )
{
    Node* next = nullptr;
    Node* prev = nullptr;
    while ( current != nullptr && k-- )
    {
       next = current->next;
       current->next = prev;
       prev = current;
       current = next;
    }

    return prev;
}

Node* skipKthNode ( Node*& current , int k )
{
    Node* prev = nullptr;
    while ( current != nullptr && k--)
    {
        prev = current;
        current = current->next;
    }

    return prev;
}



Node* reverseInGroup ( Node** headref , int k )
{
    Node* current = *headref;
    Node* next = nullptr;
    Node* frontNode = nullptr;
    Node* lastNode = nullptr;
    Node* prev = nullptr;

    while ( current != NULL )
    {
       lastNode = current;
       frontNode = reverseK (current, k);

       if (prev == NULL)
       *headref = frontNode;
       else
       prev->next = frontNode;

       lastNode->next = current;

       prev = lastNode;
        



        
    }

    return *headref;
}

void swapData ( int& x , int& y)
{
    int temp = x;
    x= y;
    y= temp;
}

void rearrangeOddEven(Node* head)
{
    stack<Node*> odd;
    stack<Node*> even;
    int i = 1;
  
    while (head != nullptr) {
  
        if (head->data % 2 != 0 && i % 2 == 0) {
  
            // Odd Value in Even Position
            // Add pointer to current node

            printf ("head data %d and position %d \n", head->data , i );
            // in odd stack
            odd.push(head);
        }
  
        else if (head->data % 2 == 0 && i % 2 != 0) {
  
            // Even Value in Odd Position
            // Add pointer to current node
            // in even stack
            printf ("head data %d and position %d \n", head->data , i );
            even.push(head);
        }
  
        head = head->next;
        i++;
    }
  
    while (!odd.empty() && !even.empty()) {
  
        // Swap Data at the top of two stacks
        printf ("odd.top()->data  %d and even.top()->data %d \n", odd.top()->data , even.top()->data );
        swap(odd.top()->data, even.top()->data);
        odd.pop();
        even.pop();
    }
}

Node* reverseAlternatingKNodes ( struct Node** headref , int k )
{
    Node* current = *headref;

    Node* prev = nullptr;

    Node* frontNode = nullptr;

    Node* lastNode = nullptr;

    while ( current != nullptr )
    {
        lastNode = current;

        frontNode = reverse ( current , k);

        if ( prev == nullptr )
        {
            *headref = frontNode;
        }
        else
        {
            prev->next = frontNode;
        }


        lastNode->next = current;

        prev = skipKthNode ( current, k );
    }

    return *headref;


}


void deleteAlt ( struct Node** headref )
{

    if (*headref == NULL ) return;

    struct Node* current = *headref;

    struct Node* prev = NULL;

    struct Node* tmp = NULL;

    

    while ( current != NULL )
    {
        prev = current;
        tmp = current->next;

        prev->next = tmp->next;

        free (tmp);

        current = current->next;

    }
}

void insertAtEnd ( struct Node** headref, struct Node* node )
{
     printf ("Added at the end %d \n", node->data );
    if (*headref == NULL )
    {
        node->next = *headref;
        *headref = node ;
        return;
    }
    struct Node* current = *headref;

    while ( current->next != NULL )
    current = current->next;

    current->next = node;

   
    
}

void MoveAtBegin ( struct Node** dest, struct Node** source )
{
    struct Node* newNode = *source ;

    assert ( newNode != NULL );

    *source = newNode->next;

    newNode->next = *dest;

    *dest = newNode;

    
}

void MoveAtEnd ( struct Node** dest , struct Node** source )
{
    struct Node* newNode = *source;
    *source = newNode->next;
    if (*dest == NULL )
    {
        
        newNode->next = *dest;
        *dest = newNode;

        return;
    }

    struct Node* current = *dest;

    while ( current->next != NULL )
    {
        current = current->next;
    }

    current->next = newNode;
    newNode->next = NULL;

    
}

bool IsIdentical ( struct Node* a , struct Node* b )
{
    while ( a && b )
    {
        if ( a->data != b->data )
        {
            return false;
        }

        a = a->next;
        b = b->next;
    }

    return ( a == NULL && b == NULL );
}

void AlternatingSplits ( struct Node** source , struct Node** aRef, struct Node** bRef )
{
    if ( *source  == NULL )   return;
    struct Node* current = *source ;

    struct Node* a = NULL;

    struct Node* b = NULL;

    while ( current != NULL )
    {
        MoveAtEnd ( &a , &current );

        if ( current != NULL )
        {
            MoveAtEnd ( &b, &current );
        }


    }

    *aRef = a ;

    *bRef = b;

    *source = NULL;


}

void reverseList ( struct Node** headref )
{
    struct Node* current = *headref;

    struct Node* next = nullptr;

    struct Node* prev = nullptr;

    while ( current != NULL)

    {
        next = current->next;
        current->next = prev;
        prev= current;
        current = next;
    }

    *headref = prev;
    
}

void _deleteLesserNode ( struct Node* headref )
{
    struct Node* current = headref;

    struct Node* maxNode = headref;

    struct Node* tmp = NULL;

    while ( current != NULL && current->next != NULL )
    {
        if ( maxNode->data > current->next->data)
        {
            printf ("if : deleting node %d and maxNode data %d\n",  current->next->data, maxNode->data );
            tmp = current->next;
            current->next = tmp->next;
            free (tmp);
            current = current->next;
        }
        else
        {
             printf ("else : deleting node %d and maxNode data %d\n",  current->next->data, maxNode->data );
            current = current->next;
            maxNode = current;
        }

        
    }
}

void deleteLesserNode ( struct Node** headref )
{
    reverseList (headref);

    _deleteLesserNode (*headref);

    reverseList (headref);
}




struct Node* SumListOfTwoList ( struct Node** aRef, struct Node** bRef )
{

   struct Node* result = NULL;

   struct Node* a = *aRef;

   struct Node* b = *bRef;

   struct Node* temp = NULL;

   struct Node* prev = NULL;

   int sum = 0;

   int carry = 0;

   while ( a && b)
   {
       sum = carry + a->data + b->data ;

       carry = sum >= 10 ? 1 : 0;

       sum = sum % 10;

       temp = getNewNode ( sum );

       if ( result == NULL )
       result = temp;
       else
       prev->next = temp;

       prev = temp;

       a = a->next;
       b = b->next;




   }

   if ( carry > 0 )
   {
       temp->next = getNewNode ( carry);
   }
   

    return result ;


}

// write a function to delete a node when pointer to pointer can not be used
// return head pointer can not be used and list can not be empty


void deleteNode ( struct Node* headref, struct Node* n )
{
    if ( headref == n )
    {
        headref->data = headref->next->data;

        n = headref->next;

        headref->next = headref->next->next;

        free (n);
        return ;
    }

    struct Node* current = headref;

    while ( current->next != NULL && current->next != n)
    {
        current = current->next;
    }

    current->next = n->next;

    free (n);
    return ;
    
}


bool CheckOfSorting ( int data1, int data2 , eOrderType type )
{
    bool status = false;
   switch ( type )
   {
       case ASCENDING:
       status = data1 < data2 ? true : false ;
       break;

       case DESCENDING:

       status = data1 > data2 ? true : false;
       break;
   }

   return status;

}

void SortList ( struct Node** head, eOrderType type  )
{
    struct Node* current = *head;

    struct Node* next = nullptr;

    while ( current != nullptr && current->next != nullptr )
    {
        next = current->next;

        while ( next != nullptr  )
        {
            if (  CheckOfSorting ( next->data, current->data, type ))
            {
              int temp = next->data;
              next->data = current->data;
              current->data = temp;
            }

            next = next->next;
        }
        current= current->next;
        
    }
}

void findTriplet ( struct Node** a , struct Node** b, struct Node** c , int givenNum )
{
    struct Node* aRef = *a;

    int sum = 0;

    while ( aRef != nullptr )
    {
        struct Node* bRef = *b;
        struct Node* cRef = *c ;

        while ( bRef != nullptr && cRef != nullptr )
        {
        sum = aRef->data + bRef->data + cRef->data;

        if ( sum == givenNum )
        {
            printf ("Triplet is [ %d %d %d ] \n",  aRef->data, bRef->data, cRef->data);
            return;
        }

        if ( sum < givenNum )
        bRef = bRef->next;

        else if ( sum > givenNum )
        cRef = cRef->next;
        }

        aRef = aRef->next;
        
    }

    printf ("Triplet not found \n");
}


void rotateList ( struct Node** head , int k )
{
    struct Node* current = *head;

    struct Node* oldHead = *head;

    struct Node* kthNode = nullptr;

    struct Node* knextNode = nullptr;

    struct Node* lastNode = nullptr;

    int count = 0;
    while ( current->next != nullptr )
    {
        if ( count == k-1 )
        {
           kthNode = current;
           knextNode = current->next;
        }

        count++;
        current = current->next;
    }

    lastNode = current;

    kthNode->next = nullptr;

    lastNode->next = oldHead;

    *head = knextNode;

    


    


}

int main ()
{


struct Node* head1 = NULL;

struct Node* head2 = NULL;

struct Node* head3 = NULL;

struct Node* newNode = getNewNode (10);

insertAtEnd (&head1, getNewNode (10));
insertAtEnd (&head1, getNewNode (20));
insertAtEnd (&head1, getNewNode (30));
insertAtEnd (&head1, getNewNode (40));
insertAtEnd (&head1, getNewNode (50));
insertAtEnd (&head1, getNewNode (60));


printf ("Before rotating \n");
printList ( head1 );

rotateList ( &head1, 4);

printf ("After rotating \n");
printList ( head1 );

printf ("Before rotating \n");
printList ( head1 );

rotateList ( &head1, 3);

printf ("After rotating \n");
printList ( head1 );

rotateList ( &head1, 2);

printf ("After rotating \n");
printList ( head1 );









}