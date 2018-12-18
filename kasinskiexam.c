#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "generic.h"
#include "kasinskiexam.h"
#define MINKEY 2

/* Sorts struct using simple bubblesort */
void sortList(charFreqs *freqChars)
{
    /* Read struct */
    charFreqs *curr = freqChars;
    while(curr->next->next != NULL){
        /* Swap data using simple bubblesort */
        if(curr->cnt < curr->next->cnt){
            //printf("\tswap %c with %c\n", curr->letter, curr->next->letter);
            
            uint ll = curr->letter;
            uint lcnt = curr->cnt;
            uint hl = curr->next->letter;
            uint hcnt = curr->next->cnt;

            curr->next->letter = ll;
            curr->next->cnt = lcnt;
            curr->letter = hl;
            curr->cnt = hcnt;

            /* Recurse.. */
            sortList(freqChars);
        }

        curr = curr->next;
    }
}

void printBars(uint cnt)
{
    int i;
    for(i = 0; i < cnt; i++)
        printf("-");
}

void printCharFreqs(charFreqs *freqChars, char *colChars)
{
    printf("Analyzing char frequencies.. of string\n");
    cuint maxChars = 15;
    uint printCnt = 0;
    charFreqs *curr = freqChars;

    while(curr->next != NULL){
         int i;
         for(i = 0; i < strlen(colChars); i++){
             if(curr->letter == colChars[i]){
                 float perc = (((float)curr->cnt / (float)strlen(colChars)) * 100.00);
                 printf("%c(%d): %d (%f) ", curr->letter, curr->letter, curr->cnt, perc);
                 printBars((uint)perc * 5);
                 printf("\n");
                 printCnt++;

                 break;
             }
         }
         curr = curr->next;

         if(printCnt > maxChars)
             break;
    }
    printf("\n");
}

/* Analyze frequency distribution of:
 * colChars STRING
 * outputs: array of most frequent chars
 * */
void analyzeFrequencies(char * colChars, uint *keyLengths, char *dest)
{
    /* Test frequency analysis */
    charFreqs *freqChars = malloc(sizeof(charFreqs));
    freqChars->next = NULL;

    /* Add chars to struct for analysis */
    pushChars(colChars, strlen(colChars), freqChars);

    /* Sort list */
    sortList(freqChars);

    /* Read and print struct (debugging) */
    printCharFreqs(freqChars, colChars);

    /* Set most frequent and second most frequent result 
     * Note: most naive code
     * */
    dest[0] = freqChars->letter;
    dest[1] = freqChars->next->letter;
    
    /* Free struct */
    freeChars(freqChars);
}

/* Gets all the factors of a number 
 * Returns void
 * - pushes numbers into numFreqs struct for analysis 
 * */
void getFactors(uint number, numFreqs *freqNums)
{
    uint i, j = 0;
    
    for(i=MINKEY; i <= number; i++)
        if(number % i == 0)
            j++;

    uint myNumbers[j];
    uint k = 0;
    
    for(i=MINKEY; i <= number; i++){
        if(number % i == 0){
            myNumbers[k] = i;
            k++;
        }
    }

    /* Push numbers to linked list */
    pushNums(myNumbers, j, freqNums);
}

/* Determines most likeley key sizes using Kasinski's examination 
 * Returns: void
 * Input: cipherText, keyLengths array
 *
 * Todo:
 *  - add some logic to deal with edge cases 
 *  - a bit slow on larger keys
 * */
extern void returnKeyLengths(char *cipherText, uint *keyLengths)
{
    uint len = strlen(cipherText);

    /* Define frequent numbers struct */
    numFreqs *freqNums = malloc(sizeof(numFreqs)); 
    freqNums->next = NULL;

    printf("Cipher text: %s, len: %d\n", cipherText, len);

    /* This must be tweaked during testing */
    uint maxLength = len / 100;
    
    /* For each key length starting from 4 to maxLength, 
     * look for repeated patterns */
    uint size;
    for(size = MINKEY; size <= maxLength; size++){
        uint w1Cnt = 0;
        uint w2Cnt = size;
        char w1[size + 1];
        char w2[size + 1];
        uint i;

        while(1){
            /* Populate words 1 and 2 of length 'size' to compare using strcmp */
            for(i = 0; i < size; i++){
                w1[i] = cipherText[w1Cnt + i];
            }
            w1[i] = '\0';

            for(i = 0; i < size; i++){
                w2[i] = cipherText[w1Cnt + w2Cnt + i];
            }
            w2[i] = '\0';

            /* Compare word 1 and word 2.
             * If strcomp returns 0, we save the offsets and: 
             *  1. calculate distance
             *  2. factorize distances
             *  3. store factors and their occurences */
            if(strcmp(w1, w2) == 0){
                uint distance = ((w2Cnt + w1Cnt) - w1Cnt);
                //printf("Pattern: %s(%d) \t= %s. Size: %d\tdist: %d\t\n", w1, w1Cnt, w2, size, distance);

                getFactors(distance, freqNums);
            }

            w2Cnt++;
            
            /* Updating counters for word by word comparison */
            if(w2Cnt + w1Cnt + size > len){
                w2Cnt = size;
                w1Cnt++;

                if(w1Cnt > len - (size * 2))
                    break;
            }
                            
        }
    }/* End of size increment for loop */
    
    /* Return most common key length (factors) */
    numFreqs *curr = freqNums;
    uint highest = 0;
    uint highestCnt = 0;

    while(curr->next != NULL){
        if( ((float)curr->cnt * 1.1) >= (float)highestCnt){
            /* Store previous highest */
            keyLengths[1] = highest;

            highestCnt = curr->cnt;
            highest = curr->number;
            /* Store current highest */
            keyLengths[0] = highest;
        }
        curr = curr->next;
    }

    freeNums(freqNums);
}

