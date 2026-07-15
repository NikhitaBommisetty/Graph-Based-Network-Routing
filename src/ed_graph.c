#include "ed_graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>


int vertexCount = 0;
IPMap* ipMap = NULL;

// Function to convert an IP address to an integer
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

// Function to print the path from the start vertex to the meeting point
void printPath(int* parent, int start, int meetingPoint) {
    if (meetingPoint == -1 || meetingPoint == start) {
        printf("%s ", ipMap[start].ip);  
        return;
    }
    printPath(parent, start, parent[meetingPoint]);
    printf("%s ", ipMap[meetingPoint].ip);
}

//function to swap nodes in a heap
void swap(QueueNode* a, QueueNode* b) {
    QueueNode temp = *a;
    *a = *b;
    *b = temp;
}

// Function to maintain the max-heap property
void heapify(MaxHeap* heap, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size && heap->nodes[left].bandwidth > heap->nodes[largest].bandwidth)
        largest = left;

    if (right < heap->size && heap->nodes[right].bandwidth > heap->nodes[largest].bandwidth)
        largest = right;

    if (largest != i) {
        swap(&heap->nodes[i], &heap->nodes[largest]);
        heapify(heap, largest);
    }
}

// Extracts the maximum element (node with highest bandwidth) from the heap
QueueNode extractMax(MaxHeap* heap) {
    QueueNode maxNode = heap->nodes[0];
    heap->nodes[0] = heap->nodes[--heap->size];
    heapify(heap, 0);
    return maxNode;
}

// Inserts a new node into the max-heap
void insertMaxHeap(MaxHeap* heap, QueueNode node) {
    heap->nodes[heap->size++] = node;
    for (int i = (heap->size - 1) / 2; i >= 0; i--)
        heapify(heap, i);
}


