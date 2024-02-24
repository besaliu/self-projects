/****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA2
*  Graph.C
*  Implementation file for FindPath using Graph ADT
*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int main(int argc, char* argv[]) {
    // Check if the correct number of command line arguments is provided
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the input file for reading
    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error: Unable to open input file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Read the number of vertices
    int n;
    fscanf(inputFile, "%d", &n);

    // Check for a valid number of vertices
    if (n < 1) {
        fprintf(stderr, "Error: Invalid number of vertices %d\n", n);
        fclose(inputFile);
        exit(EXIT_FAILURE);
    }

    // Create a new graph
    Graph G = newGraph(n);

    // Read edges and build the graph
    int u, v;
    fscanf(inputFile, "%d %d", &u, &v);
    while (u != 0 && v != 0) {
        // Add the edge to the graph
        addEdge(G, u, v);

        // Read the next edge
        fscanf(inputFile, "%d %d", &u, &v);
    }

    // Print the adjacency list representation of the graph
    FILE* outputFile = fopen(argv[2], "w");
    printGraph(outputFile, G);

    // Process source-destination pairs
    fscanf(inputFile, "%d %d", &u, &v);
    while (u != 0 && v != 0) {
        // Perform BFS
        BFS(G, u);

        // Print the distance and shortest path
        fprintf(outputFile, "\nThe distance from %d to %d is ", u, v);
        if (getDist(G, v) == INF) {
            fprintf(outputFile, "infinity\n");
            fprintf(outputFile, "No %d-%d path exists\n", u, v);
        } else {
            fprintf(outputFile, "%d\n", getDist(G, v));
            fprintf(outputFile, "A shortest %d-%d path is: ", u, v);

            // Create a list to store the shortest path
            List shortestPath = newList();
            getPath(shortestPath, G, v);

            // Print the shortest path
            printList(outputFile, shortestPath);
            fprintf(outputFile, "\n");

            // Free the list
            freeList(&shortestPath);
        }

        // Read the next source-destination pair
        fscanf(inputFile, "%d %d", &u, &v);
    }

    // Close the files and free the graph
    fclose(inputFile);
    fclose(outputFile);
    freeGraph(&G);

    return 0;
}
