// WinMain.cpp 

#include "pch.h" 
#include "WinMain.h"

#include "Core/Engine.h"
#include "Input/InputMgr.h" 

HINSTANCE hInst;                                // Global instance, current instance now 

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    hInst = hInstance; // <-- This initializes the global instance handle

    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);                    // Size of the structure.
    wcex.style = CS_HREDRAW | CS_VREDRAW;                // Redraw on horizontal/vertical resize.
    wcex.lpfnWndProc = WndProc;                          // Pointer to the window procedure.
    wcex.cbClsExtra = 0;                                 // No extra bytes for the class.
    wcex.cbWndExtra = 0;                                 // No extra bytes for the window instance. 
    wcex.hInstance = hInstance;                          // Handle to the application instance.
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHATCLIENT)); // Main window icon.
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);       // Default arrow cursor.
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);     // Default window background color.
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_CHATCLIENT); // Menu resource name.
    wcex.lpszClassName = L"WindowKey";                   // Class name used to create the window.
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // Small icon for taskbar.
    RegisterClassExW(&wcex);

    // Create Window 
    HWND hWnd = CreateWindowW(L"WindowKey", L"WindowTitle", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1080, 720, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        MessageBoxW(nullptr, L"Failed to create window", L"Error", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hWnd, true);
    UpdateWindow(hWnd);

	Engine::Get()->Init(hInstance, hWnd, 1080, 720); // Initialize the engine with the instance and window handle. 

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHATCLIENT)); // Shortcuts 

    MSG msg = {};

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            //Engine::Get()->Progress();
        }
    }

    return (int)msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // return DefWindowProc(hWnd, message, wParam, lParam);
    return Engine::Get()->HandleMessage(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
