#include "bi_graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

// Function to convert an IP address to an integer
int vertexCount = 0;
IPMap* ipMap = NULL;

int ipToInt(char* ip) {
    int a, b, c, d;
    sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
    return (a << 24) | (b << 16) | (c << 8) | d;
}

// Function to map an IP address to a vertex index
int mapIPToVertex(char* ip) {
    for (int i = 0; i < vertexCount; i++) {
        if (strcmp(ipMap[i].ip, ip) == 0) {
            return ipMap[i].index; 
        }
    }

    IPMap* temp = realloc(ipMap, (vertexCount + 1) * sizeof(IPMap));
    if (!temp) {
        fprintf(stderr, "Memory reallocation failed for IPMap.\n");
        return -1; 
    }
    ipMap = temp;

    ipMap[vertexCount].ip = strdup(ip);
    if (!ipMap[vertexCount].ip) {
        fprintf(stderr, "Memory allocation failed for IP string.\n");
        return -1;
    }
    ipMap[vertexCount].index = vertexCount;

    printf("Mapped IP %s to vertex %d\n", ip, vertexCount); 
    vertexCount++;
    return vertexCount - 1;
}

// Function to create a minimum heap with a specified capacity
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->capacity = capacity;
    minHeap->size = 0;
    minHeap->nodes = (PQNode*)malloc(capacity * sizeof(PQNode));
    return minHeap;
}

// Function to swap two nodes in the priority queue
void swap(PQNode* a, PQNode* b) {
    PQNode temp = *a;
    *a = *b;
    *b = temp;
}

// Function to maintain the min-heap property
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->nodes[left].cost < minHeap->nodes[smallest].cost)
        smallest = left;
    if (right < minHeap->size && minHeap->nodes[right].cost < minHeap->nodes[smallest].cost)
        smallest = right;

    if (smallest != idx) {
        swap(&minHeap->nodes[idx], &minHeap->nodes[smallest]);
        minHeapify(minHeap, smallest);
    }
}

// Function to insert a vertex with its cost into the min-heap
void insertMinHeap(MinHeap* minHeap, int vertex, int cost) {
    if (minHeap->size >= minHeap->capacity) return;

    PQNode newNode = {vertex, cost};
    minHeap->nodes[minHeap->size++] = newNode;

    int idx = minHeap->size - 1;
    while (idx != 0 && minHeap->nodes[(idx - 1) / 2].cost > minHeap->nodes[idx].cost) {
        swap(&minHeap->nodes[idx], &minHeap->nodes[(idx - 1) / 2]);
        idx = (idx - 1) / 2;
    }
}

// Function to extract the minimum node from the min-heap
PQNode extractMin(MinHeap* minHeap) {
    if (minHeap->size == 0) {
        return (PQNode){-1, INT_MAX};
    }

    PQNode root = minHeap->nodes[0];
    if (minHeap->size > 1) {
        minHeap->nodes[0] = minHeap->nodes[minHeap->size - 1];
        minHeap->size--;
        minHeapify(minHeap, 0);
    } else {
        minHeap->size--;
    }

    return root;
}

bool isMinHeapEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

// Function to free the memory allocated for the min-heap
void freeMinHeap(MinHeap* minHeap) {
    free(minHeap->nodes);
    free(minHeap);
}

// Function to print the path from the start vertex to the meeting point
void printPath(int* parent, int start, int meetingPoint) {
    if (meetingPoint == -1 || meetingPoint == start) {
        printf("%s ", ipMap[start].ip); 
        return;
    }
    printPath(parent, start, parent[meetingPoint]);
    printf("%s ", ipMap[meetingPoint].ip);
}

// Function to print the path in reverse from the meeting point to the start vertex
void printReversePath(int* parent, int start, int meetingPoint) {
    if (meetingPoint == -1 || meetingPoint == start) {
        printf("%s ", ipMap[start].ip);
        return;
    }
    printf("%s ", ipMap[meetingPoint].ip);
    printReversePath(parent, start, parent[meetingPoint]);
}

