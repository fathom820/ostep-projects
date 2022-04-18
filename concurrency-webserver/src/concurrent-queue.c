#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define NUM_THREADS 5

// QUEUE DEFINITIONS //
typedef struct __node_t {
  int             value; // value of node
  struct __node_t *next; // pointer to next node
} node_t;

typedef struct __queue_t {
  node_t          *head; // head of queue
  node_t          *tail; // tail of queue
  // locks for head and tail operations
  pthread_mutex_t head_lock, tail_lock; 
} queue_t;

void queue_init(queue_t *q) {
  node_t *tmp = malloc(sizeof(node_t)); // create node
  tmp -> next = NULL;         // only node in array, doesn't point to anything
  q -> head = q -> tail = tmp; // tail and head are same because only 1 node
  pthread_mutex_init(&q -> head_lock, NULL); //  init head lock
  pthread_mutex_init(&q -> tail_lock, NULL); // init tail lock
  printf("init q\n");
}

void queue_enqueue(queue_t *q, int value) {
  node_t *tmp = malloc(sizeof(node_t));
  assert(tmp != NULL);
  tmp -> value = value;
  tmp -> next  = NULL; 

  pthread_mutex_lock(&q -> tail_lock);
  q -> tail -> next = tmp;
  q -> tail = tmp;
  printf("enq %d\n", q->tail->value);
  pthread_mutex_unlock(&q -> tail_lock);
}

int queue_dequeue(queue_t *q, int *value) {
  pthread_mutex_lock(&q -> head_lock);
  node_t *tmp = q -> head;
  node_t *new_head = tmp -> next;

  if (!new_head) {
    pthread_mutex_unlock(&q -> head_lock);
    return -1; // queue was empty
  }
  *value = new_head -> value;
  q -> head = new_head;
  pthread_mutex_unlock(&q -> head_lock);
  free(tmp);
  printf("deq %d\n", *value);
  return 0;
}


typedef struct __init_args {
  queue_t *q;
} *init_args;

typedef struct __enq_args {
  queue_t *q;
  int value;
} *enq_args;

typedef struct __deq_args {
  queue_t *q;
  int *value;
} *deq_args;

void *enq_f(void *a) {
  enq_args arg = (enq_args) a;
  queue_enqueue(arg->q, arg->value);
}

void *deq_f(void *a) {
  deq_args arg = (deq_args) a;
  queue_dequeue(arg->q, &arg->value);
}

int main(int argc, char *argv[]) {
  pthread_t p1, p2, p3, p4, p5;
  queue_t queue;
  queue_init(&queue);

  enq_args p2_args = malloc(sizeof(*p2_args));
  p2_args->q = &queue;
  p2_args->value = 1;

  enq_args p3_args = malloc(sizeof(*p3_args));
  p3_args->q = &queue;
  p3_args->value = 2;

  int *val = 0;
  deq_args p4_args = malloc(sizeof(*p4_args));
  p4_args->q = &queue;
  p4_args->value = val;
  // printf("%d\n", p4_args->value);

  int *val_p4;
  pthread_create(&p2, NULL, enq_f, p2_args);
  pthread_create(&p3, NULL, enq_f, p3_args);

  pthread_join(p2, NULL);  
  pthread_join(p3, NULL);

  pthread_create(&p4, NULL, deq_f, p4_args);
  pthread_create(&p5, NULL, deq_f, p4_args);

  printf("%d\n", get_nprocs());
  pthread_join(p4, NULL);
  pthread_join(p5, NULL);
}