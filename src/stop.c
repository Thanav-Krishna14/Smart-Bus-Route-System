#include <stdio.h>
#include <string.h>
#include "../include/stop.h"

int searchStop(struct Stop stops[], int count, int stop_id)
{
    for (int i = 0; i < count; i++)
    {
        if (stops[i].stop_id == stop_id)
        {
            return i;
        }
    }

    return -1;
}

int addStop(struct Stop stops[], int *count)
{
    if (*count >= MAX_STOPS)
    {
        printf("Stop storage is full.\n");
        return 0;
    }

    int stop_id;
    char stop_name[50];

    printf("Enter Stop ID: ");
    scanf("%d", &stop_id);

    if (stop_id <= 0)
    {
        printf("Stop ID must be positive.\n");
        return 0;
    }

    if (searchStop(stops, *count, stop_id) != -1)
    {
        printf("Stop ID already exists.\n");
        return 0;
    }

    printf("Enter Stop Name: ");
    scanf(" %49[^\n]", stop_name);

    if (strlen(stop_name) == 0)
    {
        printf("Stop name cannot be empty.\n");
        return 0;
    }

    stops[*count].stop_id = stop_id;
    strcpy(stops[*count].stop_name, stop_name);

    (*count)++;

    printf("Stop added successfully.\n");

    return 1;
}

int deleteStop(struct Stop stops[], int *count, int stop_id)
{
    int index = searchStop(stops, *count, stop_id);

    if (index == -1)
    {
        printf("Stop not found.\n");
        return 0;
    }

    for (int i = index; i < *count - 1; i++)
    {
        stops[i] = stops[i + 1];
    }

    (*count)--;

    printf("Stop deleted successfully.\n");

    return 1;
}

int updateStop(struct Stop stops[], int count, int stop_id)
{
    int index = searchStop(stops, count, stop_id);

    if (index == -1)
    {
        printf("Stop not found.\n");
        return 0;
    }

    char new_name[50];

    printf("Enter new Stop Name: ");
    scanf(" %49[^\n]", new_name);

    if (strlen(new_name) == 0)
    {
        printf("Stop name cannot be empty.\n");
        return 0;
    }

    strcpy(stops[index].stop_name, new_name);

    printf("Stop updated successfully.\n");

    return 1;
}

void displayStops(struct Stop stops[], int count)
{
    if (count == 0)
    {
        printf("No stops available.\n");
        return;
    }

    printf("\n%-10s %-30s\n", "Stop ID", "Stop Name");
    printf("------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-10d %-30s\n",
               stops[i].stop_id,
               stops[i].stop_name);
    }
}
