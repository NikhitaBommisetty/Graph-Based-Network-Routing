#include <stdio.h>
#include <stdlib.h>
#include "ed_graph.h"

int main() {
    // Load the graph from a CSV file using an edge list representation
    EdgeListGraph* edgeListGraph = createEdgeListFromCSVByBandwidth("ed_test_case_2.csv");

    // Convert the edge list graph to an adjacency list graph
    AdjListGraph* adjListGraph = createAdjListFromEdgeList(edgeListGraph);

    // Check if the adjacency list conversion was successful
    if (!adjListGraph) {
        fprintf(stderr, "Failed to convert to adjacency list.\n");
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
        free(edgeListGraph->edges);
        free(edgeListGraph);
        for (int i = 0; i < adjListGraph->V; i++) {
            Node* temp = adjListGraph->head[i];
            while (temp) {
                Node* toFree = temp;
                temp = temp->next;
                free(toFree);
            }
        }
        free(adjListGraph->head);
        free(adjListGraph);
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
        for (int i = 0; i < adjListGraph->V; i++) {
            Node* temp = adjListGraph->head[i];
            while (temp) {
                Node* toFree = temp;
                temp = temp->next;
            }
        }
        free(adjListGraph->head);
        free(adjListGraph);
        return -1;
    }

    // Calculate maximum bandwidth using the Edmonds-Karp algorithm on the adjacency list graph
    int maxFlow = edmondsKarpAdjListMaxBandwidth(adjListGraph, source, sink);
    printf("Maximum Flow (Adjacency List): %d\n", maxFlow);

    // Free dynamically allocated memory for the adjacency list graph
    for (int i = 0; i < adjListGraph->V; i++) {
        Node* temp = adjListGraph->head[i];
        while (temp) {
            Node* toFree = temp;
            temp = temp->next;
        }
    }

    free(adjListGraph->head);
    free(adjListGraph);
    free(edgeListGraph->edges);
    free(edgeListGraph);

    return 0;
}
