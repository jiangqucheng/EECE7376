/**
 * @file hw5pr1.c
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 * @date 2024-03-26
 */
#pragma once
#include <pthread.h>
#include <assert.h>

typedef struct __node_t
{
    int value;
    struct __node_t* next;  // pointer to next node
} node_t;

typedef struct __queue_t
{
    node_t* head;  // pointer to queue head
    node_t* tail;  // pointer to queue tail
    pthread_mutex_t head_lock;
    pthread_mutex_t tail_lock;
} queue_t;

typedef enum __WORKER_THREAD_TYPE {
  Idle = 0,
  Writer = 1,
  Reader = 2,
} EWorkerType;

typedef enum __WORKER_THREAD_DURATION_TYPE {
  Time = 1,
  Cycle = 2,
} EWorkerDurationType;


typedef struct __worker_thread_info_t
{
    queue_t* queue;
    unsigned int worker_id;
    double worker_duration_sec;
    int worker_duration_cycle;
    EWorkerDurationType worker_duration_type;
    EWorkerType worker_type;
} worker_info_t;

const char *EWorkerType_str(EWorkerType wtype) {
  switch(wtype) {
    case Idle: return "Idle";
    case Writer: return "Writer";
    case Reader: return "Reader";
    default: assert(!"Unknown EWorkerType");
  }
}