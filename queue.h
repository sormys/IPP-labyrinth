#ifndef QUEUE
#define QUEUE

#include <inttypes.h>
#include "tools.h"

typedef struct queue queue;

// Simplified array implementation of queue.
// End - index of last used cell + 1.
// In this program queue can be only in state of pushing values or 
// emptying it, so if queue.size == queue.end then in order to add new value
// array will be expanded.
struct queue {
    uint64_t *ptr;
    uint64_t first;
    uint64_t end;
    uint64_t size;
};

void addToQueue(uint64_t value, queue *q);
queue newQueue();
void restartQueue(queue *q);
void deleteQueue(queue *q);
uint64_t pop(queue *q);
int empty(queue q);
#endif