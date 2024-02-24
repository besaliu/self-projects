#include "List.h"
#include "List.c"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
    char *line = NULL;
    size_t lineSize = 0;
    while (getline(&line, &lineSize, inputFile) != -1) {
        numberOfLines++;
    }
    fseek(inputFile, 0, SEEK_SET);

    char **stringArray = (char **)malloc(numberOfLines * sizeof(char *));
    if (stringArray == NULL) {
        fprintf(stderr, "Error: Memory allocation for string array failed\n");
        return 1; // Return an error code
    }
    for (int i = 0; i < numberOfLines; i++) {
        getline(&line, &lineSize, inputFile);
        stringArray[i] = strdup(line);
    }
    free(line);

    List sortedList = newList();
    for (int i = 0; i < numberOfLines; i++) {
        moveBack(sortedList);

        while (index(sortedList) >= 0 && strcmp(stringArray[get(sortedList)], stringArray[i]) < 0) {
            movePrev(sortedList);
        }

        if (index(sortedList) >= 0) {
            insertAfter(sortedList, i);
        } else {
            prepend(sortedList, i);
        }
    }

    // Continue with other code, such as printing the sorted lines to the output file

    // Free memory
    freeList(&sortedList);
    for (int i = 0; i < numberOfLines; i++) {
        free(stringArray[i]);
    }
    free(stringArray);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
