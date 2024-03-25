/**
 * @file hw5pr1.c
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 * @date 2024-03-26
 */

#define MY_BUILD_TYPE "2LOCKS"

#include <pthread.h>
#include <stdlib.h>
#include "h5-p1-type.h"  // typedef: node_t queue_t
#include "h5-p1-main.h"

void Queue_Init(queue_t* q)
{
    //Creating the dummy node, which is needed so that “tail” update is separated from “head” update.
    node_t* tmp = malloc(sizeof(node_t));
    tmp->next = NULL; //a dummy node has next=NULL and no need to set its value attribute
    q->head = q->tail = tmp;
    pthread_mutex_init(&q->head_lock, NULL);
    pthread_mutex_init(&q->tail_lock, NULL);
}

// Add a new node at the tail with content “value”
void Queue_Enqueue(queue_t* q, int value)
{
    node_t *tmp = malloc(sizeof(node_t));
    assert(tmp != NULL);
    tmp->value = value;
    tmp->next = NULL;
    pthread_mutex_lock(&q->tail_lock);
    mimic_standard_heavy_workload();
    q->tail->next = tmp;
    q->tail = tmp;
    // tail always points to the last enqueued node or
    // to the dummy node (if the queue is empty)
    pthread_mutex_unlock(&q->tail_lock);
}

// Remove the first node in the queue (if it exists) and return its value in the “value” parameter.
// The function returns 0 on success and -1 on failure.
int Queue_Dequeue(queue_t* q, int *value)
{
    pthread_mutex_lock(&q->head_lock);
    mimic_standard_heavy_workload();
    node_t *tmp = q->head;
    node_t *new_head = tmp->next;
    if(new_head == NULL)
    {
        // indicates an empty queue with only a dummy node
        pthread_mutex_unlock(&q->head_lock);
        *value = -1;
        return -1;
    }
    *value = new_head->value;
    q->head = new_head;
    // head always points to a dummy node,
    // which is either the original dummy node or most recently dequeued node
    pthread_mutex_unlock(&q->head_lock);
    free(tmp);  // old dummy node is removed
    return 0;
}

