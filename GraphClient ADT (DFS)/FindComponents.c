/****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA3
*  Graph.h
*  Header file for Graph ADT
*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"

int main(int argc, char* argv[]) {
    // Check if correct number of command-line arguments is provided
    if (argc != 3) {
        fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* in = fopen(argv[1], "r");
    if (in == NULL) {
        fprintf(stderr, "Unable to open input file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    FILE* out = fopen(argv[2], "w");
    if (out == NULL) {
        fprintf(stderr, "Unable to open output file %s\n", argv[2]);
        fclose(in);
        exit(EXIT_FAILURE);
    }

    int n, u, v;
    fscanf(in, "%d", &n);
    Graph G = newGraph(n);

    // Read edges from the input file and add them to the graph
    while (fscanf(in, "%d %d", &u, &v) == 2 && (u != 0 || v != 0)) {
        addArc(G, u, v);
    }

    // Print adjacency list representation of G to the output file
    fprintf(out, "Adjacency list representation of G:\n");
    printGraph(out, G);
    fprintf(out, "\n");

    fclose(in);

    // Create a new list to store the order of vertices for DFS
    List stack = newList();
    for(int i = 1; i <= getOrder(G); i++){
        append(stack, i);
    }

    // Perform DFS on G and store vertices by decreasing finish times in the stack
    DFS(G, stack);

    // Compute the transpose of G
    Graph GT = transpose(G);

    // Perform DFS on the transpose of G using the vertices in the stack
    DFS(GT, stack);

    // Determine the strongly connected components of G
    int numStronglyConnected = 0;
    moveFront(stack);
    while (index(stack) != -1) {
        int v = get(stack);
        if (getParent(GT, v) == NIL) {
            numStronglyConnected++;
        }
        moveNext(stack);
    }
    // Print the number of strongly connected components
    fprintf(out, "G contains %d strongly connected components:", numStronglyConnected);
    int componentNum = 1;
    fprintf(out, "\n");
    List new = newList();
    // Move the cursor to the front of the stack
    moveBack(stack);
    for (; index(stack) >= 0; movePrev(stack)) {
        int v = get(stack);
        if (getParent(GT, v) == NIL) {
            fprintf(out, "Component %d: ", componentNum);
            prepend(new, v);
            printList(out, new);
            fprintf(out, "\n");
            clear(new);
            componentNum++;
        }
        else {
            prepend(new, v);
        }

}

    
    fclose(out);
    freeGraph(&G);
    freeGraph(&GT);
    freeList(&stack);

    return 0;
}
