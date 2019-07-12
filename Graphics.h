#pragma once
#include "framework.h"
#include <d2d1.h>
#include <dwrite.h>
#include "util.h"

class Graphics
{
public:
	Graphics();
	~Graphics();
	HRESULT Initial(HWND hwnd);
	HRESULT SetTextFormat(int fontSize, DWRITE_PARAGRAPH_ALIGNMENT align = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	void BeginDraw();
	void EndDraw();
	void Resize(int width, int height);
	void ClearScreen(COLORREF color);
	void DrawCellText(RECT rect, COLORREF color, int num);
	void DrawLogo(RECT rect);
	void DrawScore(RECT, LPCTSTR title, int score);
	void drawRoundRect(RECT rect, COLORREF color);
private:
	ID2D1Factory* m_pFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pBrush;
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
};
