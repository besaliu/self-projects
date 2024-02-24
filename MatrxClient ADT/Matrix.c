/****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA4
*  Matrix.c
*  Implementation file for Matrix ADT
*****************************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Matrix.h"
// Define struct EntryObj and typedef it to Entry
typedef struct EntryObj {
    int number;
    double value;
} EntryObj;

typedef EntryObj* Entry;

// Define struct MatrixObj and typedef it to Matrix
typedef struct MatrixObj {
    List* rows;
    int size;
    int nnz;
} MatrixObj;

typedef MatrixObj* Matrix;

Entry newEntry(int number, double value) {
    Entry E = malloc(sizeof(EntryObj));
    if (E == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Entry\n");
        exit(EXIT_FAILURE);
    }
    E->number = number;
    E->value = value;
    return E;
}

void freeEntry(Entry *pEntry) {
    if (pEntry == NULL) {
        fprintf(stderr, "Error: Attempted to free a NULL Entry pointer\n");
        return;
    }
    if (*pEntry == NULL) {
        fprintf(stderr, "Error: Attempted to free a NULL Entry\n");
        return;
    }
    free(*pEntry);
    *pEntry = NULL;
}


// Define newMatrix function
Matrix newMatrix(int n) {
    // Allocate memory for the Matrix struct
    Matrix M = malloc(sizeof(MatrixObj));
    if (M == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Matrix\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the rows array
    M->rows = calloc((n + 1), sizeof(List*));  // Assuming row indexing starts from 1
    if (M->rows == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for rows array\n");
        free(M);  // Free previously allocated memory
        exit(EXIT_FAILURE);
    }

    // Initialize other fields of the Matrix struct
    M->size = n;
    M->nnz = 0;

    // Initialize each row of the Matrix
    for (int i = 1; i <= n; i++) {
        M->rows[i] = newList();
        if (M->rows[i] == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for row %d\n", i);
            // Free previously allocated memory
            for (int j = 1; j < i; j++) {
                freeList(&M->rows[j]);
            }
            free(M->rows);
            free(M);
            exit(EXIT_FAILURE);
        }
    }

    return M;
}
void freeMatrix(Matrix *pMatrix) {
    if (pMatrix != NULL && *pMatrix != NULL) {
        for (int i = 1; i <= (*pMatrix)->size; i++) {
            List row = (*pMatrix)->rows[i];
            moveFront(row);
            while (index(row) >= 0) {
                Entry entry = (Entry)get(row);
                freeEntry(&entry); // Free each entry
                moveNext(row);
            }
            freeList(&row); // Free the list containing the entries
        }
        free((*pMatrix)->rows); // Free the array of lists
        free(*pMatrix); // Free the matrix struct itself
        *pMatrix = NULL;
        pMatrix = NULL; 
    }
}


//  efine size function
int size(Matrix M) {
    if (M == NULL) {
        printf("Error: Attempted to get size of NULL matrix\n");
        return 0;
    }
    return M->size;
}

// Define NNZ function
int NNZ(Matrix M) {
    if (M == NULL) {
        printf("Error: Attempted to get NNZ of NULL matrix\n");
        return 0;
    }
    return M->nnz;
}


int equals(Matrix A, Matrix B){
  if (A == NULL || B == NULL) {
        printf("Error: calling equals on NULL matrices\n");
        exit(EXIT_FAILURE);
  }
    // Check if the sizes of matrices A and B are equal
    if (size(A) != size(B)) {
        return 0; // Matrices have different sizes, so they are not equal
    }

    // Iterate through each row of matrices A and B
    for (int i = 1; i <= size(A); i++) {
        // Iterate through non-zero entries of row i in matrix A
        moveFront(A->rows[i]);
        moveFront(B->rows[i]);
        
        while (index(A->rows[i]) >= 0 && index(B->rows[i]) >= 0) {
            Entry entryA = get(A->rows[i]);
            Entry entryB = get(B->rows[i]);

            // Compare non-zero entries
            if (entryA->value != entryB->value) {
                return 0; // Non-zero entries do not match, matrices are not equal
            }

            moveNext(A->rows[i]);
            moveNext(B->rows[i]);
        }

        // Check if both lists have been exhausted
        if (index(A->rows[i]) != -1 || index(B->rows[i]) != -1) {
            return 0; // One list has more entries than the other, matrices are not equal
        }
    }

    return 1; // Matrices are equal
}

// // makeZero()
// // Re-sets M to the zero Matrix state.
void makeZero(Matrix M){
    if (M == NULL){
        printf("Matrix Error: calling changeEntry() on NULL M matrix reference\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i <= size(M); i++){
        List L = M->rows[i];
        if (L != NULL){
            if(length(L) != 0){
                while(length(L) > 0){
                    Entry E = front(L);
                    freeEntry(&E);
                    deleteFront(L);
                    M->nnz -= 1;
                }
            }
        }
    }
}

void changeEntry(Matrix M, int i, int j, double x) {
    if (M != NULL && i >= 1 && i <= M->size && j >= 1 && j <= M->size) {
        List L = M->rows[i];
        moveFront(L);

        while (index(L) >= 0) {
            Entry entry = (Entry)get(L);
            if (entry->number == j) {
                if (x == 0) {
                    freeEntry(&entry);
                    delete (L);
                    M->nnz--;
                    return;
                } else {
                    entry->value = x;
                    return;
                }
            } else if (entry->number > j) {
                Entry new_entry = newEntry(j, x);
                if (new_entry == NULL) {
                    fprintf(stderr, "Error: Memory allocation failed for new Entry\n");
                    exit(EXIT_FAILURE);
                }
                insertBefore(L, new_entry);
                M->nnz++;
                return;
            }
            moveNext(L);
        }
        if (x != 0) {
            Entry new_entry = newEntry(j, x);
            if (new_entry == NULL) {
                fprintf(stderr, "Error: Memory allocation failed for new Entry\n");
                exit(EXIT_FAILURE);
            }
            append(L, new_entry);
            M->nnz++;
            return;
        }
    }
}


Matrix copy(Matrix A){
    if (A == NULL){
        printf("Matrix Error: calling copy() on NULL matrix\n");
        exit(EXIT_FAILURE);
    }
  Matrix copy = newMatrix(size(A));
  for(int i = 1; i <= size(A); i++){
    moveFront(A->rows[i]);
    while(index(A->rows[i]) >= 0){
        Entry entry = get(A->rows[i]);
        Entry e = newEntry(entry->number, entry->value);
        append(copy->rows[i], e);
        moveNext(A->rows[i]);
    }
  }
  copy->nnz = A->nnz;
  return copy;
}

Matrix transpose(Matrix A) {;
    if (A == NULL){
        printf("Matrix Error: calling changeEntry() on NULL matrix\n");
        exit(EXIT_FAILURE);
    }
    Matrix transposed = newMatrix(size(A)); // Create a new matrix with the transposed dimensions

    for (int i = 1; i <= size(A); i++) {
        moveFront(A->rows[i]); // Start from the front of each row in the original matrix
        while (index(A->rows[i]) >= 0) {
            Entry entry = (Entry)get(A->rows[i]); // Get the entry
            Entry newEntry = malloc(sizeof(EntryObj)); // Allocate memory for the new entry
            newEntry->number = i; // Swap row and column indices
            newEntry->value = entry->value;
            append(transposed->rows[entry->number], newEntry); // Append the entry to the corresponding row in the transposed matrix
            moveNext(A->rows[i]); // Move to the next entry in the original matrix
        }
    }
    transposed->nnz = A->nnz;
    return transposed;
}

Matrix scalarMult(double x, Matrix A) {
    // Create a new matrix with the same dimensions as A
    Matrix result = newMatrix(size(A));

    // Iterate over each row in the matrix A
    for (int i = 1; i <= size(A); i++) {
        moveFront(A->rows[i]); // Move to the front of the current row
        // Iterate over each entry in the current row
        while (index(A->rows[i]) >= 0) {
            Entry entry = (Entry)get(A->rows[i]); // Get the current entry
            Entry newEntry = malloc(sizeof(EntryObj)); // Create a new entry
            newEntry->number = entry->number;
            newEntry->value = entry->value * x; // Multiply the value of the entry by x
            append(result->rows[i], newEntry); // Append the updated entry to the corresponding row in the result matrix
            result->nnz++;
            moveNext(A->rows[i]); // Move to the next entry in the current row
        }
    }

    return result;
}

void printEntry(Entry entry) {
    if (entry == NULL) {

        printf("Error: Entry is NULL\n");
        return;
    }
    printf("(%d, %.1f)\n", entry->number, entry->value);
}
Matrix sum(Matrix A, Matrix B)
{
    if (equals(A, B)){
        return scalarMult(2, A);
    }
    Matrix s = newMatrix(size(A));
    for (int i = 1; i <= size(A); i++) {
        List listA = A->rows[i];
        List listB = B->rows[i];
        moveFront(listA);
        moveFront(listB);
        while (index(listA) >= 0 || index(listB) >= 0) {
            Entry entryA = index(listA) >= 0 ? (Entry)get(listA) : NULL;
            Entry entryB = index(listB) >= 0 ? (Entry)get(listB) : NULL;
            if (entryA == NULL && entryB == NULL){
                moveNext(listB);
                moveNext(listA);
            }
            else if (entryA == NULL) {
                changeEntry(s, i, entryB->number, entryB->value);
                moveNext(listB);
            } else if (entryB == NULL) {
                changeEntry(s, i, entryA->number, entryA->value);
                moveNext(listA);
            } else {
                if (entryA->number < entryB->number) {
                    changeEntry(s, i, entryA->number, entryA->value);
                    moveNext(listA);
                } else if (entryA->number > entryB->number) {
                    changeEntry(s, i, entryB->number, entryB->value);
                    moveNext(listB);
                } else {
                    changeEntry(s, i, entryA->number, entryA->value + entryB->value);
                    moveNext(listA);
                    moveNext(listB);
                }
            }
        }
    }
    return s;
}

Matrix diff(Matrix A, Matrix B) {
    if (size(A) != size(B)) {
        printf("Error: Matrices A and B must have the same dimensions\n");
        exit(EXIT_FAILURE);
    }
    if (equals(A, B)){
        Matrix n = copy(A);
        makeZero(n);
        return n;
    }
     Matrix a = scalarMult(-1.0, B);
     Matrix s = sum (A, a);
     freeMatrix(&a);
     return s;
}
double vectorDot(List A, List B){
    double dotProduct = 0.0;
    moveFront(A);
    moveFront(B);
    while (index(A) >= 0 && index(B) >= 0) {
        Entry entryA = (Entry)get(A);
        Entry entryB = (Entry)get(B);
        if (entryA->number == entryB->number) {
            dotProduct += entryA->value * entryB->value;
            moveNext(A);
            moveNext(B);
        } else if (entryA->number < entryB->number) {
            moveNext(A);
        } else {
            moveNext(B);
        }
    }
    return dotProduct;
}

Matrix product(Matrix A, Matrix B) {
    if (A == NULL || B == NULL) {
        printf("Matrix Error: calling product() on NULL matrix \n");
        exit(EXIT_FAILURE);
    }
    if (size(A) != size(B)){
        printf("Matrix Error: calling product() on NULL matrix\n");
        exit(EXIT_FAILURE);
    }

    if (size(A) == 0) {
        return newMatrix(0);
    }
    Matrix c = transpose(B);
    Matrix M = newMatrix(size(A));

    for (int i = 1; i <= size(A); i++) {
        List rowA = A->rows[i];
        if (length(rowA) == 0) continue; // Skip empty rows in matrix A
        for (int j = 1; j <= size(B); j++) {
            List colB = c->rows[j];
            if (length(colB) == 0) continue; // Skip empty columns in matrix B
            
            double dot = vectorDot(rowA, colB);
            if (dot != 0.0) {
                changeEntry(M, i, j, dot);
            }
        }
    }
    freeMatrix(&c);
    return M;
}

void printMatrix(FILE* out, Matrix M) {
    if (M == NULL) {
        fprintf(out, "Error: Matrix is NULL\n");
        return;
    }

    for (int i = 1; i <= size(M); i++) {
        if (length(M->rows[i]) > 0) {
            fprintf(out, "%d: ", i); // Print row number
            moveFront(M->rows[i]); // Move cursor to the front of the row
            while (index(M->rows[i]) >= 0) {
                Entry entry = (Entry)get(M->rows[i]); // Get entry
                fprintf(out, "(%d, %.1f) ", entry->number, entry->value); // Print column number and value
                moveNext(M->rows[i]); // Move to the next entry in the row
            }

            fprintf(out, "\n"); // End of row
        }
    }
}