// Function to create an edge list graph from a CSV file
EdgeListGraph* createEdgeListFromCSV(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    int E = 0;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        E++;
    }

    EdgeListGraph* graph = (EdgeListGraph*)malloc(sizeof(EdgeListGraph));
    if (!graph) {
        fprintf(stderr, "Memory allocation failed for EdgeListGraph.\n");
        fclose(file);
        return NULL;
    }

    graph->E = E - 1; 
    graph->edges = (Edge*)malloc(graph->E * sizeof(Edge));
    if (!graph->edges) {
        fprintf(stderr, "Memory allocation failed for edges.\n");
        free(graph);
        fclose(file);
        return NULL;
    }

    rewind(file);
    fgets(buffer, sizeof(buffer), file); 

    int index = 0;
    char srcIP[20], destIP[20];
    int latency;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "%[^,],%[^,],%d", srcIP, destIP, &latency) != 3) {
            fprintf(stderr, "Error parsing line: %s", buffer);
            continue; 
        }

        graph->edges[index].source = mapIPToVertex(srcIP);
        graph->edges[index].dest = mapIPToVertex(destIP);
        graph->edges[index].weight = latency;

        index++;
    }

    fclose(file);

    graph->V = vertexCount; 

    return graph;
}

// Function to create an adjacency list graph from an edge list graph
AdjListGraph* createAdjListFromEdgeList(EdgeListGraph* edgeListGraph) {
    AdjListGraph* graph = (AdjListGraph*)malloc(sizeof(AdjListGraph));
    graph->V = edgeListGraph->V;  
    graph->head = (Node**)malloc(graph->V * sizeof(Node*));

    for (int i = 0; i < graph->V; i++) {
        graph->head[i] = NULL;
    }

    for (int i = 0; i < edgeListGraph->E; i++) {
        int src = edgeListGraph->edges[i].source;
        int dest = edgeListGraph->edges[i].dest;
        int weight = edgeListGraph->edges[i].weight;

        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->dest = dest;
        newNode->weight = weight;
        newNode->next = graph->head[src]; 
        graph->head[src] = newNode;

        newNode = (Node*)malloc(sizeof(Node));
        newNode->dest = src;
        newNode->weight = weight;
        newNode->next = graph->head[dest];  
        graph->head[dest] = newNode;
    }

    return graph;
}

// Function to create an adjacency matrix graph from an edge list graph
AdjMatrixGraph* createAdjMatrixFromEdgeList(EdgeListGraph* edgeListGraph) {
    AdjMatrixGraph* graph = (AdjMatrixGraph*)malloc(sizeof(AdjMatrixGraph));
    graph->V = edgeListGraph->V;
    graph->matrix = (int**)malloc(graph->V * sizeof(int*));

    for (int i = 0; i < graph->V; i++) {
        graph->matrix[i] = (int*)malloc(graph->V * sizeof(int));
        for (int j = 0; j < graph->V; j++) {
            graph->matrix[i][j] = (i == j) ? 0 : INF; 
        }
    }

    for (int i = 0; i < edgeListGraph->E; i++) {
        int src = edgeListGraph->edges[i].source;
        int dest = edgeListGraph->edges[i].dest;
        int weight = edgeListGraph->edges[i].weight;
        graph->matrix[src][dest] = weight;
        graph->matrix[dest][src] = weight; 
    }

    return graph;
}

// Function to check for intersection between visited nodes from source and destination
int checkIntersection(bool* visitedFromSrc, bool* visitedFromDest, int numVertices) {
    for (int i = 0; i < numVertices; i++) {
        if (visitedFromSrc[i] && visitedFromDest[i]) {
            return i; 
        }
    }
    return -1; 
}

