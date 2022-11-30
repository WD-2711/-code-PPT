/* This program has a buffer overflow vulnerability.
    gcc -o stackguard ../src/stackguard.c
    gcc -fno-stack-protector -z noexecstack -g -o stackguard ../src/stackguard.c
    gdb -q ./stackguard
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char largebuf[] = "0123456789012345678901234567890123456789"; // 40 bytes

void foo(char *str)
{   
    char buffer[16];
    strcpy(buffer, str);
}

int main(int argc, char **argv, char **env)
{
    foo(largebuf);

    printf("Returned Properly.\n");
    return 0;
}
