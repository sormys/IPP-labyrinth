#include <inttypes.h>
#include "inputMng.h"
#include "tools.h"
#include "bitOperations.h"

int main(void)
{
    mazeData maze = newMaze();
    uint64_t distance;
    int foundWay;

    readInput(&maze);
    checkStartEnd(&maze);
    foundWay = bfs(&maze, &distance);
    freeMaze(&maze);
    if (foundWay == 0) 
        printf("NO WAY\n");
    else 
        printf("%zu\n", distance);
    return 0;
}