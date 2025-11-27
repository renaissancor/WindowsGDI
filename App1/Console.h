#pragma once

class Console {
private:
    Console() { Initialize(); }
    ~Console() = default;
    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;

    HWND _consoleWindow = NULL;
    HANDLE _stdoutHandle = INVALID_HANDLE_VALUE;
    HANDLE _stderrHandle = INVALID_HANDLE_VALUE;

    bool Initialize() {
        _consoleWindow = GetConsoleWindow();
        if (_consoleWindow == NULL) return false;
        _stderrHandle = GetStdHandle(STD_ERROR_HANDLE);
        if (_stderrHandle == INVALID_HANDLE_VALUE) return false;
        _stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (_stdoutHandle == INVALID_HANDLE_VALUE) return false;
        return true;
    }

public:
    inline static Console& GetInstance() {
        static Console instance;
        return instance;
    }
    inline void HideConsole() const { ShowWindow(_consoleWindow, SW_HIDE); }
    inline void ShowConsole() const { ShowWindow(_consoleWindow, SW_SHOW); }
};
