#include <stdio.h>
#include <stdlib.h>
#include "ed_graph.h"

int main() {
    // Load the graph from a CSV file using an edge list representation
    EdgeListGraph* edgeListGraph = createEdgeListFromCSVByBandwidth("ed_test_case_2.csv");
    if (!edgeListGraph) {
        fprintf(stderr, "Failed to load graph from CSV.\n");
        return -1;
    }

    // Convert the edge list graph to an adjacency matrix graph
    AdjMatrixGraph* adjMatrixGraph = createAdjMatrixFromEdgeList(edgeListGraph);
    if (!adjMatrixGraph) {
        fprintf(stderr, "Failed to convert to adjacency matrix.\n");
        free(edgeListGraph->edges);
        free(edgeListGraph);
        return -1;
    }

    // Take source IP address as input from the user
    char srcIP[16]; // Buffer to hold IP address (IPv4 max length is 15 characters + null terminator)
    printf("Enter the source IP address: ");
    scanf("%15s", srcIP);

    // Map the source IP address to a vertex index
    int source = mapIPToVertex(srcIP);
    if (source == -1) {
        fprintf(stderr, "Source IP mapping failed.\n");
        for (int i = 0; i < adjMatrixGraph->V; i++) {
            free(adjMatrixGraph->matrix[i]);
        }
        free(adjMatrixGraph->matrix);
        free(adjMatrixGraph);
        free(edgeListGraph->edges);
        free(edgeListGraph);
        return -1;
    }

    // Take destination IP address as input from the user
    char destIP[16]; // Buffer to hold IP address
    printf("Enter the destination IP address: ");
    scanf("%15s", destIP);

    // Map the destination IP address to a vertex index
    int sink = mapIPToVertex(destIP);
    if (sink == -1) {
        fprintf(stderr, "Destination IP mapping failed.\n");
        for (int i = 0; i < adjMatrixGraph->V; i++) {
            free(adjMatrixGraph->matrix[i]);
        }
        free(adjMatrixGraph->matrix);
        free(adjMatrixGraph);
        free(edgeListGraph->edges);
        free(edgeListGraph);
        return -1;
    }

    // Run Edmonds-Karp algorithm on the adjacency matrix graph
    int maxFlow = edmondsKarpAdjMatrixMaxBandwidth(adjMatrixGraph, source, sink);
    printf("Maximum Flow (Adjacency Matrix): %d\n", maxFlow);
        for (int i = 0; i < adjMatrixGraph->V; i++) {
    free(adjMatrixGraph->matrix[i]);  // Free individual row first
}
    free(adjMatrixGraph->matrix);        // Then free the matrix pointer itself
    return 0;
}
