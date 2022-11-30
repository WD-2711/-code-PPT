/** ==================================================================== */
/* This example demonstrates the buffer overflow error.                  */
/* Fanping Zeng.  09/25/2013      buffer_overflow.c                      */
/* ===================================================================== */
// Linux: gcc -o b ../buffer_overflow.c
// Linux: gcc -fno-stack-protector -o b ../buffer_overflow.c
// Windows: cl ../buffer_overflow.c
#include <stdio.h>
#include <string.h>

// Define a large buffer with 32 bytes.
char Lbuffer[] = "01234567890123456789========ABCD";//32Bytes
void foo()
{
    char buff[16];
    strcpy (buff, Lbuffer);
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
/*   argc, argv are the entire command line.                             */
/* ===================================================================== */

int main(int argc, char * argv[])
{
    foo();
    return 0;
}


