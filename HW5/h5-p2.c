#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Global sum variable and mutex
int sum = 0;
pthread_mutex_t sum_mutex;

// Structure to pass arguments to threads
typedef struct __runner_args_t
{
    int* v;
    int index;
    int size;
} runner_args_t;

// Thread runner function
void* runner(void* parameters) {
    runner_args_t* args = (runner_args_t*)parameters;
    int partial_sum = 0;
    // for (int i = 0; i < args->size; i++) {
    //     partial_sum += args->v[args->index + i];
    // }
    int* t_now_pointing_at = &(args->v[args->index]);
    for (int i = 0; i < args->size; i++) partial_sum += *(t_now_pointing_at++);

    pthread_mutex_lock(&sum_mutex);
    sum += partial_sum;
    pthread_mutex_unlock(&sum_mutex);

    free(args);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <num_elements> <num_threads>\n", argv[0]);
        return -1;
    }

    int num_elements = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int* v = malloc(num_elements * sizeof(int));

    // Initialize vector
    for (int i = 0; i < num_elements; i++) {v[i] = i;}

    // Initialize mutex
    pthread_mutex_init(&sum_mutex, NULL);

    pthread_t threads[num_threads];
    int chunk_size = num_elements / num_threads;

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        runner_args_t* args = malloc(sizeof(runner_args_t));
        args->v = v;
        args->index = i * chunk_size;
        args->size = (i == num_threads - 1) ? (num_elements - args->index) : chunk_size;  // last thread finishes all
        pthread_create(&threads[i], NULL, runner, args);
    }

    // Wait for threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Sum = %d\n", sum);

    // Cleanup
    free(v);
    pthread_mutex_destroy(&sum_mutex);

    return 0;
}
