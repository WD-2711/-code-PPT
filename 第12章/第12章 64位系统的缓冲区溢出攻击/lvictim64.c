/** lvictim64.c
  Coded by Fanping Zeng(USTC). 12/06/2014 
  gcc -fno-stack-protector -z execstack -o lvictim64 ../lvictim64.c
*/
//====================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ATTACK_STR_LEN 1024
//=============================================================================
// Get something from file or environment to a large buffer.
//=============================================================================
char attackStr[ATTACK_STR_LEN+1];
void smash_largebuf()
{
    char buffer[512];
    int  nBytesOfRead;
    FILE *badfile;

    // Initialize the attackStr with '0x90'.
    memset(attackStr, 0x90, ATTACK_STR_LEN);

    // Get the shellcode from the file SmashBuffer.data.
    badfile = fopen("./SmashBuffer.data", "r");
    nBytesOfRead = fread(attackStr, sizeof(char), ATTACK_STR_LEN, badfile);
    fclose(badfile);
    attackStr[nBytesOfRead]=0;
    attackStr[ATTACK_STR_LEN]=0;

    printf("You have read %d from the file SmashBuffer.data.\n", nBytesOfRead);
    printf("Smash a large buffer with %d bytes.\n\n",(int)strlen(attackStr));

    // smash it and get a shell. *************************************
    strcpy(buffer, attackStr);
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
int main(int argc, char * argv[])
{
    smash_largebuf();

    return 0;
}

