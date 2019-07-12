// 2048Game.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "2048Game.h"
#include "Game.h"
#include "Graphics.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

Game* pGame;
Graphics* pGraphic;
HWND hWnd;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Info(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY2048GAME, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2048GAME));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete pGraphic;
	delete pGame;

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2048GAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY2048GAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MY2048GAME));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 450, 600, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	pGraphic = new Graphics();
	if (!SUCCEEDED(pGraphic->Initial(hWnd)))
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		pGame = new Game();
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT rect;
		GetClientRect(hWnd, &rect);

		pGraphic->BeginDraw();
		pGame->SetRect(rect);
		pGame->SetGraphics(pGraphic);
		pGame->Draw2D();
		pGraphic->EndDraw();

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_SIZE:
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		pGraphic->Resize(width, height);
	}
	break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
		case 'W':
			if (pGame->MoveUp())
			{
				InvalidateRect(hWnd, &pGame->GetChessRect(), true);
				InvalidateRect(hWnd, &pGame->GetTopRect(), false);
			}
			break;
		case VK_DOWN:
		case 'S':
			if (pGame->MoveDown())
			{
				InvalidateRect(hWnd, &pGame->GetChessRect(), true);
				InvalidateRect(hWnd, &pGame->GetTopRect(), false);
			}
			break;
		case VK_LEFT:
		case 'A':
			if (pGame->MoveLeft())
			{
				InvalidateRect(hWnd, &pGame->GetChessRect(), true);
				InvalidateRect(hWnd, &pGame->GetTopRect(), false);
			}
			break;
		case VK_RIGHT:
		case 'D':
			if (pGame->MoveRight())
			{
				InvalidateRect(hWnd, &pGame->GetChessRect(), true);
				InvalidateRect(hWnd, &pGame->GetTopRect(), false);
			}
			break;
		}
		if (pGame->IsTerminated())
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MY2048_DIALOG), hWnd, Info);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK Info(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND stDisplay = GetDlgItem(hDlg, IDC_STATIC);

		wchar_t buffer[256];
		if (pGame->IsWon())
			wsprintfW(buffer, L"You win!!!\nYour score: %d", pGame->GetScore());
		else
			wsprintfW(buffer, L"You lose :(\nYour score: %d", pGame->GetScore());

		SendMessage(stDisplay, WM_SETTEXT, NULL, (WPARAM)buffer);
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDRETRY)
		{
			HWND hParentWnd = GetParent(hDlg);

			pGame->Restart();

			InvalidateRect(hWnd, &pGame->GetChessRect(), TRUE);
			EndDialog(hDlg, LOWORD(wParam)); // Close dialog
		}
		else if (LOWORD(wParam) == IDNO)
		{
			HWND hParentWnd = GetParent(hDlg);
			EndDialog(hDlg, LOWORD(wParam));
			DestroyWindow(hParentWnd); // Close main window, exit
		}
		break;
	}
	return 0;
}