void bidirectionalSearchEdgeList(EdgeListGraph* edgeListGraph, int src, int dest) {
    MinHeap* minHeapSrc = createMinHeap(edgeListGraph->V);
    MinHeap* minHeapDest = createMinHeap(edgeListGraph->V);
    
    bool* visitedFromSrc = (bool*)calloc(edgeListGraph->V, sizeof(bool));
    bool* visitedFromDest = (bool*)calloc(edgeListGraph->V, sizeof(bool));
    int* parentFromSrc = (int*)malloc(edgeListGraph->V * sizeof(int));
    int* parentFromDest = (int*)malloc(edgeListGraph->V * sizeof(int));
    int* costFromSrc = (int*)malloc(edgeListGraph->V * sizeof(int));
    int* costFromDest = (int*)malloc(edgeListGraph->V * sizeof(int));

    memset(visitedFromSrc, false, edgeListGraph->V * sizeof(bool));
    memset(visitedFromDest, false, edgeListGraph->V * sizeof(bool));
    memset(parentFromSrc, -1, edgeListGraph->V * sizeof(int));
    memset(parentFromDest, -1, edgeListGraph->V * sizeof(int));

    insertMinHeap(minHeapSrc, src, costFromSrc[src] = 0);
    insertMinHeap(minHeapDest, dest, costFromDest[dest] = 0);
    visitedFromSrc[src] = true;
    visitedFromDest[dest] = true;

    int meetingPoint = -1;

    while (!isMinHeapEmpty(minHeapSrc) && !isMinHeapEmpty(minHeapDest)) {
        PQNode currentNodeSrc = extractMin(minHeapSrc);

        // Expand nodes from the source side
        for (int i = 0; i < edgeListGraph->E; i++) {
            Edge edge = edgeListGraph->edges[i];
            if (edge.source == currentNodeSrc.vertex && !visitedFromSrc[edge.dest]) {
                int newCost = costFromSrc[currentNodeSrc.vertex] + edge.weight;
                if (!visitedFromSrc[edge.dest] || newCost < costFromSrc[edge.dest]) {
                    visitedFromSrc[edge.dest] = true;
                    parentFromSrc[edge.dest] = currentNodeSrc.vertex;
                    costFromSrc[edge.dest] = newCost;
                    insertMinHeap(minHeapSrc, edge.dest, costFromSrc[edge.dest]);

                    if (visitedFromDest[edge.dest]) {
                        meetingPoint = edge.dest;
                        break;
                    }
                }
            } else if (edge.dest == currentNodeSrc.vertex && !visitedFromSrc[edge.source]) {
                int newCost = costFromSrc[currentNodeSrc.vertex] + edge.weight;
                if (!visitedFromSrc[edge.source] || newCost < costFromSrc[edge.source]) {
                    visitedFromSrc[edge.source] = true;
                    parentFromSrc[edge.source] = currentNodeSrc.vertex;
                    costFromSrc[edge.source] = newCost;
                    insertMinHeap(minHeapSrc, edge.source, costFromSrc[edge.source]);

                    if (visitedFromDest[edge.source]) {
                        meetingPoint = edge.source;
                        break;
                    }
                }
            }
        }

        // If a meeting point was found, break out of the loop
        if (meetingPoint != -1) {
            break;
        }

        PQNode currentNodeDest = extractMin(minHeapDest);

        // Expand nodes from the destination side
        for (int i = 0; i < edgeListGraph->E; i++) {
            Edge edge = edgeListGraph->edges[i];
            if (edge.source == currentNodeDest.vertex && !visitedFromDest[edge.dest]) {
                int newCost = costFromDest[currentNodeDest.vertex] + edge.weight;
                if (!visitedFromDest[edge.dest] || newCost < costFromDest[edge.dest]) {
                    visitedFromDest[edge.dest] = true;
                    parentFromDest[edge.dest] = currentNodeDest.vertex;
                    costFromDest[edge.dest] = newCost;
                    insertMinHeap(minHeapDest, edge.dest, costFromDest[edge.dest]);

                    if (visitedFromSrc[edge.dest]) {
                        meetingPoint = edge.dest;
                        break;
                    }
                }
            } else if (edge.dest == currentNodeDest.vertex && !visitedFromDest[edge.source]) {
                int newCost = costFromDest[currentNodeDest.vertex] + edge.weight;
                if (!visitedFromDest[edge.source] || newCost < costFromDest[edge.source]) {
                    visitedFromDest[edge.source] = true;
                    parentFromDest[edge.source] = currentNodeDest.vertex;
                    costFromDest[edge.source] = newCost;
                    insertMinHeap(minHeapDest, edge.source, costFromDest[edge.source]);

                    if (visitedFromSrc[edge.source]) {
                        meetingPoint = edge.source;
                        break;
                    }
                }
            }
        }

        // If a meeting point was found, break out of the loop
        if (meetingPoint != -1) {
            break;
        }
    }

    // If a meeting point was found, reconstruct and print the path
    if (meetingPoint != -1) {
        int totalCost = costFromSrc[meetingPoint] + costFromDest[meetingPoint];
        printf("Path found between %d and %d with total latency %d:\n", src, dest, totalCost);
        printPath(parentFromSrc, src, meetingPoint);
        printf(" <----- MeetingPoint -----> ");
        printReversePath(parentFromDest, dest, meetingPoint);
        printf("\n");
    } else {
        printf("No path found between %d and %d.\n", src, dest);
    }

    // Free allocated memory
    free(visitedFromSrc);
    free(visitedFromDest);
    free(parentFromSrc);
    free(parentFromDest);
    free(costFromSrc);
    free(costFromDest);
    freeMinHeap(minHeapSrc);
    freeMinHeap(minHeapDest);
}

