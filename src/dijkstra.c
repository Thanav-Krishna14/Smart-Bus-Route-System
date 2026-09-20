#include <stdio.h>
#include <limits.h>
#include "../include/dijkstra.h"

int findShortestPath(
    struct Graph *graph,
    int source,
    int destination,
    int path[],
    int *pathLength)
{
    int dist[MAX_STOPS + 1];
    int visited[MAX_STOPS + 1];
    int parent[MAX_STOPS + 1];

    /*
     * Validate stop IDs.
     */
    if (source < 1 || source > MAX_STOPS ||
        destination < 1 || destination > MAX_STOPS)
    {
        printf("Invalid source or destination stop.\n");
        return -1;
    }

    /*
     * Initially:
     * - Every distance is infinity.
     * - No stop is visited.
     * - No parent is assigned.
     */
    for (int i = 0; i <= MAX_STOPS; i++)
    {
        dist[i] = INT_MAX;
        visited[i] = 0;
        parent[i] = -1;
    }

    /*
     * Distance from source to itself is zero.
     */
    dist[source] = 0;

    /*
     * Dijkstra's main loop.
     */
    for (int count = 1; count <= MAX_STOPS; count++)
    {
        int current = -1;
        int minimumDistance = INT_MAX;

        /*
         * Find the unvisited stop
         * with the smallest distance.
         */
        for (int i = 1; i <= MAX_STOPS; i++)
        {
            if (!visited[i] &&
                dist[i] < minimumDistance)
            {
                minimumDistance = dist[i];
                current = i;
            }
        }

        /*
         * No reachable unvisited stop remains.
         */
        if (current == -1)
        {
            break;
        }

        /*
         * Mark the selected stop as visited.
         */
        visited[current] = 1;

        /*
         * We can stop once the destination
         * has been finalized.
         */
        if (current == destination)
        {
            break;
        }

        /*
         * Examine all neighboring stops.
         */
        struct AdjNode *neighbor =
            graph->adjList[current];

        while (neighbor != NULL)
        {
            int nextStop = neighbor->stop_id;
            int edgeDistance = neighbor->distance;

            /*
             * Relax the edge.
             */
            if (!visited[nextStop] &&
                dist[current] != INT_MAX &&
                dist[current] + edgeDistance <
                    dist[nextStop])
            {
                dist[nextStop] =
                    dist[current] + edgeDistance;

                parent[nextStop] = current;
            }

            neighbor = neighbor->next;
        }
    }

    /*
     * Destination could not be reached.
     */
    if (dist[destination] == INT_MAX)
    {
        *pathLength = 0;

        printf("No route exists between the selected stops.\n");

        return -1;
    }

    /*
     * Reconstruct the path by following
     * parent pointers backwards.
     */
    int length = 0;
    int current = destination;

    while (current != -1)
    {
        path[length] = current;
        length++;

        if (length >= MAX_PATH)
        {
            printf("Path is too long.\n");
            *pathLength = 0;
            return -1;
        }

        current = parent[current];
    }

    /*
     * The path was constructed backwards.
     * Reverse it to get source -> destination.
     */
    for (int i = 0; i < length / 2; i++)
    {
        int temp = path[i];

        path[i] =
            path[length - 1 - i];

        path[length - 1 - i] =
            temp;
    }

    *pathLength = length;

    return dist[destination];
}
