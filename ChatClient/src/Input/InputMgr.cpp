// InputMgr.cpp

#include "pch.h"
#include "InputMgr.h"

InputMgr::~InputMgr() {
	if (m_hEditLog) DestroyWindow(m_hEditLog);
	if (m_hEditInput) DestroyWindow(m_hEditInput);
	if (m_hButtonSend) DestroyWindow(m_hButtonSend);
}

void InputMgr::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	CreateUI();
}

void InputMgr::CreateUI()
{
	m_hEditLog = CreateWindowW(L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER |
		ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
		10, 10, 460, 300, m_hWnd, (HMENU)101, nullptr, nullptr);

	m_hEditInput = CreateWindowW(L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		10, 320, 360, 25, m_hWnd, (HMENU)102, nullptr, nullptr);

	m_hButtonSend = CreateWindowW(L"BUTTON", L"Send",
		WS_CHILD | WS_VISIBLE,
		380, 320, 90, 25, m_hWnd, (HMENU)103, nullptr, nullptr);
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
            wchar_t inputText[256];
            GetWindowTextW(m_hEditInput, inputText, 256);

            int len = GetWindowTextLengthW(m_hEditLog);
            SendMessageW(m_hEditLog, EM_SETSEL, len, len);
            SendMessageW(m_hEditLog, EM_REPLACESEL, FALSE, (LPARAM)inputText);
            SendMessageW(m_hEditLog, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");

            SetWindowTextW(m_hEditInput, L"");
            return 0;
        }
        else
        {
            return DefWindowProc(m_hWnd, msg, wParam, lParam);
        }
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(m_hWnd, msg, wParam, lParam);
    }

    return 0;
}
