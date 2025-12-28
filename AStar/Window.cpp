#include "stdafx.h"
#include "Window.h"
#include "Level.h"

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
		_windowWidth,
		_windowHeight, 
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
	_pens[static_cast<size_t>(PEN_TYPE::LIGHTGRAY)] = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
	_pens[static_cast<size_t>(PEN_TYPE::DARKGRAY)] = CreatePen(PS_SOLID, 1, RGB(64, 64, 64));
	_pens[static_cast<size_t>(PEN_TYPE::RED)] = CreatePen(PS_SOLID, 1, RGB(192, 0, 0));
	_pens[static_cast<size_t>(PEN_TYPE::GREEN)] = CreatePen(PS_SOLID, 1, RGB(0, 192, 0));
	_pens[static_cast<size_t>(PEN_TYPE::BLUE)] = CreatePen(PS_SOLID, 1, RGB(0, 0, 192));
	_pens[static_cast<size_t>(PEN_TYPE::CYAN)] = CreatePen(PS_SOLID, 1, RGB(0, 128, 128));
	_pens[static_cast<size_t>(PEN_TYPE::MAGENTA)] = CreatePen(PS_SOLID, 1, RGB(128, 0, 128));
	_pens[static_cast<size_t>(PEN_TYPE::YELLOW)] = CreatePen(PS_SOLID, 1, RGB(128, 128, 0));

	_brushes[static_cast<size_t>(BRUSH_TYPE::BLACK)] = CreateSolidBrush(RGB(0, 0, 0));
	_brushes[static_cast<size_t>(BRUSH_TYPE::WHITE)] = CreateSolidBrush(RGB(255, 255, 255));
	_brushes[static_cast<size_t>(BRUSH_TYPE::GRAY)] = CreateSolidBrush(RGB(128, 128, 128));
	_brushes[static_cast<size_t>(BRUSH_TYPE::LIGHTGRAY)] = CreateSolidBrush(RGB(192, 192, 192));
	_brushes[static_cast<size_t>(BRUSH_TYPE::DARKGRAY)] = CreateSolidBrush(RGB(64, 64, 64));
	_brushes[static_cast<size_t>(BRUSH_TYPE::RED)] = CreateSolidBrush(RGB(192, 0, 0));
	_brushes[static_cast<size_t>(BRUSH_TYPE::GREEN)] = CreateSolidBrush(RGB(0, 192, 0));
	_brushes[static_cast<size_t>(BRUSH_TYPE::BLUE)] = CreateSolidBrush(RGB(0, 0, 192));
	_brushes[static_cast<size_t>(BRUSH_TYPE::CYAN)] = CreateSolidBrush(RGB(0, 128, 128));
	_brushes[static_cast<size_t>(BRUSH_TYPE::MAGENTA)] = CreateSolidBrush(RGB(128, 0, 128));
	_brushes[static_cast<size_t>(BRUSH_TYPE::YELLOW)] = CreateSolidBrush(RGB(128, 128, 0));
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

