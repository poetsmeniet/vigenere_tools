#include <getopt.h> /* getopt_long */
#include "generic.h"
#include "main.h" 
#include "tableaugen.h"

/* A couple of encoding examples found on the internets: 
 * 
 * KEY STRING; EXPECTED RESULT
 * 
 CHECKIO DONTWORRYBEHAPPY; FVRVGWFTFFGRIDRF
 LEMON ATTACKATDAWN; LXFOPVEFRNHR
 HOUGHTON MICHIGANTECHNOLOGICALUNIVERSITY; TWWNPZOAASWNUHZBNWWGSNBVCSLYPMM
 PUCK WHATFOOLSTHESEMORTALSBE; LBCDUIQVHNJOHYOYGNCVHVG
*/

/* Todo;
 * sanity check input
 * set alphabet default, unless external file present
 * */

int main(int argc, char **argv)
{
    /* Currently the alphabet is hard coded */
    /* Fuller ascii charset: 
     * cchar alphabet[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
     * */
    cchar alphabet[] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    cuint aSize = strlen(alphabet);

    /* The offset is the decimal index representation of the first letter in the
     * alphabet. I figured I could use the decimal representation of characters 
     * to avoid using a hashing function. It might have been a bad idea. It does
     * make the code cleaner though
     * */
    cuint offset = (uint)alphabet[0];
    
    /* The multi dimensional array to store the tableau 
     * Note: the size of the array is offset to be able
     * to store using ascii offset index use
     * */
    cuint dim = offset + aSize;
    char tab[dim][dim];

    /* Testing populate tableau with chars */
    _Bool ret = populateTableau(aSize, tab, alphabet);
    if(!ret)
        return EXIT_FAILURE;
    
    /* Using getopt_long for parsing command line parameters */
    uint sz = 0;
    char *clearText = NULL, *key = NULL, *cipherText = NULL;
    int c;
    
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"encode",     required_argument, 0,  'e'},
            {"decode",  required_argument,       0,  'd'},
            {"kasinsky",  required_argument, 0,  'k'},
            {"verbose", no_argument,       0,  'v'},
            {0,         0,                 0,  0}
        };
    
        c = getopt_long(argc, argv, "e:d:K:v01",
                 long_options, &option_index);

        if (c == -1 && optind == 1){
            displayHelp();
            break;
        }
        
        if (c == -1)
            break;
    
        int index;
    
        switch (c) {
        case 'v':
            /* Printing the tableau */
            printTableau(aSize, tab, offset);
            printf("\n");
            break;
    
        case 'e':
            index = optind - 1;

            if(argc - index == 1){
                displayHelp();
                break;
            }
            while(index < argc){
                if(argc - index == 1){
                    sz = strlen(argv[index]);
                    clearText = strdup(argv[index]);
                }else if(argc - index == 2){
                    key = strdup(argv[index]);
                }
                index++;
            }
            optind = index;
            
            /* Encoding function */
            cipherText = malloc(sz);

            returnCipherText(key, clearText, cipherText, aSize, tab);

            printf("%s\n", cipherText);

            free(clearText);
            free(key);
            free(cipherText);
            
            break;
    
        case 'd':
            index = optind - 1;

            while(index < argc){
                if(argc - index == 1){
                    sz = strlen(argv[index]);
                    cipherText = strdup(argv[index]);
                }else{
                    key = strdup(argv[index]);
                }

                index++;
            }
            optind = index;

            /* Decoding function using known key */
            char *clearTextDecoded = malloc(sz +1);
            returnClearText(key, clearTextDecoded, cipherText, aSize, tab, offset);

            printf("%s\n", clearTextDecoded);

            free(clearTextDecoded);
            free(cipherText);
            free(key);
            break;
    
        case 'K':
            printf("option Kasinsky\n");
            break;

        case '?':
                break;
    
        default:
            printf("?? getopt returned character code 0%o ??\n", c);
            displayHelp();
        }
    }
    
    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }

    return EXIT_SUCCESS;
}
