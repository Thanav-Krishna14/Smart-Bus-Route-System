#ifndef BUS_H
#define BUS_H

#define MAX_BUSES 100

struct Bus {
    int bus_id;
    int capacity;
    int route_id;
};

int addBus(struct Bus buses[], int *count);
int deleteBus(struct Bus buses[], int *count, int bus_id);
int updateBus(struct Bus buses[], int count, int bus_id);
void displayBuses(struct Bus buses[], int count);
int searchBus(struct Bus buses[], int count, int bus_id);

#endif
