//============================================================================
// Name:       : LinkedList.cpp
// Author      : Gavin Walters
// Date		   : January 29, 2018
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    // FIXME (1): Internal structure for list entries, housekeeping variables
	// Using a struct for a data structure to store a bid struct and also the
	// pointer for the next node.
	struct Node {
		Bid data;
		struct Node* next;
	};
	// Housekeeping variables that will be used to manage the list.
	Node* head;
	Node* current;
	Node* temp;
	// Size will be dealt with later. Should be a pointer...or maybe a static variable
	// inside of the function call.
	int size;
public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // FIXME (2): Initialize housekeeping variables
	// Once an instance of LinkedList is created, the housekeeping variables
	// are inititialized to NULL because it is a new list.
	head = NULL;
	current = NULL;
	temp = NULL;
	size = NULL;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    // FIXME (3): Implement append logic
	// Create the new node and initialize it with the given data.
	Node* node = new Node;
	node->next = NULL;
	node->data = bid;

	// Case where list is present. If list is not present, make it the head of the list.
	if (head != NULL) {
		current = head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = node;
	}
	else {
		head = node;
	}

	size++;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    // FIXME (4): Implement prepend logic
	// Create the new node and initialize it.
	Node* node = new Node;
	node->next = NULL;
	node->data = bid;

	// Easier than appending. If the list is not present then point the new nodes next pointer
	// to the head node and set the head pointer to the new node. Else, just set the head pointer
	// to the new node.
	if (head != NULL) {
		node->next = head;
		head = node;
	}
	else {
		head = node;
	}

	size++;
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // FIXME (5): Implement print logic
	// This will go from link node to link node via a while loop and print
	// out each element as it goes along until reaching the end.
	current = head;

	if (current != NULL) {
		while (current != NULL) {
				cout << current->data.bidId << " | " << current->data.title << " | "<< current->data.fund << " | " << current->data.amount << endl;
				current = current->next;
		}
	}
	else {
		cout << "Sorry, nothing here to print..." << endl;
	}


}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
    // FIXME (6): Implement remove logic
	// Going to step through each link node, checking its data struct for a
	// match to the bidId argument that was passed. Checking also,
	// as a safeguard, to make sure that there is indeed something there before something is
	// removed.
	Node* deleteNode = NULL;
	temp = head;
	current = head;

	// Stepping through each list node in a stagger so that the temp no is one step behind
	// in order to prepare for node deletion.
	while (current != NULL && current->data.bidId != bidId) {
		temp = current;
		current = current->next;
	}
	if (current == NULL) {
		// Not found
		cout << bidId << " was not found for deletion." << endl;
	}
	else {
		// Found the node, now go through routine to delete.
		// Using the deleteNode pointer to cleanup. Really only current and temp
		// are needed to remove the node from the list.
		deleteNode = current;
		current = current->next;
		temp->next = current;
		// Just cleaning up.
		delete deleteNode;

		cout << bidId << " deleted." << endl;
	}

	size--;
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
    // FIXME (7): Implement search logic
	current = head;

	while (current != NULL) {
		if (current->data.bidId == bidId) {
			return current->data;
		}
		else {
			current = current->next;
		}
	}
	// If the search doesn't find anything, just return a bid
	// with a notification to the user. A little clunky, but it works.
	Bid bid;
	bid.bidId = "Nothing here for " + bidId;
	return bid;
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "src/eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    clock_t ticks;

    LinkedList bidList;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);

            break;

        case 2:
            ticks = clock();

            loadBids(csvPath, &bidList);

            cout << bidList.Size() << " bids read" << endl;

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 3:
            bidList.PrintList();

            break;

        case 4:
            ticks = clock();

            bid = bidList.Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 5:
            bidList.Remove(bidKey);

            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
