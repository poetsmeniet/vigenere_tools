#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This is just some boiler plate code for generating a vegenere tableau */

typedef const unsigned int cuint;
typedef unsigned int uint;
typedef const char cchar;

/* Populates the vigenere tableau 
 * Returns: 1 on success
 *          0 on failure
 * */
_Bool populateTableau(cuint size, char (*tab)[size], cchar *alphabet)
{
    if(size == 0)
        return 0;

    int i;
    int j;

    for (j = 0; j < size; j++){
        for (i = 0; i < size; i++){
            char newChar = alphabet[( (i + j) % size )];
            tab[j][i] = newChar;
        }
        i = 0;
    }

    return 1;
}

/* Populates the vigenere tableau (using recursion)
 * Returns: 1 on success
 * */
_Bool populateTableauRec(cuint size, char (*tab)[size], cchar *alphabet, uint i, uint j)
{
    if(j == size)
        return 1;

    tab[j][i] = alphabet[( (i + j) % size )];
    i++;

    if(i == size){
        i = 0;
        j++;
    }

    /* Recurse */
    populateTableauRec(size, tab, alphabet, i, j);

    return 1;
}

void printTableau(cuint size, char (*tab)[size])
{
    uint i;
    uint j;

    for (j = 0; j < size; j++){
        for(i = 0; i < size; i++){
            printf("%c ", tab[j][i]);
        }

        printf("\n");
        i = 0;
    }
}

int main(void)
{
    cchar alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    cuint size = strlen(alphabet);
    
    char tab[size][size];

    /* testing populate tableau with chars */
    _Bool ret = populateTableauRec(size, tab, alphabet, 0, 0);
    //_Bool ret = populateTableau(size, tab, alphabet);
    if(ret){
        printf("Tableau populated, printing..\n");
        printTableau(size, tab);
    }else{
        return EXIT_FAILURE;
    }

    return 0;
}
