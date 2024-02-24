/****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA3
*  Graph.C
*  Implementation file for Graph ADT
*****************************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"
typedef struct GraphObj {
    int* color;
    int* parent;
    List* adjacency;
    int* d;  // discovery times
    int* f;  // finish times
    int* time;  // time variable
    int vertices;
    int edges;
    int source;
} GraphObj;

Graph newGraph(int n) {
    Graph G = malloc(sizeof(GraphObj));
    if (G == NULL) {
        printf("Error: unable to allocate memory for GraphObj");
        exit(EXIT_FAILURE);
    }
    G->adjacency = malloc((n + 1) * sizeof(List));
    G->d = malloc((n + 1) * sizeof(int));
    G->f = malloc((n + 1) * sizeof(int));
    G->parent = malloc((n + 1) * sizeof(int));
    G->color = malloc((n + 1) * sizeof(int));
    G->time = malloc((n + 1) * sizeof(int));  // Allocate space for time variable for each vertex
    G->vertices = n;
    G->edges = 0;
    G->source = NIL;

    for (int i = 1; i <= n; i++) {
        G->adjacency[i] = newList();
        G->color[i] = 'w';
        G->d[i] = UNDEF;
        G->f[i] = UNDEF;
        G->parent[i] = NIL;
        G->time[i] = UNDEF;  // Initialize time for each vertex
    }

    return G;
}

void freeGraph(Graph* pG) {
    if (pG != NULL && *pG != NULL) {
        for (int i = 1; i <= (*pG)->vertices; i++) {
            freeList(&((*pG)->adjacency[i]));  // Free adjacency lists
        }

        free((*pG)->adjacency);  // Free array of adjacency lists
        free((*pG)->d);
        free((*pG)->f);
        free((*pG)->parent);
        free((*pG)->color);
        free((*pG)->time);
        free(*pG); 
        *pG = NULL; 
    }
}


int getOrder(Graph G){
  if (G == NULL){
    fprintf(stderr, "Error, calling getOrder on NULL graph");
    exit(EXIT_FAILURE);
  }
  return G->vertices;
}
int getSize(Graph G){
  if (G == NULL){
    fprintf(stderr, "Error, calling getSize on NULL graph");
    exit(EXIT_FAILURE);
  }
  return G->edges;
}

int getParent(Graph G, int u) {
    if (G == NULL) {
        fprintf(stderr, "Error: Calling getParent on NULL graph\n");
        return NIL;  
    }

    if (u < 1 || u > G->vertices) {
        fprintf(stderr, "Error: Vertex index out of bounds in getParent\n");
        return NIL;  
    }

  return G->parent[u];
}
int getDiscover(Graph G, int u) {
    if (1 <= u && u <= getOrder(G)) {
        return G->d[u];
    }

    // Handle invalid vertex index
    fprintf(stderr, "Error: Vertex index out of bounds in getDiscover\n");
    exit(EXIT_FAILURE);
}

int getFinish(Graph G, int u) {
    if (1 <= u && u <= getOrder(G)) {
        return G->f[u];
    }

    // Handle invalid vertex index
    fprintf(stderr, "Error: Vertex index out of bounds in getFinish\n");
    exit(EXIT_FAILURE);
}

void addEdge(Graph G, int u, int v){
  if (G == NULL){
    fprintf(stderr, "Error: calling addEdge on NULL graph");
    exit(EXIT_FAILURE);
  }
  if (u < 1 || u > G->vertices) {
    fprintf(stderr, "Error: Vertex index out of bounds\n");
    exit(EXIT_FAILURE);
  }
  if (v < 1 ||  v > G->vertices) {
    fprintf(stderr, "Error: Vertex index out of bounds\n");
    exit(EXIT_FAILURE);
  }

  List uAdj = G->adjacency[u];
  append(uAdj, v);
  G->edges++;

  List vAdj = G->adjacency[v];
  append(vAdj, u);

  sortList(uAdj);
  sortList(vAdj);
}

void addArc(Graph G, int u, int v) {
    if (G == NULL) {
        fprintf(stderr, "Error: calling addArc on NULL graph\n");
        exit(EXIT_FAILURE);
    }
    if (u < 1 || u > G->vertices || v < 1 || v > G->vertices) {
        fprintf(stderr, "Error: Vertex index out of bounds\n");
        exit(EXIT_FAILURE);
    }

    List uAdj = G->adjacency[u];
    moveFront(uAdj);

    // Check if the edge (u, v) already exists
    while (index(uAdj) != -1) {
        if (get(uAdj) == v) {
            return;  // Edge already exists, no need to add
        }
        moveNext(uAdj);
    }
    // If edge doesn't exist, add it
    append(uAdj, v);
    G->edges++;
    sortList(uAdj);
}

void DFS(Graph G, List S) {
    if (length(S) != getOrder(G)) {
        fprintf(stderr, "Error: Length of S does not match getOrder(G)\n");
        exit(EXIT_FAILURE);
    }

    // Reset time variable for each DFS call
    *G->time = 0;

    List stack = newList();

    // Initialize color, parent, d, and f arrays
    for (int i = 1; i <= G->vertices; i++) {
        G->color[i] = 'w';
        G->parent[i] = NIL;
    }

    // Perform DFS for vertices in the order specified by list S
    moveFront(S);
    while (index(S) != -1) {
        int x = get(S);
        if (G->color[x] == 'w') {
            Visit(G, x, G->time, stack);
        }
        moveNext(S);
    }
    clear(S);
    moveFront(stack);
    while (index(stack) != -1) {
        append(S, get(stack));
        moveNext(stack);
    }
    freeList(&stack);
}


void Visit(Graph G, int x, int* time, List S) {
    G->d[x] = ++(*time);  // Discover x
    G->color[x] = 'g';

    // Explore neighbors
    List adjList = G->adjacency[x];
    moveFront(adjList);
    while (index(adjList) != -1) {
        int y = get(adjList);
        if (G->color[y] == 'w') {
            G->parent[y] = x;
            Visit(G, y, time, S);
        }
        moveNext(adjList);
    }

    G->color[x] = 'b';
    G->f[x] = ++(*time);  // Finish x
    prepend(S, x);
}

void printGraph(FILE* out, Graph G){
  if(out == NULL || G == NULL){
    fprintf(stderr, "Error: Calling printGraph on NULL file or graph");
    return;
  }
  for (int i = 1; i <= G->vertices; i++){
    List Adj = G->adjacency[i];
    moveFront(Adj);
    fprintf(out, "%d: ", i);

    while(index(Adj) != -1){
      fprintf(out, "%d", get(Adj));
      moveNext(Adj);

      if(index(Adj) != -1){
        fprintf(out, " ");
      }
    }

    fprintf(out, "\n");
  }
}
Graph transpose(Graph G) {
    if (G == NULL) {
        printf("Error: calling transpose on NULL graph");
        return NULL; // Return NULL to handle the error
    }

    Graph new = newGraph(getOrder(G));

    for (int u = 1; u <= getOrder(G); u++) {
        if (length(G->adjacency[u]) > 0) {
            List A = copyList(G->adjacency[u]);
            moveFront(A);

            while (index(A) != -1) {
                int v = get(A);
                addArc(new, v, u);
                moveNext(A);
            }

            freeList(&A); // Free the copied list
        }
    }

    return new;
}


Graph copyGraph(Graph G) {
    if (G == NULL) {
        fprintf(stderr, "Error: Calling copyGraph on NULL graph\n");
        exit(EXIT_FAILURE);
    }

    int n = getOrder(G);
    Graph copy = newGraph(n);

    // Copy adjacency lists
    for (int u = 1; u <= n; u++) {
        copy->adjacency[u] = copyList(G->adjacency[u]);
    }

    return copy;
}


