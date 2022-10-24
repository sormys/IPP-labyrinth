#include "queue.h"

// Function adds value to queue.
// If necessary tries to expand the array.
void addToQueue(uint64_t value, queue *q)
{
    if (q->end >= q->size) {
        q->ptr = safeRealloc(q->ptr, (q->size + 1) * 2 * sizeof(uint64_t));
        q->size = (q->size + 1)* 2;
    }
    q->ptr[q->end] = value;
    q->end++;
}

// Function returns queue with deafult values.
queue newQueue()
{
    queue tmp;
    tmp.ptr = NULL;
    tmp.end = 0;
    tmp.size = 0;
    tmp.first = 0;
    return tmp;
}

// Function restarts indexes of queue, leaving the allocated memory.
void restartQueue(queue *q)
{
    q->first = 0;
    q->end = 0;
}

// Function deletes used queue, and set it to deafult values.
void deleteQueue(queue *q)
{
    free(q->ptr);
    *q = newQueue();
}

// Function pops first element from NOT EMPTY queue.
uint64_t pop(queue *q)
{   
    uint64_t tmp = q->ptr[q->first];
    q->first++;
    return tmp;
}

// Funtion returns 1 if queue is empty.
int empty(queue q)
{
    return q.first == q.end;
}
