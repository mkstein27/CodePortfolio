/***
 * Morgan Stein
 * mkstein
 * 2026 Winter CSE101 PA1
 * List.h
 * Header file for list functions
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define FORMAT "%d" 

// Exported Types -------------------------------------------------------------

typedef int ListElement;
typedef struct ListObj*List; 

// Constructors-Destructors ---------------------------------------------------

// newList()
// Creates a new empty list.
List newList();

// freeList()
// Frees heap memory associated with *pL, sets *pL to NULL.
void freeList(List* pL);

// Access functions -----------------------------------------------------------

// length()
// Returns the number of elements in L.
int length(List L);

// position()
// If cursor is defined, returns the position of the cursor element, otherwise
// returns -1.
int position(List L);

// front()
// Returns front element. Pre: length()>0
ListElement front(List L);

// back()
// Returns back element. Pre: length()>0
ListElement back(List L);

// get()
// Returns cursor element. Pre: length()>0, position()>=0
ListElement get(List L);

// equals()
// Returns true if A and B are the same integer sequence, false otherwise. The
// cursor is not altered in either List.
bool equals(List A, List B);

// Manipulation procedures ----------------------------------------------------

// clear()
// Resets L to its original empty state.
void clear(List L);

// set()
// Overwrites the cursor element’s data with x. Pre: length()>0, position()>=0
void set(List L, ListElement x);

// moveFront()
// If L is non-empty, places the cursor under the front element, otherwise does
// nothing.
void moveFront(List L);

// moveBack()
// If List is non-empty, places the cursor under the back element, otherwise
// does nothing.
void moveBack(List L);

// movePrev()
// If cursor is defined and not at front, moves cursor one step toward front of
// L, if cursor is defined and at front, cursor becomes undefined, if cursor is
// undefined does nothing.
void movePrev(List L);


// moveNext()
// If cursor is defined and not at back, moves cursor one step toward back of
// L, if cursor is defined and at back, cursor becomes undefined, if cursor is
// undefined does nothing.
void moveNext(List L);

// prepend()
// Insert new element into L. If List is non-empty, insertion takes place
// before front element.
void prepend(List L, ListElement data);

// append()
// Insert new element into L. If List is non-empty, insertion takes place
// after back element.
void append(List L, ListElement data);

// insertBefore()
// Insert new element before cursor. Pre: length()>0, position()>=0
void insertBefore(List L, ListElement data);

// insertAfter()
// Inserts new element after cursor. Pre: length()>0, position()>=0
void insertAfter(List L, ListElement data);

// deleteFront()
// Deletes the front element. Pre: length()>0
void deleteFront(List L);

// deleteBack()
// Deletes the back element. Pre: length()>0
void deleteBack(List L);

// delete()
// Deletes cursor element, making cursor undefined. Pre: length()>0, position()>=0
void delete(List L);

// Other operations -----------------------------------------------------------

// printList()
// Prints a string representation of L consisting of a comma separated sequence
// of integers, surrounded by parentheses, with front on left, to the stream
// pointed to by out.
void printList(FILE* out, List L);

// copyList()
// Returns a new List representing the same integer sequence as L. The cursor
// in the new list is undefined, regardless of the state of the cursor in L. The
// List L is unchanged.
List copyList(List L);

// join()
// Returns the concatenation of A followed by B. The cursor in the new List is
// undefined, regardless of the states of the cursors A in and B. The states of
// A and B are unchanged.
List join(List A, List B);

// split()
// Removes all elements before (in front of but not equal to) the cursor element
// in L. The cursor element in L is unchanged. Returns a new List consisting of
// all the removed elements. The cursor in the returned list is undefined.
// Pre: length(L)>0, position(L)>=0
List split(List L);