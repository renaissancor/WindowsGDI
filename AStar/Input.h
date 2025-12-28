#pragma once

namespace Input {
    enum KEY {
        _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
        Q, W, E, R, T, Y, A, S, D, F, G, H, Z, X, C, V, B, N,
        ENTER, ESCAPE, SPACE, LEFT, UP, RIGHT, DOWN,
        LBTN, RBTN, BACKSPACE, 
		PLUS, MINUS, 
        KEY_COUNT
    };

    enum KEY_STATE { IDLE, TAPP, HOLD, FREE };

    struct KeyInfo {
        KEY_STATE state = IDLE;
        bool pressed = false;
    };

    class Manager {
    private:
        Manager();
        ~Manager();
        Manager(const Manager&) = delete;
        Manager& operator=(const Manager&) = delete;

		HWND _hWindow = nullptr; 

    public:
        static Manager& GetInstance() {
            static Manager instance;
            return instance;
        }
        void Initialize(HWND hWnd) noexcept { _hWindow = hWnd; }
        void Update();

        inline KEY_STATE GetKeyState(KEY key) const { return _keyState[key].state; }
        inline POINT GetMousePosCurr() const { return _mousePosCurr; }
		inline POINT GetMousePosPrev() const { return _mousePosPrev; } 

    private:
        KeyInfo _keyState[KEY_COUNT];
        int     _keyData[KEY_COUNT];
        POINT   _mousePosCurr = { 0, 0 };
        POINT   _mousePosPrev = { 0, 0 };
    };
}
