/****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA5
*  List.cpp
*  Implementation file for List ADT
*****************************************************************************************/
#include "List.h"
#include <set>
List::Node::Node(ListElement x) {
    data = x;
    next = nullptr;
    prev = nullptr;
}

// Creates new List in the empty state.
List::List() {
    frontDummy = new List::Node(0);
    backDummy = new List::Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    num_elements = 0;
    pos_cursor = 0;
}

// Copy constructor.
List::List(const List& L) {
    frontDummy = new List::Node(0);
    backDummy = new List::Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
    Node *temp = L.frontDummy->next;
    while (temp != L.backDummy) {
        insertBefore(temp->data);
        temp = temp->next;
    }
    moveFront();
}

// Destructor
List::~List() {
    clear();
    delete frontDummy;
    delete backDummy;
}


// Access functions --------------------------------------------------------

// length()
// Returns the length of this List.
int List::length() const { return num_elements; }

// front()
// Returns the front element in this List.
// pre: length()>0
ListElement List::front() const {
    if (length() > 0) {
        return frontDummy->next->data;
    }
    throw std::length_error("List: front(): empty list");
}

// back()
// Returns the back element in this List.
// pre: length()>0
ListElement List::back() const {
    if (length() > 0) {
        return backDummy->prev->data;
    }
    throw std::length_error("List: back(): empty list");
};

// position()
// Returns the position of cursor in this List: 0 <= position() <= length().
int List::position() const { return pos_cursor; }

// peekNext()
// Returns the element after the cursor.
// pre: position()<length()
ListElement List::peekNext() const {
    if(pos_cursor < length()) {
        return afterCursor->data;
    }
    throw std::range_error("List: peekNext(): cursor at back");
};

// peekPrev()
// Returns the element before the cursor.
// pre: position()>0
ListElement List::peekPrev() const {
    if(pos_cursor > 0) {
        return beforeCursor->data;
    }
    throw std::range_error("List: peekPrev(): cursor at front");
};


// Manipulation procedures -------------------------------------------------

// clear()
// Deletes all elements in this List, setting it to the empty state.
void List::clear() {
    moveFront();
    while(length() > 0) {
        eraseAfter();
    }
};

// moveFront()
// Moves cursor to position 0 in this List.
void List::moveFront() {
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    pos_cursor = 0;
};

// moveBack()
// Moves cursor to position length() in this List.
void List::moveBack() {
    beforeCursor = backDummy->prev;
    afterCursor = backDummy;
    pos_cursor = num_elements;
};

// moveNext()
// Advances cursor to next higher position. Returns the List element that
// was passed over. 
// pre: position()<length() 
ListElement List::moveNext() {
    if(pos_cursor < length()) {
        beforeCursor = afterCursor;
        afterCursor = afterCursor->next;
        pos_cursor++;
        return beforeCursor->data;
    }
    throw std::range_error("List: moveNext(): cursor at back");
};

// movePrev()
// Advances cursor to next lower position. Returns the List element that
// was passed over. 
// pre: position()>0
ListElement List::movePrev() {
    if(pos_cursor > 0) {
        afterCursor = beforeCursor;
        beforeCursor = beforeCursor->prev;
        pos_cursor--;
        return afterCursor->data;
    }
    throw std::range_error("List: peekPrev(): cursor at front");
};

// insertAfter()
// Inserts x after cursor.
void List::insertAfter(ListElement x) {
    Node* newNext = new List::Node(x);
    newNext->next = afterCursor;
    newNext->prev = beforeCursor;
    afterCursor->prev = newNext;
    beforeCursor->next = newNext;
    afterCursor = newNext;
    num_elements++;
};

// insertBefore()
// Inserts x before cursor.
void List::insertBefore(ListElement x) {
    Node* newPrev = new List::Node(x);
    newPrev->next = afterCursor;
    newPrev->prev = beforeCursor;
    afterCursor->prev = newPrev;
    beforeCursor->next = newPrev;
    beforeCursor = newPrev;
    num_elements++;
    pos_cursor++;
};

// setAfter()
// Overwrites the List element after the cursor with x.
// pre: position()<length()
void List::setAfter(ListElement x) {
    if(pos_cursor < length()) {
        afterCursor->data = x;
    } else {
        throw std::range_error("List: setAfter(): cursor at back");
    }
};

// setBefore()
// Overwrites the List element before the cursor with x.
// pre: position()>0
void List::setBefore(ListElement x) {
    if(pos_cursor > 0) {
        beforeCursor->data = x;
    } else {
        throw std::range_error("List: setBefore(): cursor at front");
    }
};

// eraseAfter()
// Deletes element after cursor.
// pre: position()<length()
void List::eraseAfter() {
    if(pos_cursor < length()) {
        beforeCursor->next = afterCursor->next;
        afterCursor->next->prev = beforeCursor;
        Node *temp = afterCursor;
        delete(temp);
        afterCursor = beforeCursor->next;
        num_elements--;
    } else {
        throw std::range_error("List: eraseAfter(): cursor at back");
    }
};

