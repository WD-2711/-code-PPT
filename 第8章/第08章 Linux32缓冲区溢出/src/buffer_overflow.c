/** ==================================================================== */
/* This example demonstrates the buffer overflow error.                  */
/* Fanping Zeng.  09/25/2013      buffer_overflow.c                      */
/* ===================================================================== */
// gcc -fno-stack-protector -o buf ../src/buffer_overflow.c
// gcc -fno-stack-protector -z execstack -o buf ../src/buffer_overflow.c
#include <stdio.h>
#include <string.h>

// Define a large buffer with 32 bytes.
char Lbuffer[] = "01234567890123456789========ABCD";

void foo()
{
    char buff[16];
    strcpy (buff, Lbuffer);
}

void foo01()
{
    char buff[16];
    // Define a large buffer with 32 bytes.
    char Lbuffer[] = "01234567890123456789========ABCD";    
    strcpy (buff, Lbuffer);
}
void foo02()
{
    // Define a large buffer with 32 bytes.
    char Lbuffer[] = "01234567890123456789========ABCD";    
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
    foo();    return 0;
//    foo01();    return 0;
//    foo02();    return 0;
}

