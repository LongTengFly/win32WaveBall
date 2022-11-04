#include <windows.h> 
#include <commctrl.h>
#include "Tool.h"
#pragma comment (lib,"comctl32.lib")

// GDI+
#include <gdiplus.h> 
#pragma comment (lib, "GdiPlus.lib")
using namespace Gdiplus;

// Wave ball
#include "./WaveBallApi/GdiplusWaveBall.h"

#define COLOR_RGB_DLG_BG	RGB(115, 141, 141)
// Wave ball color
#define COLOR_GDI_WAVEBALL	Color(255, 249, 205, 173)
#define COLOR_GDI_DLG_BG	Color(255, 115, 141, 141)

#define TIMER_ID_WAVEBALL	1
#define TIMER_TIME_SEC		1000
#define TIMER_TIME_300MS	300

static FlyWaveBall mWaveBall;


INT_PTR CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM); 
static VOID CALLBACK CallBackTimer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);

/*****************************************************************************
*
* ENTRY POINT: WinMain
*
*****************************************************************************/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{ 

	//Initialize GDI+.
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	InitCommonControls();
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)MyDlgProc); 

	//Unload GDI+
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return 0; 
} 




/*****************************************************************************
*
* ENTRY POINT: MyDlgProc
*
*****************************************************************************/
INT_PTR CALLBACK MyDlgProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) 
{ 
	static HBRUSH	hBsh;

	switch (msg) { 
    case WM_INITDIALOG:
        SetWindowText(hWnd, MODELNAME);
		// Set dialog background
		hBsh = CreateSolidBrush(COLOR_RGB_DLG_BG);

		// wave ball
		mWaveBall.WaveBallInit(hWnd, COLOR_GDI_WAVEBALL, COLOR_GDI_DLG_BG, 70, 38, 160);
		mWaveBall.WaveBallSetText(L"FLY.");
		mWaveBall.WaveBallSetConstantWave(0.3f);
		mWaveBall.WaveBallSetAmplitude(4.0, 0.02f, 6.0);

		SetTimer(hWnd, TIMER_ID_WAVEBALL, TIMER_TIME_300MS, (TIMERPROC)CallBackTimer);

		

        break;
	case WM_COMMAND: 
		switch (LOWORD(wp)) { 
		case IDOK: 
			TIS_Trace(TEXT("IDOK")); 
			break;
		case IDCANCEL: 
			EndDialog(hWnd, 0); 
			return TRUE; 
		} 
	case WM_CTLCOLORDLG:
		// Returns a brush to create a dialog background color  
		return (INT_PTR)hBsh;
		break;
	} 
	return FALSE; 
} 

/******************************************************************************
*
* ENTRY POINT: CallBackTimer
*
* Function: Timer call back
*
******************************************************************************/
static VOID CALLBACK CallBackTimer(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	static int iWaveBallOffset = 0;
	switch (idEvent)
	{
	case TIMER_ID_WAVEBALL:
		mWaveBall.ThreadDynamicWave(iWaveBallOffset);
		if (++iWaveBallOffset > 12)
			iWaveBallOffset = 0;
		break;
	default:
		break;
	}
}
