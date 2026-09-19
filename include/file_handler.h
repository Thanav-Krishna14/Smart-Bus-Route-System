#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "bus.h"
#include "stop.h"
#include "route.h"
#include "schedule.h"
#include "graph.h"

int saveBuses(struct Bus buses[], int count);
int loadBuses(struct Bus buses[], int *count);

int saveStops(struct Stop stops[], int count);
int loadStops(struct Stop stops[], int *count);

int saveRoutes(struct Route routes[], int count);
int loadRoutes(struct Route routes[], int *count);

int saveSchedules(struct Schedule schedules[], int count);
int loadSchedules(struct Schedule schedules[], int *count);

int saveConnections(struct Graph *graph);
int loadConnections(struct Graph *graph);

#endif
