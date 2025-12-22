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
	_wcex.lpszClassName = L"Binary Search Tree Viewer Class";
	_wcex.hIconSm = LoadIcon(_hInstance, IDI_APPLICATION);
	if (!RegisterClassEx(&_wcex)) return false;

	RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	_hWindow = CreateWindowW(
		L"Binary Search Tree Viewer Class",
		L"Binary Search Tree Viewer Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left, 
		rc.bottom - rc.top,
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
	_hBackBitmap = CreateCompatibleBitmap(_hMainDC, BITMAP_WIDTH, BITMAP_HEIGHT);	
	SelectObject(_hBackDC, _hBackBitmap);

	_hBtnInsert = CreateWindowW(L"BUTTON", L"Insert",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		130, 100, 70, 25, _hWindow, (HMENU)102, _hInstance, nullptr);

	_hBtnErase = CreateWindowW(L"BUTTON", L"Erase",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		210, 100, 70, 25, _hWindow, (HMENU)103, _hInstance, nullptr);

	CreateGDIObjects();
	ShowWindow(_hWindow, SW_SHOW);

	// Scroll(((BITMAP_WIDTH - WINDOW_WIDTH) / 2), 0);

	return true;
}

void Window::CreateGDIObjects() noexcept {
	_pens[static_cast<size_t>(PEN_TYPE::BLACK)] = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	_pens[static_cast<size_t>(PEN_TYPE::WHITE)] = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	_pens[static_cast<size_t>(PEN_TYPE::GRAY)] = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	_pens[static_cast<size_t>(PEN_TYPE::DARKGRAY)] = CreatePen(PS_SOLID, 1, RGB(32, 32, 32));
	_pens[static_cast<size_t>(PEN_TYPE::RED)] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	_pens[static_cast<size_t>(PEN_TYPE::GREEN)] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	_pens[static_cast<size_t>(PEN_TYPE::BLUE)] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	_pens[static_cast<size_t>(PEN_TYPE::CYAN)] = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	_pens[static_cast<size_t>(PEN_TYPE::MAGENTA)] = CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	_pens[static_cast<size_t>(PEN_TYPE::YELLOW)] = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));

	_brushes[static_cast<size_t>(BRUSH_TYPE::BLACK)] = CreateSolidBrush(RGB(0, 0, 0));
	_brushes[static_cast<size_t>(BRUSH_TYPE::WHITE)] = CreateSolidBrush(RGB(255, 255, 255));
	_brushes[static_cast<size_t>(BRUSH_TYPE::GRAY)] = CreateSolidBrush(RGB(128, 128, 128));
	_brushes[static_cast<size_t>(BRUSH_TYPE::DARKGRAY)] = CreateSolidBrush(RGB(32, 32, 32)); 
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
	for (size_t i = 0; i < static_cast<size_t>(BRUSH_TYPE::HOLLOW); ++i) {
		if (_brushes[i] == NULL) continue;
		DeleteObject(_brushes[i]);
		_brushes[i] = NULL;
	}
	if (_hWindow) {
		DestroyWindow(_hWindow);
		_hWindow = nullptr;
	}
	UnregisterClassW(L"Binary Search Tree Viewer Class", _hInstance);
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

void Window::RenderTree(const std::vector<lot_node<int>>& snapshot) const noexcept {
	RECT rc = { 0, 0, BITMAP_WIDTH, BITMAP_HEIGHT };
	FillRect(_hBackDC, &rc, _brushes[static_cast<size_t>(BRUSH_TYPE::DARKGRAY)]);

	if (snapshot.empty()) return;

	std::map<int, POINT> positions;  

	constexpr int NODE_RADIUS = 16;
	constexpr int LEVEL_HEIGHT = 48;
	constexpr int ROOT_Y = 32;

	const int total_nodes = static_cast<const int>(snapshot.size()); 

	auto next_pow_2 = [](int n) -> int {
		int p = 1;
		while (p < n) p <<= 1; 
		return p; 
	}; 

	int min_width = (total_nodes + 1) * NODE_RADIUS * 3;
	int tree_width = min(WINDOW_WIDTH, next_pow_2(min_width));

	for (const auto& node : snapshot) {
		int level = node.level;
		int idx = node.index;

		int level_count = 0;
		for (const auto& n : snapshot) {
			if (n.level == level) ++level_count;
		}

		int spacing = tree_width / (level_count + 1);
		int x = (tree_width * (idx + 1)) / (total_nodes + 1);
		int y = ROOT_Y + level * LEVEL_HEIGHT;

		positions[node.key] = { x, y };
	}

	HPEN oldPen = (HPEN)SelectObject(_hBackDC, _pens[static_cast<size_t>(PEN_TYPE::WHITE)]);
	for (const auto& node : snapshot) {
		if (!node.has_parent) continue;

		POINT child = positions[node.key];
		POINT parent = positions[node.key_parent];

		MoveToEx(_hBackDC, parent.x, parent.y + NODE_RADIUS, nullptr);
		LineTo(_hBackDC, child.x, child.y - NODE_RADIUS);
	}

	HBRUSH oldBrush = (HBRUSH)SelectObject(_hBackDC, _brushes[static_cast<size_t>(BRUSH_TYPE::GRAY)]);
	SetBkMode(_hBackDC, TRANSPARENT);

	for (const auto& node : snapshot) {
		POINT pos = positions[node.key];
		bool color_red = node.is_red; 

		if (node.is_red) {
			SelectObject(_hBackDC, _brushes[static_cast<size_t>(BRUSH_TYPE::RED)]);
		}
		else {
			SelectObject(_hBackDC, _brushes[static_cast<size_t>(BRUSH_TYPE::BLACK)]);
		}

		Ellipse(_hBackDC,
			pos.x - NODE_RADIUS, pos.y - NODE_RADIUS,
			pos.x + NODE_RADIUS, pos.y + NODE_RADIUS);

		wchar_t buf[8];
		swprintf_s(buf, L"%d", node.key);

		RECT textRect = {
			pos.x - NODE_RADIUS, pos.y - NODE_RADIUS,
			pos.x + NODE_RADIUS, pos.y + NODE_RADIUS
		};
		DrawTextW(_hBackDC, buf, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	SelectObject(_hBackDC, oldPen);
	SelectObject(_hBackDC, oldBrush); 
}

void Window::RenderUI() const noexcept {
	// Placeholder for UI rendering logic
	SetTextColor(_hBackDC, RGB(255, 255, 255));

	TextOutW(_hBackDC, 50 + _scrollX, 800, L"Q: Insert", 9);
	TextOutW(_hBackDC, 50 + _scrollX, 825, L"W: Erase ", 9);
	TextOutW(_hBackDC, 50 + _scrollX, 850, L"E: Insert 20 random", 19);
	TextOutW(_hBackDC, 50 + _scrollX, 875, L"R: Erase  20 random", 19);
}

void Window::Present() const noexcept {
	BitBlt(
		_hMainDC,
		0, 0,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		_hBackDC,
		_scrollX,  
		_scrollY, 
		SRCCOPY
	);
}