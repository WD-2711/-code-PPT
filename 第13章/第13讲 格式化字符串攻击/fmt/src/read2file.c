/* read2file.c */
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
    
    /* Getting the rest of the format string */
    puts("Please enter the format string:");
    scanf("%s",buf+4);
    size=strlen(buf+4)+4;
    printf("The string length is %d\n",size);

    /* Writing buf to "mystring" */
    fp=open("mystring",O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if(fp != -1){   write(fp,buf,size); close(fp);  }
    else {  printf("Open failed!\n");   }
}
void main(int argc, char * argv[])
{   read2file();    }

