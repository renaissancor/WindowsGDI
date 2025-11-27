#include "stdafx.h"
#include "Window.h"

// Window.cpp 

bool Window::Initialize() noexcept {
	_hInstance = GetModuleHandle(nullptr);
	_wcex.cbSize = sizeof(WNDCLASSEX);
	_wcex.style = CS_HREDRAW | CS_VREDRAW;
	_wcex.lpfnWndProc = Window::WndProc;
	_wcex.cbClsExtra = 0;
	_wcex.cbWndExtra = 0;
	_wcex.hInstance = _hInstance;
	_wcex.hIcon = LoadIcon(_hInstance, IDI_APPLICATION);
	_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	_wcex.lpszMenuName = nullptr;
	_wcex.lpszClassName = L"App1WindowClass";
	_wcex.hIconSm = LoadIcon(_hInstance, IDI_APPLICATION);
	if (!RegisterClassEx(&_wcex)) return false;

	_hWindow = CreateWindowW(
		L"App1WindowClass",
		L"App1 Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		GRID_WIDTH * GRID_SIZE + 16,
		GRID_HEIGHT * GRID_SIZE + 39,
		nullptr,
		nullptr,
		_hInstance,
		nullptr
	);
	if (!_hWindow) return false;

	_hMainDC = GetDC(_hWindow);
	_hBackDC = CreateCompatibleDC(_hMainDC);

	RECT clientRect;
	GetClientRect(_hWindow, &clientRect);
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;
	_hBackBitmap = CreateCompatibleBitmap(_hMainDC, width, height);
	SelectObject(_hBackDC, _hBackBitmap);

	CreateGDIObjects(); 
	ShowWindow(_hWindow, SW_SHOW); 

	return true;
}

void Window::CreateGDIObjects() noexcept {
	_pens[static_cast<size_t>(PEN_TYPE::BLACK)] = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	_pens[static_cast<size_t>(PEN_TYPE::WHITE)] = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	_pens[static_cast<size_t>(PEN_TYPE::GRAY)] = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	_pens[static_cast<size_t>(PEN_TYPE::RED)] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	_pens[static_cast<size_t>(PEN_TYPE::GREEN)] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	_pens[static_cast<size_t>(PEN_TYPE::BLUE)] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	_pens[static_cast<size_t>(PEN_TYPE::CYAN)] = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	_pens[static_cast<size_t>(PEN_TYPE::MAGENTA)] = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	_pens[static_cast<size_t>(PEN_TYPE::YELLOW)] = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));

	_brushes[static_cast<size_t>(BRUSH_TYPE::BLACK)] = CreateSolidBrush(RGB(0, 0, 0));
	_brushes[static_cast<size_t>(BRUSH_TYPE::WHITE)] = CreateSolidBrush(RGB(255, 255, 255));
	_brushes[static_cast<size_t>(BRUSH_TYPE::GRAY)] = CreateSolidBrush(RGB(128, 128, 128));
	_brushes[static_cast<size_t>(BRUSH_TYPE::RED)] = CreateSolidBrush(RGB(255, 0, 0));
	_brushes[static_cast<size_t>(BRUSH_TYPE::GREEN)] = CreateSolidBrush(RGB(0, 255, 0));
	_brushes[static_cast<size_t>(BRUSH_TYPE::BLUE)] = CreateSolidBrush(RGB(0, 0, 255));
	_brushes[static_cast<size_t>(BRUSH_TYPE::CYAN)] = CreateSolidBrush(RGB(0, 255, 255));
	_brushes[static_cast<size_t>(BRUSH_TYPE::MAGENTA)] = CreateSolidBrush(RGB(255, 0, 255));
	_brushes[static_cast<size_t>(BRUSH_TYPE::YELLOW)] = CreateSolidBrush(RGB(255, 255, 0));
	_brushes[static_cast<size_t>(BRUSH_TYPE::HOLLOW)] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
}

void Window::Shutdown() noexcept {
	if (_hBackBitmap) {
		DeleteObject(_hBackBitmap);
		_hBackBitmap = nullptr;
	}
	if (_hBackDC) {
		DeleteDC(_hBackDC);
		_hBackDC = nullptr;
	}
	if (_hMainDC) {
		ReleaseDC(_hWindow, _hMainDC);
		_hMainDC = nullptr;
	}
	for (size_t i = 0; i < static_cast<size_t>(PEN_TYPE::END); ++i) {
		if (_pens[i] == NULL) continue;
		DeleteObject(_pens[i]);
		_pens[i] = NULL;
	}
	for (size_t i = 0; i < static_cast<size_t>(BRUSH_TYPE::END); ++i) {
		if (_brushes[i] == NULL) continue;
		DeleteObject(_brushes[i]);
		_brushes[i] = NULL;
	}
	if (_hWindow) {
		DestroyWindow(_hWindow);
		_hWindow = nullptr;
	}
	UnregisterClassW(L"App1WindowClass", _hInstance);
	_hInstance = nullptr;
}

void Window::WMPaintHandler() const noexcept {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(_hWindow, &ps);
	RECT clientRect;
	GetClientRect(_hWindow, &clientRect);
	BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, _hBackDC, 0, 0, SRCCOPY);
	EndPaint(_hWindow, &ps);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY: {
		PostQuitMessage(0);
	} break;
	case WM_PAINT: {
		Window::GetInstance().WMPaintHandler();
	} break;
	case WM_MOUSEMOVE: {
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
	} break;
	case WM_LBUTTONDOWN: {
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
	} break;
	case WM_LBUTTONUP: {
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
	} break;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	} // End switch (message) 
	return 0;
}

void Window::RenderGrid() const noexcept {
	HPEN oldPen = (HPEN)SelectObject(_hBackDC, GetPen(PEN_TYPE::GRAY));
	for (int x = 0; x <= GRID_WIDTH; ++x) {
		MoveToEx(_hBackDC, x * GRID_SIZE, 0, nullptr);
		LineTo(_hBackDC, x * GRID_SIZE, GRID_HEIGHT * GRID_SIZE);
	}
	for (int y = 0; y <= GRID_HEIGHT; ++y) {
		MoveToEx(_hBackDC, 0, y * GRID_SIZE, nullptr);
		LineTo(_hBackDC, GRID_WIDTH * GRID_SIZE, y * GRID_SIZE);
	}
}

void Window::Present() const noexcept {
	RECT clientRect;
	GetClientRect(_hWindow, &clientRect);
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;
	BitBlt(
		_hMainDC,
		0, 0,
		width,
		height,
		_hBackDC,
		0, 0,
		SRCCOPY
	);
}
