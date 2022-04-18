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
  tmp -> next = NULL;                         // only node in array, doesn't point to anything
  q -> head = q -> tail = tmp;                // tail and head are same because only 1 node
  pthread_mutex_init(&q -> head_lock, NULL); //  init head lock
  pthread_mutex_init(&q -> tail_lock, NULL); // init tail lock
  printf("init q\n");
}

void queue_enqueue(queue_t *q, int value) {
  printf("%d\n", value);
  node_t *tmp = malloc(sizeof(node_t));
  assert(tmp != NULL);
  tmp -> value = value;
  tmp -> next  = NULL; 

  pthread_mutex_lock(&q -> tail_lock);
  q -> tail -> next = tmp;
  q -> tail = tmp;
  pthread_mutex_unlock(&q -> tail_lock);
  printf("enq %d\n", q->tail->value);
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
  return 0;
}


int template(int argc, char *argv[]) {
  queue_t queue;
  queue_init(&queue); 
  queue_enqueue(&queue, 3);
  queue_enqueue(&queue, 1);
  queue_enqueue(&queue, 2);
  int val;
  assert(queue_dequeue(&queue, &val) == 0);
  printf("val: %d\n", val);
  
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


int main(int argc, char *argv[]) {
  pthread_t p1, p2, p3, p4, p5;
  queue_t queue;

  enq_args p2_args = malloc(sizeof(*p2_args));
  p2_args -> q = &queue;
  p2_args -> value = 1;
  printf("%d\n", p2_args->value);


  pthread_create(&p1, NULL, (void*)queue_init, &queue);
  pthread_create(&p2, NULL, (void*)queue_enqueue, &p2_args);
  
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);  
  return 0;
}