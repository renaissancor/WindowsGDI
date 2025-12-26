#pragma once

// Window.h 

enum class PEN_TYPE
{
	BLACK,
	WHITE,
	GRAY,
	LIGHTGRAY,
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
	LIGHTGRAY,
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
	constexpr static int BITMAP_WIDTH = 2560;
	constexpr static int BITMAP_HEIGHT = 1440;
	constexpr static int SIDE_BAR_WIDTH = 200;

	int _windowWidth = 1600;
	int _windowHeight = 900;

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

	int _cameraX = 0;
	int _cameraY = 0;
	int _cameraZoom = 16; // pixels per grid cell 

public:
	inline static Window& GetInstance() noexcept {
		static Window instance;
		return instance;
	}

	inline void MoveCamera(int dx, int dy) noexcept {
		_cameraX += dx;
		_cameraY += dy;
	}
	POINT ScreenToGrid(const POINT& screenPos) const noexcept {
		return {
			(screenPos.x + _cameraX) / _cameraZoom,
			(screenPos.y + _cameraY) / _cameraZoom
		};
	}

	inline HDC GetMainDC() const noexcept { return _hMainDC; }
	inline HDC GetBackDC() const noexcept { return _hBackDC; }
	inline HWND GetHWND() const noexcept { return _hWindow; }
	inline int GetCamX() const noexcept { return _cameraX; }
	inline int GetCamY() const noexcept { return _cameraY; }
	inline int GetZoom() const noexcept { return _cameraZoom; }

	inline HPEN GetPen(PEN_TYPE type) const noexcept
	{
		return _pens[static_cast<size_t>(type)];
	}
	inline HBRUSH GetBrush(BRUSH_TYPE type) const noexcept
	{
		return _brushes[static_cast<size_t>(type)];
	}

private:
	void CreateGDIObjects() noexcept;

public:
	bool Initialize() noexcept;
	void Shutdown() noexcept;

	void Render() const noexcept;
	void Present() const noexcept;


	void WMPaintHandler() const noexcept;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
