#include "stdio.h"
#include "stdlib.h"
#include "windows.h"

void doCommandLine(char * szCmdLine)
{
    BOOL ret;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    ZeroMemory( &pi, sizeof(pi) );

    si.cb = sizeof(si);
    si.wShowWindow=TRUE;
    si.dwFlags=STARTF_USESHOWWINDOW;

    ret=CreateProcessA(
        NULL,
        szCmdLine,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
        );
    ExitProcess(ret);  // ����������ף�����shellcode�����

/*    //�ͷž���������½��̽��޷��˳�
    if(ret)
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
*/
}
void main(int argc, char* argv[])
{
    doCommandLine("notepad.exe");
}
