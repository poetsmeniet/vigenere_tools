#include <stdio.h>
#include <string.h>
#include "main.h"

void printFactors(uint number)
{
	int i;

	printf("Factors of %d are: ", number);
	for(i=1; i <= number; i++){
		if (number % i == 0)
			printf("%d ",i);
	}
    printf("\n");
}

void returnRepeatedPatterns(int len, char *cipherText)
{
    printf("Cipher text: %s, len: %d\n", cipherText, len);

	uint maxLength = len / 4 + 1;
    printf("Determined max key length: %d\n", maxLength);

    /* For each key length starting from 3 to maxLength, 
     * look for repeated patterns */
    uint size;
    for(size = 3; size <= maxLength; size++){
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
             *  3. determine most common factors */
            if(strcmp(w1, w2) == 0){
                uint w2Offset = w2Cnt + w1Cnt;
                uint distance = ((w2Cnt + w1Cnt) - w1Cnt);
                printf("Repeating pattern found! %s(offset: %d) = %s(offset: %d). Size: %d, distance: %d\t", w1, w1Cnt, w2, w2Offset, size, distance);

                printFactors(distance);
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

}

int main(void)
{
    char cipherText[] = "VHVSSPQUCEMRVBVBBBVHVSURQGIBDUGRNICJQUCERVUAXSSR";
    returnRepeatedPatterns(strlen(cipherText), cipherText);
    return 0;
}
