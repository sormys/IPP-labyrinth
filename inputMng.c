#include "inputMng.h"

// Function conversts string from input into number and saves in *number.
// If number is too big to be saved in uint64_t returns 1.
static int readNumber(int *character, uint64_t *number)
{
    int error = 0; // Error if number is too big.
    *number = 0; // Stored converted number.

    while (isdigit(*character) && !error) {
        if ((SIZE_MAX - (*character - '0')) / 10 < (*number)) 
            error = 1; // Number > SIZE_MAX.
        else {
            (*number) *=10;
            (*number) += (*character - '0');
            *character = getchar();
        }
    }
    return error;
}

// Function reads uArray from one line.
// If input is incorrect returns line number of error, else returns 0.
static int readArray(uArray *array, int line)
{
    int character;
    int numberOk; // Controls if there was an error with read number.
    uint64_t currentNumber; // Stores last read number.
    while ((character = spaceSkipper()) != '\n' && (character != EOF)) {
        numberOk = !readNumber(&character, &currentNumber);
        if (currentNumber != 0 && isAllowed(character) && numberOk) {
            addToArray(array, currentNumber);
            ungetc(character, stdin);
        }
        else 
            return line;
    }
    if (character == EOF || (character == '\n' && array->size == 0)) {
        // There is no next line or this line is empty.
        if (array->size > 0) 
            line++; // This line was not empty.
        return line;
    }
    array->ptr = safeRealloc(array->ptr, array->next * sizeof(array->ptr[0]));
    array->size = array->next;
    // Shrinks array to minimal required size.
    return 0;
}

// Function saves mazeDimensions from line of input.
static void readFirstLine(mazeData *maze, uint64_t *mazeSize)
{
    int error;

    *mazeSize = 1;
    if ((error = readArray(&(maze->dimensions), 1)) != 0  
    || (maze->dimensions.size < 1)) {
        freeMaze(maze);
        endWithError(error);
    }
    for (uint64_t i = 0; i < maze->dimensions.next; i++) {
        if (SIZE_MAX / maze->dimensions.ptr[i] < *mazeSize) {
            freeMaze(maze);
            endWithError(0);
        }
        (*mazeSize) *= maze->dimensions.ptr[i];
    }
    maze->dimension_count = maze->dimensions.next;
}

// Function creates array of prefix products of dimensions.
static uArray createPrefixArray(uArray mazeDimensions)
{
    uArray prefixes;

    prefixes.ptr = safeMalloc(sizeof(uint64_t) * mazeDimensions.size);
    prefixes.size = mazeDimensions.size;
    prefixes.next = 0;
    prefixes.ptr[0] = 1; // MazeDimensions cannot be empty.
    for (uint64_t i = 1; i < mazeDimensions.size; i++) {
        prefixes.ptr[i] = prefixes.ptr[i-1] * mazeDimensions.ptr[i-1];
    }
    return prefixes;
}

// Function reads coordinates from input.
// If coordintes are incorrect ends program with ERROR line number.
static void readCoordinates(uint64_t *coordinates, mazeData *maze, int line)
{
    *coordinates = 0;
    uint64_t used = 0; // Number of used coordinates.
    uint64_t number;
    int character;
    while (used < maze->dimension_count) {
        character = spaceSkipper();
        if (readNumber(&character, &number) || number == 0 || character == EOF
        || (character == '\n' && used < maze->dimension_count - 1)
        || (number > maze->dimensions.ptr[used] || !isAllowed(character))) {
            // If number too big, EOF, not enough coordinates, number bigger
            // than dimension or not allowed character.
            if (character == EOF && (used == maze->dimension_count - 1)
            && (number <= maze->dimensions.ptr[used]) && number != 0)
                 line++; // This line was correct.
            freeMaze(maze);
            endWithError(line);
        }
        *coordinates += (number  -1) * maze->prefixes.ptr[used];
        used++;
    }
    if(character != '\n') character = spaceSkipper();
    if (character != '\n') 
    {
        freeMaze(maze);
        endWithError(line);
    }
}

// Function convert hex from input to binary code in reverse.
// Returns line number of error. If error did not occurre returns 0.
static int hexToCode(uArray *mazeCode, uint64_t *mazeSize,
                     uint64_t *hexCharacters)
{
    *hexCharacters = 0;
    uint64_t binaryCode;
    uint64_t bitsCount = 0; 
    // How many written bits in current cell of mazeCode.
    uint64_t realBits = HEXBITS; 
    // Position of most significant 1 in first hex character.
    int line = 4;
    int character = getchar(); 

    if (!isxdigit(character)) 
        return line;
    else if (character == '0') 
        character = zeroSkipper();
    if (isxdigit(character)) {
        // Setting realBits. 
        while (!getOneBit((uint64_t)hexValue(character), realBits - 1)) 
            realBits--;
    }
    while (isxdigit(character)) {
        (*hexCharacters)++;
        character = hexValue(character);
        binaryCode = (uint64_t)character << bitsCount;
        mazeCode->ptr[mazeCode->next] |= binaryCode;
        bitsCount += HEXBITS;
        if (bitsCount == CELLBITS) {
            bitsCount = 0;
            (mazeCode->next)++;
        }
        if ((*hexCharacters - 1) * HEXBITS + realBits> *mazeSize)
            return line; // Code definitely too big.
        character = getchar();
    }
    character = spaceIgnore(character);
    if (character == '\n' && ((character = getchar()) != EOF)) 
        return line + 1; // The next line should be empty.
    if (character != EOF) 
        return line; // There should be nothing more in the line.
    return 0;
}

