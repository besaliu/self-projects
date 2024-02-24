/****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA2
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
  int* distance;
  int* parent;
  List* adjacency;
  int vertices;
  int edges;
  int source;
} GraphObj;
Graph newGraph(int n){
  Graph G = malloc(sizeof(GraphObj));
  if (G == NULL){
    printf("Error: unable to allocate memory for GraphObj");
    exit(EXIT_FAILURE);
  }
  G->adjacency = malloc((n+1)*sizeof(List));
  G->parent =  malloc((n+1)*sizeof(int));
  G->distance = malloc((n+1)*sizeof(int));
  G->color = malloc((n+1)*sizeof(int));
  G->vertices = n;
  G->edges = 0;
  G->source = NIL;

  for(int i = 1; i <=n; i++){
      G->adjacency[i] = newList();
      G->color[i] = 'w';
      G->distance[i] = INF;
      G->parent[i] = NIL;


}
  
  return G;
}
void freeGraph(Graph* pG) {
    if (pG != NULL && *pG != NULL) {
        for (int i = 1; i <= (*pG)->vertices; i++) {
            freeList(&((*pG)->adjacency[i])); 
        }

        free((*pG)->adjacency);
        free((*pG)->parent);
        free((*pG)->distance);
        free((*pG)->color);
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
int getSource(Graph G){
  if (G == NULL){
    fprintf(stderr, "Error, calling getSource on NULL graph");
    exit(EXIT_FAILURE);
  }
  return G->source;
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

int getDist(Graph G, int u) {
    if (G == NULL) {
        fprintf(stderr, "Error: Calling getDistance on NULL graph\n");
        return NIL;  
    }

    if (u < 1 || u > G->vertices) {
        fprintf(stderr, "Error: Vertex index out of bounds\n");
        return NIL;  
    }


    return G->distance[u];
}

void getPath(List L, Graph G, int u) {
    if (G == NULL) {
        fprintf(stderr, "Error: Calling getPath on NULL graph\n");
        exit(EXIT_FAILURE);
    }

    if (u < 1 || u > G->vertices) {
        fprintf(stderr, "Error: Vertex index out of bounds in getPath\n");
        exit(EXIT_FAILURE);
    }

    if (u == getSource(G)) {
        append(L, u);
    } else if (G->parent[u] == NIL) {
        // No path exists
        clear(L);  // Clear the list
        append(L, NIL);  // Append only the destination vertex
    } else {
        getPath(L, G, G->parent[u]);
        append(L, u);
    }
}



void makeNull(Graph G){
  G->edges = 0;
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

void addArc(Graph G, int u, int v){
  if (G == NULL){
    fprintf(stderr, "Error: calling addEdge on NULL graph");
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
  sortList(uAdj);
  
}

void BFS(Graph G, int s) {
    if (s < 1 || s > G->vertices) {
        fprintf(stderr, "Error: Invalid source vertex %d for BFS\n", s);
        exit(EXIT_FAILURE);
    }

    G->source = s;  // Update the source

    for (int i = 1; i <= G->vertices; i++) {
        G->color[i] = 'w';
        G->distance[i] = INF;
        G->parent[i] = NIL;
    }

    List queue = newList();
    append(queue, s);

    G->color[s] = 'g';  // Mark the source vertex as gray
    G->distance[s] = 0; // Distance from source to itself is 0

    while (length(queue) > 0) {
        int u = front(queue);
        deleteFront(queue);

        List adjList = G->adjacency[u];
        moveFront(adjList);
        while (index(adjList) != -1) {
            int v = get(adjList);

            if (G->color[v] == 'w') {
                G->color[v] = 'g';
                G->distance[v] = G->distance[u] + 1;
                G->parent[v] = u;  // Update parent only when discovering the vertex
                append(queue, v);
            }

            moveNext(adjList);
        }

        G->color[u] = 'b'; // Mark vertex as black after exploring its neighbors
    }

    freeList(&queue);
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
