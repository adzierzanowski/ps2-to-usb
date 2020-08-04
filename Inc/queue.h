#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define QCAPACITY 32


typedef uint16_t queue_elem_t;

struct queue_t
{
  queue_elem_t container[QCAPACITY];
  int head;
  int tail;
  size_t size;
};

bool queue_empty(struct queue_t *q);
bool queue_full(struct queue_t *q);
void queue_init(struct queue_t *q);
void queue_put(struct queue_t *q, queue_elem_t elem);
queue_elem_t queue_get(struct queue_t *q);

#endif
