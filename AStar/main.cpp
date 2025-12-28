#include "stdafx.h"
#include "Level.h" 
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
    Level::Manager& level = Level::Manager::GetInstance();
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

        int camSpeed = 20;
        if (input.GetKeyState(Input::LEFT) == Input::HOLD) window.MoveCamera(-camSpeed, 0);
        if (input.GetKeyState(Input::RIGHT) == Input::HOLD) window.MoveCamera(camSpeed, 0);
        if (input.GetKeyState(Input::UP) == Input::HOLD) window.MoveCamera(0, -camSpeed);
        if (input.GetKeyState(Input::DOWN) == Input::HOLD) window.MoveCamera(0, camSpeed); 

        if (input.GetKeyState(Input::PLUS) == Input::TAPP) window.ZoomIn();
        if (input.GetKeyState(Input::MINUS) == Input::TAPP) window.ZoomOut();

        if (input.GetKeyState(Input::ENTER) == Input::TAPP) {
            level.NextState();
        }
        if (input.GetKeyState(Input::BACKSPACE) == Input::TAPP) {
            level.PrevState();
        }

        POINT gCurr = window.ScreenToGrid(input.GetMousePosCurr());
        POINT gPrev = window.ScreenToGrid(input.GetMousePosPrev());

        switch (level.GetState()) {
        case Level::State::DRAW_WALL:
            if (input.GetKeyState(Input::LBTN) == Input::HOLD ||
                input.GetKeyState(Input::RBTN) == Input::HOLD) {
                int x1 = gPrev.x; int y1 = gPrev.y;
                int x2 = gCurr.x; int y2 = gCurr.y;
                int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
                int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
                int err = dx + dy, e2;
                bool setWall = (input.GetKeyState(Input::LBTN) == Input::HOLD);
                while (true) {
                    if (x1 >= 0 && x1 < Level::GRID_WIDTH && y1 >= 0 && y1 < Level::GRID_HEIGHT) {
                        Level::wall[x1][y1] = setWall;
                    }
                    if (x1 == x2 && y1 == y2) break;
                    e2 = 2 * err;
                    if (e2 >= dy) { err += dy; x1 += sx; }
                    if (e2 <= dx) { err += dx; y1 += sy; }
                }
            }
            break;

        case Level::State::SET_POINTS:
            if (input.GetKeyState(Input::LBTN) == Input::TAPP) {
                if (gCurr.x >= 0 && gCurr.x < Level::GRID_WIDTH &&
                    gCurr.y >= 0 && gCurr.y < Level::GRID_HEIGHT &&
                    !Level::wall[gCurr.x][gCurr.y]) {
                    level.SetStartPos({ gCurr.x, gCurr.y });
                }
            }
            if (input.GetKeyState(Input::RBTN) == Input::TAPP) {
                if (gCurr.x >= 0 && gCurr.x < Level::GRID_WIDTH &&
                    gCurr.y >= 0 && gCurr.y < Level::GRID_HEIGHT &&
                    !Level::wall[gCurr.x][gCurr.y]) {
                    level.SetEndPos({ gCurr.x, gCurr.y });
                }
            }
            break;

        case Level::State::RUN_ASTAR:
            if (input.GetKeyState(Input::SPACE) == Input::HOLD) {
                level.Step();
            }
            break;
        }

        window.Render();
        window.Present();
        Sleep(15);
    }
    window.Shutdown();
    return 0;
}