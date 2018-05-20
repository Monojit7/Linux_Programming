#include <iostream>

using namespace std;


struct TreeNode
{

	public :
	int data;
	struct TreeNode* right;
	struct TreeNode* left;
	TreeNode ( int data , struct TreeNode* right = NULL, struct TreeNode* left = NULL  )
	{
		this->right = right ;
		this ->left = left ;
		this->data   = data;
	}
};

typedef enum TraversalType 
{
	PREORDER,
	INORDER,
	POSTORDER

}eTraversalType;


void printHelper ( struct TreeNode* node, eTraversalType mTraversalType ) ;
void print ( struct TreeNode* root, eTraversalType mTraversalType )
{
	printf ("calling %s \n", __FUNCTION__ );
	printHelper ( root , mTraversalType );
}


void printHelper ( struct TreeNode* node, eTraversalType mTraversalType  )
{
   if ( node != NULL )
	{
	
	switch ( mTraversalType )
	{	

          case POSTORDER :
		  {		  
	             printHelper ( node -> right , mTraversalType );
	             printHelper ( node -> left,  mTraversalType  );
	             printf ("POSTORDER %s <data : %d > \n", __FUNCTION__, node ->data ) ;
		     break;
		  }
          case INORDER :
		  {		  
	             printHelper ( node -> right, mTraversalType  );
	             printf ("INORDER %s <data : %d > \n", __FUNCTION__, node ->data ) ;
	             printHelper ( node -> left , mTraversalType );
		     break;
		  }
          case PREORDER :
		  {		  
	             printf (" PREORDER %s <data : %d > \n", __FUNCTION__, node ->data ) ;
	             printHelper ( node -> right, mTraversalType  );
	             printHelper ( node -> left, mTraversalType   );
		     break;
		  }
         }
	}
}


void add ( TreeNode*& node , int data )
{

	if ( node == NULL )
	{
		// If the root node is null or if we found the position where the current node 
		// to be added through left or right recursion , then this is the end of the 
		// recursion when node is added in the corresponding left or right side of a 
		// particular node .
		//
		// Since referrenece to pointer is used so actual node -> left or node -> right 
		// position will be updated with the node and node will not be free baloon 

		node = new TreeNode ( data );
	}
	else if ( node->data > data )
	{
		// Data to be added in the BST ( Binary Search Tree ) is less than current node data 
		// Node traversal must be in the left side of the tree
			add ( node->left , data );
	}
	else if ( node ->data < data )
	{
		// Data to be added in the BST ( Binary Search Tree ) is greater than the current node 
		// data then Node traversal must happen on the right side of the tree 
			add ( node->right , data );
	}
	else
	{
		// Data to be added is equal to the current node -> data , meaning data is duplicate
		// Duplicate data can not be added in any BST 
		
		printf ("Node can not be added : %d ", data );
	}
	

}


int getMinHelper ( TreeNode*& node )
{
	// BST ( Binary search Tree ) is sorted from left side and left most node 
	// of the BST is the minimum value 
	
	if ( node ->left == NULL )
	{
		return node -> data;
	}
	else
	{
		return getMinHelper ( node -> left );
	}

}

int getMin ( TreeNode*& node )
{
	if ( node == NULL )
	{
	}
	else
	{
		return getMinHelper ( node );
	}
}

void RemoveHelper ( TreeNode*& node , int data )
{
	printf ("Calling %s with data : %d \n", __FUNCTION__, data) ;
	if ( node == NULL )
	{
		// If node does not present in the BST or root node does not have 
		// anything, so doing nothing 
	}

	else 
	{
		if ( data < node->data  )
		{
			// if data is lesser than current node data during traversal then 
			// node to deleted must be presnent in the left side children list 
			// so move recursively in the left side children list 
			RemoveHelper ( node -> left , data );
		}
		else if ( data > node -> data )
		{
			// if data of the node to be deleted is greater than the current node then the 
			// data must be present in the right side children list of the current node
			RemoveHelper ( node ->right , data );
		}
		else
		{
			// Now the data is matches with the current node data so this is the node to be deleted
			// There are three condition handled below
			// Condition 1: current node ( node to be  deleted ) does not have any leaf 
			// Condition 2 : current node ( node to be deleted ) does either have right or left leaf
			// Condition 3 : Current node has both the leaf ( right and left leave )
			// Condition 3 is the trickest one in that we are promoting minimum value of the right side 
			// children list in the current node position 
			TreeNode *trash = NULL;
			if ( node -> left == NULL && node -> right == NULL )
			{
			
			     // When there is no left and right side leaf present and current node needs to be deleted 
			     // then delete current node since it is the leaf	
				trash = node ;
				node = NULL;
			}
			else if ( node -> left == NULL )
			{
				// When there is no left side leaf then promote right side leaf in the current node position
				// and delete the current node
				trash = node ;
				node = node -> right ;
			}
			else if ( node -> right == NULL )
			{
				// When there is no right side leaf and current node is the node to deleted
				// Then make promote node -> left in the current node position 
				// and delete the current node 
				trash = node ;

			        node = node ->left;
			}
			else
			{
				// This is the node we want to delete
				// We need to get the Minimum value from right side 
				// childrens and copy it to the current node   
			       int newData = getMin ( node ->right  );
			 
			     // copying the root node as current node and data as return of getMin 
			     // so that now getMin return will becone rootNode 
			     // and current rootNode content will be overwritten
               		       node -> data = newData;
			       // Now delete the duplicate leaf of the getMin node in the right children list
			       // To avoid duplicacy of the same node we remove that particular leaf node
			       // from the right side children  list  
			       RemoveHelper  ( node -> right , newData );

			}


			if ( trash != NULL )
			{
				delete trash;
			}

		}

	
	}
}

void Remove ( TreeNode*& node , int data )
{
	if ( node == NULL )
	{
	}
	else
	{
		RemoveHelper ( node , data );
	}
}
int main ()
{
	struct TreeNode* node = new TreeNode ( 50 );
	node -> left = new TreeNode ( 60 );
	node -> left -> left  = new TreeNode ( 15 );
	node -> left -> right   = new TreeNode ( 19 );
	node -> right = new TreeNode ( 90 );
	node -> right -> right = new TreeNode ( 17 );
	node -> right  -> left = new TreeNode ( 25 );

	struct TreeNode* newNode = NULL;
	add ( newNode , 20 );
	add ( newNode , 10 );
	add ( newNode , 24 );
	add ( newNode , 34 );
	add ( newNode , 12 );
	add ( newNode , 9 );
	add ( newNode , 13 );
	add ( newNode , 10 );
	printf ("printing the newNode contain :");
        print ( newNode, PREORDER ); 
        print ( newNode, INORDER ); 
        print ( newNode, POSTORDER );
       Remove ( newNode , 9 );

        print ( newNode, POSTORDER );
       Remove ( newNode , 20 );

        print ( newNode, POSTORDER );
}
