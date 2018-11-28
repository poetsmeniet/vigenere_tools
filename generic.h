#ifndef parseoptions_H_
#define parseoptions_H_

#include <stdio.h> /* printf */
#include <stdlib.h> /* exit */
#include <string.h> /* strlen etc */

/* Basic typedefs */
typedef const unsigned int cuint;
typedef unsigned int uint;
typedef const char cchar;

/* Stores numbers and their occurences */
typedef struct numberFrequencies{
    uint number;
    uint cnt;
    struct numberFrequencies *next;
}numFreqs;

/* Push numbers into struct and increments count*/
void pushNums(uint *numbers, uint sz, numFreqs *freqNums);

/* Free allocated memory */
void freeNums(numFreqs *freqNums);

/* Displays help text */
void displayHelp();

#endif
