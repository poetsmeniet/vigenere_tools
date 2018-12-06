#include <stdio.h>
#include <string.h>
#include "generic.h"

void printBars(uint cnt)
{
    int i;
    for(i = 0; i < cnt; i++)
        printf("-");
}

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

void analyzeFrequencies(char * letters, uint *keyLengths, char *dest)
{
   printf("\nAnalyzing frequencies of chars in string\n");
   /* Test frequency analysis */
   charFreqs *freqChars = malloc(sizeof(charFreqs));
   freqChars->next = NULL;

   /* Add */
   pushChars(letters, strlen(letters), freqChars);

   /* Sort list */
   sortList(freqChars);

   /* Read struct */
   charFreqs *curr = freqChars;
   while(curr->next != NULL){

        int i;
        for(i = 0; i < strlen(letters); i++){
            if(curr->letter == letters[i]){
                float perc = (((float)curr->cnt / (float)strlen(letters)) * 100.00);
                printf("%c: %d (%f) ", curr->letter, curr->cnt, perc);
                printBars((uint)perc * 5);
                printf("\n");

                break;
            }
        }
        curr = curr->next;
   }

   /* Set most frequent and second most frequent result */
   dest[0] = freqChars->letter;
   dest[1] = freqChars->next->letter;
   /* Free struct */
   freeChars(freqChars);
}

/* Analyze text, count all letters therein*/
void analyzeFrequenciesLetters(char * letters, char *text, uint *keyLengths, char *dest)
{
   printf("\nAnalyzing frequencies of string, using letters '%s'\n\n", letters);
   /* Test frequency analysis */
   charFreqs *freqChars = malloc(sizeof(charFreqs));
   freqChars->next = NULL;

   /* Add */
   pushChars(text, strlen(text), freqChars);

   /* Sort list */
   sortList(freqChars);

   /* Read struct */
   charFreqs *curr = freqChars;
   while(curr->next != NULL){

        int i;
        for(i = 0; i < strlen(letters); i++){
            if(curr->letter == letters[i]){
                float perc = (((float)curr->cnt / (float)strlen(text)) * 100.00);
                printf("%c: %d (%f) ", curr->letter, curr->cnt, perc);
                printBars((uint)perc * 5);
                printf("\n");

                //break;
            }

            /* Add two most common letters to dest */
            if(curr->letter == letters[i] && dest[0] == '\0'){
                //printf("1Added %c to dest[0]\n", curr->letter);
                dest[0] = curr->letter;
            }else if(curr->next->letter == letters[i] && dest[1] == '\0' && dest[0] != '\0'){
                //printf("2Added %c to dest[1]\n", curr->next->letter);
                dest[1] = curr->next->letter;
            }
        }
        curr = curr->next;
   }

   /* Free struct */
   freeChars(freqChars);
}

