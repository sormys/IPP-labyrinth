#include "tools.h"

// Function detects if character is white space or digit or not.
int isAllowed(int character)
{
    return isdigit(character) || isspace(character) || character == EOF;
}

// Function returns value in decimary of hex character.
int hexValue(int character)
{
    if (isdigit(character)) 
        character -= '0';
    else if (character >='a' && character <= 'f') 
        character = character - 'a' + 10;
    else 
        character = character - 'A' + 10;
    return character;
}

// Function recognizes notation type.
// Returns NOTATION_ ERROR if notation is inocorrect.
int whatNotation(){
    int character = spaceSkipper();
    if (character == '0') {
        character = getchar();
        if (character == 'x') 
            return NOTATION_HEX;
    }
    else if (character == 'R') 
        return NOTATION_R;
    return NOTATION_ERRROR;
}

// Function returns ceiling of uint64_t fraction.
uint64_t ceilingDiv(uint64_t numerator, uint64_t denominator)
{
    uint64_t fraction = numerator / denominator;
    if (fraction * denominator < numerator) 
        fraction++;
    return fraction;
}

// Function initiates empty uArray.
uArray newUArray()
{
    uArray new;
    new.ptr = NULL;
    new.size = 0;
    new.next = 0; // Here you can store e.g. next index you want to change.
    return new;
}

// Function initiates empty mazeData.
mazeData newMaze()
{
    mazeData maze;
    maze.dimensions = newUArray();
    maze.code = newUArray();
    maze.prefixes = newUArray();
    return maze;
}

// Function skips zero and returns first no-zero.
int zeroSkipper()
{
    int character;
    while ((character = getchar()) == '0'); 
    return character;
}

// Function skips white spaces and returns next character (int).
int spaceSkipper()
{
    int character;
    while (isspace(character = getchar()) && character != '\n');
    return character;
}

// Function if needed skips to next no white space character.
int spaceIgnore(int character)
{
    if (isspace(character) && character != '\n') 
        character = spaceSkipper();
    return character;
} 

// Function works like malloc.
// If not enough memory ends program with ERROR 0.
void *safeMalloc (size_t size)
{
    void *tmp = malloc(size);
    if (tmp == NULL) 
        endWithError(0); // Not enough memory.
    return tmp;
}

// Function works like realloc.
// If not enough memory ends program with ERROR 0.
void *safeRealloc(void *ptr, size_t size)
{
    void *tmp;
    tmp = realloc(ptr, size); 
    if (tmp == NULL) 
        endWithError(0); // Not enough memory.
    return tmp;
}

// Function works like calloc.
// If not enough memory ends program with ERROR 0.
void *safeCalloc(uint64_t numberOfElements, size_t sizeOfElements)
{
    void *tmp;
    tmp = calloc(numberOfElements, sizeOfElements); 
    if (tmp == NULL) 
        endWithError(0); // Not enough memory.
    return tmp;
}

// Function frees maze. 
void freeMaze(mazeData *maze)
{
    free(maze->code.ptr);
    free(maze->dimensions.ptr);
    free(maze->prefixes.ptr);
}

// Function ends program with errorcode.
void endWithError(int errorcode)
{
    fprintf(stderr, "ERROR %d\n", errorcode);
    exit(1);
}

// Function adds element to array.
// If not enough space, expands array.
// If not enough memory, ends program with error.
void addToArray(uArray *array, uint64_t value)
{
    if (array->size  < array->next  + 1) {
        array->ptr = safeRealloc(array->ptr, sizeof(*array->ptr) * ((array->size + 1)*2)); 
        array->size = (array->size + 1) * 2;
    }
    array->ptr[array->next]=value;
    array->next++;
}