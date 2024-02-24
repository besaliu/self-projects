/****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA2
*  GraphTest.C
*  Test File for Graph ADT
*****************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"

// Test Case 1: Test addition of arcs and BFS on a small graph.
static const char* testDesc1 = "Test Case 1: Test addition of arcs and BFS on a small graph.";

static int testGraph1() {
    Graph G = newGraph(5);

    addArc(G, 1, 2);
    addArc(G, 1, 3);
    addArc(G, 2, 4);
    addArc(G, 3, 5);

    // Perform BFS starting from vertex 1
    BFS(G, 1);

    // Check the parent of vertices
    if (getParent(G, 1) != NIL || getParent(G, 2) != 1 || getParent(G, 3) != 1 || getParent(G, 4) != 2 || getParent(G, 5) != 3) {
        fprintf(stderr, "Error: Test Case 1 failed.\n");
        return 1;
    }

    freeGraph(&G);
    return 0;
}

// Test Case 2: Test BFS on a disconnected graph.
static const char* testDesc2 = "Test Case 2: Test BFS on a disconnected graph.";

static int testGraph2() {
    Graph G = newGraph(6);

    addEdge(G, 1, 2);
    addEdge(G, 1, 3);
    addEdge(G, 2, 4);
    addEdge(G, 3, 5);

    // Perform BFS starting from vertex 1
    BFS(G, 1);

    // Check the parent of vertices
    if (getParent(G, 1) != NIL || getParent(G, 2) != 1 || getParent(G, 3) != 1 || getParent(G, 4) != 2 || getParent(G, 5) != 3 || getParent(G, 6) != NIL) {
        fprintf(stderr, "Error: Test Case 2 failed.\n");
        return 1;
    }

    freeGraph(&G);
    return 0;
}

int main(int argc, char* argv[]){
   int i, s, max, min, d, n=35;
   List  C = newList(); // central vertices 
   List  P = newList(); // peripheral vertices 
   List  E = newList(); // eccentricities 
   Graph G = NULL;

   // Build graph G 
   G = newGraph(n);
   for(i=1; i<n; i++){
      if( i%7!=0 ) addEdge(G, i, i+1);
      if( i<=28  ) addEdge(G, i, i+7);
   }
   addEdge(G, 9, 31);
   addEdge(G, 17, 13);
   addEdge(G, 14, 33);

   // Print adjacency list representation of G
   printGraph(stdout, G);

   // Calculate the eccentricity of each vertex 
   for(s=1; s<=n; s++){
      BFS(G, s);
      max = getDist(G, 1);
      for(i=2; i<=n; i++){
         d = getDist(G, i);
         max = ( max<d ? d : max );
      }
      append(E, max);
   }

   // Determine the Radius and Diameter of G, as well as the Central and 
   // Peripheral vertices.
   append(C, 1);
   append(P, 1);
   min = max = front(E);
   moveFront(E);
   moveNext(E);
   for(i=2; i<=n; i++){
      d = get(E);
      if( d==min ){
         append(C, i);
      }else if( d<min ){
         min = d;
         clear(C);
         append(C, i);
      }
      if( d==max ){
         append(P, i);
      }else if( d>max ){
         max = d;
         clear(P);
         append(P, i);
      }
      moveNext(E);
   }

   // Print results 
   printf("\n");
   printf("Radius = %d\n", min);
   printf("Central vert%s: ", length(C)==1?"ex":"ices");
   printList(stdout, C);
   printf("\n");
   printf("Diameter = %d\n", max);
   printf("Peripheral vert%s: ", length(P)==1?"ex":"ices");
   printList(stdout, P);
   printf("\n");

   // Run the additional test cases
   if (testGraph1() == 0) {
       printf("%s passed.\n", testDesc1);
   }

   if (testGraph2() == 0) {
       printf("%s passed.\n", testDesc2);
   }

   // Free objects 
   freeList(&C);
   freeList(&P);
   freeList(&E);
   freeGraph(&G);

   return(0);
}
