#include <stdio.h>

#include <iostream>

#include <assert.h>

struct Node
{
	int data;
	struct Node *right;
	struct Node *down;
};

struct Node *getNewNode(int data)
{
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

	newNode->data = data;

	newNode->right = nullptr;

	newNode->down = nullptr;

	return newNode;
}

void insertAtEnd(struct Node **headref, int data)
{
	struct Node *current = *headref;

	struct Node *newNode = getNewNode(data);

	if (current == NULL)
	{
		newNode->down = *headref;
		*headref = newNode;

		return;
	}

	while (current->down != NULL)
	{
		current = current->down;
	}

	current->down = newNode;

	printf("node added %d \n", newNode->data);
}

void printList(struct Node *headref)
{
	struct Node *current = headref;

	int pos = 0;
	while (current != nullptr)
	{
		printf("[ %d ] -> %d \n", pos, current->data);
		current = current->down;
		pos++;
	}
}

Node*  mergeSort ( Node* a , Node* b )
{
	Node* result = nullptr;

	if ( a&& b )
	{
		if ( a->data < b->data )
		{
           result = a ;
		   a = a->down;
		}
		else
		{
           result = b;
		   b = b->down;
		}
	}

	while ( a && b )
	{
		if (a->data < b->data )
		{
			result->down = a ;
			result = a;
			a = result->down;
			

		}
		else
		{
			result->down = b;
			result = b ;
			b = result->down;
		}
	}

	if ( a == nullptr ) result->down = b;
	if ( b == nullptr ) result->down = a;
	return result;
}

struct Node *flattenList(struct Node *root)
{
	if (root == nullptr || root->right == nullptr)
		return root;

	return mergeSort(root, flattenList(root->right));
}

int main()
{
	struct Node *mainNode = nullptr;

	insertAtEnd(&mainNode, 5);
	insertAtEnd(&mainNode, 7);
	insertAtEnd(&mainNode, 20);
	insertAtEnd(&mainNode, 170);

	insertAtEnd(&(mainNode->right), 10);
	insertAtEnd(&(mainNode->right), 12);
	insertAtEnd(&(mainNode->right), 15);
	insertAtEnd(&(mainNode->right), 24);
	insertAtEnd(&(mainNode->right), 56);
	insertAtEnd(&(mainNode->right), 63);

	insertAtEnd(&(mainNode->right->right), 32);
	insertAtEnd(&(mainNode->right->right), 43);
	insertAtEnd(&(mainNode->right->right), 45);
	insertAtEnd(&(mainNode->right->right), 67);
	insertAtEnd(&(mainNode->right->right), 68);
	insertAtEnd(&(mainNode->right->right), 90);
	insertAtEnd(&(mainNode->right->right), 100);

	insertAtEnd(&(mainNode->right->right->right), 46);
	insertAtEnd(&(mainNode->right->right->right), 78);
	insertAtEnd(&(mainNode->right->right->right), 120);
	insertAtEnd(&(mainNode->right->right->right), 124);
	insertAtEnd(&(mainNode->right->right->right), 145);
	insertAtEnd(&(mainNode->right->right->right), 148);
	insertAtEnd(&(mainNode->right->right->right), 149);

	printf("printing before flattenlist \n");

	printList(mainNode);

	printList(mainNode->right->right->right);

	struct Node *result = flattenList(mainNode);

	printf("printing after flattenlist \n");

	printList(mainNode);
}