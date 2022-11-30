/* buffer_addr.c
*   gcc -o addr ../src/buffer_addr.c
*  Coded by Fanping Zeng. 11/08/2022 
*  To show the value of kernel.randomize_va_space and the start address of buffer.
*   sudo sysctl -w kernel.randomize_va_space=0
kernel.randomize_va_space=0  关闭地址随机化
kernel.randomize_va_space=1  栈地址随机化
kernel.randomize_va_space=2  栈和堆地址随机化
*/

#include <stdio.h>
#include <stdlib.h>

#define LEN 64
void stack()
{
    char buffer[LEN];
    printf("\tThe start address of buffer[LEN]=%p\n", buffer);
}

#define HLEN 1024
void heap()
{
    char *heapmem = malloc(sizeof(char)*HLEN);
    printf("\tThe start address of heapmem=%p\n", heapmem);
    if(heapmem != NULL)
    {
        free(heapmem);  heapmem=NULL;
    }
}

int main (int argc, char *argv[], char *evn[])
{
    stack();
    heap();
    return 0;
}

