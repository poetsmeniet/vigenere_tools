#include "main.h"
#include "tableaugen.h"

int main(void)
{
    /* Currently the alphabet is hard coded */
    cchar alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    cuint size = strlen(alphabet);
    
    /* The multi dimensional array to store the tableau */
    char tab[size][size];

    /* Testing populate tableau with chars */
    _Bool ret = populateTableau(size, tab, alphabet);

    if(ret){
        printTableau(size, tab, (uint)alphabet[0]);
    }else{
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
