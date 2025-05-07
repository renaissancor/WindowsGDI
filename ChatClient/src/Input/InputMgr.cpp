// InputMgr.cpp

#include "pch.h"
#include "InputMgr.h"
#include "Core/ChatMgr.h"


static LRESULT CALLBACK EditInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static WNDPROC g_OldEditInputProc = nullptr;
static InputMgr* g_InputMgrInstance = nullptr; // for accessing from static callback


InputMgr::~InputMgr() {
	if (m_hEditLog) DestroyWindow(m_hEditLog);
	if (m_hEditInput) DestroyWindow(m_hEditInput);
	if (m_hButtonSend) DestroyWindow(m_hButtonSend);
}

void InputMgr::Init(HWND hWnd, UINT width, UINT height)
{
    m_hWnd = hWnd;
    CreateUI(width, height);
}


void InputMgr::CreateUI(UINT _Width, UINT _Height)
{
    const int padding = 10;
    const int logHeight = _Height * 8 / 10;
	const int logWidth = _Width - padding * 4; 
    const int inputHeight = 25;
    const int inputTop = (_Height * 8 / 10 )+ padding * 5; // 10 + 300 + 10 = 320
    const int inputWidth = _Width * 7 / 10;
    const int buttonWidth = _Width * 2 / 10;

    m_hEditLog = CreateWindowW(L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER |
        ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        padding, padding*3, logWidth, logHeight,
        m_hWnd, (HMENU)101, nullptr, nullptr);

    m_hEditInput = CreateWindowW(L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        padding, inputTop, inputWidth, inputHeight,
        m_hWnd, (HMENU)102, nullptr, nullptr);

    g_InputMgrInstance = this; // Save instance pointer
    g_OldEditInputProc = (WNDPROC)SetWindowLongPtr(m_hEditInput, GWLP_WNDPROC, (LONG_PTR)EditInputProc);


    m_hButtonSend = CreateWindowW(L"BUTTON", L"Send",
        WS_CHILD | WS_VISIBLE,
        padding * 2 + inputWidth, inputTop, buttonWidth, inputHeight,
        m_hWnd, (HMENU)103, nullptr, nullptr);
}


static LRESULT CALLBACK EditInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_KEYDOWN && wParam == VK_RETURN)
    {
        wchar_t inputText[256];
        GetWindowTextW(hWnd, inputText, 256);

        ChatMgr::Get()->AddMessage(inputText);
        SetWindowTextW(hWnd, L"");

        return 0; // Suppress default beep
    }

    // Call original proc for default handling
    return CallWindowProc(g_OldEditInputProc, hWnd, message, wParam, lParam);
}



LRESULT InputMgr::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        if (wmId == 103) // IDC_BUTTON_SEND
        {
            // Store input into the buffer 
            wchar_t inputText[256];
            GetWindowTextW(m_hEditInput, inputText, 256);

            ChatMgr::Get()->AddMessage(inputText);

            // Append to UI Log panel 
            /*int len = GetWindowTextLengthW(m_hEditLog);
            SendMessageW(m_hEditLog, EM_SETSEL, len, len);
            SendMessageW(m_hEditLog, EM_REPLACESEL, FALSE, (LPARAM)inputText);
            SendMessageW(m_hEditLog, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");*/

            // Clear Input Field 
            SetWindowTextW(m_hEditInput, L"");
            return 0;
        }
        else
        {
            return DefWindowProc(m_hWnd, msg, wParam, lParam);
        }
    }

    case WM_KEYDOWN:
    {
        if (GetFocus() == m_hEditInput && wParam == VK_RETURN)
        {
            // Simulate "Send" button action
            wchar_t inputText[256];
            GetWindowTextW(m_hEditInput, inputText, 256);

            ChatMgr::Get()->AddMessage(inputText);

            // Clear Input Field
            SetWindowTextW(m_hEditInput, L"");

            return 0;
        }
        break;
    }


    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(m_hWnd, msg, wParam, lParam);
    }

    return 0;
}


void InputMgr::UpdateLogPanel()
{
    const auto& history = ChatMgr::Get()->GetHistory();

    static size_t lastMsgIndex = 0;

    // Append only new messages
    for (size_t i = lastMsgIndex; i < history.size(); ++i)
    {
        const std::wstring& msg = history[i];

        // Move cursor to the end
        int len = GetWindowTextLengthW(m_hEditLog);
        SendMessageW(m_hEditLog, EM_SETSEL, len, len);

        // Append message + newline
        std::wstring msgWithNewline = msg + L"\r\n";
        SendMessageW(m_hEditLog, EM_REPLACESEL, FALSE, (LPARAM)msgWithNewline.c_str());
    }

    lastMsgIndex = history.size();
}


