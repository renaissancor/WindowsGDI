// Engine.h 

#pragma once

#include "Singleton.h" 
#include <wtypes.h>

class Engine : public Singleton<Engine>
{
	friend class Singleton<Engine>; // private access to Singleton 

private:
	HINSTANCE	m_hInst;		// Process Instance 
	HWND		m_hMainWnd;		// Main Window Instance 
	POINT		m_Resolution;	// Windows Pixels (x y pixels total number)

	HDC			m_MainDC;		// DeviceContext, in charge of rendering in main Window 

	//HPEN		m_Pen[6];
	//HBRUSH		m_Brush[6];

private:
	bool m_Initialized = false;

public:
	void Init(HINSTANCE _Inst, HWND _hWnd, UINT _Width, UINT _Height);

	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const;
public:
	Engine();
	~Engine();
};