#ifndef PASSENGER_H
#define PASSENGER_H

struct Passenger {
    int passenger_id;
    char name[50];
    int source_stop;
    int destination_stop;
};

struct PassengerNode {
    struct Passenger data;
    struct PassengerNode *next;
};

struct PassengerQueue {
    struct PassengerNode *front;
    struct PassengerNode *rear;
};

void initializeQueue(struct PassengerQueue *q);

int enqueue(struct PassengerQueue *q, struct Passenger p);
int dequeue(struct PassengerQueue *q, struct Passenger *p);

void displayQueue(struct PassengerQueue *q);
int searchPassenger(struct PassengerQueue *q, int passenger_id);

#endif
