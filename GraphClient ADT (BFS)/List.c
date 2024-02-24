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
    int data;           // Data stored in the node
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

void sortList(List L) {
    if (L == NULL || L->length <= 1) {
        return;  
    }

    Node current = L->front->next;

    while (current != NULL) {
        int key = current->data;
        Node temp = current->prev;

        while (temp != NULL && temp->data > key) {
            temp->next->data = temp->data;
            temp = temp->prev;
        }

        if (temp == NULL) {
            L->front->data = key;
        } else {
            temp->next->data = key;
        }

        current = current->next;
    }
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

int front(List L){
  if (length > 0){
    return L->front->data;
  }
}

int back(List L){
  if (length >0){
    return L->back->data;
  }
}

int get(List L){
  if(length > 0 && L->index >= 0 && L->cursor != NULL){
    return L->cursor->data;
  }
  else{
    fprintf(stderr, "Error: Cannot get element, list is empty or cursor is undefined\n");
        exit(EXIT_FAILURE);  
  }
}

bool equals(List A, List B){
  if (A == NULL || B == NULL){
    printf("Error: called equals()  on empty list\n");
    exit(EXIT_FAILURE);;
  }
  Node c1 = A->front;
  Node c2 = B->front;
  while (c1 != NULL && c2 != NULL){
    if (c1->data != c2->data){
      return false;
    }
    c1 = c1->next;
    c2 = c2->next;
  }
  return c1 == NULL && c2 == NULL;
}

void clear(List L){
  while(L->length > 0){
    deleteFront(L);
  }
  L->front = L->back = L->cursor = NULL;
  L->length = 0;
  L->index = -1;
}

void set(List L, int x){
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
void prepend(List L, int x){
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
void append(List L, int x){
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

void insertBefore(List L, int x) {
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

void insertAfter(List L, int x) {
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

void printList(FILE* out, List L) {
     if (out == NULL || L == NULL || L->front == NULL) {
        fprintf(stderr, "Error: Invalid arguments in printList\n");
        exit(EXIT_FAILURE);  // You may choose a different way to handle this error
    }

    Node current = L->front;


    while (current != NULL) {
      fprintf(out, "%d", current->data);
      if (current->next != NULL) {
        fprintf(out, " ");
    }

        current = current->next;
    }
}
void printL(List L) {


    Node current = L->front;


    while (current != NULL) {
      fprintf(stdout, "%d", current->data);
      if (current->next != NULL) {
        fprintf(stdout, " ");
    }

        current = current->next;
    }
}

List copyList(List L) {
    if (L == NULL) {
        fprintf(stderr, "Error: Invalid argument in copyList\n");
        exit(EXIT_FAILURE);  // You may choose a different way to handle this error
    }

    List copy = newList();

    Node current = L->front;

    while (current != NULL) {
        append(copy, current->data);
        current = current->next;
    }

    return copy;
}
