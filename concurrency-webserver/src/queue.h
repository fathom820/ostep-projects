#ifndef QUEUE_H
#define QUEUE_H
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


void queue_init(queue_t *q);
void queue_enqueue(queue_t *q, int value);
int queue_dequeue(queue_t *q, int *value);
int queue_empty(queue_t *q);

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

void *enq_f(void *a);
void *deq_f(void *a);
#endif