/****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA1
*  Lex.c
*  Lexicographic sort
*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include <unistd.h>

#define MAX_LINE_LENGTH 1024 // Adjust the maximum line length accordingly

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    char *inputFileName = argv[1];
    char *outputFileName = argv[2];

    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error: Unable to open input file %s\n", inputFileName);
        return 1;
    }

    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        fclose(inputFile);
        fprintf(stderr, "Error: Unable to open output file %s\n", outputFileName);
        return 1;
    }

    int numberOfLines = 0;
    size_t lineSize = MAX_LINE_LENGTH;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, lineSize, inputFile) != NULL) {
        numberOfLines++;
    }
    fseek(inputFile, 0, SEEK_SET);

    char stringArray[numberOfLines][MAX_LINE_LENGTH];
    if (stringArray == NULL) {
        fprintf(stderr, "Error: Memory allocation for string array failed\n");
        return 1; 
    }
    
    for (int i = 0; i < numberOfLines; i++) {
        fgets(line, MAX_LINE_LENGTH, inputFile);
        strcpy(stringArray[i], line);
    }

    

    List sortedList = newList();
    for (int i = 0; i < numberOfLines; i++) {
        moveBack(sortedList);

        while (index(sortedList) >= 0 && strcmp(stringArray[i], stringArray[get(sortedList)]) < 0) {
            movePrev(sortedList);
        }

        if (index(sortedList) >= 0) {
            insertAfter(sortedList, i);
        } else {
            prepend(sortedList, i);
        }
    }
    moveFront(sortedList);
    while(index(sortedList)>=0){
        fprintf(outputFile, "%s", stringArray[get(sortedList)]);
        moveNext(sortedList);
    }
    // Free memory
    
    
    freeList(&sortedList);
    sortedList = NULL;
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
