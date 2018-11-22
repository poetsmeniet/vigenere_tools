#include "main.h"

_Bool populateTableau(cuint size, char (*tab)[size], cchar *alphabet)
{
    if(size == 0)
        return 0;

    uint i;
    uint j;
    uint offset = (uint)alphabet[0];

    for (j = offset; j < (size + offset); j++){
        for (i = offset; i < (size + offset); i++){
            char newChar = alphabet[( (i + (j-offset)) - offset ) % size];
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

    printf("Printing the tableau..\n");

    for (j = offset; j < (size + offset); j++){
        for(i = offset; i < (size + offset); i++){
            printf("%c ", tab[j][i]);
        }

        printf("\n");
        i = offset;
    }
}

void returnCipherText(cchar *key, cchar *clearText, char *cipherText, cuint aSize, char (*tab)[aSize])
{
    uint j;
    uint i = 0;
    cuint lenT = strlen(clearText);
    cuint lenC = strlen(cipherText);

    for(j = 0; j < lenT; j++){
        cuint keyChar = (uint)key[i];
        cuint ctChar = (uint)clearText[j];

        /* Assign encrypted letter to cipherText */
        cipherText[j] = tab[keyChar][ctChar];
        
        i++;

        if(i > lenC)
            i = 0;
    }
}

