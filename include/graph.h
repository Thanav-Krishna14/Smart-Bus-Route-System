#ifndef GRAPH_H
#define GRAPH_H

#include "stop.h"

struct AdjNode {
    int stop_id;
    int distance;
    struct AdjNode *next;
};

struct Graph {
    struct AdjNode *adjList[MAX_STOPS + 1];
};

void initializeGraph(struct Graph *graph);

int addConnection(
    struct Graph *graph,
    int source,
    int destination,
    int distance
);

int removeConnection(
    struct Graph *graph,
    int source,
    int destination
);

void displayGraph(struct Graph *graph);

void freeGraph(struct Graph *graph);

#endif
