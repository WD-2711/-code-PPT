/** w32Lvictim.cpp
  Coded by Fanping Zeng(USTC). 
  cl /Zi /GS- w32Lvictim.cpp
*/
//====================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LARGE_BUFF_LEN 1024
//=============================================================================
// Get something from file or environment to a small buffer.
//=============================================================================
#define BUFF_LEN 512
void overflow(char largebuf[])
{
    char buffer[BUFF_LEN];
    strcpy(buffer, largebuf);	// smash it and run shellcode.
}

void smash_buffer()
{
    char largebuf[LARGE_BUFF_LEN+1];
    FILE *badfile;

    badfile = fopen("attackstr.data", "r");
    fread(largebuf, sizeof(char), LARGE_BUFF_LEN, badfile);
    fclose(badfile);
    largebuf[LARGE_BUFF_LEN]=0;

    printf("Smash a buffer with %d bytes.\n\n",strlen(largebuf));
    overflow(largebuf);	// smash it and run shellcode.
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
int main(int argc, char * argv[])
{
    smash_buffer();
    return 0;
}
