#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define INF INT_MAX

// Structure to represent a node in the queue
typedef struct {
    int vertex;
    int bandwidth;
} QueueNode;

// Structure to represent a maximum heap
typedef struct {
    QueueNode* nodes;
    int size;
} MaxHeap;

// Structure to represent an edge in the graph
typedef struct Edge {
    int source;
    int dest;
    int weight;
} Edge;

// Structure to map IP addresses to vertex indices
typedef struct {
    char* ip;
    int index;
} IPMap;

// Structure to represent a graph using an edge list
typedef struct EdgeListGraph {
    int V;      
    int E;      
    Edge* edges;
} EdgeListGraph;

// Structure to represent a node in the adjacency list
typedef struct Node {
    int dest;
    int weight;
    struct Node* next;
} Node;

// Structure to represent a graph using an adjacency list
typedef struct AdjListGraph {
    int V;
    Node** head;
} AdjListGraph;

// Structure to represent a graph using an adjacency matrix
typedef struct AdjMatrixGraph {
    int V;
    int** matrix;
} AdjMatrixGraph;

// Function to convert an IP address to an integer
int ipToInt(char* ip);

// Function to map an IP address to a vertex index
int mapIPToVertex(char* ip);

// Function to print the path from the start vertex to the meeting point
void printPath(int* parent, int start, int meetingPoint);

//function to swap nodes in a heap
void swap(QueueNode* a, QueueNode* b);

// Function to maintain the max-heap property
void heapify(MaxHeap* heap, int i);

// Extracts the maximum element (node with highest bandwidth) from the heap
QueueNode extractMax(MaxHeap* heap);

// Inserts a new node into the max-heap
void insertMaxHeap(MaxHeap* heap, QueueNode node);

// Function to create an edge list graph from a CSV file
EdgeListGraph* createEdgeListFromCSVByBandwidth(const char* filename);

// Function to create an adjacency list graph from an edge list graph
AdjListGraph* createAdjListFromEdgeList(EdgeListGraph* edgeListGraph);

// Function to create an adjacency matrix graph from an edge list graph
AdjMatrixGraph* createAdjMatrixFromEdgeList(EdgeListGraph* edgeListGraph);

// BFS to find the maximum bandwidth path from src to dest in an edge list graph
bool bfsEdgeListMaxBandwidth(EdgeListGraph* graph, int src, int dest, int* parent);

// Finds the maximum bandwidth path in an edge list graph using Edmonds-Karp
int edmondsKarpEdgeListMaxBandwidth(EdgeListGraph* graph, int src, int dest);

// BFS to find the maximum bandwidth path in an adjacency list graph
bool bfsAdjListMaxBandwidth(AdjListGraph* graph, int src, int dest, int* parent);

// Finds maximum bandwidth using Edmonds-Karp algorithm on adjacency list graph
int edmondsKarpAdjListMaxBandwidth(AdjListGraph* graph, int src, int dest);

// BFS to find the maximum bandwidth path in an adjacency matrix graph
bool bfsAdjMatrixMaxBandwidth(AdjMatrixGraph* graph, int src, int dest, int* parent);

// Finds maximum bandwidth using Edmonds-Karp algorithm on adjacency matrix graph
int edmondsKarpAdjMatrixMaxBandwidth(AdjMatrixGraph* graph, int src, int dest);

#endif
