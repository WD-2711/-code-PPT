//  mypara:   gcc -o mypara ../src/mypara.c

#include <stdio.h>
#include <stdlib.h>

int f01(const char *command)
{
    system(command);
    return 0;
}

void f02()
{
    char commandToRun[] = "/bin/sh";
    f01( commandToRun );
}

int main(int argc, char * argv[])
{
    f02();
    return 0;
}

