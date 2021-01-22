// Lab 5.1.cpp : Определяет точку входа для приложения.
//

#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK newWndProc(HWND, UINT, WPARAM, LPARAM);


void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap);
HWND Button;
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR szClassName[] = L"Мой класс";
	MSG msg;
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;
	wc.cbWndExtra = NULL;
	wc.cbClsExtra = NULL;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hInstance = hInst;
	if (!RegisterClassEx(&wc)) {

		MessageBox(NULL, L"Не получилось зарегистрировать класс!", L"Ошибка", MB_OK);
		return NULL;
	}
	HWND hWnd = CreateWindowEx(0,
		szClassName,
		_T("Лабораторная работа №5"),
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		NULL,
		hInst,
		NULL);
	if (!hWnd)
	{
		MessageBox(NULL, TEXT("Не получилось создать окно!"), TEXT("Ошибка"), MB_OK);
		return NULL;
	}
	Button = CreateWindow(TEXT("Button"), TEXT("Press me"), WS_CHILD | WS_VISIBLE, 500, 300, 130, 130, hWnd, reinterpret_cast<HMENU>(1), hInst, NULL);
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}





LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;
	HBRUSH hBrush;
	switch (uMsg)
	{
	case  WM_CREATE:
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		RedrawWindow(hWnd, NULL, NULL, RDW_ERASENOW);
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		hBrush = CreateSolidBrush(RGB(150, 150, 150));
		SelectObject(hDC, hBrush);
		FillRect(hDC, &rect, hBrush);
		ReleaseDC(hWnd, hDC);
		DeleteObject(hBrush);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:

		if (lParam == reinterpret_cast<LPARAM>(Button))
		{
			InvalidateRect(hWnd, NULL, TRUE);
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&newWndProc));
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


LRESULT CALLBACK newWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;
	HBITMAP hBit = static_cast<HBITMAP>(LoadImage(NULL, TEXT("hBitMap.bmp"), NULL, 500, 500, LR_LOADFROMFILE));

	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		RedrawWindow(hWnd, NULL, NULL, RDW_ERASENOW);
		hDC = BeginPaint(hWnd, &ps);
		DrawBitmap(hDC, 300, 300, hBit);
		EndPaint(hWnd, &ps);
		break;
	case WM_COMMAND:
		if (lParam == reinterpret_cast<LPARAM>(Button))
		{
			InvalidateRect(hWnd, NULL, TRUE);
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc));
			break;
		}
	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBitmap)
{
	HBITMAP hbm, hOldbm;
	HDC hMemDC;
	BITMAP bm;
	POINT  ptSize, ptOrg;
	hMemDC = CreateCompatibleDC(hDC);

	hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmap);

	if (hOldbm)
	{
		SetMapMode(hMemDC, GetMapMode(hDC));
		GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);

		ptSize.x = bm.bmWidth;
		ptSize.y = bm.bmHeight;

		DPtoLP(hDC, &ptSize, 1);

		ptOrg.x = 0;
		ptOrg.y = 0;
		DPtoLP(hMemDC, &ptOrg, 1);


		BitBlt(hDC, x, y, ptSize.x, ptSize.y,
			hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);


		SelectObject(hMemDC, hOldbm);
	}
	DeleteDC(hMemDC);
}