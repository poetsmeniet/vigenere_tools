#include "main.h"
#include "tableaugen.h"

int main(void)
{
    /* Currently the alphabet is hard coded */
    /* Fuller ascii charset: 
     * cchar alphabet[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
     * */
    cchar alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    cuint aSize = strlen(alphabet);
    
    /* The multi dimensional array to store the tableau */
    char tab[aSize * 2][aSize * 2];

    /* Testing populate tableau with chars */
    _Bool ret = populateTableau(aSize, tab, alphabet);
    
    if(ret){
        printTableau(aSize, tab, (uint)alphabet[0]);
    }else{
        return EXIT_FAILURE;
    }

    /* Test excoding */
    //char clearText[] = "DONTWORRYBEHAPPY";
    //char key[] = "CHECKIO";
    char clearText[] = "ATTACKATDAWN";
    char key[] = "LEMONLEMONLE";
    
    cuint sz = strlen(clearText);
    char cipherText[sz];

    returnCipherText(key, clearText, cipherText, aSize, tab);

    printf("Message '%s', encrypted with key '%s' had value: '%s'\n", clearText, key, cipherText);

    return EXIT_SUCCESS;
}
