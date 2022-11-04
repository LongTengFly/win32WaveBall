# win32WaveBall
Wave ball effect for win32 or C++projects

(C++/Win32波浪球效果)
# 简介
这是一个基于C++在win32上实现的波浪球效果。它使用gid+进行图形绘制，支持指定签名、动态百分比显示以及特殊项目的PASS/FAIL显示。

对其封装成了一个类，并开放了相关参数给用户设置，可以灵活使用。

部分效果图小如下：

![wave_ball_show](https://github.com/LongTengFly/win32WaveBall/blob/main/GIF%202022-11-4%2014-58-12.gif)
## 使用
只需要包含了头文件GdiplusWaveBall.h和添加gdi+库即可使用该类；详情可以参考Sample。

```cpp
// GDI+
#include <gdiplus.h> 
#pragma comment (lib, "GdiPlus.lib")
using namespace Gdiplus;

// Wave ball
#include "./WaveBallApi/GdiplusWaveBall.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{ 

	//Initialize GDI+.
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// other code

	//Unload GDI+
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return 0; 
} 

```
