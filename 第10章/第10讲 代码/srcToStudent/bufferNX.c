/* bufferNX.c
*   gcc -o bufferNX ../src/bufferNX.c
*  Coded by Fanping Zeng. 11/08/2022 
*  To show the No-eXecute of buffer.
    gcc -z execstack -o bufferNX ../src/bufferNX.c      栈可执行
    gcc -z noexecstack -o bufferNX ../src/bufferNX.c    栈不可执行
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char shellcode_dosh[] = //  do "/bin/sh"
"\x31\xd2"              // xor    %edx,%edx
"\x52"                  // push   %edx
"\x68""n/sh"    //"\x68\x6e\x2f\x73\x68"  // push   $0x68732f6e   
"\x68""//bi"    //"\x68\x2f\x2f\x62\x69"  // push   $0x69622f2f   
"\x89\xe3"              // mov    %esp,%ebx
"\x52"                  // push   %edx
"\x53"                  // push   %ebx
"\x89\xe1"              // mov    %esp,%ecx
"\x8d\x42\x0b"          // lea    0xb(%edx),%eax
"\xcd\x80";             // int    $0x80

void run_stack()
{
    char buffer[sizeof(shellcode_dosh)];
    printf("\tRun at buffer[%d]=%p\n", sizeof(buffer), buffer);
    strcpy(buffer, shellcode_dosh);
    ((void (*)())buffer)();
}

void run_heap()
{
    char *heapmem = malloc(sizeof(shellcode_dosh));
    printf("\tRun at heapmem[%d]=%p\n", sizeof(heapmem), heapmem);
    strcpy(heapmem, shellcode_dosh);
    ((void (*)())heapmem)();
    if(heapmem != NULL)
    {
        free(heapmem);  heapmem=NULL;
    }
}

int main (int argc, char *argv[], char *evn[])
{
    //printf("sizeof(\"123456\"=%d\n)", sizeof("123456")); return 0;
    if(argc==1)
    {    run_stack();}
    else{   run_heap();}
    return 0;
}

