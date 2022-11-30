// shell_asm.c  gcc -o shell_asm ../src/shell_asm.c
void foo()
{
   __asm__(
        "mov    $0x0,%edx   ;"  // $edx=0=NULL
        "push   %edx        ;"  // NULL
        "push   $0x0068732f ;"  // "/sh"
        "push   $0x6e69622f ;"  // "/bin"
        "mov    %esp,%ebx   ;"  // $ebx = name[0]
        "push   %edx        ;"  // name[1]=NULL
        "push   %ebx        ;"  // name[0]=$ebx;    $esp=name
        "mov    %esp,%ecx   ;"  // $ecx=$esp=name
        "mov    $0xb,%eax   ;"  // $eax = 0xb = 11
        "int    $0x80       ;"  //"sysenter   ;"
    );
}

int main(int argc, char * argv[])
{
    foo();
    return 0;
}

