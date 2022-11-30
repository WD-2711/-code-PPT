/** overflow.c   
    »º³åÇøÒç³öÊ¾Àý¡£ Fanping Zeng. USTC. 2013-10-09. Modified on Nov 16, 2022.
    cl overflow.c
    cl /Fd /Zi overflow.c
    cl /Fd /Zi /GS- overflow.c
*/
#include <stdio.h>
#include <string.h>

char largebuff[] ="01234567890123456789ABCDEFGH";  //28 bytes
void foo()
{
    char smallbuff[16];
    strcpy (smallbuff, largebuff);
}
int main (void)
{
    foo();
}