void getFactors(uint number, numFreqs *freqNums)
{
	uint i, j = 0;
    uint minKey = 4;

	printf("Factors(%d) are: ", number);
	for(i=minKey; i <= number; i++)
		if(number % i == 0)
            j++;

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

   uint minKey = 4;

   /* This must be tweaked during testing */
   uint maxLength = len / 4 + 1;
    if(maxLength > 12)
        maxLength = 12;
   printf("Determined max key length: %d\n", maxLength);

   /* For each key length starting from 4 to maxLength, 
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
    /* ./vtools -e AGTWKJD ALLMYCRYTANALYSISBRINGSALLTHEBOYSTOTHEYARDANDDAMNRIGHTITSBETTERTHANYOURSALLRIGHTIMIGHTADDALLWORDSALLMOSTANDSOMESPELLINGERRORALLRIGHTYTHENERRORMISTAKEALLERRORSAREBROKNEDOWNINTOONEERRORBUTBOYSISALLSOAWORDERRORTPINGSOMEMORETEXTMIGHTJUSTBETHETHINGTOMAKEITMOREREALISTICFORTESTINGTHESCRIPT */
    //char cipherText[] = "AREIILUYZTJKUBSOLXBRQGYTHVCKEHHUCCRTNXUKAGATWZKVQROZDDRWSHXPDNUTNTJIXXRYTHVALGNMEWRJHZTZNJOLCHNNBDLRFKCCDNJLKWNVPKEHSWJEXKKBJOLXBCRCBTNXJOAUOXFECCDKKTHVNURUKOKAHBXHGXNGOCGEXCROTXABARRHNPLXBSOLWVUVOGPKBMHRXHNDYLNMLKWNPOXXPOGWMOZDDSXSZUADQHTNBJQCRMGDASCPOXXNOJOIYMEMORRZXODRQGZAACLUIVM";

    // challenge: https://pi.math.cornell.edu/~mec/2003-2004/cryptography/polyalpha/polyalpha.html
    //char cipherText[] = "ICJEVAQIPWBCIJRQFVIFAZCPQYMJAHNGFYDHWEQRNARELKBRYGPCSPKWBUPGKBKZWDSZXSAFZLOIWETVPSITQISOTFKKVTQPSEOWKPVRLJIECHOHITFPSUDXXARCLJSNLUBOIPRJHYPIEFJERBTVMUQOIJZAGYLOHSEOHWJFCLJGGTWACWEKEGKZNASGEKAIETWARJEDPSJYHQHILOEBKSHAJVYWKTKSLOBFEVQQTPHZWERZAARVHISOTFKOGCRLCJLOKTRYDHZZLQYSFYWDSWZOHCNTQCPRDLOARVHSOIERCSKSHNARVHLSRNHPCXPWDSILPLZVQLJOENLWZJFSLCIEDJRRYXJRVCVPOEOLJUFYRQFGLUPHYLWISOTFKWJERNSTZQMIVCWDSCZVPHVCUEHFCBEBKPAWGEPZISOTFKOEODNWQZQWHYPVAHKWHISEEGAHRTOEGCPIPHFJRQ";

	char cipherText[] = "CVJTNAFENMCDMKBXFSTKLHGSOJWHOFUISFYFBEXEINFIMAYSSDYYIJNPWTOKFRHWVWTZFXH\
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

   /* Currently only interested in the two most likely key lengths */
   uint keyLengths[2];
   returnKeyLengths(cipherText, keyLengths);
   printf("Most likely key length: %d or: %d\n", keyLengths[0], keyLengths[1]);


   /* Analyse frequenty distribution of every nth column*/
   uint i;
   uint j = 0;
   uint col = 5;
   uint totSz = strlen(cipherText);
   uint totColSz = (strlen(cipherText) / keyLengths[0]) + col + 1;
   char letters[totColSz + 1];

   /* Add every nth column to char array */
   for(i = 0; i < totSz; i++){
       if(i % (keyLengths[0]) == 0){
           letters[j] = cipherText[i + col];
           //printf("add letter (index %d) '%c'\n", j, letters[j]);
           j++;
       }
   }
   
   printf("totColSz: %d, j=%d\n", totColSz, j);
   letters[totColSz - col] = '\0';

   /* Declare variables to storeshift values */
   char dictChars[] = {'\0', '\0'};
   char ctChars[] = {'\0', '\0'};

   /* compare with English dict */
   analyzeFrequencies(enDict, keyLengths, dictChars);
   analyzeFrequencies(letters, keyLengths, ctChars);

   printf("dict: %c en %c\n", dictChars[0], dictChars[1]);
   printf("ct: %c en %c\n", ctChars[0], ctChars[1]);

   //printf("Shift 1st: (%c - %c): %d\n", ctChars[0], dictChars[0], (ctChars[0] - dictChars[0]));
   //printf("Shift 2nd: (%c - %c): %d\n", ctChars[1], dictChars[1], (ctChars[1] - dictChars[1]));
   //printf("manual: %c - %c = %d\n", 'S', 'E', 'S' - 'E');
   //printf("manual: %c - %c = %d\n", 'S', 'T', 'S' - 'T');
   //printf("manual: %c - %c = %d\n", 'S', 'A', 'S' - 'A');

   /* Shift monoalphabetic cipher */
   int detShift = (ctChars[0] - dictChars[0]);

   char shiftedString[totColSz + 1];printf("totcolsz: %d\n", totColSz);
   int shiftChar;
   uint l;

   /* Apply shift to column */
   for(l = 0; l < totColSz; l++){
       shiftChar = letters[l] - (detShift - 26);

       uint modulus; 

       if(col == 0)
           modulus = 65;
       else
           modulus = 64;

       /* Greater than Z -> modulo */
       if(shiftChar > 90)
           shiftChar = (shiftChar % 90) + modulus;

       if(detShift == 1)
           shiftChar -= 1;

       if(shiftChar < 65)
           shiftChar = 'Z';

       shiftedString[l] = shiftChar;
   }
   
   shiftedString[totColSz - col] = '\0';

   printf("Shifting word (col %d):\n'%s' to: \n'%s' by value %d\n", col, letters, shiftedString, detShift);

   return 0;
}
