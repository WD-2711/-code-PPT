//  findFuncAddr.cpp : 
#include <stdio.h>
#include <stdlib.h>
#include "windows.h"

unsigned long GetHash(char * c)
{
    unsigned long h=0;
    while(*c)
    {
        h = ( ( h << 25 ) | ( h >> 7 ) ) + *(c++);
    }
    return h;
}

unsigned long findFuncAddr(unsigned long lHash)
// lHash: hash of the function name.
{
    unsigned long lHashFunAddr;

    __asm{
        push    lHash;      // load lHash to edx
        pop     edx;
        //  call some functions to do the job.
        call    get_base_address;
        cmp     eax,0   ;   // the base address is 0, done.
        jle     end_of_findFuncAddr; if ecx <=0 done.
        mov     ebx,eax ;   // save the base to ebx;
        call    get_function_addr;
        jmp     end_of_findFuncAddr;    // finish all job. 
   
//  Define some sub processes here.  ===========================================
    // begin of get_base_address    ==================================
    // get_base_address: put the DLL nIndex to ecx. eax=return value
    get_base_address:
        mov     eax, fs:30h     ; PEB base
        mov     eax, [eax+0ch]  ; PEB_LER_DATA
        // base of first element
        mov     eax,[eax+1ch]  ; The first element of InInitOrderModuleList
        mov     eax,[eax]       ; Next element
        mov     eax,[eax+8]     ; eax = Base address of the module
        retn;
    // end of get_base_address  ======================================

    // begin of get_function_addr    =================================
    // get_function_addr, in: ebx=base, edx=hash(name); out:eax=return value
    get_function_addr:
        // get the addrs of first function =========
        mov     eax,[ebx+3ch]       ; e_lfanew
        mov     eax,[eax+ebx+78h]   ; DataDirectory[0]
        add     eax,ebx             ; RVA + base
        mov     esi,eax             ; Save first DataDirectory to esi
        // get fields of IMAGE_EXPORT_DIRECTORY pNameOfModule
        //mov     eax,[esi+0ch]       ; Name RVA, real address should "add eax,ebx"
        //mov     eax,[esi+14h]       ; NumberOfFunctions
        //mov     eax,[esi+18h]       ; NumberOfNames
        //mov     eax,[esi+1ch]       ; AddressOfFunctions RVA
        //mov     eax,[esi+20h]       ; AddressOfNames RVA
        //mov     eax,[esi+24h]       ; AddressOfNameOrdinals RVA
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
        //cmp     ebx, [edi]                      ; compare to hash
        //jnz     short find_start
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
    // end of get_function_addr ======================================

    // begin of hash_process    ======================================
    //hash_proc: you should put the address of the string to edi
    //when ret, the hash value stores in eax
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
    // end of hash_process    ========================================
//==============================================================================
end_of_findFuncAddr:
        mov lHashFunAddr,eax;
    };

    //printf("Hash=0x%.8x  addr=0x%p\n",lHash, lHashFunAddr);

    return lHashFunAddr;
}
void main(void)
{
    printf("LoadLibraryA\t=%p\tfindHashaddr: %p\n",
        LoadLibraryA, findFuncAddr(GetHash("LoadLibraryA")));
    printf("CreateProcessA\t=%p\tfindHashaddr: %p\n",
        CreateProcessA,findFuncAddr(GetHash("CreateProcessA"))); 
}
