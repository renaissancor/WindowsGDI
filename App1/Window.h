#pragma once

// Window.h 

enum class PEN_TYPE
{
	BLACK,
	WHITE,
	GRAY,
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
	constexpr static const int GRID_SIZE = 16;
	constexpr static const int GRID_WIDTH = 100;
	constexpr static const int GRID_HEIGHT = 50;

	Window() = default;
	~Window() = default;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	HINSTANCE _hInstance = nullptr;
	HWND _hWindow = nullptr;
	WNDCLASSEX _wcex = { 0 };

	HDC _hMainDC = nullptr;
	HDC _hBackDC = nullptr;
	HBITMAP _hBackBitmap = nullptr;

	HPEN _pens[static_cast<size_t>(PEN_TYPE::END)] = { NULL };
	HBRUSH _brushes[static_cast<size_t>(BRUSH_TYPE::END)] = { NULL };
public:
	inline static Window& GetInstance() noexcept {
		static Window instance;
		return instance;
	}

	inline HDC GetMainDC() const noexcept { return _hMainDC; }
	inline HDC GetBackDC() const noexcept { return _hBackDC; }
	inline HWND GetHWND() const noexcept { return _hWindow; } 

	inline HPEN GetPen(PEN_TYPE type) const noexcept 
	{ return _pens[static_cast<size_t>(type)]; }
	inline HBRUSH GetBrush(BRUSH_TYPE type) const noexcept 
	{ return _brushes[static_cast<size_t>(type)]; }

private:
	void CreateGDIObjects() noexcept;

public:
	bool Initialize() noexcept; 
	void Shutdown() noexcept; 

	void RenderGrid() const noexcept; 
	void Present() const noexcept; 


	void WMPaintHandler() const noexcept; 
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