// eraseBefore()
// Deletes element before cursor.
// pre: position()>0
void List::eraseBefore() {
    if(pos_cursor > 0) {
        afterCursor->prev = beforeCursor->prev;
        beforeCursor->prev->next = afterCursor;
        Node *temp = beforeCursor;
        delete(temp);
        beforeCursor = afterCursor->prev;
        pos_cursor--;
        num_elements--;
    } else {
        throw std::range_error("List: eraseBefore(): cursor at front");
    }
};


// Other Functions ---------------------------------------------------------

// findNext()
// Starting from the current cursor position, performs a linear search (in 
// the direction front-to-back) for the first occurrence of element x. If x
// is found, places the cursor immediately after the found element, then 
// returns the final cursor position. If x is not found, places the cursor 
// at position length(), and returns -1. 
int List::findNext(ListElement x) {
    if(position() == length()) {
        return -1;
    }
    ListElement L = moveNext();
    while (L != x && L > 0) {
        try {
            L = moveNext();
        } catch (std::range_error& e) {
            return -1;
        }
    }
    return position();
};

// findPrev()
// Starting from the current cursor position, performs a linear search (in 
// the direction back-to-front) for the first occurrence of element x. If x
// is found, places the cursor immediately before the found element, then
// returns the final cursor position. If x is not found, places the cursor 
// at position 0, and returns -1. 
int List::findPrev(ListElement x) {
    if(position() == 0) {
        return -1;
    }
    ListElement L = movePrev();
    while (L != x && L > 0) {
        try {
            L = movePrev();
        } catch (std::range_error& e) {
            return -1;
        }
    }
    return position();
};


// cleanup()
// Removes any repeated elements in this List, leaving only unique elements.
// The order of the remaining elements is obtained by retaining the frontmost
// occurrence of each element, and removing all other occurrences. The cursor   
// is not moved with respect to the retained elements, i.e. it lies between
// the same two retained elements that it did before cleanup() was called.
void List::cleanup() {
    std::set<ListElement> original;
    int index = 0;
    int original_pos = pos_cursor;
    Node *temp = frontDummy->next;
    Node *del = nullptr;
    while (temp != backDummy) {
        if (original.count(temp->data) == 0) {
            original.insert(temp->data);
            temp = temp->next;
        } else {
            del = temp->next;
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            
            // Check if the deleted node is beforeCursor or afterCursor
            if (temp == beforeCursor) {
                beforeCursor = temp->prev;
            } else if (temp == afterCursor) {
                afterCursor = temp->next;
            }
            
            delete temp;
            num_elements--;
            
            // Adjust pos_cursor if necessary
            if (index < original_pos) {
                pos_cursor--;
            }
            temp = del;
        }
        index++;
    }
}


// concat()
// Returns a new List consisting of the elements of this List, followed by
// the elements of L. The cursor in the returned List will be at position 0.
List List::concat(const List& L) const {
    List mergedList(*this); // Create a new list initialized with elements of this list
    mergedList.moveBack(); // Move the cursor to the back of the merged list
    Node *currentNode = L.frontDummy->next; // Start from the first node after the front dummy of L

    // Traverse through each node of list L
    while (currentNode != L.backDummy) {
        // Insert the data of the current node before the cursor in the merged list
        mergedList.insertBefore(currentNode->data);
        currentNode = currentNode->next; // Move to the next node in list L
    }
    mergedList.moveFront(); // Move the cursor to the front of the merged list // Move to the back to correctly update the length
    return mergedList; // Return the merged list
}
// to_string()
// Returns a string representation of this List consisting of a comma
// separated sequence of elements, surrounded by parentheses.
std::string List::to_string() const {
    std::string str = "(";
    if (length() > 0) {
        str += std::to_string(front());
        for (Node* N = frontDummy->next->next; N != backDummy; N = N->next) {
            str += ", " + std::to_string(N->data);
        }
    }
    str += ")";
    return str;
}

// equals()
// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List& R) const{
    //bool eq = false;
    Node* a = nullptr;
    Node* b = nullptr;
    if (this->num_elements != R.num_elements){
        return false;
    }
    a = this->frontDummy;
    b = R.frontDummy;
    while(a!=backDummy){
        if (a->data!=b->data){
            return false;
        }
        a = a->next;
        b = b->next;
    }
    return true;
}

// Overriden Operators -----------------------------------------------------

// operator<<()
// Inserts string representation of L into stream.
std::ostream& operator<<( std::ostream& stream, const List& L ) {
    stream << L.to_string();
    return stream;
};

// operator==()
// Returns true if and only if A is the same integer sequence as B. The 
// cursors in both Lists are unchanged.
bool operator==(const List& A, const List& B) {
    return A.equals(B);
}
// operator=()
// Overwrites the state of this List with state of L.
List& List::operator=(const List& other) {
    if (this != &other) {  // Check for self-assignment
        // Clear the current contents of the list
        clear();

        // Copy the elements from 'other' to 'this'
        Node* temp = other.frontDummy->next;
        while (temp != other.backDummy) {
            insertBefore(temp->data);
            temp = temp->next;
        }
    }
    moveFront();
    return *this;
}


