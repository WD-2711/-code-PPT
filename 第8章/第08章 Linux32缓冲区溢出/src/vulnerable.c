/*  vulnerable.c
*   Modified for ubuntu-16.04.6-desktop-i386 by Dr. Fanping Zeng on Dec. 28, 2019.
*   gcc -fno-stack-protector -z execstack -o vul ../src/vulnerable.c 
*/

#include <stdio.h>
#include <string.h>

// Define a large buffer with 32 bytes.
char Lbuffer[] = "01234567890123456789========ABCD";

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

/*  
On Dec. 28, 2019, Dr. Fanping Zeng find an interesting fact which was described as below.
From ubuntu14.0, there are some strong protection in main(int argc, char *argv[]), we cannot exploit a buffer overflow vulnerability in main function. But we can exploit a function other than main.
*/
int main2012 (int argc, char *argv[])
{
    char vulnbuff[16];
    strcpy (vulnbuff, argv[1]);
    printf ("\n%s\n", vulnbuff);
    getchar(); /* for debug */
}

