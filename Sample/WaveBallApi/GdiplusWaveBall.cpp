#include "GdiplusWaveBall.h"
#include <process.h>
#include <strsafe.h>

/*****************************************************************************
*
* GET VALUE POINT: CreateOriginalBmp
*
*****************************************************************************/
Bitmap* FlyWaveBall::CreateOriginalBmp()
{
	HDC			hdc;
	REAL		nWidth, nHeight;
	Graphics	*baseGraph;
	Bitmap		*bmp;
	Graphics	*pGraphics;
	Pen			*myPen;
	SolidBrush	*blackBrush;
	REAL		strSize;
	INT			iStrLength;

	nHeight = 2 * (threadWavesMove.radius + 1);
	nWidth = 2 * (threadWavesMove.radius + 1);
	Gdiplus::RectF gdiRc(threadWavesMove.originalX,
		threadWavesMove.originalY,
		nWidth,
		nHeight);

	hdc = GetDC(threadWavesMove.hWnd);
	baseGraph = new Gdiplus::Graphics(hdc);
	bmp = new Bitmap((int)nWidth, (int)nHeight);
	for (int i = 0; i < nWidth; i++)
	{
		for (int j = 0; j < nHeight; j++)
		{
			bmp->SetPixel(i, j, threadWavesMove.colorOriginal);
		}
	}
	pGraphics = Graphics::FromImage(bmp);

	pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
	myPen = new Pen(threadWavesMove.colorWaveBall, 1);
	pGraphics->DrawEllipse(myPen, 0.0f, 0.0f, 2 * threadWavesMove.radius, 2 * threadWavesMove.radius);
	strSize = 2 * threadWavesMove.radius / 5;
	//WaveBallSetText(threadWavesMove.hWnd, threadWavesMove.colorWaveBall, AUTOGRAPH, lstrlen(AUTOGRAPH), (REAL)2 * threadWavesMove.radius / 5);
	Font myFont(TEXT("Arial"), strSize);
	iStrLength = lstrlen(AUTOGRAPH);
	blackBrush = new SolidBrush(threadWavesMove.colorWaveBall);
	pGraphics->DrawString(AUTOGRAPH, iStrLength, &myFont,
		PointF(threadWavesMove.radius - (strSize / 3)*iStrLength - 7,
			threadWavesMove.radius - 2 * strSize / 3),
		blackBrush);

	baseGraph->DrawImage(bmp, gdiRc, 0.0f, 0.0f, nWidth, nHeight, UnitPixel);

	delete(myPen);
	delete(blackBrush);
	delete(pGraphics);
	delete(baseGraph);
	DeleteDC(hdc);

	return bmp;
}
/*****************************************************************************
*
* GET VALUE POINT: WaveBallInit
*
* Function: Initialize wave ball
*
*****************************************************************************/
VOID FlyWaveBall::WaveBallInit(
	IN HWND hWnd,
	IN Gdiplus::Color &color,
	IN Gdiplus::Color &colorOriginal,
	IN REAL x,
	IN REAL y,
	IN REAL radius)
{
	memset(&threadWavesMove, 0, sizeof(ThreadParam));
	threadWavesMove.bConstantWave = FALSE;
	threadWavesMove.hWnd = hWnd;
	threadWavesMove.originalX = x;
	threadWavesMove.originalY = y;
	threadWavesMove.colorWaveBall = color;
	threadWavesMove.colorOriginal = colorOriginal;
	threadWavesMove.colorText = Color(255, 255, 235, 205);
	threadWavesMove.percentage = 0;
	threadWavesMove.radius = radius;

	amplitudeHigh = 1.0;
	amplitudeWidth = 0.08f;
	amplitudeOffset = 6.0;

	bResult = TRUE;
	lstrcpyn(strText, AUTOGRAPH, sizeof(strText) / sizeof(strText[0]));

	//bmpOriginal = CreateOriginalBmp();

}
/*****************************************************************************
*
* GET VALUE POINT: ThreadDynamicWave
*
* Function: Make the waves move
*
*****************************************************************************/
VOID FlyWaveBall::ThreadDynamicWave(int	offset)
{
	if (threadWavesMove.percentage <= 0)
	{
		WaveBallRedrawCircle();
		return;
	}
	else if (threadWavesMove.percentage >= 1)
	{
		ShowResult();
		return;
	}

	HDC			hdc;
	REAL		nWidth, nHeight;
	Graphics	*baseGraph;
	Bitmap		*bmp;
	Graphics	*pGraphics;
	Pen			*myPen;
	SolidBrush	*blackBrush;
	REAL		strSize;
	/***********************/
	float			sX, sY;			// start X/Y
	float			x, y;
	int				count;
	//int				offset;
	float			iCircularInterval = 3;
	PointF			points[MAX_POINT];
	Brush			*brush;
	Brush			*brushOriginal;
	float			centerX, centerY;
	TCHAR			strText[128];

	nHeight = 2 * (threadWavesMove.radius + 1);
	nWidth = 2 * (threadWavesMove.radius + 1);
	Gdiplus::RectF gdiRc(threadWavesMove.originalX,
		threadWavesMove.originalY,
		nWidth,
		nHeight);
	bmp = new Bitmap((int)nWidth, (int)nHeight);
	pGraphics = Graphics::FromImage(bmp);
	pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
	hdc = GetDC(threadWavesMove.hWnd);
	baseGraph = new Gdiplus::Graphics(hdc);

	// 初始化画布
	for (int i = 0; i < nWidth; i++)
	{
		for (int j = 0; j < nHeight; j++)
		{
			bmp->SetPixel(i, j, threadWavesMove.colorOriginal);
		}
	}

	strSize = 2 * threadWavesMove.radius / 5;
	Font myFont(TEXT("Arial"), strSize);
	blackBrush = new SolidBrush(threadWavesMove.colorText);
	myPen = new Pen(threadWavesMove.colorWaveBall, 1);

	sX = 0.0f;
	centerX = threadWavesMove.radius;
	centerY = threadWavesMove.radius;
	brush = new SolidBrush(threadWavesMove.colorWaveBall);
	brushOriginal = new SolidBrush(threadWavesMove.colorOriginal);
	//offset = 0;

	sY = 2 * threadWavesMove.radius*(1 - threadWavesMove.percentage);
	count = 0;
	for (x = sX; x < (int)sX + MAX_POINT; x += 1.0f) 
	{
		//此处坐标(x,y)的取点，依靠公式 “振幅高*sin(x*振幅宽 + 振幅偏移量)”

		// The point of coordinates (x, y) here depends on the formula 
		// "amplitude height * sin (x * amplitude width+amplitude offset)"
		if (threadWavesMove.percentage < 0.1)
			y = threadWavesMove.percentage*(float)sin((sX + x) * 0.08 - offset*PI / 6);
		else if (threadWavesMove.percentage > 0.9)
			y = (float)(fabs(threadWavesMove.percentage - 1)*sin((sX + x) * 0.08 - offset*PI / 6));
		else
			y = amplitudeHigh*(float)sin((sX + x) * amplitudeWidth - offset*PI / amplitudeOffset);// 4,0.02,6
		

		//TIS_Trace(TEXT("x,y:%f,%f\n"), x, sY + y * 5);
		if (IsPointInCircle(x, sY + y * 5, centerX, centerY, threadWavesMove.radius - iCircularInterval))
		{
			points[count] = Gdiplus::PointF(x, (sY + y * 5));
			count++;
		}
		if (count >= MAX_POINT || x > (centerX + threadWavesMove.radius))
			break;
	}

	// 绘制圆形
	pGraphics->DrawEllipse(myPen, 0.0f, 0.0f, 2 * threadWavesMove.radius, 2 * threadWavesMove.radius);

	if (threadWavesMove.percentage > 0.95f)
	{
		pGraphics->FillEllipse(brush,
			iCircularInterval,//x
			iCircularInterval,//y
			2 * (threadWavesMove.radius - iCircularInterval),
			2 * (threadWavesMove.radius - iCircularInterval));

	}
	else if (count > 0)
	{
		double startAngle = (atan((points[count - 1].Y - centerY) / (points[count - 1].X - centerX)) * 180 / PI);
		double endAngle = 180 + (atan((points[1].Y - centerY) / (points[1].X - centerX)) * 180 / PI);
		//TIS_Trace(TEXT("Angle:%d,%d\n"), startAngle, endAngle);
		for (; startAngle<endAngle; startAngle += 1.0f)
		{

			if (startAngle > 0 && startAngle < 90) // 4 quadrant
			{
				points[count] = Gdiplus::PointF(centerX + threadWavesMove.radius * (float)cos(startAngle* PI / 180) - iCircularInterval,
					centerY + threadWavesMove.radius * (float)sin(startAngle* PI / 180) - iCircularInterval);
			}
			else if (startAngle > 90 && startAngle < 180)// 3 quadrant
			{
				points[count] = Gdiplus::PointF(centerX + threadWavesMove.radius * (float)cos(startAngle* PI / 180) + iCircularInterval,
					centerY + threadWavesMove.radius * (float)sin(startAngle* PI / 180) - iCircularInterval);
			}
			else if (startAngle>180 && startAngle < 270)// 2 quadrant
			{
				points[count] = Gdiplus::PointF(centerX + threadWavesMove.radius * (float)cos(startAngle* PI / 180) + iCircularInterval,
					centerY + threadWavesMove.radius * (float)sin(startAngle* PI / 180) + iCircularInterval);
			}
			else if (startAngle>270 || startAngle <0)// 1 quadrant
			{
				points[count] = Gdiplus::PointF(centerX + threadWavesMove.radius * (float)cos(startAngle* PI / 180) - iCircularInterval,
					centerY + threadWavesMove.radius * (float)sin(startAngle* PI / 180) + iCircularInterval);
			}
			else if (startAngle == 0)// 0
			{
				points[count] = Gdiplus::PointF(centerX + threadWavesMove.radius * (float)cos(startAngle* PI / 180) - iCircularInterval,
					centerY + threadWavesMove.radius * (float)sin(startAngle* PI / 180));
			}
			else if (startAngle == 90) // 90
			{
				points[count] = Gdiplus::PointF(centerX + threadWavesMove.radius * (float)cos(startAngle* PI / 180),
					centerY + threadWavesMove.radius * (float)sin(startAngle* PI / 180) - iCircularInterval);
			}
			else if (startAngle == 180) //180
			{
				points[count] = Gdiplus::PointF(centerX + threadWavesMove.radius * (float)cos(startAngle* PI / 180) + iCircularInterval,
					centerY + threadWavesMove.radius * (float)sin(startAngle* PI / 180));
			}
			else //270
			{
				points[count] = Gdiplus::PointF(centerX + threadWavesMove.radius * (float)cos(startAngle* PI / 180) - iCircularInterval,
					centerY + threadWavesMove.radius * (float)sin(startAngle* PI / 180));
			}
			count++;
		}

		// Draw wave ball
		pGraphics->FillClosedCurve(brush, points, count);
	}

	// Set text.
	if (!threadWavesMove.bConstantWave)
		swprintf_s(strText, TEXT("%.1f%%"), threadWavesMove.percentage * 100);
	else
		lstrcpy(strText, AUTOGRAPH);
	// Draw String
	pGraphics->DrawString(strText, lstrlen(strText), &myFont,
		PointF(threadWavesMove.radius - (strSize / 3)*lstrlen(strText) - 7,
			threadWavesMove.radius - 2 * strSize / 3),
		blackBrush);
	// Draw memory DC to Dialog
	baseGraph->DrawImage(bmp, gdiRc, 0.0f, 0.0f, nWidth, nHeight, UnitPixel);


	delete(brushOriginal);
	delete(brush);
	delete(myPen);
	delete(blackBrush);
	delete(pGraphics);
	delete(baseGraph);
	delete(bmp);
	DeleteDC(hdc);

}
/*****************************************************************************
*
* GET VALUE POINT: IsPointInCircle
*
*****************************************************************************/
BOOL FlyWaveBall::IsPointInCircle(double x, double y, double CenterX, double CenterY, double radius)
{
	//到圆心的距离 是否大于半径。半径是R  
	//如O(x,y)点圆心，任意一点P（x1,y1） （x-x1）*(x-x1)+(y-y1)*(y-y1)>R*R 那么在圆外 反之在圆内
	double r = radius;
	double x1 = x;
	double y1 = y;



	if (!((CenterX - x1)*(CenterX - x1) + (CenterY - y1)*(CenterY - y1) > r*r))
	{
		return true;        //当前点在圆内
	}
	else
	{
		return false;       //当前点在圆外
	}
}
/*****************************************************************************
*
* GET VALUE POINT: WaveBallRedrawCircle
*
* Function: Erase fill.Redraw waveball
*
*****************************************************************************/
BOOL FlyWaveBall::WaveBallRedrawCircle()
{
	HDC			hdc;
	REAL		nWidth, nHeight;
	Graphics	*baseGraph;
	Bitmap		*bmp;
	Graphics	*pGraphics;
	Pen			*myPen;
	SolidBrush	*blackBrush;
	REAL		strSize;
	INT			iStrLength;

	nHeight = 2 * (threadWavesMove.radius + 1);
	nWidth = 2 * (threadWavesMove.radius + 1);
	Gdiplus::RectF gdiRc(threadWavesMove.originalX,
		threadWavesMove.originalY,
		nWidth,
		nHeight);

	hdc = GetDC(threadWavesMove.hWnd);
	baseGraph = new Gdiplus::Graphics(hdc);
	bmp = new Bitmap((int)nWidth, (int)nHeight);
	for (int i = 0; i < nWidth; i++)
	{
		for (int j = 0; j < nHeight; j++)
		{
			bmp->SetPixel(i, j, threadWavesMove.colorOriginal);
		}
	}
	pGraphics = Graphics::FromImage(bmp);

	pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
	myPen = new Pen(threadWavesMove.colorWaveBall, 1);
	pGraphics->DrawEllipse(myPen, 0.0f, 0.0f, 2 * threadWavesMove.radius, 2 * threadWavesMove.radius);
	strSize = 2 * threadWavesMove.radius / 5;
	//WaveBallSetText(threadWavesMove.hWnd, threadWavesMove.colorWaveBall, AUTOGRAPH, lstrlen(AUTOGRAPH), (REAL)2 * threadWavesMove.radius / 5);
	Font myFont(TEXT("Arial"), strSize);
	iStrLength = lstrlen(strText);
	blackBrush = new SolidBrush(threadWavesMove.colorWaveBall);
	pGraphics->DrawString(strText, iStrLength, &myFont,
		PointF(threadWavesMove.radius - (strSize / 3)*iStrLength - 7,
			threadWavesMove.radius - 2 * strSize / 3),
		blackBrush);

	baseGraph->DrawImage(bmp, gdiRc, 0.0f, 0.0f, nWidth, nHeight, UnitPixel);

	delete(myPen);
	delete(blackBrush);
	delete(pGraphics);
	delete(bmp);
	delete(baseGraph);
	DeleteDC(hdc);

	return TRUE;
}
/*****************************************************************************
*
* GET VALUE POINT: WaveBallSetText
*
* Function: Set wave ball text
*
*****************************************************************************/
VOID FlyWaveBall::WaveBallUpdateProgress(IN REAL	percentage)
{
	if (percentage>1)
		threadWavesMove.percentage = 1;
	else if (percentage<0)
		threadWavesMove.percentage = 0;
	else
		threadWavesMove.percentage = percentage;
}
/*****************************************************************************
*
* GET VALUE POINT: WaveBallSetSpeed
*
* Function: Set wave ball speed. Milliseconds.
*
*****************************************************************************/
VOID FlyWaveBall::WaveBallSetConstantWave(IN REAL percentage)
{
	threadWavesMove.bConstantWave = TRUE;
	threadWavesMove.percentage = percentage;
}
/*****************************************************************************
*
* GET VALUE POINT: WaveBallSetTextColor
*
* Function: Set wave ball text color.
*
*****************************************************************************/
VOID FlyWaveBall::WaveBallSetTextColor(IN Color	&color)
{
	threadWavesMove.colorText = color;
}
/*****************************************************************************
*
* GET VALUE POINT: CreateOriginalBmp
*
*****************************************************************************/
VOID FlyWaveBall::ShowResult()
{
	HDC			hdc;
	REAL		nWidth, nHeight;
	Graphics	*baseGraph;
	Bitmap		*bmp;
	Graphics	*pGraphics;
	Pen			*myPen;
	int			iOffset = 8;

	nHeight = 2 * (threadWavesMove.radius + 1);
	nWidth = 2 * (threadWavesMove.radius + 1);
	Gdiplus::RectF gdiRc(threadWavesMove.originalX,
		threadWavesMove.originalY,
		nWidth,
		nHeight);

	hdc = GetDC(threadWavesMove.hWnd);
	baseGraph = new Gdiplus::Graphics(hdc);
	bmp = new Bitmap((int)nWidth, (int)nHeight);
	for (int i = 0; i < nWidth; i++)
	{
		for (int j = 0; j < nHeight; j++)
		{
			bmp->SetPixel(i, j, threadWavesMove.colorOriginal);
		}
	}
	pGraphics = Graphics::FromImage(bmp);

	pGraphics->SetSmoothingMode(SmoothingModeHighQuality);
	

	if (bResult)
	{// PASS
		myPen = new Pen(Color(255, 0, 255, 0), 2* (float)iOffset);
		pGraphics->DrawLine(myPen, 
			Point(iOffset, (int)nHeight/2 + iOffset),
			Point((int)nWidth/2, (int)nHeight - iOffset)
		);
		pGraphics->DrawLine(myPen,
			Point((int)nWidth - iOffset,5 * iOffset),
			Point((int)nWidth / 2 -(iOffset+1), (int)nHeight -(iOffset+1))
		);
	}
	else
	{// FAIL
		myPen = new Pen(Color(255, 255, 0, 0), 2 * (float)iOffset);
		pGraphics->DrawLine(myPen,
			Point(0, 0),
			Point((int)nWidth, (int)nHeight)
		);
		pGraphics->DrawLine(myPen,
			Point((int)nWidth, 0),
			Point(0, (int)nHeight)
		);
	}

	baseGraph->DrawImage(bmp, gdiRc, 0.0f, 0.0f, nWidth, nHeight, UnitPixel);

	delete(myPen);
	delete(pGraphics);
	delete(bmp);
	delete(baseGraph);
	DeleteDC(hdc);

}
/*****************************************************************************
*
* ENTRY POINT: WaveBallSetText
*
*****************************************************************************/
VOID FlyWaveBall::WaveBallSetText(TCHAR * string)
{
	lstrcpyn(strText, string, sizeof(strText) / sizeof(strText[0]));
}
/*****************************************************************************
*
* ENTRY POINT: WaveBallSetText
*
*****************************************************************************/
VOID FlyWaveBall::WaveBallSetResult(BOOL result)
{
	bResult = result;
}
/*****************************************************************************
*
* ENTRY POINT: WaveBallSetColor
*
*****************************************************************************/
VOID FlyWaveBall::WaveBallSetColor(IN Color	&color)
{
	threadWavesMove.colorWaveBall = color;
}

/*****************************************************************************
*
* ENTRY POINT: WaveBallSetText
*
*****************************************************************************/
VOID FlyWaveBall::WaveBallSetAmplitude(float High, float Width, float Offset)
{
	if (High > 0)
		amplitudeHigh = High;
	if (Width > 0)
		amplitudeWidth = Width;

	amplitudeOffset = Offset;
}