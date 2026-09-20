#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

#define MAX_DIJKSTRA_PATH 100

int findShortestPath(
    struct Graph *graph,
    int source,
    int destination,
    int path[],
    int *pathLength
);

#endif
