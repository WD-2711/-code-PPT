/** example.c
  an example for buffer overflow vulnerability.
  gcc -o example example.c
*/
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[])
{
    char BigBuffer[]="012345678901234567890123456789AB"; //32 Bytes
    char buf01[16];
    char SmallBuffer[16];
    char buf02[16];
    
    printf("  address of BigBuffer=%p\n", BigBuffer);
    printf("      address of buf01=%p\n", buf01);
    printf("address of SmallBuffer=%p\n", SmallBuffer);
    printf("      address of buf02=%p\n\n", buf02);
    
    strcpy(buf01,"Buf01");
    strcpy(buf02,"Buf02");    
    printf("Original buf01='%s'\n", buf01);
    printf("Original buf02='%s'\n\n", buf02);
    
    strcpy(SmallBuffer, BigBuffer);
    puts("After strcpy is done,");
    printf("\tbuf01='%s'\n\tbuf02='%s'\n", buf01,buf02);
    
    return 0;
}

