#pragma once

// Window.h 
#include "level_order_traversal.h"

enum class PEN_TYPE
{
	BLACK,
	WHITE,
	GRAY,
	DARKGRAY, 
	RED,
	GREEN,
	BLUE,
	CYAN,
	MAGENTA,
	YELLOW,
	END,
};

enum class BRUSH_TYPE
{
	BLACK,
	WHITE,
	GRAY,
	DARKGRAY,
	RED,
	GREEN,
	BLUE,
	CYAN,
	MAGENTA,
	YELLOW,
	HOLLOW,
	END,
};

class Window {
private:
	constexpr static int BITMAP_WIDTH  = 2560; 
	constexpr static int BITMAP_HEIGHT = 1440;

	constexpr static int WINDOW_WIDTH  = 1920;
	constexpr static int WINDOW_HEIGHT = 1080;

	Window() = default;
	~Window() = default;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	HINSTANCE _hInstance = nullptr;
	HWND _hWindow = nullptr;
	HWND _hBtnInsert = nullptr; 
	HWND _hBtnErase = nullptr; 

	WNDCLASSEX _wcex = { 0 };

	HDC _hMainDC = nullptr;
	HDC _hBackDC = nullptr;
	HBITMAP _hBackBitmap = nullptr;

	HPEN _pens[static_cast<size_t>(PEN_TYPE::END)] = { NULL };
	HBRUSH _brushes[static_cast<size_t>(BRUSH_TYPE::END)] = { NULL };

	mutable int _scrollX = 0; 
	mutable int _scrollY = 0; 

public:
	inline static Window& GetInstance() noexcept {
		static Window instance;
		return instance;
	}

	inline HDC GetMainDC() const noexcept { return _hMainDC; }
	inline HDC GetBackDC() const noexcept { return _hBackDC; }
	inline HWND GetHWND() const noexcept { return _hWindow; }

	inline HPEN GetPen(PEN_TYPE type) const noexcept
	{
		return _pens[static_cast<size_t>(type)];
	}
	inline HBRUSH GetBrush(BRUSH_TYPE type) const noexcept
	{
		return _brushes[static_cast<size_t>(type)];
	}
	inline void Scroll(int dx, int dy) {
		_scrollX += dx;
		_scrollY += dy;
		_scrollX = max(0, min(_scrollX, BITMAP_WIDTH - WINDOW_WIDTH));
		_scrollY = max(0, min(_scrollY, BITMAP_HEIGHT - WINDOW_HEIGHT));
	}
	inline void SetScroll(int x, int y) noexcept {
		_scrollX = max(0, min(x, BITMAP_WIDTH - WINDOW_WIDTH));
		_scrollY = max(0, min(y, BITMAP_HEIGHT - WINDOW_HEIGHT));
	}

private:
	void CreateGDIObjects() noexcept;

public:
	bool Initialize() noexcept;
	void Shutdown() noexcept;

	void RenderTree(const std::vector<lot_node<int>>& snapshot) const noexcept;
	void RenderUI() const noexcept; 
	void Present() const noexcept;

	void WMPaintHandler() const noexcept;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
