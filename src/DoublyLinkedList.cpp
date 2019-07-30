//============================================================================
// Name:       : LinkedList.cpp
// Author      : Gavin Walters
// Date		   : October 3, 2018
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

double strToDouble(string str, char ch);

// structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

int loadCount = 0; // count lists load number for easy reading


//============================================================================
// Doubly Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a doubly linked-list.
 */
class DoublyLinkedList {

private:
    // Internal structure for list entries, housekeeping variables
	// Using a struct for a data structure to store a bid struct and also the
	// pointer for the next node.
	struct Node {
		Bid data;
		struct Node* next;
		struct Node* prev;
	};
	// Housekeeping variables that will be used to manage the list.
	Node* head;
	Node* tail;
	Node* current;
	Node* temp;
	int size;


public:
    DoublyLinkedList();
    virtual ~DoublyLinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void InsertAfter(Bid bid);
    void InsertBefore(Bid bid);
    void PrintList();
    void Remove();
    Bid Search();
    int Size();
};

/**
 * Default constructor
 */
DoublyLinkedList::DoublyLinkedList() {
    // Initialize housekeeping variables
	// Once an instance of LinkedList is created, the housekeeping variables
	// are inititialized to NULL because it is a new list.
	head = NULL;
	tail = NULL;
	current = NULL;
	temp = NULL;
	size = 0;
}

/**
 * Destructor
 */
DoublyLinkedList::~DoublyLinkedList() {
}


/**
 * Append a new bid to the end of the list
 */
void DoublyLinkedList::Append(Bid bid) {
    // Implement append logic
	// Create the new node and initialize it with the given data.
	Node* new_node = new Node;
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->data = bid;

	if (head == NULL && tail == NULL) { // no list yet
		head = new_node;
		return;
	}
	else if (tail == NULL) {  // single node
		head->next = new_node;
		new_node->prev = head;
		tail = new_node;
		return;
	}
	else { // list exists with more then one member
		tail->next = new_node;
		new_node->prev = tail;
		tail = new_node;
	}
	size++;
}

/**
 * Prepend a new bid to the start of the list
 */
void DoublyLinkedList::Prepend(Bid bid) {
    // Implement prepend logic
	// Create the new node and initialize it.
	Node* node = new Node;
	node->next = NULL;
	node->prev = NULL;
	node->data = bid;

	if (head != NULL) {
		head->prev = node;
		node->next = head;
		head = node;
	}
	else {
		head = node;
	}
	size++;
}

void DoublyLinkedList::InsertAfter(Bid bid) {
	Node* node = new Node;
	node->next = NULL;
	node->prev = NULL;
	node->data = bid;
	string bidId;

	cout << ">>Enter bidId to insert after: " << endl;
	cin >> bidId;

	if (tail != NULL) {
		current = tail;
		while (current != NULL) {
			if (current->prev->data.bidId != bidId && (current->data.bidId != bidId)) { // no matches
				current = current->prev;
				continue;
			}
			else if (current->prev->data.bidId == bidId) { // stop here and insert
				node->prev = current->prev;
				node->next = current;
				current->prev->next = node; // update node behind current
				current->prev = node;
				break;
			}
			else { // the tail node was the node to insert after
				current->next = node;
				node->prev = current;
				tail = node;
				break;
			}
		}
	}
	else {
		cout << "\n**Error: Could not locate node: " << bidId << "**" << endl;
	}
}

void DoublyLinkedList::InsertBefore(Bid bid) {
	Node* node = new Node;
	node->next = NULL;
	node->prev = NULL;
	node->data = bid;
	string bidId;

	cout << "Enter bidId to insert before: " << endl;
	cin >> bidId;

	if (head != NULL) {
		current = head;
		while (current != NULL) {
			if (current->next->data.bidId != bidId && (current->data.bidId != bidId)) {
				current = current->next;
				continue;
			}
			else if (current->next->data.bidId == bidId) { // insert node here
				node->next = current->next;
				if (node->prev != NULL) {
					current->prev = node->prev;
					node->prev = current;
				}
				current->next = node;
				node->prev = current;
				current->next->prev = node; // next node record in current needs to point  back at new node
				break;
			}
			else { // the head node was the node to insert before
				current->prev = node; // head looks back at node
				node->next = current; // node looks up at head
				head = node;
				break;
			}
		}
	}
	else {
		cout << "\n**Error: Could not locate node: " << bidId << "\n" << endl;
	}
}


