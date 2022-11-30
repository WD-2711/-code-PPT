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
    ExitProcess(ret);  // 用这个较稳妥，否则shellcode会出错

/*    //释放句柄，否则新进程将无法退出
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
