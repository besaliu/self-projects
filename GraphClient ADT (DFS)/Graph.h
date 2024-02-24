/****************************************************************************************
*  Benjamin Liu
*  besaliu
*  2024 Winter CSE101 PA3
*  Graph.h
*  Header file for Graph ADT
*****************************************************************************************/
#ifndef GRAPH_H
#define GRAPH_H
#include <stdbool.h>
#include <stdio.h>
#include "List.h"
#define NIL 0
#define UNDEF -1
typedef struct GraphObj* Graph;
// Constructors-Destructors
Graph newGraph(int n);
void freeGraph(Graph* pG);
// Access functions
int getOrder(Graph G);
int getSize(Graph G);
int getParent(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
int getDiscover(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
int getFinish(Graph G, int u); /* Pre: 1<=u<=n=getOrder(G) */
// Manipulation procedures
void addArc(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */
void addEdge(Graph G, int u, int v); /* Pre: 1<=u<=n, 1<=v<=n */
void DFS(Graph G, List S); /* Pre: length(S)==getOrder(G) */
void Visit(Graph G, int x, int* time, List stack);
// Other Functions
Graph transpose(Graph G);
Graph copyGraph(Graph G);
void printGraph(FILE* out , Graph G);
int edgeDirectionality(Graph G, int u, int v);
int isEdgeInGraph(Graph G, int u, int v);
#endif