#include "stdafx.h"
#include "Input.h"

namespace Input {
    Manager::Manager() {
        int temp[] = { '0','1','2','3','4','5','6','7','8','9',
                       'Q','W','E','R','T','Y','A','S','D','F','G','H','Z','X','C','V','B','N',
                       VK_RETURN, VK_ESCAPE, VK_SPACE, VK_LEFT, VK_UP, VK_RIGHT, VK_DOWN,
                       VK_LBUTTON, VK_RBUTTON, VK_BACK, VK_OEM_PLUS, VK_OEM_MINUS };
        for (int i = 0; i < KEY_COUNT; ++i) {
            _keyData[i] = temp[i];
        }
    }

    Manager::~Manager() {
	}

    void Manager::Update() {
        _mousePosPrev = _mousePosCurr;

		POINT point; 
		GetCursorPos(&point); 
		if (_hWindow) ScreenToClient(_hWindow, &point); 
		_mousePosCurr = point; 

        for (int i = 0; i < KEY_COUNT; i++) {
            if (0x8000 & GetAsyncKeyState(_keyData[i])) {
                if (_keyState[i].pressed) _keyState[i].state = HOLD;
                else _keyState[i].state = TAPP;
                _keyState[i].pressed = true;
            }
            else {
                if (_keyState[i].pressed) _keyState[i].state = FREE;
                else _keyState[i].state = IDLE;
                _keyState[i].pressed = false;
            }
        }
    }
}