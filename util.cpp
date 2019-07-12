#include "util.h"
#include <cmath>

D2D1_COLOR_F GetColorF(COLORREF color)
{
	float r = ((float)GetRValue(color)) / 255;
	float g = ((float)GetGValue(color)) / 255;
	float b = ((float)GetBValue(color)) / 255;
	return D2D1::ColorF(r, g, b);
}

D2D1_RECT_F GetRectF(RECT rect)
{
	return D2D1::RectF((float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom);
}

int getRectHeight(RECT rect)
{
	return rect.bottom - rect.top;
}

int getRectWidth(RECT rect)
{
	return  rect.right - rect.left;
}

void saveTopScore(int score)
{
	HANDLE hFile;
	hFile = CreateFile(L"config.2048", GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		if (GetLastError() == 2)//ERROR_FILE_NOT_FOUND
		{
			hFile = CreateFile(L"config.2048", GENERIC_READ | GENERIC_WRITE, 0, NULL,
				CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		}
	}
	WriteFile(hFile, &score, (DWORD)sizeof(score), NULL, NULL);
	CloseHandle(hFile);
}

int readTopScore()
{
	HANDLE hFile;
	int score = 0;
	hFile = CreateFile(L"config.2048", GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		if (!ReadFile(hFile, &score, (DWORD)sizeof(int), NULL, NULL))
		{
			score = 0;
		}
	}
	CloseHandle(hFile);
	return score;
}

void getCellColour(int num, LPCOLORREF pCellBgColor, LPCOLORREF pFontColor)
{
	switch (num)
	{
	case 2:
		*pFontColor = RGB(119, 110, 101);
		*pCellBgColor = RGB(238, 228, 218);
		break;
	case 4:
		*pFontColor = RGB(119, 110, 101);
		*pCellBgColor = RGB(237, 224, 200);
		break;
	case 8:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(242, 177, 121);
		break;
	case 16:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(245, 149, 99);
		break;
	case 32:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(246, 124, 95);
		break;
	case 64:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(246, 94, 59);
		break;
	case 128:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(237, 207, 114);
		break;
	case 256:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(237, 204, 97);
		break;
	case 512:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(237, 200, 80);
		break;
	case 1024:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(237, 197, 63);
		break;
	case 2048:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(237, 194, 46);
		break;
	default:
		*pCellBgColor = RGB(205, 193, 180);
		break;
	}
}