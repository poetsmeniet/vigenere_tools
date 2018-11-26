#include "main.h"
#include "tableaugen.h"

int main(int argc, char **argv)
{
    /* Currently the alphabet is hard coded */
    /* Fuller ascii charset: 
     * cchar alphabet[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
     * */
    cchar alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    cuint aSize = strlen(alphabet);

    /* The offset is the decimal index representation of the first letter in the
     * alphabet. I figured I could use the decimal representation of characters 
     * to avoid using a hashing function. It might have been a bad idea though.
     * */
    cuint offset = (uint)alphabet[0];
    
    /* The multi dimensional array to store the tableau 
     * Note: the size of the array is doubled to be able
     * to store using ascii offset index use
     * */
    cuint dim = aSize * 2;
    char tab[dim][dim];

    /* Testing populate tableau with chars */
    _Bool ret = populateTableau(aSize, tab, alphabet);
    
    /* Printing the tableau */
    if(ret){
        printTableau(aSize, tab, offset);
        printf("\n");
    }else{
        return EXIT_FAILURE;
    }

    /* Test encoding, going to offload to argv */
    //char clearText[] = "DONTWORRYBEHAPPY"; //FVRVGWFTFFGRIDRF
    //char key[] = "CHECKIO";
    //char clearText[] = "ATTACKATDAWN"; //LXFOPVEFRNHR
    //char key[] = "LEMON";
    //char clearText[] = "MICHIGANTECHNOLOGICALUNIVERSITY"; //TWWNPZOAASWNUHZBNWWGSNBVCSLYPMM
    //char key[] = "HOUGHTON";
    char clearText[] = "WHATFOOLSTHESEMORTALSBE"; //LBCDUIQVHNJOHYOYGNCVHVG
    char key[] ="PUCK"; 
    //char clearText[] = "My name is Thomas, Thomas is my name"; //LBCDUIQVHNJOHYOYGNCVHVG
    //char key[] ="tjap";
    
    /* Encoding function */
    cuint sz = strlen(clearText);
    char cipherText[sz];

    returnCipherText(key, clearText, cipherText, aSize, tab);

    printf("Message '%s', encrypted with key '%s' has value: '%s'\n", clearText, key, cipherText);

    /* Decoding function using known key */
    char clearTextDecoded[sz + 1];
    returnClearText(key, clearTextDecoded, cipherText, aSize, tab, offset);

    printf("Message '%s', decrypted with key '%s' has value: '%s'\n", cipherText, key, clearTextDecoded);

    return EXIT_SUCCESS;
}
