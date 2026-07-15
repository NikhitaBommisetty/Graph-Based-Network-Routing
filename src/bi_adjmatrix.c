#include "bi_graph.h"
#include <stdio.h>

int main() {
    char* filename = "test_case_3.csv";

    // Creating an edge list graph from the CSV file
    EdgeListGraph* edgeListGraph = createEdgeListFromCSV(filename);

    if (!edgeListGraph) {
        printf("Failed to create edge list graph.\n");
        return -1;
    }
    
    // Converting the edge list graph to an adjacency matrix graph
    AdjMatrixGraph* adjMatrixGraph = createAdjMatrixFromEdgeList(edgeListGraph);
    if (!adjMatrixGraph) {
        printf("Failed to create adjacency matrix graph.\n");
        free(edgeListGraph->edges);
        free(edgeListGraph);
        return -1;
    }
    
    // Take source IP address as input
    char srcIP[16]; // Buffer to hold IP address (IPv4 max length is 15 characters + null terminator)
    printf("Enter the source IP address: ");
    scanf("%15s", srcIP);

    // Map the source IP address to a vertex index
    int src = mapIPToVertex(srcIP);
    if (src == -1) {
        printf("Source IP mapping failed.\n");
        free(edgeListGraph->edges);
        free(edgeListGraph);
        for (int i = 0; i < adjMatrixGraph->V; i++) {
            free(adjMatrixGraph->matrix[i]);
        }
        free(adjMatrixGraph->matrix);
        free(adjMatrixGraph);
        return -1;
    }
    
    // Take destination IP address as input
    char destIP[16]; // Buffer to hold IP address
    printf("Enter the destination IP address: ");
    scanf("%15s", destIP);

    // Map the destination IP address to a vertex index
    int dest = mapIPToVertex(destIP);
    if (dest == -1) {
        printf("Destination IP mapping failed.\n");
        free(edgeListGraph->edges);
        free(edgeListGraph);
        for (int i = 0; i < adjMatrixGraph->V; i++) {
            free(adjMatrixGraph->matrix[i]);
        }
        free(adjMatrixGraph->matrix);
        free(adjMatrixGraph);
        return -1;
    }

    printf("Bidirectional search on Adjacency Matrix graph:\n");
    bidirectionalSearchAdjMatrix(adjMatrixGraph, src, dest);

    // Free the adjacency matrix graph
    for (int i = 0; i < adjMatrixGraph->V; i++) {
        free(adjMatrixGraph->matrix[i]);
    }
    free(adjMatrixGraph->matrix);
    free(adjMatrixGraph);

    // Free the edge list graph
    free(edgeListGraph->edges);
    free(edgeListGraph);

    return 0;
}
