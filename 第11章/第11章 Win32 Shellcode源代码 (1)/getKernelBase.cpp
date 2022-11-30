//  GetKernelBase.cpp : 
#include <stdio.h>
#include <stdlib.h>

unsigned long GetKernel32Addr()
{
    unsigned long pAddress;
    __asm{
        mov eax, fs:30h     ; PEB base
        mov eax, [eax+0ch]  ; PEB_LER_DATA
        // base of ntdll.dll=====================
        mov ebx, [eax+1ch]      ; The first element 
        // base of kernel32.dll=====================
        mov ebx,[ebx]           ; Next element
        mov eax,[ebx+8]         ; Base address of second module
        mov pAddress,eax        ; Save it to local variable
    };
    printf("Base address of kernel32.dll is %p\n", pAddress);
    return pAddress;
}
void main(void)
{
    GetKernel32Addr();
}