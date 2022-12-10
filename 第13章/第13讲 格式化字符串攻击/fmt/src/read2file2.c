/* read2file2.c */
/* read from keyboard and write to file */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void read2file()
{
    char buf[1024];
    int fp,size;
    unsigned int u_addr, *address;
 
    // getting the address of the variable. 
    puts("Please enter an address.");
    scanf("%u", &u_addr);
    address = (unsigned int *)buf;
    *address = u_addr;
    *(address+1) = u_addr+2;
    *(address+2) = u_addr+2;
    
    /* Getting the rest of the format string */
    puts("Please enter the format string:");
    scanf("%s",buf+12);
    size=strlen(buf+12)+4*3;
    printf("The string length is %d\n",size);

    /* Writing buf to "mystring" */
    fp=open("mystring",O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if(fp != -1){   write(fp,buf,size); close(fp);  }
    else {  printf("Open failed!\n");   }
}
void main(int argc, char * argv[])
{   read2file();    }

