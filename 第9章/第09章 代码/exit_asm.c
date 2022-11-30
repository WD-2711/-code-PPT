//exit_asm.c   gcc -o exit_asm exit_asm.c

void main()
{
    __asm__(
        "mov    $0xfc,%eax;"
        "mov    $0x12,%ebx;"
        "sysenter;" 
        //"int  $0x80;"
    );
}

