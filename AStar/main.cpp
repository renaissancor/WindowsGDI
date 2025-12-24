#include "stdafx.h"
#include "Window.h"

// main.cpp 

class Engine {
private:
	Engine() = default;
	~Engine() = default;
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
public:
	static Engine& GetInstance() noexcept {
		static Engine instance;
		return instance;
	}
};

int main() {

	Window& window = Window::GetInstance();
	window.Initialize();

	while (true) {
		MSG msg;
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			window.RenderGrid();
			window.Present();
			Sleep(16); // Approximate ~60 FPS 
		}
	}

	window.Shutdown();

	return 0;
}

