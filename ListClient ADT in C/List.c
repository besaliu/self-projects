#include "List.h"
#include <stdlib.h>
#include <stdio.h>
#include <stbool.h>
typedef struct Node {
    int data;           // Data stored in the node
    struct Node* prev;  // Pointer to the previous node
    struct Node* next;  // Pointer to the next node
} Node;

typedef struct ListObj {
    Node* front;
    Node* back;
    Node* prev;
    Node* next;
    Node* cursor;
    int length;
} ListObj;

List newList(void){
  ListObj* list = (ListObj*)malloc(sizeof(ListObj));
  if (list == NULL) {
    fprintf(stderr, " Error: Unable to allocate memory for ListObj\n")
     exit(EXIT_FAILURE);
  }
  list->prev = list->next = list->cursor = NULL;
  list->length = 0;
  return list;
}

void freeList(List* pL){
  if (pL != NULL && *pL != NULL){
    Node* current = (*pL)->front;
    while (current != NULL){
      Node* temp = current;
      current = current->next;
      free(temp);
          }
    free(*pL);
    *pL = NULL
    }
}

int length(List L){
  return L->length;
}
int index(List L){
  int count = -1;
  if (L.cursor == NULL || L.cursor->list == NULL) {
        return count;
  else {
    Node* current = L.front;
    while (current != NULL && current->list != L.cursor->list){
      count++;
      current = current->next;
    }
  }
  return count;
  }
}

int front(List L){
  if (length > 0){
    return L.front->data;
  }
}

int back(List L){
  if (length >0){
    return L.back->data;
  }
}

int get(List L){
  if(length > 0 && L.cursor != NULL){
    reeturn L.cursor-> data;
  }
  else{
    fprintf(sterr, "Error: Cannot get element, list is empty or cursor is undefined\n");
        exit(EXIT_FAILURE);  
  }
}

bool equals(List A, List B){
  if (A == NULL || B == NULL){
    return false
  }
  Node* c1 = A->front;
  Node* c2 = B->front;
  while (c1 != NULL and c2 != NULL){
    if (c1->data != c2->data){
      return false
    }
    c1 = c1->next;
    c2 = c2->next;
  }
  return c1 == NULL && c2 == NULL;
}

void clear(List L){
  if (L == NULL && L->front == NULL){
    return;
  }
  Node* current = L->front;
  Node* nextNode;
  while(current != NULL){
    nextNode = current->next;
    free(current);
    current = nextNode;
  }
  L->front = L->prev = L->next = L->back = L->cursor = NULL;
  L->length = 0;
}

void set(List L, int x){
  if (L.length > 0 && L.cursor != NULL) {
    L.cursor->data = x;
  }
}

void moveFront(List L){
  if (L.length > 0){
    L.cursor = L.front;
  }
}

void moveBack(LList L){
  if (L.length > 0){
    L.cursor = L.back;
  }
}

void movePrev(List L){
  if (L.length > 0 && L.cursor!= NULL){
    if (L.cursor->prev == NULL) {
      L.cursor = NULL;
    }
    else{
      L.cursor = L.cursor->prev;
    }
  }
}
void moveNext(List L){
  if (L.length > 0 && L.cursor != NULL){
    if (L->cursor->next == NULL){
      L.cursor = NULL;
    }
    else{
      L.cursor = L.cursor->next;
    }
  }
}
void prepend(List L, int x){
  Node* newNode = (Node*)malloc(sizeof(Node));
  if(newNode == NULL){
    fprintf(stderr, "Error: Unable to allocate memory for new node\n");
    exit(EXIT_FAILURE);
  }
  newNode->data = x;
  newNode->prev = NULL;
  newNode->next = L->front;

  if(L->front != NULL){
    L->front->prev = newNode;
  }
  L->front = newNode;
  L->length++;
}
void append(List L, int x){
  Node* newNode = (Node*)malloc(sizeof(Node));
  if (newNode == NULL){
    fprintf(stderr, "Error: Unable to allocate memory for new node\n");
    exit(EXIT_FAILURE);
  }
  newNode->data = x;
  newNode->prev = L->back;
  newNode->next = NULL;
  if(L->back != NULL){
    L->back->next = newNode;
  }
  L->back = newNode;
  L->length++;
}

void insertBefore(List L, int x) {
    if (L->length > 0 && L->cursor != NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL) {
            fprintf(stderr, "Error: Unable to allocate memory for new node\n");
            exit(EXIT_FAILURE);
        }

        newNode->data = x;
        Node* temp = L->cursor->prev;

        newNode->prev = (temp != NULL) ? temp : NULL;
        newNode->next = L->cursor;

        if (temp != NULL) {
            temp->next = newNode;
        } else {
            L->front = newNode;  
        }

        L->cursor->prev = newNode;
        L->length++;
    }
}

void insertAfter(List L, int x) {
    if (L->length > 0 && L->cursor != NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
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
    if (L->length > 0) {
        Node* temp = L->front->next;
        if (temp != NULL) {
            temp->prev = NULL;
            free(L->front); 
            L->front = temp;
            L->length--;
            if (L->length == 0) {
                L->back = NULL;  // If the list becomes empty, update the back pointer
                L->cursor = NULL;  // Cursor becomes undefined when the list is empty
            }
        }
    }
}


void deleteBack(List L){
  if(L->length > 0){
    Node* temp = L->back->prev;
    if(temp != NULL){
      temp->next = NULL
      free(L->back);
      L->back = temp;
      L->length--;
      if (L->length == 0) {
                L->front = NULL;  
                L->cursor = NULL; 
            }
    }
  }
}

void delete(List L) {
    if (L->length > 0 && L->cursor != NULL) {
        Node* temp = L->cursor->prev;

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
    if (out == NULL || L == NULL) {
        fprintf(stderr, "Error: Invalid arguments in printList\n");
        exit(EXIT_FAILURE);  // You may choose a different way to handle this error
    }

    Node* current = L->front;

    while (current != NULL) {
        fprintf(out, "%d", current->data);

        if (current->next != NULL) {
            fprintf(out, " ");
        }

        current = current->next;
    }

    fprintf(out, "\n");
}

List copyList(List L) {
    if (L == NULL) {
        fprintf(stderr, "Error: Invalid argument in copyList\n");
        exit(EXIT_FAILURE);  // You may choose a different way to handle this error
    }

    List copy = newList();

    Node* current = L->front;

    while (current != NULL) {
        append(copy, current->data);
        current = current->next;
    }

    return copy;
}
