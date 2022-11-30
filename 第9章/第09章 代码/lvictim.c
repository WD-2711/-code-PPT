/** lvictim.c
  Coded by Fanping Zeng(USTC). 10/01/2013. Modified on 10/31/2022
  To show a program with a buffer overflow vulnerability.
  gcc -o lvictim -fno-stack-protector -z execstack ../src/lvictim.c
*/
//====================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// You should determine RETURN address through GDB.
//#define RETURN 0xbffff1b0
#define LARGE_BUFF_LEN 1024

//=============================================================================
// Get something from file or environment to a small buffer.
//=============================================================================
void smash_smallbuf(char * largebuf)
{
    char buffer[32];
    FILE *badfile;

    badfile = fopen("./SmashSmallBuf.bin", "r");
    fread(largebuf, sizeof(char), LARGE_BUFF_LEN, badfile);
    fclose(badfile);
    largebuf[LARGE_BUFF_LEN]=0;

    printf("Smash a small buffer with %d bytes.\n\n",strlen(largebuf));
    strcpy(buffer, largebuf);	// smash it and get a shell.
}

//=============================================================================
// Get something from file or environment to a large buffer.
//=============================================================================
void smash_largebuf(char * largebuf)
{
    char buffer[512];
    FILE *badfile;
    badfile = fopen("./SmashLargeBuf.bin", "r");
    fread(largebuf, sizeof(char), LARGE_BUFF_LEN, badfile);
    fclose(badfile);
    largebuf[LARGE_BUFF_LEN]=0;
    printf("Smash a large buffer with %d bytes.\n\n",strlen(largebuf));
    strcpy(buffer, largebuf);	// smash it and get a shell.
}
//=============================================================================
// Get something from file or environment to a large buffer.
//=============================================================================
void smash_it(char * buf)
{
    char buffer[32];
    printf("Huge buffer (%d bytes) smash a real buffer(%d bytes) addr=%p.\n\n",strlen(buf), sizeof(buffer), buffer);
    strcpy(buffer, buf);	// smash it and get a shell.
}

#define ATTACK_LEN 1024*1024*2
void smash_realbuf()
{
    char hugebuf[ATTACK_LEN+1];
    FILE *badfile;

    badfile = fopen("./SmashRealBuf.bin", "r");
    fread(hugebuf, sizeof(char), ATTACK_LEN, badfile);
    fclose(badfile);
    hugebuf[ATTACK_LEN]=0;

    smash_it((char *)hugebuf);
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
int main(int argc, char * argv[])
{
    char attackStr[LARGE_BUFF_LEN+1];

    switch(argc)
    {
        case 1: smash_smallbuf(attackStr); break;
        case 2: smash_largebuf(attackStr); break;
        case 3: smash_realbuf(); break;
        default: puts("exit with return code 0.");  break;
    }    
    return 0;

}

