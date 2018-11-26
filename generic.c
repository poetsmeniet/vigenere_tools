#include <stdio.h>

void displayHelp()
{
    printf("Usage: vtools OPTION... [KEY]... [STRING]\n");
    printf("Encodes/decodes/cracks STRING with vigenere cipher using custom alphabet\n");
    printf("Options:\n");
    printf("\t-e, --encode\t\tuse KEY to encode STRING\n");
    printf("\t-d, --encode\t\tuse KEY to decode STRING\n");
    printf("\t-K, --kasinsky\t\tAttempt cracking using Kasinski examination\n");
    printf("\t-v, --verbose\t\tshow alphabet, tableau etc\n");
}
