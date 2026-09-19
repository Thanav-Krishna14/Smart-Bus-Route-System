#ifndef ROUTE_H
#define ROUTE_H

#define MAX_ROUTES 100

struct RouteNode {
    int stop_id;
    struct RouteNode *next;
};

struct Route {
    int route_id;
    char route_name[50];
    struct RouteNode *head;
};

int addRoute(struct Route routes[], int *count);
int deleteRoute(struct Route routes[], int *count, int route_id);
int updateRoute(struct Route routes[], int count, int route_id);
void displayRoutes(struct Route routes[], int count);
int searchRoute(struct Route routes[], int count, int route_id);

int addStopToRoute(struct Route *route, int stop_id);
int deleteStopFromRoute(struct Route *route, int stop_id);
void displayRouteStops(struct Route *route);

int findRouteBetweenStops(
    struct Route routes[],
    int count,
    int source_stop,
    int destination_stop
);


#endif
