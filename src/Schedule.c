#include <stdio.h>
#include "../include/schedule.h"

/*
 * Checks whether time is in valid HHMM format.
 * Examples:
 * 930  -> 09:30
 * 1445 -> 14:45
 */
static int isValidTime(int time)
{
    int hours = time / 100;
    int minutes = time % 100;

    if (hours < 0 || hours > 23)
    {
        return 0;
    }

    if (minutes < 0 || minutes > 59)
    {
        return 0;
    }

    return 1;
}

int searchSchedule(
    struct Schedule schedules[],
    int count,
    int schedule_id)
{
    for (int i = 0; i < count; i++)
    {
        if (schedules[i].schedule_id == schedule_id)
        {
            return i;
        }
    }

    return -1;
}

int addSchedule(struct Schedule schedules[], int *count)
{
    if (*count >= MAX_SCHEDULES)
    {
        printf("Schedule storage is full.\n");
        return 0;
    }

    int schedule_id;

    printf("Enter Schedule ID: ");
    scanf("%d", &schedule_id);

    if (schedule_id <= 0)
    {
        printf("Schedule ID must be positive.\n");
        return 0;
    }

    if (searchSchedule(schedules, *count, schedule_id) != -1)
    {
        printf("Schedule ID already exists.\n");
        return 0;
    }

    printf("Enter Bus ID: ");
    scanf("%d", &schedules[*count].bus_id);

    if (schedules[*count].bus_id <= 0)
    {
        printf("Invalid Bus ID.\n");
        return 0;
    }

    printf("Enter Route ID: ");
    scanf("%d", &schedules[*count].route_id);

    if (schedules[*count].route_id <= 0)
    {
        printf("Invalid Route ID.\n");
        return 0;
    }

    printf("Enter Departure Time (HHMM): ");
    scanf("%d", &schedules[*count].departure_time);

    if (!isValidTime(schedules[*count].departure_time))
    {
        printf("Invalid departure time.\n");
        return 0;
    }

    printf("Enter Arrival Time (HHMM): ");
    scanf("%d", &schedules[*count].arrival_time);

    if (!isValidTime(schedules[*count].arrival_time))
    {
        printf("Invalid arrival time.\n");
        return 0;
    }

    if (schedules[*count].arrival_time <=
        schedules[*count].departure_time)
    {
        printf("Arrival time must be later than departure time.\n");
        return 0;
    }

    schedules[*count].schedule_id = schedule_id;

    (*count)++;

    printf("Schedule added successfully.\n");

    return 1;
}

int deleteSchedule(
    struct Schedule schedules[],
    int *count,
    int schedule_id)
{
    int index = searchSchedule(
        schedules,
        *count,
        schedule_id
    );

    if (index == -1)
    {
        printf("Schedule not found.\n");
        return 0;
    }

    for (int i = index; i < *count - 1; i++)
    {
        schedules[i] = schedules[i + 1];
    }

    (*count)--;

    printf("Schedule deleted successfully.\n");

    return 1;
}

int updateSchedule(
    struct Schedule schedules[],
    int count,
    int schedule_id)
{
    int index = searchSchedule(
        schedules,
        count,
        schedule_id
    );

    if (index == -1)
    {
        printf("Schedule not found.\n");
        return 0;
    }

    printf("Enter new Bus ID: ");
    scanf("%d", &schedules[index].bus_id);

    if (schedules[index].bus_id <= 0)
    {
        printf("Invalid Bus ID.\n");
        return 0;
    }

    printf("Enter new Route ID: ");
    scanf("%d", &schedules[index].route_id);

    if (schedules[index].route_id <= 0)
    {
        printf("Invalid Route ID.\n");
        return 0;
    }

    printf("Enter new Departure Time (HHMM): ");
    scanf("%d", &schedules[index].departure_time);

    if (!isValidTime(schedules[index].departure_time))
    {
        printf("Invalid departure time.\n");
        return 0;
    }

    printf("Enter new Arrival Time (HHMM): ");
    scanf("%d", &schedules[index].arrival_time);

    if (!isValidTime(schedules[index].arrival_time))
    {
        printf("Invalid arrival time.\n");
        return 0;
    }

    if (schedules[index].arrival_time <=
        schedules[index].departure_time)
    {
        printf("Arrival time must be later than departure time.\n");
        return 0;
    }

    printf("Schedule updated successfully.\n");

    return 1;
}

void displaySchedules(
    struct Schedule schedules[],
    int count)
{
    if (count == 0)
    {
        printf("No schedules available.\n");
        return;
    }

    printf(
        "\n%-12s %-10s %-10s %-18s %-18s\n",
        "Schedule ID",
        "Bus ID",
        "Route ID",
        "Departure",
        "Arrival"
    );

    printf(
        "------------------------------------------------------------------------\n"
    );

    for (int i = 0; i < count; i++)
    {
        int departureHour =
            schedules[i].departure_time / 100;

        int departureMinute =
            schedules[i].departure_time % 100;

        int arrivalHour =
            schedules[i].arrival_time / 100;

        int arrivalMinute =
            schedules[i].arrival_time % 100;

        printf(
            "%-12d %-10d %-10d %02d:%02d             %02d:%02d\n",
            schedules[i].schedule_id,
            schedules[i].bus_id,
            schedules[i].route_id,
            departureHour,
            departureMinute,
            arrivalHour,
            arrivalMinute
        );
    }
}

void sortSchedulesByDeparture(
    struct Schedule schedules[],
    int count)
{
    if (count <= 1)
    {
        printf("Not enough schedules to sort.\n");
        return;
    }

    /*
     * Bubble sort:
     * Schedules with earlier departure times
     * are moved toward the beginning.
     */
    for (int i = 0; i < count - 1; i++)
    {
        int swapped = 0;

        for (int j = 0; j < count - i - 1; j++)
        {
            if (schedules[j].departure_time >
                schedules[j + 1].departure_time)
            {
                struct Schedule temp =
                    schedules[j];

                schedules[j] =
                    schedules[j + 1];

                schedules[j + 1] =
                    temp;

                swapped = 1;
            }
        }

        /*
         * If no elements were swapped,
         * the array is already sorted.
         */
        if (!swapped)
        {
            break;
        }
    }

    printf("Schedules sorted by departure time.\n");
}

void displaySchedulesForBus(
    struct Schedule schedules[],
    int count,
    int bus_id)
{
    int found = 0;

    for (int i = 0; i < count; i++)
    {
        if (schedules[i].bus_id == bus_id)
        {
            if (!found)
            {
                printf(
                    "\nSchedules for Bus %d:\n",
                    bus_id
                );

                printf(
                    "%-12s %-10s %-10s %-18s %-18s\n",
                    "Schedule ID",
                    "Bus ID",
                    "Route ID",
                    "Departure",
                    "Arrival"
                );

                printf(
                    "------------------------------------------------------------------------\n"
                );

                found = 1;
            }

            int departureHour =
                schedules[i].departure_time / 100;

            int departureMinute =
                schedules[i].departure_time % 100;

            int arrivalHour =
                schedules[i].arrival_time / 100;

            int arrivalMinute =
                schedules[i].arrival_time % 100;

            printf(
                "%-12d %-10d %-10d %02d:%02d             %02d:%02d\n",
                schedules[i].schedule_id,
                schedules[i].bus_id,
                schedules[i].route_id,
                departureHour,
                departureMinute,
                arrivalHour,
                arrivalMinute
            );
        }
    }

    if (!found)
    {
        printf("No schedules found for Bus %d.\n", bus_id);
    }
}
