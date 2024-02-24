  /****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA1
*  List.c
*  List ADT implementation
*****************************************************************************************/
#include "List.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
typedef struct NodeObj {
    void* data;           // Data stored in the node
    struct NodeObj* prev;  // Pointer to the previous node
    struct NodeObj* next;  // Pointer to the next node
} NodeObj;

typedef NodeObj* Node;

typedef struct ListObj {
    Node front;
    Node back;
    //Node prev;
    //Node next;
    Node cursor;
    int length;
    int index;
} ListObj;

List newList(void){
  List list = malloc(sizeof(ListObj));
  if (list == NULL) {
    fprintf(stderr, " Error: Unable to allocate memory for ListObj\n");
     exit(EXIT_FAILURE);
  }
  list->front = list->back = list->cursor = NULL;
  list->length = 0;
  list->index = -1;
  return list;
}

void freeList(List* pL) {
    if (pL != NULL && *pL != NULL) {
        Node current = (*pL)->front;
        while (current != NULL) {
            Node temp = current;
            current = current->next;
            free(temp);
        }
        free(*pL);
    }
    *pL = NULL;
}

int length(List L){
  return L->length;
}
int index(List L) {
    if (L->cursor == NULL){
      return (-1);
    }
    return L->index;
}

void* front(List L){
  if (L->length > 0){
    return L->front->data;
  }
  printf("Error: calling front on NULL list");
  exit(EXIT_FAILURE);
}

void* back(List L){
  if (L->length > 0){
    return L->back->data;
  }
  printf("Error: calling back on NULL list");
  exit(EXIT_FAILURE);
}

void* get(List L){
  if(L->length > 0 && L->index >= 0 && L->cursor != NULL){
    return L->cursor->data;
  }
  else{
    fprintf(stderr, "Error: Cannot get element, list is empty or cursor is undefined\n");
        exit(EXIT_FAILURE);  
  }
}


void clear(List L){
  while(L->length > 0){
    deleteFront(L);
  }
  L->front = L->back = L->cursor = NULL;
  L->length = 0;
  L->index = -1;
}

void set(List L, void* x){
  if (L->length > 0 && L->index >= 0) {
    L->cursor->data = x;
  }
}

void moveFront(List L){
  if (L->length > 0){
    L->cursor = L->front;
    L->index = 0;
  }
}

void moveBack(List L){
  if (L->length > 0){
    L->cursor = L->back;
    L->index = L->length-1;
  }
}

void movePrev(List L){
  if (L->length > 0 && L->cursor!= NULL){
    if (L->cursor->prev == NULL) {
      L->cursor = NULL;
    }
    else{
      L->cursor = L->cursor->prev;
    }
  L->index--;
  }
}
void moveNext(List L){
  if (L->length > 0 && L->cursor != NULL){
    if (L->cursor->next == NULL){
      L->cursor = NULL;
    }
    else{
      L->cursor = L->cursor->next;
    }
    L->index++;
  }
}
void prepend(List L, void* x){
  Node newNode = malloc(sizeof(NodeObj));
  if(newNode == NULL){
    fprintf(stderr, "Error: Unable to allocate memory for new node\n");
    exit(EXIT_FAILURE);
  }
  newNode->data = x;
  newNode->prev = NULL;
  newNode->next = L->front;

  if(L->length == 0){
    L->back = newNode;
  } else {
    L->front->prev = newNode;
  }
  L->front = newNode;
  L->length++;
  L->index++;
}
void append(List L, void* x){
  Node newNode = malloc(sizeof(NodeObj));
  if (newNode == NULL){
    fprintf(stderr, "Error: Unable to allocate memory for new node\n");
    exit(EXIT_FAILURE);
  }
  newNode->data = x;
  newNode->prev = L->back;
  newNode->next = NULL;
  if(L->length == 0){
    L->front = newNode;
  } else {
    L->back->next = newNode;
  }
  L->back = newNode;
  L->length++;
}

void insertBefore(List L, void* x) {
    if (L->length > 0 && L->cursor != NULL) {
        Node newNode = malloc(sizeof(NodeObj));
        if (newNode == NULL) {
            fprintf(stderr, "Error: Unable to allocate memory for new node\n");
            exit(EXIT_FAILURE);
        }

        newNode->data = x;
        Node temp = L->cursor->prev;

        newNode->prev = (temp != NULL) ? temp : NULL;
        newNode->next = L->cursor;

        if (temp != NULL) {
            temp->next = newNode;
        } else {
            L->front = newNode;  
        }

        L->cursor->prev = newNode;
        L->length++;
        L->index++;
    }
}

void insertAfter(List L, void* x) {
    if (L->length > 0 && L->cursor != NULL) {
        Node newNode = malloc(sizeof(NodeObj));
        if (newNode == NULL) {
            fprintf(stderr, "Error: Unable to allocate memory for new node\n");
            exit(EXIT_FAILURE);
        }

        newNode->data = x;
        newNode->prev = L->cursor;
        newNode->next = L->cursor->next;

        if (L->cursor->next != NULL) {
            L->cursor->next->prev = newNode;
        } else {
            L->back = newNode; 
        }

        L->cursor->next = newNode;
        L->length++;
    }
}

void deleteFront(List L) {

    if (L->front != NULL) {
        Node temp = L->front;
        if(L->cursor == L->front){
          L->index = -1;
          L->cursor = NULL;

        }
        if (L->front->next != NULL) {
            L->front = L->front->next;
            L->front->prev = NULL;
        } else {
            L->front = NULL;
            L->back = NULL;
        }
        L->length--;
        L->index--;
        free(temp);
    }
  
}


void deleteBack(List L) {

    if (L->back != NULL) {
        Node temp = L->back;
        if(L->cursor == L->back){
          L->index = -1;
          L->cursor = NULL;
        }
        if (L->back->prev != NULL) {
            L->back = L->back->prev;
            L->back->next = NULL;
        } else {
            L->front = NULL;
            L->back = NULL;
        }
        L->length--;
        free(temp);
    }
  
}


void delete(List L) {
    if (L == NULL) {
      printf("Error: called delete() on NULL list reference\n");
      exit(EXIT_FAILURE);
    }
    if (L->length == 0) {
      printf("Error: called delete() on empty list\n");
      exit(EXIT_FAILURE);
    }
    if(L->index < 0){
      printf("Error: called delete() on NULL cursor\n");
      exit(EXIT_FAILURE); 
    }
    if (L->length > 0 && L->cursor != NULL && L->index >= 0) {
        Node temp = L->cursor->prev;

        if (temp != NULL) {
            temp->next = L->cursor->next;
        } else {
            L->front = L->cursor->next;  // Update front if cursor is at the front
        }

        if (L->cursor->next != NULL) {
            L->cursor->next->prev = temp;
        } else {
            L->back = temp;  // Update back if cursor is at the back
        }

        free(L->cursor);
        L->cursor = NULL;
        L->length--;
    }
}


