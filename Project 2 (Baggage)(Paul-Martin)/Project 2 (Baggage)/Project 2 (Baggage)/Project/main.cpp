/*
 * Name: Paul Martin
 * Baggage Project
 * Date: 11/5/2024
 * Description: Remove baggage from an airplane and move it to a carousel
 *				at which passengers (customers) may retrieve their baggage.
 *				There is only 1 piece of baggage per passenger.  A ticket #
 *				is assigned to each piece of baggage and the same ticket #
 *				is given to the passenger who owns the baggage.  Baggage from
 *				the plane is stacked onto a set of loaders in the random
 *				order it comes off the plane.  Then, the loaders are unloaded
 *				and baggage is sent up to the carousel.  Passengers disembark
 *				in their own random order and wait in a single queue to
 *				retrieve baggage.  The queue only advances when the first
 *				passenger in the queue retrieves their baggage.  Time is
 *				counted in minutes and seconds during baggage retrieval.
 *				The carousel is shut down after a certain number of minutes.
 *				The user provides the number of passengers, the size of the
 *				carousel (its baggage capacity), and for how many minutes
 *				the carousel will operate.
 * 
 */

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>  // for rand()
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>  // for random_shuffle()
#include "roundabout.cpp"  // for Roundabout template class
using namespace std;

// Constants used throughout program.

// Since all baggage will have a positive ticket #, zero is used
// to represent no baggage.
const int NO_BAGGAGE = 0;

// Declaration for functions that process baggage and passengers.

// Get next piece of baggage off of loader.  Loaders are
// emptied starting with the first (index 0) through the
// last.  If all loaders are empty, will return NO_BAGGAGE.
int getNextBaggage(stack<int> trucks[], int numTrucks);

void displayCustomer(queue<int>& customer); //function to display customers in queue waiting for baggage

