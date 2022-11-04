#include <windows.h> 
#include "Tool.h"

/*****************************************************************************
*
* ENTRY POINT: TIS_Trace
*
*****************************************************************************/
VOID TIS_Trace(LPTSTR format, ...)
{
    int i;
    TCHAR Buffer[1024];
    va_list arglist;
    va_start(arglist, format);
    lstrcpy(Buffer, TEXT("[UNI] "));
    i = lstrlen(Buffer);
    StringCchVPrintf(&Buffer[i], 1024, format, arglist);
    va_end(arglist);
    OutputDebugString(Buffer);
}

