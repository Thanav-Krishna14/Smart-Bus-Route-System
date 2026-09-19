#ifndef SCHEDULE_H
#define SCHEDULE_H

#define MAX_SCHEDULES 100

struct Schedule {
    int schedule_id;
    int bus_id;
    int route_id;
    int departure_time;
    int arrival_time;
};

int addSchedule(struct Schedule schedules[], int *count);

int deleteSchedule(
    struct Schedule schedules[],
    int *count,
    int schedule_id
);

int updateSchedule(
    struct Schedule schedules[],
    int count,
    int schedule_id
);

void displaySchedules(
    struct Schedule schedules[],
    int count
);

int searchSchedule(
    struct Schedule schedules[],
    int count,
    int schedule_id
);

void sortSchedulesByDeparture(
    struct Schedule schedules[],
    int count
);

void displaySchedulesForBus(
    struct Schedule schedules[],
    int count,
    int bus_id
);

#endif
