#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define INF INT_MAX

// Structure to represent a node in the priority queue
typedef struct PQNode {
    int vertex;  
    int cost;    
} PQNode;

// Structure to represent a minimum heap
typedef struct MinHeap {
    PQNode* nodes;  
    int capacity;   
    int size;       
} MinHeap;

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
// Parameters:
//   - ip: IP address as a string (e.g., "192.168.1.1")
// Returns the integer representation of the IP address.
int ipToInt(char* ip);

// Function to map an IP address to a vertex index
// Parameters:
//   - ip: IP address as a string
// Returns the vertex index associated with the given IP.
int mapIPToVertex(char* ip);

// Function to create a minimum heap with a specified capacity
// Parameters:
//   - capacity: Maximum number of nodes the heap can hold
// Returns a pointer to the created MinHeap.
MinHeap* createMinHeap(int capacity);

// Function to swap two nodes in the priority queue
// Parameters:
//   - a: Pointer to the first PQNode to swap
//   - b: Pointer to the second PQNode to swap
void swap(PQNode* a, PQNode* b);

// Function to maintain the min-heap property
// Parameters:
//   - minHeap: Pointer to the MinHeap structure
//   - idx: Index of the node in the heap to heapify
void minHeapify(MinHeap* minHeap, int idx);

// Function to insert a vertex with its cost into the min-heap
// Parameters:
//   - minHeap: Pointer to the MinHeap structure
//   - vertex: Vertex index to insert
//   - cost: Cost associated with the vertex
void insertMinHeap(MinHeap* minHeap, int vertex, int cost);

// Function to extract the minimum node from the min-heap
// Parameters:
//   - minHeap: Pointer to the MinHeap structure
// Returns the PQNode with the minimum cost.
PQNode extractMin(MinHeap* minHeap);

// Function to free the memory allocated for the min-heap
// Parameters:
//   - minHeap: Pointer to the MinHeap to free
void freeMinHeap(MinHeap* minHeap);

// Function to print the path from the start vertex to the meeting point
// Parameters:
//   - parent: Array where parent[i] holds the previous vertex on the shortest path to i
//   - start: Starting vertex for the path
//   - meetingPoint: Vertex where the two searches meet
void printPath(int* parent, int start, int meetingPoint);

// Function to print the path in reverse from the meeting point to the start vertex
// Parameters:
//   - parent: Array where parent[i] holds the previous vertex on the shortest path to i
//   - start: Starting vertex for the path
//   - meetingPoint: Vertex where the two searches meet
void printReversePath(int* parent, int start, int meetingPoint);

// Function to create an edge list graph from a CSV file
// Parameters:
//   - filename: Name of the CSV file containing the edges
// Returns a pointer to the created EdgeListGraph.
EdgeListGraph* createEdgeListFromCSV(const char* filename);

// Function to create an adjacency list graph from an edge list graph
// Parameters:
//   - edgeListGraph: Pointer to the EdgeListGraph to convert
// Returns a pointer to the created AdjListGraph.
AdjListGraph* createAdjListFromEdgeList(EdgeListGraph* edgeListGraph);

// Function to create an adjacency matrix graph from an edge list graph
// Parameters:
//   - edgeListGraph: Pointer to the EdgeListGraph to convert
// Returns a pointer to the created AdjMatrixGraph.
AdjMatrixGraph* createAdjMatrixFromEdgeList(EdgeListGraph* edgeListGraph);

// Function to check for intersection between visited nodes from source and destination
// Parameters:
//   - visitedFromSrc: Array of booleans for nodes visited from the source direction
//   - visitedFromDest: Array of booleans for nodes visited from the destination direction
//   - numVertices: Number of vertices in the graph
// Returns the index of the meeting point, or -1 if no intersection exists.
int checkIntersection(bool* visitedFromSrc, bool* visitedFromDest, int numVertices);

// Function to perform bidirectional search on the edge list graph
// Parameters:
//   - edgeListGraph: Pointer to the EdgeListGraph structure
//   - src: Source vertex index
//   - dest: Destination vertex index
void bidirectionalSearchEdgeList(EdgeListGraph* edgeListGraph, int src, int dest);

// Function to perform bidirectional search on the adjacency list graph
// Parameters:
//   - graph: Pointer to the AdjListGraph structure
//   - src: Source vertex index
//   - dest: Destination vertex index
void bidirectionalSearchAdjList(AdjListGraph* graph, int src, int dest);

// Function to perform bidirectional search on the adjacency matrix graph
// Parameters:
//   - graph: Pointer to the AdjMatrixGraph structure
//   - src: Source vertex index
//   - dest: Destination vertex index
void bidirectionalSearchAdjMatrix(AdjMatrixGraph* graph, int src, int dest);

#endif
