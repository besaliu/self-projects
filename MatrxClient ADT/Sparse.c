/****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA4
*  Sparse.c
*  Implementation file for performing matrix operations on sparse matrices
*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
#include "List.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error: input file is NULL");
        exit(EXIT_FAILURE);
    }

    FILE* outputFile = fopen(argv[2], "w");
    if (outputFile == NULL) {
        perror("Error: output file is NULL");
        fclose(inputFile);
        exit(EXIT_FAILURE);
    }

    int n, a, b;
    fscanf(inputFile, "%d %d %d", &n, &a, &b);


    Matrix A = newMatrix(n);
    Matrix B = newMatrix(n);


    for (int i = 0; i < a; i++) {
        int row, col;
        double val;
        fscanf(inputFile, "%d %d %lf", &row, &col, &val);
        changeEntry(A, row, col, val);
    }


    for (int i = 0; i < b; i++) {
        int row, col;
        double val;
        fscanf(inputFile, "%d %d %lf", &row, &col, &val);
        changeEntry(B, row, col, val);
    }

    fprintf(outputFile, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(outputFile, A);

    fprintf(outputFile, "\nB has %d non-zero entries:\n", NNZ(B));
    printMatrix(outputFile, B);

    Matrix ans;

    ans = scalarMult(1.5, A);
    fprintf(outputFile, "\n(1.5)*A =\n");
    printMatrix(outputFile, ans);
    freeMatrix(&ans);
    ans = sum(A, B);
    fprintf(outputFile, "\nA+B =\n");
    printMatrix(outputFile, ans);
    freeMatrix(&ans);

    ans = sum(A, A);
    fprintf(outputFile, "\nA+A =\n");
    printMatrix(outputFile, ans);
    freeMatrix(&ans);
    ans = diff(B, A);
    fprintf(outputFile, "\nB-A =\n");
    printMatrix(outputFile, ans);
    freeMatrix(&ans);
    ans = diff(A, A);
    fprintf(outputFile, "\nA-A =\n");
    printMatrix(outputFile, ans);
    freeMatrix(&ans);
    ans = transpose(A);
    fprintf(outputFile, "\nTranspose(A) =\n");
    printMatrix(outputFile, ans);
    freeMatrix(&ans);
    ans = product(A, B);
    fprintf(outputFile, "\nA*B =\n");
    printMatrix(outputFile, ans);
    freeMatrix(&ans);
    ans = product(B, B);
    fprintf(outputFile, "\nB*B =\n");
    printMatrix(outputFile, ans);

    freeMatrix(&ans);
    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&ans);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
