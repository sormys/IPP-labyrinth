#ifndef BITOPERATIONS
#define BITOPERATIONS

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "tools.h"
void lightBitInArray(uint64_t bitPosition, uArray array);
uint64_t getBitFromArray(uint64_t bitPosition, uArray array);
uint64_t getOneBit(uint64_t number, uint64_t bitPosition);
uint64_t getFourBits(uint64_t number, uint64_t firstBitPlace);
void checkStartEnd(mazeData *maze);
#endif