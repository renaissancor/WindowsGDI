// Engine.cpp 

#pragma once 

#include "pch.h"
#include "Singleton.h"
#include "Engine.h" 

#include "Input/InputMgr.h"

Engine::Engine()
	: m_hInst(nullptr)
	, m_hMainWnd(nullptr)
	, m_Resolution{}
	, m_MainDC(nullptr)
{}


Engine::~Engine()
{
	ReleaseDC(m_hMainWnd, m_MainDC);
}


void Engine::Init(HINSTANCE _Inst, HWND _hWnd, UINT _Width, UINT _Height)
{
	m_hInst = _Inst;
	m_hMainWnd = _hWnd;
	m_Resolution = POINT{ (LONG)_Width , (LONG)_Height };

	SetWindowPos(m_hMainWnd, nullptr, 10, 10, m_Resolution.x, m_Resolution.y, 0);

	m_MainDC = GetDC(m_hMainWnd);

	InputMgr::Get()->Init(m_hMainWnd, m_Resolution.x, m_Resolution.y);
	
	m_Initialized = true;
}


LRESULT Engine::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) const
{
	if (!m_Initialized)
		return DefWindowProc(hWnd, msg, wParam, lParam);

	return InputMgr::Get()->HandleMessage(hWnd, msg, wParam, lParam);
}

void Engine::Progress()
{
	// Update the chat log panel
	InputMgr::Get()->UpdateLogPanel();
}
