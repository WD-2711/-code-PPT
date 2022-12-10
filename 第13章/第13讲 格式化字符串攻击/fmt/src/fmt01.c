#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//  gcc -o fmt ../src/fmt01.c

void vul_example()  {
    char user_input[1024];
    scanf("%s", user_input);	//  用户可以输入任意字符串(可含格式字符%：如%x、%s、%n)
    printf("%s\n", user_input);	//  按字符串格式输出，无漏洞
    printf(user_input);		    //  直接输出用户的输入，有漏洞
}

void no_formatstr_vul()
{
    unsigned int A=0x123,B=0x456,C=0x789;
    printf("\tA is 0x%x and is at %08x, B is 0x%x and is at %08x.\n",A,&A,B,&B);
}

void formatstr_vul()
{
    char user_input[1024];
    int A=0x123,B=0x456,C=0x789;
    puts("Please enter a string:");
    scanf("%s", user_input);
    printf(user_input);
    puts("\n");
}

void main(int argc, char * argv[])
{   
    if (argc == 1){
        puts("vul_example(): input a word from console.");
        vul_example();
    } else if (argc == 2){
        no_formatstr_vul();
    } else if (argc == 3){
        formatstr_vul();
    }
}

