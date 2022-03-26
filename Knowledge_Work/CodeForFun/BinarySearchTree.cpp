#include <iostream>

using namespace std;

struct Node
{
	int key;
	struct Node *left;
	struct Node *right;
};

void inorder(struct Node *root)
{
	if (root != NULL)
	{
		inorder(root->left);

		cout << "  " << root->key;

		inorder(root->right);
	}
}

struct Node *newNode(int key)
{
	struct Node *node = (struct Node *)malloc(sizeof(struct Node));
	node->key = key;
	node->left = NULL;
	node->right = NULL;

	return node;
}

struct Node *minValueNode(struct Node *root)
{
	struct Node *current = root;

	// left most node will be having minimum value
	while (current != NULL && current->left != NULL)
	{
		current = current->left;
	}

	return current;
}

struct Node *insert(struct Node *root, int key)
{
	if (root == NULL)
		return newNode(key);

	if (key < root->key)
	{
		root->left = insert(root->left, key);
	}
	else if (key > root->key)
	{
		root->right = insert(root->right, key);
	}

	return root;
}

struct Node *DeleteNode(struct Node *root, int key)
{
	if (root == NULL)
		return root;

	if (key < root->key)
	{
		root->left = DeleteNode(root->left, key);
	}
	else if (key > root->key)
	{
		root->right = DeleteNode(root->right, key);
	}
	else
	{

		if (root->left == NULL && root->right == NULL)
		{
			free(root);
			return NULL;
		}
		else
		{

			if (root->left == NULL)
			{
				struct Node *temp = root->right;
				free(root);
				return temp;
			}
			else if (root->right == NULL)
			{
				struct Node *temp = root->left;
				free(root);
				return temp;
			}
		}

		// fetching the inroder successor
		struct Node *temp = minValueNode(root->right);

		root->key = temp->key;

		root->right = DeleteNode(root->right, temp->key);
	}

	return root;
}

int main()
{
	struct Node *root = NULL;

	root = insert(root, 8);
	root = insert(root, 3);
	root = insert(root, 1);
	root = insert(root, 6);
	root = insert(root, 7);
	root = insert(root, 10);
	root = insert(root, 14);
	root = insert(root, 4);

	cout << "Inorder Traversal : ";

	inorder(root);

	cout << endl;

	cout << "After deleting 3: ";

	root = DeleteNode(root, 10);

	cout << "Inorder Traversal : ";

	inorder(root);
	cout << endl;
}