/* vulnerable.c
*  gcc -fno-stack-protector -o v vulnerable.c 
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  Vulnerable program on the IA32 architecture.
*/
// Modified for new linux by Dr. Fanping Zeng in 2014.
// (1) add foo()
// (2) gcc -fno-stack-protector -z execstack -o v vulnerable.c 

#include <stdio.h>
#include <string.h>

// Declare a large buffer.
char Lbuffer[128];

void foo()
{
    char vulnbuff[16];
    strcpy (vulnbuff, Lbuffer);
    printf ("\n%s\n", vulnbuff);
    getchar(); /* for debug */
}

int main (int argc, char *argv[])
{
    // Copy a string from the command line argument.
    strcpy (Lbuffer, argv[1]);
    foo();
}
