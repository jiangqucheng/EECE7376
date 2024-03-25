/**
 * @file hw5pr1.c
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 * @date 2024-03-26
 */

#pragma once
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "h5-p1-type.h"  // typedef: node_t queue_t

// #define MY_BUILD_TYPE ""
#ifndef MY_BUILD_TYPE
    #error "MY_BUILD_TYPE" unspecified!
#endif

#if DEBUG
    #undef DEBUG
    #define DEBUG(X) X
#else
    #undef DEBUG
    #define DEBUG(X) 
#endif


static inline void mimic_standard_heavy_workload(){usleep(1000);}


int  Queue_Dequeue(queue_t* q, int *value);
void Queue_Enqueue(queue_t* q, int value);
void Queue_Init(queue_t* q);

static inline double update_time(struct timespec* start_time, struct timespec* end_time, double* time_elapsed)
{
    clock_gettime(CLOCK_REALTIME, end_time);
    *time_elapsed = (end_time->tv_sec - start_time->tv_sec) + ((end_time->tv_nsec - start_time->tv_nsec) / 1000000000.0);
    return *time_elapsed;
}

void *worker(void* arg) {
    worker_info_t* winfo = (worker_info_t*) arg;
    int t_val;
    int t_cycles = 0;
    struct timespec start_time, end_time;
    double time_elapsed;

    printf("[%d-%s]: Begin.\n", winfo->worker_id, EWorkerType_str(winfo->worker_type));
    update_time(&start_time, &start_time, &time_elapsed);

    switch (winfo->worker_type)
    {
    case Writer:
        {
            while (1)
            {
                if (winfo->worker_duration_type == Cycle){ if (t_cycles >= winfo->worker_duration_cycle) break; }
                else if (winfo->worker_duration_type == Time){ if (update_time(&start_time, &end_time, &time_elapsed) >= winfo->worker_duration_sec) break; }

                Queue_Enqueue(winfo->queue, winfo->worker_id);
                DEBUG(printf("+%1d\n", winfo->worker_id);)

                if (winfo->worker_duration_type == Time) usleep(100000);
                t_cycles++; 
            }
        }
        break;
    case Reader:
        {
            while (1)
            {
                if (winfo->worker_duration_type == Cycle){ if (t_cycles >= winfo->worker_duration_cycle) break; }
                else if (winfo->worker_duration_type == Time){ if (update_time(&start_time, &end_time, &time_elapsed) >= winfo->worker_duration_sec) break; }

                if (Queue_Dequeue(winfo->queue, &t_val) != 0) t_val = 0;  // reset if fail
                DEBUG(printf("   %1d->%1d\n", t_val, winfo->worker_id);)

                if (winfo->worker_duration_type == Time) usleep(100000);
                t_cycles++; 
            }
            
        }
        break;
    case Idle:
    default:
        break;
    }

    update_time(&start_time, &end_time, &time_elapsed);
    printf("[%d-%s]: Done. with [%04d]cyc [%2.4f]sec.\n", winfo->worker_id, EWorkerType_str(winfo->worker_type), t_cycles, time_elapsed);
    return NULL;
}

int main(int argc, char *argv[]) {
    struct timespec start_time, end_time;
    double time_elapsed;

    pthread_t p1, p2, p3, p4, p5, p6, p7, p8;

    queue_t q;
    Queue_Init(&q);

    printf("[[ " MY_BUILD_TYPE " ]]\n");

    worker_info_t p1i = {.queue = &q, .worker_id = 1, .worker_duration_cycle = 1000, .worker_duration_type = Cycle, .worker_type = Writer};
    worker_info_t p2i = {.queue = &q, .worker_id = 2, .worker_duration_cycle = 1000, .worker_duration_type = Cycle, .worker_type = Writer};
    worker_info_t p3i = {.queue = &q, .worker_id = 3, .worker_duration_cycle = 1000, .worker_duration_type = Cycle, .worker_type = Writer};
    worker_info_t p4i = {.queue = &q, .worker_id = 4, .worker_duration_cycle = 1000, .worker_duration_type = Cycle, .worker_type = Writer};
    worker_info_t p5i = {.queue = &q, .worker_id = 5, .worker_duration_cycle = 1000, .worker_duration_type = Cycle, .worker_type = Reader};
    worker_info_t p6i = {.queue = &q, .worker_id = 6, .worker_duration_cycle = 1000, .worker_duration_type = Cycle, .worker_type = Reader};
    worker_info_t p7i = {.queue = &q, .worker_id = 7, .worker_duration_cycle = 1000, .worker_duration_type = Cycle, .worker_type = Reader};
    worker_info_t p8i = {.queue = &q, .worker_id = 8, .worker_duration_cycle = 1000, .worker_duration_type = Cycle, .worker_type = Reader};

    printf("[MAIN]: Begin.\n");
    update_time(&start_time, &start_time, &time_elapsed);

    pthread_create(&p1, NULL, worker, (void*) &p1i);
    pthread_create(&p2, NULL, worker, (void*) &p2i);
    pthread_create(&p3, NULL, worker, (void*) &p3i);
    pthread_create(&p4, NULL, worker, (void*) &p4i);
    pthread_create(&p5, NULL, worker, (void*) &p5i);
    pthread_create(&p6, NULL, worker, (void*) &p6i);
    pthread_create(&p7, NULL, worker, (void*) &p7i);
    pthread_create(&p8, NULL, worker, (void*) &p8i);

    // threads running here

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(p4, NULL);
    pthread_join(p5, NULL);
    pthread_join(p6, NULL);
    pthread_join(p7, NULL);
    pthread_join(p8, NULL);

    update_time(&start_time, &end_time, &time_elapsed);
    printf("[MAIN]: Done. with [%2.4f]sec.\n", time_elapsed);
    // printf("[MAIN]: Done\n [counter: %d]\n [should: %d]\n", counter, max*2);
    return 0;
}
