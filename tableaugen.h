#ifndef tableaugen_H_
#define tableaugen_H_

/* Populates the vigenere tableau
 * Indexes to access tableau are the ascii values of chars
 * Returns: 1 on success
 *          0 on failure
 * */
_Bool populateTableau(cuint size, char (*tab)[size], cchar *alphabet);

/* Prints out the generated tableau 
 * Returns void
 * */
void printTableau(cuint size, char (*tab)[size], uint offset);

/* Encodes clearText with key using tableau
 * Returns void
 * */
void returnCipherText(cchar *key, cchar *clearText, char *cipherText, cuint aSize, char (*tab)[aSize]);
#endif
