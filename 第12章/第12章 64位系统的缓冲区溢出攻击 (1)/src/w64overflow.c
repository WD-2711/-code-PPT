/** w64overflow.c   
*  »º³åÇøÒç³öÊ¾Àý¡£ Fanping Zeng. USTC. 2013-10-09  
*/
//  cl /Zi /GS- w64overflow.c
#include <stdio.h>
#include <string.h>
char largebuff[] ="012345678901234567890123ABCDEFGH";  //32 bytes
void foo()
{
    char smallbuff[16];
    strcpy (smallbuff, largebuff);
}
int main (void)
{
    foo();
	return 0;
}