void Window::Render() const noexcept {
	RECT clientRect;
	GetClientRect(_hWindow, &clientRect);
	FillRect(_hBackDC, &clientRect, _brushes[static_cast<size_t>(BRUSH_TYPE::DARKGRAY)]);

	const int gWidth = Level::GRID_WIDTH;
	const int gHeight = Level::GRID_HEIGHT;
	const int gSize = _cameraZoom;
	Level::Manager& level = Level::Manager::GetInstance();  // 루프 밖에서 한 번만
	Level::Pos start = level.GetStartPos();
	Level::Pos end = level.GetEndPos();

	// 1. 그리드 내부 노드 렌더링
	for (int x = 0; x < gWidth; ++x) {
		for (int y = 0; y < gHeight; ++y) {
			RECT cell = {
				x * gSize - _cameraX,
				y * gSize - _cameraY,
				(x + 1) * gSize - _cameraX,
				(y + 1) * gSize - _cameraY
			};

			if (cell.right < 0 || cell.left > clientRect.right ||
				cell.bottom < 0 || cell.top > clientRect.bottom) continue;

			// 벽
			if (Level::wall[x][y]) {
				FillRect(_hBackDC, &cell, _brushes[static_cast<size_t>(BRUSH_TYPE::BLACK)]);
			}
			// Closed 노드
			else if (level.IsClosed(x, y)) {
				FillRect(_hBackDC, &cell, _brushes[static_cast<size_t>(BRUSH_TYPE::BLUE)]);
			}
			// Open 노드
			else if (level.GetNode(x, y) != nullptr) {
				FillRect(_hBackDC, &cell, _brushes[static_cast<size_t>(BRUSH_TYPE::MAGENTA)]);
			}

			// 시작점/도착점 (항상 위에)
			if (x == start.x && y == start.y)
				FillRect(_hBackDC, &cell, _brushes[static_cast<size_t>(BRUSH_TYPE::GREEN)]);
			if (x == end.x && y == end.y)
				FillRect(_hBackDC, &cell, _brushes[static_cast<size_t>(BRUSH_TYPE::RED)]);

			// f, g, h 표시 (확대 시)
			if (gSize >= 48) {
				const Level::Node* node = level.GetNode(x, y);
				if (node != nullptr) {
					SetTextColor(_hBackDC, RGB(96, 96, 0)); // Dark Yellow 
					WCHAR buf[16];
					swprintf_s(buf, L"g:%.1f", node->g);
					TextOutW(_hBackDC, cell.left + 2, cell.top + 2, buf, (int)wcslen(buf));
					swprintf_s(buf, L"h:%.1f", node->h);
					TextOutW(_hBackDC, cell.left + 2, cell.top + 16, buf, (int)wcslen(buf));
					swprintf_s(buf, L"f:%.1f", node->f);
					TextOutW(_hBackDC, cell.left + 2, cell.top + 30, buf, (int)wcslen(buf));
				}
			}
		}
	}

	// 2. Grid Lines Rendering 
	HPEN oldPen = (HPEN)SelectObject(_hBackDC, GetPen(PEN_TYPE::BLACK));
	for (int x = 0; x <= gWidth; ++x) {
		int posX = x * gSize - _cameraX;
		if (posX < 0 || posX > clientRect.right) continue;
		MoveToEx(_hBackDC, posX, 0 - _cameraY, nullptr);
		LineTo(_hBackDC, posX, gHeight * gSize - _cameraY);
	}
	for (int y = 0; y <= gHeight; ++y) {
		int posY = y * gSize - _cameraY;
		if (posY < 0 || posY > clientRect.bottom) continue;
		MoveToEx(_hBackDC, 0 - _cameraX, posY, nullptr);
		LineTo(_hBackDC, gWidth * gSize - _cameraX, posY);
	}

	// 3. Path Rendering by line 

	for (int i = 0; i < gWidth * gHeight; ++i) {
		const Level::Node* node = level.GetNode(i % gWidth, i / gWidth);
		if (node == nullptr) continue;
		if (node->parent.x == -1 && node->parent.y == -1) continue;
		int x1 = node->pos.x * gSize + gSize / 2 - _cameraX;
		int y1 = node->pos.y * gSize + gSize / 2 - _cameraY;
		int x2 = node->parent.x * gSize + gSize / 2 - _cameraX;
		int y2 = node->parent.y * gSize + gSize / 2 - _cameraY;
		MoveToEx(_hBackDC, x1, y1, nullptr);
		LineTo(_hBackDC, x2, y2);
	}

	// 4. final Path Rendering if game ended 
	if (level.GetState() == Level::State::SHOW_RESULT) {
		SelectObject(_hBackDC, GetPen(PEN_TYPE::WHITE)); 
		Level::Pos currPos = end;
		while (true) {
			const Level::Node* currNode = level.GetNode(currPos.x, currPos.y);
			if (currNode == nullptr) break;
			if (currNode->parent.x == -1 && currNode->parent.y == -1) break;
			int x1 = currNode->pos.x * gSize + gSize / 2 - _cameraX;
			int y1 = currNode->pos.y * gSize + gSize / 2 - _cameraY;
			int x2 = currNode->parent.x * gSize + gSize / 2 - _cameraX;
			int y2 = currNode->parent.y * gSize + gSize / 2 - _cameraY;
			MoveToEx(_hBackDC, x1, y1, nullptr);
			LineTo(_hBackDC, x2, y2);
			currPos = currNode->parent;
		}
		SelectObject(_hBackDC, GetPen(PEN_TYPE::BLACK));
	}
	
	

	// 5. UI Text 
	SetBkMode(_hBackDC, TRANSPARENT);
	SetTextColor(_hBackDC, RGB(255, 255, 255));

	WCHAR buf[128];
	swprintf_s(buf, L"Zoom: %d | Start: (%d, %d) | End: (%d, %d)",
		gSize, start.x, start.y, end.x, end.y);
	TextOutW(_hBackDC, 10, 10, buf, (int)wcslen(buf));

	const wchar_t* stateText[] = {
		L"[1/4] DRAW WALL - LClick:Wall, RClick:Erase, Enter:Next",
		L"[2/4] SET POINTS - LClick:Start, RClick:End, Enter:Next",
		L"[3/4] RUN A* - Space:Step, Backspace:Back",
		L"[4/4] RESULT - Backspace:Back"
	};
	Level::State currState = level.GetState();
	TextOutW(_hBackDC, 10, 30, stateText[static_cast<size_t>(currState)],
		(int)wcslen(stateText[static_cast<size_t>(currState)]));

	SelectObject(_hBackDC, oldPen);
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