void bidirectionalSearchAdjList(AdjListGraph* adjListGraph, int src, int dest) {
    int numVertices = adjListGraph->V;

    bool* visitedFromSrc = (bool*)calloc(numVertices, sizeof(bool));
    bool* visitedFromDest = (bool*)calloc(numVertices, sizeof(bool));
    int* parentFromSrc = (int*)malloc(numVertices * sizeof(int));
    int* parentFromDest = (int*)malloc(numVertices * sizeof(int));
    int* costFromSrc = (int*)malloc(numVertices * sizeof(int));
    int* costFromDest = (int*)malloc(numVertices * sizeof(int));

    MinHeap* minHeapSrc = createMinHeap(numVertices);
    MinHeap* minHeapDest = createMinHeap(numVertices);

    // Initialize arrays
    for (int i = 0; i < numVertices; i++) {
        parentFromSrc[i] = -1;
        parentFromDest[i] = -1;
        costFromSrc[i] = INT_MAX;  
        costFromDest[i] = INT_MAX;  
    }

    // Insert source and destination into the heaps
    insertMinHeap(minHeapSrc, src, 0);
    insertMinHeap(minHeapDest, dest, 0);
    costFromSrc[src] = 0;
    costFromDest[dest] = 0;
    visitedFromSrc[src] = true;
    visitedFromDest[dest] = true;

    int meetingPoint = -1;

    while (!isMinHeapEmpty(minHeapSrc) && !isMinHeapEmpty(minHeapDest)) {
        // Expand from source
        PQNode currentNodeSrc = extractMin(minHeapSrc);
        int currentVertexSrc = currentNodeSrc.vertex;

        // If the node is already visited from the destination side, we've found the meeting point
        if (visitedFromDest[currentVertexSrc]) {
            meetingPoint = currentVertexSrc;
            break; 
        }

        // Visit neighbors of currentNodeSrc
        for (Node* current = adjListGraph->head[currentVertexSrc]; current != NULL; current = current->next) {
            int neighbor = current->dest; 
            int weight = current->weight; 

            int newCost = costFromSrc[currentVertexSrc] + weight;

            // Check if the new cost is better
            if (!visitedFromSrc[neighbor] && newCost < costFromSrc[neighbor]) {
                visitedFromSrc[neighbor] = true;
                parentFromSrc[neighbor] = currentVertexSrc;
                costFromSrc[neighbor] = newCost;
                insertMinHeap(minHeapSrc, neighbor, newCost);
            }
        }

        // Expand from destination
        PQNode currentNodeDest = extractMin(minHeapDest); 
        int currentVertexDest = currentNodeDest.vertex;

        // If the node is already visited from the source side, we've found the meeting point
        if (visitedFromSrc[currentVertexDest]) {
            meetingPoint = currentVertexDest;
            break;
        }

        // Visit neighbors of currentNodeDest
        for (Node* current = adjListGraph->head[currentVertexDest]; current != NULL; current = current->next) {
            int neighbor = current->dest; 
            int weight = current->weight; 

            int newCost = costFromDest[currentVertexDest] + weight;

            // Check if the new cost is better
            if (!visitedFromDest[neighbor] && newCost < costFromDest[neighbor]) {
                visitedFromDest[neighbor] = true;
                parentFromDest[neighbor] = currentVertexDest;
                costFromDest[neighbor] = newCost;
                insertMinHeap(minHeapDest, neighbor, newCost);
            }
        }
    }

    // If we found a meeting point, reconstruct the path
    if (meetingPoint != -1) {
        int totalCost = costFromSrc[meetingPoint] + costFromDest[meetingPoint];
        printf("Path found between %d and %d with total latency: %d\n", src, dest, totalCost);
        printPath(parentFromSrc, src, meetingPoint);
        printf(" <----- MeetingPoint -----> ");
        printReversePath(parentFromDest, dest, meetingPoint);
        printf("\n");
    } else {
        printf("No path found between %d and %d.\n", src, dest);
    }

    // Free memory
    free(visitedFromSrc);
    free(visitedFromDest);
    free(parentFromSrc);
    free(parentFromDest);
    free(costFromSrc);
    free(costFromDest);
    freeMinHeap(minHeapSrc);
    freeMinHeap(minHeapDest);
}


