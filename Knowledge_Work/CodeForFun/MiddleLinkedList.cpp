#include <stdio.h>

#include <iostream>

#include <assert.h>

#include <string.h>

using namespace std;

string toString(bool status);
using namespace std;
struct Node
{

	int data;
	struct Node *next;
};

// Adding a node in the front

void push(struct Node **headref, struct Node *newNode)
{
	printf("Adding node with data : %d new node %p headref %p \n", newNode->data, newNode, *headref);

	// to add a node Headpointer needs to be changes
	// First old head pointer needs to be pointed of the
	// next of newNode
	// Then newNode becomes the head pointer
	newNode->next = *headref;
	*headref = newNode;

	printf("Added node with data : %d newNode->next %p headref %p \n", newNode->data, newNode->next, *headref);
}

void printList(struct Node *headref)
{
	struct Node *current = headref;
	int pos = 0;
	while (current != NULL)
	{
		printf("The data is [%d ->] : %d \n", pos, current->data);
		current = current->next;
		pos++;
	}
}

//mergeing two sorted linkedlist in ascending order

int searchElement(struct Node **headref, int key)
{
	struct Node *current = *headref;

	int pos = 0;
	while (current != NULL)
	{
		printf("searchElement the value is %d \n", current->data);
		if (current->data == key)
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
struct Node *getNewNode(int data)
{
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;

	return newNode;
}

int findElementFromEnd(struct Node **headref, int nthFromEnd)
{
	struct Node *current = *headref;

	int length = 0;
	while (current != NULL)
	{
		length++;
		current = current->next;
	}

	printf("the length is %d \n ", length);
	if (length < nthFromEnd)
		return -1;

	int position = length - nthFromEnd + 1;

	current = *headref;

	for (int index = 1; index < position; index++)
	{
		current = current->next;
	}

	return current->data;
}

int ValueOfPosition(struct Node **headref, int position)
{
	struct Node *current = *headref;

	int index = 0;
	while (current != NULL)
	{
		if (index == position)
		{
			return current->data;
		}

		index++;

		current = current->next;
	}

	return -1;
}

int MiddleLinkedList(struct Node **headref)
{
	struct Node *slowptr = *headref;
	struct Node *fastptr = *headref;

	while (fastptr != NULL && fastptr->next != NULL)
	{
		slowptr = slowptr->next;
		fastptr = fastptr->next->next;
	}

	printf("The middle node value is %d \n", slowptr->data);
	return slowptr->data;
}

int detectLoop(struct Node **headref)
{
	struct Node *slowptr = *headref;
	struct Node *fastptr = *headref;
	while (fastptr != NULL && fastptr->next != NULL)
	{
		fastptr = fastptr->next->next;
		slowptr = slowptr->next;
		if (fastptr == slowptr)
		{
			return slowptr->data;
			/* code */
		}

		/* data */
	};

	return -1;
}

void reverse(struct Node **headref)
{
	struct Node *current = *headref;
	struct Node *next = *headref;
	struct Node *prev = NULL;

	while (current != NULL)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}

	*headref = prev;
}

bool CompareList(struct Node **headref, struct Node **headref2)
{
	struct Node *temp1 = *headref;
	struct Node *temp2 = *headref2;

	bool status = false;

	while (temp1 != NULL && temp2 != NULL)
	{
		if (temp1->data == temp2->data)
		{
			temp1 = temp1->next;
			temp2 = temp2->next;
		}
		else
			return false;
	}

	if (temp1 == NULL && temp2 == NULL)
		status = true;
	else
		status = false;

	return status;
}

string IsPalinDrome(struct Node **headref)
{
	struct Node *fast_ptr = *headref;
	struct Node *slow_ptr = *headref;
	struct Node *prev_of_slow_ptr = NULL;
	struct Node *MidNode = NULL;
	struct Node *secondHalf = NULL;

	while (fast_ptr != NULL && slow_ptr != NULL && fast_ptr->next != NULL)
	{
		fast_ptr = fast_ptr->next->next;
		prev_of_slow_ptr = slow_ptr;
		slow_ptr = slow_ptr->next;
	}

	// if number of Node is even then fast_ptr becomes NULL
	// else fast_ptr becomes non NULL

	if (fast_ptr != NULL)
	{
		MidNode = slow_ptr;
		// prev_of_slow_ptr->next = NULL;
		slow_ptr = slow_ptr->next;
	}

	secondHalf = slow_ptr;
	prev_of_slow_ptr->next = NULL;
	reverse(&secondHalf);

	bool res = CompareList(headref, &secondHalf);
	reverse(&secondHalf);

	if (MidNode != NULL)
	{
		prev_of_slow_ptr->next = MidNode;
		MidNode->next = secondHalf;
	}
	else
		prev_of_slow_ptr->next = secondHalf;

	return toString(res);
}

int CountNodesInLoop(struct Node **headref)
{
	struct Node *fastptr = *headref;
	struct Node *slowptr = *headref;

	int count = 0;

	while (fastptr && slowptr != NULL)
	{
		slowptr = slowptr->next;
		fastptr = fastptr->next->next;

		count++;

		if (slowptr == fastptr)
		{
			return count;
		}
	}

	return -1;
}

void removeDuplicates(struct Node **headref)
{
	struct Node *current = *headref;

	struct Node *next_next = NULL;

	if (current == NULL)
		return;

	while (current->next != NULL)
	{
		if (current->data == current->next->data)
		{
			next_next = current->next->next;
			free(current->next);
			current->next = next_next;
		}
		else
			current = current->next;
	}
}

string toString(bool status)
{
	return status ? "Pallindrome " : "Not pallindrome";
}

int main()
{

	struct Node *headref = NULL;

	push(&headref, getNewNode(1));
	push(&headref, getNewNode(2));
	push(&headref, getNewNode(2));
	push(&headref, getNewNode(2));
	push(&headref, getNewNode(1));

	//printf("xdf\n");
	printList(headref);
	// printf ( "linklist is %s \n",  IsPalinDrome(&headref).c_str());

	removeDuplicates(&headref);

	printf("LinkedList is %s \n", IsPalinDrome(&headref).c_str());
	printList(headref);

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