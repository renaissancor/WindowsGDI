// InputMgr.h
#pragma once

#include "Singleton.h"
#include <wtypes.h>

class InputMgr : public Singleton<InputMgr>
{
	friend class Singleton<InputMgr>;

private:
	HWND m_hWnd = nullptr;
	HWND m_hEditLog = nullptr;
	HWND m_hEditInput = nullptr;
	HWND m_hButtonSend = nullptr;

public:
	~InputMgr(); 
	void Init(HWND hWnd, UINT width, UINT height);
	void CreateUI(UINT _Width, UINT _Height); 
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void UpdateLogPanel(); 


};