#include <stdio.h>
#include <string.h>
#include "generic.h"

void getFactors(uint number, numFreqs *freqNums)
{
	uint i, j = 0;
    uint minKey = 3;

	printf("Factors(%d) are: ", number);
	for(i=minKey; i <= number; i++){
		if(number % i == 0)
            j++;
	}

    uint myNumbers[j];
    uint k = 0;
	
    for(i=minKey; i <= number; i++){
		if(number % i == 0){
            printf("%d ", i);
            myNumbers[k] = i;
            k++;
        }
	}

    /* Push numbers to linked list */
    pushNums(myNumbers, j, freqNums);
    printf("\n");
}

/* Determines most likeley key sizes using Kasinski's examination 
 * Returns: void
 * Input: cipherText, keyLengths array
 * */
extern void returnKeyLengths(char *cipherText, uint *keyLengths)
{
   uint len = strlen(cipherText);

   /* Define frequent numbers struct */
   numFreqs *freqNums = malloc(sizeof(numFreqs)); 
   freqNums->next = NULL;

   printf("Cipher text: %s, len: %d\n", cipherText, len);

   uint minKey = 3;

   /* This must be tweaked during testing */
   uint maxLength = len / 4 + 1;
    if(maxLength > 12)
        maxLength = 12;
   printf("Determined max key length: %d\n", maxLength);

   /* For each key length starting from 3 to maxLength, 
    * look for repeated patterns */
   uint size;
   for(size = minKey; size <= maxLength; size++){
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
               uint w2Offset = w2Cnt + w1Cnt;
               uint distance = ((w2Cnt + w1Cnt) - w1Cnt);
               printf("Pattern: %s(%d) \t= %s(%d). Size: %d\tdist: %d\t", w1, w1Cnt, w2, w2Offset, size, distance);

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
       printf("%d(%d) ", curr->number, curr->cnt);
       if(curr->cnt >= highestCnt){
           /* Store previous highest */
           keyLengths[1] = highest;

           highestCnt = curr->cnt;
           highest = curr->number;
           /* Store current highest */
           keyLengths[0] = highest;
       }
       curr = curr->next;
   }
   printf("\nHighest factor with higest count: %d (cnt: %d)\n", highest, highestCnt);

   freeNums(freqNums);
}

