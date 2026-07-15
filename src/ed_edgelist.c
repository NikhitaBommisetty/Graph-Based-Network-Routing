#include <stdio.h>
#include <stdlib.h>
#include "ed_graph.h"

int main() {
    // Load the graph from a CSV file using an edge list representation
    EdgeListGraph* edgeListGraph = createEdgeListFromCSVByBandwidth("ed_test_case_2.csv");

    // Check if the graph was loaded successfully
    if (!edgeListGraph) {
        fprintf(stderr, "Failed to load graph.\n");
        return -1;
    }

    // Take source IP address as input from the user
    char srcIP[16]; 
    printf("Enter the source IP address: ");
    scanf("%15s", srcIP);

    // Map the source IP address to a vertex index
    int source = mapIPToVertex(srcIP);
    if (source == -1) {
        fprintf(stderr, "Source IP mapping failed.\n");
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
        free(edgeListGraph->edges);
        free(edgeListGraph);
        return -1;
    }


    // Calculate maximum bandwidth using the Edmonds-Karp algorithm on the edge list graph
    int maxflow = edmondsKarpEdgeListMaxBandwidth(edgeListGraph, source, sink);
    printf("Maximum Flow (Edge list): %d\n", maxflow);

    // Free dynamically allocated memory
    free(edgeListGraph->edges);
    free(edgeListGraph);

    return 0;
}
