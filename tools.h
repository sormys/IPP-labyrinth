#ifndef TOOLS
#define TOOLS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>

#define CELLBITS 64 // Bits of uArray one cell (bits of uint64_t)
#define HEXBITS 4 // Bits of one hex character in binary.
#define FOUR_BITS 15UL
#define NOTATION_HEX 0
#define NOTATION_R 1
#define NOTATION_ERRROR -1
typedef struct uArray uArray;
typedef struct mazeData mazeData;

// Dynamic array of uint64_t
struct uArray {
    uint64_t *ptr;
    size_t size;
    uint64_t next;
};

// Struct containing data of maze
struct mazeData {
    uint64_t dimension_count;
    uint64_t start;
    uint64_t end;
    uArray dimensions;
    uArray code;
    uArray prefixes;
};

uArray newUArray();
mazeData newMaze();
uint64_t ceilingDiv(uint64_t numerator, uint64_t denominator);
int isAllowed(int character);
int hexValue(int character);
int whatNotation();
int zeroSkipper();
int spaceSkipper();
int spaceIgnore(int character);
void *safeMalloc(size_t size);
void *safeRealloc(void *ptr, size_t size);
void *safeCalloc(uint64_t numberOfElements, size_t sizeOfElements);
void freeMaze(mazeData *maze);
void endWithError(int errorcode);
void addToArray(uArray *array, uint64_t value);
#endif