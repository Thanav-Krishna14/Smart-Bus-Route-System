#include <stdio.h>
#include <stdlib.h>
#include "../include/graph.h"

static int isValidStopId(int stop_id)
{
    return stop_id >= 1 && stop_id <= MAX_STOPS;
}

static int findEdge(
    struct Graph *graph,
    int source,
    int destination)
{
    struct AdjNode *current = graph->adjList[source];

    while (current != NULL)
    {
        if (current->stop_id == destination)
        {
            return 1;
        }

        current = current->next;
    }

    return 0;
}

void initializeGraph(struct Graph *graph)
{
    for (int i = 0; i <= MAX_STOPS; i++)
    {
        graph->adjList[i] = NULL;
    }
}

int addConnection(
    struct Graph *graph,
    int source,
    int destination,
    int distance)
{
    if (!isValidStopId(source) ||
        !isValidStopId(destination))
    {
        printf("Invalid stop ID.\n");
        return 0;
    }

    if (source == destination)
    {
        printf("Source and destination cannot be the same.\n");
        return 0;
    }

    if (distance <= 0)
    {
        printf("Distance must be greater than 0.\n");
        return 0;
    }

    /*
     * Check whether the connection already exists.
     */
    if (findEdge(graph, source, destination))
    {
        printf("Connection already exists.\n");
        return 0;
    }

    /*
     * Create node for source -> destination.
     */
    struct AdjNode *newNode =
        (struct AdjNode *)malloc(sizeof(struct AdjNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return 0;
    }

    newNode->stop_id = destination;
    newNode->distance = distance;

    newNode->next = graph->adjList[source];
    graph->adjList[source] = newNode;

    /*
     * Since the bus connection is bidirectional,
     * create the reverse edge as well.
     */
    newNode =
        (struct AdjNode *)malloc(sizeof(struct AdjNode));

    if (newNode == NULL)
    {
        /*
         * Remove the first edge if the second allocation fails.
         */
        struct AdjNode *temp =
            graph->adjList[source];

        graph->adjList[source] =
            temp->next;

        free(temp);

        printf("Memory allocation failed.\n");
        return 0;
    }

    newNode->stop_id = source;
    newNode->distance = distance;

    newNode->next = graph->adjList[destination];
    graph->adjList[destination] = newNode;

    printf("Connection added successfully.\n");

    return 1;
}

int removeConnection(
    struct Graph *graph,
    int source,
    int destination)
{
    if (!isValidStopId(source) ||
        !isValidStopId(destination))
    {
        printf("Invalid stop ID.\n");
        return 0;
    }

    if (source == destination)
    {
        printf("Source and destination cannot be the same.\n");
        return 0;
    }

    /*
     * Remove source -> destination.
     */
    struct AdjNode *current =
        graph->adjList[source];

    struct AdjNode *previous = NULL;

    while (current != NULL &&
           current->stop_id != destination)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Connection not found.\n");
        return 0;
    }

    if (previous == NULL)
    {
        graph->adjList[source] =
            current->next;
    }
    else
    {
        previous->next =
            current->next;
    }

    free(current);

    /*
     * Remove destination -> source.
     */
    current = graph->adjList[destination];
    previous = NULL;

    while (current != NULL &&
           current->stop_id != source)
    {
        previous = current;
        current = current->next;
    }

    if (current != NULL)
    {
        if (previous == NULL)
        {
            graph->adjList[destination] =
                current->next;
        }
        else
        {
            previous->next =
                current->next;
        }

        free(current);
    }

    printf("Connection removed successfully.\n");

    return 1;
}

void displayGraph(struct Graph *graph)
{
    int found = 0;

    printf("\n========== BUS NETWORK ==========\n");

    for (int i = 1; i <= MAX_STOPS; i++)
    {
        if (graph->adjList[i] != NULL)
        {
            found = 1;

            printf("Stop %d: ", i);

            struct AdjNode *current =
                graph->adjList[i];

            while (current != NULL)
            {
                printf(
                    "-> Stop %d (%d km)",
                    current->stop_id,
                    current->distance
                );

                if (current->next != NULL)
                {
                    printf(" | ");
                }

                current = current->next;
            }

            printf("\n");
        }
    }

    if (!found)
    {
        printf("No connections available.\n");
    }

    printf("=================================\n");
}

void freeGraph(struct Graph *graph)
{
    for (int i = 0; i <= MAX_STOPS; i++)
    {
        struct AdjNode *current =
            graph->adjList[i];

        while (current != NULL)
        {
            struct AdjNode *temp =
                current;

            current = current->next;

            free(temp);
        }

        graph->adjList[i] = NULL;
    }
}
