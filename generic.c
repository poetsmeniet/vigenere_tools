#include <stdio.h>
#include "generic.h"

extern void pushNums(uint *numbers, uint sz, numFreqs *freqNums)
{
    uint i;
    uint maxFactor = 20;

    for(i = 0; i < sz; i++){
        uint incr = 0;

        /* Appropriate next item in list */
        numFreqs *curr = freqNums;
        while(curr->next != NULL){
            /* Number exists? cnt is incremented */
            if(curr->number == numbers[i]){
                curr->cnt++;
                incr = 1;
                break;
            }
            curr = curr->next;
        }
        
        /* A new number is added */
        if(incr == 0 && numbers[i] < maxFactor){
            curr->number = numbers[i];
            curr->cnt = 1;
            curr->next = malloc(sizeof(numFreqs));
            curr->next->next = NULL;
            curr = curr->next;
        }
    }
}

extern void pushChars(char *letters, uint sz, charFreqs *freqChars)
{
    uint i;

    for(i = 0; i < sz; i++){
        uint incr = 0;

        /* Appropriate next item in list */
        charFreqs *curr = freqChars;
        while(curr->next != NULL){
            /* Number exists? cnt is incremented */
            if(curr->letter == letters[i]){
                curr->cnt++;
                incr = 1;
                break;
            }
            curr = curr->next;
        }
        
        /* A new number is added */
        if(incr == 0){
            curr->letter= letters[i];
            curr->cnt = 1;
            curr->next = malloc(sizeof(numFreqs));
            curr->next->next = NULL;
            curr = curr->next;
        }
    }
}


extern void freeNums(numFreqs *freqNums)
{
    numFreqs *curr = freqNums;
    numFreqs *currNext = NULL;
    while(curr != NULL){
        currNext = curr->next;
        free(curr);
        curr = currNext;
    }
}

extern void freeChars(charFreqs *freqChars)
{
    charFreqs *curr = freqChars;
    charFreqs *currNext = NULL;
    while(curr != NULL){
        currNext = curr->next;
        free(curr);
        curr = currNext;
    }
}

extern void displayHelp()
{
    printf("Usage: vtools OPTION... [KEY]... [STRING]\n");
    printf("Encodes/decodes/cracks STRING with vigenere cipher using custom alphabet\n");
    printf("Options:\n");
    printf("\t-e, --encode\t\tuse KEY to encode STRING\n");
    printf("\t-d, --encode\t\tuse KEY to decode STRING\n");
    printf("\t-K, --kasinsky\t\tAttempt cracking using Kasinski examination\n");
    printf("\t-v, --verbose\t\tshow alphabet, tableau etc\n");
}
