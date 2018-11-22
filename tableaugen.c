#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This is just some boiler plate code for generating a vegenere tableau */

typedef const unsigned int cuint;
typedef unsigned int uint;
typedef const char cchar;

/* Populates the vigenere tableau
 * Indexes to access tableau are the ascii values of chars
 * Returns: 1 on success
 *          0 on failure
 * */
_Bool populateTableau(cuint size, char (*tab)[size], cchar *alphabet)
{
    if(size == 0)
        return 0;

    uint i;
    uint j;
    uint offset = (uint)alphabet[0];

    for (j = offset; j < (size + offset); j++){
        for (i = offset; i < (size + offset); i++){
            char newChar = alphabet[( (i + j) % size )];
            tab[j][i] = newChar;
        }
        i = offset;
    }

    return 1;
}

void printTableau(cuint size, char (*tab)[size], uint offset)
{
    uint i;
    uint j;

    for (j = offset; j < (size + offset); j++){
        for(i = offset; i < (size + offset); i++){
            printf("%c ", tab[j][i]);
        }

        printf("\n");
        i = offset;
    }
}

int main(void)
{
    cchar alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    cuint size = strlen(alphabet);
    
    char tab[size][size];

    /* testing populate tableau with chars */
    _Bool ret = populateTableau(size, tab, alphabet);
    if(ret){
        printf("Tableau populated, printing..\n");
        printTableau(size, tab, (uint)alphabet[0]);
    }else{
        return EXIT_FAILURE;
    }

    return 0;
}