int main()
{
	// Maximum pieces of baggage on a single loader.
	const int LOADER_CAPACITY = 10;

	// Number of loaders available.
	const int NUM_LOADERS = 5;

	// Seed random number generator to randomize baggage and
	// passenger order.
	//srand((unsigned int)time(NULL));

	// Allow user to enter # of passengers on the plane.
	// This cannot exceed the capacity of the loaders.

	const int MAX_PASSENGERS = LOADER_CAPACITY * NUM_LOADERS;
	int numPassengers;
	
	do
	{
		cout << "\nEnter # of passengers (1 to " << MAX_PASSENGERS << "): ";
		cin >> numPassengers;
	} while (numPassengers < 1 || numPassengers > MAX_PASSENGERS);

	// Allow user to enter the # of pieces of baggage (suitcases)
	// that fit on the carousel.  Must be at least as many as the
	// passengers; otherwise, deadlock may occur since only the
	// passenger at the front of the queue can retrieve baggage.
	int carouselSize;
	do
	{
		cout << "\nEnter # of suitcases that fit on carousel (at least as many as passengers): ";
		cin >> carouselSize;
	} while (carouselSize < numPassengers);

	// Allow user to enter the # of minutes to unload baggage
	// and allow retrieval (other planes are coming)!
	int totalMinutes;
	do
	{
		cout << "\nEnter total minutes to get baggage (at least 1): ";
		cin >> totalMinutes;
	} while (totalMinutes < 1);

	cin.ignore();  // eat leftover newline from last input

	// Create vector with sequential (positive) ticket #'s for all
	// passengers' baggage.
	vector<int> tickets(numPassengers);
	for (int ticketIdx = 0; ticketIdx < numPassengers; ticketIdx++)
		tickets[ticketIdx] = ticketIdx + 1;

	// Randomize the ticket numbers to represent order in which baggage
	// is unloaded from the plane.
	random_shuffle(tickets.begin(), tickets.end());

	// Create set of loaders onto which baggage from the plane is then
	// stacked in order from the first loader to the last.
	
	stack<int> loaders[NUM_LOADERS];

	int loaderNum = 0;

	for (int baggageNum = 0; baggageNum < numPassengers; baggageNum++)
	{
		// If current loader is full, go to next one.
		// Note: Cannot run out of loaders since already limited
		// # of passengers (and therefore baggage) to prevent that.
		if (loaders[loaderNum].size() == LOADER_CAPACITY)
		{
			cout << "Loader #" << (loaderNum + 1) << " full, ";
			loaderNum++;
			cout << "filling loader #" << (loaderNum + 1) << endl;
		}

		// Place baggage on top of loader.
		loaders[loaderNum].push(tickets[baggageNum]);
	}

	// Create baggage carousel of requested size with all positions
	// initially empty (no baggage on carousel yet).
	Roundabout<int> carousel;
	for (int carouselPos = 0; carouselPos < carouselSize; carouselPos++)
		carousel.insert(NO_BAGGAGE);

	
	// Randomize the ticket numbers to represent order in which passengers
	// get off the plane and into the queue to retrieve baggage.
	random_shuffle(tickets.begin(), tickets.end());
	queue<int> passengers;
	int passengerNum = 0;

	// Simulate the operation of the carousel over a series of minutes.
	// Each second another piece of baggage arrives on the carousel
	// (if room at the retrieval point).  Only the passenger at the
	// front of the queue may retrieve their baggage when it arrives
	// at the retrieval point.

	const int SECS_IN_MIN = 60;

	for (int mins = 0; mins < totalMinutes; mins++)
		for (int secs = 0; secs < SECS_IN_MIN; secs++)
		{
			// Display current simulated time of the carousel operation.
			cout << "\nTime: " << mins << " mins, " << secs << " secs" << endl;

			// Next passenger (if any remaining) disembarks the plane
			// and joins the queue to await their baggage.
			if (passengerNum < numPassengers)
			{
				passengers.push(tickets[passengerNum]); 
				passengerNum++;
			}

			//When there is room on the retrieval point of the carousel (where baggage comes up
			//the ramp), call function getNextBaggage() within the time loop in main() to
			//	remove a piece of baggage from the loadersand place that baggage on the carousel
			if (carousel.getCurrent() == 0) //if there is a blank spot for a bag
			{
				carousel.remove(); //remove blank spot
				carousel.insert(getNextBaggage(loaders, NUM_LOADERS)); //insert bag from loader
			}


			//The carouselSize ensures all packages have room to be added within the loop
			


			// Display what is on the carousel on a single console line
			// (user must imagine that the carousel wraps around in
			// a circular fashion).  The position where passengers
			// retrieve baggage (retrieval point) is on the far left.
			cout << "Carousel:";
			carousel.output();
			cout << endl;
			displayCustomer(passengers);

			// If there is no baggage on the carousel or no passengers
			// waiting, then cannot retrieve any baggage.
			if (!carousel.isEmpty() && !passengers.empty())
			{
				// Does ticket on current baggage match ticket of
				// passenger at the front of the queue?
				if (carousel.getCurrent() == passengers.front())
				{
			 		cout << "Baggage #" << carousel.getCurrent() << " being retrieved" << endl;
					carousel.remove();		      // Remove baggage from carousel.
					carousel.insert(NO_BAGGAGE);  // Make carousel position empty.
					passengers.pop();		      // Remove passenger from queue.
				}
			}

			// Rotate carousel moving baggage in circular fashion.
			carousel.rotate(false);

			cout << "Press Enter to advance time";
			cin.ignore();  // wait for and eat newline
		}

	// Report whether any baggage remains.
	if (!carousel.isEmpty())
		cout << "\nThere is unclaimed luggage!" << endl;

	// Report whether any passengers are still waiting for their baggage.
	if (!passengers.empty())
		cout << "\nSome customers are still waiting in line!" << endl;

	return 0;
}

// Definitions for functions that process baggage and passengers.

// Get next piece of baggage off of loader.  Loaders are
// emptied starting with the first (index 0) through the
// last.  If all loaders are empty, will return NO_BAGGAGE.
int getNextBaggage(stack<int> trucks[], int numTrucks)
{
	// FILL IN FUNCTION.
	for (int i = 0; i < numTrucks; i++)
	{
		if (!trucks[i].empty()) //as long as the stack isnt empty we can pop from it, so below implement algorithm
		{
			int baggageTicketNum = trucks[i].top(); //the ticket for the baggage we will return is the top bag
			trucks[i].pop(); //pop that bag from stack
			return baggageTicketNum; //return ticketNum of next baggage
		}
	}
	return NO_BAGGAGE; //if no more bags
}

//Function to go through and display ticket numbers of customers in queue

void displayCustomer(queue<int>& customer) 
{
	 queue<int> copyQueue = customer; //copy customer into new copyQueue

	cout << "Customer: ";
	for (int i = 0; i < copyQueue.size(); i++) //while we havent reached the size of the queue, go through it by increment
	{
		cout << copyQueue.front() << '\n'; //display the passenger number in the front of the queue
		cout << "          "; //space to line up passenger numbers waiting 
		copyQueue.pop();
	}
} //copyQueue automatically destroyed at end of scope
