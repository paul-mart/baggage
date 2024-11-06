/*Roundabout Member Function Definitions
 * Date: 11/5/2024
 * Description: Roundabout member function definitions.
 *				Circular singly-linked list implementation, keeping track
 *				of list with a pointer to the current node in list.
 */

#ifndef ROUNDABOUT_CPP
#define ROUNDABOUT_CPP

#include <iostream>
#include "roundabout.h"
using namespace std;

// Roundabout member function definitions.

// Default constructor.
template<class T>
Roundabout<T>::Roundabout()
{
	// Start out empty.
	currP = NULL;
	numElements = 0;
}

// Copy constructor.
template<class T>
Roundabout<T>::Roundabout(const Roundabout<T>& otherList)
{
	// Start out empty.
	currP = NULL;
	numElements = otherList.numElements;

	// Copy values from other list.
	append(otherList);
}

// Destructor.
template<class T>
Roundabout<T>::~Roundabout()
{
	clear();  // pointer/count reset once cleared
}

// Assignment operator.
template<class T>
Roundabout<T>& Roundabout<T>::operator =(const Roundabout<T>& otherList)
{
	// Avoid copying if assigning object to self.
	if (this != &otherList)
	{
		// Empty current list.
		clear();

		// Copy values from other list.
		append(otherList);
	}

	// Return self.
	return *this;
}

// Accessors.

// Return whether list is empty.
template<class T>
bool Roundabout<T>::isEmpty() const
{
	return currP == NULL;
}

// Get the value at current position in list (current position
// is affected by rotating).
template<class T>
T Roundabout<T>::getCurrent() const
{
	if (currP == NULL)
		return T();
	return currP->info;
}

// Display all values in list to standard output, all on 1 line.
// Start from current position.
// Precondition: T must support insertion (<<) on an output stream.
template<class T>
void Roundabout<T>::output() const
{
	// NULL should only be encountered when list is empty
	// (since circular).
	if (currP != NULL)
	{
		Node *traverseP = currP;

		// Stop when processed as many nodes as in list.
		for (int count = 0; count < numElements; count++)
		{
			cout << " " << traverseP->info;
			traverseP = traverseP->nextP;
		}
	}
}


// Mutators

// Insert a new value before current position in list.
// Postcondition: New value becomes current position.
template<class T>
void Roundabout<T>::insert(const T& val)
{
	//// FILL IN FUNCTION.
	Node* newNode = new Node{ val, nullptr, nullptr }; //use constructor

	if (numElements == 0) { //if no nodes
		newNode->prevP = newNode; //pointers both just point to own node
		newNode->nextP = newNode;
	}
	else {

		newNode->prevP = currP->prevP; //newNodes previous pointer pointers to currP->Prevp
		newNode->nextP = currP; //newNode nextp point to currP
		currP->prevP->nextP = newNode; //currP prevp->nextP point to newNode, keep the newNode linked in circular list
		currP->prevP = newNode; //currP prevP points to new node, since it inserted before currP before it becomes currP

	}
	currP = newNode; //newNode becomes currP regardless of node numbers

	numElements++; //added element
}

// Remove the value at current position (if any) in list.
// Postcondition: Next position (after one removed) becomes
// current position.
template<class T>
void Roundabout<T>::remove()
{
	// FILL IN FUNCTION.

	if (numElements == 0) //if list empty, nothing to remove
		return; 
	else if (numElements == 1) //1 elements
	{
		Node* tempCurrent = currP;
		//prevent dangling pointers
		tempCurrent->nextP = NULL;
		tempCurrent->prevP = NULL;
		delete tempCurrent; //deallocate the only node
		currP = NULL; //prevent dangling pointer

	}
	else {

		currP->prevP->nextP = currP->nextP; //point the nextP of the previos node to the nextP after current
		currP->nextP->prevP = currP->prevP; //point the prevP of the next node to the previousP of the current node
		Node* tempCurrent = currP; //temp current P
		currP = currP->nextP; //next node is now current

		//prevent dangling pointers
		tempCurrent->nextP = NULL;
		tempCurrent->prevP = NULL;

		delete tempCurrent; //deallocate temp pointer

		//prevent dangling pointer
		tempCurrent = NULL;
		
	}
	numElements--; //1 less element
}

// Rotate current position by 1 either in the forward (true)
// or backward (false) direction.
template<class T>
void Roundabout<T>::rotate(bool forward)
{
	if (currP != NULL)
		if (forward)
			currP = currP->nextP;
		else
			currP = currP->prevP;
}

// Remove all values from list.
template<class T>
void Roundabout<T>::clear()
{
	

	while (numElements != 0) //while there is still elements
	{

		
		Node* tempCurrent = currP; //placeholder for the current currP spot 
		if (numElements == 1) //need special case because if there is one node, assigning currP = currP->nextP like seen later wont work correctly
		{
			//remove the last node and get rid of tempcurrent
			tempCurrent->info = NULL;
			tempCurrent->prevP = NULL;
			tempCurrent->nextP = NULL;
			delete tempCurrent; //deallocate
			tempCurrent = NULL; //prevent dangling ptr
			numElements--; //one last element
		}
		currP = currP->nextP; //move currP along to the next node, we will use this to move on after clearing current node with tempCurrent

		//set nodes pointers and info to null, prevP and nextP will be deleted with the node later so we must prevent dangling pointers now
		tempCurrent->info = NULL;
		tempCurrent->prevP = NULL;
		tempCurrent->nextP = NULL;

		delete tempCurrent; //deallocate node
		tempCurrent = NULL; //prevent dangling pointers
		tempCurrent = currP;
		numElements--; //one less node
		
	}

	currP = NULL; //currP set to null
}

#endif
