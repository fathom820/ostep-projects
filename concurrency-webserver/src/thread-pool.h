#include <pthread.h>
#include "queue.h"

#ifndef THREAD_POOL_H
#define THREAD_POOL_H


queue_t jobs;
pthread_cond_t fill;
pthread_mutex_t m;

void *worker(void *arg);
int thread_pool_init(int n);

#endif