/**
 * Simple output of all bids in the list
 */
void DoublyLinkedList::PrintList() {
    // Implement print logic
	// This will go from link node to link node via a while loop and print
	// out each element as it goes along until reaching the end.
	cout << "\n********************************************" << endl;
	cout << "\n*               START " << loadCount << "  *" << endl;
	cout << "\n********************************************" << endl;
	current = head;

	if (current != NULL) {
		while (current != NULL) {
				cout << current->data.bidId << " | " << current->data.title << " | "<< current->data.fund << " | " << current->data.amount << endl;
				current = current->next;
		}
		cout << "\n********************************************" << endl;
		cout << "\n*               END " << loadCount << "    *" << endl;
		cout << "\n********************************************\n" << endl;
	}
	else {
		cout << "\nSorry, nothing here to print..." << endl;
	}
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void DoublyLinkedList::Remove() {
    // Implement remove logic
	// Step through each link node, checking its data struct for a
	// match to the bidId argument that was passed. Checking also,
	// as a safeguard, to make sure that there is indeed something there before something is
	// removed.
	Node* targetNode = NULL;
	temp = head;
	current = head;
	//string testBid = "98109";
	string bidId;

	cout << ">>Enter a bid id to remove a bid: " << endl;
	cin >> bidId;

	// Stepping through each list node in a stagger so that the temp no is one step behind
	// in order to prepare for node deletion.
	while (current != NULL && current->data.bidId != bidId) {
		temp = current;			// store last node
		current = current->next;// move to next node
	}
	if (current == NULL) {
		// Not found
		cout << "\n**Error: " << bidId << " was not found for deletion.**" << endl;
	}
	else {
		// Found the node, now go through routine to delete.
		// Using the deleteNode pointer to cleanup. Really only current and temp
		// are needed to remove the node from the list
		targetNode = current;// node to be deleted

		if (targetNode->prev == NULL) { // deleting head node
			targetNode->next->prev = NULL;
			head = targetNode->next;
			delete targetNode;
			cout << "\n" << bidId << " deleted." << endl;
			return;
		}
		if (targetNode->next == NULL) { // deleting tail node
			temp->next = NULL;
			tail = temp;
			delete targetNode;
			cout << "\n" << bidId << " deleted." << endl;
			return;
		}
		temp->next = current->next;
		current->next->prev = temp;
		delete targetNode;		 // clear out the old node
		cout << "\n" << bidId << " deleted." << endl;
	}
	size--;
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid DoublyLinkedList::Search() {
    // Implement search logic
	current = head;
	//string testBid = "98109";
	string bidId;

    cout << ">>Enter a bid id to find a bid: " << endl;
    cin >> bidId;

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
	bid.bidId = "\n**Error:Nothing here for " + bidId + "**";
	return bid;
}

/**
 * Returns the current size (number of elements) in the list
 */
int DoublyLinkedList::Size() {
    return size;
}

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
	cout << "\n********************************************" << endl;
	cout << "*               BidID: " << bid.bidId << "  *" << endl;
	cout << "********************************************" << endl;
    cout << "|" << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << " |" <<endl;
    cout << "\n********************************************" << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;
    cout << "\n**Bid Parameters**\n([unique bid id], [name of the bid],[source of fund],[amount of bid]" << endl;
    cout << ">>Enter bid ID: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << ">>Enter title: ";
    getline(cin, bid.title);

    cout << ">>Enter fund: ";
    cin >> bid.fund;

    cout << ">>Enter amount: ";
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
void loadBids(string csvPath, DoublyLinkedList *list) {
	cout << "\n********************************************" << endl;
	cout << "\nLoading CSV file " << csvPath << endl;

	// initialize the CSV Parser
	csv::Parser file = csv::Parser(csvPath);

	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// initialize a bid using data from current row (i)
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.amount = strToDouble(file[i][4], '$');
			list->Append(bid);
		}
		loadCount++;
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

int choiceValid(string str) {
	char input = str[0];
	int output = 0;
	if ((str.length() > 1) || isalpha(input)) {
		cout << "\n**Error:bad input**" << endl;
		return 0;
	}
	output = (int)input - 48; // magic #48: ascii character dec - 48 equals actual integer value
	if (!(output >= 1 && output <= 5) && !(output == 9)) { // if inputs are not valid choices
		cout << "\n**Error:Sorry, but that is an invalid menu selection. Try again.**" << endl;
		cout << output << endl;
		return 0;
	}
	return output;
}

int insertValid(string str) {
	char input = str[0];
	int output = 0;
	if ((str.length() > 1) || isalpha(input)) {
		cout << "bad input" << endl;
		return 0;
	}
	output = ((int)input - 48); // magic #48: ascii character dec - 48 equals actual integer value
	if (!(output >=1 && output <= 5)) { // if inputs are not valid choices
		cout << "\n**Error: Sorry, but that is an invalid option. Try again.**" << endl;
		cout << output << endl;
		return 0;
	}
	return output;
}

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

	cout << "Welcome to DoublyLinkedList!" << endl;

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
        csvPath = "DoublyLinkedList2/src/eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    clock_t ticks; // use to calculate stats for list operations
    DoublyLinkedList bidList; // initialize the DLL to store bids and create object on stack
    bool listLoaded = false;
    Bid bid;
    int insertOption = 0;
    int menuChoice = 0;
    string input; // make sure that input is screened before storing in int.


    while (menuChoice != 9) {
    	cout << "________________________ " << endl;
        cout << "|Menu:                 |" << endl;
        cout << "|----------------------|" << endl;
        cout << "| 1. Enter a Bid       |" << endl;
        cout << "| 2. Load Bids         |" << endl;
        cout << "| 3. Display All Bids  |" << endl;
        cout << "| 4. Find Bid          |" << endl;
        cout << "| 5. Remove Bid        |" << endl;
        cout << "| 9. Exit              |" << endl;
        cout << "|______________________|" << endl;
        cout << ">>Enter choice: ";
        cin >> input;
        if(!(menuChoice = choiceValid(input))) {continue;}


        switch (menuChoice) {
        case 1: // enter bid
        	if (!listLoaded) {cout << "\n**Error: Load a \"Bid List\" first.**" << endl; break;}
        	while (insertOption > 4 || insertOption < 1) {
        		cout << ">>Choose insert option: (1)append list (2)prepend list (3)before Bid (4)after bid"
        				" (5)back to main menu"<< endl;
        		cin >> input;
        		if(!(insertOption = insertValid(input))) {continue;}
        	}
            bid = getBid(); // get user inputs and store in bid struct; returns the bid struct
            switch (insertOption) {
            case 1:
				bidList.Append(bid); // the bid DLL object has been created so we can now store our bid there
				break;
            case 2:
				bidList.Prepend(bid);
				break;
            case 3:
				bidList.InsertBefore(bid);
				break;
            case 4:
            	bidList.InsertAfter(bid);
				break;
            case 5:
            	break;
            default:
            	cout << "**Error: Invalid option." << endl;
            }
            displayBid(bid);
            insertOption = 0; // reset
            break;

        case 2: // load bid
        	if (listLoaded) {
        		cout << "\nGenerating new list\n" << endl;
        	}
        	else {
        		listLoaded = true;
        	}
            ticks = clock();
              loadBids(csvPath, &bidList);
              cout << bidList.Size() << " bids read" << endl;
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            cout << "\n********************************************\n" << endl;
            break;

        case 3: // display bids
        	if (!listLoaded) {cout << "\n**Error: Load a \"Bid List\" first.**" << endl; break;}
            bidList.PrintList();
            break;

        case 4: // find a bid
        	if (!listLoaded) {cout << "\n**Error: Load a \"Bid List\" first.**" << endl; break;}
            ticks = clock();
              bid = bidList.Search();
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "\n**Error:Bid Id " << bidKey << " not found.**" << endl;
            }
            cout << "time: " << ticks << " clock ticks        " << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds"  << endl;
            break;

        case 5: // remove bid
        	if (!listLoaded) {cout << "\n**Error: Load a \"Bid List\" first.**" << endl; break;}
            bidList.Remove();
            break;
        } // end switch
    } // end while

    cout << "Good bye." << endl;

    return 0;
}
