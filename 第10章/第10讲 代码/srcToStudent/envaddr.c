/* envaddr.c
    export MYSHELL="/bin/sh"
    gcc -o envaddr ../envaddr.c
    ./envaddr
    try export MYSHELL="/bin/cat /etc/passwd"
    try export MYSHELL="/bin/cat /etc/shadow"
    gcc -g -o envaddr_dbg ../envaddr.c
    gdb -q envaddr_dbg
*/

#include <stdlib.h>
#include <stdio.h>

int main()
{
    char *shell = (char *)getenv("MYSHELL");
    if(shell)
    {
        printf("\t Value: %s\n", shell);
        printf("\t Address: %x\n", (unsigned int)shell);
    }
    return 0;
}
