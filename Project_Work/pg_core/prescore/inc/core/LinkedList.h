/*********************************************************************
* Project         Presentation Control Core
* (c) copyright   2016
* Company         Harman International
*                 All rights reserved
*********************************************************************/
/**
* @file            LinkedList.h
* @ingroup         presctrlcore
* @author          Ankur Tibrewal (ankur.tibrewal@harman.com)
* @brief           Template based implementation of a simple linked list. 
*/


#ifndef _LINKED_LIST_
#define _LINKED_LIST_

template <class T>
class LinkedList
{
public : 

	LinkedList()
		:mpRoot(nullptr)
		, mpTail(nullptr)
		, mpCurrent(nullptr)
	{

	}

	void searchAndDestroy(T* data)
	{
		Node* pNode = mpRoot;
		while (nullptr != pNode)
		{
			if (data == pNode->mData)
			{
				deleteNode(pNode); 
				break;
			}

			pNode = pNode->mpNext; 
		}
	}

	void deleteCurrentAndSetNext()
	{
		if (nullptr != mpCurrent)
		{
			Node* tmp = mpCurrent->mpNext;
			deleteNode(mpCurrent);
			mpCurrent = tmp;
		}
	}

	void deleteCurrentAndSetPrev()
	{
		if (nullptr != mpCurrent)
		{
			Node* tmp = mpCurrent->mpPrev;
			deleteNode(mpCurrent);
			mpCurrent = tmp;
		}
	}


	/**
	* Clears and frees memory for all the nodes. Does not take care of deleting the data associated with the nodes. 
	* Cleaning up data is callers responsibility
	*/
	void clear()
	{
		Node* pNode = mpRoot; 
		while (nullptr != pNode)
		{
			Node* tmp = pNode->mpNext; 
			delete pNode; 
			pNode = tmp; 
		}
		mpRoot = nullptr; 
		mpTail = nullptr; 
		mpCurrent = nullptr; 
	}

	void initializeCurrent()
	{
		mpCurrent = nullptr;
	}

	/**
	*	Cyclic calls will return all elements starting from the head.
	*	Ensure before using that the current is set to Head. 
	*   On encountering last element, current will be set to null, and null will be returned. 
	*/
	T* peekNext()
	{
		T* data = nullptr; 
		if (nullptr != mpCurrent)
		{
			mpCurrent = mpCurrent->mpNext;
		}
		else
		{
			mpCurrent = mpRoot; 
		}

		if (nullptr != mpCurrent)
		{
			data = mpCurrent->mData;
		}

		return data; 
	}

	/**
	*	Cyclic calls will return all elements starting from the tail.
	*	Ensure before using that the current is set to Tail.
	*   On encountering last element, current will be set to null, and null will be returned.
	*/
	T* peekPrev()
	{
		T* data = nullptr;
		if (nullptr != mpCurrent)
		{
			mpCurrent = mpCurrent->mpPrev;
		}
		else
		{
			mpCurrent = mpTail;
		}

		if (nullptr != mpCurrent)
		{
			data = mpCurrent->mData;
		}

		return data;
	}

	void push_head(T* data)
	{
		Node * p = new Node(data);
		if (nullptr == mpRoot)
		{
			mpRoot = p;
			mpTail = p;
		}
		else
		{
			p->mpNext = mpRoot;
			mpRoot->mpPrev = p;
			mpRoot = p;
		}
	}

	void push_tail(T* data)
	{
		Node * p = new Node(data);
		if (nullptr == mpTail)
		{
			mpRoot = p;
			mpTail = p;
		}
		else
		{
			mpTail->mpNext = p;
			p->mpPrev = mpTail;
			mpTail = p;
		}
	}

	T* pop_head()
	{
		T* data = nullptr;

		if (nullptr != mpRoot)
		{
			data = mpRoot->mData;
			Node* tmp = mpRoot;
			mpRoot = mpRoot->mpNext;
			delete tmp;
			if (nullptr != mpRoot)
			{
				mpRoot->mpPrev = nullptr;
			}
			else
			{
				mpTail = nullptr;
			}
		}

		return data;
	}

	T* pop_tail()
	{
		T* data = nullptr;

		if (nullptr != mpTail)
		{
			data = mpTail->mData;
			Node* tmp = mpTail;
			mpTail = mpTail->mpPrev;
			delete tmp;
			if (nullptr != mpTail)
			{
				mpTail->mpNext = nullptr;
			}
			else
			{
				mpRoot = nullptr;
			}
		}

		return data;
	}


private : 

	struct Node
	{
		Node(T* data)
			: mData(data)
			,mpNext(nullptr)
			,mpPrev(nullptr)
		{

		}

		T* mData; 
		Node* mpNext; 
		Node* mpPrev;
	};


	void deleteNode(Node* pNode)
	{
		if (pNode == mpRoot)
		{
			// Handle deletion of root node. 
			mpRoot = mpRoot->mpNext;
			if (nullptr != mpRoot)
			{
				mpRoot->mpPrev = nullptr;
			}
			else
			{
				// Last node got deleted. 
				mpTail = nullptr;
			}

		}
		else if (pNode == mpTail)
		{
			// Handle deletion of last node. 
			mpTail = mpTail->mpPrev;
			if (nullptr != mpTail)
			{
				mpTail->mpNext = nullptr;
			}
			else
			{
				// Last node got deleted. 
				mpRoot = nullptr;
			}
		}
		else
		{
			//Somewhere in middle. 
			pNode->mpPrev->mpNext = pNode->mpNext;
			pNode->mpNext->mpPrev = pNode->mpPrev;
		}
		delete pNode;
		pNode = nullptr;
	}

	
	Node* mpRoot;
	Node* mpTail;
	Node* mpCurrent;
};

#endif