// Function to create an edge list graph from a CSV file
EdgeListGraph* createEdgeListFromCSVByBandwidth(const char* filename) {
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
    float bandwidth;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (sscanf(buffer, "%[^,],%[^,],%*d,%f", srcIP, destIP, &bandwidth) != 3) {
            fprintf(stderr, "Error parsing line: %s", buffer);
            continue; 
        }

        printf("Processing edge from %s to %s with bandwidth %.2f Mbps\n", srcIP, destIP, bandwidth); 

        graph->edges[index].source = mapIPToVertex(srcIP);
        graph->edges[index].dest = mapIPToVertex(destIP);
        graph->edges[index].weight = bandwidth; 

        index++;
    }

    fclose(file);

    graph->V = vertexCount; 

    return graph;
}
// Function to create an adjacency list graph from an edge list graph//Function to create an Adjacency list from Edgelist
AdjListGraph* createAdjListFromEdgeList(EdgeListGraph* edgeListGraph) {
    AdjListGraph* graph = (AdjListGraph*)malloc(sizeof(AdjListGraph));
    graph->V = edgeListGraph->V;  
    graph->head = (Node**)malloc(graph->V * sizeof(Node));

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
    graph->matrix = (int**)malloc(graph->V * sizeof(int));

    for (int i = 0; i < graph->V; i++) {
        graph->matrix[i] = (int*)malloc(graph->V * sizeof(int));
        for (int j = 0; j < graph->V; j++) {
            graph->matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < edgeListGraph->E; i++) {
        int src = edgeListGraph->edges[i].source;
        int dest = edgeListGraph->edges[i].dest;
        int weight = edgeListGraph->edges[i].weight;
        graph->matrix[src][dest] = weight;
    }

    return graph;
}

// BFS to find the maximum bandwidth path from src to dest in an edge list graph
bool bfsEdgeListMaxBandwidth(EdgeListGraph* graph, int src, int dest, int* parent) {
    bool* visited = (bool*)calloc(graph->V, sizeof(bool)); // Keep track of visited vertices
    MaxHeap* maxHeap = (MaxHeap*)malloc(sizeof(MaxHeap));
    maxHeap->nodes = (QueueNode*)malloc(graph->V * sizeof(QueueNode));
    maxHeap->size = 0;

    // Start with the source node and its bandwidth
    insertMaxHeap(maxHeap, (QueueNode){src, INT_MAX});
    visited[src] = true;
    parent[src] = -1;

    while (maxHeap->size > 0) {
        QueueNode current = extractMax(maxHeap);
        int u = current.vertex;

        // If we reach the destination, return true
        if (u == dest) {
            free(visited);
            free(maxHeap->nodes);
            free(maxHeap);
            return true;
        }

        // Explore adjacent nodes
        for (int i = 0; i < graph->E; i++) {
            int v = graph->edges[i].dest;
            int bandwidth = graph->edges[i].weight;

            // Check if the edge exists and is not visited, or if we can improve the path bandwidth
            if (graph->edges[i].source == u && (bandwidth > 0) && !visited[v]) {
                int minBandwidth = (current.bandwidth < bandwidth) ? current.bandwidth : bandwidth;

                insertMaxHeap(maxHeap, (QueueNode){v, minBandwidth});
                visited[v] = true;
                parent[v] = u;
            }
        }
    }

    free(visited);
    free(maxHeap->nodes);
    free(maxHeap);
    return false;
}

// Finds the maximum bandwidth path in an edge list graph using Edmonds-Karp
int edmondsKarpEdgeListMaxBandwidth(EdgeListGraph* graph, int src, int dest) {
    int maxBandwidth = 0;
    int* parent = (int*)malloc(graph->V * sizeof(int));

    // Run the BFS to find the maximum bandwidth path
    while (bfsEdgeListMaxBandwidth(graph, src, dest, parent)) {
        int pathBandwidth = INT_MAX;

        // Find the minimum bandwidth along the path found
        for (int v = dest; v != src; v = parent[v]) {
            int u = parent[v];
            for (int i = 0; i < graph->E; i++) {
                if (graph->edges[i].source == u && graph->edges[i].dest == v) {
                    pathBandwidth = (graph->edges[i].weight < pathBandwidth) ? graph->edges[i].weight : pathBandwidth;
                }
            }
        }

        // Print the path and its bandwidth
        printf("Path with maximum bandwidth: ");
        printPath(parent, src, dest);
        printf(" Bandwidth: %d\n", pathBandwidth);


        // Now, update the graph by reducing the capacity of the forward edges
        // and increasing the capacity of the reverse edges (simulating flow)
        for (int v = dest; v != src; v = parent[v]) {
            int u = parent[v];
            for (int i = 0; i < graph->E; i++) {
                if (graph->edges[i].source == u && graph->edges[i].dest == v) {
                    graph->edges[i].weight -= pathBandwidth; // Reduce the capacity
                } else if (graph->edges[i].source == v && graph->edges[i].dest == u) {
                    graph->edges[i].weight += pathBandwidth; // Increase the reverse capacity
                }
            }
        }
        maxBandwidth += pathBandwidth;
    }

    free(parent);
    return maxBandwidth;
}


// BFS to find the maximum bandwidth path in an adjacency list graph
bool bfsAdjListMaxBandwidth(AdjListGraph* graph, int src, int dest, int* parent) {
    bool* visited = (bool*)calloc(graph->V, sizeof(bool));
    MaxHeap* maxHeap = (MaxHeap*)malloc(sizeof(MaxHeap));
    maxHeap->nodes = (QueueNode*)malloc(graph->V * sizeof(QueueNode));
    maxHeap->size = 0;

    // Start with the source node and infinite bandwidth
    insertMaxHeap(maxHeap, (QueueNode){src, INT_MAX});
    visited[src] = true;
    parent[src] = -1;

    while (maxHeap->size > 0) {
        QueueNode current = extractMax(maxHeap);
        int u = current.vertex;

        // If we reach the destination, return true
        if (u == dest) {
            free(visited);
            free(maxHeap->nodes);
            free(maxHeap);
            return true;
        }

        // Explore all adjacent nodes
        Node* adjNode = graph->head[u];
        while (adjNode) {
            int v = adjNode->dest;
            int bandwidth = adjNode->weight;

            // Only consider valid paths (positive bandwidth and unvisited or higher bandwidth path)
            int pathBandwidth = (current.bandwidth < bandwidth) ? current.bandwidth : bandwidth;

            if (bandwidth > 0 && !visited[v]) {
                visited[v] = true;
                parent[v] = u;
                insertMaxHeap(maxHeap, (QueueNode){v, pathBandwidth});
            }
            adjNode = adjNode->next;
        }
    }

    free(visited);
    free(maxHeap->nodes);
    free(maxHeap);
    return false;
}

// Corrected function to calculate the maximum bandwidth using Edmonds-Karp
int edmondsKarpAdjListMaxBandwidth(AdjListGraph* graph, int src, int dest) {
    int maxBandwidth = 0;
    int* parent = (int*)malloc(graph->V * sizeof(int));

    // Run the BFS to find augmenting paths
    while (bfsAdjListMaxBandwidth(graph, src, dest, parent)) {
        int pathBandwidth = INT_MAX;

        // Find the minimum bandwidth along the found path
        for (int v = dest; v != src; v = parent[v]) {
            int u = parent[v];
            Node* adjNode = graph->head[u];
            while (adjNode && adjNode->dest != v) {
                adjNode = adjNode->next;
            }
            if (adjNode) {
                pathBandwidth = (adjNode->weight < pathBandwidth) ? adjNode->weight : pathBandwidth;
            }
        }

        // Print the path with maximum bandwidth
        printf("Path with maximum bandwidth: ");
        printPath(parent, src, dest);
        printf(" Bandwidth: %d\n", pathBandwidth);


        // Update the graph by reducing the bandwidth along the path (residual graph)
        for (int v = dest; v != src; v = parent[v]) {
            int u = parent[v];

            // Find the edge (u, v) and reduce its bandwidth
            Node* adjNode = graph->head[u];
            while (adjNode && adjNode->dest != v) {
                adjNode = adjNode->next;
            }
            if (adjNode) {
                adjNode->weight -= pathBandwidth;
            }

            // Find the reverse edge (v, u) and increase its bandwidth
            adjNode = graph->head[v];
            while (adjNode && adjNode->dest != u) {
                adjNode = adjNode->next;
            }
            if (adjNode) {
                adjNode->weight += pathBandwidth;
            } else {
                // If reverse edge doesn't exist, create it
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->dest = u;
                newNode->weight = pathBandwidth;
                newNode->next = graph->head[v];
                graph->head[v] = newNode;
            }
        }
        maxBandwidth += pathBandwidth;
    }

    free(parent);
    return maxBandwidth;
}

// BFS to find the maximum bandwidth path in an adjacency matrix graph
bool bfsAdjMatrixMaxBandwidth(AdjMatrixGraph* graph, int src, int dest, int* parent) {
    bool* visited = (bool*)calloc(graph->V, sizeof(bool));
    MaxHeap* maxHeap = (MaxHeap*)malloc(sizeof(MaxHeap));
    maxHeap->nodes = (QueueNode*)malloc(graph->V * sizeof(QueueNode));
    maxHeap->size = 0;

    insertMaxHeap(maxHeap, (QueueNode){src, INT_MAX});
    visited[src] = true;
    parent[src] = -1;

    while (maxHeap->size > 0) {
        QueueNode current = extractMax(maxHeap);
        int u = current.vertex;

        if (u == dest) {
            free(visited);
            free(maxHeap->nodes);
            free(maxHeap);
            return true;
        }

        for (int v = 0; v < graph->V; v++) {
            int bandwidth = graph->matrix[u][v];
            if (bandwidth > 0 && (!visited[v] || current.bandwidth < bandwidth)) {
                int minBandwidth = (current.bandwidth < bandwidth) ? current.bandwidth : bandwidth;
                insertMaxHeap(maxHeap, (QueueNode){v, minBandwidth});
                visited[v] = true;
                parent[v] = u;
            }
        }
    }

    free(visited);
    free(maxHeap->nodes);
    free(maxHeap);
    return false;
}

int edmondsKarpAdjMatrixMaxBandwidth(AdjMatrixGraph* graph, int src, int dest) {
    int maxBandwidth = 0;
    int* parent = (int*)malloc(graph->V * sizeof(int));
    if (!parent) {
        fprintf(stderr, "Memory allocation failed for parent array.\n");
        return -1; // Exit with an error
    }

    // Run BFS to find augmenting paths
    while (bfsAdjMatrixMaxBandwidth(graph, src, dest, parent)) {
        int pathBandwidth = INT_MAX;

        // Find the minimum bandwidth along the path found
        for (int v = dest; v != src; v = parent[v]) {
            int u = parent[v];
            if (u < 0 || u >= graph->V || v < 0 || v >= graph->V) {
                fprintf(stderr, "Invalid index in path (%d, %d).\n", u, v);
                free(parent);
                return -1; // Exit with an error
            }
            pathBandwidth = (pathBandwidth < graph->matrix[u][v]) ? pathBandwidth : graph->matrix[u][v];
        }

        // Print the path and its bandwidth
        printf("Path with maximum bandwidth: ");
        printPath(parent, src, dest);
        printf(" Bandwidth: %d\n", pathBandwidth);

        // Update the graph with the new capacities
        for (int v = dest; v != src; v = parent[v]) {
            int u = parent[v];
            graph->matrix[u][v] -= pathBandwidth;
            graph->matrix[v][u] += pathBandwidth;
        }

        maxBandwidth += pathBandwidth;
    }

    free(parent);
    return maxBandwidth;
}