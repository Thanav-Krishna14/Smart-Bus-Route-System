#ifndef STOP_H
#define STOP_H

#define MAX_STOPS 100

struct Stop {
    int stop_id;
    char stop_name[50];
};

int addStop(struct Stop stops[], int *count);
int deleteStop(struct Stop stops[], int *count, int stop_id);
int updateStop(struct Stop stops[], int count, int stop_id);
void displayStops(struct Stop stops[], int count);
int searchStop(struct Stop stops[], int count, int stop_id);

#endif
