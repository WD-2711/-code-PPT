/* This program has a buffer overflow vulnerability.
gcc -fno-stack-protector -z noexecstack -o stack ../stack.c
sudo sysctl -w kernel.randomize_va_space=0
gcc -fno-stack-protector -z noexecstack -g -o stack_dbg ../stack.c
gdb -q ./stack_dbg
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int foo(char *str)
{   
    char buffer[100];
    strcpy(buffer, str);
    return 1;
}

int main(int argc, char **argv, char **env)
{
    char str[400];
    FILE *badfile;

    badfile = fopen("badfile", "r");
    fread(str, sizeof(char), 300, badfile);
    fclose(badfile);

    foo(str);

    printf("Returned Properly.\n");
    return 0;
}
