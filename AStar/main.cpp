#include "stdafx.h"
#include "Grid.h" 
#include "Input.h"
#include "Window.h"

// main.cpp 

/*
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
*/

int main() {
    Input::Manager& input = Input::Manager::GetInstance();
    Window& window = Window::GetInstance();
    window.Initialize();
	input.Initialize(window.GetHWND()); 

    for (;;) {
        MSG msg;
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
			continue;
        }
        input.Update();

        if (input.GetKeyState(Input::ESCAPE) == Input::TAPP) break;

        int camSpeed = 10;
        if (input.GetKeyState(Input::LEFT) == Input::HOLD) window.MoveCamera(-camSpeed, 0);
        if (input.GetKeyState(Input::RIGHT) == Input::HOLD) window.MoveCamera(camSpeed, 0);
        if (input.GetKeyState(Input::UP) == Input::HOLD) window.MoveCamera(0, -camSpeed);
        if (input.GetKeyState(Input::DOWN) == Input::HOLD) window.MoveCamera(0, camSpeed);
        
        POINT gCurr = window.ScreenToGrid(input.GetMousePosCurr());
        POINT gPrev = window.ScreenToGrid(input.GetMousePosPrev()); 

        if (input.GetKeyState(Input::S) == Input::HOLD && input.GetKeyState(Input::LBTN) == Input::TAPP) {
            Grid::Manager::GetInstance().SetStartPos({ gCurr.x, gCurr.y });
        }
        else if (input.GetKeyState(Input::E) == Input::HOLD && input.GetKeyState(Input::LBTN) == Input::TAPP) {
            Grid::Manager::GetInstance().SetEndPos({ gCurr.x, gCurr.y });
        }
        else if (input.GetKeyState(Input::LBTN) == Input::HOLD || input.GetKeyState(Input::RBTN) == Input::HOLD) {
            int x1 = gPrev.x; int y1 = gPrev.y;
            int x2 = gCurr.x; int y2 = gCurr.y;
            int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
            int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
            int err = dx + dy, e2;
            bool setWall = (input.GetKeyState(Input::LBTN) == Input::HOLD);
            while (true) {
                if (x1 >= 0 && x1 < Grid::GRID_WIDTH && y1 >= 0 && y1 < Grid::GRID_HEIGHT) {
                    Grid::wall[x1][y1] = setWall;
                }
                if (x1 == x2 && y1 == y2) break;
                e2 = 2 * err;
                if (e2 >= dy) { err += dy; x1 += sx; }
                if (e2 <= dx) { err += dx; y1 += sy; }
            }
        }
    
        if (input.GetKeyState(Input::SPACE) == Input::TAPP) {
            Grid::Manager::GetInstance().StepAStar();
        }
        if (input.GetKeyState(Input::ENTER) == Input::TAPP) {
            Grid::Manager::GetInstance().Clear(); // 초기화 후 탐색 준비
            // Grid::Manager::GetInstance().Init(); // (필요 시 구현)
        }

        window.Render();
        window.Present();
        Sleep(15);
    }

    window.Shutdown();
    return 0;
}