/* Main is just for testing now */
int main(void)
{
    //char cipherText[] = "AREIILUYZTJKUBSOLXBRQGYTHVCKEHHUCCRTNXUKAGATWZKVQROZDDRWSHXPDNUTNTJIXXRYTHVALGNMEWRJHZTZNJOLCHNNBDLRFKCCDNJLKWNVPKEHSWJEXKKBJOLXBCRCBTNXJOAUOXFECCDKKTHVNURUKOKAHBXHGXNGOCGEXCROTXABARRHNPLXBSOLWVUVOGPKBMHRXHNDYLNMLKWNPOXXPOGWMOZDDSXSZUADQHTNBJQCRMGDASCPOXXNOJOIYMEMORRZXODRQGZAACLUIVM";
	//char cipherText[] = "CVJTNAFENMCDMKBXFSTKLHGSOJWHOFUISFYFBEXEINFIMAYSSDYYIJNPWTOKFRHWVWTZFXH\
LUYUMSGVDURBWBIVXFAFMYFYXPIGBHWIFHHOJBEXAUNFIYLJWDKNHGAOVBHHGVINAULZFOF\
UQCVFBYNFTYGMMSVGXCFZFOKQATUIFUFERQTEWZFOKMWOJYLNZBKSHOEBPNAYTFKNXLBVU\
AXCXUYYKYTFRHRCFUYCLUKTVGUFQBESWYSSWLBYFEFZVUWTRLLNGIZGBMSZKBTNTSLNNMD\
PMYMIUBVMTLOBJHHFWTJNAUFIZMBZLIVHMBSUWLBYFEUYFUFENBRVJVKOLLGTVUZUAOJNVU\
WTRLMBATZMFSSOJQXLFPKNAULJCIOYVDRYLUJMVMLVMUKBTNAMFPXXJPDYFIJFYUWSGVIUM\
BWSTUXMSSNYKYDJMCGASOUXBYSMCMEUNFJNAUFUYUMWSFJUKQWSVXXUVUFFBPWBCFYL\
WFDYGUKDRYLUJMFPXXEFZQXYHGFLACEBJBXQSTWIKNMORNXCJFAIBWWBKCMUKIVQTMNBC\
CTHLJYIGIMSYCFVMURMAYOBJUFVAUZINMATCYPBANKBXLWJJNXUJTWIKBATCIOYBPPZHLZJJZ\
HLLVEYAIFPLLYIJIZMOUDPLLTHVEVUMBXPIBBMSNSCMCGONBHCKIVLXMGCRMXNZBKQHODESY\
TVGOUGTHAGRHRMHFREYIJIZGAUNFZIYZWOUYWQZPZMAYJFJIKOVFKBTNOPLFWHGUSYTLGN\
RHBZSOPMIYSLWIKBANYUOYAPWZXHVFUQAIATYYKYKPMCEYLIRNPCDMEIMFGWVBBMUPLHML\
QJWUGSKQVUDZGSYCFBSWVCHZXFEXXXAQROLYXPIUKYHMPNAYFOFHXBSWVCHZXFEXXXAIR\
PXXGOVHHGGSVNHWSFJUKNZBESHOKIRFEXGUFVKOLVJNAYIVVMMCGOFZACKEVUMBATVHKID\
MVXBHLIVWTJAUFFACKHCIKSFPKYQNWOLUMYVXYYKYAOYYPUKXFLMBQOFLACKPWZXHUFJYG\
ZGSTYWZGSNBBWZIVMNZXFIYWXWBKBAYJFTIFYKIZMUIVZDINLFFUVRGSSBUGNGOPQAILIFOZ\
BZFYUWHGIRHWCFIZMWYSUYMAUDMIYVYAWVNAYTFEYYCLPWBBMVZZHZUHMRWXCFUYYVIEN\
FHPYSMKBTMOIZWAIXZFOLBSMCHHNOJKBMBATZXXJSSKNAULBJCLFWXDSUYKUCIOYJGFLMBW\
HFIWIXSFGXCZBMYMBWTRGXXSHXYKZGSDSLYDGNBXHAUJBTFDQCYTMWNPWHOFUISMIFFVXF\
SVFRNA";

    // challenge: https://pi.math.cornell.edu/~mec/2003-2004/cryptography/polyalpha/polyalpha.html
    char cipherText[] = "ICJEVAQIPWBCIJRQFVIFAZCPQYMJAHNGFYDHWEQRNARELKBRYGPCSPKWBUPGKBKZWDSZXSAFZLOIWETVPSITQISOTFKKVTQPSEOWKPVRLJIECHOHITFPSUDXXARCLJSNLUBOIPRJHYPIEFJERBTVMUQOIJZAGYLOHSEOHWJFCLJGGTWACWEKEGKZNASGEKAIETWARJEDPSJYHQHILOEBKSHAJVYWKTKSLOBFEVQQTPHZWERZAARVHISOTFKOGCRLCJLOKTRYDHZZLQYSFYWDSWZOHCNTQCPRDLOARVHSOIERCSKSHNARVHLSRNHPCXPWDSILPLZVQLJOENLWZJFSLCIEDJRRYXJRVCVPOEOLJUFYRQFGLUPHYLWISOTFKWJERNSTZQMIVCWDSCZVPHVCUEHFCBEBKPAWGEPZISOTFKOEODNWQZQWHYPVAHKWHISEEGAHRTOEGCPIPHFJRQ";


   /* Currently only interested in the two most likely key lengths */
   uint keyLengths[2];
   returnKeyLengths(cipherText, keyLengths);
   printf("Most likely key length: %d or: %d\n", keyLengths[0], keyLengths[1]);

   return 0;
}
