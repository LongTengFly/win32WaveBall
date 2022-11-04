#include <windows.h> 
#include <strsafe.h>
#include "UI_MakeModel.h"
#include "Resource\resource.h"

#define MALLOC(bytes) (LPBYTE)VirtualAlloc(NULL, bytes, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE)
#define FREE(address) if ((address) != NULL) { VirtualFree(address, 0, MEM_RELEASE); address = NULL; }
#define ARRAY_COUNT(a) (sizeof(a) / sizeof(a[0]))
#define MBSTOWCS(wcs,mbs) MultiByteToWideChar(CP_ACP, 0, mbs, -1, wcs, ARRAY_COUNT(wcs))
#define WCSTOMBS(mbs,wcs) WideCharToMultiByte(CP_ACP, 0, wcs, -1, mbs, ARRAY_COUNT(mbs), NULL, NULL)
#define MBSTOWCS_s(wcs,mbs,max_wcs) MultiByteToWideChar(CP_ACP, 0, mbs, -1, wcs, max_wcs)
#define WCSTOMBS_s(mbs,wcs,max_mbs) WideCharToMultiByte(CP_ACP, 0, wcs, -1, mbs, max_mbs, NULL, NULL)



extern VOID TIS_Trace(LPTSTR format, ...);
