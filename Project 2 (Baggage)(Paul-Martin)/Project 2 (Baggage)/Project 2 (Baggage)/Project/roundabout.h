/*
 * Name: Paul Martin
 * Roundabout Class Definition
 * Date: 11/5/2024
 * Description: Roundabout template class definition declaring
 *				data members and member functions for type "T".
 */

#ifndef ROUNDABOUT_H
#define ROUNDABOUT_H

// Class Roundabout represents a circular data structure with a
// "current positions" that can be rotated in the forward or
// backward direction.
template<class T>
class Roundabout
{
public:
	// Default constructor.
	Roundabout();  // no size since can grow

	// Copy constructor.
	Roundabout(const Roundabout<T>& otherList);

	// Assignment operator.
	Roundabout<T>& operator =(const Roundabout<T>& otherList);

	// Destructor.
	~Roundabout();

	// Accessors

	// Return whether list is empty.
	bool isEmpty() const;

	// Get the value at current position in list (current position
	// is affected by rotating).
	T getCurrent() const;

	// Display all values in list to standard output, all on 1 line.
	// Start from current position.
	// Precondition: T must support insertion (<<) on an output stream.
	void output() const;

	// Mutators.

	// Insert a new value before current position in list.
	// Postcondition: New value becomes current position.
	void insert(const T& val);

	// Remove the value at current position (if any) in list.
	// Postcondition: Next position (after one removed) becomes
	// current position.
	void remove();

	// Rotate current position by 1 either in the forward (true)
	// or backward (false) direction.
	void rotate(bool forward);

	// Remove all values from list.
	void clear();

private:
	// Linked list node (lists to both previous and next nodes).
	struct Node
	{
		T info;
		Node* nextP;
		Node* prevP;
	};

	Node* currP;	  // point to "current" position
	int numElements;  // number of nodes in list
};

#endif
