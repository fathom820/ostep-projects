#include <stdlib.h>
#include <pthread.h>

#include "thread-pool.h"
#include "request.h"
#include "queue.h"
#include "io_helper.h"

queue_t jobs;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m   = PTHREAD_MUTEX_INITIALIZER;

void *worker(void *arg) {
   while (1) {
    pthread_mutex_lock(&m);
    while (queue_empty(&jobs)) // if empty, no job to do
      pthread_cond_wait(&fill, &m); // wait for jobs

    int fd;
    queue_dequeue(&jobs, &fd);
    pthread_mutex_unlock(&m);

    request_handle(fd);
    close_or_die(fd);
  }
  return NULL;
}

// n workers
int thread_pool_init(int n) {
  printf("open thread_pool_init\n");
  assert(n > 0);
  queue_init(&jobs);
  printf("queue_init\n");
  pthread_t t;
  
  for (int i = 0; i < n; i++) {
    pthread_create(&t, NULL, worker, NULL);
    printf("made worker\n");
  }
  printf("done making workers\n");
  return 1;
}
