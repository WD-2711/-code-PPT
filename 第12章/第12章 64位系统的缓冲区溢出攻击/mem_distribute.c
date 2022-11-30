/* mem_distribute.c
*
*  Coded by Fanping Zeng. 09/25/2013 
*  To show the mem map on Linux system for the IA32 architecture.
*
*/

#include <stdio.h>
#include <string.h>


int fun1(int a, int b)
{
	return a+b;
}

int fun2(int a, int b)
{
	return a*b;
}

int x=10, y, z=20;

int main (int argc, char *argv[])
{
    char buff[64];
    
    int a=5,b,c=6;

    printf("(.text)address of \n\tfun1=%p  \n\tfun2=%p  \n\tmain=%p\n", fun1, fun2, main);
    printf("(.data inited Global variable)address of \n\tx(inited)=%p  \n\tz(inited)=%p\n", &x, &z);
    printf("(.bss uninited Global variable)address of \n\ty(uninit)=%p\n\n", &y);

    printf("(stack)address of \n\targc   =%p  \n\targv   =%p  \n\targv[0]=%p\n", &argc, &argv, argv[0]);
    printf("(Local  variable)address of  \n\tvulnbuff[64]=%p\n", buff);
    printf("(Local  variable)address of \n\ta(inited)   =%p  \n\tb(uninit)   =%p  \n\tc(inited)   =%p\n\n", &a, &b, &c);

    return 0;
}


