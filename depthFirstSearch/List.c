/***
 * Morgan Stein
 * mkstein
 * 2026 Winter CSE101 PA1
 * List.c
 * Definition of List.h functions
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "List.h"


typedef struct node{
    ListElement data;        
    struct node* next;
    struct node* prev;
} node;

typedef struct ListObj{
    node* front;
    node* back;
    node* cursor;
    int length;
    int position; 
} ListObj;

// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns a pointer to a new node object
node* newNode(ListElement data){
    node* N = malloc(sizeof(node));
    assert (N!=NULL);
    N->data = data;
    N->next = NULL;
    N->prev = NULL;
    return (N);
}

// newList()
// Creates a new empty list.
List newList(){
    List L;
    L = malloc(sizeof(ListObj));
    assert (L != NULL);
    L->front = L->back = L->cursor = NULL;
    L->length = 0;
    L->position = -1;
    return (L);
}

// freeList()
// Frees heap memory associated with *pL, sets *pL to NULL.
void freeList(List* pL){
    if (pL!=NULL && *pL!=NULL){
        clear(*pL);
        free(*pL);
        *pL=NULL;
    }
}

// Access functions -----------------------------------------------------------

// length()
// Returns the number of elements in L.
int length(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: length(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    return(L->length);
}

// position()
// If cursor is defined, returns the position of the cursor element, otherwise
// returns -1.
int position(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: position(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }else{
        return(L->position);
    }
}

// front()
// Returns front element. Pre: length()>0
ListElement front(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: front(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(length(L)==0){
        fprintf(stderr, "List Error: front(): empty list\n");
        exit(EXIT_FAILURE);
    }
    return(L->front->data);
}

// back()
// Returns back element. Pre: length()>0
ListElement back(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: back(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(length(L)==0){
        fprintf(stderr, "List Error: back(): empty list\n");
        exit(EXIT_FAILURE);
    }
    return(L->back->data);
}

// get()
// Returns cursor element. Pre: length()>0, position()>=0
ListElement get(List L){
    if (L==NULL){
        fprintf(stderr, "List Error: get(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if(length(L)==0){
        fprintf(stderr, "List Error: get(): empty list\n");
        exit(EXIT_FAILURE);
    }
    return(L->cursor->data);
}

// equals()
// Returns true if A and B are the same integer sequence, false otherwise. The
// cursor is not altered in either List.
bool equals(List A, List B){
    if (A==NULL || B==NULL){
        fprintf(stderr, "List Error: equals(): NULL List reference\n");
        exit(EXIT_FAILURE);   
    }
    bool eq = (A->length == B-> length);
    node* N = A->front;
    node* M = B->front;
    while(eq && N!=NULL){
        eq = (N->data==M->data);
        N = N->next;
        M = M->next;
    }
    return eq;
}

// Manipulation procedures ----------------------------------------------------

// clear()
// Resets L to its original empty state.
void clear(List L){
    while (length(L)>0){
        deleteFront(L);
    }
}

// set()
// Overwrites the cursor element’s data with x. Pre: length()>0, position()>=0
void set(List L, ListElement x){
    if (length(L)==0){
        fprintf(stderr, "List Error: set(): List is empty\n");
        exit(EXIT_FAILURE);
    }
    if (L->position == 0){
        fprintf(stderr, "List Error: set(): position of cursor is 0\n");
        exit(EXIT_FAILURE);
    }else{
        L->cursor->data = x;
    }
}

// moveFront()
// If L is non-empty, places the cursor under the front element, otherwise does
// nothing.
void moveFront(List L){
    if(!(length(L)<0)){
        L->cursor = L->front;
        L->position = 0;
    }
}

// moveBack()
// If List is non-empty, places the cursor under the back element, otherwise
// does nothing.
void moveBack(List L){
    if(!(length(L)==0)){
        L->cursor = L->back;
        L->position = (L->length) - 1;
    }
}

// movePrev()
// If cursor is defined and not at front, moves cursor one step toward front of
// L, if cursor is defined and at front, cursor becomes undefined, if cursor is
// undefined does nothing.
void movePrev(List L){
    if (L->cursor!=NULL){
       if(L->cursor != L->front){
        L->cursor = L->cursor->prev;
        L->position--;
       }
        else{ L->cursor=NULL;
        L->position = -1;
       }

    }
}


// moveNext()
// If cursor is defined and not at back, moves cursor one step toward back of
// L, if cursor is defined and at back, cursor becomes undefined, if cursor is
// undefined does nothing.
void moveNext(List L){
    if (L->cursor!=NULL){
       if(L->cursor != L->back){
        L->cursor = L->cursor->next;
        L->position++;
       }
        else{ L->cursor=NULL;
        L->position = -1;
       }

    }
}

// prepend()
// Insert new element into L. If List is non-empty, insertion takes place
// before front element.
void prepend(List L, ListElement data){        
    node* N = newNode(data);
    if (length(L)==0){
        L->front = L->back = N;
    }else{                           
        N->next = L->front;
        L->front->prev = N;
        L->front = N;
        if(L->cursor!=NULL){
            L->position++;
        }
    }
    L->length++;
}

// append()
// Insert new element into L. If List is non-empty, insertion takes place
// after back element.
void append(List L, ListElement data){
    node* N = newNode(data);
    if (length(L)==0){
        L->front = L->back = N;
    }else{                           
        N->prev = L->back;
        L->back->next = N;
        L->back = N;
    }
    L->length++;
}

// insertBefore()
// Insert new element before cursor. Pre: length()>0, position()>=0
void insertBefore(List L, ListElement data){
    node* N = newNode(data);
    if (L->position == -1){
        fprintf(stderr, "List Error: insertBefore(): Cursor position <0\n");
        exit(EXIT_FAILURE);
    }else if(length(L)==0){
        fprintf(stderr, "List Error: insertBefore(): List is empty\n");
        exit(EXIT_FAILURE); 
    }
    if (L->cursor == L->front){
        N->next = L->front;
        L->front->prev = N;
        L->front = N;
    }else{
        N->prev = L->cursor->prev;
        N->next = L->cursor;
        L->cursor->prev->next = N;
        L->cursor->prev = N;
    }
    L->length++;
    L->position++;
}

// insertAfter()
// Inserts new element after cursor. Pre: length()>0, position()>=0
void insertAfter(List L, ListElement data){
    node* N = newNode(data);
    node* curr = L->cursor;
    node* next = L->cursor->next;
    N->prev = curr;
    curr->next = N;
    if (L->position == -1){
        fprintf(stderr, "List Error: insertAfter(): Cursor position <0\n");
        exit(EXIT_FAILURE);
    }else if(length(L)==0){
        fprintf(stderr, "List Error: insertAfter(): List is empty\n");
        exit(EXIT_FAILURE); 
    }
    if (L->length ==1){
        curr->next = N;
        L->back = N;
        L->length++;
        return;
    }
    if (next ==NULL) {
        curr->next = N;
        L->back = N;
        L->length++;
    }else{
        curr->next = N;
        N->next = next;
        L->length++;
    }
    
}

// deleteFront()
// Deletes the front element. Pre: length()>0
void deleteFront(List L){
    node*N = L->front;
    if (L==NULL){
        fprintf(stderr, "List Error: deleteFront(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if (length(L)==0){
        fprintf(stderr, "List Error: deleteFront(): Empty List\n");
        exit(EXIT_FAILURE);    
    }
    if(L->length == 1){      
        L->front = L->back = NULL;
    } else{
        L->front = L->front->next;
        L->front->prev = NULL;
    }
    if(L->cursor == N){
        L->cursor = NULL;
        L->position = -1;
    }else if (L->cursor != NULL){
        L->position--;
    }
    L->length--;
    free(N);
}

// deleteBack()
// Deletes the back element. Pre: length()>0
void deleteBack(List L){
    node*N = L->back;

    if (length(L)==0){
        fprintf(stderr, "List Error: deleteBack(): Empty List\n");
        exit(EXIT_FAILURE);    
    }
    if(L->length == 1){      
        L->back = L->back = NULL;
    } else{
        L->back = L->back->prev;
        L->back->next = NULL;
    }
    if(L->cursor == N){
        L->cursor = NULL;
        L->position = -1;
    }
    L->length--;
    free(N);
}

// delete()
// Deletes cursor element, making cursor undefined. Pre: length()>0, position()>=0
void delete(List L){
    node*N = L->cursor;
    if (length(L)==0){
        fprintf(stderr, "List Error: delete(): Empty List\n");
        exit(EXIT_FAILURE);    
    }
    if (L->position == -1){
        fprintf(stderr, "List Error: delete(): Cursor position <0\n");
        exit(EXIT_FAILURE);    
    }
    if(N==L->front){
        deleteFront(L);
    }else if (N==L->back){
        deleteBack(L);
    }else{
        N->prev->next = N->next;
        N->next->prev = N->prev;
        L->length--;
        free(N);
    }
    L->cursor = NULL;
    L->position = -1;
}

// Other operations -----------------------------------------------------------

// printList()
// Prints a string representation of L consisting of a comma separated sequence
// of integers, surrounded by parentheses, with front on left, to the stream
// pointed to by out.
void printList(FILE* out, List L){
    node* N;
    if (L==NULL){
        fprintf(stderr, "List Error: printList(): NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    fprintf(out,"(");
    for(N=L->front; N!=NULL;N=N->next){
        fprintf(out,FORMAT, N->data);
        if(N->next!=NULL){
            fprintf(out,", ");
        }
    }
    fprintf(out,")\n");
}

// copyList()
// Returns a new List representing the same integer sequence as L. The cursor
// in the new list is undefined, regardless of the state of the cursor in L. The
// List L is unchanged.
List copyList(List L){
    List M = newList();
    for(node* N=L->front; N!=NULL;N=N->next){
        append(M,L->front->data);
    }
    return M;
}

// join()
// Returns the concatenation of A followed by B. The cursor in the new List is
// undefined, regardless of the states of the cursors A in and B. The states of
// A and B are unchanged.
List join(List A, List B){
    List L = newList();
    for(node*N=A->front; N!=NULL;N=N->next){
        append(L,N->data);
    }
    for(node* N=B->front; N!=NULL; N=N->next){
        append(L,N->data);
    }
    return L;
}


// split()
// Removes all elements before (in front of but not equal to) the cursor element
// in L. The cursor element in L is unchanged. Returns a new List consisting of
// all the removed elements. The cursor in the returned list is undefined.
// Pre: length(L)>0, position(L)>=0
List split(List L){
    List M = newList();
    if (length(L)==0){
        fprintf(stderr, "List Error: split(): List is Empty \n");
        exit(EXIT_FAILURE);  
    }
    if (L->position == 0){
        fprintf(stderr, "List Error: split(): position of cursor is 0\n");
        exit(EXIT_FAILURE);
    }else{
        int i = 0;
        int j = L->position;
        while (i<j){
            append(M, L->front->data);
            deleteFront(L);
            i++;
        }
    }
    return M;
}
