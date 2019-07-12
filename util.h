#pragma once
#include "framework.h"
#include <d2d1.h>

template <class T>
void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

D2D1_COLOR_F GetColorF(COLORREF color);
D2D1_RECT_F GetRectF(RECT rect);

int getRectHeight(RECT rect);
int getRectWidth(RECT rect);

void saveTopScore(int i);
int readTopScore();

void getCellColour(int num, LPCOLORREF bgColor, LPCOLORREF fontColor);