// Function puts code converted by convertToBinCode in required order.
static void reverseCode(uArray *mazeCode, uint64_t hexCharacters)
{
    uint64_t bitMask, usedBits = hexCharacters * HEXBITS, copiedBits = 0;
    uint64_t copyCell = 0; // Current cell index.
    uArray tmp = newUArray();
    uint64_t copySize = usedBits > 0 ? ceilingDiv(usedBits, CELLBITS) - 1 : 0;
    // How many cells in mazeCode were modified - 1.
    
    tmp.ptr = safeCalloc(copySize + 1, sizeof(uint64_t));
    tmp.size = copySize + 1; 
    tmp.next = 0; // We will be using .next as current positon in cell.
    mazeCode->next = usedBits % CELLBITS;  // Used bits in the last cell.
    if (mazeCode->next == 0) 
        mazeCode->next = CELLBITS; 
    // If all bits were used in last cell we want the last four of the cell.
    mazeCode->next -= HEXBITS; // We want position of first bit of the four.
    while (copiedBits != usedBits) {
        bitMask = getFourBits(mazeCode->ptr[copySize], mazeCode->next);
        bitMask = bitMask << tmp.next;
        tmp.ptr[copyCell] |= bitMask;
        if(mazeCode->next == 0) { // Skipping to next cell.
            mazeCode->next = CELLBITS - HEXBITS; // Last four bits.
            copySize--;
        }
        else 
            mazeCode->next -= HEXBITS;
        tmp.next += HEXBITS;
        if (tmp.next == CELLBITS) {
            tmp.next = 0;
            copyCell++; // Skipping to next cell.
        }
        copiedBits += HEXBITS;
    }
    for(uint64_t i = 0; i < tmp.size; i++) {
        // Copying modified cells.
        mazeCode->ptr[i] = tmp.ptr[i];
    }
    free(tmp.ptr);
}

// Function reads maze code given as a hex number.
static int readHexCode(uint64_t *mazeSize, uArray *mazeCode)
{
    int error;
    uint64_t hexCharacters;

    *mazeCode = newUArray();
    mazeCode->size = ceilingDiv(*mazeSize, CELLBITS);
    mazeCode->ptr = safeCalloc(mazeCode->size, sizeof(uint64_t));
    if ((error = hexToCode(mazeCode, mazeSize, &hexCharacters)) != 0)
        return error;
    reverseCode(mazeCode, hexCharacters);
    return 0;
}

// Function reads number in R notation.
// If number is too big returns 1.
static int readRNumber(int *character, uint64_t *number)
{
    readNumber(character, number);
    return *number > UINT32_MAX;
}

// Function sets all bits of the remainder  according to the equation.
static void lightRBits(uint64_t sValue, uint64_t mazeSize, uArray *mazeCode)
{
    for (uint64_t i = sValue % mazeSize; i < mazeSize; i += (1UL << 32)) {
        // i values are all values that are equal w_i (mod mazeSize)
        lightBitInArray(i, *mazeCode);
    }
}

// Function reads maze code given in R notation.
static int readRNotation(uint64_t *mazeSize, uArray *mazeCode)
{
    uint64_t array[5]; // a - 0, b - 1, m - 2, r - 3, s - 4 
    int error = 4;
    int character = spaceSkipper();
    *mazeCode = newUArray();
    mazeCode->size = ceilingDiv(*mazeSize, CELLBITS);
    mazeCode->ptr = safeCalloc(mazeCode->size, sizeof(uint64_t));

    for (uint64_t i = 0; i < 5; i++) {
        character = spaceIgnore(character);
        if (character == EOF || character == '\n') 
            return error; // Not enough numbers.
        if (readRNumber(&character, &(array[i])) || !isAllowed(character))
            return error; // Number too big or not allowed character.
    }
    character = spaceIgnore(character);
    if ((character == '\n') && ((character=getchar()) != EOF))
        return error + 1;
    if (character!=EOF || array[2] == 0)
        return error;
    for (uint64_t i = 1; i <= array[3]; i++) {
        array[4] = (array[4] * array[0] + array[1]) % array[2];
        lightRBits(array[4], *mazeSize, mazeCode);
    }
    return 0;
}

// Function reads maze code (bits of full and empty cubes).
// Returns line of error if occured, 0 if no error occured.
static int readMazeCode(uint64_t *mazeSize, uArray *mazeCode)
{
    int error = 4; // We are in 4-th line.

    switch (whatNotation())
    {
    case NOTATION_HEX:
        if ((error = readHexCode(mazeSize, mazeCode)) != 0) 
            return error;
        break;
    case NOTATION_R:
        if ((error = readRNotation(mazeSize, mazeCode)) != 0) 
            return error;
        break;
    default:
        return error;
        break;
    }
    return 0;
}

// Function read and interprets input.
// Saves dimensions and coordinates of starting and ending positions.
void readInput(mazeData *maze)
{
    int error;
    uint64_t mazeSize;

    readFirstLine(maze, &mazeSize);
    maze->prefixes = createPrefixArray(maze->dimensions);
    readCoordinates(&(maze->start), maze, 2);
    readCoordinates(&(maze->end), maze, 3);
    if ((error = readMazeCode(&mazeSize, &(maze->code))) != 0) {
        freeMaze(maze);
        endWithError(error);
    }
}