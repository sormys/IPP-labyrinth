#include "bitOperations.h"

// Function changes bit in uArray.
void lightBitInArray(uint64_t bitPosition, uArray array)
{
    array.ptr[bitPosition / CELLBITS] |= (1UL << (bitPosition % CELLBITS));
}

// Function gets one bit from uArray.
uint64_t getBitFromArray(uint64_t bitPosition, uArray array)
{
    return getOneBit(array.ptr[bitPosition / CELLBITS], bitPosition % CELLBITS);
}

// Function returns bit from bitPosition in binary representation of a number.
uint64_t getOneBit(uint64_t number, uint64_t bitPosition)
{
    uint64_t bit = 0;
    
    bit = ((number & (1UL << bitPosition)) >> bitPosition);
    return bit;
}

// Function extracts four bits from number.
uint64_t getFourBits(uint64_t number, uint64_t firstBitPlace)
{
    uint64_t bits;

    bits = number & (FOUR_BITS << firstBitPlace);
    bits = bits >> firstBitPlace; 
    return bits;
}

// Function checks if starting and ending positions are empty.
// If start is a full cube ends program with ERROR 2.
// If only end is a full cube ends program with ERROR 3.
void checkStartEnd(mazeData *maze)
{
    if (getBitFromArray(maze->start, maze->code)) {
        freeMaze(maze);
        endWithError(2);
    }
    else if (getBitFromArray(maze->end, maze->code)) {
        freeMaze(maze);
        endWithError(3);
    }
}