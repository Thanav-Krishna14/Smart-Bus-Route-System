#include <stdio.h>
#include <stdlib.h>

#include "../include/passenger.h"

void initializeQueue(struct PassengerQueue *q)
{
    q->front = NULL;
    q->rear = NULL;
}

int enqueue(struct PassengerQueue *q, struct Passenger p)
{
    struct PassengerNode *newNode =
        (struct PassengerNode *)malloc(sizeof(struct PassengerNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return 0;
    }

    newNode->data = p;
    newNode->next = NULL;

    if (q->rear == NULL)
    {
        q->front = newNode;
        q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }

    printf("Passenger added to queue successfully.\n");

    return 1;
}

int dequeue(struct PassengerQueue *q, struct Passenger *p)
{
    if (q->front == NULL)
    {
        printf("Passenger queue is empty.\n");
        return 0;
    }

    struct PassengerNode *temp = q->front;

    *p = temp->data;

    q->front = q->front->next;

    if (q->front == NULL)
    {
        q->rear = NULL;
    }

    free(temp);

    printf("Passenger removed from queue.\n");

    return 1;
}

void displayQueue(struct PassengerQueue *q)
{
    if (q->front == NULL)
    {
        printf("Passenger queue is empty.\n");
        return;
    }

    struct PassengerNode *current = q->front;

    printf("\n%-15s %-20s %-15s %-15s\n",
           "Passenger ID",
           "Name",
           "Source",
           "Destination");

    printf("---------------------------------------------------------------------\n");

    while (current != NULL)
    {
        printf("%-15d %-20s %-15d %-15d\n",
               current->data.passenger_id,
               current->data.name,
               current->data.source_stop,
               current->data.destination_stop);

        current = current->next;
    }
}

int searchPassenger(struct PassengerQueue *q, int passenger_id)
{
    struct PassengerNode *current = q->front;

    while (current != NULL)
    {
        if (current->data.passenger_id == passenger_id)
        {
            return 1;
        }

        current = current->next;
    }

    return 0;
}