// Function to perform bidirectional search on the adjacency matrix graph
void bidirectionalSearchAdjMatrix(AdjMatrixGraph* graph, int src, int dest) {
    int numVertices = graph->V;

    bool* visitedFromSrc = (bool*)calloc(numVertices, sizeof(bool));
    bool* visitedFromDest = (bool*)calloc(numVertices, sizeof(bool));
    int* parentFromSrc = (int*)malloc(numVertices * sizeof(int));
    int* parentFromDest = (int*)malloc(numVertices * sizeof(int));
    int* costFromSrc = (int*)malloc(numVertices * sizeof(int));
    int* costFromDest = (int*)malloc(numVertices * sizeof(int));

    MinHeap* minHeapSrc = createMinHeap(numVertices);
    MinHeap* minHeapDest = createMinHeap(numVertices);

    for (int i = 0; i < numVertices; i++) {
        parentFromSrc[i] = -1;
        parentFromDest[i] = -1;
        costFromSrc[i] = INT_MAX;
        costFromDest[i] = INT_MAX;
    }

    insertMinHeap(minHeapSrc, src, 0);
    insertMinHeap(minHeapDest, dest, 0);
    costFromSrc[src] = 0;
    costFromDest[dest] = 0;

    int meetingPoint = -1;
    int minTotalCost = INT_MAX;

    while (!isMinHeapEmpty(minHeapSrc) && !isMinHeapEmpty(minHeapDest)) {
        // Process the source side
        PQNode currentNodeSrc = extractMin(minHeapSrc);
        int currentVertexSrc = currentNodeSrc.vertex;

        // Check if this node was already visited from the destination side
        if (visitedFromDest[currentVertexSrc]) {
            meetingPoint = currentVertexSrc;
            minTotalCost = costFromSrc[currentVertexSrc] + costFromDest[currentVertexSrc];
            break;
        }

        visitedFromSrc[currentVertexSrc] = true;

        for (int neighbor = 0; neighbor < numVertices; neighbor++) {
            if (graph->matrix[currentVertexSrc][neighbor] != INF) {
                int newCost = costFromSrc[currentVertexSrc] + graph->matrix[currentVertexSrc][neighbor];

                if (!visitedFromSrc[neighbor] && newCost < costFromSrc[neighbor]) {
                    parentFromSrc[neighbor] = currentVertexSrc;
                    costFromSrc[neighbor] = newCost;
                    insertMinHeap(minHeapSrc, neighbor, newCost);
                }
            }
        }

        // Process the destination side
        PQNode currentNodeDest = extractMin(minHeapDest);
        int currentVertexDest = currentNodeDest.vertex;

        // Check if this node was already visited from the source side
        if (visitedFromSrc[currentVertexDest]) {
            meetingPoint = currentVertexDest;
            minTotalCost = costFromSrc[currentVertexDest] + costFromDest[currentVertexDest];
            break;
        }

        visitedFromDest[currentVertexDest] = true;

        for (int neighbor = 0; neighbor < numVertices; neighbor++) {
            if (graph->matrix[currentVertexDest][neighbor] != INF) {
                int newCost = costFromDest[currentVertexDest] + graph->matrix[currentVertexDest][neighbor];

                if (!visitedFromDest[neighbor] && newCost < costFromDest[neighbor]) {
                    parentFromDest[neighbor] = currentVertexDest;
                    costFromDest[neighbor] = newCost;
                    insertMinHeap(minHeapDest, neighbor, newCost);
                }
            }
        }
    }

    // If we found a meeting point, reconstruct the path
    if (meetingPoint != -1) {
        printf("Path found between %d and %d with total latency: %d\n", src, dest, minTotalCost);
        printPath(parentFromSrc, src, meetingPoint);
        printf(" <----- MeetingPoint -----> ");
        printReversePath(parentFromDest, dest, meetingPoint);
        printf("\n");
    } else {
        printf("No path found between %d and %d.\n", src, dest);
    }

    // Free memory
    free(visitedFromSrc);
    free(visitedFromDest);
    free(parentFromSrc);
    free(parentFromDest);
    free(costFromSrc);
    free(costFromDest);
    freeMinHeap(minHeapSrc);
    freeMinHeap(minHeapDest);
}
