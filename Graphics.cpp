#include "Graphics.h"

Graphics::Graphics() :
	m_pFactory{ NULL },
	m_pRenderTarget{ NULL },
	m_pBrush{ NULL },
	m_pDWriteFactory{ NULL },
	m_pTextFormat{ NULL }
{
}

Graphics::~Graphics()
{
	SafeRelease(&m_pFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pBrush);
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pTextFormat);
}

HRESULT Graphics::Initial(HWND hwnd)
{
	HRESULT hr = S_FALSE;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
	if (SUCCEEDED(hr))
	{
		RECT rc;
		GetClientRect(hwnd, &rc);
		D2D1_SIZE_U size{ UINT32(rc.right - rc.left),UINT32(rc.bottom - rc.top) };

		hr = m_pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hwnd, size),
			&m_pRenderTarget);
		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
			hr = m_pRenderTarget->CreateSolidColorBrush(color, &m_pBrush);
		}
	}
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown * *> (&m_pDWriteFactory));
	}
	return hr;
}

HRESULT Graphics::SetTextFormat(int fontSize, DWRITE_PARAGRAPH_ALIGNMENT  align)
{
	HRESULT hr = m_pDWriteFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(float)fontSize,
		L"",
		&m_pTextFormat
	);
	if (SUCCEEDED(hr))
	{
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pTextFormat->SetParagraphAlignment(align);
	}
	return hr;
}

void Graphics::BeginDraw()
{
	m_pRenderTarget->BeginDraw();
}

void Graphics::EndDraw()
{
	m_pRenderTarget->EndDraw();
}

void Graphics::Resize(int widht, int height)
{
	D2D1_SIZE_U sz = D2D1::SizeU(widht, height);
	m_pRenderTarget->Resize(sz);
}

void Graphics::ClearScreen(COLORREF color)
{
	m_pRenderTarget->Clear(GetColorF(color));
}

void Graphics::DrawCellText(RECT rect, COLORREF color, int num)
{
	TCHAR buffer[256];
	wsprintf(buffer, L"%d", num);

	int i = (int)log2(num);
	double factor = i < 7 ? 1 : (i < 10 ? 1.5 : 2);
	int height = (int)(getRectHeight(rect) * 0.66 / factor);

	HRESULT hr = this->SetTextFormat(height);
	if (SUCCEEDED(hr))
	{
		m_pBrush->SetColor(GetColorF(color));
		m_pRenderTarget->DrawTextW(buffer,
			(UINT32)_tcslen(buffer), m_pTextFormat, GetRectF(rect), m_pBrush);
	}
}

void Graphics::DrawLogo(RECT rect)
{
	int height = (int)(getRectHeight(rect));
	HRESULT hr = this->SetTextFormat(height);
	if (SUCCEEDED(hr))
	{
		m_pBrush->SetColor(GetColorF(RGB(119, 110, 101)));
		m_pRenderTarget->DrawTextW(L"2048", (UINT32)4, m_pTextFormat, GetRectF(rect), m_pBrush);
	}
}

void Graphics::DrawScore(RECT rect, LPCTSTR title, int score)
{
	RECT rc1{ rect.left,rect.top,rect.right,rect.bottom - getRectHeight(rect) / 2 };
	RECT rc2{ rect.left,rect.top + getRectHeight(rect) / 2,rect.right,rect.bottom };
	drawRoundRect(rect, RGB(187, 173, 160));

	HRESULT hr = this->SetTextFormat((int)(getRectHeight(rc1) * 0.45), DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	if (SUCCEEDED(hr))
	{
		m_pBrush->SetColor(GetColorF(RGB(238, 228, 218)));
		m_pRenderTarget->DrawTextW(title, (UINT32)_tcslen(title), m_pTextFormat, GetRectF(rc1), m_pBrush);
	}

	hr = this->SetTextFormat((int)(getRectHeight(rc2) * 0.8));
	if (SUCCEEDED(hr))
	{
		m_pBrush->SetColor(GetColorF(RGB(255, 255, 255)));
		TCHAR buffer[256];
		wsprintf(buffer, L"%d", score);
		m_pRenderTarget->DrawTextW(buffer, (UINT32)_tcslen(buffer), m_pTextFormat, GetRectF(rc2), m_pBrush);
	}
}

void Graphics::drawRoundRect(RECT rect, COLORREF color)
{
	m_pBrush->SetColor(GetColorF(color));

	D2D1_ROUNDED_RECT roundRect = D2D1::RoundedRect(GetRectF(rect), 4.0f, 4.0f);

	m_pRenderTarget->FillRoundedRectangle(roundRect, m_pBrush);
}