/* This function inserts chars;
 * - from src
 * - to dst
 * - with len length
 * - every keySz' column
 * It is used to zip back the analyzed and modified monoalphabetic 
 * substition ciphertexts into the decrypted clearText variable.
 * */
void insertStringAtCol(char *src, char *dst, uint col, cuint len, uint keySz)
{
    uint i, j = col;
    uint k = 0;
    uint dstCol;

    for(i = 0; i < (len - col); i++){
        if(i % keySz == 0){
            dstCol = (i + col);

            /* Add colChars to appropriate column and make lower case */
            assert(dstCol <= len);
            dst[dstCol] = src[k];

            j+=keySz;
            k++;
        }

        assert(dstCol < len);
    }
}

/* Applies a ceasar shift to string */
void applyShift(char *shiftedString, 
        char *colChars, 
        uint shift, 
        uint totColSz, 
        uint col,
        cchar *alphabet)
{
    uint shiftChar;
    uint aSz = strlen(alphabet);        //The alphabet size; 
    uint offset = alphabet[0];          //First char ascii code
    uint offsEnd = alphabet[aSz - 1];   //Last char ascii code
    uint modulus = alphabet[0] - 1;     //First char ascii code - 1
    uint l;

    /* Apply shift to this column */
    for(l = 0; l < totColSz; l++){
        shiftChar = colChars[l] - (shift - aSz);
    
        /* Greater than Z -> modulo */
        if(shiftChar >= offsEnd){
            shiftChar = ((shiftChar % offsEnd) + modulus) % offsEnd;
        }
    
        if(shiftChar < offset){
            shiftChar = ((shiftChar % offsEnd) + modulus) % offsEnd;
        }

        if(shift == 1)
            shiftChar -= 1;
    
        assert(shiftChar <= offsEnd && shiftChar >= offset);
    
        shiftedString[l] = shiftChar;
    }
    
    shiftedString[totColSz - col] = '\0';
}

/* Development printing, this will be removed */
void devPrint(char *dictChars, char *ctChars)
{
      printf("dict: %c en %c\n", dictChars[0], dictChars[1]);
      printf("ct: %c en %c\n", ctChars[0], ctChars[1]);
      printf("Shift 1st: (%c - %c): %d\n", ctChars[0], dictChars[0], (ctChars[0] - dictChars[0]));
      printf("Shift 2nd: (%c - %c): %d\n", ctChars[1], dictChars[1], (ctChars[1] - dictChars[1]));
      printf("Shift 3nd: (%c - %c): %d\n", ctChars[0], dictChars[1], (ctChars[0] - dictChars[1]));
      printf("Shift 4nd: (%c - %c): %d\n", ctChars[1], dictChars[0], (ctChars[1] - dictChars[0]));
}

/* Todo: 
 * - implement alphabet as passed param 
 * - improve freq analysis
 *   */
extern void crackVig(char *cipherText, cchar *alphabet)
{
    /* Currently only interested in the two most likely key lengths 
     * for determining shift of decomposed poyalphabetic cipher text*/
    uint keyLengths[2];
    keyLengths[0] = 0;
    keyLengths[1] = 0;

    returnKeyLengths(cipherText, keyLengths);

    assert(keyLengths[0] > 0);

    /* char array clearText will be filled with decrypted chars */
    const uint ctLen = strlen(cipherText);
    char clearText[ctLen];
    char keyString[keyLengths[0]];

        /* Declare variable to store shift values of dictionary*/
        char dictChars[] = {'\0', '\0'};
        analyzeFrequencies(enDict, keyLengths, dictChars);

    /* Analyse frequency distribution of every nth column*/
    uint col;
    for(col = 0; col < keyLengths[0]; col++){
        uint i;
        uint j = 0;
        uint totColSz = (ctLen / keyLengths[0]) + col + 1;
        char colChars[totColSz + 1];

        /* Add every nth column to char array */
        for(i = 0; i < ctLen; i++){
            if(i % (keyLengths[0]) == 0){
                colChars[j] = cipherText[i + col];
                j++;
            }
        }
        
        colChars[totColSz - col] = '\0';

        /* Declare variable to store shift values of ciphertext */
        char ctChars[] = {'\0', '\0'};

        /* Compare with English dict */
        analyzeFrequencies(colChars, keyLengths, ctChars);

        //devPrint(dictChars, ctChars);

        /* Determine shift of monoalphabetic cipher */
        int detShift = (ctChars[0] - dictChars[0]);
        
        /* Apply shift and store in new string */
        char shiftedString[totColSz + 1];
        
        applyShift(shiftedString, colChars, detShift, totColSz, col, alphabet);

        /* Copy shifted column values to decrypted string */
        insertStringAtCol(shiftedString, clearText, col, ctLen, keyLengths[0]);

        /* Add a letter to the key string */
        uint offset = alphabet[0];

        if(detShift < 0){
            keyString[col] = ((alphabet[strlen(alphabet) - 1]) + (detShift + 1));
        }else{
            keyString[col] = offset + detShift;
        }
    }

    keyString[col] = '\0';
    clearText[ctLen] = '\0';

    printf("\nKey(%li): '%s', Clear text: \n%s\n", strlen(keyString), keyString, clearText);
}
