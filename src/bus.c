#include <stdio.h>
#include "../include/bus.h"

int searchBus(struct Bus buses[], int count, int bus_id)
{
    for (int i = 0; i < count; i++)
    {
        if (buses[i].bus_id == bus_id)
        {
            return i;
        }
    }

    return -1;
}

int addBus(struct Bus buses[], int *count)
{
    if (*count >= MAX_BUSES)
    {
        printf("Bus storage is full.\n");
        return 0;
    }

    int bus_id;

    printf("Enter Bus ID: ");
    scanf("%d", &bus_id);

    if (searchBus(buses, *count, bus_id) != -1)
    {
        printf("Bus ID already exists.\n");
        return 0;
    }

    printf("Enter Capacity: ");
    scanf("%d", &buses[*count].capacity);

    if (buses[*count].capacity <= 0 ||
        buses[*count].capacity > MAX_BUS_CAPACITY)
    {
        printf("Capacity must be between 1 and %d.\n",
               MAX_BUS_CAPACITY);
        return 0;
    }

    printf("Enter Route ID: ");
    scanf("%d", &buses[*count].route_id);

    if (buses[*count].route_id <= 0)
    {
        printf("Invalid Route ID.\n");
        return 0;
    }

    buses[*count].bus_id = bus_id;

    (*count)++;

    printf("Bus added successfully.\n");

    return 1;
}

int deleteBus(struct Bus buses[], int *count, int bus_id)
{
    int index = searchBus(buses, *count, bus_id);

    if (index == -1)
    {
        printf("Bus not found.\n");
        return 0;
    }

    for (int i = index; i < *count - 1; i++)
    {
        buses[i] = buses[i + 1];
    }

    (*count)--;

    printf("Bus deleted successfully.\n");

    return 1;
}

int updateBus(struct Bus buses[], int count, int bus_id)
{
    int index = searchBus(buses, count, bus_id);

    if (index == -1)
    {
        printf("Bus not found.\n");
        return 0;
    }

    printf("Enter new capacity: ");
    scanf("%d", &buses[index].capacity);

    if (buses[index].capacity <= 0 ||
        buses[index].capacity > MAX_BUS_CAPACITY)
    {
        printf("Capacity must be between 1 and %d.\n",
               MAX_BUS_CAPACITY);
        return 0;
    }

    printf("Enter new Route ID: ");
    scanf("%d", &buses[index].route_id);

    if (buses[index].route_id <= 0)
    {
        printf("Invalid Route ID.\n");
        return 0;
    }

    printf("Bus updated successfully.\n");

    return 1;
}

void displayBuses(struct Bus buses[], int count)
{
    if (count == 0)
    {
        printf("No buses available.\n");
        return;
    }

    printf("\n%-10s %-10s %-10s\n",
           "Bus ID", "Capacity", "Route ID");

    printf("--------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-10d %-10d %-10d\n",
               buses[i].bus_id,
               buses[i].capacity,
               buses[i].route_id);
    }
}
