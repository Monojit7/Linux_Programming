#include <stdio.h>

#include <iostream>

#include <assert.h>

#include <string.h>

#include <bits/stdc++.h>

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

// This function is used to get a new node with the mentioned data
struct Node *getNewNode(int data)
{
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;

	return newNode;
}

//mergeing two sorted linkedlist in ascending order

// remove duplicates unsorted list

struct Node *SortedInsert(struct Node *a, struct Node *b)
{
	struct Node *tail = NULL;

	struct Node dummyNode;

	dummyNode.next = NULL;

	tail = &dummyNode;

	while (a != NULL && b != NULL)

	{
		if (a->data == b->data)
		{
			push(&tail->next, getNewNode(a->data));
			a = a->next;
			b = b->next;
		}
		else if (a->data < b->data)
		{
			push(&tail->next, getNewNode(a->data));
			a = a->next;
		}
		else
		{
			push(&tail->next, getNewNode(b->data));
			b = b->next;
		}

		tail = tail->next;
	}

	if (a == NULL)
		tail->next = b;

	if (b == NULL)
		tail->next = a;

	return dummyNode.next;
}

// remove duplicates of sorted list
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

void swap(struct Node *a, struct Node *b)
{
	Node *temp = a;
	a = b;
	b = temp;
}

void swapNodes2ndMethod(struct Node **headref, int x, int y)
{
	struct Node *current = *headref;

	struct Node *a = NULL;
	struct Node *b = NULL;

	while (current != NULL)
	{
		if (current->data == x)
		{
			a = current;
		}

		if (current->data == y)
		{
			b = current;
		}

		current = current->next;
	}

	if (a && b)
	{
		swap(a, b);
		swap(a->next, b->next);
	}
}

int main()
{

	struct Node *a = NULL;

	push(&a, getNewNode(14));
	push(&a, getNewNode(12));
	push(&a, getNewNode(8));
	push(&a, getNewNode(6));
	push(&a, getNewNode(4));
	push(&a, getNewNode(1));

	printf("Priniting a nodes list \n");
	printList(a);

	struct Node *b = NULL;

    push(&b, getNewNode(123));
	push(&b, getNewNode(16));
	push(&b, getNewNode(12));
	push(&b, getNewNode(6));
	push(&b, getNewNode(4));
	push(&b, getNewNode(3));
	push(&b, getNewNode(1));


	

	printf("Priniting b nodes list \n");
	printList(b);

	struct Node *result = SortedInsert(a, b);

	printf("Priniting sorted list  \n");
	printList(result);

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