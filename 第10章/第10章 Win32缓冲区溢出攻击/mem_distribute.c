/* mem_distribute.c
*
*  Coded by Fanping Zeng. 09/25/2013. Modified on Nov 16, 2022.
*  To show the mem map for the IA32 architecture.
*  for Windows: cl mem_distribute.c; for Linux: gcc -o mem mem_distribute.c
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

int fun3(int a)
{
  return a*10;
}

int x=10, y, z=20;

int main (int argc, char *argv[])
{
  char buff[64];
  int a=5,b,c=6;
  char buff02[64];
  printf("(.text)address of\n\tfun1=%p\n\tfun2=%p\n\tmain=%p\n", fun1, fun2, main);
  printf("(.data inited Global variable)address of\n\tx(inited)=%p\n\tz(inited)=%p\n", &x, &z);
  printf("(.bss uninited Global variable)address of\n\ty(uninit)=%p\n\n", &y);

  printf("(stack)address of\n\targc   =%p\n\targv   =%p\n", &argc, &argv);
  printf("(Local  variable)address of\n\tbuff[64]=%p\n\tbuff02[64]=%p\n", buff, buff02);
  printf("(Local  variable)address of\n\ta(inited)   =%p\n\tb(uninit)   =%p\n\tc(inited)   =%p\n\n", &a, &b, &c);
  
  return 0;
}
