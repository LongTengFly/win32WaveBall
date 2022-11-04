#ifndef _FLY_GDIPLUS_WAVE_BALL_H_
#define _FLY_GDIPLUS_WAVE_BALL_H_

#include <windows.h>
// GDI+
#include <gdiplus.h> 
#pragma comment (lib, "GdiPlus.lib")
using namespace Gdiplus;



class FlyWaveBall
{
public:
	//FlyWaveBall();
	//~FlyWaveBall();
	VOID WaveBallInit(
		IN HWND hWnd,
		IN Gdiplus::Color &color,
		IN Gdiplus::Color &colorOriginal,
		IN REAL x,
		IN REAL y,
		IN REAL radius);
	VOID ThreadDynamicWave(int offset);
	VOID WaveBallUpdateProgress(IN REAL	percentage);
	VOID WaveBallSetConstantWave(IN REAL percentage);
	VOID WaveBallSetTextColor(IN Color	&color);
	VOID WaveBallSetColor(IN Color	&color);
	VOID WaveBallSetText(TCHAR * string);
	VOID WaveBallSetResult(BOOL result);
	VOID WaveBallSetAmplitude(float High, float Width, float amplitudeOffset);
private:
#define PI			3.141592654
#define MAX_POINT	1024
#define AUTOGRAPH	TEXT("FLY.")
	typedef struct ThreadParameters
	{
		HWND	hWnd;
		REAL	originalX;
		REAL	originalY;
		REAL	radius;
		REAL	percentage;
		Color	colorWaveBall;
		Color	colorOriginal;
		Color	colorText;
		BOOL	bConstantWave;
	}ThreadParam;

	Bitmap* CreateOriginalBmp();
	BOOL WaveBallRedrawCircle();
	VOID ShowResult();
	BOOL IsPointInCircle(
		double x, 
		double y, 
		double CenterX, 
		double CenterY, 
		double radius);

	ThreadParam threadWavesMove;
	BOOL	bResult;
	TCHAR	strText[16];

	float amplitudeHigh;

	float amplitudeWidth;

	float amplitudeOffset;
};

#endif
