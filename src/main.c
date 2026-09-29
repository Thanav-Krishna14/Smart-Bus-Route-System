#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/bus.h"
#include "../include/stop.h"
#include "../include/route.h"
#include "../include/passenger.h"
#include "../include/schedule.h"
#include "../include/graph.h"
#include "../include/dijkstra.h"
#include "../include/file_handler.h"

#define MAX_INPUT 100

/* ==================== DATA STORAGE ==================== */

struct Bus buses[MAX_BUSES];
int busCount = 0;

struct Stop stops[MAX_STOPS];
int stopCount = 0;

struct Route routes[MAX_ROUTES];
int routeCount = 0;

struct Schedule schedules[MAX_SCHEDULES];
int scheduleCount = 0;

struct PassengerQueue passengerQueue;
struct Graph graph;

/* ==================== HELPER FUNCTIONS ==================== */

int getStopIndex(int stop_id)
{
    return searchStop(stops, stopCount, stop_id);
}

int getBusIndex(int bus_id)
{
    return searchBus(buses, busCount, bus_id);
}

int getRouteIndex(int route_id)
{
    return searchRoute(routes, routeCount, route_id);
}

void saveData(void)
{
    int success = 1;

    if (!saveBuses(buses, busCount))
    {
        success = 0;
    }

    if (!saveStops(stops, stopCount))
    {
        success = 0;
    }

    if (!saveRoutes(routes, routeCount))
    {
        success = 0;
    }

    if (!saveSchedules(schedules, scheduleCount))
    {
        success = 0;
    }

    if (!saveConnections(&graph))
    {
        success = 0;
    }

    if (success)
    {
        printf("All data saved successfully.\n");
    }
    else
    {
        printf("Some data could not be saved.\n");
    }
}

void cleanupData(void)
{
    /*
     * Free all route linked-list nodes.
     */
    for (int i = 0; i < routeCount; i++)
    {
        struct RouteNode *current = routes[i].head;

        while (current != NULL)
        {
            struct RouteNode *temp = current;
            current = current->next;
            free(temp);
        }

        routes[i].head = NULL;
    }

    /*
     * Free passenger queue nodes.
     */
    struct Passenger passenger;

    while (passengerQueue.front != NULL)
    {
        dequeue(&passengerQueue, &passenger);
    }

    /*
     * Free graph adjacency-list nodes.
     */
    freeGraph(&graph);
}

/* ==================== BUS MENU ==================== */

