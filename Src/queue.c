#include "queue.h"

bool queue_empty(struct queue_t *q)
{
  return q->size == 0;
}

bool queue_full(struct queue_t *q)
{
  return q->size == QCAPACITY;
}

void queue_init(struct queue_t *q)
{
  q->head = QCAPACITY - 1;
  q->tail = QCAPACITY - 1;
}

void queue_put(struct queue_t *q, queue_elem_t elem)
{
  if (q->size < QCAPACITY)
  {
    q->container[q->head--] = elem;
    q->size++;
    if (q->head < 0)
    {
      q->head = QCAPACITY-1;
    }
  }
}

queue_elem_t queue_get(struct queue_t *q)
{
  queue_elem_t out = 0;

  if (q->size > 0)
  {
    out = q->container[q->tail--];
    q->size--;
    if (q->tail < 0)
    {
      q->tail = QCAPACITY-1;
    }
  }
  return out;
}
