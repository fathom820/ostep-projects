#include <pthread.h>

// non-concurrent counter
typedef struct __counter_t {
  int value;
} counter_t;

void init(counter_t *c) {
  c -> value = 0;
}

void increment(counter_t *c) {
  c -> value++;
}

void decrement(counter_t *c) {
  c -> value--;
}

int get(counter_t *c) {
  return c -> value;
}



// simple concurrent counter
pthread_mutex_t mutex;

typedef struct __counter_ct {
  int value;
} counter_ct;

void init(counter_ct *c) {
  pthread_mutex_lock(&mutex);
  c -> value = 0;
  pthread_mutex_unlock(&mutex);
}

void increment(counter_ct *c) {
  pthread_mutex_lock(&mutex);
  c -> value++;
  pthread_mutex_unlock(&mutex);
}

void decrement(counter_ct *c) {
  pthread_mutex_lock(&mutex);
  c -> value++;
  pthread_mutex_unlock(&mutex);
}

// accesses don't need lock, only assignments
int get(counter_ct *c) {
  return c -> value;
}