void busMenu(void)
{
    int choice;
    int bus_id;

    do
    {
        printf("\n========== BUS MANAGEMENT ==========\n");
        printf("1. Add Bus\n");
        printf("2. Delete Bus\n");
        printf("3. Update Bus\n");
        printf("4. View Buses\n");
        printf("5. Search Bus\n");
        printf("6. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addBus(buses, &busCount);
                break;

            case 2:
                printf("Enter Bus ID to delete: ");
                scanf("%d", &bus_id);
                deleteBus(buses, &busCount, bus_id);
                break;

            case 3:
                {
                int new_route_id;

    printf("Enter Bus ID to update: ");
    scanf("%d", &bus_id);

    printf("Enter new Route ID: ");
    scanf("%d", &new_route_id);

    if (searchRoute(routes, routeCount, new_route_id) == -1)
    {
        printf("Route not found.\n");
        break;
    }

    updateBus(
        buses,
        busCount,
        bus_id,
        new_route_id
    );

    break;
                }

            case 4:
                displayBuses(buses, busCount);
                break;

            case 5:
                printf("Enter Bus ID to search: ");
                scanf("%d", &bus_id);

                if (searchBus(buses, busCount, bus_id) != -1)
                {
                    printf("Bus found.\n");
                }
                else
                {
                    printf("Bus not found.\n");
                }

                break;

            case 6:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 6);
}

/* ==================== STOP MENU ==================== */

void stopMenu(void)
{
    int choice;
    int stop_id;

    do
    {
        printf("\n========== STOP MANAGEMENT ==========\n");
        printf("1. Add Stop\n");
        printf("2. Delete Stop\n");
        printf("3. Update Stop\n");
        printf("4. View Stops\n");
        printf("5. Search Stop\n");
        printf("6. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addStop(stops, &stopCount);
                break;

            case 2:
                printf("Enter Stop ID to delete: ");
                scanf("%d", &stop_id);
                deleteStop(stops, &stopCount, stop_id);
                break;

            case 3:
                printf("Enter Stop ID to update: ");
                scanf("%d", &stop_id);
                updateStop(stops, stopCount, stop_id);
                break;

            case 4:
                displayStops(stops, stopCount);
                break;

            case 5:
                printf("Enter Stop ID to search: ");
                scanf("%d", &stop_id);

                if (searchStop(stops, stopCount, stop_id) != -1)
                {
                    printf("Stop found.\n");
                }
                else
                {
                    printf("Stop not found.\n");
                }

                break;

            case 6:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 6);
}

/* ==================== ROUTE MENU ==================== */

void routeMenu(void)
{
    int choice;
    int route_id;
    int stop_id;
    int routeIndex;

    do
    {
        printf("\n========== ROUTE MANAGEMENT ==========\n");
        printf("1. Add Route\n");
        printf("2. Delete Route\n");
        printf("3. Update Route\n");
        printf("4. View Routes\n");
        printf("5. Add Stop to Route\n");
        printf("6. Remove Stop from Route\n");
        printf("7. View Stops in Route\n");
        printf("8. Search Route\n");
        printf("9. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addRoute(routes, &routeCount);
                break;

            case 2:
                printf("Enter Route ID to delete: ");
                scanf("%d", &route_id);
                deleteRoute(routes, &routeCount, route_id);
                break;

            case 3:
                printf("Enter Route ID to update: ");
                scanf("%d", &route_id);
                updateRoute(routes, routeCount, route_id);
                break;

            case 4:
                displayRoutes(routes, routeCount);
                break;

            case 5:
                printf("Enter Route ID: ");
                scanf("%d", &route_id);

                routeIndex =
                    searchRoute(routes, routeCount, route_id);

                if (routeIndex == -1)
                {
                    printf("Route not found.\n");
                    break;
                }

                printf("Enter Stop ID to add: ");
                scanf("%d", &stop_id);

                if (searchStop(stops, stopCount, stop_id) == -1)
                {
                    printf("Stop not found.\n");
                    break;
                }

                addStopToRoute(
                    &routes[routeIndex],
                    stop_id
                );

                break;

            case 6:
                printf("Enter Route ID: ");
                scanf("%d", &route_id);

                routeIndex =
                    searchRoute(routes, routeCount, route_id);

                if (routeIndex == -1)
                {
                    printf("Route not found.\n");
                    break;
                }

                printf("Enter Stop ID to remove: ");
                scanf("%d", &stop_id);

                deleteStopFromRoute(
                    &routes[routeIndex],
                    stop_id
                );

                break;

            case 7:
                printf("Enter Route ID: ");
                scanf("%d", &route_id);

                routeIndex =
                    searchRoute(routes, routeCount, route_id);

                if (routeIndex == -1)
                {
                    printf("Route not found.\n");
                    break;
                }

                printf(
                    "\nStops in Route %d (%s):\n",
                    routes[routeIndex].route_id,
                    routes[routeIndex].route_name
                );

                displayRouteStops(&routes[routeIndex]);

                break;

            case 8:
                printf("Enter Route ID to search: ");
                scanf("%d", &route_id);

                if (searchRoute(routes, routeCount, route_id) != -1)
                {
                    printf("Route found.\n");
                }
                else
                {
                    printf("Route not found.\n");
                }

                break;

            case 9:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 9);
}

/* ==================== SCHEDULE MENU ==================== */

void scheduleMenu(void)
{
    int choice;
    int schedule_id;

    do
    {
        printf("\n========== SCHEDULE MANAGEMENT ==========\n");
        printf("1. Add Schedule\n");
        printf("2. Delete Schedule\n");
        printf("3. Update Schedule\n");
        printf("4. View Schedules\n");
        printf("5. Search Schedule\n");
        printf("6. Sort by Departure Time\n");
        printf("7. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
            {
                int bus_id;
                int route_id;

                /*
                 * Validate Bus and Route before adding
                 * the schedule.
                 */
                printf("Enter Bus ID: ");
                scanf("%d", &bus_id);

                if (searchBus(buses, busCount, bus_id) == -1)
                {
                    printf("Bus not found.\n");
                    break;
                }

                printf("Enter Route ID: ");
                scanf("%d", &route_id);

                if (searchRoute(routes, routeCount, route_id) == -1)
                {
                    printf("Route not found.\n");
                    break;
                }

                /*
                 * addSchedule() asks for these IDs again.
                 * This keeps the module independent.
                 */
                addSchedule(
                            schedules,
                            &scheduleCount,
                            bus_id,
                            route_id
                            );

                break;
            }

            case 2:
                printf("Enter Schedule ID to delete: ");
                scanf("%d", &schedule_id);

                deleteSchedule(
                    schedules,
                    &scheduleCount,
                    schedule_id
                );

                break;

            case 3:
                printf("Enter Schedule ID to update: ");
                scanf("%d", &schedule_id);

                updateSchedule(
                    schedules,
                    scheduleCount,
                    schedule_id
                );

                break;

            case 4:
                displaySchedules(
                    schedules,
                    scheduleCount
                );

                break;

            case 5:
                printf("Enter Schedule ID to search: ");
                scanf("%d", &schedule_id);

                if (searchSchedule(
                        schedules,
                        scheduleCount,
                        schedule_id) != -1)
                {
                    printf("Schedule found.\n");
                }
                else
                {
                    printf("Schedule not found.\n");
                }

                break;

            case 6:
                sortSchedulesByDeparture(
                    schedules,
                    scheduleCount
                );

                break;

            case 7:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 7);
}

/* ==================== PASSENGER MENU ==================== */

void passengerMenu(void)
{
    int choice;
    int passenger_id;

    do
    {
        printf("\n========== PASSENGER MANAGEMENT ==========\n");
        printf("1. Add Passenger to Queue\n");
        printf("2. Remove Passenger from Queue\n");
        printf("3. View Passenger Queue\n");
        printf("4. Search Passenger\n");
        printf("5. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
            {
                struct Passenger passenger;

                printf("Enter Passenger ID: ");
                scanf("%d", &passenger.passenger_id);

                if (passenger.passenger_id <= 0)
                {
                    printf("Passenger ID must be positive.\n");
                    break;
                }

                if (searchPassenger(
                        &passengerQueue,
                        passenger.passenger_id))
                {
                    printf("Passenger ID already exists in queue.\n");
                    break;
                }

                printf("Enter Passenger Name: ");
                scanf(" %49[^\n]", passenger.name);

                printf("Enter Source Stop ID: ");
                scanf("%d", &passenger.source_stop);

                if (searchStop(
                        stops,
                        stopCount,
                        passenger.source_stop) == -1)
                {
                    printf("Source stop not found.\n");
                    break;
                }

                printf("Enter Destination Stop ID: ");
                scanf("%d", &passenger.destination_stop);

                if (searchStop(
                        stops,
                        stopCount,
                        passenger.destination_stop) == -1)
                {
                    printf("Destination stop not found.\n");
                    break;
                }

                if (passenger.source_stop ==
                    passenger.destination_stop)
                {
                    printf(
                        "Source and destination cannot be the same.\n"
                    );
                    break;
                }

                enqueue(&passengerQueue, passenger);

                break;
            }

            case 2:
            {
                struct Passenger passenger;

                dequeue(&passengerQueue, &passenger);

                break;
            }

            case 3:
                displayQueue(&passengerQueue);
                break;

            case 4:
                printf("Enter Passenger ID to search: ");
                scanf("%d", &passenger_id);

                if (searchPassenger(
                        &passengerQueue,
                        passenger_id))
                {
                    printf("Passenger found in queue.\n");
                }
                else
                {
                    printf("Passenger not found in queue.\n");
                }

                break;

            case 5:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 5);
}

/* ==================== NETWORK MENU ==================== */

void networkMenu(void)
{
    int choice;
    int source;
    int destination;
    int distance;

    do
    {
        printf("\n========== NETWORK MANAGEMENT ==========\n");
        printf("1. Add Connection\n");
        printf("2. Remove Connection\n");
        printf("3. Display Network\n");
        printf("4. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                printf("Enter Source Stop ID: ");
                scanf("%d", &source);

                if (searchStop(
                        stops,
                        stopCount,
                        source) == -1)
                {
                    printf("Source stop not found.\n");
                    break;
                }

                printf("Enter Destination Stop ID: ");
                scanf("%d", &destination);

                if (searchStop(
                        stops,
                        stopCount,
                        destination) == -1)
                {
                    printf("Destination stop not found.\n");
                    break;
                }

                printf("Enter Distance (km): ");
                scanf("%d", &distance);

                addConnection(
                    &graph,
                    source,
                    destination,
                    distance
                );

                break;

            case 2:
                printf("Enter Source Stop ID: ");
                scanf("%d", &source);

                printf("Enter Destination Stop ID: ");
                scanf("%d", &destination);

                removeConnection(
                    &graph,
                    source,
                    destination
                );

                break;

            case 3:
                displayGraph(&graph);
                break;

            case 4:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 4);
}

/* ==================== ADMIN MENU ==================== */

void adminMenu(void)
{
    int choice;

    do
    {
        printf("\n========== ADMIN MENU ==========\n");
        printf("1. Bus Management\n");
        printf("2. Stop Management\n");
        printf("3. Route Management\n");
        printf("4. Schedule Management\n");
        printf("5. Passenger Management\n");
        printf("6. Network Management\n");
        printf("7. View Network\n");
        printf("8. Save Data\n");
        printf("9. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                busMenu();
                break;

            case 2:
                stopMenu();
                break;

            case 3:
                routeMenu();
                break;

            case 4:
                scheduleMenu();
                break;

            case 5:
                passengerMenu();
                break;

            case 6:
                networkMenu();
                break;

            case 7:
                displayGraph(&graph);
                break;

            case 8:
                saveData();
                break;

            case 9:
                printf("Admin logged out.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 9);
}

/* ==================== USER FUNCTIONS ==================== */

void findRoute(void)
{
    int source;
    int destination;

    printf("Enter Source Stop ID: ");
    scanf("%d", &source);

    if (searchStop(stops, stopCount, source) == -1)
    {
        printf("Source stop not found.\n");
        return;
    }

    printf("Enter Destination Stop ID: ");
    scanf("%d", &destination);

    if (searchStop(stops, stopCount, destination) == -1)
    {
        printf("Destination stop not found.\n");
        return;
    }

    if (source == destination)
    {
        printf("Source and destination are the same.\n");
        return;
    }

    int routeIndex =
        findRouteBetweenStops(
            routes,
            routeCount,
            source,
            destination
        );

    if (routeIndex == -1)
    {
        printf("No direct bus route found between these stops.\n");
        return;
    }

    printf(
        "\nRoute found:\n"
        "Route ID   : %d\n"
        "Route Name : %s\n"
        "Stops      : ",
        routes[routeIndex].route_id,
        routes[routeIndex].route_name
    );

    displayRouteStops(&routes[routeIndex]);
}

void findShortestRoute(void)
{
    int source;
    int destination;

    int path[MAX_DIJKSTRA_PATH];
    int pathLength = 0;

    printf("Enter Source Stop ID: ");
    scanf("%d", &source);

    if (searchStop(stops, stopCount, source) == -1)
    {
        printf("Source stop not found.\n");
        return;
    }

    printf("Enter Destination Stop ID: ");
    scanf("%d", &destination);

    if (searchStop(stops, stopCount, destination) == -1)
    {
        printf("Destination stop not found.\n");
        return;
    }

    if (source == destination)
    {
        printf("Source and destination are the same.\n");
        return;
    }

    int distance =
        findShortestPath(
            &graph,
            source,
            destination,
            path,
            &pathLength
        );

    if (distance == -1)
    {
        return;
    }

    printf("\nShortest Route:\n");

    for (int i = 0; i < pathLength; i++)
    {
        int index = getStopIndex(path[i]);

        if (index != -1)
        {
            printf(
                "%d (%s)",
                stops[index].stop_id,
                stops[index].stop_name
            );
        }
        else
        {
            printf("%d", path[i]);
        }

        if (i < pathLength - 1)
        {
            printf(" -> ");
        }
    }

    printf("\nTotal Distance: %d km\n", distance);
}

void viewBusSchedule(void)
{
    int bus_id;

    printf("Enter Bus ID: ");
    scanf("%d", &bus_id);

    if (getBusIndex(bus_id) == -1)
    {
        printf("Bus not found.\n");
        return;
    }

    displaySchedulesForBus(
        schedules,
        scheduleCount,
        bus_id
    );
}

void joinPassengerQueue(void)
{
    struct Passenger passenger;

    printf("Enter Passenger ID: ");
    scanf("%d", &passenger.passenger_id);

    if (passenger.passenger_id <= 0)
    {
        printf("Passenger ID must be positive.\n");
        return;
    }

    if (searchPassenger(
            &passengerQueue,
            passenger.passenger_id))
    {
        printf("Passenger ID already exists in queue.\n");
        return;
    }

    printf("Enter Passenger Name: ");
    scanf(" %49[^\n]", passenger.name);

    printf("Enter Source Stop ID: ");
    scanf("%d", &passenger.source_stop);

    if (searchStop(
            stops,
            stopCount,
            passenger.source_stop) == -1)
    {
        printf("Source stop not found.\n");
        return;
    }

    printf("Enter Destination Stop ID: ");
    scanf("%d", &passenger.destination_stop);

    if (searchStop(
            stops,
            stopCount,
            passenger.destination_stop) == -1)
    {
        printf("Destination stop not found.\n");
        return;
    }

    if (passenger.source_stop ==
        passenger.destination_stop)
    {
        printf(
            "Source and destination cannot be the same.\n"
        );
        return;
    }

    enqueue(&passengerQueue, passenger);
}

/* ==================== USER MENU ==================== */

void userMenu(void)
{
    int choice;

    do
    {
        printf("\n========== USER MENU ==========\n");
        printf("1. View Buses\n");
        printf("2. View Bus Stops\n");
        printf("3. View Routes\n");
        printf("4. Find Route\n");
        printf("5. Find Shortest Route\n");
        printf("6. View Bus Schedule\n");
        printf("7. Join Passenger Queue\n");
        printf("8. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                displayBuses(buses, busCount);
                break;

            case 2:
                displayStops(stops, stopCount);
                break;

            case 3:
                displayRoutes(routes, routeCount);
                break;

            case 4:
                findRoute();
                break;

            case 5:
                findShortestRoute();
                break;

            case 6:
                viewBusSchedule();
                break;

            case 7:
                joinPassengerQueue();
                break;

            case 8:
                printf("User logged out.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 8);
}

/* ==================== MAIN ==================== */

int main(void)
{
    int choice;

    /*
     * Initialize dynamic data structures.
     */
    initializeQueue(&passengerQueue);
    initializeGraph(&graph);

    /*
     * Load saved data.
     *
     * Stops are loaded before routes and connections
     * because routes and connections refer to stop IDs.
     */
    loadStops(stops, &stopCount);
    loadRoutes(routes, &routeCount);
    loadBuses(buses, &busCount);
    loadSchedules(schedules, &scheduleCount);
    loadConnections(&graph);

    printf("\n============================================\n");
    printf("       SMART BUS ROUTE SYSTEM\n");
    printf("============================================\n");

    do
    {
        printf("\n========== MAIN MENU ==========\n");
        printf("1. Admin\n");
        printf("2. User\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                adminMenu();
                break;

            case 2:
                userMenu();
                break;

            case 3:
                printf("\nSaving data...\n");
                saveData();

                printf("Cleaning up memory...\n");
                cleanupData();

                printf("Thank you for using Smart Bus Route System.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 3);

    return 0;
}
