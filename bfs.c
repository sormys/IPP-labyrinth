#include "bfs.h"

// Function adds all empty neighbours of cube to queue.
static void addNeighbours(mazeData *maze, uint64_t cube, queue *q)
{
    uint64_t tmp = cube;
    uint64_t coordinate = cube;

    for (uint64_t i = 0; i < maze->dimensions.size; i++) {
        tmp /= maze->dimensions.ptr[i];
        coordinate -= (tmp * maze->dimensions.ptr[i]);
        if (coordinate > 0) {
            if (!getBitFromArray(cube - maze->prefixes.ptr[i], maze->code)) {
                lightBitInArray(cube - maze->prefixes.ptr[i], maze->code);
                addToQueue(cube - maze->prefixes.ptr[i], q);
            }
        } 
        if (coordinate < maze->dimensions.ptr[i] - 1) {
            if(!getBitFromArray(cube + maze->prefixes.ptr[i], maze->code)) {
                lightBitInArray(cube + maze->prefixes.ptr[i], maze->code);
                addToQueue(cube + maze->prefixes.ptr[i], q);
            }
        }
        // Checking if neighbour with coordinate z_(i+1) +/- 1 is in labyrinth.
        coordinate = tmp;
    }
}

// Function is a simple bfs with 2 queues to use less memory.
// Return 1 if found a way, 0 if didn't, -1 if start or end is a full cube.
int bfs(mazeData *maze, uint64_t *distance)
{
    uint64_t current = maze->start;
    int modulo = 0;
    queue queues[2];
    *distance = 0;

    if (maze->start == maze->end) 
        return 1;
    queues[0] = newQueue();
    queues[1] = newQueue();
    addNeighbours(maze, maze->start, &(queues[modulo^1]));
    lightBitInArray(maze->start, maze->code);
    while (!empty(queues[modulo]) || !empty(queues[modulo^1])) {
        if (empty(queues[modulo])) {
            restartQueue(&(queues[modulo]));
            modulo = modulo^1;
            (*distance)++;
        }
        current = pop(&(queues[modulo]));
        if (current == maze->end) {
            deleteQueue(&(queues[modulo]));
            deleteQueue(&(queues[modulo^1]));
            return 1;
        }
        addNeighbours(maze, current, &queues[modulo^1]);
    }
    deleteQueue(&(queues[0]));
    deleteQueue(&(queues[1]));
    return 0;
}
