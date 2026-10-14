#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/file_handler.h"

#define BUS_FILE "data/buses.dat"
#define STOP_FILE "data/stops.dat"
#define ROUTE_FILE "data/routes.dat"
#define SCHEDULE_FILE "data/schedules.dat"
#define CONNECTION_FILE "data/connections.dat"

/*
 * Adds a stop to a route while loading data from a file.
 * This is kept private because it is only needed internally
 * by the file-handling module.
 */
static int appendRouteStop(
    struct Route *route,
    int stop_id)
{
    struct RouteNode *newNode =
        (struct RouteNode *)malloc(sizeof(struct RouteNode));

    if (newNode == NULL)
    {
        return 0;
    }

    newNode->stop_id = stop_id;
    newNode->next = NULL;

    if (route->head == NULL)
    {
        route->head = newNode;
        return 1;
    }

    struct RouteNode *current = route->head;

    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = newNode;

    return 1;
}

/* ==================== BUS FILE HANDLING ==================== */

int saveBuses(struct Bus buses[], int count)
{
    FILE *file = fopen(BUS_FILE, "wb");

    if (file == NULL)
    {
        printf("Unable to open bus data file.\n");
        return 0;
    }

    if (fwrite(&count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return 0;
    }

    if (count > 0)
    {
        if (fwrite(
                buses,
                sizeof(struct Bus),
                count,
                file) != (size_t)count)
        {
            fclose(file);
            return 0;
        }
    }

    fclose(file);

    return 1;
}

int loadBuses(struct Bus buses[], int *count)
{
    FILE *file = fopen(BUS_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    if (fread(count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return 0;
    }

    if (*count < 0 || *count > MAX_BUSES)
    {
        fclose(file);
        *count = 0;
        return 0;
    }

    if (*count > 0)
    {
        if (fread(
                buses,
                sizeof(struct Bus),
                *count,
                file) != (size_t)*count)
        {
            fclose(file);
            *count = 0;
            return 0;
        }
    }

    fclose(file);

    return 1;
}

/* ==================== STOP FILE HANDLING ==================== */

int saveStops(struct Stop stops[], int count)
{
    FILE *file = fopen(STOP_FILE, "wb");

    if (file == NULL)
    {
        printf("Unable to open stop data file.\n");
        return 0;
    }

    if (fwrite(&count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return 0;
    }

    if (count > 0)
    {
        if (fwrite(
                stops,
                sizeof(struct Stop),
                count,
                file) != (size_t)count)
        {
            fclose(file);
            return 0;
        }
    }

    fclose(file);

    return 1;
}

int loadStops(struct Stop stops[], int *count)
{
    FILE *file = fopen(STOP_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    if (fread(count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return 0;
    }

    if (*count < 0 || *count > MAX_STOPS)
    {
        fclose(file);
        *count = 0;
        return 0;
    }

    if (*count > 0)
    {
        if (fread(
                stops,
                sizeof(struct Stop),
                *count,
                file) != (size_t)*count)
        {
            fclose(file);
            *count = 0;
            return 0;
        }
    }

    fclose(file);

    return 1;
}

/* ==================== ROUTE FILE HANDLING ==================== */

int saveRoutes(struct Route routes[], int count)
{
    FILE *file = fopen(ROUTE_FILE, "wb");

    if (file == NULL)
    {
        printf("Unable to open route data file.\n");
        return 0;
    }

    if (fwrite(&count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return 0;
    }

    for (int i = 0; i < count; i++)
    {
        if (fwrite(
                &routes[i].route_id,
                sizeof(int),
                1,
                file) != 1)
        {
            fclose(file);
            return 0;
        }

        if (fwrite(
                routes[i].route_name,
                sizeof(routes[i].route_name),
                1,
                file) != 1)
        {
            fclose(file);
            return 0;
        }

        /*
         * Count the number of stops in this route.
         */
        int stopCount = 0;

        struct RouteNode *current =
            routes[i].head;

        while (current != NULL)
        {
            stopCount++;
            current = current->next;
        }

        if (fwrite(
                &stopCount,
                sizeof(int),
                1,
                file) != 1)
        {
            fclose(file);
            return 0;
        }

        /*
         * Save only stop IDs.
         * We do NOT save the pointer itself.
         */
        current = routes[i].head;

        while (current != NULL)
        {
            if (fwrite(
                    &current->stop_id,
                    sizeof(int),
                    1,
                    file) != 1)
            {
                fclose(file);
                return 0;
            }

            current = current->next;
        }
    }

    fclose(file);

    return 1;
}

int loadRoutes(struct Route routes[], int *count)
{
    FILE *file = fopen(ROUTE_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    if (fread(count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return 0;
    }

    if (*count < 0 || *count > MAX_ROUTES)
    {
        fclose(file);
        *count = 0;
        return 0;
    }

    for (int i = 0; i < *count; i++)
    {
        int stopCount;

        routes[i].head = NULL;

        if (fread(
                &routes[i].route_id,
                sizeof(int),
                1,
                file) != 1)
        {
            fclose(file);
            *count = 0;
            return 0;
        }

        if (fread(
                routes[i].route_name,
                sizeof(routes[i].route_name),
                1,
                file) != 1)
        {
            fclose(file);
            *count = 0;
            return 0;
        }

        if (fread(
                &stopCount,
                sizeof(int),
                1,
                file) != 1)
        {
            fclose(file);
            *count = 0;
            return 0;
        }

        if (stopCount < 0 || stopCount > MAX_STOPS)
        {
            fclose(file);
            *count = 0;
            return 0;
        }

        for (int j = 0; j < stopCount; j++)
        {
            int stop_id;

            if (fread(
                    &stop_id,
                    sizeof(int),
                    1,
                    file) != 1)
            {
                fclose(file);
                *count = 0;
                return 0;
            }

            if (!appendRouteStop(
                    &routes[i],
                    stop_id))
            {
                fclose(file);
                *count = 0;
                return 0;
            }
        }
    }

    fclose(file);

    return 1;
}

/* ================= SCHEDULE FILE HANDLING ================= */

int saveSchedules(
    struct Schedule schedules[],
    int count)
{
    FILE *file = fopen(SCHEDULE_FILE, "wb");

    if (file == NULL)
    {
        printf("Unable to open schedule data file.\n");
        return 0;
    }

    if (fwrite(&count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return 0;
    }

    if (count > 0)
    {
        if (fwrite(
                schedules,
                sizeof(struct Schedule),
                count,
                file) != (size_t)count)
        {
            fclose(file);
            return 0;
        }
    }

    fclose(file);

    return 1;
}

int loadSchedules(
    struct Schedule schedules[],
    int *count)
{
    FILE *file = fopen(SCHEDULE_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    if (fread(count, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return 0;
    }

    if (*count < 0 || *count > MAX_SCHEDULES)
    {
        fclose(file);
        *count = 0;
        return 0;
    }

    if (*count > 0)
    {
        if (fread(
                schedules,
                sizeof(struct Schedule),
                *count,
                file) != (size_t)*count)
        {
            fclose(file);
            *count = 0;
            return 0;
        }
    }

    fclose(file);

    return 1;
}

/* ================= CONNECTION FILE HANDLING ================= */

int saveConnections(struct Graph *graph)
{
    FILE *file = fopen(CONNECTION_FILE, "wb");

    if (file == NULL)
    {
        printf("Unable to open connection data file.\n");
        return 0;
    }

    /*
     * First count unique undirected connections.
     *
     * Since every connection is stored twice:
     *
     * 1 -> 2
     * 2 -> 1
     *
     * we save only the connection where
     * source < destination.
     */
    int connectionCount = 0;

    for (int source = 1;
         source <= MAX_STOPS;
         source++)
    {
        struct AdjNode *current =
            graph->adjList[source];

        while (current != NULL)
        {
            if (source < current->stop_id)
            {
                connectionCount++;
            }

            current = current->next;
        }
    }

    if (fwrite(
            &connectionCount,
            sizeof(int),
            1,
            file) != 1)
    {
        fclose(file);
        return 0;
    }

    /*
     * Save each undirected connection once.
     */
    for (int source = 1;
         source <= MAX_STOPS;
         source++)
    {
        struct AdjNode *current =
            graph->adjList[source];

        while (current != NULL)
        {
            if (source < current->stop_id)
            {
                int destination =
                    current->stop_id;

                int distance =
                    current->distance;

                if (fwrite(
                        &source,
                        sizeof(int),
                        1,
                        file) != 1)
                {
                    fclose(file);
                    return 0;
                }

                if (fwrite(
                        &destination,
                        sizeof(int),
                        1,
                        file) != 1)
                {
                    fclose(file);
                    return 0;
                }

                if (fwrite(
                        &distance,
                        sizeof(int),
                        1,
                        file) != 1)
                {
                    fclose(file);
                    return 0;
                }
            }

            current = current->next;
        }
    }

    fclose(file);

    return 1;
}

int loadConnections(struct Graph *graph)
{
    FILE *file = fopen(CONNECTION_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    int connectionCount;

    if (fread(
            &connectionCount,
            sizeof(int),
            1,
            file) != 1)
    {
        fclose(file);
        return 0;
    }

    if (connectionCount < 0 ||
        connectionCount > MAX_STOPS * MAX_STOPS)
    {
        fclose(file);
        return 0;
    }

    for (int i = 0;
         i < connectionCount;
         i++)
    {
        int source;
        int destination;
        int distance;

        if (fread(
                &source,
                sizeof(int),
                1,
                file) != 1)
        {
            fclose(file);
            return 0;
        }

        if (fread(
                &destination,
                sizeof(int),
                1,
                file) != 1)
        {
            fclose(file);
            return 0;
        }

        if (fread(
                &distance,
                sizeof(int),
                1,
                file) != 1)
        {
            fclose(file);
            return 0;
        }

        /*
         * addConnection() automatically creates
         * both directions.
         */
        if (!addConnection(
                graph,
                source,
                destination,
                distance))
        {
            fclose(file);
            return 0;
        }
    }

    fclose(file);

    return 1;
}
