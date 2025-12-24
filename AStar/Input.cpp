// Input.cpp 

#include "stdafx.h"
#include "input.h" 

KeyInfo KeyState[KEY_COUNT];

int KeyData[KEY_COUNT] = {
	'0','1','2','3','4','5','6','7','8','9',
	'Q','W','E','R','T','Y','A','S','D','F','G','H','Z','X','C','V','B','N',
	VK_RETURN, VK_ESCAPE, VK_SPACE, VK_LEFT, VK_UP, VK_RIGHT, VK_DOWN,
	VK_LBUTTON, VK_RBUTTON
};

void InputCheck()
{
	for (size_t i = 0; i < KEY_COUNT; i++)
	{
		if (0x8000 & GetAsyncKeyState(KeyData[i]))
		{
			if (KeyState[i].pressed) KeyState[i].state = KEY_HOLD; // During Press
			else KeyState[i].state = KEY_TAPP; // Pressed Moment
			KeyState[i].pressed = true;
		}
		else {
			if (KeyState[i].pressed) KeyState[i].state = KEY_FREE; // Released Moment
			else KeyState[i].state = KEY_IDLE; // No Press
			KeyState[i].pressed = false;
		}
	}
}
