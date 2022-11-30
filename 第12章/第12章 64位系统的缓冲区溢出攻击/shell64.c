// shell.c   gcc -o shell shell64.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void foo()
{
    char * name[2];
    name[0] = "/bin/sh";
    name[1] = NULL;
    execve( name[0], name, NULL );
}

int main(int argc, char * argv[])
{
    foo();  return 0;
}
