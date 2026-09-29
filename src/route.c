
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/route.h"

int searchRoute(struct Route routes[], int count, int route_id)
{
    for (int i = 0; i < count; i++)
    {
        if (routes[i].route_id == route_id)
        {
            return i;
        }
    }

    return -1;
}

int addRoute(struct Route routes[], int *count)
{
    if (*count >= MAX_ROUTES)
    {
        printf("Route storage is full.\n");
        return 0;
    }

    int route_id;
    char route_name[50];

    printf("Enter Route ID: ");
    scanf("%d", &route_id);

    if (route_id <= 0)
    {
        printf("Route ID must be positive.\n");
        return 0;
    }

    if (searchRoute(routes, *count, route_id) != -1)
    {
        printf("Route ID already exists.\n");
        return 0;
    }

    printf("Enter Route Name: ");
    scanf(" %49[^\n]", route_name);

    if (strlen(route_name) == 0)
    {
        printf("Route name cannot be empty.\n");
        return 0;
    }

    routes[*count].route_id = route_id;
    strcpy(routes[*count].route_name, route_name);
    routes[*count].head = NULL;

    (*count)++;

    printf("Route added successfully.\n");

    return 1;
}

int deleteRoute(struct Route routes[], int *count, int route_id)
{
    int index = searchRoute(routes, *count, route_id);

    if (index == -1)
    {
        printf("Route not found.\n");
        return 0;
    }

    struct RouteNode *current = routes[index].head;

    while (current != NULL)
    {
        struct RouteNode *temp = current;
        current = current->next;
        free(temp);
    }

    for (int i = index; i < *count - 1; i++)
    {
        routes[i] = routes[i + 1];
    }

    (*count)--;

    printf("Route deleted successfully.\n");

    return 1;
}

int updateRoute(struct Route routes[], int count, int route_id)
{
    int index = searchRoute(routes, count, route_id);

    if (index == -1)
    {
        printf("Route not found.\n");
        return 0;
    }

    char new_name[50];

    printf("Enter new Route Name: ");
    scanf(" %49[^\n]", new_name);

    if (strlen(new_name) == 0)
    {
        printf("Route name cannot be empty.\n");
        return 0;
    }

    strcpy(routes[index].route_name, new_name);

    printf("Route updated successfully.\n");

    return 1;
}

void displayRouteStops(struct Route *route)
{
    if (route == NULL)
    {
        printf("Invalid route.\n");
        return;
    }

    if (route->head == NULL)
    {
        printf("No stops are assigned to this route.\n");
        return;
    }

    struct RouteNode *current = route->head;

    while (current != NULL)
    {
        printf("%d", current->stop_id);

        if (current->next != NULL)
        {
            printf(" -> ");
        }

        current = current->next;
    }

    printf("\n");
}

void displayRoutes(struct Route routes[], int count)
{
    if (count == 0)
    {
        printf("No routes available.\n");
        return;
    }

    printf("\n%-10s %-30s %-30s\n",
           "Route ID", "Route Name", "Stops");

    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-10d %-30s ",
               routes[i].route_id,
               routes[i].route_name);

        if (routes[i].head == NULL)
        {
            printf("No stops");
        }
        else
        {
            struct RouteNode *current = routes[i].head;

            while (current != NULL)
            {
                printf("%d", current->stop_id);

                if (current->next != NULL)
                {
                    printf(" -> ");
                }

                current = current->next;
            }
        }

        printf("\n");
    }
}

int addStopToRoute(struct Route *route, int stop_id)
{
    if (route == NULL)
    {
        printf("Invalid route.\n");
        return 0;
    }

    struct RouteNode *newNode =
        (struct RouteNode *)malloc(sizeof(struct RouteNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return 0;
    }

    newNode->stop_id = stop_id;
    newNode->next = NULL;

    if (route->head == NULL)
    {
        route->head = newNode;

        printf("Stop added to route.\n");

        return 1;
    }

    struct RouteNode *current = route->head;

    while (current != NULL)
    {
        if (current->stop_id == stop_id)
        {
            printf("Stop already exists in this route.\n");
            free(newNode);
            return 0;
        }

        current = current->next;
    }

    current = route->head;

    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = newNode;

    printf("Stop added to route.\n");

    return 1;
}

int deleteStopFromRoute(struct Route *route, int stop_id)
{
    if (route == NULL || route->head == NULL)
    {
        printf("Route has no stops.\n");
        return 0;
    }

    struct RouteNode *current = route->head;
    struct RouteNode *previous = NULL;

    if (current->stop_id == stop_id)
    {
        route->head = current->next;
        free(current);

        printf("Stop removed from route.\n");

        return 1;
    }

    while (current != NULL &&
           current->stop_id != stop_id)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Stop not found in route.\n");
        return 0;
    }

    previous->next = current->next;

    free(current);

    printf("Stop removed from route.\n");

    return 1;
}

int findRouteBetweenStops(
    struct Route routes[],
    int count,
    int source_stop,
    int destination_stop)
{
    for (int i = 0; i < count; i++)
    {
        struct RouteNode *current = routes[i].head;

        int sourceFound = 0;
        int destinationFound = 0;

        while (current != NULL)
        {
            if (current->stop_id == source_stop)
            {
                sourceFound = 1;
            }

            if (current->stop_id == destination_stop)
            {
                destinationFound = 1;
            }

            current = current->next;
        }

        if (sourceFound && destinationFound)
        {
            return i;
        }
    }

    return -1;
}
